#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<pthread.h>
#include<unistd.h>
#include"hashtable.h"

pthread_t thread0, thread1, thread2, thread3;
hashtable *table0;// = (hashtable *)malloc(sizeof(hashtable));
hashtable *table1; //= (hashtable *)malloc(sizeof(hashtable));
char* file0;// = "war.txt";
char* file1;// = "anna.txt";
FILE *infile0, *infile1;
int count = 0;
pthread_mutex_t mutex;


int file_initialize0(char* name)
{
  if (name == NULL){
     printf("Error! File name provided to file_initialize is null\n");
     return 0;
   }
   infile0 = fopen(name, "r");
   if (infile0 == NULL){
     printf("Error! Could not open file \"%s\" for reading\n", name);
     return 0;
   }
   else return 1;
}
char file_word0[257];
char* file_read_next0()
{
   if (fscanf(infile0, "%s", file_word0) == EOF) return NULL;
   else return file_word0;
}
int file_initialize1(char* name)
{
  if (name == NULL){
     printf("Error! File name provided to file_initialize is null\n");
     return 0;
   }
   infile1 = fopen(name, "r");
   if (infile1 == NULL){
     printf("Error! Could not open file \"%s\" for reading\n", name);
     return 0;
   }
   else return 1;
}
char file_word1[257];
char* file_read_next1()
{
   if (fscanf(infile1, "%s", file_word1) == EOF) return NULL;
   else return file_word1;
}
    /*
     * Compare two files to see how many words they have in common.
     */
void *input0()
{
    char *next;

    if (file_initialize0(file0) == 0){
            printf("Error! Open file failure.");
            exit(1);
            }
    printf("Reading first book...\n");
   while ((next = file_read_next0()) != NULL) {
         put(table0, next);
   }
   printf("done.\n");
   pthread_exit(NULL);
}
void *input1()
{
    char *next;

    file_initialize1(file1);
    printf("Reading second book...\n");
   while ((next = file_read_next1()) != NULL) {
         put(table1, next);
   }
   printf("second book done.\n");
   pthread_exit(NULL);
}
void *comp0()
{
   char* compare;
   char* iterate(hashtable *h);
   reset_iterator(table0);
   int i=0;
   while((compare = iterate(table0)) != NULL){
        if(get(table1, compare)){
           pthread_mutex_lock(&mutex);
                  count++;
           pthread_mutex_unlock(&mutex);
        }
        i++;
        if(i==8945)break;
   }
   pthread_exit(NULL);
}
void *comp1()
{
   char* compare;
   char* initerate(hashtable *h);
   reset_inite(table0);
   while((compare = initerate(table0)) != NULL){
        if(get1(table1, compare)){
                  pthread_mutex_lock(&mutex);
                  count++;
                  pthread_mutex_unlock(&mutex);
        }
   }
   pthread_exit(NULL);
}
int compareFiles(char* file0, char* file1) {

	//int count = 0; // track the number of words in common
	char* next;
        char* compare;
        table0 = (hashtable *)malloc(sizeof(hashtable));
        table1 = (hashtable *)malloc(sizeof(hashtable));
        //table2 = (hashtable *)mallox(sizeof(hashtable));
        int exit1, exit2;
        char* initerate(hashtable *h);
        int i=0;
        pthread_create(&thread1, NULL, input1, NULL);
	// to hold the distinct words for each file
	    // read first file into a ArrayList
        if (file_initialize0(file0) == 0){
            printf("Error! Open file failure.");
            exit(1);
            }
        else{
	    printf("Reading first book...\n");
	    while ((next = file_read_next0()) != NULL) {
		  put(table0, next);
	    }
	    printf("first book done.\n");
          }
	    //in.close();

	    // read second file into an ArrayList
         /*
         if (file_initialize1(file1) == 0){
            printf("Error! Open file failure.");
            exit(1);
            }
         else{
	    printf("Reading second book...\n");
	    while ((next = file_read_next1()) != NULL) {
	          put(table1, next);
	    }
	    printf("done.\n");
          }*/
          //pthread_create(&thread0, NULL, input0, NULL);

          //while((exit1=pthread_join(thread0, NULL)!=0)||
           while((exit2=pthread_join(thread1, NULL)!=0));
	    // now compare the lists one word at a time
	  printf("Comparing documents...\n");
          reset_inite(table0);
           while((compare = initerate(table0)) != NULL){
                if(get(table1, compare)){
                  pthread_mutex_lock(&mutex);
                  count++;
                  pthread_mutex_unlock(&mutex);
                }
                //i++;
                //if(i==8945)break;
            }
          //pthread_create(&thread2, NULL, comp0, NULL);
          //pthread_create(&thread3, NULL, comp1, NULL);
          //while((exit1=pthread_join(thread2, NULL))!=0);
          //while((exit2=pthread_join(thread3, NULL))!=0);
	return count;
    }

void main(int argc, char* args[])
{
   int words;
   //char* iterate(hashtable *h);
   //clock_t start, end;
   double total;
   //int put(hashtable *h, char *string);
	if (argc < 2) {
	    printf("Please specify the names of the files!");
	    exit(1);
	}

	// get the filenames
	file0 = args[1];
	file1 = args[2];
	//start = clock();
    words = compareFiles(file0, file1);
	printf("The files have %d words in common\n", words);
        //end = clock();
        //total = (double)(end - start)/CLOCKS_PER_SEC;
        //printf("The total time is %fs\n",total);
}






