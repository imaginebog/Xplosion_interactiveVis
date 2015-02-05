#ifndef GLWIDGET_H
#define GLWIDGET_H

// OpenGL Graphics includes
#include <GL/glew.h>
#if defined (WIN32)
#include <GL/wglew.h>
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
#include <QTimer>


#define DEFAULT_COLOR_CONFIG PATH_INI"/colores.config"

class GLWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    void refreshView();
    void setCurrentFrame(int indFrame);
    int getNumFrames();
    void changeCurrentVar(int indexVar);
    bool vectorialMode();
    void setSimulOpacity(float val);
    void setObjOpacity(float val);
    void loadSimulationSystem();
    explicit GLWidget(QWidget *parent = 0);
    ParticleSystem * getSimulationSystem()
    {
        return psystem;
    }

    ~GLWidget();

    void key(unsigned char k, int /*x*/, int /*y*/);
    void special(uint keyP);

    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
    void resizeGL(int width, int height) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;



private:
    bool notFlushed;
    void initConstants();
    bool isResetted();
    bool reset;
    bool refresh;
    int resetCount;
    bool isNewKey;
    uint newKey;
    void special2(uint kp);
    Gnuplot gp;

    bool cameraHasChange;
    uint width, height;

    const char* datafilepath;
    const char* colorconfigpath;
    // Scale reference:

    int lenCol;
    float** coloresScale;
    float* valoresScale;
    const char* currentVarName;

    // view params
    int ox, oy;
    int buttonState;
    float* camera_trans;
    float* camera_rot;
    float* camera_trans_lag;
    float* camera_rot_lag;
    float inertia;
    ParticleRenderer::DisplayMode displayMode;

    int mode;
    bool playMode;
    bool displayEnabled;
    bool bPause;
    bool displaySliders;

    bool demoMode;

    int idleCounter;
    int demoCounter;
    int playCounter;
    int idleDelay;

    enum {
        M_VIEW = 0, M_MOVE
    };

    uint numParticles;


    // simulation parameters
    float timestep;
    int rangeColor;

    ParticleSystem *psystem;

    // fps

    StopWatchInterface *timer = NULL;

    int fpsCount;
    int fpsLimit;

    ParticleRenderer *renderer;

    Model_OBJ obj;

    bool obj_drawmode;
    float obj_alpha;

    float modelView[16];
    float modelView2[16];

    ParamListGL *params;
    ParamListGL *videoPlayer;

    //void *m_font = (void *)  GLUT_BITMAP_8_BY_13;

    // Auto-Verification Code
    int frameCheckNumber;
    unsigned int frameCount;
    unsigned int g_TotalErrors;
    char *g_refFile = NULL;

    const char *sSDKsample;


    //void refreshLegend();
    void showHistogram();
    void changeObjDrawMode();
    void colorConfig(string configFilePath);
    void initSimulationSystem(bool bUseOpenGL, string filePath);
    void cleanup();


    void computeFPS();
    void glArrayBox(float w, float h, float d);
    //void paintColorBoxScale(const char *nameVar, float** colors,float* values,int length);
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


    void initParams();
    void mainMenu(int i);

signals:

public slots:
    void idle(void);
};

#endif // GLWIDGET_H
