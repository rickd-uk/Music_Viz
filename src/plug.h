// Header guard -> so you do not accidentally include the headers twice
#ifndef PLUG_H_
#define PLUG_H_

#include <complex.h>
#include <raylib.h>

#define N ((1) << (13))
// 1 << 13  =  8,192


#define LIST_OF_PLUGS                                                                                                  \
  PLUG(plug_init, void )                                                                                                     \
  PLUG(plug_pre_reload, void*, void)                                                                                                \
  PLUG(plug_post_reload, void, void*)                                                                                               \
  PLUG(plug_update, void, void)

#define PLUG(name, ret, ...) typedef ret (name##_t)(__VA_ARGS__);
LIST_OF_PLUGS
#undef PLUG 

#endif // PLUG_H_
