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
#incspr(witchbtl,"characters/witchbattle.pcx")
#incspr(priestbtl,"characters/priest.pcx")
#incpal(witchbtlpal,"characters/witchbattle.pcx")
#incspr(knightbtl, "map/sprites/knight_battle.pcx")
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
// #incspr(raiderbtl,"characters/piratebtl.pcx")
// #incpal(raiderbtlpal,"characters/piratebtl.pcx")

#incchr(portraitgfx, "map/sprites/portrait.pcx", 0, 0, 4, 4)
#incchr(portraitm, "map/sprites/portraitm.pcx", 0, 0, 4, 4)
#incpal(portraitpal, "map/sprites/portrait.pcx")
#incpal(portraitmpal, "map/sprites/portraitm.pcx")

#define MAX_UNIT_COUNT 29

//attack types
#define NORMAL  0b00000001 //sword
#define PIERCE  0b00000010 //spear
#define AXE     0b00000100 //axe
#define MISSILE 0b00001000 //bow
#define MAGIC   0b00010000 //magic
#define UNARMED 0b00100000 //bare
#define NONE    0b00000000 //none, for beasts?

//advantage types
#define NO_ADV 0
#define ADVANTAGE 1
#define DISADVANTAGE 2

#define MAX_BUYABLE_UNITS 20
#define TYPE_COUNT 6

//hp, atk, def, int, res, spd, mov, sta
typedef struct{
	unsigned char hp, atk, def, intel, res, spd, mov, sta, rng, id, a_type,
                art, points, pow, is_cmdr;
  char attacks[3];
  char growth_chart;
  char bank; int address;
	// int hp;
} Unit;

typedef struct{
  char id;
  unsigned char hp, sta;
  int exp;
  char level;
} Unit_Entity;

const char attack_types[] = { 1, 2, 4, 8, 16, 32, 0 };

const char buyable_units[] = { SWORD_UNIT, SPEAR_UNIT, AXE_UNIT, MAGE_UNIT, LANCER_UNIT, ARCHER_UNIT, STALKER_UNIT, KNIGHT_UNIT,
                              PALADIN_UNIT, MONK_UNIT, FIGHTER_UNIT, BRAWLER_UNIT, BERSERKER_UNIT, CLERIC_UNIT, WITCH_UNIT,
                              BLACK_MAGE_UNIT, GOLEM_UNIT, HOUND_UNIT, RAIDER_UNIT
                              };

// Unit unit_list[MAX_UNIT_COUNT+3];
Unit unit_header[2];
char unlocked_units[MAX_UNIT_COUNT];
unsigned char unit_entity_count = 0;
char buyable_unit_count = 0;
int upgrade_cost = 0;
int unit_cost = 0;

void display_unit_types_row(char x, char y)
{
  put_string("SWD",x,y);
  put_string("POL",x+4,y);
  put_string("AXE ",x+8,y);
  put_string("BOW",x+12,y);
  put_string("MAG",x+16,y);
  put_string("FST",x+20,y);
  put_string("BEA",x+24,y);
}

//0 = no advantage, 1 = advantage
unsigned char check_advantage(unsigned char unit_1, unsigned char unit_2)
{
  return get_weapon_adv(unit_1) & unit_2;
}

char get_weapon_adv(unsigned char weapon_type)
{
  switch(weapon_type)
  {
    case NORMAL:
    return MISSILE;

    case MISSILE:
    return PIERCE;

    case AXE:
    return MAGIC;

    case PIERCE:
    return UNARMED;

    case MAGIC:
    return NORMAL;

    case UNARMED:
    return AXE;
  }
  return NONE;
}

void print_unit_info(Unit_Entity *ue, char x, char y)
{
  int hp;
  if(!ue->hp)
  {
    put_string("    ",x,y);
    put_string("    ",x,y+1);
    put_string("    ",x,y+2);
    return;
  }
  hp = ue->hp * 100;
  print_unit_attack_icon(ue->id,x,y);
  print_unit_type(ue->id,x+1,y);
  put_char('M',x,y+1);
  put_number(ue->level,2,x+1,y+1);
  put_char('%',x,y+2);
  put_number(hp/ue->hp,3,x+1,y+2);
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

    print_unit_fullname(unit_id,x,y);

    put_string("LV",x,y+1);
    put_number(level,2,x+3,y+1);

    // put_string("HP ",x,y+2);
    // put_number(unit_header[0].hp,2,x+3,y+);

    put_string("ATK ",x,y+2);
    put_number(unit_header[0].atk,2,x+4,y+2);

    put_string("DEF ",x,y+3);
    put_number(unit_header[0].def,2,x+4,y+3);

    // put_string("SPD ",x,y+3);
    // put_number(unit_header[0].spd,2,x+4,y+3);

    put_string("PTS ",x, y+4);
    put_number(unit_header[0].points,2,x+4,y+4);

    put_string("Type ",x, y+5);
    print_unit_attack_icon(unit_header[0].id,x+5,y+5);
  }
  else
  {
    put_string("       ",x,y);

    put_string("       ",x,y+1);

    put_string("       ",x,y+2);

    put_string("       ",x,y+3);

    put_string("       ",x, y+4);

    put_string("       ",x, y+5);
  }
}

// void print_attack_type(char attack_type, char row, char x, char y)
void print_attack_type(char attack_no, char row, char x, char y)
{
  set_font_pal(10);

  switch(arts[attack_no].target)
  {
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
    case ARCHER_UNIT: unit_cost = 150; break;
    case SNIPER_UNIT: unit_cost = 300; break;
    case STALKER_UNIT: unit_cost = 600; break;
    case WITCH_UNIT: unit_cost = 450; break;
    case PRIEST_UNIT: unit_cost = 450; break;
    case FIGHTER_UNIT: unit_cost = 100; break;
    case BRAWLER_UNIT: unit_cost = 200; break;
    case MONK_UNIT: unit_cost = 500; break;
    case RAIDER_UNIT: unit_cost = 100; break;
    case THIEF_UNIT: unit_cost = 200; break;
    default: unit_cost = 100;
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