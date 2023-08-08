#pragma once
#include <vector>
using std::vector;

//! an ordered list of pairs of option and value
template<class K, class V,class B>
struct option_list {
    vector<K> options;
    vector<V> values;
    B& add(K o, V v) {
        options.push_back(o);
        values.push_back(v);
        return static_cast<B&>(*this); }
    size_t size() const { return options.size(); }
};

