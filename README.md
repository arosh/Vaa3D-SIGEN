# Vaa3D-SIGEN

## About

TBD...

## Architecture

* loader :: (image\_files | vaa3d\_memory) -> image\_sequence
* binarizer :: image\_sequence -> binary\_cube
* extractor :: binary\_cube -> cluster
* builder :: cluster -> neuron
* writer :: neuron -> (swc\_file | vaa3d\_memory)

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
$ ./waf configure
$ ./waf --checkall # run test
$ ./waf
```

## References

* T.Yamasaki, T.Isokawa, N.Matsui, H.Ikeno and R.Kanzaki, "Reconstruction and simulation for three-dimensional morphological structure of insect neurons," Neurocomputing, Jun. 2006. \[[link](http://dx.doi.org/10.1016/j.neucom.2005.12.042)\]

## How to use `NeuronTree`

1. declare `NeuronTree nt`
2. set `nt.name` and `nt.comment`
3. describe `infostring` as needed
4. declare `NeuronSWC pt`
5. set `pt.n`, `pt.type`, `pt.x`, `pt.y`, `pt.z`, `pt.r`, `pt.pn`
6. append `pt` to `nt.listNeuron`

types

* 0-Undefined
* 1-Soma
* 2-Axon
* 3-Dendrite
* 4-Apical Dendrite
* 5-Fork Point
* 6-End Point
* 7-Custom

references

* `NeuronSWC` at `v3d_main/basic_c_fun/basic_surf_objs.h`
* `writeSWC_file` at `v3d_main/basic_c_fun/basic_surf_objs.cpp`

## How to handle `Image4DSimple` from `callback.getImage(curwin)`

```
V3DLONG su = img->getUnitBytes();
V3DLONG sx = img->getXDim();
V3DLONG sy = img->getYDim();
V3DLONG sz = img->getZDim();
V3DLONG sc = img->getCDim();

V3DLONG stride_x = su;
V3DLONG stride_y = su*sx;
V3DLONG stride_z = su*sx*sy;
V3DLONG stride_c = su*sx*sy*sz;

v3d_uint8 ptr = img->getRawData()
return ptr[stride_x*x + stride_y*y + stride_z*z + stride_c*c]
```

references

* `Image4DProxy#at` at `v3d_main/basic_c_fun/basic_4dimage.h`

## TODO

* 設定メニューっぽいもののサンプル `vaa3d_tools/released_plugins/v3d_plugins/neurontracing_vn2/vn_app1.h`
* https://code.google.com/p/vaa3d/wiki/PluginDesignGuide

## Setting

* Interpolation (VT, DT)
* Clipping (level)
* Smoothing (level)
* Output sub dendrites
