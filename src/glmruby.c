#include <stdio.h>
#include <dirent.h>

#include <SDL.h>
#include <SDL_image.h>

#include "mruby.h"
#include "mruby/class.h"
#include "mruby/data.h"
#include "mruby/compile.h"
#include "mruby/variable.h"

#include "sprite.h"
#include "input.h"

/* ���C���̃E�B���h�E�T�[�t�F�C�X */
SDL_Surface *mWIN;

/* �^�C�}�p */
struct Timer {
  Uint32 now,
         wit,
         lev;
} timer;



/* �C�x���g���� */
int quits(void)
{
  SDL_Event evnts;
  if (SDL_PollEvent(&evnts)) {
    switch(evnts.type)
    {
    case SDL_QUIT:
      return 0;
      break;
    case SDL_KEYDOWN:
      if (evnts.key.keysym.sym==SDLK_ESCAPE ||
          evnts.key.keysym.sym==SDLK_q) return 0;
      break;
    default: break;
    }
  }
  return 1;
}

int main(int argc, char *argv[])
{
  mrb_value s;
  FILE *fp;
  DIR *dp;
  struct dirent *dirst;

  mrb_state *mrb  = mrb_open();

  glmrb_sprite_init(mrb);
  glmrb_input_init(mrb);

  dp = opendir("./lib");
  while((dirst = readdir(dp)) != NULL)
  {
    char buf[256];
    if (dirst->d_name[0] == '.') continue;
    strcpy(buf, "./lib/");
    strcat(buf, dirst->d_name);
    fp = fopen(buf, "r");
    mrb_load_file(mrb, fp);
    fclose(fp);
  }
  closedir(dp);

  fp = fopen("main.rb", "r");
  s = mrb_load_file(mrb, fp);
  fclose(fp);

  /* SDL������ */
  if (SDL_Init(SDL_INIT_VIDEO) < 0 ) {
    exit(-1);
  }

  /* �t���[���o�b�t�@�쐬 */
  mWIN = SDL_SetVideoMode(640, 480, 0, 0);
  if (!mWIN) {
    SDL_Quit();
    exit(-1);
  }

  /* �L���v�V�����ݒ� */
  SDL_WM_SetCaption("glmruby application",NULL);

  /* ���C�����[�v */
  while(quits() != 0) {

    mrb_funcall(mrb, s, "update", 0);

    /* ��ʏ��� */
    SDL_FillRect(mWIN, NULL, SDL_MapRGB(mWIN->format, 0, 0, 0));

    mrb_funcall(mrb, s, "draw", 0);

    /* �E�F�C�g���� */
    timer.now = SDL_GetTicks();
    timer.wit = timer.now - timer.lev;
    if (timer.wit < 16) SDL_Delay(16 - timer.wit);
    timer.lev = SDL_GetTicks();

    /* ��ʍX�V */
    SDL_UpdateRect(mWIN, 0, 0, 0, 0);
  }

  /* �I������ */
  SDL_Quit();
  return 0;
}

