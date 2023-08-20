#define NO_ART 0
#define ZAP_ART 1
#define RAIN_ARROW_ART 2
#define RAPID_THRUST_ART 3
#define PLUNDER_ART 4
#define SPD_UP_ART 5
#define CAPTURE_ART 6
#define POWER_WAVE_ART 7
#define BLOW_BACK_ART 8
#define CLEAVE_ART 9
#define HEAL_ART 10
#define HEAL_ALL_ART 11
#define RUN_THROUGH_ART 12
#define FATAL_SHOT_ART 13
#define BOMBARD_ART 14
#define TRACK_ART 15
#define INNVIGORATE_ART 16
#define RESTORE_ART 17
#define RED_EYE_ART 18
#define BLACK_EYE_ART 19
// #define CLEAR_EYES_ART 20
// #define SEDUCE_ART 21
// #define SEA_LEGS_ART 22
// #define FRENZY_ART 0

#define PHYSICAL_SINGLE_ATTACK 23
#define PHYSICAL_COLUMN_ATTACK 24
#define PHYSICAL_ROW_ATTACK 25
#define PHYSICAL_ALL_ATTACK 26

#define RANGED_SINGLE_ATTACK 20
#define RANGED_COLUMN_ATTACK 21
#define RANGED_ROW_ATTACK 22
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

Art arts[24+4];

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

  arts[2].frame_count = 8;
  arts[2].name = "Volley  ";
  arts[2].cost = 1;
  arts[2].target = SINGLE_HIT;
  arts[2].relationship = ONE_TO_ONE;
  arts[2].stunning = 1;

  arts[3].frame_count = 11; //rapid thrust
  arts[3].name = "Rapid AT";
  arts[3].cost = 1;
  arts[3].target = SINGLE_HIT;
  arts[3].relationship = NONE_TO_NONE;
  arts[3].stunning = 0;

  arts[4].frame_count = 11;
  arts[4].name = "Plunder ";
  arts[4].cost = 1;
  arts[4].target = ALL_ALLIES;
  arts[4].relationship = MANY_TO_MANY;
  arts[4].stunning = 0;
  arts[4].move_type = MOVE_ART_SUPPORT; 

  arts[SPD_UP_ART].frame_count = 11;
  arts[SPD_UP_ART].name = "Spd up ";
  arts[SPD_UP_ART].cost = 7;
  arts[SPD_UP_ART].target = ALL_ALLIES;
  arts[SPD_UP_ART].relationship = MANY_TO_MANY;
  arts[SPD_UP_ART].stunning = 0;
  arts[SPD_UP_ART].base_amt = 3;
  arts[SPD_UP_ART].move_type = MOVE_ART_SUPPORT;

  arts[6].frame_count = 11;
  arts[6].name = "Capture  ";
  arts[6].cost = 1;
  arts[6].target = SINGLE_HIT;
  arts[6].relationship = ONE_TO_ONE;
  arts[6].stunning = 0;

  arts[POWER_WAVE_ART].frame_count = 11;
  arts[POWER_WAVE_ART].name = "P.Wave  ";
  arts[POWER_WAVE_ART].cost = 1;
  arts[POWER_WAVE_ART].target = MULTI_ROW;
  arts[POWER_WAVE_ART].relationship = ONE_TO_MANY;
  arts[POWER_WAVE_ART].stunning = 1;
  arts[POWER_WAVE_ART].move_type = MOVE_ART_ATTACK;

  arts[8].frame_count = 0;
  arts[8].name = "Blow Back";
  arts[8].cost = 1;
  arts[8].target = SINGLE_HIT;
  arts[8].relationship = ONE_TO_ONE;
  arts[8].stunning = 1;

  arts[9].frame_count = 11;
  arts[9].name = "Cleave   ";
  arts[9].cost = 1;
  arts[9].target = MULTI_COL_3;
  arts[9].relationship = ONE_TO_MANY;
  arts[9].stunning = 1;

  arts[HEAL_ART].frame_count = 11;
  arts[HEAL_ART].name = "Heal    ";
  arts[HEAL_ART].cost = 1;
  arts[HEAL_ART].target = HEAL;
  arts[HEAL_ART].relationship = NONE_TO_NONE;
  arts[HEAL_ART].stunning = 0;
  arts[HEAL_ART].base_amt = 20;
  arts[HEAL_ART].move_type = MOVE_HEAL;

  arts[11].frame_count = 0;
  arts[11].name = "Heal All";
  arts[11].cost = 1;
  arts[11].target = HEAL_ALL;
  arts[11].relationship = NONE_TO_NONE;
  arts[11].stunning = 0;
  arts[11].move_type = MOVE_HEAL;

  arts[12].frame_count = 5;
  arts[12].name = "Run Thru ";
  arts[12].cost = 1;
  arts[12].target = MULTI_ROW;
  arts[12].relationship = ONE_TO_MANY;
  arts[12].stunning = 1;

  arts[13].frame_count = 5;
  arts[13].name = "Ftl Shot ";
  arts[13].cost = 1;
  arts[13].target = //SINGLE_HIT;
  arts[13].relationship = ONE_TO_MANY;
  arts[13].stunning = 0;

  arts[14].frame_count = 5;
  arts[14].name = "Bombard  ";
  arts[14].cost = 1;
  arts[14].target = ALL_OPPONENTS;
  arts[14].relationship = ONE_TO_MANY;
  arts[14].stunning = 0;

  arts[15].frame_count = 5;
  arts[15].name = "Track    ";
  arts[15].cost = 1;
  arts[15].target = SINGLE_HIT;
  arts[15].relationship = ONE_TO_ONE;
  arts[15].stunning = 0;

  arts[16].frame_count = 11;
  arts[16].name = "Renew    ";
  arts[16].cost = 2;
  arts[16].target = ALL_ALLIES;
  arts[16].relationship = ONE_TO_ONE;
  arts[16].stunning = 0;

  arts[17].frame_count = 5;
  arts[17].name = "Restore  ";
  arts[17].cost = 1;
  arts[17].target = ALL_ALLIES;
  arts[17].relationship = ONE_TO_ONE;
  arts[17].stunning = 0;

  arts[18].frame_count = 0;
  arts[18].name = "Red Eye  ";
  arts[18].cost = 1;
  arts[18].target = SINGLE_HIT;
  arts[18].relationship = ONE_TO_ONE;
  arts[18].stunning = 1;

  arts[19].frame_count = 0;
  arts[19].name = "Black Eye";
  arts[19].cost = 1;
  arts[19].target = SINGLE_HIT;
  arts[19].relationship = ONE_TO_ONE;
  arts[19].stunning = 1;

  arts[RANGED_SINGLE_ATTACK].frame_count = 0;
  arts[RANGED_SINGLE_ATTACK].name = "Range S";
  arts[RANGED_SINGLE_ATTACK].cost = 1;
  arts[RANGED_SINGLE_ATTACK].target = SINGLE_HIT;
  arts[RANGED_SINGLE_ATTACK].relationship = NONE_TO_NONE;
  arts[RANGED_SINGLE_ATTACK].stunning = 1;
  arts[RANGED_SINGLE_ATTACK].base_amt = 10;
  arts[RANGED_SINGLE_ATTACK].move_type = MOVE_RANGED_ATTACK;

  arts[RANGED_COLUMN_ATTACK].frame_count = 0;
  arts[RANGED_COLUMN_ATTACK].name = "Range C";
  arts[RANGED_COLUMN_ATTACK].cost = 1;
  arts[RANGED_COLUMN_ATTACK].target = MULTI_COL_3;
  arts[RANGED_COLUMN_ATTACK].relationship = NONE_TO_NONE;
  arts[RANGED_COLUMN_ATTACK].stunning = 1;
  arts[RANGED_COLUMN_ATTACK].base_amt = 7;
  arts[RANGED_COLUMN_ATTACK].move_type = MOVE_RANGED_ATTACK;

  arts[RANGED_ROW_ATTACK].frame_count = 0;
  arts[RANGED_ROW_ATTACK].name = "Range R";
  arts[RANGED_ROW_ATTACK].cost = 1;
  arts[RANGED_ROW_ATTACK].target = MULTI_ROW;
  arts[RANGED_ROW_ATTACK].relationship = NONE_TO_NONE;
  arts[RANGED_ROW_ATTACK].stunning = 1;
  arts[RANGED_ROW_ATTACK].base_amt = 7;
  arts[RANGED_ROW_ATTACK].move_type = MOVE_RANGED_ATTACK;

  arts[PHYSICAL_SINGLE_ATTACK].frame_count = 0;
  arts[PHYSICAL_SINGLE_ATTACK].name = "Melee S";
  arts[PHYSICAL_SINGLE_ATTACK].cost = 0;
  arts[PHYSICAL_SINGLE_ATTACK].target = SINGLE_HIT;
  arts[PHYSICAL_SINGLE_ATTACK].relationship = NONE_TO_NONE;
  arts[PHYSICAL_SINGLE_ATTACK].stunning = 1;
  arts[PHYSICAL_SINGLE_ATTACK].move_type = MOVE_PHYSICAL_ATTACK;
  arts[PHYSICAL_SINGLE_ATTACK].base_amt = 10;

  arts[PHYSICAL_COLUMN_ATTACK].frame_count = 0;
  arts[PHYSICAL_COLUMN_ATTACK].name = "Melee C";
  arts[PHYSICAL_COLUMN_ATTACK].cost = 0;
  arts[PHYSICAL_COLUMN_ATTACK].target = MULTI_COL_3;
  arts[PHYSICAL_COLUMN_ATTACK].relationship = NONE_TO_NONE;
  arts[PHYSICAL_COLUMN_ATTACK].stunning = 1;
  arts[PHYSICAL_COLUMN_ATTACK].move_type = MOVE_PHYSICAL_ATTACK;
  arts[PHYSICAL_COLUMN_ATTACK].base_amt = 7;

  arts[PHYSICAL_ROW_ATTACK].frame_count = 0;
  arts[PHYSICAL_ROW_ATTACK].name = "Melee R";
  arts[PHYSICAL_ROW_ATTACK].cost = 0;
  arts[PHYSICAL_ROW_ATTACK].target = MULTI_ROW;
  arts[PHYSICAL_ROW_ATTACK].relationship = NONE_TO_NONE;
  arts[PHYSICAL_ROW_ATTACK].stunning = 1;
  arts[PHYSICAL_ROW_ATTACK].move_type = MOVE_PHYSICAL_ATTACK;
  arts[PHYSICAL_ROW_ATTACK].base_amt = 7;
  
  arts[PHYSICAL_ALL_ATTACK].frame_count = 0;
  arts[PHYSICAL_ALL_ATTACK].name = "All      ";
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

    case PLUNDER_ART:
    create_atk_up(x,y);
    break;

    case SPD_UP_ART:
    create_def_up(x,y);
    break;

    case RAPID_THRUST_ART:
    create_atk_3(x,y);
    break;

    case POWER_WAVE_ART:
    create_power_wave(x,y,flip);
    break;

    case INNVIGORATE_ART:
    create_healing(x,y,flip);
    break;

    // case JUDGEMENT_ART:
    // create_healing(x+8,y+16,flip);
    // break;

    case RAIN_ARROW_ART:
    create_arrows(x,y,flip);
    break;

    case CAPTURE_ART:
    create_capture(x,y,flip);
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
    case PLUNDER_ART: return 5;
    case SPD_UP_ART: return 10;
    case POWER_WAVE_ART: return 20;
    case BLOW_BACK_ART: return 10;
    case CLEAVE_ART: return 10;
    // case FRENZY_ART: return 10;
    case RUN_THROUGH_ART: return 15;
    case BOMBARD_ART: return 15;
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
    case PLUNDER_ART: return MOD_ATK;
    case SPD_UP_ART: return MOD_SPD;
    case POWER_WAVE_ART: return MOD_HP;
    case BLOW_BACK_ART: return MOD_HP;
    case CLEAVE_ART: return MOD_HP;
    // case FRENZY_ART: return MOD_HP;
    case RUN_THROUGH_ART: return MOD_HP;
    case BOMBARD_ART: return MOD_HP;
    // case SEDUCE_ART: return MOD_DEF;
  }
  return MOD_NONE;
}