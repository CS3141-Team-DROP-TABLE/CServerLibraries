#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>


#include <AVL.h>


int verify_tree_h(struct tree_node *x, cmp_func cmp, size_t cmp_sz){

  
  int retval = 0;
  
  if(x != NULL){
    if(x->left != NULL){
      if(cmp(x->key, x->left->key, cmp_sz) <= 0){ 
	retval--;
      }
    }
    if(x->right != NULL){
      if(cmp(x->key, x->right->key, cmp_sz) >= 0){
	retval--;
      }
    }

    
    return retval+(verify_tree_h(x->left, cmp, cmp_sz), verify_tree_h(x->right, cmp, cmp_sz));
      
  }

}

int avl_verify_tree(struct tree *t, cmp_func cmp, size_t cmp_sz){
  pthread_mutex_lock(&t->mut);
  struct tree_node *x = (t != NULL)?t->root : NULL;
  int retval = verify_tree_h(x, cmp, cmp_sz);
  pthread_mutex_unlock(&t->mut);
  return retval;
}

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
    if(cmp(key, mk->key, cmp_sz) > 0){
      mk = mk->right;

    }else{
      mk = mk->left;
      
    }
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
int update_node_height(struct tree_node *x){
  int retval = 0;
  if(x != NULL){
    x->height = max(update_node_height(x->left), update_node_height(x->right)) + 1;
    retval = x->height;
  }
  return retval;
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
    x->right->parent = x->parent;
    x->parent = x->right;
    if(rl != NULL)
      rl->parent = x;

    x->parent->left = x;
    x->right = rl;


    
    x->height = max(height(x->left), height(x->right))+1;
    retval->height = max(height(retval->left), height(retval->right))+1;

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
    x->left->parent = x->parent;
    x->parent = x->left;
    if(lr != NULL)
      lr->parent = x;

    
    x->parent->right = x;
    x->left = lr;


    x->height = max(height(x->left), height(x->right))+1;
    retval->height = max(height(retval->left), height(retval->right))+1;
    
  }
  return retval;
}

/**
 * Rotates the right subtree tree to the right then the tree left, 
 * I.E. the right subtree of x is too big and it's left subtree is too big
 * 
 * Args:
 * t: the tree struct
 * x: is the root of the tree to balance
 *
 * Returns:
 * The root of the balanced tree, NULL if the programmer calling is an idiot
 *
 */
struct tree_node *rotate_rightleft(struct tree *t, struct tree_node *x){
  struct tree_node *y, *z;
  struct tree_node *syl, *syr;
  
  y = NULL;
  z = NULL;

  if(x->right != NULL && x->right->left != NULL){
    z = x->right;
    y = x->right->left;
    change_parent_child(t, x, y);
    syl = y->left;
    syr = y->right;

    y->parent = x->parent;
    x->parent = y;
    z->parent = y;

    y->left = x;
    y->right = z;

    
    if(syl != NULL)
      syl->parent = x;

    x->right = syl;


    if(syr != NULL)
      syr->parent = z;

    z->left = syr;

    if(z != NULL)
      z->height = max(height(z->left), height(z->right))+1;
    
    x->height = max(height(x->left), height(x->right))+1;
    y->height = max(height(y->left), height(y->right))+1;
    
  } else {
    fprintf(stderr, "[AVL lib] Something may not be right\n");
  }

 
  
  return y;
}

/**
 * Rotates the left subtree tree to the left then the tree right, 
 * I.E. the left subtree of x is too big and it's right subtree is too big
 * 
 * Args:
 * t: the tree struct
 * x: is the root of the tree to balance
 *
 * Returns:
 * The root of the balanced tree, NULL if the programmer calling is an idiot
 *
 */
struct tree_node *rotate_leftright(struct tree *t, struct tree_node *x){
  struct tree_node *y, *z;
  struct tree_node *syl, *syr;

  y = NULL;
  z = NULL;

  if(x->left != NULL && x->left->right != NULL){
    z = x->left;
    y = x->left->right;
    change_parent_child(t, x, y);
    syl = y->left;
    syr = y->right;

    y->parent = x->parent;
    x->parent = y;
    z->parent = y;

    y->left = z;
    y->right = x;

    
    if(syl != NULL)
      syl->parent = z;

    z->right = syl;


    if(syr != NULL)
      syr->parent = x;

    x->left = syr;

    if(z != NULL)
      z->height = max(height(z->left), height(z->right))+1;
    
    x->height = max(height(x->left), height(x->right))+1;
    y->height = max(height(y->left), height(y->right))+1;
    
  } else {
    fprintf(stderr, "[AVL lib] Something may not be right\n");
  }
  
  return y;
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
      retval = rotate_leftright(t, x);
    } else {
      retval = rotate_right(t, x);
    }
  }else if(hr > (hl+1)){

    if(height(x->right->left) > height(x->right->right)){
      retval = rotate_rightleft(t, x);
    } else {
      retval = rotate_left(t, x);
    }
    
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
    n->height = 1;
    if(c > 0)
      mk->right = n;
    else
      mk->left = n;
    retval = 1;
  }

  if(retval){
    mk = n;
    
    while(mk != NULL){
      mk->height = max(height(mk->left), height(mk->right)) +1;
      mk = mk->parent;
    }
    
    mk = n;

    
    while(mk != NULL){
      if(dif(height(mk->left), height(mk->right)) > 1){
	rebalance(t, mk);
      }
      mk = mk->parent;
    }
  
    update_node_height(t->root);
    t->size++;
  }
  
  return retval;
}


/**
 * Returns the next reatest node, or NULL if none exists
 *
 * Args: 
 * x: the node to get the successor of
 *
 * Returns:
 * a pointer to the successor node
 */
struct tree_node *successor(struct tree_node *x){
  struct tree_node *x_lag = NULL;
  if(x->right != NULL){
    x = x->right;
    while(x != NULL){
      x_lag = x;
      x = x->left;
    }
  }

  return x_lag;
}


/**
 * Removes a node
 *
 * Args:
 * t: the tree struct
 * x: the node to remove
 *
 * Returns:
 * The pointer to the removed node
 */
struct tree_node *remove_node(struct tree *t, struct tree_node *x){
  struct tree_node *su = NULL;
  struct tree_node *mk = NULL;

  if(x->right == NULL){

    if(x->left != NULL){
      su = x->left;
      swap(x, su);
      x->left = NULL;
    } else {
      change_parent_child(t, x, NULL);
    }

  } else {

    su = successor(x);
    swap(x, su);
    change_parent_child(t, su, su->right);
    if(su->right != NULL){
      su->right->parent = su->parent;
      su = su->right;
    }
    
  }

  mk = (su == NULL)? x : su;
  
  while(su != NULL){
    su->height = (max(height(su->left), height(su->right)))+1;
    if(dif(height(su->left), height(su->right)) > 1){
      rebalance(t, su);
    }
    su = su->parent;
  }

  if(mk->parent){
    if(mk->parent->right == mk){
      mk->parent->right = NULL;
    } else if(mk->parent->left == mk){
      mk->parent->left = NULL;
    }
  }
    
  update_node_height(t->root);
  
  return mk;
}
 


/**
 * Inserts a node into the tree
 *
 * N.B. !!!! Only cmp_sz worth of the key will be stored
 * They key is copied. 
 *
 * The value is not copied, only the pointer is stored, so don't free the
 * data.
 *
 * Args:
 * t: the tree struct
 * key: a pointer to the key
 * cmp: a function pointer to the compare function
 * cmp_sz: the size to be compared
 *
 * Returns:
 * 1 on success, 0 on failure
 */ 

int avl_insert(struct tree *t, void *key, void *value, cmp_func cmp, size_t cmp_sz){
  int retval = 0;
    
  if( t != NULL && key != NULL && cmp != NULL && cmp_sz > 0) {
    pthread_mutex_lock(&t->mut);
    void *key_store = malloc(cmp_sz);
    memcpy(key_store, key, cmp_sz);
    struct tree_node *n = create_node(key_store, value);
    retval = insert(t, n, cmp, cmp_sz);
    
    pthread_mutex_unlock(&t->mut);
  }
  return retval;
}

/**
 * Removes a node from the tree
 *
 * Args:
 * t: the tree struct
 * key: a pointer to the key
 * cmp: a function pointer to the compare function
 * cmp_sz: the size to be compared
 *
 * Returns:
 * A pointer to the value, NULL if no such value was found.
 */ 

void* avl_remove(struct tree *t, void *key, cmp_func cmp, size_t cmp_sz){
  void *retval = NULL;
  pthread_mutex_lock(&t->mut);
  struct tree_node *mk = search(t, key, cmp, cmp_sz);
  if(mk != NULL && cmp(mk->key, key, cmp_sz) == 0){
    retval = mk->val;
    mk = remove_node(t, mk);
    prinf("Free %p\n", mk);
    avl_intern_print(t->root);
    free(mk->key);
    free(mk);
  }

  pthread_mutex_unlock(&t->mut);
  return retval;
}

 
/**
 * Searches for a key in the avl tree
 * 
 * Args:
 * t: the tree
 * key: the key to search for
 * cmp: the function to compare with
 * cmp_sz: the size to compare
 *
 * Returns:
 * A pointer to the value, or NULL if the key is not found
 */
void *avl_search(struct tree *t, void *key, cmp_func cmp, size_t cmp_sz){
  void *retval = NULL;
  pthread_mutex_lock(&t->mut);
  struct tree_node *mk = search(t, key, cmp, cmp_sz);
  if(mk != NULL && cmp(mk->key, key, cmp_sz) == 0)
    retval = mk->val;
  
  pthread_mutex_unlock(&t->mut);
  return retval;
}


void apply_recurse(struct tree_node *x, all_func fn, void *args){
  if(x != NULL){
    apply_recurse(x->left, fn, args);
    apply_recurse(x->right, fn, args);
    fn(x->key, x->val, args);
  }
}

void avl_apply_to_all(struct tree *t, all_func fn, void *args){
  pthread_mutex_lock(&t->mut);
  apply_recurse(t->root, fn, args);
  pthread_mutex_unlock(&t->mut);
}

void avl_clear_tree(struct tree *t, all_func del){
  pthread_mutex_lock(&t->mut);
  apply_recurse(t->root, del, NULL);
  t->root = NULL;
  t->size = 0;

  pthread_mutex_unlock(&t->mut);
}

void avl_intern_print(struct tree_node *mk){
  if(mk){
    avl_intern_print(mk->left);
    printf("%p  : %p  l:%p r:%p\n", mk->parent, mk, mk->left, mk->right);
    avl_intern_print(mk->right);
    
  }
}
