#pragma once
#include <string>
#include <vector>
#include <map>
#include <boost/utility.hpp>
#include <boost/shared_ptr.hpp>
// #include "common/variant.h"
namespace sigen {
enum neuron_type {
  EDGE,
  BRANCH,
  CONNECT
};
class neuron_node : boost::noncopyable {
public:
  // 1-based
  int id_;
  // gravity point
  double gx_, gy_, gz_;
  double radius_;
  neuron_type type_;
  std::vector<neuron_node *> adjacent_;
  // Variant may contain `degree`, `real_distance`, `electrical_distance`
  // std::map<std::string, Variant> values_;
  void add_connection(neuron_node *node);
};
class neuron /* : noncopyable */ {
public:
  neuron_node *root_;
  std::vector<boost::shared_ptr<neuron_node> > storage_;
  neuron clone() const;
};
} // namespace sigen
