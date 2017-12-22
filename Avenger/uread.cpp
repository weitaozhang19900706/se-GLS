#include <bits/stdc++.h>
#include <sys/time.h>
#include <iostream>
#include <fstream>
#include "db.h"   
#include "leveldb/filter_policy.h"

using namespace std;
    
ifstream infile;




int main( ) {
   
    
    leveldb::Options options;
    options.block_cache = NULL;
    options.filter_policy = leveldb::NewBloomFilterPolicy(8);
    options.max_open_files = 1000;
    
    leveldb::DB* db;
    std::string dbpath = "tdb";   
    leveldb::Status status = leveldb::DB::Open(options, dbpath, &db);   
    leveldb::Status s ;  
    assert(status.ok());   
    printf("Open db OK\n");  
    
    struct timeval tvmain;
    double t_start, t_end, t_duration;
    time_t t;
    
    
    infile.open("../../dataset/uread.txt");
    
    
    string str,val;
    int total = 0, found = 0;
   
    
    gettimeofday(&tvmain, NULL);
    t_start = (double)tvmain.tv_sec + (double)tvmain.tv_usec * 1e-6;
    
    
    
    while ( getline(infile, str) ) {
       
	total ++;
        
        int pos = str.find(' ');
        string operation = str.substr(0, pos);
        str.erase(0, pos + 1);
        if (operation == "delete") {
           
	    s = db->Delete(leveldb::WriteOptions(),str);
	    

        } else if (operation == "read") {
           
	   s = db->Get(leveldb::ReadOptions (), str, &val);  
	  

        } else {
            pos = str.find(' ');
            string key = str.substr(0, pos);
            str.erase(0, pos + 1);
            if (operation == "put") {
                s = db->Put(leveldb::WriteOptions(), key,str); 
            } else {
                s = db->Put(leveldb::WriteOptions(), key,str); 
            }
            

        }
        
        if(s.ok())
	  found ++;
    }
    
    
    
    gettimeofday(&tvmain, NULL);
    t_end = (double)tvmain.tv_sec + (double)tvmain.tv_usec * 1e-6;
    t_duration = t_end - t_start;

    
    
    printf("       %.3lf  time: \n", t_duration);
    printf("       %d, %d   \n", total, found);
    db->Get_time();
    

    delete db;
    
    infile.close();
    
    return 0;
}	  
    
    
   
    
