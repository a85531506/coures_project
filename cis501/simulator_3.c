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
  int64_t totalCondBranches = 0;

  int correct = 0;
  int incorrect = 0;
  ////////////////////////////////////before q5
  
  int counter[1048576] = {0};
  int index;

  int BH = 1048575;
  int i;
  
  
  fprintf(outputFile, "Processing trace...\n");
  
  while (true) {
    int result = fscanf(inputFile, 
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

	if (TNnotBranch == '-') {
      continue;  // Not a branch
    }

	if (conditionRegister != 'R') {
      continue;  // Not a conditional branch
    }
    //for(i=0;i<16;i++)fprintf(outputFile, "%d", counter[i]);
	//fprintf(outputFile, "   ");
	//fprintf(outputFile, "%d", BH);
	//fprintf(outputFile, "\n");
	totalCondBranches++;

	char pred;
	//static predict
	//pred = 'T'

	//bimodal predict
	//index = instructionAddress%65536;
	index = (instructionAddress^BH)%1048576;
	if (counter[index] < 2)pred = 'N';
	else pred = 'T';

	if (pred == TNnotBranch) {
      correct++;
	  if (pred == 'T'){
		  counter[index] += 1;
		  BH = BH * 2;
		  //BH |= 1020;
		  if (counter[index] > 3)counter[index] = 3;
	  }
	  else{
		  counter[index] -= 1;
		  BH = BH * 2 + 1;
		  //BH |= 1020;
		  //BH += 1;
		  if(counter[index] < 0)counter[index] = 0;
	  }
    } 
	else {
      incorrect++;
	  if(pred == 'N'){
		  counter[index] += 1;
		  BH = BH * 2;
		  //BH |= 1020;
	  }
	  else{
		  counter[index] -= 1; 
		  BH = BH * 2 + 1;
		  //BH |= 1020;
		  //BH += 1;
	  }
    }


  }
  
  fprintf(outputFile, "Processed %" PRIi64 " trace records.\n", totalMicroops);

  fprintf(outputFile, "Micro-ops: %" PRIi64 "\n", totalMicroops);
  fprintf(outputFile, "totalCondBranch: %" PRIi64 "\n", totalCondBranches);
  fprintf(outputFile, "correct: %d\n", correct);
  fprintf(outputFile, "incorrect: %d\n", incorrect);
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
