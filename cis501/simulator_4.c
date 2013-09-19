#define __STDC_FORMAT_MACROS
#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#define set 512
#define pred 65536

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
  
  //uint64_t cache[4]={0};
  
  uint64_t cache[set][2]={0};
  int LRU[set][2]={0};
  int dbit[set][2]={0};
  uint64_t index;
  uint64_t tag;
  int LS_num=0;
  int miss=0;
  int hit=0;
  int WT_store=0;
  int WT_traffic;
  int predictor[pred]={0};
  int entry;
  int mispred=0;
  
  
  
  
  //fprintf(outputFile, "Processing trace...\n");
  
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
	/*if(loadStore=='L'||loadStore=='S'){
	LS_num++;
	addressForMemoryOp >>=6;
	index=addressForMemoryOp%4;
	tag=addressForMemoryOp >>=2;
	if(cache[index]==tag){
	hit++;
	//fprintf(outputFile,"hit\n");
	}
	else{
	cache[index]=tag;
	miss++;
	//fprintf(outputFile,"miss\n");
	}
	}*/
	//for cache
	
	if(loadStore=='L'||loadStore=='S'){
	LS_num++;
	//if(loadStore=='S')WT_store++;
	addressForMemoryOp >>= 5;
	index = addressForMemoryOp%set;
	entry = addressForMemoryOp%pred;
	tag = addressForMemoryOp >>=9;
	if(cache[index][0]==tag){
	    if(predictor[entry]!=0){
		mispred++;
		predictor[entry]=0;
		}
		//fprintf(outputFile,"hit\n");
        hit++;
		LRU[index][0]=0;
		LRU[index][1]=1;
		if(loadStore=='S')dbit[index][0]=1;
	}
	else if(cache[index][1]==tag){
	     if(predictor[entry]!=1){
		    mispred++;
			predictor[entry]=1;
		 }
		 //fprintf(outputFile,"hit\n");
	     hit++;
		 LRU[index][0]=1;
		 LRU[index][1]=0;
		 if(loadStore=='S')dbit[index][1]=1;
	}
	else{
	miss++;
	//fprintf(outputFile,"miss ");
	if(LRU[index][0]==0){
	    cache[index][1]=tag;
		predictor[entry]=1;
		LRU[index][0]=1;
		LRU[index][1]=0;
		if(dbit[index][1]==1){
		WT_store++;
		//fprintf(outputFile,"dirty ");
		}
		if(loadStore=='S')dbit[index][1]=1;
		else dbit[index][1]=0;
	}
	else{
	   cache[index][0]=tag;
	   predictor[entry]=0;
	   LRU[index][0]=0;
	   LRU[index][1]=1;
	   if(dbit[index][0]==1){
	   WT_store++;
	   	//fprintf(outputFile,"dirty ");
	   }
	   if(loadStore=='S')dbit[index][0]=1;
	   else dbit[index][0]=0;
	}
	//fprintf(outputFile,"\n");
	}
	}
	
	WT_traffic=(miss+WT_store)*32;
	

    // For each micro-op
    totalMicroops++;

    // For each macro-op:
    if (microOpCount == 1) {
      totalMacroops++;
    }
  }
  fprintf(outputFile, "mispredictor: %d\n", mispred);
  fprintf(outputFile, "traffic: %d\n", WT_traffic);
  fprintf(outputFile, "total: %d\n", LS_num);
  fprintf(outputFile, "miss: %d\n", miss);
  fprintf(outputFile, "hit: %d\n", hit);
  fprintf(outputFile, "Processed %" PRIi64 " trace records.\n", totalMicroops);

  fprintf(outputFile, "Micro-ops: %" PRIi64 "\n", totalMicroops);
  fprintf(outputFile, "Macro-ops: %" PRIi64 "\n", totalMacroops);

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
