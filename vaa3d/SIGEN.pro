VAA3DPATH = /Users/arosh/git/vaa3d

TEMPLATE = lib
CONFIG += qt plugin
INCLUDEPATH += $$VAA3DPATH/v3d_main/basic_c_fun
INCLUDEPATH += $$VAA3DPATH/v3d_main/common_lib/include

HEADERS += SIGEN_plugin.h image3d.h
SOURCES += basic_surf_objs.cpp v3d_message.cpp
SOURCES += SIGEN_plugin.cpp image3d.cpp

# QMAKE_CXXFLAGS += -std=c++11 -stdlib=libc++ -Wno-reserved-user-defined-literal
# QMAKE_LFLAGS += -stdlib=libc++
LIBS += -L../build/sigen_core -lsigen
INCLUDEPATH += ../sigen_core
# QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.7

TARGET = $$qtLibraryTarget(SIGEN)
