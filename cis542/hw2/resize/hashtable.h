// the initial capacity (number of buckets)
#define MAXLOAD 6
typedef struct Node node;
struct Node {
  char *value;
  node *next; // next node in this bucket
  node *ordered_next; // next node in the linked list going through the hashtable
};

typedef struct Hashtable hashtable;
struct Hashtable {
  node *list[40000]; // "buckets" of linked lists
  node *ordered_head; // for the "linked hashtable" implementation
  node *iterator; // keeps track of which node we're looking at
};

