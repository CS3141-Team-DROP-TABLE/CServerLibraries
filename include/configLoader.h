#ifndef CONFIG_LOADER_H
#define CONFIG_LOADER_H

#include <stringMap.h>

#define DEFAULT_MAX_KEYSZ 254
#define DEFAULT_MAX_VALSZ 2047

struct config{
  struct str_map st;
  size_t max_keysz;
  size_t max_valsz;
  char *scn_fmt;

};

void config_loader_init(struct config *c, size_t max_keysize, size_t max_valsize);
void load_config(struct config *c, char *filename);
char *get_config(struct config *c, char *key);
void clear_config(struct config *c);
char *remove_config(struct config *c, char *key);
void parse_line(struct config *c, char *line);
#endif
