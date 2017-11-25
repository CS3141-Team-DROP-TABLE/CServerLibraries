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
typedef void (*all_func)(void *, void *, void *);

void avl_init(struct tree *t);
int avl_insert(struct tree *t, void *key, void *value, cmp_func cmp, size_t cmp_sz);
void* avl_remove(struct tree *t, void *key, cmp_func cmp, size_t cmp_sz);
void *avl_search(struct tree *t, void *key, cmp_func cmp, size_t cmp_sz);
int avl_verify_tree(struct tree *t, cmp_func cmp, size_t cmp_sz);
void avl_apply_to_all(struct tree *t, all_func fn, void *args);
void avl_clear_tree(struct tree *t, all_func del);
#endif
