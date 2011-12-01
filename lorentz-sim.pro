TEMPLATE = app
TARGET = lorentz-sim
QT += core gui opengl

# tmp dirs
OBJECTS_DIR = .build/
UI_DIR = .build/
MOC_DIR = .build/

VPATH += src
INCLUDEPATH += src
DEPENDPATH += src
HEADERS += SimulationViewer.h \
    Simulation.h \
    Geom.h \
    MainWindow.h
SOURCES += Main.cpp \
    SimulationViewer.cpp \
    Simulation.cpp \
    Geom.cpp \
    MainWindow.cpp

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
  openmp:QMAKE_CXXFLAGS += /openmp
}

FORMS += \
    src/MainWindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/3rdparty/qwt-6.0.1/lib/ -lqwt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/3rdparty/qwt-6.0.1/lib/ -lqwtd
else:unix:!macx:!symbian: LIBS += -L$$PWD/3rdparty/qwt-6.0.1/lib/ -lqwt

INCLUDEPATH += $$PWD/3rdparty/qwt-6.0.1/src
DEPENDPATH += $$PWD/3rdparty/qwt-6.0.1/src

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/3rdparty/qwt-6.0.1/lib/qwt.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/3rdparty/qwt-6.0.1/lib/qwtd.lib
else:unix:!macx:!symbian: PRE_TARGETDEPS += $$PWD/3rdparty/qwt-6.0.1/lib/libqwt.a
