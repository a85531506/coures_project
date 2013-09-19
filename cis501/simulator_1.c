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

  int32_t count = 0;
  int32_t p[6]={0};
  int32_t i;
  uint64_t totle_size=0;
  uint64_t max_size=0;
  uint64_t size[12]={0};
  float avg_size;
  int64_t maxoff=0;
  int64_t off_bit=0;
  int64_t offsize[24]={0};
  int offcount=0;

  int loads=0;
  int stores=0;
  int ncon=0;
  int con=0;
  int others=0;
  int fusion=0;
  bool flag=false;

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

    // For each macro-op:
    if (microOpCount == 1) {
      totalMacroops++;
	  totle_size += fallthroughPC-instructionAddress;
      size[fallthroughPC-instructionAddress-1]++;
    }

	//statistic microOps
	  if(microOpCount>count)count=microOpCount;
	  if((fallthroughPC-instructionAddress)>max_size)max_size=fallthroughPC-instructionAddress;
	p[microOpCount-1]++;
//statistic offsize
if(targetAddressTakenBranch){
off_bit=instructionAddress-targetAddressTakenBranch;
off_bit=abs(off_bit);
off_bit=log(off_bit)/log(2);
off_bit=2+floor(off_bit);
if(off_bit>maxoff)maxoff=off_bit;
offsize[off_bit-1]++;
offcount++;}
//catetory:
if(loadStore=='L')loads++;
else if(loadStore=='S')stores++;
else if((targetAddressTakenBranch!=0)&&(conditionRegister=='-'))ncon++;
else if((targetAddressTakenBranch!=0)&&(conditionRegister=='R'))con++;
else others++;
//Op fusion
if((targetAddressTakenBranch!=0)&&(conditionRegister=='R')&&(flag==true))fusion++;
flag=false;
if(conditionRegister=='W')flag=true;
  }
  avg_size=totle_size/totalMacroops;
  for(i=0;i<5;i++)p[i]=p[i]-p[i+1];
  for(i=1;i<23;i++)offsize[i]=offsize[i]+offsize[i-1];
  
  fprintf(outputFile, "Processed %" PRIi64 " trace records.\n", totalMicroops);

  fprintf(outputFile, "Micro-opsss: %" PRIi64 "\n", totalMicroops);
  fprintf(outputFile, "Macro-ops: %" PRIi64 "\n", totalMacroops);
  for(i=0;i<6;i++){
	  fprintf(outputFile,"macroOp with %"PRIi32" microOp: %"PRIi32"    ",i+1,p[i]);
  }
  fprintf(outputFile,"average instruction size: %f\n",avg_size);
  fprintf(outputFile,"max size: %"PRIi64"\n", max_size);
  for(i=0;i<12;i++){
  fprintf(outputFile,"instruction of size %"PRIi32": %"PRIi64"     ",i+1,size[i]);
}
fprintf(outputFile,"maxoff: %"PRIi64"\n",maxoff);
fprintf(outputFile,"count: %d\n",offcount);
for(i=0;i<23;i++){
fprintf(outputFile,"offset with %"PRIi32"bits : %"PRIi64"     ",i+1,offsize[i]);
}
fprintf(outputFile,"loads: %d    ",loads);
fprintf(outputFile,"stores: %d    ",stores);
fprintf(outputFile,"unconditional branched: %d    ",ncon);
fprintf(outputFile,"conditional branchers: %d    ",con);
fprintf(outputFile,"others: %d    ",others);
fprintf(outputFile,"pairs of fusion: %d     ",fusion);
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
