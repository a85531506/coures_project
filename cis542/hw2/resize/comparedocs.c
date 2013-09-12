#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
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
	char* next;
        char* compare;
        char* iterate(hashtable* h);
        int capacity1 = 0;
        int capacity2 = 0;
        int temp;
	// to hold the distinct words for each file
        hashtable *table0 = (hashtable *)malloc(sizeof(hashtable));
        hashtable *table1 = (hashtable *)malloc(sizeof(hashtable));
	    // read first file into a ArrayList
        if (file_initialize(file0) == 0){
            printf("Error! Open file failure.");
            exit(1);
            }
        else{
	    printf("Reading first book...\n");
	    while ((next = file_read_next()) != NULL) {
		  temp = put(table0, next);
                  if(temp>capacity1)capacity1=temp;
	    }
	    printf("done.\n");
            printf("new capacity is %d\n", capacity1);
          }
	    //in.close();

	    // read second file into an ArrayList
         if (file_initialize(file1) == 0){
            printf("Error! Open file failure.");
            exit(1);
            }
         else{
	    printf("Reading second book...\n");
	    while ((next = file_read_next()) != NULL) {
	          temp =put(table1, next);
                  if(temp>capacity2)capacity2 = temp;
	    }
	    printf("done.\n");
            printf("new capacity is %d\n", capacity2);
          }
	    // now compare the lists one word at a time
	  printf("Comparing documents...\n");
          reset_iterator(table0);
            while((compare = iterate(table0)) != NULL){
                if(get(table1, compare)){
                  count++;
                }
            }
	return count;
    }

void main(int argc, char* args[])
{
   char* file0;// = "war.txt";
   char* file1;// = "anna.txt";
   int words;
   //char* iterate(hashtable *h);
   clock_t start, end;
   double total;
   //int put(hashtable *h, char *string);
	if (argc < 2) {
	    printf("Please specify the names of the files!");
	    exit(1);
	}

	// get the filenames
	file0 = args[1];
	file1 = args[2];
	start = clock();
    words = compareFiles(file0, file1);
	printf("The files have %d words in common\n", words);
        end = clock();
        total = (double)(end - start)/CLOCKS_PER_SEC;
        printf("The total time is %fs\n",total);
}






