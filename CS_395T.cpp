#include <stdio.h>
#include "pin.H"

#define WINDOW_LENGTH 32


RTN global_routine;
void appendRTN(RTN rtn);
void printTrace();
VOID Routine(RTN rtn, VOID *v);

//out file
FILE* outfile;

//sliding window of instruction addresses
int COUNTER = 0;
UINT64 routine_window[WINDOW_LENGTH];

//insert routine address into sliding window
void appendRTN(RTN rtn) {
  routine_window[(COUNTER++) % WINDOW_LENGTH] = (UINT64) RTN_Address(rtn);
}

void printTrace() {

  fprintf(outfile, "MEMORY ACCESS (rtn %s):\n", RTN_Name(global_routine).c_str());
  if(COUNTER <= WINDOW_LENGTH) {

    for(int i = 0; i < COUNTER; i++)
      fprintf(outfile, "%lu\n", routine_window[i]);

  } else {
  
    for(int i = COUNTER; i < COUNTER + WINDOW_LENGTH; i++)
      fprintf(outfile, "%lu\n", routine_window[i % WINDOW_LENGTH]);
  }
}

VOID Routine(RTN rtn, VOID *v) {
  
  global_routine = rtn;
  //open routine to read instructions
  RTN_Open(rtn);

  //each instruction that is a memory reference triggers data write
  for(INS ins = RTN_InsHead(rtn); INS_Valid(ins); ins = INS_Next(ins)) {
  
    if(INS_IsMemoryRead(ins) || INS_IsMemoryWrite(ins)) {
      INS_InsertPredicatedCall(ins, IPOINT_BEFORE, (AFUNPTR)printTrace, IARG_END);
    }
  }

  RTN_Close(rtn);
  appendRTN(rtn);
}


VOID Fini(INT32 code, VOID *v) {

  fprintf(outfile, "#eof\n");
  fclose(outfile);

}

int main(int argc, char *argv[]) {
 
  PIN_InitSymbols(); 
  PIN_Init(argc, argv);

  outfile = fopen("cs_395t_rou.out", "w");

  RTN_AddInstrumentFunction(Routine, 0);
  PIN_AddFiniFunction(Fini, 0);

  //begin program
  PIN_StartProgram();  

  return 0;
}






