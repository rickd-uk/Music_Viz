#include <assert.h>
#include <complex.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <raylib.h>

// posix specific; will not work on Windows
#include <dlfcn.h>

#include "plug.h"

#define ARRAY_LEN(xs) sizeof(xs) / sizeof(xs[0])

const char *libplug_file_name = "libplug.so";
void *libplug = NULL;

#ifdef HOTRELOAD
#define PLUG(name, ...) name##_t *name = NULL;
#else
#define PLUG(name, ...) name##_t name;
#endif
LIST_OF_PLUGS
#undef PLUG

#ifdef HOTRELOAD
bool reload_libplug(void) {

  if (libplug != NULL)
    dlclose(libplug);

  libplug = dlopen(libplug_file_name, RTLD_NOW);
  if (libplug == NULL) {
    fprintf(stderr, "ERROR: could not load %s: %s", libplug_file_name, dlerror());
    return false;
  }

#define PLUG(name, ...)                                                                                                \
  name = dlsym(libplug, #name);                                                                                        \
  if (name == NULL) {                                                                                                  \
    fprintf(stderr, "ERROR: could not find %s symbol in %s:  %s", #name, libplug_file_name, dlerror());                \
    return false;                                                                                                      \
  }
  LIST_OF_PLUGS
#undef PLUG

  return true;
}
#else
#define reload_libplug() true
#endif

int main(void) {

  if (!reload_libplug())
    return 1;

  InitWindow(800, 480, "musicviz");
  SetTargetFPS(60);
  InitAudioDevice();

  plug_init();

  while (!WindowShouldClose()) {
    if (IsKeyPressed(KEY_R)) {
      // allocate plugins state on a heap;  still avaliable after unloading?
      // we are about to reload you, so save the state.  Give the pointer to the
      // next version
      void *state = plug_pre_reload(); // signals to the plugin we are about to reload
      if (!reload_libplug())
        return 1;
      plug_post_reload(state);
    }
    plug_update();
  }
  return 0;
}

// url: https://www.youtube.com/watch?v=PgDqBZFir1A&list=PLpM-Dvs8t0Vak1rrE2NJn8XYEJ5M7-BqT&index=
// ./build.sh && ./build/musicviz music/Starship.ogg
