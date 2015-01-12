#ifndef GLWIDGET_H
#define GLWIDGET_H



// OpenGL Graphics includes
#include <GL/glew.h>
#if defined (WIN32)
#include <GL/wglew.h>
#endif
#if defined(__APPLE__) || defined(__MACOSX)
#include <GLUT/glut.h>
#ifndef glutCloseFunc
#define glutCloseFunc glutWMCloseFunc
#endif
#else
#include <GL/freeglut.h>
#endif

// CUDA runtime
#include <cuda_runtime.h>

// CUDA utilities and system includes
#include <helper_functions.h>
#include <helper_cuda.h>    // includes cuda.h and cuda_runtime_api.h
#include <helper_cuda_gl.h> // includes cuda_gl_interop.h// includes cuda_gl_interop.h
// Includes
#include <stdlib.h>
#include <cstdlib>
#include <cstdio>
#include <algorithm>
#include "particleSystem.h"
#include "render_particles.h"
#include "paramgl.h"
#include "gnuplot-iostream.h"

#include <QCoreApplication>
#include <QtOpenGL/QGLWidget>
#include <QOpenGLWidget>
#include <QOpenGLContext>


#define DEFAULT_COLOR_CONFIG PATH_INI"/colores.config"

class GLWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    void loadSimulationSystem();
    explicit GLWidget(QWidget *parent = 0);
    ~GLWidget();

protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
    void resizeGL(int width, int height) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

private:
    Gnuplot gp;

    uint width = 640, height = 480;

    char* datafilepath;
    char* colorconfigpath;
    // Scale reference:

    int lenCol;
    float** coloresScale;
    float* valoresScale;
    char* currentVarName;

    // view params
    int ox, oy;
    int buttonState = 0;
    float camera_trans[];
    float camera_rot[];
    float camera_trans_lag[];
    float camera_rot_lag[];
    const float inertia = 0.1f;
    ParticleRenderer::DisplayMode displayMode = ParticleRenderer::PARTICLE_SPHERES;
    //ParticleRenderer::DisplayMode displayMode = ParticleRenderer::PARTICLE_POINTS; //important!!

    int mode = 0;
    bool playMode = false;
    bool displayEnabled = true;
    bool bPause = false;
    bool displaySliders = false;

    bool demoMode = false;

    int idleCounter = 0;
    int demoCounter = 0;
    int playCounter = 0;
    const int idleDelay = 2000;

    enum {
        M_VIEW = 0, M_MOVE
    };

    uint numParticles = 0;


    // simulation parameters
    float timestep = 0.5f;
    int rangeColor = 200;

    ParticleSystem *psystem = 0;

    // fps

    StopWatchInterface *timer = NULL;

    int fpsCount = 0;
    int fpsLimit = 1;

    ParticleRenderer *renderer = 0;

    Model_OBJ obj;

    bool obj_drawmode = false;
    float obj_alpha = 0.4f;

    float modelView[16];
    float modelView2[16];

    ParamListGL *params;
    ParamListGL *videoPlayer;

    void *m_font = (void *)  GLUT_BITMAP_8_BY_13;

    // Auto-Verification Code
    const int frameCheckNumber = 4;
    unsigned int frameCount = 0;
    unsigned int g_TotalErrors = 0;
    char *g_refFile = NULL;

    const char *sSDKsample = "Xplotion Simulation";


    void refreshLegend();
    void showHistogram();
    void changeObjDrawMode();
    void colorConfig(string configFilePath);
    void initSimulationSystem(bool bUseOpenGL, string filePath);
    void cleanup();


    void computeFPS();
    void glArrayBox(float w, float h, float d);
    void paintColorBoxScale(const char *nameVar, float** colors,float* values,int length);
    void paintBoxCutter();
    void paintPosition(float3 pos);
    void paintCutter(float3 p, float3 tamSel);
    void paintAxis();
    void display();
    inline float frand();
    void reshape(int w, int h);
    void mouse(int button, int state, int x, int y,Qt::KeyboardModifiers mods);
    void resetView();
    void motion(int x, int y);
    void key(unsigned char k, int /*x*/, int /*y*/);
    void special(int k, int x, int y);
    void idle(void);
    void initParams();
    void mainMenu(int i);
    int mainreplaced(int argc, char **argv);

signals:

public slots:
};

#endif // GLWIDGET_H
