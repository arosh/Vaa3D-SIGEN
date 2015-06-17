# Vaa3D-SIGEN

## About

TBD...

## Architecture

* Reader :: (ImageFiles | Vaa3dMemory) -> ImageSequence
* PreProcessor :: ImageSequence -> ImageSequence
* Extractor :: ImageSequence -> Connection
* GraphConstructor :: Connection -> Graph
* PostProcessor :: Graph -> Graph
* Writer :: Graph -> (SwcFile | Vaa3dMemory)

- Graph inherits Connection

## Library

* [waf](https://waf.io/) (included in this repository)
* [waf-unittest](https://github.com/tanakh/waf-unittest) (included in this repository)
* [google-glog](https://github.com/google/glog) >= ?.?
* [cmdline.h](https://github.com/tanakh/cmdline) (included in this repository)
* [OpenCV](http://opencv.org/) >= ?.?

## References

* T.Yamasaki, T.Isokawa, N.Matsui, H.Ikeno and R.Kanzaki, "Reconstruction and simulation for three-dimensional morphological structure of insect neurons," Neurocomputing, Jun. 2006. \[[link](http://dx.doi.org/10.1016/j.neucom.2005.12.042)\]
