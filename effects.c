#incspr(adv_effect,"map/effects/adv.pcx")
#incspr(def_effect_5,"map/effects/def_5.pcx")
#incspr(atk_effect_3,"map/effects/atk_5.pcx")
#incspr(lightening_effect,"map/effects/lightening.pcx")
#incspr(heal_effect,"map/effects/healing.pcx")
#incspr(hit_spark,"map/effects/hit_spark.pcx")
#incspr(power_wave,"map/effects/power_wave.pcx")
#incspr(ice_effect,"map/effects/ice.pcx")
#incspr(fire_effect,"map/effects/fire.pcx")
#incspr(rain_arrow,"map/effects/arrow_fall.pcx")
#incspr(atk_up,"map/effects/atk_up.pcx")
#incspr(def_up,"map/effects/def_up.pcx")
#incspr(capture_effect,"map/effects/capture_effect.pcx")

#incpal(effect_pal,"map/effects/adv.pcx")
#incpal(lightening_effect_pal,"map/effects/lightening.pcx")
#incpal(fire_effect_pal,"map/effects/fire.pcx")
#incpal(arrow_effect_pal,"map/effects/arrow_fall.pcx")

// #define EFFECTS_VRAM 0x3600
#define EFFECTS_VRAM 0x6C00
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
#define EFFECT_ATK_3 7
#define EFFECT_HEAL 8
#define EFFECT_HIT_SPARK 9
#define EFFECT_WAVE 10
#define EFFECT_ICE 11
#define EFFECT_FIRE 12
#define EFFECT_ARROW 13
#define EFFECT_ATK_UP 14
#define EFFECT_DEF_UP 15
#define EFFECT_CAPTURE 16

#define MAX_EFFECT_COUNT 5

const char LIGHTENING_TOP_ANIMATION[]   = { 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0 };
const char LIGHTENING_MID_1_ANIMATION[] = { 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0 };
const char LIGHTENING_MID_2_ANIMATION[] = { 0, 0, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0 };
const char LIGHTENING_BOT_ANIMATION[]   = { 0, 0, 0, 1, 1, 0, 1, 1, 1, 0, 0, 0 };
const int HEAL_ANIMATION[] = { 0x40, 0x80, 0xC0, 0x100, 0x40, 0x80, 0xC0, 0x100, 0x40, 0x80, 0xC0 };
const int HIT_ANIMATION[] = { 0x40, 0x80, 0xC0, 0x100, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
const int ICE_ANIMATION[] = { 0x00, 0x00, 0x40, 0x40, 0x80, 0x80, 0xC0, 0xC0, 0x100, 0x100, 0x00 };

char effect_count = 0;
char effects[MAX_EFFECT_COUNT];
int effects_x[MAX_EFFECT_COUNT];
int effects_y[MAX_EFFECT_COUNT];
char effects_pal[MAX_EFFECT_COUNT];
char effect_frames[MAX_EFFECT_COUNT];
char effects_flip[MAX_EFFECT_COUNT];

char create_effect(char effect_type, int x, int y, char flip)
{
  if(effect_count >= MAX_EFFECT_COUNT)
  {
    put_string("max effects",10,0);
    wait_for_I_input();
    return;
  }
  effects[effect_count] = effect_type;
  effects_x[effect_count] = x;
  effects_y[effect_count] = y;
  effect_frames[effect_count] = 0;
  effects_flip[effect_count] = (flip)? 8 : 0;
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

      // case EFFECT_DEF_5:
      //   load_vram(EFFECTS_VRAM+(effect_count*0x200),def_effect_5,0x40);
      //   effects_pal[effect_count] = EFFECTS_WORD_PAL;
      //   break;

      case EFFECT_ATK_3:
        load_vram(EFFECTS_VRAM+(effect_count*0x200),atk_effect_3,0x40);
        effects_pal[effect_count] = EFFECTS_WORD_PAL;
        break;    

      case EFFECT_ATK_UP:
        load_vram(EFFECTS_VRAM+(effect_count*0x200),atk_up,0x40);
        effects_pal[effect_count] = EFFECTS_WORD_PAL;
        break;

      case EFFECT_DEF_UP:
        load_vram(EFFECTS_VRAM+(effect_count*0x200),def_up,0x40);
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

      case EFFECT_CAPTURE:
        load_vram(EFFECTS_VRAM+(effect_count*0x200),capture_effect,0x100);
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

      case EFFECT_ICE:
        load_vram(EFFECTS_VRAM+(effect_count*0x200),ice_effect,0x140);
        effects_pal[effect_count] = EFFECTS_ARTS_PAL;
        break;

      case EFFECT_FIRE:
        load_vram(EFFECTS_VRAM+(effect_count*0x200),fire_effect,0x140);
        effects_pal[effect_count] = EFFECTS_ARTS_PAL;
        break;

      case EFFECT_ARROW:
        load_vram(EFFECTS_VRAM+(effect_count*0x200),rain_arrow,0x140);
        effects_pal[effect_count] = EFFECTS_ARTS_PAL;
        break;
    }
}

void display_effect(char effect_no)
{
  // put_number(effect_no,3,0,0);
  // wait_for_I_input();
  spr_make(effect_no,effects_x[effect_no],effects_y[effect_no],EFFECTS_VRAM+(effect_no*0x200),FLIP_MAS,effects_flip[effect_no],effects_pal[effect_no],1);
  spr_hide();
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
    effects[i] = EFFECT_NONE;//effects[i+1];
    effects_x[i] = 0;//effects_x[i+1];
    effects_y[i] = 0;//effects_y[i+1];
    effects_pal[i] = 0;//effects_pal[i+1];
    effect_frames[i] = 0;//effect_frames[i+1];
    effects_flip[i] = 0;//effect_frames[i+1];
    effect_count--; 
  }
}

void remove_ice(char n){}
void remove_fire(char n){}

void remove_effects()
{
  char i;
  for(i=0; i<MAX_EFFECT_COUNT; i++)
  {
    remove_effect(i);
  }
  effect_count = 0;
}

void create_art_by_type(char effect_no, int x, int y, char flip)
{
  switch(effect_no)
  {
    case EFFECT_LIGHTENING_BOT:
    case EFFECT_LIGHTENING_MID_1:
    case EFFECT_LIGHTENING_MID_2:
    case EFFECT_LIGHTENING_TOP:
    create_lightening(x,y,flip);
    break;

    case EFFECT_HEAL:
    create_healing(x,y,flip);
    break;

    case EFFECT_ATK_3:
    create_atk_3(x,y);
    break;

    case EFFECT_ATK_UP:
    create_atk_up(x,y);
    break;

    case EFFECT_DEF_UP:
    create_def_up(x,y);
    break;

    case EFFECT_ICE:
    create_ice(x,y,flip);
    break;

    case EFFECT_FIRE:
    create_fire(x,y,flip);
    break;
  }
}

char create_lightening(int x, int y, char flip)
{
  create_effect(EFFECT_LIGHTENING_TOP,x,y-32,flip);
  create_effect(EFFECT_LIGHTENING_MID_1,x,y-16,flip);
  create_effect(EFFECT_LIGHTENING_MID_2,x,y,flip);
  create_effect(EFFECT_LIGHTENING_BOT,x,y+16,flip);
  load_palette(31,lightening_effect_pal,1);
  return effect_count - 3;
}

char create_healing(int x, int y, char flip)
{
  load_palette(31,lightening_effect_pal,1);
  return create_effect(EFFECT_HEAL,x,y,flip);
}

char create_capture(int x, int y, char flip)
{
  load_palette(31,lightening_effect_pal,1);
  return create_effect(EFFECT_CAPTURE,x,y,flip);
}

char create_arrows(int x, int y, char flip)
{
  load_palette(31,arrow_effect_pal,1);
  return create_effect(EFFECT_ARROW,x,y,flip);
}

char create_ice(int x, int y, char flip)
{
  load_palette(31,lightening_effect_pal,1);
  return create_effect(EFFECT_ICE,x,y,flip);
}

char create_fire(int x, int y, char flip)
{
  load_palette(31,fire_effect_pal,1);
  return create_effect(EFFECT_FIRE,x,y,flip);
}

char create_hit_spark(int x, int y, char flip)
{
  load_palette(31,lightening_effect_pal,1);
  return create_effect(EFFECT_HIT_SPARK,x,y,flip);
}

char create_def_up(int x, int y)
{
  return create_effect(EFFECT_DEF_UP,x,y,0);
}

char create_atk_3(int x, int y)
{
  return create_effect(EFFECT_ATK_3,x,y,0);
}

char create_atk_up(int x, int y)
{
  return create_effect(EFFECT_ATK_UP,x,y,0);
}

char create_power_wave(int x, int y, char flip)
{
  load_palette(31,lightening_effect_pal,1);
  return create_effect(EFFECT_WAVE,x,y,flip);
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

void animate_ice(char effect_no)
{
  spr_set(effect_no);
  spr_show();
  spr_pattern(EFFECTS_VRAM+(effect_no*0x200)+ICE_ANIMATION[effect_frames[effect_no]++]);
}

void animate_arrow(char effect_no)
{
  spr_set(effect_no);
  spr_show();
  spr_pattern(EFFECTS_VRAM+(effect_no*0x200)+ICE_ANIMATION[effect_frames[effect_no]++]);
}

void animate_hit_spark(char effect_no)
{
  char frame_no;
  frame_no = effect_frames[effect_no]++;
  if(frame_no > 11)
  {
    return;
  }

  spr_set(effect_no);
  spr_pattern(EFFECTS_VRAM+(effect_no*0x200)+HIT_ANIMATION[frame_no]);
}

void animate_healing(char effect_no)
{
  char frame_no;
  spr_set(effect_no);
  // put_number(g,3,10,g++);
  if(effect_frames[effect_no]++ < 11)
  {
    // wait_for_I_input();
    frame_no = effect_frames[effect_no];
    spr_pattern(EFFECTS_VRAM+(effect_no*0x200)+HEAL_ANIMATION[frame_no]);
    spr_y(spr_get_y()-1);
  }
}

void animate_lightening(char effect_no)
{
  char frame_no, i;

  if(effect_count > 0)
  {
    frame_no = effect_frames[effect_no];
    // for(i=0; i<4; i++)
    // {
      effect_frames[effect_no]++;

      spr_set(effect_no);
      if(LIGHTENING_TOP_ANIMATION[frame_no + (effect_no*11)])
      {
        spr_show(effect_no);
      }
      else
      {
        spr_hide(effect_no);
      }
    // }
  }
}

void animate_power_wave(char effect_no)
{
  char direction;
  direction = (effects_flip[effect_no])? -12 : 12;
  spr_set(effect_no);
  spr_show();
  spr_x(effects_x[effect_no]);
  effects_x[effect_no] += direction;
}