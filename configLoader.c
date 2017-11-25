#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include <stringMap.h>
#include <configLoader.h>


void config_loader_init(struct config *c, size_t max_keysize, size_t max_valsize){
  c->max_keysz = (max_keysize < 1)? DEFAULT_MAX_KEYSZ : max_keysize;
  c->max_valsz = (max_valsize < 1)? DEFAULT_MAX_VALSZ : max_valsize;

  fprintf(stderr, "%lu\n", c->max_keysz);
  
  str_map_init(&c->st, NULL, c->max_keysz);
  c->scn_fmt = malloc(sizeof(char)*255);
  snprintf(c->scn_fmt, 255, "%%%lu[^=]%%*c %%%lu[^;]%%*c%%*c", c->max_keysz,  c->max_valsz);

}



void load_config(struct config *c, char *filename){
  char key[c->max_keysz];
  char val[c->max_valsz];

  FILE *confFile = fopen(filename, "r");
  if(confFile == NULL){
    fprintf(stderr, "Error opening config file: %s\n", strerror(errno));
  }

  while(fscanf(confFile, c->scn_fmt, key, val) != EOF){
    str_map_insert_str(&c->st, key, val);
  }

  fclose(confFile);
}

char *get_config(struct config *c, char *key){
  return str_map_search(&c->st, key);
}

void clear_config(struct config *c){
  str_clear_all(&c->st);
}

char *remove_config(struct config *c, char *key){
  return str_map_remove(&c->st, key);
}
