//
//  bloomfilter.h
//  BloomFilter
//
//  Created by Rumi Shakya on 2019/01/24.
//  Copyright © 2019 Rumi Shakya. All rights reserved.
//

#ifndef bloomfilter_h
#define bloomfilter_h
#include <iostream>
#include <vector>
#include <array>
#include <fstream>
#include <string>
#include <cmath>
#include "MurmurHash3.h"
#include "bloomfilter.h"
#include "DataOwner.hpp"


class BloomParams{
public:
    uint64_t num_elements;
    uint8_t num_hashes;
    uint64_t filter_size;
    double false_positive_probability;
    
    BloomParams(uint64_t num_ele);
    BloomParams(uint64_t num_ele, double error);
    void createParams();
    BloomParams& operator=(const BloomParams& other);
};
class BloomFilter{
    uint64_t num_elements;
    uint8_t num_hashes;
    uint64_t filter_size;
    double false_positive_probability;
   // uint8_t m_numHashes;
    
public:
    std::vector<bool> m_bits;
    std::vector< vector<bool> > bloom;
    
    BloomFilter();
    BloomFilter(const BloomParams& params);
    BloomFilter(const BloomFilter& filter);
    BloomFilter(size_t size, uint8_t numHashes);
    BloomFilter(size_t size);
    
    inline std::vector<vector <bool>>& getBitValue(){return bloom;}
    vector<size_t> bloomVector(const string data, int len);
    bool possiblyContains(const uint8_t *data, int len);
    bool contains(const vector<string> &data); //--------- part check
    void bloomJoin(const BloomFilter& filter);
    void insertBFM(size_t atr1,size_t atr2);
    std::array<uint64_t,2> hash(const uint8_t* data, std::size_t len);
   // uint64_t nthHash(uint8_t n, uint64_t hashA, uint64_t hashB);
  //  bool contains(const BloomFilter& filter);
    void clear();
    void print();
    
};


#endif /* bloomfilter_h */
