#incspr(adv_effect,"map/effects/adv.pcx")
#incspr(lightening_effect,"map/effects/lightening.pcx")

#incpal(effect_pal,"map/effects/adv.pcx")
#incpal(lightening_effect_pal,"map/effects/lightening.pcx")

#define EFFECTS_VRAM 0x3600
#define EFFECTS_WORD_PAL 30
#define EFFECTS_ARTS_PAL 31
#define EFFECTS_SPR_NUM 0

#define EFFECT_NON 0
#define EFFECT_ADV 1
#define EFFECT_LIGHTENING_TOP 2
#define EFFECT_LIGHTENING_MID_1 3
#define EFFECT_LIGHTENING_MID_2 4
#define EFFECT_LIGHTENING_BOT 5

const LIGHTENING_TOP_ANIMATION[] = { 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0};
const LIGHTENING_MID_1_ANIMATION[] = { 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 0};
const LIGHTENING_MID_2_ANIMATION[] = { 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0};
const LIGHTENING_BOT_ANIMATION[] = { 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0};

char effect_count = 0;
char effects[5];
int effects_x[5];
int effects_y[5];
char effects_pal[5];
char effect_frames[5];

void create_effect(char effect_type, int x, int y)
{
  effects[effect_count] = effect_type;
  effects_x[effect_count] = x;
  effects_y[effect_count] = y;
  effect_frames[effect_count] = 0;
  load_effect(effect_type);
  display_effect(effect_count);
  effect_count++;
}

void load_effect(char effect_no)
{
    switch(effect_no)
    {
        case EFFECT_ADV:
          load_vram(EFFECTS_VRAM+(effect_count*0x40),adv_effect,0x40);
          effects_pal[effect_count] = EFFECTS_WORD_PAL;
          break;

        case EFFECT_LIGHTENING_TOP:
          load_vram(EFFECTS_VRAM+(effect_count*0x40),lightening_effect+0x80,0x40);
          effects_pal[effect_count] = EFFECTS_ARTS_PAL;
          break;

        case EFFECT_LIGHTENING_MID_1:
          load_vram(EFFECTS_VRAM+(effect_count*0x40),lightening_effect+0xC0,0x40);
          effects_pal[effect_count] = EFFECTS_ARTS_PAL;
          break;

        case EFFECT_LIGHTENING_MID_2:
          load_vram(EFFECTS_VRAM+(effect_count*0x40),lightening_effect+0x100,0x40);
          effects_pal[effect_count] = EFFECTS_ARTS_PAL;
          break;

        case EFFECT_LIGHTENING_BOT:
          load_vram(EFFECTS_VRAM+(effect_count*0x40),lightening_effect+0x140,0x40);
          effects_pal[effect_count] = EFFECTS_ARTS_PAL;
          break;
    }
}

void display_effect(char effect_no)
{
  spr_make(effect_no,effects_x[effect_no],effects_y[effect_no],EFFECTS_VRAM+(effect_no*0x40),0,0,effects_pal[effect_no],1);
}

void hide_effect(char effect_no)
{
  spr_set(effect_no);
  spr_hide();
}

void remove_effect(char effect_no)
{
  char i;
  hide_effect(effect_no);
  for(i=effect_no; i<5; i++)
  {
    effects[i] = effects[i+1];
    effects_x[i] = effects_x[i+1];
    effects_y[i] = effects_y[i+1];
    effects_pal[i] = effects_pal[i+1];
    effect_frames[i] = 0; //= effect_frames[i+1];
  }
  effect_count--;
}

void create_lightening(int x, int y)
{
  create_effect(EFFECT_LIGHTENING_TOP,x,y-32);
  create_effect(EFFECT_LIGHTENING_MID_1,x,y-16);
  create_effect(EFFECT_LIGHTENING_MID_2,x,y);
  create_effect(EFFECT_LIGHTENING_BOT,x,y+16);
  load_palette(31,lightening_effect_pal,1);
}

void remove_lightening(char frame_no)
{
  remove_effect(frame_no+3);
  remove_effect(frame_no+2);
  remove_effect(frame_no+1);
  remove_effect(frame_no);
}

void animate_effect(char effect_no)
{
  animate_lightening(effect_no);
}

void animate_lightening(char effect_no)
{
  char frame_no, i;

  if(effect_count > 0)
  {
    frame_no = effect_frames[effect_no];
    for(i=0; i<4; i++)
    {
      effect_frames[effect_no+i]++;

      spr_set(effect_no+i);
      if(LIGHTENING_TOP_ANIMATION[frame_no + (i*11)])
      {
        spr_show(effect_no+i);
      }
      else
      {
        spr_hide(effect_no+i);
      }
    }
  }
}