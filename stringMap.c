#include <stdlib.h>
#include <string.h>
#include <stdio.h>


#include <AVL.h>
#include <stringMap.h>



int default_cmp(void *a, void *b, size_t max_keysz){
  char *sa = (char*)a;
  char *sb = (char*)b;
  size_t min = (strnlen(sa, max_keysz) < strnlen(sb, max_keysz))?
		strnlen(sa, max_keysz):
		strnlen(sb, max_keysz);

  return strncmp(sa, sb, (min < max_keysz)? min : max_keysz);
}


void str_map_init(struct str_map *s, cmp_func cmp, size_t max_keysz){
  s->cmp = cmp;
  
  s->max_keysz = (max_keysz > 0)? max_keysz : DEFAULT_MXKEY;
  
  s->cmp = (cmp != NULL)? cmp : &default_cmp;
  
  avl_init(&s->avl_tree);
}


int str_map_insert_str(struct str_map *s, char* key_str, char* val_str){
  

  size_t keylen = strnlen(key_str, s->max_keysz);
  
  char *val = malloc(strlen(val_str) +1*sizeof(char));
  size_t len = strlen(val_str);
  strncpy(val, val_str, len);
  *(val + len) = '\0';


  int retval = avl_insert(&s->avl_tree, key_str, val, s->cmp, keylen);


  
  if(!retval){
    free(val);
  }

  return retval;
}

char *str_map_search(struct str_map *s, char *key_str){
  return avl_search(&s->avl_tree, key_str, s->cmp, strnlen(key_str, s->max_keysz));

}


char *str_map_remove(struct str_map *s, char *key_str){
  return avl_remove(&s->avl_tree, key_str, s->cmp, strnlen(key_str, s->max_keysz));

}
