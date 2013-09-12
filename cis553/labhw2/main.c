#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"hashtable.h"

FILE *infile;
int file_initialize(char* name)
{
  if (name == NULL){
     printf("Error! File name provided to file_initialize is null\n");
     return 0;
   }
   infile = fopen(name, "r");
   if (infile == NULL){
     printf("Error! Could not open file \"%s\" for reading\n", name);
     return 0;
   }
   else return 1;
}
char file_word[257];
char* file_read_next()
{
   if (fscanf(infile, "%s", file_word) == EOF) return NULL;
   else return file_word;
}



    /*
     * Compare two files to see how many words they have in common.
     */
int compareFiles(char* file0, char* file1) {

	int count = 0; // track the number of words in common
	char* nextword;
        char* compare;

	// to hold the distinct words for each file
        hashtable table0;
        hashtable table1;
	    // read first file into a ArrayList
        if (file_initialize(file0) == 0){
            printf("Error! Open file failure.");
            exit(1);
            }
        else{
	    printf("Reading first book...");
	    while ((nextword = file_read_next()) != NULL) {
		  put(&table0, nextword);
	    }
	    printf("done.");
          }
	    //in.close();

	    // read second file into an ArrayList
         if (file_initialize(file1) == 0){
            printf("Error! Open file failure.");
            exit(1);
            }
         else{
	    printf("Reading second book...");
	    while ((nextword = file_read_next()) != NULL) {
	          put(&table1, nextword);
	    }
	    printf("done.");
          }
	    // now compare the lists one word at a time
	  printf("Comparing documents...");
          reset_iterator(&table0);
            while((compare = iterate(&table0)) != NULL){
                if(get(&table1, compare)){
                  count++;
                }
            }
	return count;
    }


void main(int argc, char* args[])
{
   char* file0 = "war.txt";
   char* file1 = "anna.txt";
   //long startTime, endTime, totalTime;
   int words;
	//if (argc < 2) {
	//    printf("Please specify the names of the files!");
	//    exit(1);
	//}

	// get the filenames
	//file0 = args[1];
	//file1 = args[2];
	//startTime = System.nanoTime();
    words = compareFiles(file0, file1);
	printf("The files have %d words in common", words);
        //endTime = System.nanoTime();
        //totalTime = endTime - startTime;
        //System.out.println("The total time is " + totalTime);
}




