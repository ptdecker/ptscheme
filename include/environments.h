/* environments.h */

#ifndef FILE_ENVIRONMENTS_SEEN
#define FILE_ENVIRONMENTS_SEEN

extern object *lookup_variable_value(object *var, object *env);
extern void set_variable_value(object *var, object *val, object *env);
extern void define_variable(object *var, object *val, object *env);

extern object *setup_environment(void);
extern object *make_environment(void);
extern void init_environments();
extern object *extend_environment(object *vars, object *vals, object *base_env);

extern object *the_empty_environment;
extern object *the_global_environment;

#endif // !FILE_ENVIRONMENTS_SEEN
