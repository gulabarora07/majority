#pragma once

namespace majority {

template <typename ClassType>
void moore<ClassType>::add(const std::pair<ClassType, int>& elem) {
  if (counts.find(elem.first) != counts.end()) { // already exists as candidate
    counts[elem.first] += elem.second;
  } else if (counts.size() < k_majority - 1) { // suicidal fight cannot be done yet
    counts[elem.first] = elem.second;
  } else { // suicidal fight
    counts[elem.first] = elem.second;
    std::vector<ClassType> min_elems;     // multiple classes can be removed
    int min_count = std::numeric_limits<int>::max();
    for(auto & it : counts) {
      if (min_count > it.second) {
        min_count = it.second;
        min_elems.clear();
        min_elems.push_back(it.first);
      } else if (min_count == it.second) {
        min_elems.push_back(it.first);
      }
    }
    for(auto & it : min_elems) {
      counts.erase(it);
    }
    for(auto & it : counts) {
      it.second -= min_count;
    }
  }
}

template <typename ClassType>
void moore<ClassType>::add(const ClassType& elem) {
  add(std::make_pair(elem, 1));
}

template <typename ClassType>
void moore<ClassType>::add(const moore<ClassType>& other) {
  for(auto & it : other.counts) {
    add(it);
  }
}

template <typename ClassType>
std::unordered_set<ClassType> moore<ClassType>::get_candidates() {
  std::unordered_set<ClassType> ans;
  for(auto & it : counts) {
    ans.insert(it.first);
  }
  return ans;
}

template <typename ClassType>
void generic_moore<ClassType>::add(const std::pair<ClassType, int>& elem) {
  dummy_count += (p-1)*elem.second; // no dummy elements for p == 1
  if (counts.find(elem.first) != counts.end()) { // already exists as candidate
    counts[elem.first] += elem.second;
  } else {
    counts[elem.first] = elem.second;
  }
  auto print_state = [&]() {
    std::cout << "dummy:" << dummy_count << ", ";
    for (auto & it: counts) {
      std::cout << it.first << ":" << it.second << ", ";
    }
    std::cout << std::endl;
  };
  auto process_min_count = [&](int min_count) -> int {
    for(auto & it : counts) {
      min_count = std::min(min_count, it.second);
    }
    std::vector<ClassType> min_elems;     // multiple classes can be removed
    for(auto & it : counts) {
      if (min_count == it.second) {
        min_elems.push_back(it.first);
      }
    }
    for(auto & it : min_elems) {
      counts.erase(it);
    }
    for(auto & it : counts) {
      it.second -= min_count;
    }
    return min_count;
  };

  int min_count = std::numeric_limits<int>::max();
  if (counts.size() == q) {
    process_min_count(min_count);
  }
  for (int k = q - counts.size(); k < q;) {
    min_count = dummy_count / k;
    if (min_count == 0) break;
    min_count = process_min_count(min_count);
    dummy_count -= k * min_count;
    k = q - counts.size();
    //std::cout << "k: " << k << " min_count: " << min_count << std::endl;
  }
  //print_state();
}

template <typename ClassType>
void generic_moore<ClassType>::add(const ClassType& elem) {
  add(std::make_pair(elem, 1));
}

template <typename ClassType>
void generic_moore<ClassType>::add(const generic_moore<ClassType>& other) {
  for(auto & it : other.counts) {
    add(it);
  }
}

template <typename ClassType>
std::unordered_set<ClassType> generic_moore<ClassType>::get_candidates() {
  std::unordered_set<ClassType> ans;
  for(auto & it : counts) {
    ans.insert(it.first);
  }
  return ans;
}

}
