cmake_minimum_required(VERSION 2.8)

include_directories(../src)
include_directories(../third_party)

add_library(gtest-all STATIC ../third_party/gtest/gtest-all.cc)
add_library(gtest_main STATIC ../third_party/gtest/gtest_main.cc)

foreach(t binary_cube_test.cpp builder_test.cpp clipping_test.cpp extractor_test.cpp smart_ptr_test.cpp variant_test.cpp)
  get_filename_component(basename ${t} NAME_WE)
  add_executable(${basename} ${t})
  target_link_libraries(${basename} sigen gtest-all gtest_main pthread)
  add_test(${basename} ${basename})
endforeach(t)

if(BUILD_MAIN)
  foreach(t fileutils_test.cpp swc_writer_test.cpp)
    get_filename_component(basename ${t} NAME_WE)
    add_executable(${basename} ${t})
    target_link_libraries(${basename} sigen sigen_io gtest-all gtest_main pthread boost_system boost_filesystem glog)
    add_test(${basename} ${basename})
  endforeach(t)
endif(BUILD_MAIN)
