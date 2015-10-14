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
$ ./waf --checkall # test
$ ./waf
```

## References

* T.Yamasaki, T.Isokawa, N.Matsui, H.Ikeno and R.Kanzaki, "Reconstruction and simulation for three-dimensional morphological structure of insect neurons," Neurocomputing, Jun. 2006. \[[link](http://dx.doi.org/10.1016/j.neucom.2005.12.042)\]

## TODO

* `vaa3d_tools/bigneuron_ported/neuronAssembler/neutube`をパクる
