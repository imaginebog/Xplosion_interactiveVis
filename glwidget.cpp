#include <cmath>
#include "glwidget.h"
#include <QMouseEvent>
#include <QKeyEvent>

#define MOUSE_DOWN 1
#define MOUSE_UP 2


extern "C" void cudaInit(int argc, char **argv);
extern "C" void cudaGLInit(int argc, char **argv);
extern "C" void copyArrayFromDevice(void *host, const void *device,
        unsigned int vbo, int size);

void GLWidget::initConstants()
{
    notFlushed=true;
    buttonState = 0;
    inertia = 0.1f;
    displayMode = ParticleRenderer::PARTICLE_FLAT_SPHERES;
    mode = 0;
    playMode = false;
    displayEnabled = true;
    bPause = false;
    displaySliders = false;
    demoMode = false;

    idleCounter = 0;
    demoCounter = 0;
    playCounter = 0;
    idleDelay = 2000;
    numParticles = 0;
    timestep = 0.5f;
    rangeColor = 200;
    psystem = 0;
    fpsCount = 0;
    fpsLimit = 1;

    renderer = 0;
    obj_drawmode = false;
    obj_alpha = 0.4f;
    frameCheckNumber = 4;
    frameCount = 0;
    g_TotalErrors = 0;

    sSDKsample = "Xplotion Simulation";
}

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    initConstants();
    camera_trans=(float*)calloc(3,sizeof(float));
    camera_trans_lag=(float*)calloc(3,sizeof(float));
    camera_rot_lag=(float*)calloc(3,sizeof(float));
    camera_rot=(float*)calloc(3,sizeof(float));

    camera_trans[0]=0;
    camera_trans[1]=0;
    camera_trans[2]=-3;
    camera_rot[0]=0;
    camera_rot[1]=0;
    camera_rot[2]=0;

    camera_trans_lag[0]=0;
    camera_trans_lag[1]=0;
    camera_trans_lag[2]=-3;
    camera_rot_lag[0]=0;
    camera_rot_lag[1]=0;
    camera_rot_lag[2]=0;

#if defined(__linux__)
    setenv("DISPLAY", ":0", 0);
#endif


    printf("%s Starting...\n\n", sSDKsample);


    QStringList argos=QCoreApplication::arguments();

    int dfarg=argos.indexOf("-datafile")+1;
    if (dfarg!=0) {//qcore arguments
        const char* pth;
        pth=argos[dfarg].toStdString().c_str();
        fflush(stdout);

        printf("datafile: %s\n", pth);
        fflush(stdout);
        datafilepath=const_cast<char*>(pth);
    } else{datafilepath=DATAFILE_PATH;}

    printf("inicia colorconfig...");
    int colorarg=argos.indexOf("-colorconfig")+1;
    if (colorarg!=0) {//qcore arguments
        const char* pth;
        pth=argos[colorarg].toStdString().c_str();
        colorconfigpath=const_cast<char*>(pth);
    } else {
        colorconfigpath=DEFAULT_COLOR_CONFIG;
    }
    fflush(stdout);


    isNewKey=false;
    newKey=0;
}

GLWidget::~GLWidget()
{
    cleanup();
}

void GLWidget::loadSimulationSystem()
{

    initSimulationSystem(true, datafilepath);
    colorConfig(colorconfigpath);
    initParams();
    refreshLegend();

    psystem->demoCutting = false;
    printf("\nantes del obj\n");
    fflush(stdout);
    obj.Load(OBJ_PATH, psystem->maxTotal);
    printf("\nvolumen cargado\n");
    fflush(stdout);
}

void GLWidget::initializeGL()//initGL
{
    //glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);//TODO Check qt
    glewInit();

    if (!glewIsSupported(
            "GL_VERSION_2_0 GL_VERSION_1_5 GL_ARB_multitexture GL_ARB_vertex_buffer_object")) {
        fprintf(stderr, "Required OpenGL extensions missing.");
        exit(EXIT_FAILURE);
    }
    else{
        printf("glew ok!");
        fflush(stdout);
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0);
    glClearColor(0.2, 0.2, 0.2, 1.0); //TODO should be in colores.config

    //enable alpha for simulation points
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    cudaGLInit(0, NULL);

    loadSimulationSystem();
    reshape(size().width(),size().height());
    QTimer *idleTimer=new QTimer(this);
    connect(idleTimer, SIGNAL(timeout()), this, SLOT(idle()));
         idleTimer->start(0);
}
void GLWidget::resizeGL(int wid, int hei)
{
    reshape(wid,hei);
}
void GLWidget::paintGL() {//display()

    if(isNewKey)
    {
        glFlush();
        glFinish();
        //notFlushed=false;
        isNewKey=false;
        special2(newKey);
    }
    sdkStartTimer(&timer);

    // update the simulation
    renderer->setVertexBuffer(psystem->getCurrentReadBuffer(),
            psystem->getNumParticles());

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // view transform
    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, size().width(), size().height());
    glLoadIdentity();

    for (int c = 0; c < 3; ++c) {
        camera_trans_lag[c] += (camera_trans[c] - camera_trans_lag[c])
                * inertia;
        camera_rot_lag[c] += (camera_rot[c] - camera_rot_lag[c]) * inertia;
    }
    glTranslatef(camera_trans_lag[0], camera_trans_lag[1], camera_trans_lag[2]);
    //TODO Solution:: calibrate axis 3 cuz wasn't working!!!
    glRotatef(camera_rot_lag[0], 1.0, 0.0, 0.0);
    glRotatef(camera_rot_lag[1], 0.0, 1.0, 0.0);

    glGetFloatv(GL_MODELVIEW_MATRIX, modelView);

    // cube
    glColor3f(1.0, 1.0, 1.0);
    //glutWireCube(2.0);//TODO REPLACE

    paintAxis();

    paintBoxCutter();
    paintCutter(psystem->cutterX.pos, psystem->cutterX.size);
    paintCutter(psystem->cutterY.pos, psystem->cutterY.size);
    paintCutter(psystem->cutterZ.pos, psystem->cutterZ.size);

    //paint wireframe
    if (obj_alpha > 0 && obj_drawmode)
        obj.DrawMode(4, obj_alpha); //4 is for triangles

    if (renderer && displayEnabled) {
        renderer->display(displayMode);
    }

    if (displaySliders) {
        glDisable(GL_DEPTH_TEST);
        glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ZERO); // invert color

        params->Render(0, 0);

        glEnable(GL_DEPTH_TEST);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    }
    { //display slider of player
        glDisable(GL_DEPTH_TEST);
        glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ZERO); // invert color

        videoPlayer->Render(0, height - 20);

        glEnable(GL_DEPTH_TEST);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    paintColorBoxScale(currentVarName,coloresScale,valoresScale,lenCol);

    glMatrixMode(GL_MODELVIEW);
    glViewport(0, height * 3 / 4, width * 1 / 4, height * 1 / 4);
    glLoadIdentity();

    //glClearColor(0,0,0.5,1);

    glTranslatef(0.2f, 0, -3);

    glGetFloatv(GL_MODELVIEW_MATRIX, modelView2);

    // cube
    glColor3f(1.0, 1.0, 1.0);
    //glutWireCube(2.0);//TODO REPLACE

    //obj.DrawMode(GL_POINTS,0.1f);
    paintPosition(
            make_float3(camera_trans_lag[0], camera_trans_lag[1],
                    camera_trans_lag[2]));

    sdkStopTimer(&timer);


    //QOpenGLContext::swapBuffers();
    //glutReportErrors();//TODO REPLACE

    computeFPS();
    //glFlush();//TODO es en vano??

}
void GLWidget::mousePressEvent(QMouseEvent *event)
{
    mouse(event->button()-1,MOUSE_DOWN,event->x(),event->y(),event->modifiers());
}
void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    mouse(event->button()-1,MOUSE_UP,event->x(),event->y(),event->modifiers());
}
void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    motion(event->x(),event->y());
}
void GLWidget::setCurrentFrame(int indFrame)
{
    makeCurrent();
    psystem->setCurrentFrame(indFrame);
}
int GLWidget::getNumFrames()
{
    return psystem->nframes;
}

void GLWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Escape:
        key('\033',0,0);
        break;
    case Qt::Key_Enter:
    case Qt::Key_Return:
        key('\r',0,0);
        break;
    case Qt::Key_Up:
        psystem->forwardDirectionCutter = true;
        psystem->advanceCutter();
        break;
    case Qt::Key_Down:
        psystem->forwardDirectionCutter = false;
        psystem->advanceCutter();
        break;
    case Qt::Key_Left:
        psystem->rewind();
        break;

    case Qt::Key_Right:
        psystem->forward();
        break;
    case Qt::Key_F1:
        psystem->currentCutter = 0;
        psystem->initCutters();
        psystem->demoCutting = true;
        psystem->clipped = true;
        makeCurrent();
        psystem->updateColor();
        break;
    case Qt::Key_F2:
        psystem->currentCutter = 1;
        psystem->initCutters();
        psystem->demoCutting = true;
        psystem->clipped = true;
        makeCurrent();
        psystem->updateColor();
        break;
    case Qt::Key_F3:
        psystem->currentCutter = 2;
        psystem->initCutters();
        psystem->demoCutting = true;
        psystem->clipped = true;
        makeCurrent();
        psystem->updateColor();
        break;
    case Qt::Key_F4:
        psystem->demoCutting = false;
        psystem->initCutters2();
        psystem->enableCutting = false;
        psystem->clipped = false;
        makeCurrent();
        psystem->updateColor();
        break;
    default:
        key(event->key(),0,0);
        break;
    }

}
void GLWidget::special(uint keyP)
{
    isNewKey=true;
    newKey=keyP;
}

void GLWidget::special2(uint keyP)
{
    switch (keyP) {
    case Qt::Key_Escape:
        key('\033',0,0);
        break;
    case Qt::Key_Enter:
        key('\r',0,0);
        break;
    case Qt::Key_Up:
        psystem->forwardDirectionCutter = true;
        psystem->advanceCutter();
        break;
    case Qt::Key_Down:
        psystem->forwardDirectionCutter = false;
        psystem->advanceCutter();
        break;
    case Qt::Key_Left:
        psystem->rewind();
        break;

    case Qt::Key_Right:
        psystem->forward();
        break;
    case Qt::Key_F1:
        psystem->currentCutter = 0;
        psystem->initCutters();
        psystem->demoCutting = true;
        psystem->clipped = true;
        psystem->updateColor();
        break;
    case Qt::Key_F2:
        psystem->currentCutter = 1;
        psystem->initCutters();
        psystem->demoCutting = true;
        psystem->clipped = true;
        psystem->updateColor();
        break;
    case Qt::Key_F3:
        psystem->currentCutter = 2;
        psystem->initCutters();
        psystem->demoCutting = true;
        psystem->clipped = true;
        psystem->updateColor();
        break;
    case Qt::Key_F4:
        psystem->demoCutting = false;
        psystem->initCutters2();
        psystem->enableCutting = false;
        psystem->clipped = false;
        psystem->updateColor();
        break;
    default:
        key(keyP,0,0);
        break;
    }

}

void GLWidget::refreshLegend()
{
    lenCol=psystem->totalValuesScale;
    coloresScale=psystem->getColorsScale();
    currentVarName=psystem->getCurrentVarName();
    valoresScale=psystem->getValuesScale();

    printf("legend refreshed:\n");
    printf("%s\n",currentVarName);
    printf("valores:(%f,%f,%f,%f)",valoresScale[0],valoresScale[1],valoresScale[2],valoresScale[3]);
    fflush(stdout);
}

void GLWidget::showHistogram() {

    float widthBar = psystem->width_histogram;
    int boxwidth = (int) (widthBar * 0.9);
    gp << "set boxwidth " << boxwidth << "\n";
    gp << "set style fill solid 0.5\n";
    gp << "set xlabel \"x\"\n";//TODO Set to current variable
    gp << "set ylabel \"Frequency\"\n";
    gp << "set title \"Histogram\"\n";
    gp << "plot \"histog.dat\" using 1:2:3 w boxes lc rgb variable notitle\n";
    gp.flush();

}

void GLWidget::changeObjDrawMode() {
    obj_drawmode = !obj_drawmode;
}

void GLWidget::colorConfig(string configFilePath) {
    if (configFilePath.empty()) {
        psystem->setColorRangeMode(psystem->COLOR_GRADIENT);
        psystem->setColorInitialGradient(new float[3] { 1, 1, 0 });
        psystem->setColorFinalGradient(new float[3] { 1, 0, 0 });
    } else {
        std::ifstream data(configFilePath.c_str());

        std::string line;

        std::cout << line << "\n";
        int nlines = 0;
        while (std::getline(data, line)) {

            std::istringstream is_line(line);
            std::string key;
            if (std::getline(is_line, key, '=')) {
                std::string value;
                if (std::getline(is_line, value)) {
                    std::cout << key << "::" << value << "\n";

                }
                if (key.find("colormode") != -1) {

                    if (value.find("gradient") != -1) {
                        printf("gradient!!!");
                        psystem->setColorRangeMode(psystem->COLOR_GRADIENT);
                    } else if (value.find("short_rainbow") != -1) {
                        psystem->setColorRangeMode(
                                psystem->COLOR_SHORT_RAINBOW);
                    } else if (value.find("full_rainbow") != -1) {
                        psystem->setColorRangeMode(psystem->COLOR_FULL_RAINBOW);
                    }
                } else if (key.find("color") != -1) {
                    const char* str = value.c_str();
                    char *token, *strpos = const_cast<char*>(str);
                    ;

                    float *newcolor = (float*) calloc(3, sizeof(float));
                    for (int var = 0; var < 3; ++var) {
                        token = strsep(&strpos, ",");
                        newcolor[var] = (float) ::atof(token);

                    }

                    if (key.find("color1") != -1) {
                        printf("color1!");
                        psystem->setColorInitialGradient(newcolor);
                    }

                    else if (key.find("color2") != -1) {
                        printf("color2!");
                        psystem->setColorFinalGradient(newcolor);
                    } else if (key.find("color+") != -1) {
                        printf("color+!");
                        psystem->setColorWarningHigh(newcolor);
                    } else if (key.find("color-") != -1) {
                        printf("color-!");
                        psystem->setColorWarningLow(newcolor);
                    }
                }

                else if (key.find("minNormal") != -1) {
                    float valor = (float) ::atof(value.c_str());
                    if (key.find("Temperature") != -1) {
                        psystem->setNormalLow(ParticleSystem::VAR_TEMPERATURE,
                                valor);

                    } else if (key.find("Pressure") != -1) {
                        psystem->setNormalLow(ParticleSystem::VAR_PRESSURE,
                                valor);

                    } else if (key.find("Velocity") != -1) {
                        psystem->setNormalLow(ParticleSystem::VAR_VELOCITY,
                                valor);
                    }
                } else if (key.find("maxNormal") != -1) {
                    float valor = (float) ::atof(value.c_str());
                    if (key.find("Temperature") != -1) {
                        psystem->setNormalHigh(ParticleSystem::VAR_TEMPERATURE,
                                valor);

                    } else if (key.find("Pressure") != -1) {
                        psystem->setNormalHigh(ParticleSystem::VAR_PRESSURE,
                                valor);

                    } else if (key.find("Velocity") != -1) {
                        psystem->setNormalHigh(ParticleSystem::VAR_VELOCITY,
                                valor);
                    }

                }
            }

            nlines++;
        }

    }
    printf("\ncolor config done\n");
    fflush(stdout);

}
void GLWidget::initSimulationSystem(bool bUseOpenGL, string filePath) {

    psystem = new ParticleSystem(bUseOpenGL);

    if (filePath.empty())
        psystem->setFileSource(DATAFILE_PATH);
    else {
        try {
            psystem->setFileSource(filePath);
        } catch (int ex) {
            psystem->setFileSource(DATAFILE_PATH);
        }
    }
    printf("data simulation loaded");
    psystem->reset(ParticleSystem::CONFIG_SIMULATION_DATA);

    if (bUseOpenGL) {
        renderer = new ParticleRenderer;
        renderer->setParticleRadius(psystem->getParticleRadius());
        renderer->setColorBuffer(psystem->getColorBuffer());
        renderer->setColorVectBuffer(psystem->getColorVectBuffer());
    }

    sdkCreateTimer(&timer);
}

void GLWidget::cleanup() {
    sdkDeleteTimer(&timer);
    if (psystem) {
        delete psystem;
    }
    cudaDeviceReset();
    exit(g_TotalErrors > 0 ? EXIT_FAILURE : EXIT_SUCCESS);
}


void GLWidget::computeFPS() {
    frameCount++;
    fpsCount++;

    if (fpsCount == fpsLimit) {
        char fps[256];
        float ifps = 1.f / (sdkGetAverageTimerValue(&timer) / 1000.f);
        sprintf(fps, "Xplotion simulation: %3.1f fps", ifps);

        //glutSetWindowTitle(fps);//TODO
        fpsCount = 0;

        fpsLimit = (int) MAX(ifps, 1.f);
        sdkResetTimer(&timer);
    }
}

void GLWidget::glArrayBox(float w, float h, float d) {

    glBegin(GL_LINES);
    glVertex3f(-w / 2, -h / 2, d / 2);
    glVertex3f(-w / 2, h / 2, d / 2);
    glVertex3f(-w / 2, h / 2, d / 2);
    glVertex3f(-w / 2, h / 2, -d / 2);
    glVertex3f(-w / 2, h / 2, -d / 2);
    glVertex3f(-w / 2, -h / 2, -d / 2);
    glVertex3f(-w / 2, -h / 2, -d / 2);
    glVertex3f(-w / 2, -h / 2, d / 2);

    glVertex3f(w / 2, -h / 2, d / 2);
    glVertex3f(w / 2, h / 2, d / 2);
    glVertex3f(w / 2, h / 2, d / 2);
    glVertex3f(w / 2, h / 2, -d / 2);
    glVertex3f(w / 2, h / 2, -d / 2);
    glVertex3f(w / 2, -h / 2, -d / 2);
    glVertex3f(w / 2, -h / 2, -d / 2);
    glVertex3f(w / 2, -h / 2, d / 2);

    glVertex3f(-w / 2, -h / 2, d / 2);
    glVertex3f(w / 2, -h / 2, d / 2);
    glVertex3f(-w / 2, h / 2, d / 2);
    glVertex3f(w / 2, h / 2, d / 2);
    glVertex3f(-w / 2, h / 2, -d / 2);
    glVertex3f(w / 2, h / 2, -d / 2);
    glVertex3f(-w / 2, -h / 2, -d / 2);
    glVertex3f(w / 2, -h / 2, -d / 2);

    glEnd();
}
void GLWidget::paintColorBoxScale(const char *nameVar, float** colors,float* values,int length) {
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    beginWinCoords();

    float heightTotal=300, diffTotal=values[length-1]-values[0];
    float gradHeight = heightTotal/(length-1), gradWidth = 50;
    int len=0;

    float posY=heightTotal+20;
    char * testing=(char*)calloc(20,sizeof(char));
    int var=0;
    for (var = 0; var < length-1; ++var) {
        gradHeight=(values[var+1]-values[var])*heightTotal/diffTotal;
        float * col1=colors[var];
        float * col2=colors[var+1];
        glBegin(GL_QUADS);
        glColor3fv(col1);
        glVertex2f(width - gradWidth, posY);
        glVertex2f(width - 5, posY);
        glColor3fv(col2);
        glVertex2f(width - 5, posY-gradHeight);
        glVertex2f(width - gradWidth, posY-gradHeight);
        glEnd();

        glColor3f(1.0f, 1.0f, 1.0f);

        testing=(char*)calloc(20,sizeof(char));
        snprintf(testing,20,"%.2f",values[var]);
        len = (int) strlen(testing)*8;
        glPrint(width - gradWidth-len, posY+10,testing,m_font);

        posY-=gradHeight;
    }
    testing=(char*)calloc(20,sizeof(char));
    snprintf(testing,20,"%.2f",values[var]);
    len = (int) strlen(testing)*8;
    glPrint(width - gradWidth-len, posY+10,testing,m_font);

    len = (int) strlen(nameVar)*8;
    glPrint(width - len, 15,nameVar,m_font);

    endWinCoords();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
void GLWidget::paintBoxCutter() {

    glPushMatrix();
    float3 p = psystem->getCutterBoxPos();
    glTranslatef(p.x, p.y, p.z);
    glColor3f(1.0, 0.0, 0.0);
    float3 tamSel = psystem->getSelectSize();
    glArrayBox(tamSel.x, tamSel.y, tamSel.z);

    glPopMatrix();

}
void GLWidget::paintPosition(float3 pos) {
    glPushMatrix();
    float3 p2 = pos;
    glTranslatef(p2.x, p2.y, p2.z);
    glColor3f(1.0, 1.0, 0.0);
    float3 tamSel = make_float3(0.1, 0.1, 0.1);
    //glutSolidCylinder(tamSel.x, tamSel.y, 10, 10);//TODO REPLACE

    float3 p = pos;
    p.x += 0.2f;
    p.z -= 3;

    glBegin(GL_LINES);
    glVertex3f(p.x, p.y, -11);
    glVertex3f(p.x, p.y, -3);

    glVertex3f(p.x, -4, p.z);
    glVertex3f(p.x, 4, p.z);

    glVertex3f(-4, p.y, p.z);
    glVertex3f(4, p.y, p.z);

    glEnd();

    glPopMatrix();

}
void GLWidget::paintCutter(float3 p, float3 tamSel) {
    glPushMatrix();
    glTranslatef(p.x, p.y, p.z);
    glColor3f(0.0, 1.0, 1.0);
    glArrayBox(tamSel.x, tamSel.y, tamSel.z);
    glPopMatrix();

}
void GLWidget::paintAxis() {
    glPushMatrix();

    float widln = 0;
    glGetFloatv(GL_LINE_WIDTH, &widln);
    glLineWidth(widln * 5);
    glBegin(GL_LINES);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(-1, -1, -1);
    glVertex3f(-0.5f, -1, -1);

    glColor3f(0, 1.0, 0.0);
    glVertex3f(-1, -1, -1);
    glVertex3f(-1, -0.5f, -1);

    glColor3f(0, 0.0, 1.0);
    glVertex3f(-1, -1, -1);
    glVertex3f(-1, -1, -0.5f);

    glEnd();
    glLineWidth(widln);
    glPopMatrix();

}

inline float frand() {
    return rand() / (float) RAND_MAX;
}

void GLWidget::reshape(int w, int h) {
    width = w;
    height = h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (float) w / (float) h, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, w, h);

    if (renderer) {
        renderer->setWindowSize(w, h);
        renderer->setFOV(60.0);
    }
}

void GLWidget::mouse(int button, int state, int x, int y, Qt::KeyboardModifiers mods) {

    if (state == MOUSE_DOWN) {
        buttonState |= 1 << button;
    } else if (state == MOUSE_UP) {
        buttonState = 0;
    }

    //if (mods & GLUT_ACTIVE_SHIFT) //MOVE
    if(mods &Qt::ShiftModifier)
    {
        buttonState = 2;
    }
    //else if (mods & GLUT_ACTIVE_CTRL) //ZOOM
    else if(mods &Qt::ControlModifier)
    {
        buttonState = 3;
    }

    ox = x;
    oy = y;

    demoMode = false;
    idleCounter = 0;

    int last = psystem->getCurrentFrame();
    videoPlayer->Mouse(x, y, button, state);
    if (last != psystem->getCurrentFrame()) {
        psystem->updateFrame();

    }
    if (displaySliders) {
        if (params->Mouse(x, y, button, state)) {
            //
        }
    }

    repaint();
    update();
}

void GLWidget::resetView() {
    camera_trans[0] = 0;
    camera_trans[1] = 0;
    camera_trans[2] = -3;

    camera_rot[0] = 0;
    camera_rot[1] = 0;
    camera_rot[2] = 0;
}

// transfrom vector by matrix
void xxform(float *v, float *r, GLfloat *m) {
    r[0] = v[0] * m[0] + v[1] * m[4] + v[2] * m[8] + m[12];
    r[1] = v[0] * m[1] + v[1] * m[5] + v[2] * m[9] + m[13];
    r[2] = v[0] * m[2] + v[1] * m[6] + v[2] * m[10] + m[14];
}

// transform vector by transpose of matrix
void ixxform(float *v, float *r, GLfloat *m) {
    r[0] = v[0] * m[0] + v[1] * m[1] + v[2] * m[2];
    r[1] = v[0] * m[4] + v[1] * m[5] + v[2] * m[6];
    r[2] = v[0] * m[8] + v[1] * m[9] + v[2] * m[10];
}

void ixxformPoint(float *v, float *r, GLfloat *m) {
    float x[4];
    x[0] = v[0] - m[12];
    x[1] = v[1] - m[13];
    x[2] = v[2] - m[14];
    x[3] = 1.0f;
    ixxform(x, r, m);
}

void GLWidget::motion(int x, int y) {
    float dx, dy;
    dx = (float) (x - ox);
    dy = (float) (y - oy);

    if (displaySliders) {
        if (params->Motion(x, y)) {
            ox = x;
            oy = y;

            update();
            return;
        }
    }

    switch (mode) {
    case M_VIEW:
        if (buttonState == 3) {
            // left+middle = zoom
            camera_trans[2] += (dy / 100.0f) * 0.5f * fabs(camera_trans[2]);
        } else if (buttonState & 2) {
            // middle = translate
            camera_trans[0] += dx / 100.0f;
            camera_trans[1] -= dy / 100.0f;
        } else if (buttonState & 1) {
            // left = rotate	int mods;
            camera_rot[0] += dy / 5.0f;
            camera_rot[1] += dx / 5.0f;
        }

        break;

    case M_MOVE: {
        float translateSpeed = 0.003f;
        float3 p = psystem->getCutterBoxPos();

        if (buttonState == 1) {
            float v[3], r[3];
            v[0] = dx * translateSpeed;
            v[1] = -dy * translateSpeed;
            v[2] = 0.0f;
            ixxform(v, r, modelView);
            p.x += r[0];
            p.y += r[1];
            p.z += r[2];
        } else if (buttonState == 2) { //shift
            float v[3], r[3];
            v[0] = 0.0f;
            v[1] = 0.0f;
            v[2] = dy * translateSpeed;
            ixxform(v, r, modelView);
            p.x += r[0];
            p.y += r[1];
            p.z += r[2];
        } else if (buttonState == 3) //ctrl
                {
            //escala la caja de seleccion
            float v[3], r[3];
            v[0] = dx * translateSpeed;
            v[1] = -dy * translateSpeed;
            v[2] = 0.0f;
            ixxform(v, r, modelView);

            //TODO no tiene en cuenta la direccion del arrastre con respecto a la posicion de la caja. De espaldas escala diferente a atras en perspectiva
            float3 tamSel = psystem->getSelectSize();
            tamSel.x += r[0];
            if (tamSel.x < 0.01)
                tamSel.x = 0.01;
            tamSel.y += r[1];
            if (tamSel.y < 0.01)
                tamSel.y = 0.01;
            tamSel.z += r[2];
            if (tamSel.z < 0.01)
                tamSel.z = 0.01;
            psystem->setSelectedSize(tamSel);

        }

        psystem->setCutterBoxPos(p);
    }
        break;
    }

    ox = x;
    oy = y;

    demoMode = false;
    idleCounter = 0;

    update();
}

void GLWidget::changeCurrentVar(int indexVar)
{
    psystem->changeActiveVariable(indexVar);
    if (psystem->currentVariable == ParticleSystem::VAR_VELOCITY) {
        psystem->reset(ParticleSystem::CONFIG_SIMULATION_DATA_VEL);
        displayMode = ParticleRenderer::PARTICLE_ARROWS;
    } else{
        psystem->reset(ParticleSystem::CONFIG_SIMULATION_DATA);
        displayMode = ParticleRenderer::PARTICLE_SPHERES;
    }
    refreshLegend();
}
bool GLWidget::vectorialMode()
{
    if(psystem->currentVariable==ParticleSystem::VAR_VELOCITY)
        return true;
    else return false;
}

void GLWidget::key(unsigned char k, int /*x*/, int /*y*/) {
    char kMayus = k;
    if (k > 96)
        kMayus = k - 32; //toUpperCase()
    switch (kMayus) {
    //custom cases:
    case 'R':
        resetView();
        break;
    case 'X':
        //printf("\nframe::%d\n",psystem->getCurrentFrame());
        psystem->changeActiveVariable();
        if (psystem->currentVariable == ParticleSystem::VAR_VELOCITY) {
            psystem->reset(ParticleSystem::CONFIG_SIMULATION_DATA_VEL);
            displayMode = ParticleRenderer::PARTICLE_ARROWS;
        } else {
            psystem->reset(ParticleSystem::CONFIG_SIMULATION_DATA);
            displayMode = ParticleRenderer::PARTICLE_SPHERES;
        }
        refreshLegend();

        break;

    case 'C':
        psystem->clipped = !psystem->clipped;
        psystem->updateColor();
        break;

    case 'T':
        psystem->toggleDisplayLow();
        break;
    case 'Y':
        psystem->toggleDisplayMiddle();
        break;
    case 'U':
        psystem->toggleDisplayHigh();
        break;
    case 'O':
        changeObjDrawMode();
        break;

    case 'P': //play and pause simulation
        playMode = !playMode;
        break;
    case 'Q':
        psystem->setAlpha(
                psystem->getAlpha() >= 1 ? 1.0f : psystem->getAlpha() + 0.1f);
        break;
    case 'A':
        psystem->setAlpha(
                psystem->getAlpha() <= 0 ? 0.0f : psystem->getAlpha() - 0.1f);
        break;

    case 'I':
        obj_alpha = obj_alpha >= 1 ? 1.0f : obj_alpha + 0.1f;

        break;
    case 'K':
        obj_alpha = obj_alpha <= 0 ? 0 : obj_alpha - 0.1f;
        break;

    case 'V':
        mode = M_VIEW;
        break;

    case 'S':
        mode = M_MOVE;
        break;

    case 'M':
        displayMode = (ParticleRenderer::DisplayMode) ((displayMode + 1)
                % ParticleRenderer::PARTICLE_NUM_MODES);
        if (displayMode == ParticleRenderer::PARTICLE_ARROWS
                && psystem->currentVariable != ParticleSystem::VAR_VELOCITY) {
            key('M', 0, 0);
        }
        break;
    case 'D':
        displaySliders = !displaySliders;
        break;
    case 'H':
        psystem->generateHistogram();
        showHistogram();

        break;

    case '\r':
        psystem->demoCutting = !psystem->demoCutting;
        break;
    case '\033':
        // cudaDeviceReset causes the driver to clean up all state. While
        // not mandatory in normal operation, it is good practice.  It is also
        // needed to ensure correct operation when the application is being
        // profiled. Calling cudaDeviceReset causes all profile data to be
        // flushed before the application exits
        cudaDeviceReset();
        gp << "exit\n";
        exit(EXIT_SUCCESS);
        break;

    case 'F':
        displayMode = (ParticleRenderer::DisplayMode) ParticleRenderer::PARTICLE_POINTS;
        break;
    case 'G':
        displayMode = (ParticleRenderer::DisplayMode) ParticleRenderer::PARTICLE_SPHERES;
        break;
    case 'B':
        displayMode = (ParticleRenderer::DisplayMode) ParticleRenderer::PARTICLE_FLAT_SPHERES;
        break;
    case 'N':
        if (psystem->currentVariable == ParticleSystem::VAR_VELOCITY) {
            displayMode = (ParticleRenderer::DisplayMode) ParticleRenderer::PARTICLE_ARROWS;
        }
        break;
    }

    demoMode = false;
    idleCounter = 0;
    update();
}
void GLWidget::setSimulOpacity(float val)
{
    if(val>=0&&val<=1)
    {
        obj_alpha=val;
    }
    update();

}
void GLWidget::setObjOpacity(float val)
{
    if(val>=0&&val<=1)
    {
        obj_alpha=val;
    }
    update();
}


void GLWidget::idle(void) {

    if (psystem->demoCutting) {
        demoCounter++;
        if (demoCounter++ > 20) {

            makeCurrent();
            psystem->advanceCutter();
            demoCounter = 0;
        }
    }
    if (playMode) {
        playCounter++;
        if (playCounter++ > 15) {

            makeCurrent();
            psystem->forward();
            playCounter = 0;
        }
    }
    update();

}

void GLWidget::initParams() {
    if (g_refFile) {
        timestep = 0.0f;

    } else {
        // create a new parameter list
        params = new ParamListGL("misc");
        params->AddParam(
                new Param<float>("time step", timestep, 0.0f, 1.0f, 0.01f,
                        &timestep));
    }

    //create videoplayer
    videoPlayer = new ParamListGL("videoplayer");
    printf("total frames: %d\n\n", psystem->getFramesCount());
    videoPlayer->AddParam(
            new Param<int>("frame playing", psystem->getCurrentFrame(), 0,
                    psystem->getFramesCount() + 1, 1,
                    psystem->getFramePointer()));
}

void GLWidget::mainMenu(int i) {
    key((unsigned char) i, 0, 0);
}
