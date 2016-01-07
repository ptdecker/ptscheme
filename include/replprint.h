/* replprint.h */

#ifndef FILE_REPLPRINT_SEEN
#define FILE_REPLPRINT_SEEN

/* REPL - Print */

extern void expand_esc_seq(char str[], const char c);
extern void write_pair(FILE *out, object *pair);
extern void write(FILE *out, object *obj);

#endif // FILE_REPLPRINT_SEEN/
