# Vaa3D-SIGEN

## About

TBD...

## Requirements for Plugin

* [CMake](https://cmake.org/) >= 2.8

## Requirements for Runner

* [CMake](https://cmake.org/) >= 2.8
* [google-glog](https://github.com/google/glog) >= ?.? ([documentation](http://google-glog.googlecode.com/svn/trunk/doc/glog.html))
* [OpenCV](http://opencv.org/) >= ?.?
* [Boost](http://www.boost.org/) >= ?.? (using boost::filesystem)

## How to Build Plugin

```
$ mkdir build
$ cd build
$ cmake -DCMAKE_BUILD_TYPE=Release ..
$ make
```

If you run `cmake .` in project top directory, `CMakeFiles`, `CMakeCache.txt`, `cmake_install.cmake`, `Makefile` will be located in undesirable directories.

## How to Build Runner

```
$ mkdir build
$ cd build
$ cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_MAIN=ON ..
$ make
```

## How to Test

```
$ mkdir build
$ cd build
$ cmake -DCMAKE_BUILD_TYPE=Debug -DBUILD_MAIN=ON ..
$ make test
```

## Setting

* Interpolation (VT, DT)
* Clipping (level)
* Smoothing (level)

## References

* Yamasaki, T., Isokawa, T., Matsui, N., Ikeno, H. and Kanzaki, R.: Reconstruction and simulation for three-dimensional morphological structure of insect neurons, Neurocomputing, Vol.69, pp.1043-1047 (2006). \[[link](http://dx.doi.org/10.1016/j.neucom.2005.12.042)\]
