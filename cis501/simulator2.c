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
  
  int CHcounter[262144] = {0};
  int BMcounter[262144] = {0};
  int GHcounter[524288] = {0};
  int CHindex;
  int BMindex;
  int GHindex;

  int BH = 524287;
  int i;
  
  
  fprintf(outputFile, "Processing trace...\n");
  
  while (totalCondBranches<15) {
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
	//for(i=0;i<8;i++)fprintf(outputFile, "%d", CHcounter[i]);
	//fprintf(outputFile, "   ");
	//for(i=0;i<8;i++)fprintf(outputFile, "%d", BMcounter[i]);
	//fprintf(outputFile, "   ");
    //for(i=0;i<16;i++)fprintf(outputFile, "%d", GHcounter[i]);
	//fprintf(outputFile, "   ");
	//fprintf(outputFile, "%d", BH);
	//fprintf(outputFile, "\n");
	//totalCondBranches++;

	char pred;
	char BMpred;
	char GHpred;
	//static predict
	//pred = 'T'
	//bimodal predict
	//index = instructionAddress%65536;
	CHindex = instructionAddress%262144;
	BMindex = instructionAddress%262144;
	GHindex = (instructionAddress^BH)%524288;
	if (BMcounter[BMindex] < 2)BMpred = 'N';
	else BMpred = 'T';
	if (GHcounter[GHindex] < 2)GHpred = 'N';
	else GHpred = 'T';
	if (CHcounter[CHindex] < 2)pred = BMpred;
	else pred = GHpred;
	if (pred == TNnotBranch)correct++;
	else incorrect++;
	if ((BMpred == TNnotBranch)&&(GHpred != TNnotBranch)){
		CHcounter[CHindex]--;
		if (CHcounter[CHindex]<0)CHcounter[CHindex]=0;
	}
	if((BMpred != TNnotBranch)&&(GHpred == TNnotBranch)){
		CHcounter[CHindex]++;
		if (CHcounter[CHindex]>3)CHcounter[CHindex]=3;
	}
	if(TNnotBranch == 'N'){
		GHcounter[GHindex]--;
		if (GHcounter[GHindex]<0)GHcounter[GHindex]=0;
		BH = BH * 2 + 1;
		BMcounter[BMindex]--;
		if (BMcounter[BMindex]<0)BMcounter[BMindex]=0;
	}
	else{
		GHcounter[GHindex]++;
		if (GHcounter[GHindex]>3)GHcounter[GHindex]=3;
		BH = BH * 2;
		BMcounter[BMindex]++;
		if (BMcounter[BMindex]>3)BMcounter[BMindex]=3;
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
