## Note

Before push, please run `./tools/cppformat.sh` and `./tools/lint.sh`.

## Architecture

* loader :: (image\_files | vaa3d\_memory) -> image\_sequence
* binarizer :: image\_sequence -> binary\_cube
* extractor :: binary\_cube -> cluster
* builder :: cluster -> neuron
* writer :: neuron -> (swc\_file | vaa3d\_memory)

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

## CMake options

```
-DCMAKE_BUILD_TYPE=Debug -DBUILD_MAIN=ON
```

## Coverage

```
gcovr --html --html-detail -r .. -o coverage.html
```

## TODO

* sample of configure window? `vaa3d_tools/released_plugins/v3d_plugins/neurontracing_vn2/vn_app1.h`
* https://code.google.com/p/vaa3d/wiki/PluginDesignGuide
* [picogc](https://github.com/kazuho/picogc)
