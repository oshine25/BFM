//
//  CSVReader.h
//  BloomFilter
//
//  Created by Rumi Shakya on 2018/11/18.
//  Copyright © 2018 Rumi Shakya. All rights reserved.
//

#ifndef CSVReader_h
#define CSVReader_h
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iterator>
#include <string>
#include <algorithm>

using namespace std;
class CSVReader{
    public:
    static vector < vector <string> > read_data(string filename){

        vector < vector <string> > data;
        ifstream infile(filename);
        string line;
        
        while (getline(infile, line)){
            vector <string> temp;
            stringstream ss(line);
            string content;
            char delim = ',';
            
            while (getline(ss, content, delim)) {
                // Provide proper checks here for content like if empty
                // Also strip down symbols like !, ., ?, etc.
                // Finally push it.
                //cout << content << endl;
                //cout<<content<<endl;
                // cout<<"From inside"<<endl;
                temp.push_back(content);
            }
            data.push_back(temp);
            
        }
        //close file
        infile.close();
        
        return data;
    }
    
};
#endif /* CSVReader_h */
