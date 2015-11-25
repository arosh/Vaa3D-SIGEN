VAA3DPATH = /Users/arosh/git/vaa3d

TEMPLATE = lib
CONFIG += qt plugin release
QMAKE_CXXFLAGS += -Wno-c++11-extensions

INCLUDEPATH += $$VAA3DPATH/v3d_main/basic_c_fun
INCLUDEPATH += $$VAA3DPATH/v3d_main/common_lib/include
SOURCES	+= $$VAA3DPATH/v3d_main/basic_c_fun/v3d_message.cpp
SOURCES	+= $$VAA3DPATH/v3d_main/basic_c_fun/basic_surf_objs.cpp

HEADERS += SIGEN_plugin.h
SOURCES += SIGEN_plugin.cpp

INCLUDEPATH += ../sigen_core
SOURCES += ../sigen_core/common/binary_cube.cpp
SOURCES += ../sigen_core/common/disjoint_set.cpp
SOURCES += ../sigen_core/common/neuron.cpp
SOURCES += ../sigen_core/common/voxel.cpp
SOURCES += ../sigen_core/common/cluster.cpp
SOURCES += ../sigen_core/extractor/extractor.cpp
SOURCES += ../sigen_core/builder/builder.cpp
SOURCES += ../sigen_core/interface.cpp

TARGET = $$qtLibraryTarget(SIGEN)
