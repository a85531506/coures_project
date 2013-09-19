#define __STDC_FORMAT_MACROS
#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "BimodalPredictor.h"
#include "GSharePredictor.h"
#include "TournamentPredictor.h"

void simulate(int32_t predictor_size, FILE* inputFile, FILE* outputFile, FILE* debugOutputFile, FILE* debugInputFile)
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
  int64_t totalCondBranches = 0;

  int correct = 0;
  int incorrect = 0;
  Predictor* predictor = new BimodalPredictor(predictor_size);
  // Predictor* predictor = new GSharePredictor(4, 3);
  // Predictor* predictor = new TournamentPredictor(3, new BimodalPredictor(3), new GSharePredictor(4, 4));

  fprintf(stderr, "Processing trace...\n");
  
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

    totalCondBranches++;

    if (debugOutputFile) { // debug output trace
      predictor->print(debugOutputFile);
    }
    
    char pred = predictor->makePrediction(instructionAddress, TNnotBranch);

    if (pred == TNnotBranch) {
      correct++;
    } else {
      incorrect++;
    }
    
    if (debugOutputFile) { // debug output trace
      fprintf(debugOutputFile, "| %" PRIx64 "  %c | %c  %s%d\n", 
              instructionAddress, 
              TNnotBranch, 
              pred,
              (pred == TNnotBranch) ? "correct    " : "incorrect  ",
              incorrect);
    }
    
    if (debugInputFile) {
      int expected_incorrect;
      fscanf(debugInputFile, "%*[^|] %*s %*s %*s %*s %*s %*s %d",  // Read many fields, throwing away all but the last
             &expected_incorrect);
      
      if (incorrect != expected_incorrect) {
        fprintf(stderr, "Mismatched mis-prediction count: %d vs %d\n", incorrect, expected_incorrect);
        abort();
      }
    }

  }
    
  fprintf(stderr, "Processed %" PRIi64 " trace records.\n", totalMicroops);
  
  std::string s = predictor->getConfig();
  fprintf(outputFile, "%45s %11i %11i %8.2f%%\n", 
          s.c_str(), correct, incorrect, 100.0*correct/(correct+incorrect));
  

}

int main(int argc, char *argv[]) 
{
  int32_t predictor_size = 1;
  FILE *inputFile = stdin;
  FILE *outputFile = stdout;
  FILE *debugInputFile = NULL;
  FILE *debugOutputFile = NULL;

  if (argc >= 2) {
    predictor_size = atoi(argv[1]);
  }
  
  if (argc >= 3) {
    debugOutputFile = fopen(argv[2], "w");
    assert(debugOutputFile != NULL);
  }

  if (argc >= 4) {
    debugInputFile = fopen(argv[3], "r");
    assert(debugInputFile != NULL);
  }

  simulate(predictor_size, inputFile, outputFile, debugOutputFile, debugInputFile);
  return 0;
}
