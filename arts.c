#define NO_ART 0
#define ZAP_ART 1
#define RAIN_ARROW_ART 2
#define RAPID_THRUST_ART 3
// #define PLUNDER_ART 4
#define SPD_UP_ART 4
// #define CAPTURE_ART 6
#define POWER_WAVE_ART 5
// #define BLOW_BACK_ART 8
// #define CLEAVE_ART 9
#define HEAL_ART 6
#define HEAL_ALL_ART 14
// #define RUN_THROUGH_ART 12
// #define FATAL_SHOT_ART 13
// #define BOMBARD_ART 14
// #define TRACK_ART 15
// #define INNVIGORATE_ART 16
// #define RESTORE_ART 17
// #define RED_EYE_ART 18
// #define BLACK_EYE_ART 19
// #define CLEAR_EYES_ART 20
// #define SEDUCE_ART 21
// #define SEA_LEGS_ART 22
// #define FRENZY_ART 0

#define PHYSICAL_SINGLE_ATTACK 10
#define PHYSICAL_COLUMN_ATTACK 11
#define PHYSICAL_ROW_ATTACK 12
#define PHYSICAL_ALL_ATTACK 13

#define RANGED_SINGLE_ATTACK 7
#define RANGED_COLUMN_ATTACK 8
#define RANGED_ROW_ATTACK 9
// #define SUPPORT_ALL_ATTACK 30

#define NONE_TO_NONE 0
#define ONE_TO_ONE 1
#define ONE_TO_MANY 2
#define MANY_TO_MANY 3

#define MOD_NONE 0
#define MOD_ATK 1
#define MOD_DEF 2
#define MOD_HP 3
#define MOD_SPD 4
#define MOD_POW 5

#define MOVE_NONE 0
#define MOVE_PHYSICAL_ATTACK 1
#define MOVE_HEAL 2
#define MOVE_ART_ATTACK 3
#define MOVE_ART_SUPPORT 4
#define MOVE_RANGED_ATTACK 5

typedef struct{
  char cost, frame_count, target, relationship, stunning, move_type, base_amt;
  char *name;
} Art;

Art arts[15];

void init_arts()
{
  arts[0].cost = 0;
  arts[0].frame_count = 0;
  arts[0].name = "None    ";
  arts[0].target = NO_TARGET;
  arts[0].relationship = NONE_TO_NONE;
  arts[0].stunning = 0;
  arts[0].move_type = MOVE_NONE;

  arts[ZAP_ART].frame_count = 12;
  arts[ZAP_ART].name = "Zap     ";
  arts[ZAP_ART].cost = 2;
  arts[ZAP_ART].target = SINGLE_HIT;
  arts[ZAP_ART].relationship = ONE_TO_ONE;
  arts[ZAP_ART].stunning = 1;
  arts[ZAP_ART].move_type = MOVE_ART_ATTACK;

  arts[SPD_UP_ART].frame_count = 11;
  arts[SPD_UP_ART].name = "Spd up ";
  arts[SPD_UP_ART].cost = 7;
  arts[SPD_UP_ART].target = ALL_ALLIES;
  arts[SPD_UP_ART].relationship = MANY_TO_MANY;
  arts[SPD_UP_ART].stunning = 0;
  arts[SPD_UP_ART].base_amt = 3;
  arts[SPD_UP_ART].move_type = MOVE_ART_SUPPORT;

  arts[POWER_WAVE_ART].frame_count = 11;
  arts[POWER_WAVE_ART].name = "P.Wave  ";
  arts[POWER_WAVE_ART].cost = 1;
  arts[POWER_WAVE_ART].target = MULTI_ROW;
  arts[POWER_WAVE_ART].relationship = ONE_TO_MANY;
  arts[POWER_WAVE_ART].stunning = 1;
  arts[POWER_WAVE_ART].move_type = MOVE_ART_ATTACK;

  arts[HEAL_ART].frame_count = 11;
  arts[HEAL_ART].name = "Heal   ";
  arts[HEAL_ART].cost = 1;
  arts[HEAL_ART].target = HEAL;
  arts[HEAL_ART].relationship = NONE_TO_NONE;
  arts[HEAL_ART].stunning = 0;
  arts[HEAL_ART].base_amt = 20;
  arts[HEAL_ART].move_type = MOVE_HEAL;

  arts[HEAL_ALL_ART].frame_count = 0;
  arts[HEAL_ALL_ART].name = "Heal   ";
  arts[HEAL_ALL_ART].cost = 1;
  arts[HEAL_ALL_ART].target = HEAL_ALL;
  arts[HEAL_ALL_ART].relationship = NONE_TO_NONE;
  arts[HEAL_ALL_ART].stunning = 0;
  arts[HEAL_ALL_ART].move_type = MOVE_HEAL;

  arts[RANGED_SINGLE_ATTACK].frame_count = 0;
  arts[RANGED_SINGLE_ATTACK].name = "Ranged ";
  arts[RANGED_SINGLE_ATTACK].cost = 1;
  arts[RANGED_SINGLE_ATTACK].target = SINGLE_HIT;
  arts[RANGED_SINGLE_ATTACK].relationship = NONE_TO_NONE;
  arts[RANGED_SINGLE_ATTACK].stunning = 1;
  arts[RANGED_SINGLE_ATTACK].base_amt = 10;
  arts[RANGED_SINGLE_ATTACK].move_type = MOVE_RANGED_ATTACK;

  arts[RANGED_COLUMN_ATTACK].frame_count = 0;
  arts[RANGED_COLUMN_ATTACK].name = "Ranged ";
  arts[RANGED_COLUMN_ATTACK].cost = 1;
  arts[RANGED_COLUMN_ATTACK].target = MULTI_COL_3;
  arts[RANGED_COLUMN_ATTACK].relationship = NONE_TO_NONE;
  arts[RANGED_COLUMN_ATTACK].stunning = 1;
  arts[RANGED_COLUMN_ATTACK].base_amt = 7;
  arts[RANGED_COLUMN_ATTACK].move_type = MOVE_RANGED_ATTACK;

  arts[RANGED_ROW_ATTACK].frame_count = 0;
  arts[RANGED_ROW_ATTACK].name = "Ranged ";
  arts[RANGED_ROW_ATTACK].cost = 1;
  arts[RANGED_ROW_ATTACK].target = MULTI_ROW;
  arts[RANGED_ROW_ATTACK].relationship = NONE_TO_NONE;
  arts[RANGED_ROW_ATTACK].stunning = 1;
  arts[RANGED_ROW_ATTACK].base_amt = 7;
  arts[RANGED_ROW_ATTACK].move_type = MOVE_RANGED_ATTACK;

  arts[PHYSICAL_SINGLE_ATTACK].frame_count = 0;
  arts[PHYSICAL_SINGLE_ATTACK].name = "Melee  ";
  arts[PHYSICAL_SINGLE_ATTACK].cost = 0;
  arts[PHYSICAL_SINGLE_ATTACK].target = SINGLE_HIT;
  arts[PHYSICAL_SINGLE_ATTACK].relationship = NONE_TO_NONE;
  arts[PHYSICAL_SINGLE_ATTACK].stunning = 1;
  arts[PHYSICAL_SINGLE_ATTACK].move_type = MOVE_PHYSICAL_ATTACK;
  arts[PHYSICAL_SINGLE_ATTACK].base_amt = 10;

  arts[PHYSICAL_COLUMN_ATTACK].frame_count = 0;
  arts[PHYSICAL_COLUMN_ATTACK].name = "Melee  ";
  arts[PHYSICAL_COLUMN_ATTACK].cost = 0;
  arts[PHYSICAL_COLUMN_ATTACK].target = MULTI_COL_3;
  arts[PHYSICAL_COLUMN_ATTACK].relationship = NONE_TO_NONE;
  arts[PHYSICAL_COLUMN_ATTACK].stunning = 1;
  arts[PHYSICAL_COLUMN_ATTACK].move_type = MOVE_PHYSICAL_ATTACK;
  arts[PHYSICAL_COLUMN_ATTACK].base_amt = 7;

  arts[PHYSICAL_ROW_ATTACK].frame_count = 0;
  arts[PHYSICAL_ROW_ATTACK].name = "Melee  ";
  arts[PHYSICAL_ROW_ATTACK].cost = 0;
  arts[PHYSICAL_ROW_ATTACK].target = MULTI_ROW;
  arts[PHYSICAL_ROW_ATTACK].relationship = NONE_TO_NONE;
  arts[PHYSICAL_ROW_ATTACK].stunning = 1;
  arts[PHYSICAL_ROW_ATTACK].move_type = MOVE_PHYSICAL_ATTACK;
  arts[PHYSICAL_ROW_ATTACK].base_amt = 7;
  
  arts[PHYSICAL_ALL_ATTACK].frame_count = 0;
  arts[PHYSICAL_ALL_ATTACK].name = "Melee  ";
  arts[PHYSICAL_ALL_ATTACK].cost = 0;
  arts[PHYSICAL_ALL_ATTACK].target = ALL_OPPONENTS;
  arts[PHYSICAL_ALL_ATTACK].relationship = NONE_TO_NONE;
  arts[PHYSICAL_ALL_ATTACK].stunning = 1;
  arts[PHYSICAL_ALL_ATTACK].move_type = MOVE_PHYSICAL_ATTACK;
  arts[PHYSICAL_ALL_ATTACK].base_amt = 7;
}

void load_art(char art_no, int x, int y, char flip)
{
  switch(art_no)
  {
    case ZAP_ART:
    create_lightening(x,y-16,flip);
    break;

    // case PLUNDER_ART:
    // create_atk_up(x,y);
    // break;

    case SPD_UP_ART:
    create_def_up(x,y);
    break;

    // case RAPID_THRUST_ART:
    // create_atk_3(x,y);
    // break;

    case POWER_WAVE_ART:
    create_power_wave(x,y,flip);
    break;

    // case INNVIGORATE_ART:
    // create_healing(x,y,flip);
    // break;

    // case JUDGEMENT_ART:
    // create_healing(x+8,y+16,flip);
    // break;

    case RAIN_ARROW_ART:
    create_arrows(x,y,flip);
    break;

    // case CAPTURE_ART:
    // create_capture(x,y,flip);
    // break;
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

    case EFFECT_HEAL:
    animate_healing(effect_no);
    break;

    case EFFECT_DEF_UP:
    case EFFECT_ATK_UP:
    case EFFECT_ATK_3:
    case EFFECT_ADV:
    case EFFECT_MISS:
    case EFFECT_CRIT:
    // case EFFECT_DEF_5:
    animate_word_effect(effect_no);
    break;

    case EFFECT_WAVE:
    animate_power_wave(effect_no);
    break;

    case EFFECT_HIT_SPARK:
    animate_hit_spark(effect_no);
    break;

    case EFFECT_CAPTURE:
    case EFFECT_ICE:
    case EFFECT_FIRE:
    case EFFECT_POOF:
    case EFFECT_POWER_UP:
    animate_ice(effect_no);
    break;

    case EFFECT_ARROW:
    animate_arrow(effect_no);
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

char get_modifier_amount(char art_id)
{
  switch(art_id)
  {
    case ZAP_ART: return 20;
    case RAIN_ARROW_ART: return 15;
    // case PLUNDER_ART: return 10;
    case SPD_UP_ART: return 10;
    case POWER_WAVE_ART: return 20;
    // case BLOW_BACK_ART: return 10;
    // case CLEAVE_ART: return 10;
    // case FRENZY_ART: return 10;
    // case RUN_THROUGH_ART: return 15;
    // case BOMBARD_ART: return 15;
    // case SEDUCE_ART: return 5;
    default: return 0;
  }
  return 0;
}

char get_modifier_type(char art_id)
{
  switch(art_id)
  {
    case ZAP_ART: return MOD_HP;
    case RAIN_ARROW_ART: return MOD_HP;
    // case PLUNDER_ART: return MOD_ATK;
    case SPD_UP_ART: return MOD_SPD;
    case POWER_WAVE_ART: return MOD_HP;
    // case BLOW_BACK_ART: return MOD_HP;
    // case CLEAVE_ART: return MOD_HP;
    // case FRENZY_ART: return MOD_HP;
    // case RUN_THROUGH_ART: return MOD_HP;
    // case BOMBARD_ART: return MOD_HP;
    // case SEDUCE_ART: return MOD_DEF;
  }
  return MOD_NONE;
}