#define NO_ART 0
#define ZAP_ART 1
#define POWER_SHOT 2
#define ICE_ART 3
#define SPD_UP_ART 4
#define POWER_WAVE_ART 5

#define HEAL_ART 6
#define RANGED_SINGLE_ATTACK 7
#define RANGED_COLUMN_ATTACK 8
#define RANGED_ROW_ATTACK 9

#define PHYSICAL_SINGLE_ATTACK 10
#define PHYSICAL_COLUMN_ATTACK 11
#define PHYSICAL_ROW_ATTACK 12
#define PHYSICAL_ALL_ATTACK 13

#define HEAL_ALL_ART 14
#define RES_UP_ART 15
#define ATK_UP_ART 16
#define DEF_UP_ART 17

#define NONE_TO_NONE 0
#define ONE_TO_ONE 1
#define ONE_TO_MANY 2
#define MANY_TO_MANY 3

#define MOD_NONE 0
#define MOD_ATK 1
#define MOD_DEF 2
#define MOD_HP 3
#define MOD_SPD 4
#define MOD_RES 5

#define MOVE_NONE 0
#define MOVE_PHYSICAL_ATTACK 1
#define MOVE_HEAL 2
#define MOVE_ART_ATTACK 3
#define MOVE_ART_SUPPORT 4
#define MOVE_RANGED_ATTACK 5

typedef struct{
  char frame_count, target, relationship, stunning, move_type, base_amt;
  char *name;
} Art;

Art arts[18];

void init_arts()
{
  arts[0].frame_count = 0;
  arts[0].name = "None    ";
  arts[0].target = NO_TARGET;
  arts[0].relationship = NONE_TO_NONE;
  arts[0].stunning = 0;
  arts[0].move_type = MOVE_NONE;

  arts[ZAP_ART].frame_count = 12;
  arts[ZAP_ART].name = "Zap     ";
  arts[ZAP_ART].target = SINGLE_HIT;
  arts[ZAP_ART].relationship = ONE_TO_ONE;
  arts[ZAP_ART].stunning = 1;
  arts[ZAP_ART].move_type = MOVE_ART_ATTACK;
  arts[ZAP_ART].base_amt = 5;

  arts[POWER_SHOT].frame_count = 12;
  arts[POWER_SHOT].name = "P. Shot ";
  arts[POWER_SHOT].target = MULTI_ROW;
  arts[POWER_SHOT].relationship = ONE_TO_MANY;
  arts[POWER_SHOT].stunning = 1;
  arts[POWER_SHOT].base_amt = 3;
  arts[POWER_SHOT].move_type = MOVE_ART_ATTACK;

  arts[ICE_ART].frame_count = 12;
  arts[ICE_ART].name = "Freeze  ";
  arts[ICE_ART].target = SINGLE_HIT;
  arts[ICE_ART].relationship = ONE_TO_ONE;
  arts[ICE_ART].stunning = 1;
  arts[ICE_ART].base_amt = 5;
  arts[ICE_ART].move_type = MOVE_ART_ATTACK;

  arts[SPD_UP_ART].frame_count = 11;
  arts[SPD_UP_ART].name = "SPD up ";
  arts[SPD_UP_ART].target = ALL_ALLIES;
  arts[SPD_UP_ART].relationship = MANY_TO_MANY;
  arts[SPD_UP_ART].stunning = 0;
  arts[SPD_UP_ART].base_amt = 5;
  arts[SPD_UP_ART].move_type = MOVE_ART_SUPPORT;

  arts[DEF_UP_ART].frame_count = 11;
  arts[DEF_UP_ART].name = "DEF up ";
  arts[DEF_UP_ART].target = ALL_ALLIES;
  arts[DEF_UP_ART].relationship = MANY_TO_MANY;
  arts[DEF_UP_ART].stunning = 0;
  arts[DEF_UP_ART].base_amt = 5;
  arts[DEF_UP_ART].move_type = MOVE_ART_SUPPORT;
  
  arts[RES_UP_ART].frame_count = 11;
  arts[RES_UP_ART].name = "RES up ";
  arts[RES_UP_ART].target = ALL_ALLIES;
  arts[RES_UP_ART].relationship = MANY_TO_MANY;
  arts[RES_UP_ART].stunning = 0;
  arts[RES_UP_ART].base_amt = 5;
  arts[RES_UP_ART].move_type = MOVE_ART_SUPPORT;

  arts[ATK_UP_ART].frame_count = 11;
  arts[ATK_UP_ART].name = "ATK up ";
  arts[ATK_UP_ART].target = ALL_ALLIES;
  arts[ATK_UP_ART].relationship = MANY_TO_MANY;
  arts[ATK_UP_ART].stunning = 0;
  arts[ATK_UP_ART].base_amt = 5;
  arts[ATK_UP_ART].move_type = MOVE_ART_SUPPORT;

  arts[POWER_WAVE_ART].frame_count = 11;
  arts[POWER_WAVE_ART].name = "P.Wave  ";
  arts[POWER_WAVE_ART].target = MULTI_ROW;
  arts[POWER_WAVE_ART].relationship = ONE_TO_MANY;
  arts[POWER_WAVE_ART].stunning = 1;
  arts[POWER_WAVE_ART].move_type = MOVE_ART_ATTACK;
  arts[POWER_WAVE_ART].base_amt = 10;

  arts[HEAL_ART].frame_count = 11;
  arts[HEAL_ART].name = "Heal   ";
  arts[HEAL_ART].target = HEAL;
  arts[HEAL_ART].relationship = NONE_TO_NONE;
  arts[HEAL_ART].stunning = 0;
  arts[HEAL_ART].base_amt = 20;
  arts[HEAL_ART].move_type = MOVE_HEAL;

  arts[HEAL_ALL_ART].frame_count = 0;
  arts[HEAL_ALL_ART].name = "Heal   ";
  arts[HEAL_ALL_ART].target = HEAL_ALL;
  arts[HEAL_ALL_ART].relationship = NONE_TO_NONE;
  arts[HEAL_ALL_ART].stunning = 0;
  arts[HEAL_ALL_ART].move_type = MOVE_HEAL;

  arts[RANGED_SINGLE_ATTACK].frame_count = 0;
  arts[RANGED_SINGLE_ATTACK].name = "Ranged ";
  arts[RANGED_SINGLE_ATTACK].target = SINGLE_HIT;
  arts[RANGED_SINGLE_ATTACK].relationship = NONE_TO_NONE;
  arts[RANGED_SINGLE_ATTACK].stunning = 1;
  arts[RANGED_SINGLE_ATTACK].base_amt = 10;
  arts[RANGED_SINGLE_ATTACK].move_type = MOVE_RANGED_ATTACK;

  arts[RANGED_COLUMN_ATTACK].frame_count = 0;
  arts[RANGED_COLUMN_ATTACK].name = "Ranged ";
  arts[RANGED_COLUMN_ATTACK].target = MULTI_COL_3;
  arts[RANGED_COLUMN_ATTACK].relationship = NONE_TO_NONE;
  arts[RANGED_COLUMN_ATTACK].stunning = 1;
  arts[RANGED_COLUMN_ATTACK].base_amt = 5;
  arts[RANGED_COLUMN_ATTACK].move_type = MOVE_RANGED_ATTACK;

  arts[RANGED_ROW_ATTACK].frame_count = 0;
  arts[RANGED_ROW_ATTACK].name = "Ranged ";
  arts[RANGED_ROW_ATTACK].target = MULTI_ROW;
  arts[RANGED_ROW_ATTACK].relationship = NONE_TO_NONE;
  arts[RANGED_ROW_ATTACK].stunning = 1;
  arts[RANGED_ROW_ATTACK].base_amt = 5;
  arts[RANGED_ROW_ATTACK].move_type = MOVE_RANGED_ATTACK;

  arts[PHYSICAL_SINGLE_ATTACK].frame_count = 0;
  arts[PHYSICAL_SINGLE_ATTACK].name = "Melee  ";
  arts[PHYSICAL_SINGLE_ATTACK].target = SINGLE_HIT;
  arts[PHYSICAL_SINGLE_ATTACK].relationship = NONE_TO_NONE;
  arts[PHYSICAL_SINGLE_ATTACK].stunning = 1;
  arts[PHYSICAL_SINGLE_ATTACK].move_type = MOVE_PHYSICAL_ATTACK;
  arts[PHYSICAL_SINGLE_ATTACK].base_amt = 10;

  arts[PHYSICAL_COLUMN_ATTACK].frame_count = 0;
  arts[PHYSICAL_COLUMN_ATTACK].name = "Melee  ";
  arts[PHYSICAL_COLUMN_ATTACK].target = MULTI_COL_3;
  arts[PHYSICAL_COLUMN_ATTACK].relationship = NONE_TO_NONE;
  arts[PHYSICAL_COLUMN_ATTACK].stunning = 1;
  arts[PHYSICAL_COLUMN_ATTACK].move_type = MOVE_PHYSICAL_ATTACK;
  arts[PHYSICAL_COLUMN_ATTACK].base_amt = 7;

  arts[PHYSICAL_ROW_ATTACK].frame_count = 0;
  arts[PHYSICAL_ROW_ATTACK].name = "Melee  ";
  arts[PHYSICAL_ROW_ATTACK].target = MULTI_ROW;
  arts[PHYSICAL_ROW_ATTACK].relationship = NONE_TO_NONE;
  arts[PHYSICAL_ROW_ATTACK].stunning = 1;
  arts[PHYSICAL_ROW_ATTACK].move_type = MOVE_PHYSICAL_ATTACK;
  arts[PHYSICAL_ROW_ATTACK].base_amt = 7;
  
  arts[PHYSICAL_ALL_ATTACK].frame_count = 0;
  arts[PHYSICAL_ALL_ATTACK].name = "Melee  ";
  arts[PHYSICAL_ALL_ATTACK].target = ALL_OPPONENTS;
  arts[PHYSICAL_ALL_ATTACK].relationship = NONE_TO_NONE;
  arts[PHYSICAL_ALL_ATTACK].stunning = 1;
  arts[PHYSICAL_ALL_ATTACK].move_type = MOVE_PHYSICAL_ATTACK;
  arts[PHYSICAL_ALL_ATTACK].base_amt = 1;
}

void load_art(char art_no, int x, int y, char flip)
{
  switch(art_no)
  {
    case ZAP_ART:
    create_lightening(x,y-16,flip);
    break;

    case SPD_UP_ART:
    // create_def_up(SPD_UP_ART,x,y);
    create_word_effect(EFFECT_SPD_UP,x,y);
    break;

    case RES_UP_ART:
    create_word_effect(EFFECT_RES_UP,x,y);
    break;

    case ATK_UP_ART:
    create_word_effect(EFFECT_ATK_UP,x,y);
    break;

    case DEF_UP_ART:
    create_word_effect(EFFECT_DEF_UP,x,y);
    break;

    case POWER_WAVE_ART:
    create_power_wave(x,y,flip);
    break;

    case ICE_ART:
    create_ice(x,y,flip);
    break;

    case POWER_SHOT:
    create_power_shot(x,y,flip);
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

    case EFFECT_HEAL:
    animate_healing(effect_no);
    break;

    case EFFECT_DEF_UP:
    case EFFECT_ATK_UP:
    case EFFECT_SPD_UP:
    case EFFECT_RES_UP:
    case EFFECT_ADV:
    case EFFECT_MISS:
    case EFFECT_CRIT:
    // case EFFECT_DEF_5:
    animate_word_effect(effect_no);
    break;

    case EFFECT_WAVE:
    case EFFECT_POWER_SHOT:
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

// char get_modifier_amount(char art_id)
// {
//   switch(art_id)
//   {
//     case ZAP_ART: return 20;
//     case RAIN_ARROW_ART: return 15;
//     // case PLUNDER_ART: return 10;
//     case SPD_UP_ART: return 10;
//     case POWER_WAVE_ART: return 20;
//     // case BLOW_BACK_ART: return 10;
//     // case CLEAVE_ART: return 10;
//     // case FRENZY_ART: return 10;
//     // case RUN_THROUGH_ART: return 15;
//     // case BOMBARD_ART: return 15;
//     // case SEDUCE_ART: return 5;
//     default: return 0;
//   }
//   return 0;
// }

char get_modifier_type(char art_id)
{
  switch(art_id)
  {
    case ZAP_ART: return MOD_HP;
    case POWER_SHOT: return MOD_HP;
    case POWER_WAVE_ART: return MOD_HP;
    case ICE_ART: return MOD_HP;

    case SPD_UP_ART: return MOD_SPD;
    case DEF_UP_ART: return MOD_DEF;
    case ATK_UP_ART: return MOD_ATK;
    case RES_UP_ART: return MOD_RES;
  }
  return MOD_NONE;
}