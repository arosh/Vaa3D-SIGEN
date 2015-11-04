#pragma once
#include <string>
#include <boost/variant.hpp>
namespace sigen {
typedef boost::variant<bool, int, double, std::string> Variant;
};
