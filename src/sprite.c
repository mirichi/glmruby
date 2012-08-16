#include <stdio.h>

#include <SDL.h>
#include <SDL_image.h>

#include "mruby.h"
#include "mruby/class.h"
#include "mruby/data.h"
#include "mruby/variable.h"

#include "sprite.h"

/* Spriteクラス */
struct RClass *cSprite;

/* メインのウィンドウサーフェイス */
extern SDL_Surface *mWIN;

static void glmrb_sprite_free(mrb_state *mrb, void *ptr);

/* mruby用Spriteクラスタイプ定義 */
static struct mrb_data_type mrb_sprite_type = { "Sprite", glmrb_sprite_free };

static mrb_value
glmrb_sprite_initialize(mrb_state *mrb, mrb_value self)
{
  struct glmrb_sprite *sprite;
  mrb_value vsprite, vx, vy;
  char *filename;

  mrb_get_args(mrb, "ooz", &vx, &vy, &filename);
  sprite = (struct glmrb_sprite *)mrb_malloc(mrb, sizeof(struct glmrb_sprite));
  if (!sprite) mrb_raise(mrb, E_RUNTIME_ERROR, "out of memory.");
  DATA_PTR(self) = sprite;
  DATA_TYPE(self) = &mrb_sprite_type;
  mrb_iv_set(mrb, self, mrb_intern(mrb, "@x"), vx);
  mrb_iv_set(mrb, self, mrb_intern(mrb, "@y"), vy);

  /* 画像読み込み */
  sprite->surface = IMG_Load(filename);
  if (!sprite->surface) mrb_raise(mrb, E_RUNTIME_ERROR, "file not found.");

  return vsprite;
}

static mrb_value
glmrb_sprite_draw(mrb_state *mrb, mrb_value self)
{
  struct glmrb_sprite *sprite = DATA_PTR(self);
  SDL_Rect src, drw;

  /* 描画座標設定 */
  src.x = 0;
  src.y = 0;
  src.w = -1;
  src.h = -1;
  drw.x = mrb_fixnum(mrb_iv_get(mrb, self, mrb_intern(mrb, "@x")));
  drw.y = mrb_fixnum(mrb_iv_get(mrb, self, mrb_intern(mrb, "@y")));

  /* 描画 */
  SDL_BlitSurface(sprite->surface, &src, mWIN, &drw);
  return self;
}

static void
glmrb_sprite_free(mrb_state *mrb, void *ptr)
{
  mrb_free(mrb, ptr);
}

void glmrb_sprite_init(mrb_state *mrb)
{
  cSprite = mrb_define_class(mrb, "Sprite", mrb->object_class);
  MRB_SET_INSTANCE_TT(cSprite, MRB_TT_DATA);

  mrb_define_method(mrb, cSprite, "initialize", glmrb_sprite_initialize, ARGS_REQ(3));
  mrb_define_method(mrb, cSprite, "draw", glmrb_sprite_draw, ARGS_NONE());
}
