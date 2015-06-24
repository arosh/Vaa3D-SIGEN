#include "../common/binary_cube.h"

namespace sigen {
class extractor {
  binary_cube c;

public:
  extractor(const binary_cube &cube);
  remove_isolation_point();
};
}
