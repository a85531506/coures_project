#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

#define CAPACITY 16

typedef struct Node node;
struct Node {
  char *value;
  node *next; // next node in this bucket
  node *ordered_next; // next node in the linked list going through the hashtable
  int count;
};

typedef struct Hashtable hashtable;
struct Hashtable {
  node *list[CAPACITY]; // "buckets" of linked lists
  node *ordered_head; // for the "linked hashtable" implementation
  node *iterator; // keeps track of which node we're looking at
};

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
 * Reset the iterator pointer to the beginning of the linked list that
 * runs throughout the hashtable.
 */
void reset_iterator(hashtable *h) {
  h->iterator = h->ordered_head;
}

/*
 * Return the value stored in the node and advance the iterator pointer
 * to the next element in the linked list. Returns NULL when the end of
 * the list is reached.
 */
node* iterate(hashtable *h) {
  if (h->iterator == NULL) return NULL;
  else {
    node *s = h->iterator;
    //printf("The next value is %s\n", h->iterator->value);
    h->iterator = h->iterator->ordered_next;
    return s;
  }
}


long long power(long a, long b) {
  long long retval = 1;
  int i;
  for (i = 0; i < b; i++) {
    retval *= a;
  }
  return retval;
}

/*
 * For the given string refered to by the pointer "string",
 * calculate the hashcode and update the int "value".
 *
 * Return 1 if successful, and 0 if an error occurred
 * (e.g. if the string is null).
 */
int hash(char *string, long long *value)
{
  if (string == NULL) {
    printf("Error! Attempt to calculate hashcode of null string\n");
    return 0;
  }
  *value = 0;
  int i;
  int n = strlen(string);
  if (n > 10) n = 10; // only consider the first 10 chars, so we don't overflow
  for (i = 0; i < n; i++) {
    //printf("considering %c with value %d\n", string[i], (int)(string[i]));
    long long temp = (int)(string[i]) * power(31, n-i-1);
    //printf("temp is %ld\n", temp);
    *value += temp;
    //printf("value is %ld\n", *value);
  }
  //printf("hashvalue for %s is %ld\n", string, *value);
  if (*value < 0) {
    //printf("Overflow occurred in calculating the hashcode for \"%s\"\n", string);
    *value = *value * -1;
    //return 0;
  }
  return 1;
}

/*
 * Add the string to the hashtable in the appropriate "bucket".
 *
 * Return 1 if successful, and 0 if an error occurred
 * (e.g. if the string is null or if the entry already exists).
 */
int put(hashtable *h, char *string)
{
  if (string == NULL) {
    printf("Error! Attempt to put null string in hashtable\n");
    return 0;
  }
  // get the hashvalue
  long long hashvalue;
  if (hash(string, &hashvalue) == 0) {
    printf("Error! Could not calculate hashcode \n");
    return 0;
  }

  // get the index
  int index = hashvalue % CAPACITY;
  //printf("index for %s is %d\n", string, index);

  // before adding it, see if it's already there
  // get the head of the linked list
  node *n = h->list[index];

  while (n != NULL && n->value != NULL)
    {
      //printf("Looking at %s\n", n->value);
      if (strcmp(n->value, string) == 0) {
	//printf("already had %s in bucket %d.\n", string, index);
        n->count++;
	return 0;
      }
      else n = n->next;
    }

  // if we get here, it's not in the list
  //printf("trying to insert %s in bucket %d\n", string, index);

  // get the head of the linked list for this bucket
  node *head = h->list[index];
  node *new = (node *)malloc(sizeof(node));
  if (new == NULL) {
    printf("Error! Could not allocate memory to insert string\n");
    return 0;
  }
  new->value = (char *)malloc(sizeof(char)*(strlen(string)+1));
  strcpy(new->value, string);
  new->next = head;
  h->list[index] = new;
  //printf("successfully added %s in bucket #%d\n", string, index);

  // now update the linked list that goes through the hashtable
  new->ordered_next = h->ordered_head;
  h->ordered_head = new;

  return 1;
}

/*
 * Determine whether the specified string is in the hashtable.
 * Return 1 if it is found, 0 if it is not (or if it is null).
 */
int get(hashtable *h, char *string)
{
  if (string == NULL) {
    printf("Error! Attempt to get null string from hashtable\n");
    return 0;
  }

  // get the hashvalue
  long long hashvalue;
  if (hash(string, &hashvalue) == 0) {
    printf("Error! Could not calculate hashcode\n");
    return 0;
  }

  // get the index
  int index = hashvalue % CAPACITY;

  // get the head of the linked list
  node *n = h->list[index];

  while (n != NULL)
    {
      //printf("Looking at %s\n", n->value);
      if (strcmp(n->value, string) == 0) return 1;
      else n = n->next;
    }
  return 0; // not found!
}

    /*
     * Compare two files to see how many words they have in common.
     */
int compareFiles(char* file0, char* file1) {

	int count = 0; // track the number of words in common
	char* next;
        node* compare;
        int occur[10];
        char* words[10];
        int i,j;
        for(i=0;i<10;i++){
           occur[i]=0;
           words[i]=" ";
        }
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
		  put(table0, next);
	    }
	    printf("done.\n");
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
	          put(table0, next);
	    }
	    printf("done.\n");
          }
	    // now compare the lists one word at a time
	  printf("Comparing documents...\n");
          reset_iterator(table0);
            while((compare = iterate(table0)) != NULL){
                if(compare->count > occur[9]){
                  for(i=0;i<10;i++){
                     if(compare->count>occur[i]){
                          for(j=8;j>=i;j--){
                              occur[j+1]=occur[j];
                              words[j+1]=words[j];
                          }
                          occur[i]=compare->count;
                          words[i]=compare->value;
                          break;
                     }
                  }
                  //printf("%s:%d\n",compare->value,compare->count);
                }
            }
        for(i=0;i<10;i++)
            printf("%s:%d\n",words[i], occur[i]);
	return 0;
    }


void main(int argc, char* args[])
{
   char* file0;// = "war.txt";
   char* file1;// = "anna.txt";
   int words;
   node* iterate(hashtable *h);
   clock_t start, end;
   double total;
   int put(hashtable *h, char *string);
	if (argc < 2) {
	    printf("Please specify the names of the files!");
	    exit(1);
	}

	// get the filenames
	file0 = args[1];
	file1 = args[2];
	start = clock();
        words = compareFiles(file0, file1);
	//printf("The files have %d words in common\n", words);
        end = clock();
        total = (double)(end - start)/CLOCKS_PER_SEC;
        printf("The total time is %fs\n",total);
}




