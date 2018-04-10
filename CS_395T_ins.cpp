#include <stdio.h>
#include "pin.H"

#define WINDOW_LENGTH 100

void appendIP(VOID *ip);
void printTrace();
VOID Instruction(INS ins, VOID *v);

//out file
FILE* outfile;

//sliding window of instruction addresses
static int COUNTER = 0;
void* ip_window[WINDOW_LENGTH];

void appendIP(VOID *ip) {
  ip_window[(COUNTER++) % WINDOW_LENGTH] = ip;
}

void printTrace() {

  fprintf(outfile, "MEMORY ACCESS:\n");
  if(COUNTER <= WINDOW_LENGTH) {

    for(int i = 0; i < COUNTER; i++)
      fprintf(outfile, "%p\n", ip_window[i]);

  } else {
  
    for(int i = COUNTER; i < COUNTER + WINDOW_LENGTH; i++)
      fprintf(outfile, "%p\n", ip_window[i % WINDOW_LENGTH]);
  }
}

VOID Instruction(INS ins, VOID *v) {
  
  //insert ip into sliding window for ALL instructions
  INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)appendIP, IARG_INST_PTR, IARG_END);

  if(INS_IsMemoryRead(ins) || INS_IsMemoryWrite(ins)) {
    INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)printTrace, IARG_END);
  }
}


VOID Fini(INT32 code, VOID *v) {

  fprintf(outfile, "#eof\n");
  fclose(outfile);

}

int main(int argc, char *argv[]) {
  
  PIN_Init(argc, argv);
  outfile = fopen("cs_395t_ins.out", "w");

  INS_AddInstrumentFunction(Instruction, 0);
  PIN_AddFiniFunction(Fini, 0);
  
  //begin program
  PIN_StartProgram();  

}






