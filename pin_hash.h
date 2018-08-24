#ifndef PIN_HASH_INCLUDE
#define PIN_HASH_INCLUDE

#include <iostream>
#include <fstream>
#include <cstdint>
#include <string>

//This file includes code for a hash table that maps a memory load address
//to the set of function call histories that led up to that load

//Holds string information in Hash Set
class SetNode {

  public:
    std::string history;
    SetNode* next;

    SetNode(std::string h) {
      history = h;
      next = NULL;
    }
};

//Hash set of strings
class PinHashSet {
  
  private:
    static const int PRIME_1 = 54059;
    static const int PRIME_2 = 76963;

  public:
    int size; //must be power of 2
    int num_paths;
    int num_calls;
    SetNode** table;

    PinHashSet(int s) {
      size = s;
      num_paths = 0;
      num_calls = 0;
      table = (SetNode**) malloc(sizeof(SetNode*) * s);
    }

    //function used to find potential string location
    int hash(std::string* history) {

      int len = history->length();
      int h = 37;

      for(int i = 0; i < len; i++) {
        h = (h * PRIME_1) ^ ((*history)[i] * PRIME_2);
      }

      return h & (size-1);
    }

    //add string to hash set
    void insert(std::string history) {
  
      num_calls++;

      int index = hash(&history);
      SetNode* current = table[index];
      SetNode* prev = NULL;
      std::string &ref = history;

      while(current != NULL) {

        if((current->history).compare(ref) == 0) {
          //string already in hash set
          return;
        }

        prev = current;
        current = current->next;
      }

      //string not in set, add it
      SetNode* node = new SetNode(history);
      if(prev == NULL)
        table[index] = node;
      else 
        prev->next = node;

      num_paths++;
    }

};

//holds pairing of instruction address and hash set of histories
class MapNode {
  
  public:
    uint64_t ins;
    PinHashSet* past;
    MapNode* next;

    MapNode(uint64_t i) {
      ins = i;
      past = new PinHashSet(8192);
      next = NULL;
    }
};


//stores pairings of instructions and history sets
class PinHashTable {
  
  public:
    int size;
    MapNode** table;  

    PinHashTable(int s) {
      size = s;
      table = (MapNode**) malloc(sizeof(MapNode*) * s);
    }  

    uint64_t hash(uint64_t ins) {
      uint64_t x = ((ins >> 16) ^ ins) * 0x119de1f3;
      x = ((x >> 16) ^ x) * 0x119de1f3;
      x = (x >> 16) ^ x;
      return x & (size-1);
    }

    void insert(uint64_t ins, std::string history) {
  
      uint64_t index = hash(ins);
      MapNode* prev = NULL;
      MapNode* current = table[index];

      while(current != NULL) {
  
        //instruction already in map
        if(current->ins == ins) {
          (current->past)->insert(history);
          return;  
        }

        prev = current;
        current = current->next;
      }

      //add instruction to map
      MapNode* node = new MapNode(ins);
      (node->past)->insert(history);

      if(prev == NULL)
        table[index] = node;
      else
        prev->next = node;
    }

    void print_histories() {

      ofstream outfile ("results.txt", ios::out | ios::trunc);

      //for each unique instruction, print # unique function call paths  
      for(int i = 0; i < size; i++) {
        MapNode* current = table[i];
        while(current != NULL) {
          if (current->past->num_calls > 1)
          outfile << current->past->num_paths << " " << current->past->num_calls << endl;
          current = current->next;
        }
      }

      outfile.close();

    }

};


#endif



