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
  char cost, modifier_amt, modify, effect, target;
  char *name;
} Art;

Art arts[10];

void init_arts()
{
  arts[0].cost = 0;
  arts[0].effect = 0;
  arts[0].name = "None";
  arts[0].modifier_amt = 0;
  arts[0].modify = 0;
  arts[0].target = TARGET_SELF;

  arts[1].effect = LIGHTENING_ART;
  arts[1].name = "Lightening";
  arts[1].cost = 100;
  arts[1].modifier_amt = -15;
  arts[1].target = TARGET_ENEMY;
  arts[1].modify = MOD_HP;

  arts[2].effect = FIRE_ART;
  arts[2].name = "Fire";
  arts[2].cost = 100;
  arts[2].modifier_amt = -15;
  arts[2].target = TARGET_ENEMY;
  arts[2].modify = MOD_HP;

  arts[3].effect = ICE_ART;
  arts[3].name = "Ice";
  arts[3].cost = 100;
  arts[3].modifier_amt = -15;
  arts[3].target = TARGET_ENEMY;
  arts[3].modify = MOD_HP;

  arts[4].effect = DEF_5_ART;
  arts[4].name = "Ice";
  arts[4].cost = 100;
  arts[4].modifier_amt = 5;
  arts[4].target = TARGET_ENEMY;
  arts[4].modify = MOD_DEF;
 
  arts[5].effect = ATK_5_ART;
  arts[5].name = "Ice";
  arts[5].cost = 100;
  arts[5].modifier_amt = 5;
  arts[5].target = TARGET_SELF;
  arts[5].modify = MOD_ATK;

  arts[6].effect = HEALING_ART;
  arts[6].name = "Ice";
  arts[6].cost = 100;
  arts[6].modifier_amt = 10;
  arts[6].target = TARGET_SELF;
  arts[6].modify = MOD_HP;

  arts[7].effect = POWER_WAVE_ART;
  arts[7].name = "Power Wave";
  arts[7].cost = 100;
  arts[7].target = TARGET_ENEMY;
  arts[7].modify = MOD_HP;
  arts[7].modifier_amt = 30;
}

void load_art(char art_no, int x, int y)
{
  switch(art_no)
  {
    case LIGHTENING_ART:
    display_window(2,8,12,3);
    put_string("Lightening",3,9);
    create_lightening(x,y);
    break;

    case HEALING_ART:
    display_window(2,8,10,3);
    put_string("Healing",3,9);
    create_healing(x+8,y+16);
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
    create_power_wave(x,y);
    
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
  }
}

void animate_effect(char art_no)
{
  switch(art_no)
  {
    case LIGHTENING_ART:
    animate_lightening(0);
    break;

    case HEALING_ART:
    animate_healing(0);
    break;

    case ATK_5_ART:
    case DEF_5_ART:
    animate_word_effect(0);
    break;

    case POWER_WAVE_ART:
    animate_power_wave(0);
    break;
  }
}