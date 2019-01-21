// Copyright (c) 2019 Winter Chen. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//本代码是从LevelDB中移植过来的bloomfilter

// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.


#ifndef BLOOMFILTER_H
#define BLOOMFILTER_H

#include <string>

using namespace std;

class BloomFilter {
private:
    size_t bits_per_key_;
    size_t k_;
    size_t bits;
    string bloom_filter;

public:
    //bits_per_key：即m/n，每一个元素的位数，理论上这个值越大误判率就越小，但是占用空间就越大。
    explicit BloomFilter(int bits_per_key)//防止隐式转化
      : bits_per_key_(bits_per_key) {
        // We intentionally round down to reduce probing cost a little bit
        k_ = static_cast<size_t>(bits_per_key * 0.69);  // 0.69 =~ ln(2)
        if (k_ < 1) k_ = 1;
        if (k_ > 30) k_ = 30;
    }
    //n:元素个数
    void CreateFilter(int n);

    bool KeyMayMatch(const string& key);

    void Add(const string& key);
    //return the info of the BloomFilter
    string GetInfo();
};
#endif // !BLOOMFILTER_H
