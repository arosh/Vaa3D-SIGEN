VAA3DPATH = /Users/arosh/git/vaa3d

TEMPLATE = lib
CONFIG += qt plugin
INCLUDEPATH += $$VAA3DPATH/v3d_main/basic_c_fun
INCLUDEPATH += $$VAA3DPATH/v3d_main/common_lib/include

HEADERS += SIGEN_plugin.h image3d.h
SOURCES += basic_surf_objs.cpp v3d_message.cpp
SOURCES += SIGEN_plugin.cpp image3d.cpp

LIBS += -L../build/sigen_core -lsigen_core
INCLUDEPATH += ../sigen_core

TARGET = $$qtLibraryTarget(SIGEN)
