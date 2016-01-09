cmake_minimum_required(VERSION 2.8)
add_library(sigen_io STATIC
  sigen/loader/file_loader.cpp
  sigen/writer/swc_writer.cpp
  sigen/writer/fileutils.cpp
  sigen/binarizer/binarizer.cpp
)
add_library(sigen STATIC
  sigen/common/binary_cube.cpp
  sigen/common/disjoint_set.cpp
  sigen/common/neuron.cpp
  sigen/common/voxel.cpp
  sigen/common/cluster.cpp
  sigen/extractor/extractor.cpp
  sigen/builder/builder.cpp
  sigen/neuronprocess/neuronprocess.cpp
  sigen/interface.cpp
)
# set(THIRD_PARTY ../third_party)
# include_directories(${THIRD_PARTY})
# add_executable(main
#   sigen/main.cpp
#   ${THIRD_PARTY}/cmdline/cmdline.h
# )
include_directories(. ../third_party ${OPENCV_INCLUDE_DIRS})
add_executable(main
  sigen/main.cpp
)
target_link_libraries(main sigen sigen_io boost_system boost_filesystem glog ${OPENCV_LIBRARIES})
