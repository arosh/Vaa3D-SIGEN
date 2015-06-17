# Vaa3D-SIGEN

## About

## Architecture

* Reader :: (ImageFiles | Vaa3dMemory) -> ImageSequence
* PreProcessor :: ImageSequence -> ImageSequence
* Extractor :: ImageSequence -> Connection
* GraphConstructor :: Connection -> Graph
* PostProcessor :: Graph -> Graph
* Writer :: Graph -> (SwcFile | Vaa3dMemory)

- Graph inherits Connection

## References

* T.Yamasaki, T.Isokawa, N.Matsui, H.Ikeno and R.Kanzaki, "Reconstruction and simulation for three-dimensional morphological structure of insect neurons," Neurocomputing, Jun. 2006. \[[link](http://dx.doi.org/10.1016/j.neucom.2005.12.042)\]
