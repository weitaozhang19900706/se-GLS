// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#ifndef STORAGE_LEVELDB_INCLUDE_DB_H_
#define STORAGE_LEVELDB_INCLUDE_DB_H_

#include <stdint.h>
#include <stdio.h>
#include "leveldb/iterator.h"
#include "leveldb/options.h"

namespace leveldb {

// Update Makefile if you change these
static const int kMajorVersion = 1;
static const int kMinorVersion = 18;

//add by weitao to test duration of Get operation 
struct timeval tv;

//db_impl.cc
double Get_Srt, Get_End, Get_duration = 0;
double immAndmem_Srt, immAndmem_End, immAndmem_duration = 0;
double currentGet_Srt, currentGet_End, currentGet_duration = 0;

//version_set.cc
double tablecacheGet_Srt, tablecacheGet_End, tablecacheGet_duration = 0;

//table_cache.cc
double FindTable_Srt, FindTable_End, FindTable_duration = 0;
double InternalGet_Srt, InternalGet_End, InternalGet_duration = 0;

//table.cc
double filter_Srt, filter_End, filter_duration = 0;
double BlockReader_Srt, BlockReader_End, BlockReader_duration = 0;
double indexblock_Srt, indexblock_End, indexblock_duration = 0;
double iterSeek_Srt, iterSeek_End, iterSeek_duration = 0;

//counting by MB
int compaction_read = 0;  
int compaction_write = 0; 

int block_read_times = 0;

int mem_hit_times = 0;
int filter_negative_times = 0;
int find_table_times = 0;

int Level_Summary[20] = {0};//counting the sstables in each level



struct Options;
struct ReadOptions;
struct WriteOptions;
class WriteBatch;

// Abstract handle to particular state of a DB.
// A Snapshot is an immutable object and can therefore be safely
// accessed from multiple threads without any external synchronization.
class Snapshot {
 protected:
  virtual ~Snapshot();
};

// A range of keys
struct Range {
  Slice start;          // Included in the range
  Slice limit;          // Not included in the range

  Range() { }
  Range(const Slice& s, const Slice& l) : start(s), limit(l) { }
};

// A DB is a persistent ordered map from keys to values.
// A DB is safe for concurrent access from multiple threads without
// any external synchronization.
class DB {
 public:
  // Open the database with the specified "name".
  // Stores a pointer to a heap-allocated database in *dbptr and returns
  // OK on success.
  // Stores NULL in *dbptr and returns a non-OK status on error.
  // Caller should delete *dbptr when it is no longer needed.
  static Status Open(const Options& options,
                     const std::string& name,
                     DB** dbptr);

  DB() { }
  virtual ~DB();

  // Set the database entry for "key" to "value".  Returns OK on success,
  // and a non-OK status on error.
  // Note: consider setting options.sync = true.
  virtual Status Put(const WriteOptions& options,
                     const Slice& key,
                     const Slice& value) = 0;

  // Remove the database entry (if any) for "key".  Returns OK on
  // success, and a non-OK status on error.  It is not an error if "key"
  // did not exist in the database.
  // Note: consider setting options.sync = true.
  virtual Status Delete(const WriteOptions& options, const Slice& key) = 0;

  // Apply the specified updates to the database.
  // Returns OK on success, non-OK on failure.
  // Note: consider setting options.sync = true.
  virtual Status Write(const WriteOptions& options, WriteBatch* updates) = 0;

  // If the database contains an entry for "key" store the
  // corresponding value in *value and return OK.
  //
  // If there is no entry for "key" leave *value unchanged and return
  // a status for which Status::IsNotFound() returns true.
  //
  // May return some other Status on an error.
  virtual Status Get(const ReadOptions& options,
                     const Slice& key, std::string* value) = 0;

  // Return a heap-allocated iterator over the contents of the database.
  // The result of NewIterator() is initially invalid (caller must
  // call one of the Seek methods on the iterator before using it).
  //
  // Caller should delete the iterator when it is no longer needed.
  // The returned iterator should be deleted before this db is deleted.
  virtual Iterator* NewIterator(const ReadOptions& options) = 0;

  // Return a handle to the current DB state.  Iterators created with
  // this handle will all observe a stable snapshot of the current DB
  // state.  The caller must call ReleaseSnapshot(result) when the
  // snapshot is no longer needed.
  virtual const Snapshot* GetSnapshot() = 0;

  // Release a previously acquired snapshot.  The caller must not
  // use "snapshot" after this call.
  virtual void ReleaseSnapshot(const Snapshot* snapshot) = 0;

  // DB implementations can export properties about their state
  // via this method.  If "property" is a valid property understood by this
  // DB implementation, fills "*value" with its current value and returns
  // true.  Otherwise returns false.
  //
  //
  // Valid property names include:
  //
  //  "leveldb.num-files-at-level<N>" - return the number of files at level <N>,
  //     where <N> is an ASCII representation of a level number (e.g. "0").
  //  "leveldb.stats" - returns a multi-line string that describes statistics
  //     about the internal operation of the DB.
  //  "leveldb.sstables" - returns a multi-line string that describes all
  //     of the sstables that make up the db contents.
  virtual bool GetProperty(const Slice& property, std::string* value) = 0;

  // For each i in [0,n-1], store in "sizes[i]", the approximate
  // file system space used by keys in "[range[i].start .. range[i].limit)".
  //
  // Note that the returned sizes measure file system space usage, so
  // if the user data compresses by a factor of ten, the returned
  // sizes will be one-tenth the size of the corresponding user data size.
  //
  // The results may not include the sizes of recently written data.
  virtual void GetApproximateSizes(const Range* range, int n,
                                   uint64_t* sizes) = 0;

  // Compact the underlying storage for the key range [*begin,*end].
  // In particular, deleted and overwritten versions are discarded,
  // and the data is rearranged to reduce the cost of operations
  // needed to access the data.  This operation should typically only
  // be invoked by users who understand the underlying implementation.
  //
  // begin==NULL is treated as a key before all keys in the database.
  // end==NULL is treated as a key after all keys in the database.
  // Therefore the following call will compact the entire database:
  //    db->CompactRange(NULL, NULL);
  virtual void CompactRange(const Slice* begin, const Slice* end) = 0;

  /*
  void Time_setup()
  {
       Get_Srt = Get_End = Get_duration = 0;
       immAndmem_Srt = immAndmem_End = immAndmem_duration = 0;
       currentGet_Srt = currentGet_End = currentGet_duration = 0;

      //version_set.cc
       tablecacheGet_Srt = tablecacheGet_End = tablecacheGet_duration = 0;

      //table_cache.cc
       FindTable_Srt = FindTable_End = FindTable_duration = 0;
       InternalGet_Srt, InternalGet_End, InternalGet_duration = 0;

      //table.cc
       filter_Srt = filter_End = filter_duration = 0;
       BlockReader_Srt = BlockReader_End = BlockReader_duration = 0;
       indexblock_Srt = indexblock_End = indexblock_duration = 0;
       iterSeek_Srt = iterSeek_End = iterSeek_duration = 0;

       compaction_overhead = 0;

       mem_hit_times = 0;
       filter_negative_times = 0;
       find_table_times = 0;
  }*/
  
  //add to get duration
  void Summary()
  {

       //printf("Get  : %0.6f\n ", Get_duration);
      
       /* 
       printf("(1)immAndmem  : %0.6f\n ", immAndmem_duration);
       printf("(2)currentGet  : %0.6f\n ", currentGet_duration);
       printf("(3)tablecacheGet  : %0.6f\n ", tablecacheGet_duration);
       printf("(4)FindTable  : %0.6f\n ", FindTable_duration);
       printf("(5)InternalGet  : %0.6f\n ", InternalGet_duration);
       printf("(6)filter  : %0.6f\n ", filter_duration);
       printf("(7)BlockReader  : %0.6f\n ", BlockReader_duration);
       printf("(8)indexblock  : %0.6f\n ", indexblock_duration);
       printf("(9)iterSeek  : %0.6f\n ", iterSeek_duration);
       
       printf("mem_hit_times: %d\n" , mem_hit_times);
       printf("find_table_times %d\n " , find_table_times);
       printf("filter_negative_times %d\n", filter_negative_times);*/
      
       printf("compaction_read: %d\n ", compaction_read);
       
       printf("compaction_write: %d\n ", compaction_write);
       
       //printf("block_read_times: %d\n ", block_read_times);
       
       /*
       int i = 0;
       for(i=0;i<7;i++)
	 printf("%d ",Level_Summary[i]);
       
       printf("\n");
       */
  }
 
 /*
 const char* VersionSet::LevelSummary(LevelSummaryStorage* scratch) const {
  // Update code if kNumLevels changes
  assert(config::kNumLevels == 7);
  snprintf(scratch->buffer, sizeof(scratch->buffer),
           "files[ %d %d %d %d %d %d %d ]",
           int(current_->files_[0].size()),
           int(current_->files_[1].size()),
           int(current_->files_[2].size()),
           int(current_->files_[3].size()),
           int(current_->files_[4].size()),
           int(current_->files_[5].size()),
           int(current_->files_[6].size()));
  return scratch->buffer;
}*/
 

  
 private:
  // No copying allowed
  DB(const DB&);
  void operator=(const DB&);
};

// Destroy the contents of the specified database.
// Be very careful using this method.
Status DestroyDB(const std::string& name, const Options& options);

// If a DB cannot be opened, you may attempt to call this method to
// resurrect as much of the contents of the database as possible.
// Some data may be lost, so be careful when calling this function
// on a database that contains important information.
Status RepairDB(const std::string& dbname, const Options& options);

}  // namespace leveldb

#endif  // STORAGE_LEVELDB_INCLUDE_DB_H_