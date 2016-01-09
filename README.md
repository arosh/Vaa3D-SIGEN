# Vaa3D-SIGEN

## About

TBD...

## Requirements

* [Python](https://www.python.org/) >= ?.? (to use Waf)
* [waf](https://waf.io/) (included in this repository)
* [waf-unittest](https://github.com/tanakh/waf-unittest) (included in this repository)
* [cmdline.h](https://github.com/tanakh/cmdline) (included in this repository)
* [google-glog](https://github.com/google/glog) >= ?.? ([documentation](http://google-glog.googlecode.com/svn/trunk/doc/glog.html))
* [OpenCV](http://opencv.org/) >= ?.?
* [Boost](http://www.boost.org/) >= ?.? (using boost::filesystem)

## How to Build

```
$ mkdir build
$ cd build
$ cmake ..
```

If you run `cmake .` in project top directory, `CMakeFiles`, `CMakeCache.txt`, `cmake_install.cmake`, `Makefile` will be located in undesirable directories.

## Setting

* Interpolation (VT, DT)
* Clipping (level)
* Smoothing (level)

## References

* Yamasaki, T., Isokawa, T., Matsui, N., Ikeno, H. and Kanzaki, R.: Reconstruction and simulation for three-dimensional morphological structure of insect neurons, Neurocomputing 69, pp.1043-1047 (2006). \[[link](http://dx.doi.org/10.1016/j.neucom.2005.12.042)\]
