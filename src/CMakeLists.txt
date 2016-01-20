cmake_minimum_required(VERSION 2.8)

include_directories(.)

# Vaa3D plugin
add_library(sigen STATIC
  sigen/common/binary_cube.cpp
  sigen/common/disjoint_set.cpp
  sigen/common/neuron.cpp
  sigen/common/voxel.cpp
  sigen/extractor/extractor.cpp
  sigen/builder/builder.cpp
  sigen/neuronprocess/neuronprocess.cpp
  sigen/interface.cpp
)

if(BUILD_MAIN)
  include_directories(../third_party ${OPENCV_INCLUDE_DIRS})

  add_library(sigen_io STATIC
    sigen/loader/file_loader.cpp
    sigen/writer/swc_writer.cpp
    sigen/writer/fileutils.cpp
    sigen/binarizer/binarizer.cpp
  )

  add_executable(main
    sigen/main.cpp
  )

  target_link_libraries(main sigen sigen_io boost_system boost_filesystem glog ${OPENCV_LIBRARIES})
endif(BUILD_MAIN)
