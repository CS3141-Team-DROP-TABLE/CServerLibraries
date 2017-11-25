#ifndef STRING_MAP_H
#define STRING_MAP_H

#include <AVL.h>

#define DEFAULT_MXKEY 254

struct str_map{
  struct tree avl_tree;
  cmp_func cmp;
  size_t max_keysz;
};

void str_map_init(struct str_map *s, cmp_func cmp, size_t max_keysz);
char *str_map_search(struct str_map *s, char *key_str);
char *str_map_remove(struct str_map *s, char *key_str);
int str_map_insert_str(struct str_map *s, char* key_str, char* val_str);

#endif
