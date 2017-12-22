    #include <assert.h>   
    #include <iostream>   
    #include "../db.h"   
    #include "leveldb/filter_policy.h"
    #include <stdio.h>
    #include <stdlib.h>
    #include <sys/time.h>
    #include <stdint.h>
    
    #include <cassert>
    #include <cmath>
    #include <vector>
    #include "../src/cuckoofilter.h"
  
    #define  WriteNum 1000000*10
    #define  ReadNum  1000000*10
    #define  key_size 3
    //using namespace std;   
    using cuckoofilter::CuckooFilter;  
    

    
    int CreateRandStr(char * ar,int len)
    {
	int i;
    
	for( i=0; i<len; i++ )
	    ar[i] = rand() % 256;
	    //ar[i] = rand() % (0x7a - 0x61) + 0x61;
	//ar[i] = '\0';
    }
    
    /*
    int main(int argc,char * argv[])  
    {
	srand(0);
	char x[10];
	string y;
	for(int i = 0; i < 100; i++){
	    CreateRandStr(x,10);
	    y = x;
	    cout<<y<<endl;
	}
	return 0;
	
    }
    */
    
    /*
    int main(int argc,char * argv[])   
    {   
      	  struct timeval tv;
	  double t_start, t_end, t_duration;
	  time_t t;
	  
	  srand(0);
	  char key1[4],key2[10];
	  string k3,k4;
	  
	  
	  
	  leveldb::Options options;
	  options.block_cache = NULL;
	  options.filter_policy = leveldb::NewBloomFilterPolicy(10);
	  options.create_if_missing = true;
	  
	  leveldb::DB* db;
	  std::string dbpath = "tdb";   
	  
	  leveldb::Status status = leveldb::DB::Open(options, dbpath, &db);   
	  assert(status.ok());   
	  cout<<"Open db OK"<<std::endl;   
	    
	  std::string value;   
	  leveldb::Status s ;   
	  int i = 0;


	gettimeofday(&tv, NULL);
	t_start = (double)tv.tv_sec + (double)tv.tv_usec * 1e-6;
	
	s = db->Put(leveldb::WriteOptions(), "helloworld","whosyourdaddy");
	  
	  while(i++ < WriteNum){
	      CreateRandStr(key1,4);
	      CreateRandStr(key2,10);
	      
	      k3=key1;
	      k4=key2;
	      
	      if(i<100)
		  cout<<k3<<endl;
	      s = db->Put(leveldb::WriteOptions(), k3,k4); 
	  }	 	 
	  
	
	gettimeofday(&tv, NULL);
	t_end = (double)tv.tv_sec + (double)tv.tv_usec * 1e-6;
	t_duration = t_end - t_start;

	printf("       %.3lf Write time: \n", t_duration);
	
	
	
	i=0;
	std::string val = "test_value"; 
	
	int hit = 0, miss = 0;


	gettimeofday(&tv, NULL);
	t_start = (double)tv.tv_sec + (double)tv.tv_usec * 1e-6;
	  
	  while(i++ < ReadNum){
	      
	      CreateRandStr(key1,4);
	      k3=key1;	
	      s = db->Get (leveldb::ReadOptions (), k3, &val);
	      if(s.ok())
		  hit++;
	      if(s.IsNotFound())
		  miss++;
	  }	 	 
	  
	  s = db->Get (leveldb::ReadOptions (), "helloworld", &val);
	      if(s.ok())
		  hit++;
	      if(s.IsNotFound())
		  miss++;

	gettimeofday(&tv, NULL);
	t_end = (double)tv.tv_sec + (double)tv.tv_usec * 1e-6;
	t_duration = t_end - t_start;

	printf("       %.3lf Read time: \n", t_duration);	
	printf("       %d hit: \n", hit);	
	printf("       %d miss: \n", miss);	

	  delete db;
	  	 
	  return 0;   
    }  
    */
    
    //cuckoofilter begin	    
	  CuckooFilter<size_t, 12> filter(WriteNum);
    /*
    int main(int argc,char * argv[])   
    {   
      	  struct timeval tvmain;
	  double t_start, t_end, t_duration;
	  time_t t;
	  srand((unsigned) time(&t));
	  
	  leveldb::Options options;
	  //options.block_cache = NULL;
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
	  
	  
	      
	  
	  int i = 0;
	  {
	      gettimeofday(&tvmain, NULL);
	      t_start = (double)tvmain.tv_sec + (double)tvmain.tv_usec * 1e-6;
	      
		
		while(i++ < WriteNum){
		    
		    char key1[key_size],key2[17];
		    std::string k3,k4;
		    
		    CreateRandStr(key1,key_size);
		    CreateRandStr(key2,17);
		    
		    k3=key1;
		    k4=key2;
		    
		    //add cuckoofilter items here 
		    if (filter.Add(k3) != cuckoofilter::Ok) 
			  break;
 
		    s = db->Put(leveldb::WriteOptions(), k3,k4); 
		}	 	 
		
	      
	      gettimeofday(&tvmain, NULL);
	      t_end = (double)tvmain.tv_sec + (double)tvmain.tv_usec * 1e-6;
	      t_duration = t_end - t_start;

	      printf("       %.3lf Write time: \n", t_duration);
	
	  }
	
	
	
	i=0;
	std::string val = "test_value"; 

	{
	      gettimeofday(&tvmain, NULL);
	      t_start = (double)tvmain.tv_sec + (double)tvmain.tv_usec * 1e-6;
		    
		    char key1[key_size];
		    std::string k3;
		    
	      
		while(i++ < ReadNum){
		    
		    CreateRandStr(key1,key_size);  
		    k3=key1;
		    
		    if(filter.Contain(k3) == cuckoofilter::Ok){//CuckooFilter passed 
		    
			  s = db->Get(leveldb::ReadOptions (), k3, &val);  
			  
			  if(s.ok())
			      hit++;
			  if(s.IsNotFound())
			      miss++;
		    }
		    
		}	 	 
		
		//cout<<hit<<" "<<miss<<endl;
		printf("%d %d ", hit, miss);
		

	      gettimeofday(&tvmain, NULL);
	      t_end = (double)tvmain.tv_sec + (double)tvmain.tv_usec * 1e-6;
	      t_duration = t_end - t_start;

	      printf("       %.3lf Read time: \n", t_duration);	
	      
	      db->Get_time();
	      
	}
	  delete db;
	  	 
	  return 0;   
    }  
    */
    
    
    // to test the time cost of timeget function. about 1 million opeartions correspond to 0.109 seconds
    /*
    int i = 0 ;
    
    struct timeval tvmain;
    double t_start, t_end, t_duration = 0;
    double t_start_, t_end_, t_duration_ = 0;
	
    void test()
    {
	  gettimeofday(&tvmain, NULL);
	  t_start = (double)tvmain.tv_sec + (double)tvmain.tv_usec * 1e-6;
	  
	  i = i*(i+119);
	  if(i > 1000000)
	      i = i % 999999 ;
	  
	  gettimeofday(&tvmain, NULL);
	  t_end = (double)tvmain.tv_sec + (double)tvmain.tv_usec * 1e-6;
	  t_duration += (t_end - t_start);
    }
    
    int main()
    {
          int j;
	  gettimeofday(&tvmain, NULL);
	  t_start_ = (double)tvmain.tv_sec + (double)tvmain.tv_usec * 1e-6;
	  
	  for(j = 0; j< ReadNum; j++)
	  {
		gettimeofday(&tvmain, NULL);
		t_start = (double)tvmain.tv_sec + (double)tvmain.tv_usec * 1e-6;
		
		i = i*(i+119);
		if(i > 1000000)
		    i = i % 999999 ;
		
		gettimeofday(&tvmain, NULL);
		t_end = (double)tvmain.tv_sec + (double)tvmain.tv_usec * 1e-6;
		t_duration += (t_end - t_start);
	  }
	    
	  gettimeofday(&tvmain, NULL);
	  t_end_ = (double)tvmain.tv_sec + (double)tvmain.tv_usec * 1e-6;
	  t_duration_ = (t_end_ - t_start_);
	  
	  printf("all : %0.6f\n ", t_duration_);
	  printf("partAdd : %0.6f\n ", t_duration);
	  
	  printf("delta/times : %0.6f\n ", (t_duration_ - t_duration)/(ReadNum/1000000));
      
	  return 0;
    }
    */
    
