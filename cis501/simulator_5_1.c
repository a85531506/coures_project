#define __STDC_FORMAT_MACROS
#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

void simulate(FILE* inputFile, FILE* outputFile)
{
  // See the documentation to understand what these variables mean.
  int32_t microOpCount;
  uint64_t instructionAddress;
  int32_t sourceRegister1;
  int32_t sourceRegister2;
  int32_t destinationRegister;
  char conditionRegister;
  char TNnotBranch;
  char loadStore;
  int64_t immediate;
  uint64_t addressForMemoryOp;
  uint64_t fallthroughPC;
  uint64_t targetAddressTakenBranch;
  char macroOperation[12];
  char microOperation[23];

  int64_t totalMicroops = 0;
  int64_t totalMacroops = 0;
  
  ////////////////////////////////////////////////////////////////////////////
  bool next_instruction = true;
  int flag[50] = {0};
  int64_t cycles = 0;
  int i,j;
  bool sr1_ready, sr2_ready, dr_ready, flag_ready;
  int result;
  ////////////////////////////////////////////////////////////////////////////
  
  fprintf(outputFile, "Processing trace...\n");
  
  while (totalMicroops<10000) {
    	for (i=0; i<3; i++){


		    if(next_instruction){
    result = fscanf(inputFile, 
                        "%" SCNi32
                        "%" SCNx64 
                        "%" SCNi32
                        "%" SCNi32
                        "%" SCNi32
                        " %c"
                        " %c"
                        " %c"
                        "%" SCNi64
                        "%" SCNx64
                        "%" SCNx64
                        "%" SCNx64
                        "%11s"
                        "%22s",
                        &microOpCount,
                        &instructionAddress,
                        &sourceRegister1,
                        &sourceRegister2,
                        &destinationRegister,
                        &conditionRegister,
                        &TNnotBranch,
                        &loadStore,
                        &immediate,
                        &addressForMemoryOp,
                        &fallthroughPC,
                        &targetAddressTakenBranch,
                        macroOperation,
                        microOperation);
                        
    if (result == EOF) {
      break;
    }

    if (result != 14) {
      fprintf(stderr, "Error parsing trace at line %" PRIi64 "\n", totalMicroops);
      abort();
    }

    // For each micro-op
    totalMicroops++;

    // For each macro-op:
    if (microOpCount == 1) {
      totalMacroops++;
    }


		     next_instruction = false;
			 sr1_ready = false;
			 sr2_ready = false;
			 dr_ready = false;
			 flag_ready = false;
		}
		if(sourceRegister1 == -1)sr1_ready = true;
		else if(flag[sourceRegister1]==0)sr1_ready = true;
	    if(sourceRegister2 == -1)sr2_ready = true;
		else if(flag[sourceRegister2]==0)sr2_ready = true;
		if(destinationRegister == -1)dr_ready = true;
		else if(flag[destinationRegister]==0)dr_ready = true;
		if(conditionRegister != 'R' && conditionRegister != 'W')flag_ready = true;
		else if(flag[49]==0)flag_ready = true;
		if(sr1_ready && sr2_ready &&dr_ready && flag_ready){
		    if(loadStore == 'L')flag[destinationRegister] = 2;
			else if(destinationRegister != -1)flag[destinationRegister]=1;
			if(conditionRegister == 'W')flag[49] = 1;
			next_instruction = true;
		}
			fprintf(outputFile, "%" PRIi64"  ", cycles);
	fprintf(outputFile, "%" PRIi64"  ", totalMicroops);
	for(j=0;j<50;j++){
	     fprintf(outputFile, "%d", flag[j]);
	}
	fprintf(outputFile, "\n");
		if(!(sr1_ready && sr2_ready &&dr_ready && flag_ready)) break;
	}
	if(result == EOF)break;
	cycles++;
	for(i=0; i<50; i++){
	    if(flag[i] > 0)flag[i]--;
	}
  }
  
  fprintf(outputFile, "Processed %" PRIi64 " trace records.\n", totalMicroops);

  fprintf(outputFile, "Micro-ops: %" PRIi64 "\n", totalMicroops);
  fprintf(outputFile, "Macro-ops: %" PRIi64 "\n", totalMacroops);
  fprintf(outputFile, "TotalCycle: %" PRIi64 "\n", cycles);
}

int main(int argc, char *argv[]) 
{
  FILE *inputFile = stdin;
  FILE *outputFile = stdout;
  
  if (argc >= 2) {
    inputFile = fopen(argv[1], "r");
    assert(inputFile != NULL);
  }
  if (argc >= 3) {
    outputFile = fopen(argv[2], "w");
    assert(outputFile != NULL);
  }
  
  simulate(inputFile, outputFile);
  return 0;
}
