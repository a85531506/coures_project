/*
 * To add a string S to hashtable H, call hashtable_put(H, S), where H is a 
 * pointer to a variable of type hashtable and S is a char*. It will return 
 * 1 if S was added succesfully, and 0 if not (note that duplicates are not
 * allowed, so it will return 0 if S is already in the hashtable).
 *
 * To find a string S in hashtable H, call hashtable_get(H, S). It will return
 * 1 if S is in the hashtable, and 0 if it is not.
 *
 * This implementation has an iterator. First, call reset_iterator(H) to point
 * the iterator to the right place. Then, each time you call iterate(H), it
 * will return the char* held at the next node in the hashtable; the function 
 * will return null when you get to the end.
 *
 * Unlike the Java implementation, this hashtable does not automatically resize 
 * itself. The size of the hashtable is specified by the CAPACITY constant
 * defined below, and all hashtables have the same size.
 * 
 * For questions or problems with this implementation, contact a member of
 * the instruction staff.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"


/*
 * Reset the iterator pointer to the beginning of the linked list that
 * runs throughout the hashtable.
 */
void reset_iterator(hashtable *h) {
  h->iterator = h->ordered_head;
}
void reset_inite(hashtable *h){
  node *n=h->ordered_head;
  while(n->ordered_next!=NULL)n=n->ordered_next;
  h->inite=n;
}

/*
 * Return the value stored in the node and advance the iterator pointer
 * to the next element in the linked list. Returns NULL when the end of
 * the list is reached.
 */
char* iterate(hashtable *h) {
  if (h->iterator == NULL) return NULL;
  else {
    char *s = h->iterator->value;
    //printf("The next value is %s\n", h->iterator->value);
    h->iterator = h->iterator->ordered_next;
    return s;
  }
}
char *initerate(hashtable *h){
     if(h->inite ==NULL)return NULL;
     else{
       char *s=h->inite->value;
       h->inite = h->inite->inorder;
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
    printf("Error! Could not calculate hashcode\n");
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
  if(h->ordered_head!=NULL)h->ordered_head->inorder=new;
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
int get1(hashtable *h, char *string)
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



