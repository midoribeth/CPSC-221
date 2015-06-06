#ifndef _DOUBLEHASHDICT_CPP
#define _DOUBLEHASHDICT_CPP

//DoubleHashDict.cpp
#include "DoubleHashDict.hpp"
#include <cassert>
#include <cstdlib>//for NULL
#include <iostream>

// An implementation of a dictionary ADT as hash table with double hashing
//

const int DoubleHashDict::primes[] = {53, 97, 193, 389, 769, 1543, 3079,
      6151, 12289, 24593, 49157, 98317, 196613, 393241, 786433, 1572869,
      3145739, 6291469, 12582917, 25165843, 50331653, 100663319,
      201326611, 402653189, 805306457, 1610612741, -1};
// List of good primes for hash table sizes from
// http://planetmath.org/goodhashtableprimes
// The -1 at the end is to guarantee an immediate crash if we run off
// the end of the array.

DoubleHashDict::DoubleHashDict() {
  size_index = 0;
  size = primes[size_index];
  table = new bucket[size](); // Parentheses force initialization to 0
  number = 0;

  // Initialize the array of counters for probe statistics
  probes_stats = new int[MAX_STATS]();
}

DoubleHashDict::~DoubleHashDict() {
  // Delete all table entries...
  for (int i=0; i<size; i++) {
    if (table[i].key!=NULL) {
      delete table[i].key;
      // Don't delete data here, to avoid double deletions.
    }
  }
  // Delete the table itself
  delete [] table;

  // It's not good style to put this into a destructor,
  // but it's convenient for this assignment...
  cout << "Probe Statistics for find():\n";
  for (int i=0; i<MAX_STATS; i++)
    cout << i << ": " << probes_stats[i] << endl;
  delete [] probes_stats;
}

int DoubleHashDict::hash1(string keyID) {
  int h=0;
  for (int i=keyID.length()-1; i>=0; i--) {
    h = (keyID[i] + 31*h) % size;
  }
// 221 Students:  DO NOT CHANGE OR DELETE THE NEXT FEW LINES!!!
// We will use this code when marking to be able to watch what
// your program is doing, so if you change things, we'll mark it wrong.
#ifdef MARKING_TRACE
std::cout << "Hash 1:  " << keyID << " to " << h << std::endl;
#endif
// End of "DO NOT CHANGE" Block
  return h;
}

int DoubleHashDict::hash2(string keyID) {
  int h=0;
  for (int i=keyID.length()-1; i>=0; i--) {
    h = (keyID[i] + 63*h) % size;
  }
  // Make sure second hash is never 0 or size
  h = h/2 + 1;

// 221 Students:  DO NOT CHANGE OR DELETE THE NEXT FEW LINES!!!
// We will use this code when marking to be able to watch what
// your program is doing, so if you change things, we'll mark it wrong.
#ifdef MARKING_TRACE
std::cout << "Hash 2:  " << keyID << " to " << h << std::endl;
#endif
// End of "DO NOT CHANGE" Block
  return h;
}

void DoubleHashDict::rehash() {
// 221 Students:  DO NOT CHANGE OR DELETE THE NEXT FEW LINES!!!
// And leave this at the beginning of the rehash() function.
// We will use this code when marking to be able to watch what
// your program is doing, so if you change things, we'll mark it wrong.
#ifdef MARKING_TRACE
std::cout << "*** REHASHING " << size;
#endif
// End of "DO NOT CHANGE" Block


  // TODO:  Your code goes here...
  
 number = 0;
 int oldsize = size;
 size = primes[++size_index];
 bucket *oldtable = table;
 table = new bucket[size]();
  
 for (int i = 0; i < oldsize; i++) {
   if (oldtable[i].key != NULL)
      add(oldtable[i].key, oldtable[i].data);
    
    }
 
   delete [] oldtable; 

// 221 Students:  DO NOT CHANGE OR DELETE THE NEXT FEW LINES!!!
// And leave this at the end of the rehash() function.
// We will use this code when marking to be able to watch what
// your program is doing, so if you change things, we'll mark it wrong.
#ifdef MARKING_TRACE
std::cout << " to " << size << " ***\n";
#endif
// End of "DO NOT CHANGE" Block
} 

bool DoubleHashDict::find(PuzzleState *key, PuzzleState *&pred) {
  // Returns true iff the key is found.
  // Returns the associated value in pred
  // Be sure not to keep calling getUniqId() over and over again!

  string keyID = key->getUniqId();
  int index = hash1(keyID);
  int num_probes = 0;
  int i = 0;

  while ((table[index].key) && (table[index].keyID != keyID) && (num_probes < size)) {
     num_probes+=1;
     i++;
     index = (hash1(keyID) + (i*hash2(keyID))) % size;
  }

  if (num_probes < MAX_STATS) {
	probes_stats[num_probes] += 1;
  }

  if ((num_probes >= size) || (table[index].key == NULL)) {
	return false;
  }
  pred = table[index].data;
  return true;
}

// You may assume that no duplicate PuzzleState is ever added.
void DoubleHashDict::add(PuzzleState *key, PuzzleState *pred) {
  
  if ((((float)number + 1) / (float)size) > 0.5)     
            rehash();

       string keyID = key->getUniqId();
         int probe = hash1(keyID);
         int i = 0;
         while (i < size) {
                 if (table[probe].key == NULL){
                         table[probe].key = key;
                         table[probe].keyID = keyID;
                         table[probe].data = pred;
                         i++;
                         number++;
                         return;
                 }
 
                 else{
                         probe = (hash1(keyID) + (i*hash2(keyID))) % size;
                         i++;
         }
 }
 }


#endif 
