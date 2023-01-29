#define NO_ART 0
#define LIGHTENING_ART 1
#define FIRE_ART 2
#define ICE_ART 3
#define DEF_5_ART 4
#define ATK_5_ART 5
#define HEALING_ART 6
#define POWER_WAVE_ART 7

#define TARGET_SELF 0
#define TARGET_ENEMY 1

#define MOD_ATK 1
#define MOD_DEF 2
#define MOD_HP 3

typedef struct{
  char cost, modifier_amt, modify, frame_count, target;
  char *name;
} Art;

Art arts[10];

void init_arts()
{
  arts[0].cost = 0;
  arts[0].frame_count = 0;
  arts[0].name = "None";
  arts[0].modifier_amt = 0;
  arts[0].modify = 0;
  arts[0].target = NO_ATTACK;

  arts[1].frame_count = 11;
  arts[1].name = "Lightening";
  arts[1].cost = 100;
  arts[1].modifier_amt = -15;
  arts[1].target = SINGLE_HIT;
  arts[1].modify = MOD_HP;

  arts[2].frame_count = 0;
  arts[2].name = "Fire";
  arts[2].cost = 100;
  arts[2].modifier_amt = -15;
  arts[2].target = SINGLE_HIT;
  arts[2].modify = MOD_HP;

  arts[3].frame_count = 11;
  arts[3].name = "Ice";
  arts[3].cost = 100;
  arts[3].modifier_amt = -15;
  arts[3].target = SINGLE_HIT;
  arts[3].modify = MOD_HP;

  arts[4].frame_count = 11;
  arts[4].name = "4";
  arts[4].cost = 100;
  arts[4].modifier_amt = 5;
  arts[4].target = SINGLE_HIT;
  arts[4].modify = MOD_DEF;
 
  arts[5].frame_count = 11;
  arts[5].name = "5";
  arts[5].cost = 100;
  arts[5].modifier_amt = 5;
  arts[5].target = SINGLE_HIT;
  arts[5].modify = MOD_ATK;

  arts[6].frame_count = 11;
  arts[6].name = "6";
  arts[6].cost = 100;
  arts[6].modifier_amt = 10;
  arts[6].target = MULTI_HEAL_AOE;
  arts[6].modify = MOD_HP;

  arts[7].frame_count = 5;
  arts[7].name = "Power Wave";
  arts[7].cost = 100;
  arts[7].target = MULTI_COL_3;
  arts[7].modify = MOD_HP;
  arts[7].modifier_amt = -20;
}

void load_art(char art_no, int x, int y, char flip)
{
  switch(art_no)
  {
    case LIGHTENING_ART:
    display_window(2,8,12,3);
    put_string("Lightening",3,9);
    create_lightening(x,y,flip);
    break;

    case HEALING_ART:
    display_window(2,8,10,3);
    put_string("Healing",3,9);
    create_healing(x+8,y+16,flip);
    break;

    case DEF_5_ART:
    display_window(2,8,8,3);
    put_string("Def Up",3,9);
    create_def_up(x,y);
    break;

    case ATK_5_ART:
    display_window(2,8,8,3);
    put_string("Atk Up",3,9);
    create_atk_up(x,y);
    break;

    case POWER_WAVE_ART:
    display_window(2,8,12,3);
    put_string("Power Wave",3,9);
    create_power_wave(x,y,flip);
    
    case ICE_ART:
    display_window(2,8,8,3);
    put_string(arts[art_no].name,3,9);
    create_ice(x+8,y+16,flip);
    break;
  }
}

void remove_art(char art_no)
{
  switch(art_no)
  {
    case LIGHTENING_ART:
    remove_lightening_effect(0);
    break;

    case HEALING_ART:
    remove_effect(0);
    break;

    case ATK_5_ART:
    case DEF_5_ART:
    remove_word_effect(0);
    break;

    case POWER_WAVE_ART:
    remove_effect(0);
    break;

    case ICE_ART:
    remove_ice(0);
    break;
  }
}

void animate_effect(char effect_type, char effect_no)
{
  switch(effect_type)
  {
    // case LIGHTENING_ART:
    case EFFECT_LIGHTENING_BOT:
    case EFFECT_LIGHTENING_MID_1:
    case EFFECT_LIGHTENING_MID_2:
    case EFFECT_LIGHTENING_TOP:
    animate_lightening(effect_no);
    break;

    // case HEALING_ART:
    case EFFECT_HEAL:
    animate_healing(effect_no);
    break;

    // case ATK_5_ART:
    // case DEF_5_ART:
    case EFFECT_ATK_5:
    case EFFECT_DEF_5:
    animate_word_effect(effect_no);
    break;

    // case POWER_WAVE_ART:
    case EFFECT_WAVE:
    animate_power_wave(effect_no);
    break;

    case EFFECT_HIT_SPARK:
    animate_hit_spark(effect_no);
    break;

    case EFFECT_ICE:
    animate_ice(effect_no);
    break;
  }
}

void animate_effects()
{
  char i;
  for(i=0; i<effect_count; i++)
  {
    animate_effect(effects[i],i);
  }
}