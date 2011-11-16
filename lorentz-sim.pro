TEMPLATE = app
TARGET = lorentz-sim
QT += core gui opengl

# tmp dirs
OBJECTS_DIR = .build
UI_DIR = .build
MOC_DIR = .build

VPATH += src
INCLUDEPATH += src
DEPENDPATH += src
HEADERS += SimulationViewer.h \
    Simulation.h \
    Geom.h
SOURCES += Main.cpp \
    SimulationViewer.cpp \
    Simulation.cpp \
    Geom.cpp

unix {
  clang {
    QMAKE_CXX = clang++
    QMAKE_LINK = clang++
  }
  openmp {
    QMAKE_CXXFLAGS += -fopenmp
    QMAKE_LFLAGS += -fopenmp
  }
}

win32 {
  DEFINES += _USE_MATH_DEFINES
  openmp {
    QMAKE_CXXFLAGS += /openmp
    QMAKE_LFLAGS += /openmp
  }
}

