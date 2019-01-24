#ifndef BLOOM_FILTER_H
#define BLOOM_FILTER_H
#include <iostream>
#include <vector>
#include <array>
#include <fstream>
#include "MurmurHash3.h"
#include "DataOwner.hpp"

//basic structure of a bloom filter object
class BloomFilter {
public:
    BloomFilter(size_t size, uint8_t numHashes);
    BloomFilter(size_t size);
    vector<size_t> bloomVector(const uint8_t *data, int len,uint32_t seed);
    bool possiblyContains(const uint8_t *data, int len);
    bool contains(const vector<uint8_t> *data, int len);
    void bloomJoin(const BloomFilter& filter);
    void insertBFM(size_t atr1,size_t atr2);
   // array<uint64_t,2> myhash(const uint8_t *data, int len, uint32_t seed);
    
private:
    uint8_t m_numHashes;

    std::vector<bool> m_bits;
    std::vector< vector<bool> > bloom;
};

//Bloom filter constructor
BloomFilter::BloomFilter(size_t size, uint8_t numHashes)
: m_bits(size),
m_numHashes(numHashes) {}

BloomFilter::BloomFilter(size_t size)
: bloom(size,vector <bool> (size)){}


//Hash array created using the MurmurHash3 code
static std::array<uint64_t, 2>  myhash(const uint8_t *data, int len, uint32_t seed)
{
    array<uint64_t, 2> hashValue;
    MurmurHash3_x64_128(data, len, seed, hashValue.data());
    return hashValue;
}

//Hash array created using the MurmurHash3 code
inline size_t nthHash(int n, uint64_t hashA, uint64_t hashB, size_t filterSize) {
    return (hashA + n * hashB) % filterSize;
}


//Retrieve the hash results
vector<size_t> BloomFilter::bloomVector(const uint8_t *data, int len, uint32_t seed) {
    vector<size_t> arrayResult;
    array<uint64_t, 2> hashValues = myhash(data, len, seed);
    //cout<<hashValues.data()<<endl<<m_bits.size()<<endl;
  //  cout<<"List of hash results:"<<endl;
    for (int n = 0; n < m_numHashes; n++)
    {
       // cout<<hashValues[0]<<hashValues[1]<<endl;
        size_t hashResult = nthHash(n, hashValues[0], hashValues[1], m_bits.size());
        arrayResult.push_back(hashResult);
        //cout<<arrayResult[n]<<endl;
        
       // m_bits[nthHash(n, hashValues[0], hashValues[1], m_bits.size())] = true;
    }
  // cout<<"*********************"<<endl;
    return arrayResult;
}

//Returns true or false based on a probabilistic assesment of the array using MurmurHash3
bool BloomFilter::possiblyContains(const uint8_t *data, int len) {
    array<uint64_t, 2> hashValues1 = myhash(data, len, 0);
  //  auto hashValues2 = myhash(data, len, 1);
    for (int n = 0; n < m_numHashes; n++)
    {
        if (!m_bits[nthHash(n, hashValues1[0], hashValues1[1], m_bits.size())])
        {
            return false;
        }
    }
    return true;
}

//Returns true or false based on a probabilistic assesment of the array in Bloom Filter Matrix
bool BloomFilter::contains(const vector<uint8_t> *data, int len)
{
    vector<uint8_t> adata;
    for(int i=0;i<data->size();i++)
    {
        //cout<<data[i]<<endl;
        array<uint64_t, 2> hashValues = myhash((const uint8_t*)data->at(i), len, 0);
        for (int n = 0; n < m_numHashes; n++)
        {
            size_t hashResult = nthHash(n, hashValues[0], hashValues[1], m_bits.size());
            adata.push_back(hashResult);
        }
    }
    for(int j=0;j<adata.size();j++)
    {
        for(int k=j+1;j<adata.size();k++)
        {
            if(bloom[j][k]!=1)
            {
                return false;
            }
        }
    }
  return true;
}
#endif

void BloomFilter::bloomJoin(const BloomFilter& filter){
    //bloomfilter<-filter
    for(uint64_t i=0;i<m_bits.size();i++){
        m_bits[i]=m_bits[i]*filter.m_bits[i];
    }
    
}

//insert the membership value in the matrix
void BloomFilter::insertBFM(size_t atr1,size_t atr2)
{
    cout<<"attr1:"<<atr1<<endl<<"attr2:"<<atr2<<endl;
    bloom[atr1][atr2]=true;
}

void createBloomFilterA(vector<DataOwnerA> dataArray)
{
    int i;
    BloomFilter bfi(1024, 7);
    BloomFilter bfn(1024, 7);
    BloomFilter bfa(1024, 7);
    BloomFilter bfm(1024);
    vector<uint8_t> queryArr;
    vector<uint8_t> queryArray;
    
    for(i=0;i<dataArray.size();i++)
    {
        
       vector<size_t> ida = bfi.bloomVector((uint8_t*)dataArray[i].Id.c_str(), (int)dataArray[i].Id.size(),0);
        //cout<<ida[i];
        for(int j=0;j<ida.size();j++)
        {
       vector<size_t> name = bfn.bloomVector((uint8_t*)dataArray[i].Name.c_str(), (int)dataArray[i].Name.size(),1);
       // bfa.add((uint8_t*)dataArray[i].Age.c_str(), dataArray[i].Name.size(),2);
            for(int k=0;k<name.size();k++)
            {
               // cout<<ida[j]<<endl<<name[k];
                bfm.insertBFM(ida[j],name[k]);
            }
        }
    }
    
    // bf.add((uint8_t*)t.c_str(), t.size());
    for(int i=0;i<dataArray.size();i++)
    {
        //bool possiblyContains  = bfi.possiblyContains((uint8_t*)dataArray[i].Id.c_str(), (int)dataArray[i].Id.size());
        
       queryArr.push_back(*(uint8_t*)dataArray[i].Id.c_str());
       queryArr.push_back(*(uint8_t*)dataArray[i].Name.c_str());
        //queryArray=queryArr;
        //cout<<*(uint8_t*)dataArray[i].Id.c_str()<<",";
        //cout<<*(uint8_t*)dataArray[i].Name.c_str()<<endl;
       bool Contains = bfm.contains(&queryArr, (int)dataArray[i].Id.size());
        cout<<Contains<<endl;
    }
    
}


void createBloomFilterB(vector<DataOwnerB> dataArray)
{
  //  int i;
    BloomFilter bfi(1024, 7);
    BloomFilter bfn(1024, 7);
    BloomFilter bfa(1024, 7);
    //vector<BloomFilter> bfm(1024 * 1024);
    
    for(int i=0;i<dataArray.size();i++)
    {
        bool possiblyContainsID  = bfi.possiblyContains((uint8_t*)dataArray[i].Id.c_str(), (int)dataArray[i].Id.size());
        //  assert(possiblyContains);
    }
  
}


#include <functional>
#include <assert.h>
using namespace std;

int main()
{
    vector<int> srr = {5,6,7,8};
 
   // std::vector<string> s = {"abcd","addfgh"};T.Luo
    vector<DataOwnerA> dataArray = DataOwnerA::populate("/Users/rumishakya/Documents/Xcode/BF/BloomFilter/BloomFilter/DataSetA.csv");
    vector<DataOwnerB> dataArrayB = DataOwnerB::populate("/Users/rumishakya/Documents/Xcode/BF/BloomFilter/BloomFilter/DataSetA.csv");
    
    createBloomFilterA(dataArray);
    createBloomFilterB(dataArrayB);
    
 
}


