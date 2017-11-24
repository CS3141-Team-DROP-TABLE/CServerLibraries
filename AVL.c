#include <stdlib.h>
#include <string.h>
#include <pthread.h>


#include <AVL.h>


void avl_init(struct tree *t){
  t->root = NULL;
  t->size = 0;

  t->mut = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
  pthread_mutex_unlock(&(t->mut));
}

/**
 * Swaps the keys and values in the nodes at two points
 */
void swap(struct tree_node *a, struct tree_node *b){
  void *aval;
  void *akey;

  aval = a->val;
  akey = a->key;

  a->val = b->val;
  a->key = b->key;
  
  b->val = aval;
  b->key = akey;
}


struct tree_node *create_node(void *key, void *val){
  struct tree_node *new = malloc(sizeof(struct tree_node));
  new->key = key;
  new->val = val;
  new->right = NULL;
  new->left = NULL;
  new->parent = NULL;
  new->height = 0;

  return new;
}

/**
 * Searches a tree for a key
 *
 * Args:
 * t: the tree
 * key: a pointer to the key
 * cmp: a pointer to a valid cmp_func compare function
 * cmp_sz: the size of the key to compare
 *
 * Returns a node with a matching key, or if none exists, the node above 
 * where it would be. Returns NULL for an empty tree
 *
 */
struct tree_node* search(struct tree *t, void *key, cmp_func cmp, size_t cmp_sz){
  struct tree_node *mk = t->root;
  struct tree_node *mk_lag = t->root;


  while(mk != NULL && cmp(mk->key, key, cmp_sz) != 0){
    mk_lag = mk;
    if(cmp(key, mk, cmp_sz) > 0)
      mk = mk->right;
    else
      mk = mk->left;
    
  }
    
  return (mk == NULL)? mk_lag : mk;
}


/**
 * Returns the max of the two args
 */
unsigned int max(unsigned int a, unsigned int b){
  return (a > b)? a : b;
}
/**
 * Returns the min of the two args
 */
unsigned int min(unsigned int a, unsigned int b){
  return (a < b)? a : b;
}
/**
 * Returns the difference of the two args
 */
unsigned int dif(unsigned int a, unsigned int b){
  return max(a,b) - min(a,b);
}




/**
 * Gets the height of a node, or 0 if null
 *
 * Args: 
 * n: the node to get the height of
 *
 * Returns:
 * The height of n, or 0 if n is null
 */
unsigned int height(struct tree_node *n){
  return (n != NULL)? n->height : 0;
}

/**
 * fixes the height of a node
 *
 * Args:
 * x: the node to be balanced
 */
void update_node_height(struct tree_node *x){
  x->height = max(height(x->left), height(x->right));
}

/**
 * changes x's parent's reference to x to be y. if x has no parent, sets
 * tree root to y
 *
 * Args:
 * x: the node to have it's parent altered
 * y: the new child of x's parent, should be one of x's children
 */
void change_parent_child(struct tree *t, struct tree_node *x, struct tree_node *y) {
  if(x->parent == NULL){
    t->root = y;
  } else {
    if(x->parent->left == x)
      x->parent->left = y;
    else
      x->parent->right = y;
  }

}

/**
 * Rotates the tree to the left, I.E. the right subtree of x is too big
 * 
 * Args:
 * x: is the root of the tree to balance
 *
 * Returns:
 * The root of the balanced tree, NULL if the programmer calling is an idiot
 *
 */

struct tree_node *rotate_left(struct tree * t, struct tree_node *x){
  struct tree_node *retval = x->right;

  if(retval != NULL){ 
    change_parent_child(t, x, x->right);
    struct tree_node *rl = x->right->left;
    x->right->left = x;
    x->right = rl;
    x->parent = x->right;

    
    update_node_height(x);
    update_node_height(x->parent);

  }
  return retval;
}

/**
 * Rotates the tree to the right, I.E. the left subtree of x is too big
 * 
 * Args:
 * t: the tree struct
 * x: is the root of the tree to balance
 *
 * Returns:
 * The root of the balanced tree, NULL if the programmer calling is an idiot
 *
 */

struct tree_node *rotate_right(struct tree * t, struct tree_node *x){
  struct tree_node *retval = x->left;

  if(retval != NULL){ 
    change_parent_child(t, x, x->left);
    struct tree_node *lr = x->left->right;
    x->left->right = x;
    x->left = lr;
    x->parent = x->left;


    update_node_height(x);
    update_node_height(x->parent);
    
  }
  return retval;
}



/**
 * Rebalances the tree
 *
 * Args:
 * t: the tree struct
 * x:the root of the tree to balance
 *
 * Returns:
 * The root of the balanced tree
 */
struct tree_node *rebalance(struct tree *t, struct tree_node *x){
  struct tree_node *retval = x;
  unsigned int hl = height(x->left);
  unsigned int hr = height(x->right);

  if(hl > (hr+1)){

    if(height(x->left->right) > height(x->left->left)){
      rotate_left(t, x->left);
    }

    retval = rotate_right(t, x);
    
  }else if(hr > (hl+1)){

    if(height(x->right->left) > height(x->right->right)){
      rotate_right(t, x->right);
    }

    retval = rotate_left(t, x);
    
  }

  return retval;
  
}


/**
 * Inserts a node
 *
 * Args:
 * t: the tree
 * n: the new node
 * cmp: a pointer to a valid cmp_func compare function
 * cmp_sz: the size of the key to compare
 *
 * Returns:
 * 1 on success, 0 if the node already exists
 *
 */
int insert(struct tree *t, struct tree_node *n, cmp_func cmp, size_t cmp_sz){
  struct tree_node *mk = search(t, n->key, cmp, cmp_sz);
  int retval = 0;
  int c = 0;
  
  if(mk == NULL){
    t->root = n;
    t->size = 0;
    retval = 1;
  } else if((c = cmp(n->key, mk->key, cmp_sz)) != 0){
    n->parent = mk;
    if(c > 0)
      mk->right = n;
    else
      mk->left = n;
  }

  mk = n;

  unsigned int old;
  while(mk != NULL){
    if(dif(height(mk->left), height(mk->right)) > 1)
      rebalance(t, mk);

    update_node_height(mk);
    mk = mk->parent;
  }

  
}
