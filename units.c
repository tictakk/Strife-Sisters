#include "stats.c"

// #include "game_data/units/soldier.asm"
/* sprites */
#incspr(attack, "map/sprites/swordy.pcx")
#incpal(soldierpal, "map/sprites/swordy.pcx")
#incspr(blobbattle, "map/sprites/blobbattle.pcx")
#incpal(blobbattlepal, "map/sprites/blobbattle.pcx")
#incspr(attack2, "map/sprites/spear_anim.pcx")
#incpal(spearpal, "map/sprites/spear_anim.pcx")
#incspr(axebtl, "characters/axeunit.pcx")
#incpal(axebtlpal, "characters/axeunit.pcx")
#incspr(sniperbtl, "characters/sniper.pcx")
#incspr(berserkerbtl, "characters/berserker.pcx")
#incpal(sniperbtlpal, "characters/sniper.pcx")
#incspr(golembtl, "characters/golem_good.pcx")
#incpal(golempal, "characters/golem_good.pcx")
#incspr(musketbtl, "map/sprites/archer.pcx")
#incpal(musketbtlpal, "map/sprites/archer.pcx")
#incspr(stalkerbtl,"characters/stalker.pcx")
#incpal(stalkerpal,"characters/stalker.pcx")
#incspr(demonbtl, "map/sprites/demon.pcx")
#incpal(demonbtlpal, "map/sprites/demon.pcx")
#incspr(houndbtl, "map/sprites/hound_big.pcx")
#incpal(houndbtlpal, "map/sprites/hound_big.pcx")
#incspr(magebtl, "map/sprites/magebattle.pcx")
#incpal(magebtlpal, "map/sprites/magebattle.pcx",0,3)
#incspr(witchbtl,"characters/witch_big.pcx")
#incspr(dancer_big,"characters/dancer_big.pcx")

#incpal(dancerbigpal,"characters/dancer_big.pcx")
#incpal(witchbtlpal,"characters/witch_big.pcx")
#incspr(knightbtl, "characters/knight_big.pcx")
#incspr(paladinbtl, "characters/paladin_battle.pcx")
#incpal(paladinpal, "characters/paladin_battle.pcx")
#incspr(monkbtl, "map/sprites/monkbtl.pcx") 
#incpal(monkbtlpal, "map/sprites/monkbtl.pcx")
#incspr(fighterbtl, "characters/fighter.pcx")
#incspr(brawlerbtl, "characters/brawler.pcx")
#incspr(lancerbtl, "map/sprites/lancerbattle.pcx")
#incspr(raiderbtl,"characters/banditbattle.pcx")
#incpal(raiderbtlpal,"characters/banditbattle.pcx")
#incspr(thiefbtl,"characters/thiefbattle.pcx")

#incpal(enemypal,"map/sprites/spear_anim.pcx")
// #incspr(raiderbtl,"characters/piratebtl.pcx")
// #incpal(raiderbtlpal,"characters/piratebtl.pcx")

#incchr(portraitgfx, "map/sprites/portrait.pcx", 0, 0, 4, 4)
#incchr(portraitm, "map/sprites/portraitm.pcx", 0, 0, 4, 4)
#incpal(portraitpal, "map/sprites/portrait.pcx")
#incpal(portraitmpal, "map/sprites/portraitm.pcx")

#define MAX_UNIT_COUNT 29

#define ICON_NO_TARGET 0
#define ICON_SINGLE_TARGET 1
#define ICON_ROW_TARGET 2
#define ICON_COLUMN_TARGET 3
#define ICON_ALL_TARGET 4

//attack types
#define NORMAL  0b00000001 //sword
#define PIERCE  0b00000010 //spear
#define AXE     0b00000100 //axe
#define MISSILE 0b00001000 //bow
#define MAGIC   0b00010000 //magic
#define UNARMED 0b00100000 //bare
#define BEAST   0b01000000
#define NONE    0b00000000 //none, for beasts?

//advantage types
#define NO_ADV 0
#define ADVANTAGE 1
#define DISADVANTAGE 2

// #define MAX_BUYABLE_UNITS 20
#define TYPE_COUNT 6

//hp, atk, def, int, res, spd, mov, sta
typedef struct{
	unsigned char hp, atk, def, intel, res, spd, mov, sta, rng, id, a_type,
                cmdr_bonus, points, pow, is_cmdr;
  char attacks[3];
  char growth_chart;
  char bank; int address;
} Unit;

typedef struct{
  char id;
  unsigned char hp, sta;
  int exp;
  char level; 
} Unit_Entity;

const char attack_types[] = { 1, 2, 4, 8, 16, 32, 0 };

// const char buyable_units[] = { SWORD_UNIT, SPEAR_UNIT, AXE_UNIT, MAGE_UNIT, LANCER_UNIT, ARCHER_UNIT, STALKER_UNIT, KNIGHT_UNIT,
//                               PALADIN_UNIT, MONK_UNIT, FIGHTER_UNIT, BRAWLER_UNIT, BERSERKER_UNIT, CLERIC_UNIT, WITCH_UNIT,
//                               BLACK_MAGE_UNIT, GOLEM_UNIT, HOUND_UNIT, RAIDER_UNIT
//                               };

// Unit unit_list[MAX_UNIT_COUNT+3];
Unit unit_header[2];
char unlocked_units[MAX_UNIT_COUNT];
unsigned char unit_entity_count = 0;
char buyable_unit_count = 0;
int upgrade_cost = 0;
int unit_cost = 0;
char adv_plus_plus, disadv_minus_minus;
char adv_plus[2], disadv_minus[2];

char adv_plus_flag = 0;
char adv_plus_plus_flag = 0;

void display_unit_types_row(char x, char y)
{
  put_string("SWD",x,y);
  put_string("POL",x+4,y);
  put_string("AXE ",x+8,y);
  put_string("BOW",x+12,y);
  put_string("MAG",x+16,y);
  put_string("UNA",x+20,y);
  put_string("BEA",x+24,y);
}

//0 = no advantage, 1 = advantage
// unsigned char check_advantage(unsigned char unit_1, unsigned char unit_2)
// {
//   return get_weapon_adv(unit_1) & unit_2;
// }

void check_advantage_plus(char target_type)
{
  adv_plus_flag = ((adv_plus[0] == target_type) || (adv_plus[1] == target_type));
}

void check_advantage_plus_plus(char target_type)
{
  adv_plus_plus_flag = (adv_plus_plus == target_type);
}

void get_advantages(unsigned char weapon_type)
{
  get_plus_plus_adv(weapon_type);
  get_plus_adv(weapon_type);
}

char get_plus_plus_adv(unsigned char weapon_type)
{
  switch(weapon_type)
  {
    case NORMAL:
    adv_plus_plus = BEAST;
    disadv_minus_minus = PIERCE;
    break;

    case PIERCE:
    adv_plus_plus = NORMAL;
    disadv_minus_minus = BEAST;
    break;

    case AXE:
    adv_plus_plus = MISSILE;
    disadv_minus_minus = UNARMED;
    break;

    case MISSILE:
    adv_plus_plus = MAGIC;
    disadv_minus_minus = AXE;
    break;

    case MAGIC:
    adv_plus_plus = UNARMED;
    disadv_minus_minus = MISSILE;
    break;

    case UNARMED:
    adv_plus_plus = AXE;
    disadv_minus_minus = MAGIC;
    break;

    case BEAST:
    adv_plus_plus = PIERCE;
    disadv_minus_minus = NORMAL;
    break;

    default:
    adv_plus_plus = NONE;
    disadv_minus_minus = NONE;
    break;
  }
}

void get_plus_adv(unsigned char weapon_type)
{
  switch(weapon_type)
  {
    case NORMAL:
    adv_plus[0] = MISSILE;
    adv_plus[1] = UNARMED;
    disadv_minus[0] = MAGIC;
    disadv_minus[1] = AXE;
    break;

    case PIERCE:
    adv_plus[0] = AXE;
    adv_plus[1] = MAGIC;
    disadv_minus[0] = MISSILE;
    disadv_minus[1] = UNARMED;
    break;

    case AXE:
    adv_plus[0] = NORMAL;
    adv_plus[1] = BEAST;
    disadv_minus[0] = PIERCE;
    disadv_minus[1] = MAGIC;
    break;

    case MISSILE:
    adv_plus[0] = PIERCE;
    adv_plus[1] = UNARMED;
    disadv_minus[0] = NORMAL;
    disadv_minus[1] = BEAST;
    break;

    case MAGIC:
    adv_plus[0] = NORMAL;
    adv_plus[1] = AXE;
    disadv_minus[0] = PIERCE;
    disadv_minus[1] = BEAST;
    break;

    case UNARMED:
    adv_plus[0] = PIERCE;
    adv_plus[1] = BEAST;
    disadv_minus[0] = NORMAL;
    disadv_minus[1] = MISSILE;
    break;

    case BEAST:
    adv_plus[0] = MISSILE;
    adv_plus[1] = MAGIC;
    disadv_minus[0] = AXE;
    disadv_minus[1] = UNARMED;
    break;

    default:
    adv_plus[0] = NONE;
    adv_plus[1] = NONE;
    disadv_minus[0] = NONE;
    disadv_minus[1] = NONE;
    break;
  }
}


void print_unit_info(Unit_Entity *ue, char x, char y)
{
  if(!ue->hp)
  {
    put_string("    ",x,y);
    put_string("    ",x,y+1);
    put_string("    ",x,y+2);
    return;
  }
  load_unit_header(ue->id,0);
  apply_level_to_header(ue->level,0);
  // hp = ue->hp * 100;
  print_unit_attack_icon(unit_header[0].a_type,x,y);
  print_unit_type(ue->id,x+1,y);
  put_string("LV",x,y+1);
  put_number(ue->level,2,x+2,y+1);
  put_char('%',x,y+2);
  put_number(get_percentage(ue->hp,unit_header[0].hp),3,x+1,y+2);
}

void print_unit_stats(char unit_id, char x, char y, char level)
{

  if(unit_id > 0)
  {
    load_unit_header(unit_id,0);
    if(level > 1)
    {
      apply_level_to_header(level,0);
    }
    set_font_pal(GOLD_FONT);
    print_unit_fullname(unit_id,x,y);
    put_string("LV",x,y+1);
    put_string("HP",x,y+2);
    put_string("ATK",x,y+3);
    put_string("DEF",x,y+4);
    put_string("RES",x,y+5);
    put_string("SPD",x,y+6);
    set_font_pal(WHITE_FONT);
    // put_string("PTS ",x, y+4);
    // put_number(unit_header[0].points,2,x+2,y+4);

    put_number(level,2,x+2,y+1);
    put_number(unit_header[0].hp,3,x+2,y+2);
    put_number(unit_header[0].atk,2,x+3,y+3);
    put_number(unit_header[0].def,2,x+3,y+4);
    put_number(unit_header[0].res,2,x+3,y+5);
    put_number(unit_header[0].spd,2,x+3,y+6);
  }
  else
  {
    put_string("       ",x,y);

    put_string("       ",x,y+1);

    put_string("       ",x,y+2);

    put_string("       ",x,y+3);

    put_string("       ",x, y+4);

    put_string("       ",x, y+5);
    
    put_string("       ",x, y+6);
  }
}

// void print_attack_type(char attack_type, char row, char x, char y)
void print_attack_type(char attack_no, char row, char x, char y)
{
  set_font_pal(10);

  switch(arts[attack_no].target)
  {
    case HEAL:
    case SINGLE_HIT:
      put_char(':',x,y);
      // put_string("Single ",x+1,y);
      break;

    case MULTI_ROW:
      put_char('<',x,y);
      // put_string("Column ",x+1,y);
      break;

    case MULTI_COL_3:
      put_char(';',x,y);
      // put_string("Row    ",x+1,y);
      break;

    case NO_TARGET:
      put_char('=',x,y);
      // put_string("None  ",x+1,y);
      break;

    default:
      put_char('>',x,y);
      // put_string("All   ",x+1,y);
      break;
  }
  put_string(arts[attack_no].name,x+1,y);
  set_font_pal(10);
}

void print_target_type_icon(char attack_type, char x, char y)
{
  switch(attack_type)
  {
    case SINGLE_HIT:
      put_char(':',x,y);
      break;

    case MULTI_ROW:
      put_char('<',x,y);
      break;

    case MULTI_COL_3:
      put_char(';',x,y);
      break;

    case NO_TARGET:
      put_char('=',x,y);
      break;

    default:
      put_char('>',x,y);
      break;
  }
}

void unlock_unit(char unit_id)
{
  unlocked_units[buyable_unit_count++] = unit_id;
  // buyable_unit_count++;
}

void unlock_all_units()
{
  char i;
  for(i=0; i<MAX_UNIT_COUNT; i++)
  {
    unlock_unit(i);
  }
}

void get_upgrade_cost(char unit_id)
{
  switch(unit_id)
  {
    case SWORD_UNIT: upgrade_cost = 0; break;
    case SPEAR_UNIT: upgrade_cost = 100; break;
    case AXE_UNIT: upgrade_cost = 100; break;
    case KNIGHT_UNIT: upgrade_cost = 100; break;
    case BERSERKER_UNIT: upgrade_cost = 300; break;
    case LANCER_UNIT: upgrade_cost = 300; break;
    case PALADIN_UNIT: upgrade_cost = 350; break;
    case ARCHER_UNIT: upgrade_cost = 0; break;
    case SNIPER_UNIT: upgrade_cost = 150; break;
    case STALKER_UNIT: upgrade_cost = 300; break;
    case WITCH_UNIT: upgrade_cost = 300; break;
    case PRIEST_UNIT: upgrade_cost = 300; break;
    case FIGHTER_UNIT: upgrade_cost = 100; break;
    case BRAWLER_UNIT: upgrade_cost = 200; break;
    case MONK_UNIT: upgrade_cost = 400; break;
    case RAIDER_UNIT: upgrade_cost = 50; break;
    case THIEF_UNIT: upgrade_cost = 150; break;
  }
}

void get_unit_cost(char unit_id)
{
  switch(unit_id)
  {
    case SWORD_UNIT: unit_cost = 100; break;
    case SPEAR_UNIT: unit_cost = 250; break;
    case AXE_UNIT: unit_cost = 250; break;
    case KNIGHT_UNIT: unit_cost = 250; break;
    case BERSERKER_UNIT: unit_cost = 600; break;
    case LANCER_UNIT: unit_cost = 600; break;
    case PALADIN_UNIT: unit_cost = 650; break;
    case ARCHER_UNIT: unit_cost = 100; break;
    case SNIPER_UNIT: unit_cost = 300; break;
    case STALKER_UNIT: unit_cost = 600; break;
    case WITCH_UNIT: unit_cost = 450; break;
    case PRIEST_UNIT: unit_cost = 450; break;
    case FIGHTER_UNIT: unit_cost = 100; break;
    case BRAWLER_UNIT: unit_cost = 200; break;
    case MONK_UNIT: unit_cost = 500; break;
    case RAIDER_UNIT: unit_cost = 100; break;
    case THIEF_UNIT: unit_cost = 200; break;
    case DANCER_UNIT: unit_cost = 100; break;
    case CLERIC_UNIT: unit_cost = 250; break;
    default: unit_cost = 0;
  }
}

char level_up_unit(Unit_Entity *unit)
{
  while(unit->exp > next_level(unit->level))
  {
    unit->level++;
  }
  return unit->level;
}

void level_unit_to(Unit_Entity *unit, char level)
{
  int exp_required;
  if(level <= 1)
  {
    unit->exp = 0;
    unit->level = 1;
    return;
  }
  exp_required = next_level(level);
  unit->exp = exp_required;
  level_up_unit(unit);
}