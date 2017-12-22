#include <bits/stdc++.h>
#include <sys/time.h>
#include <iostream>
#include <fstream>
#include "db.h"   
#include "leveldb/filter_policy.h"

#define opcount 100000

using namespace std;
    
ifstream infile;




int main( ) {
   
    
    leveldb::Options options;
    options.block_cache = NULL;
    options.filter_policy = leveldb::NewBloomFilterPolicy(10);
    options.create_if_missing = true;
    leveldb::DB* db;
    std::string dbpath = "tdb";   
    leveldb::Status status = leveldb::DB::Open(options, dbpath, &db);   
    leveldb::Status s ;  
    assert(status.ok());   
    printf("Open db OK\n");  
    
    struct timeval tvmain;
    double t_start, t_end, t_duration, t_total;
    time_t t;
    
    
    infile.open("/home/weitao/Test/original/Avenger/readonly.txt");
    
    
    string str,val;
    
   
    
    
    while (getline(infile, str)) {
       
        gettimeofday(&tvmain, NULL);
	t_start = (double)tvmain.tv_sec + (double)tvmain.tv_usec * 1e-6;  
      
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
        
	gettimeofday(&tvmain, NULL);
	t_end = (double)tvmain.tv_sec + (double)tvmain.tv_usec * 1e-6;
	t_duration = t_end - t_start;
	
	t_total += t_duration;
        
    }
    
   

    
    
    printf("       %.3lf  time: \n", t_total/opcount);

    delete db;
    
    return 0;
}	  
    
    
   
    
