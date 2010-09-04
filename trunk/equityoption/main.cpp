/*
 *  main.cpp
 *  
 *
 *  Created by William Wang on 01/09/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>
#include <string>

#include "COptionRecord.h"
#include "CBadRecord.h"

using namespace std;

int main(int argc, char * argv[])
{
    // check file name
    if (argc != 2) {
        cout << "Please supply the input option data file name!" << endl;
        cout << "For example: "<<argv[0]<< " OptionData.txt" << endl;
        exit(0);
    }
    
    ifstream ifsOptionData(argv[1]);
    if ( !ifsOptionData.is_open())  {
        cout << "Failed to read file " << argv[1] << endl;
    } else {
        vector<COptionRecord> vOptionData;
        string sLine;
		
        // assume the file starts with data, with no header info, 
        getline(ifsOptionData, sLine); // item numbers
        getline(ifsOptionData, sLine); // header
        // otherwise we just process or skip these lines
        while( getline(ifsOptionData, sLine) )
        {
            try {
                COptionRecord rec(sLine);
				
                vOptionData.push_back(rec);
            } 
            catch (CBadRecord &badRec) {
                cout << "Bad record: " << badRec.what() << endl; 
            }
            catch (exception &e) {
                cout << "Standard exception: "<< e.what() << endl;
            }
            catch(...) {
                cout << "Unknown exception" << endl;
            }
        }
        ifsOptionData.close();

        // print out the price
        for_each(vOptionData.begin(), vOptionData.end(), mem_fun_ref(&COptionRecord::price));
    } 
}
