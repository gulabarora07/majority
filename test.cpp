#include <iostream>
#include <vector>
#include <map>
#include <random>
#include<unistd.h>

#include "moore.h"
#include "moore.inl"

using namespace std;

int count(const std::pair<int, int>& wp) {
  return wp.second;
}

int count(const int& x) {
  return 1;
}

int classname(const std::pair<int, int>& wp) {
  return wp.first;
}

int classname(const int& x) {
  return x;
}

void print(const unordered_map<int, int>& um) {
  for (auto &it : um) {
    cout << it.first << ":" << it.second << ", ";
  }
}

template <typename Generator>
void checker(Generator& g, int p, int q) {
  majority::generic_moore<int> gm(p, q);
  double total = 0;
  unordered_map<int, int> um;
  while (not g.empty()) {
    auto curr = g.next();
    //cout << curr << endl;
    total += count(curr);
    if (um.find(classname(curr)) == um.end()) {
      um[classname(curr)] = count(curr);
    } else {
      um[classname(curr)] += count(curr);
    }
    gm.add(curr);
    auto candidates = gm.get_candidates();
    if (candidates.size() > q - p) {
      cout << "Size test failed for (" << p << "/" << q << ") size:" <<
        candidates.size() << endl;
    }
    for (auto &it : um) {
      if (it.second / total > double(p)/q and
          candidates.find(it.first) == candidates.end()) {
        cout << "Test failed for (" << p << "/" << q << ") for key:" <<
          it.first << "(" << it.second << ") total:" << total << endl;
        sleep(1);
      }
    }
  }
}

class generator_int {
 public:
  generator_int(map<int, double>& m, int _n) : n(_n), dis(0.0, 1.0) {
    double total = 0;
    for (auto &it : m) {
      total += it.second;
    }
    double cumm = 0;
    for (auto &it : m) {
      it.second /= total;     // normalization of probability if sum != 1
      cumm += it.second;
      p_class[cumm] = it.first;
    }
    if (p_class.find(1) == p_class.end()) {
      p_class[1] = m.rbegin()->first + 1;
    }
  }
  bool empty() {
    return generated >= n;
  }
  int next() {
    generated++;
    double p = dis(gen);
    return p_class.lower_bound(p)->second;
  }
 private:
  std::uniform_real_distribution<> dis;
  std::mt19937 gen{random_device()()};
  map<double, int> p_class;
  int n;
  int generated;
};

int main() {
  map<int, double> m = {{1, 0.25}, {2, 0.5}, {3, 0.25}};
  generator_int g_i(m, 1000000);
  checker(g_i, 3, 5);
}
