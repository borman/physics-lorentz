TEMPLATE = app
TARGET = lorentz-sim
QT = core gui opengl

# tmp dirs

CONFIG(debug) {
  OBJECTS_DIR = .build/debug
  UI_DIR = .build/debug
  MOC_DIR = .build/debug
}
else {
  OBJECTS_DIR = .build/release
  UI_DIR = .build/release
  MOC_DIR = .build/release
}

VPATH += src
INCLUDEPATH += src
DEPENDPATH += src
HEADERS += SimulationViewer.h \
    Simulation.h \
    Geom.h \
    MainWindow.h \
    SimulationStats.h
SOURCES += Main.cpp \
    SimulationViewer.cpp \
    Simulation.cpp \
    Geom.cpp \
    MainWindow.cpp \
    SimulationStats.cpp
FORMS += \
    MainWindow.ui
TRANSLATIONS += \
    lang/ru_RU.ts

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

# Link with Qwt

DEFINES += QWT_DLL

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/3rdparty/qwt-6.0.1/lib/ -lqwt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/3rdparty/qwt-6.0.1/lib/ -lqwtd
else:unix:!macx:!symbian: LIBS += -L$$PWD/3rdparty/qwt-6.0.1/lib/ -lqwt

INCLUDEPATH += $$PWD/3rdparty/qwt-6.0.1/src
DEPENDPATH += $$PWD/3rdparty/qwt-6.0.1/src

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/3rdparty/qwt-6.0.1/lib/qwt.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/3rdparty/qwt-6.0.1/lib/qwtd.lib
else:unix:!macx:!symbian: PRE_TARGETDEPS += $$PWD/3rdparty/qwt-6.0.1/lib/libqwt.so


