#ifndef PIN_HASH2_INCLUDE
#define PIN_HASH2_INCLUDE

#include <iostream>
#include <fstream>
#include <cstdint>
#include <string>

//Holds instruction information in Hash Set
class ReverseSetNode {

  public:
    uint64_t ins;
    ReverseSetNode* next;

    ReverseSetNode(uint64_t input_ins) {
      ins = input_ins;
      next = NULL;
    }
};

//Hash set of instructions
class PinHashSetReverse {
  
  public:
    int size; //must be power of 2
    int num_calls;
    int num_instructions;
    ReverseSetNode** table;

    PinHashSetReverse(int s) {
      size = s;
      num_calls = 0;
      num_instructions = 0;
      table = (ReverseSetNode**) malloc(sizeof(ReverseSetNode*) * s);
    }

    //function used to find potential instruction location
    uint64_t hash(uint64_t ins) {
      uint64_t x = ((ins >> 16) ^ ins) * 0x119de1f3;
      x = ((x >> 16) ^ x) * 0x119de1f3;
      x = (x >> 16) ^ x;
      return x & (size-1);
    }

    //add instruction to hash set
    void insert(uint64_t ins) {

      int index = hash(ins);
      ReverseSetNode* current = table[index];
      ReverseSetNode* prev = NULL;

      num_calls++;

      while(current != NULL) {

        if(current->ins == ins) {
          //instruction already in hash set
          return;
        }

        prev = current;
        current = current->next;
      }

      //instruction not in set, add it
      ReverseSetNode* node = new ReverseSetNode(ins);
      if(prev == NULL)
        table[index] = node;
      else 
        prev->next = node;

      num_instructions++;
    }

};

//holds pairing of instruction address and hash set of histories
class ReverseMapNode {
  
  public:
    std::string history;
    PinHashSetReverse* instructions;
    ReverseMapNode* next;

    ReverseMapNode(std::string h) {
      history = h;
      instructions = new PinHashSetReverse(8192);
      next = NULL;
    }
};


//stores pairings of histories to instruction sets
class PinHashTableReverse {
  
  private:
    static const int PRIME_1 = 54059;
    static const int PRIME_2 = 76963;

  public:
    int size;
    ReverseMapNode** table;  

    PinHashTableReverse(int s) {
      size = s;
      table = (ReverseMapNode**) malloc(sizeof(ReverseMapNode*) * s);
    }  

    int hash(std::string* history) {

      int len = history->length();
      int h = 37;

      for(int i = 0; i < len; i++) {
        h = (h * PRIME_1) ^ ((*history)[i] * PRIME_2);
      }

      return h & (size-1);
    }

    void insert(std::string history, uint64_t ins) {
  
      uint64_t index = hash(&history);
      ReverseMapNode* prev = NULL;
      ReverseMapNode* current = table[index];
      std::string &ref = history;

      while(current != NULL) {
  
        //history already in map
        if((current->history).compare(ref) == 0) {
          (current->instructions)->insert(ins);
          return;  
        }

        prev = current;
        current = current->next;
      }

      //add instruction to map
      ReverseMapNode* node = new ReverseMapNode(history);
      (node->instructions)->insert(ins);

      if(prev == NULL)
        table[index] = node;
      else
        prev->next = node;
    }

    void print_histories() {

      ofstream outfile ("reverse_results.txt", ios::out | ios::trunc);

      //for each unique call history, print # unique instructions  
      for(int i = 0; i < size; i++) {
        ReverseMapNode* current = table[i];
        while(current != NULL) {
          if (current->instructions->num_calls > 1)
          outfile << current->instructions->num_instructions << " " << current->instructions->num_calls << endl;
          current = current->next;
        }
      }

      outfile.close();

    }

};


#endif



