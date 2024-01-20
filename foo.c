

#define LIST_OF_PLUGS                                                                                                  \
  PLUG(plug_hello)                                                                                                     \
  PLUG(plug_init)                                                                                                      \
  PLUG(plug_pre_reload)                                                                                                \
  PLUG(plug_post_reload)                                                                                               \
  PLUG(plug_update)

#define PLUG(name) name##_t name = NULL;
LIST_OF_PLUGS
#undef PLUG
