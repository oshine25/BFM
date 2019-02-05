
#include "bloomfilter.h"

//Bloom filter constructor
/*---------------------Bloom Filter parameters------------------------*/
BloomParams::BloomParams(uint64_t num_ele):
num_elements(num_ele),
false_positive_probability(0.001)
{
    createParams();
}

BloomParams::BloomParams(uint64_t num_ele, double error):
num_elements(num_ele),
false_positive_probability(error)
{
    createParams();
}
void BloomParams::createParams(){
    filter_size= -1.0*num_elements*log(false_positive_probability)/pow(log(2.0),2.0);
    num_hashes=(uint8_t)filter_size*log(2.0)/num_elements;
    cout<<unsigned(num_hashes);
}

BloomParams& BloomParams::operator=(const BloomParams& other) {
    if(this!=&other){
        num_elements=other.num_elements;
        num_hashes=other.num_hashes;
        filter_size=other.filter_size;
        false_positive_probability=other.false_positive_probability;
    }
    return *this;
}

/*---------------------Bloom Filter construction------------------------*/

// bloom filter constructor
BloomFilter::BloomFilter(){}

BloomFilter::BloomFilter(const BloomParams& params):
num_elements(params.num_elements),
num_hashes(params.num_hashes),
filter_size(params.filter_size),
false_positive_probability(params.false_positive_probability)
{
    //set all bits to  0
    for(uint64_t i=0;i<filter_size;i++){
        m_bits.push_back(0);
    }
}

BloomFilter::BloomFilter(const BloomFilter& filter):
num_elements(filter.num_elements),
num_hashes(filter.num_hashes),
filter_size(filter.filter_size),
false_positive_probability(filter.false_positive_probability),
bloom(filter.bloom)
{}

BloomFilter::BloomFilter(size_t size, uint8_t numHashes)
: m_bits(size),
num_hashes(numHashes) {}

BloomFilter::BloomFilter(size_t size)
: bloom(size,vector <bool> (size)){}
//--------------------

//Hash array created using the MurmurHash3 code
static std::array<uint64_t, 2>  myhash(const string *data, int len)
{
    array<uint64_t, 2> hashValue;
    MurmurHash3_x64_128(data, len, 0, hashValue.data());
    return hashValue;
}

//Hash array created using the MurmurHash3 code
uint64_t nthHash(int n, uint64_t hashA, uint64_t hashB, size_t filterSize) {
    return (hashA + n * hashB) % filterSize;
}


//Retrieve the hash results
vector<size_t> BloomFilter::bloomVector(const string data, int len) {
    vector<size_t> arrayResult;
    array<uint64_t, 2> hashValues = myhash(&data, len);
    
    //cout << data << endl;
    
    //cout<<"hash numbers:"<<unsigned(num_hashes)<<endl;
    //cout<<"-------"<<endl;

    for (int n = 0; n < num_hashes; n++)
    {
        uint64_t hashResult = ::nthHash(n, hashValues[0], hashValues[1], m_bits.size());
        //cout<<unsigned(hashResult)<<" ";
        arrayResult.push_back(hashResult);
       // m_bits[nthHash(n, hashValues[0], hashValues[1], m_bits.size())] = true;
    }
  //cout<<"\n"<<"*********************"<<endl;
    return arrayResult;
}

//Returns true or false based on a probabilistic assesment of the array using MurmurHash3
/*bool BloomFilter::possiblyContains(const uint8_t *data, int len) {
    array<uint64_t, 2> hashValues1 = myhash(data, len);
  //  auto hashValues2 = myhash(data, len, 1);
    for (int n = 0; n < num_hashes; n++)
    {
        if (!m_bits[::nthHash(n, hashValues1[0], hashValues1[1], m_bits.size())])
        {
            return false;
        }
    }
    return true;
}*/

//Returns true or false based on a probabilistic assesment of the array in Bloom Filter Matrix
bool BloomFilter::contains(const vector<string> &data)
{
    vector<uint8_t> adata;
    cout<<data.size()<<endl;
    for(int i=0;i<data.size();i++)
    {
        cout<<"hash numbers:"<<unsigned(num_hashes)<<endl;
        cout<<"-------"<<endl;
        
        array<uint64_t, 2> hashValues = myhash (&data.at(i), (int)data.size());
        for (int n = 0; n < num_hashes; n++)
        {
            size_t hashResult = ::nthHash(n, hashValues[0], hashValues[1], 71);
          //  cout<< hashResult<<((i == num_hashes-1) ? "\n" :",");
            adata.push_back(hashResult);
           cout<< unsigned(adata[n])<<" ";
        }
    }

        for(int j=0;j<adata.size();j++)
        {
            //cout<<adata[j];
            for(int k=j+num_hashes;j<adata.size();k++)
            {
                if(bloom[j][k]!=true)
                {
                    return false;
                }
        }
        }
  return true;
}

//insert the membership value in the matrix
void BloomFilter::insertBFM(size_t atr1,size_t atr2)
{
  //  cout<<"attr1:"<<atr1<<endl<<"attr2:"<<atr2<<endl;
    bloom[atr1][atr2]=true;
}

void createBloomFilterA(vector<DataOwnerA> dataArray)
{
    int i;
    int counter=0;
    BloomParams param(7);
    BloomFilter bfi(param);
    BloomFilter bfn(param);
    
  //  BloomFilter bfa(1024, 7);
    BloomFilter bfm(1024);
    
    vector<string> queryArr;
    
      //cout<< dataArray.size();
    
    for(i=0;i<dataArray.size();i++) // for the membership value insertion in the matrix
    {
       vector<size_t> ida = bfi.bloomVector(dataArray[i].Id.c_str(), (int)dataArray[i].Id.size()); //attribute A
       vector<size_t> name = bfn.bloomVector(dataArray[i].Name.c_str(), (int)dataArray[i].Name.size()); //attribute B
        
        for(int j=0;j<ida.size() && j< name.size();j++)
        {
                bfm.insertBFM(ida[j],name[j]);  //defines their inner relation of the attributes and their existence
        }
    }
    
    //// for the query processing purpose
    for(int i=0;i<dataArray.size();i++)
    {
        //bool possiblyContains  = bfi.possiblyContains((uint8_t*)dataArray[i].Id.c_str(), (int)dataArray[i].Id.size());
        queryArr.push_back(dataArray[i].Id.c_str());
        queryArr.push_back(dataArray[i].Name.c_str());
        
      bool Contains = bfm.contains(queryArr);
        
        if(Contains==true)
        {
            counter++;
        }
    
      //  cout<<"contains:"<<Contains<<endl;
    }
    
}


void createBloomFilterB(vector<DataOwnerB> dataArray)
{
    int i;
    int counter=0;
    BloomParams param(7);
    BloomFilter bfi(param);
    BloomFilter bfn(param);
    
    //  BloomFilter bfa(1024, 7);
    BloomFilter bfmB(1024);
    
    vector<string> queryArr;
    
    //cout<< dataArray.size();
    
    for(i=0;i<dataArray.size();i++) // for the membership value insertion in the matrix
    {
        vector<size_t> ida = bfi.bloomVector(dataArray[i].Id.c_str(), (int)dataArray[i].Id.size()); //attribute A
        vector<size_t> name = bfn.bloomVector(dataArray[i].Name.c_str(), (int)dataArray[i].Name.size()); //attribute B
        
        for(int j=0;j<ida.size() && j< name.size();j++)
        {
            bfmB.insertBFM(ida[j],name[j]);  //defines their inner relation of the attributes and their existence
        }
    }
    
    //// for the query processing purpose
    for(int i=0;i<dataArray.size();i++)
    {
        //bool possiblyContains  = bfi.possiblyContains((uint8_t*)dataArray[i].Id.c_str(), (int)dataArray[i].Id.size());
        queryArr.push_back(dataArray[i].Id.c_str());
        queryArr.push_back(dataArray[i].Name.c_str());
        
        bool Contains = bfmB.contains(queryArr);
        
        if(Contains==true)
        {
            counter++;
        }
        
        //  cout<<"contains:"<<Contains<<endl;
    }
  
}


#include <functional>
#include <assert.h>
using namespace std;

int main()
{
    BloomFilter bfm(1024);
    
   // vector<int> srr = {5,6,7,8};
 
   // std::vector<string> s = {"abcd","addfgh"};T.Luo
    vector<DataOwnerA> dataArrayA = DataOwnerA::populate("/Users/rumishakya/Documents/Xcode/BF/BloomFilter/BloomFilter/DataSetA.csv");
    vector<DataOwnerB> dataArrayB = DataOwnerB::populate("/Users/rumishakya/Documents/Xcode/BF/BloomFilter/BloomFilter/DataSetA.csv");
    
    createBloomFilterA(dataArrayA);
    createBloomFilterB(dataArrayB);
    
    for (size_t i = 0, e = bfm.getBitValue().size(); i < e; ++i){
       // vector<bool> *arr1=&bfm.getBitValue()[i];
      // cout << arr1 << ((i == e-1) ? "\n" :",");
    }
  
    
 
}


