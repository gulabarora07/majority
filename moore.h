#pragma once

#include <unordered_set>
#include <unordered_map>
#include <limits>

namespace majority {

// T(n) = O(k*n) online algo
// S(n) = O(k) space
// k - 1 max candidates
template <typename ClassType>
class moore {
 public:
  moore(int k_majority_ = 2) : k_majority(k_majority_) {}
  void add(const ClassType& );
  void add(const std::pair<ClassType, int>& );
  void add(const moore<ClassType>& );
  std::unordered_set<ClassType> get_candidates();
 protected:
  int k_majority;
  std::unordered_map<ClassType, int> counts;
};

// T(n) = O(k*n) online algo amortized
// S(n) = O(k) space
// q - p max candidates
template <typename ClassType>
class generic_moore {
 public:
  generic_moore(int p_, int q_) : p(p_), q(q_) {}
  void add(const ClassType& );
  void add(const std::pair<ClassType, int>& );
  void add(const generic_moore<ClassType>& );
  std::unordered_set<ClassType> get_candidates();
 protected:
  int p, q;
  int dummy_count{0};
  std::unordered_map<ClassType, int> counts;
};

}
