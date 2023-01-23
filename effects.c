#incspr(adv_effect,"map/effects/adv.pcx")
#incspr(def_effect_5,"map/effects/def_5.pcx")
#incspr(atk_effect_5,"map/effects/atk_5.pcx")
#incspr(lightening_effect,"map/effects/lightening.pcx")
#incspr(heal_effect,"map/effects/healing.pcx")
#incspr(hit_spark,"map/effects/hit_spark.pcx")
#incspr(power_wave,"map/effects/power_wave.pcx")

#incpal(effect_pal,"map/effects/adv.pcx")
#incpal(lightening_effect_pal,"map/effects/lightening.pcx")

#define EFFECTS_VRAM 0x3600
#define EFFECTS_WORD_PAL 30
#define EFFECTS_ARTS_PAL 31
#define EFFECTS_SPR_NUM 0

#define EFFECT_NONE 0
#define EFFECT_ADV 1
#define EFFECT_LIGHTENING_TOP 2
#define EFFECT_LIGHTENING_MID_1 3
#define EFFECT_LIGHTENING_MID_2 4
#define EFFECT_LIGHTENING_BOT 5
#define EFFECT_DEF_5 6
#define EFFECT_ATK_5 7
#define EFFECT_HEAL 8
#define EFFECT_HIT_SPARK 9
#define EFFECT_WAVE 10

const char LIGHTENING_TOP_ANIMATION[] = { 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0 };
const char LIGHTENING_MID_1_ANIMATION[] = { 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 0 };
const char LIGHTENING_MID_2_ANIMATION[] = { 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0 };
const char LIGHTENING_BOT_ANIMATION[] = { 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0 };
const int HEAL_ANIMATION[] = { 0x40, 0x80, 0xC0, 0x100, 0x40, 0x80, 0xC0, 0x100, 0x40, 0x80, 0xC0 };
const int HIT_ANIMATION[] = { 0x40, 0x80, 0xC0, 0x100, 0x140, 0x180, 0x00, 0x00, 0x00, 0x00, 0x00 };

// const int HIT_ANIMATION[] = { 0x00, 0x40, 0x40, 0x80, 0x80, 0xC0, 0xC0, 0x100, 0x100, 0x140, 0x140 };
// const int HIT_ANIMATION[] = { 0x40, 0x80, 0xC0, 0x100, 0x140, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

char effect_count = 0;
char effects[5];
int effects_x[5];
int effects_y[5];
char effects_pal[5];
char effect_frames[5];

char create_effect(char effect_type, int x, int y)
{
  if(effect_count >= 5)
  {
    return;
  }
  effects[effect_count] = effect_type;
  effects_x[effect_count] = x;
  effects_y[effect_count] = y;
  effect_frames[effect_count] = 0;
  load_effect(effect_type);
  display_effect(effect_count);
  return effect_count++;
}

void load_effect(char effect_no)
{
  switch(effect_no)
  {
      case EFFECT_ADV:
        load_vram(EFFECTS_VRAM+(effect_count*0x200),adv_effect,0x40);
        effects_pal[effect_count] = EFFECTS_WORD_PAL;
        break;

      case EFFECT_DEF_5:
        load_vram(EFFECTS_VRAM+(effect_count*0x200),def_effect_5,0x40);
        effects_pal[effect_count] = EFFECTS_WORD_PAL;
        break;

      case EFFECT_ATK_5:
        load_vram(EFFECTS_VRAM+(effect_count*0x200),atk_effect_5,0x40);
        effects_pal[effect_count] = EFFECTS_WORD_PAL;
        break;    

      case EFFECT_LIGHTENING_TOP:
        load_vram(EFFECTS_VRAM+(effect_count*0x200),lightening_effect+0x80,0x40);
        effects_pal[effect_count] = EFFECTS_ARTS_PAL;
        break;

      case EFFECT_LIGHTENING_MID_1:
        load_vram(EFFECTS_VRAM+(effect_count*0x200),lightening_effect+0xC0,0x40);
        effects_pal[effect_count] = EFFECTS_ARTS_PAL;
        break;

      case EFFECT_LIGHTENING_MID_2:
        load_vram(EFFECTS_VRAM+(effect_count*0x200),lightening_effect+0x100,0x40);
        effects_pal[effect_count] = EFFECTS_ARTS_PAL;
        break;

      case EFFECT_LIGHTENING_BOT:
        load_vram(EFFECTS_VRAM+(effect_count*0x200),lightening_effect+0x140,0x40);
        effects_pal[effect_count] = EFFECTS_ARTS_PAL;
        break;
        
      case EFFECT_HEAL:
        load_vram(EFFECTS_VRAM+(effect_count*0x200),heal_effect,0x100);
        effects_pal[effect_count] = EFFECTS_ARTS_PAL;
        break;
        
      case EFFECT_HIT_SPARK:
        load_vram(EFFECTS_VRAM+(effect_count*0x200),hit_spark,0x140);
        effects_pal[effect_count] = EFFECTS_ARTS_PAL;
        break;

      case EFFECT_WAVE:
        load_vram(EFFECTS_VRAM+(effect_count*0x200),power_wave,0x40);
        effects_pal[effect_count] = EFFECTS_ARTS_PAL;
        break;
    }
}

void display_effect(char effect_no)
{
  spr_make(effect_no,effects_x[effect_no],effects_y[effect_no],EFFECTS_VRAM+(effect_no*0x200),0,0,effects_pal[effect_no],1);
}

void hide_effect(char effect_no)
{
  spr_set(effect_no);
  spr_hide();
}

void remove_effect(char effect_no)
{
  char i;
  if(effect_no != -1)
  {
    hide_effect(effect_no);
    // for(i=effect_no; i<5; i++)
    // {
    effects[i] = EFFECT_NONE;//effects[i+1];
    effects_x[i] = 0;//effects_x[i+1];
    effects_y[i] = 0;//effects_y[i+1];
    effects_pal[i] = 0;//effects_pal[i+1];
    effect_frames[i] = 0;//effect_frames[i+1];
    // }
    effect_count--; 
  }
}

char create_lightening(int x, int y)
{
  create_effect(EFFECT_LIGHTENING_TOP,x,y-32);
  create_effect(EFFECT_LIGHTENING_MID_1,x,y-16);
  create_effect(EFFECT_LIGHTENING_MID_2,x,y);
  create_effect(EFFECT_LIGHTENING_BOT,x,y+16);
  load_palette(31,lightening_effect_pal,1);
  return effect_count - 3;
}

char create_healing(int x, int y)
{
  load_palette(31,lightening_effect_pal,1);
  return create_effect(EFFECT_HEAL,x,y);
}

char create_hit_spark(int x, int y)
{
  load_palette(31,lightening_effect_pal,1);
  return create_effect(EFFECT_HIT_SPARK,x,y);
}

char create_def_up(int x, int y)
{
  return create_effect(EFFECT_DEF_5,x,y);
}

char create_atk_up(int x, int y)
{
  return create_effect(EFFECT_ATK_5,x,y);
}

char create_power_wave(int x, int y)
{
  load_palette(31,lightening_effect_pal,1);
  return create_effect(EFFECT_WAVE,x,y);
}

void remove_lightening_effect(char frame_no)
{
  remove_effect(frame_no+3);
  remove_effect(frame_no+2);
  remove_effect(frame_no+1);
  remove_effect(frame_no);
}

void remove_word_effect(char frame_no)
{
  remove_effect(frame_no);
}

void animate_word_effect(char effect_no)
{
  if(effect_no == -1)
  {
    return;
  }
  spr_set(effect_no);
  spr_y(spr_get_y(effect_no)-1);
}

void animate_hit_spark(char effect_no)
{
  char frame_no;
  if(effect_no == -1)
  {
    return;
  }
  spr_set(effect_no);
  
  frame_no = effect_frames[effect_no]++;
  spr_pattern(EFFECTS_VRAM+(effect_no*0x200)+HIT_ANIMATION[frame_no]);
}

void animate_healing(char effect_no)
{
  char frame_no;

  if(effect_no == -1 || effects[effect_no] != 8)
  {
    return;
  }
  spr_set(effect_no);
  
  frame_no = effect_frames[effect_no]++;
  // put_number(effects[effect_no],3,0,0);
  spr_pattern(EFFECTS_VRAM+(effect_no*0x200)+HEAL_ANIMATION[frame_no]);
  spr_y(spr_get_y()-1);
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

void animate_power_wave(char effect_no)
{
  if(effect_count > 0)
  {
    spr_set(effect_no);
    spr_x(spr_get_x()+4);
  }
}