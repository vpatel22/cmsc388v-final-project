/* gui.c: sdl gui backend */
#include <SDL2/SDL.h>
#include "fb.h"
#include "util.h"

static SDL_Window *win;
static SDL_Renderer *renderer;
static SDL_Texture *texture;
static Uint32 framedelay;

void
gui_init(int width, int height, int scale, int fps)
{
  int rc;

  rc = SDL_Init(SDL_INIT_VIDEO);
  if (rc != 0)
    die("fail to init sdl");

  rc = SDL_CreateWindowAndRenderer(width*scale, height*scale,
                                   SDL_WINDOW_ALLOW_HIGHDPI,
                                   &win, &renderer);
  if (rc != 0)
    die("fail to create window");
  SDL_SetWindowTitle(win, "pitch");

  texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_BGRA32,
                              SDL_TEXTUREACCESS_STREAMING,
                              width, height);
  if (!texture)
    die("fail to create texture");

  framedelay = 1000/fps;
}

int
gui_tick(void) {
  static Uint32 ntick = 0;
  SDL_Event event;

  /* cap fps to prevent burning cpu */
  Uint32 tick = SDL_GetTicks();
  if (tick < ntick)
    SDL_Delay(ntick - tick);
  ntick = SDL_GetTicks() + framedelay;

  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT:
      return 1;
    case SDL_KEYDOWN:
      return event.key.keysym.sym == SDLK_ESCAPE;
    }
  }
  return 0;
}

void
gui_finish(void)
{
  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(win);
  SDL_Quit();
}

void
gui_redraw(frmbuf *fb) {
  Pixel *texbuf;
  int pitch;

  /* copy fb to texture */
  SDL_LockTexture(texture, NULL, (void **)&texbuf, &pitch);
  memcpy(texbuf, fb->buf, fb->size);
  SDL_UnlockTexture(texture);

  /* texture -> renderer -> screen */
  SDL_RenderCopy(renderer, texture, NULL, NULL);
  SDL_RenderPresent(renderer);
}
