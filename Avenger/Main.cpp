#include <assert.h>   
#include <iostream>   
#include "db.h"   
#include "leveldb/filter_policy.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stdint.h>
   
    
#include <cassert>
#include <cmath>
#include <vector>
#include "src/cuckoofilter.h"

#define  WriteNum 1000000*10
#define  ReadNum  100000
#define  key_size 3
#define  value_size 1000
    
    using cuckoofilter::CuckooFilter;  
    

    void CreateRandStr(char * ar,int len)
    {
	int i;
    
	for( i=0; i<len; i++ )
	    ar[i] = rand() % 256;
	   
    }
    
    #define CreateRandInt (rand()*(rand()+999999))%(1<<31)
    
    int int_create()
    {
	int i = 0 ;
	for(int j = 0; j < 4; j++){
	    i = rand()%256 + i<<8;
	}
	return i;
    }
	

    
    void InttoChar(int x, char* ch, int size)
    {
	int i;
	for(i = 0; i < size; i++ )
	{
	    ch[i] = char(x % 256);
	    x = x/256;
	}
    }
    
    int CTI(char *ch, int size)
    {
	int i,v = 0;
	for(i = size - 1;i >= 0; i--)
	{
	    v = v*256 + int(ch[i]); 
	}
	
	return v;
    }
    
    
    //cuckoofilter begin	    
	  CuckooFilter<size_t, 12> filter(WriteNum);
	  
	
    
    int main(int argc,char * argv[])   
    {   
      	  struct timeval tvmain;
	  double t_start, t_end, t_duration;
	  time_t t;
	  srand((unsigned) time(&t));
	  
	  leveldb::Options options;
	  options.block_cache = NULL;
	  options.filter_policy = leveldb::NewBloomFilterPolicy(10);
	  options.create_if_missing = true;
	  
	  leveldb::DB* db;
	  std::string dbpath = "tdb";   
	  
	  leveldb::Status status = leveldb::DB::Open(options, dbpath, &db);   
	  assert(status.ok());   
	  //cout<<"Open db OK"<<std::endl;   
	  printf("Open db OK\n");  
	  
	  std::string value;   
	  leveldb::Status s ;   
	  
	  int hit = 0;
	  int miss = 0;
	  int keyin,keyout;
	  
    
	  
	  int i = 0;
	  
	      gettimeofday(&tvmain, NULL);
	      t_start = (double)tvmain.tv_sec + (double)tvmain.tv_usec * 1e-6;
	      
	      char key1[key_size],key2[value_size];
	      std::string k3,k4,val;
		
		while(i++ < WriteNum){
		    
		    if( rand() % 2 == 0){
		    
			  CreateRandStr(key1,key_size);
			  CreateRandStr(key2,value_size);
			  keyin = CTI(key1,key_size);
			  
			  k3 = key1;
			  k4 = key2;  
			  s = db->Put(leveldb::WriteOptions(), k3,k4); 
			  
		    } else{
			  
			  CreateRandStr(key1,key_size);  
			  k3=key1;  
		   
			  s = db->Get(leveldb::ReadOptions (), k3, &val);  
			  
			  if(s.ok())
			      hit++;
			  if(s.IsNotFound())
			      miss++;
		    }
 
		    
		}	 	 
		
	
	      
	      gettimeofday(&tvmain, NULL);
	      t_end = (double)tvmain.tv_sec + (double)tvmain.tv_usec * 1e-6;
	      t_duration = t_end - t_start;

	      
	      printf("       %d  hit: \n", hit);
	      printf("       %d  miss: \n", miss);
	      printf("       %.3lf  time: \n", t_duration);
	
	  
	
	
	
	
	  delete db;
	  	 
	  return 0;   
    }  
    
