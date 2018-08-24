#include <cstdint>
#include <cstdlib>
#include <string>
#include <sstream>
#include "pin.H"
#include "pin_hash.h"
#include "pin_hash2.h"

#define WINDOW_LENGTH 8

VOID appendRTN(ADDRINT rtn_addr);
std::string to_string(UINT64 rtn_addr);
std::string toHistoryString();
VOID Routine(RTN rtn, VOID *v);


//sliding window of instruction address history
//length specified by WINDOW_LENGTH macro
int COUNTER = 0;
UINT64 routine_window[WINDOW_LENGTH];


//hash maps (made myself)

//this one keeps track of the number of unique function histories leading
//up to a load instruction at a specific memory address
static PinHashTable* hash_map = NULL;

//this one keeps track of the number of unique memory load instructions
//following a given function call history 
static PinHashTableReverse* hash_map_reverse = NULL;

//insert routine address into sliding window
VOID appendRTN(ADDRINT rtn_addr) {
  routine_window[COUNTER % WINDOW_LENGTH] = (UINT64) rtn_addr;
  COUNTER++;
}

//converts routine address into string form
std::string to_string(UINT64 rtn_addr) {
  ostringstream stream;
  stream << rtn_addr;
  std::string str = stream.str();
  return str;
}


//generate string representation of function call history 
std::string toHistoryString() {

  std::string history = "";

  if(COUNTER <= WINDOW_LENGTH) {

    for(int i = 0; i < COUNTER; i++)
      history.append(to_string(routine_window[i]));

  } else {
  
    for(int i = COUNTER; i < COUNTER + WINDOW_LENGTH; i++)
      history.append(to_string(routine_window[i % WINDOW_LENGTH]));
  }

  return history;
}


//add instruction->history and history->instruction mappings
//to the corresponding hash maps
VOID insertHistory(ADDRINT ins_addr) {
  std::string history = toHistoryString();
  hash_map->insert((uint64_t) ins_addr, history);
  hash_map_reverse->insert(history, (uint64_t) ins_addr);
}

//every time routine is called, extract memory load instructions
VOID Routine(RTN rtn, VOID *v) {
  
  //open routine to read instructions
  RTN_Open(rtn);

  RTN_InsertCall(rtn, IPOINT_BEFORE, (AFUNPTR) appendRTN, IARG_ADDRINT, RTN_Address(rtn), IARG_END);

  //each instruction that is a memory reference triggers data write
  for(INS ins = RTN_InsHead(rtn); INS_Valid(ins); ins = INS_Next(ins)) {
  
    //call insertHistory
    if(INS_IsMemoryRead(ins)) {
      INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR) insertHistory, IARG_INST_PTR, IARG_END);
    }
  }

  RTN_Close(rtn);
}


VOID Fini(INT32 code, VOID *v) {
  
  //print contents of both hash maps before exiting program
  hash_map->print_histories();
  hash_map_reverse->print_histories();
}

int main(int argc, char *argv[]) {

  PIN_InitSymbols(); 
  PIN_Init(argc, argv);

  //initialize hash maps for both metrics
  hash_map = new PinHashTable(8192);
  hash_map_reverse = new PinHashTableReverse(8192);

  RTN_AddInstrumentFunction(Routine, 0);
  PIN_AddFiniFunction(Fini, 0);

  //begin program
  PIN_StartProgram();  

  return 0;
}

