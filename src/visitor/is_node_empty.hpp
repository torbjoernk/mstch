#pragma once

#include "mstch/mstch.hpp"

namespace mstch {

class is_node_empty {
 public:
  template<class T>
  bool operator()(const T&) const {
    return false;
  }

  bool operator()(const empty&) const {
    return true;
  }

  bool operator()(const std::nullptr_t&) const {
    return true;
  }

  bool operator()(const int& value) const {
    return value == 0;
  }

  bool operator()(const unsigned int& value) const {
    return value == 0;
  }

  bool operator()(const double& value) const {
    return value == 0;
  }

  bool operator()(const bool& value) const {
    return !value;
  }

  bool operator()(const std::string& value) const {
    return value.empty();
  }

  bool operator()(const array& array) const {
    return array.empty();
  }
};

}
