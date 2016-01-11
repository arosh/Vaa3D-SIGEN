cmake_minimum_required(VERSION 2.8)

include_directories(../src)
include_directories(../third_party/googletest)
include_directories(../third_party/googletest/include)

add_library(gtest-all STATIC ../third_party/googletest/src/gtest-all.cc)

foreach(t binary_cube_test.cpp builder_test.cpp clipping_test.cpp extractor_test.cpp smart_ptr_test.cpp variant_test.cpp)
  get_filename_component(basename ${t} NAME_WE)
  add_executable(${basename} ../third_party/googletest/src/gtest_main.cc ${t})
  target_link_libraries(${basename} sigen gtest-all pthread)
  add_test(${basename} ${basename})
endforeach(t)

if(BUILD_MAIN)
  foreach(t fileutils_test.cpp swc_writer_test.cpp)
    get_filename_component(basename ${t} NAME_WE)
    add_executable(${basename} ../third_party/googletest/src/gtest_main.cc ${t})
    target_link_libraries(${basename} sigen sigen_io gtest-all pthread boost_system boost_filesystem glog)
    add_test(${basename} ${basename})
  endforeach(t)
endif(BUILD_MAIN)
