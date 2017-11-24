#ifndef AVL_H
#define AVL_H

#include <pthread.h>

struct tree_node{
  void *key;
  void *val;

  struct tree_node *parent;
  struct tree_node *left;
  struct tree_node *right;

  unsigned int height;

  
};

struct tree{
  struct tree_node *root;
  unsigned int size;

  pthread_mutex_t mut;
};


/**
 * return 0 if equal, less than 0 if arg 1 is less than 2 and greater than 0
 * if arg 1 is greater than arg 2
 */
typedef int (*cmp_func)(void *, void *, size_t cmp_sz);

void avl_init(struct tree *t);


//TMP
struct tree_node *create_node(void *key, void *val);
int insert(struct tree *t, struct tree_node *n, cmp_func cmp, size_t cmp_sz);

#endif
