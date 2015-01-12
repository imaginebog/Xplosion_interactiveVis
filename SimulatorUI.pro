#-------------------------------------------------
#
# Project created by QtCreator 2015-01-06T21:06:13
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SimulatorUI
TEMPLATE = app


SOURCES += main.cpp\
    glwidget.cpp \
    ModelOBJ.cpp \
    particleSystem.cpp \
    particles.cpp \
    render_particles.cpp \
    shaders.cpp \
    simului.cpp

HEADERS  += \
    glwidget.h \
    gnuplot-iostream.h \
    ModelOBJ.h \
    nvcc_custom_operators.h \
    particleSystem.h \
    render_particles.h \
    shaders.h \
    particleSystem.cuh \
    simului.h

FORMS    += \
    simului.ui

OTHER_FILES +=  ./particleSystem_cuda.cu



CUDA_SOURCES = ./particleSystem_cuda.cu

CUDA_SDK = "/usr/local/cuda-6.5/"   # Path to cuda SDK install
CUDA_DIR = "/usr/local/cuda-6.5/"
CUDA_SAMPLES= "/usr/local/cuda-6.5/samples/common/inc"


SYSTEM_NAME = unix         # Depending on your system either 'Win32', 'x64', or 'Win64'
SYSTEM_TYPE = 64            # '32' or '64', depending on your system
CUDA_ARCH = sm_21           # Type of CUDA architecture, for example 'compute_10', 'compute_11', 'sm_10'
NVCC_OPTIONS = --use_fast_math

INCLUDEPATH += $$CUDA_DIR/include $$CUDA_SAMPLES

QMAKE_LIBDIR += $$CUDA_DIR/lib/

CUDA_OBJECTS_DIR = ./

QMAKE_LFLAGS += -L/usr/local/cuda-6.5/samples/common/lib/linux/x86_64
QMAKE_LFLAGS += -L/usr/local/cuda-6.5/lib64
# Add the necessary libraries
CUDA_LIBS = -lcuda -lcudart -lGL -lutil -lboost_iostreams -lboost_system -lboost_filesystem -lGLEW -lGLU -lX11 -lXi -lXmu -lglut

# The following makes sure all path names (which often include spaces) are put between quotation marks
CUDA_INC = $$join(INCLUDEPATH,'" -I"','-I"','"')
#LIBS += $$join(CUDA_LIBS,'.so ', '', '.so')
LIBS += $$CUDA_LIBS

CONFIG(debug, debug|release) {
    # Debug mode
    cuda_d.input = CUDA_SOURCES
    cuda_d.output = $$CUDA_OBJECTS_DIR/${QMAKE_FILE_BASE}.o
    cuda_d.commands = $$CUDA_DIR/bin/nvcc -D_DEBUG $$NVCC_OPTIONS $$CUDA_INC $$NVCC_LIBS --machine $$SYSTEM_TYPE -arch=$$CUDA_ARCH -c -o ${QMAKE_FILE_OUT} ${QMAKE_FILE_NAME}
    cuda_d.dependency_type = TYPE_C
    QMAKE_EXTRA_COMPILERS += cuda_d
}
else {
    # Release mode
    cuda.input = CUDA_SOURCES
    cuda.output = $$CUDA_OBJECTS_DIR/${QMAKE_FILE_BASE}.o
    cuda.commands = $$CUDA_DIR/bin/nvcc $$NVCC_OPTIONS $$CUDA_INC $$NVCC_LIBS --machine $$SYSTEM_TYPE -arch=$$CUDA_ARCH -c -o ${QMAKE_FILE_OUT} ${QMAKE_FILE_NAME}
    cuda.dependency_type = TYPE_C
    QMAKE_EXTRA_COMPILERS += cuda
}

