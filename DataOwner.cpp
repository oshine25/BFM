//
//  DataOwner.cpp
//  BloomFilter
//
//  Created by Rumi Shakya on 2018/11/18.
//  Copyright © 2018 Rumi Shakya. All rights reserved.
//

//#include "CSVReader.h"
//#include<iostream>
//#include <vector>
//
//using namespace std;
//
//class DataOwnerA
//{
//public:
//    std::string Id;
//    std::string Name;
//    std::string Age;
//
//public:
//    void setData(vector<string> data){
//        for (int i=0;i<data.size();i++){
//            Id=data[0];
//            Name=data[1];
//            Age=data[2];
//        }
//    }
//    static vector<DataOwnerA>  populate(string filename){
//
//        vector <vector <string> > info = CSVReader::read_data(filename);
//        vector<DataOwnerA> finalData;
//        for (int i=1;i<info.size();i++){
//            DataOwnerA dataOwnerA;
//            dataOwnerA.setData(info[i]);
//            finalData.push_back(dataOwnerA);
//        }
//        return finalData;
//    }
//};

/*int main(){
    vector<DataOwnerA> dataOwnerArray=DataOwnerA::populate("DataSetA.csv");
    for (int i=0;i<dataOwnerArray.size();i++){
        cout<<"*******"<<endl;
        cout<<i<<endl;
        cout<<"ID: "+dataOwnerArray[i].Id<<endl;
        cout<<"Name: "+dataOwnerArray[i].Name<<endl;
        cout<<"Age: "+dataOwnerArray[i].Age<<endl;
    }
}*/
