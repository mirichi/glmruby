#include <stdio.h>

#include <SDL.h>
#include <SDL_image.h>

#include "mruby.h"

//#include "input.h"

/* InputÉÇÉWÉÖÅ[Éã */
struct RClass *mInput;


static mrb_value
glmrb_input_get_x(mrb_state *mrb, mrb_value obj)
{
  mrb_int x = 0;
  const Uint8* sdlKeyState = SDL_GetKeyState(NULL);

  if (sdlKeyState[SDLK_LEFT] == 1) {
    x--;
  }
  if (sdlKeyState[SDLK_RIGHT] == 1) {
    x++;
  }

  return mrb_fixnum_value(x);
}

static mrb_value
glmrb_input_get_y(mrb_state *mrb, mrb_value obj)
{
  mrb_int y = 0;
  const Uint8* sdlKeyState = SDL_GetKeyState(NULL);

  if (sdlKeyState[SDLK_UP] == 1) {
    y--;
  }
  if (sdlKeyState[SDLK_DOWN] == 1) {
    y++;
  }

  return mrb_fixnum_value(y);
}

void glmrb_input_init(mrb_state *mrb)
{
  mInput = mrb_define_module(mrb, "Input");
  mrb_define_module_function(mrb, mInput, "x", glmrb_input_get_x, ARGS_NONE());
  mrb_define_module_function(mrb, mInput, "y", glmrb_input_get_y, ARGS_NONE());
}
