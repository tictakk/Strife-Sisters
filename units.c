/* sprites */
#incspr(attack, "map/sprites/swordy.pcx")
#incpal(soldierpal, "map/sprites/swordy.pcx",0,2)
#incspr(blobbattle, "map/sprites/blobbattle.pcx")
#incpal(blobbattlepal, "map/sprites/blobbattle.pcx",0,2)
#incspr(attack2, "map/sprites/spear_anim.pcx")
#incpal(spearpal, "map/sprites/spear_anim.pcx",0,2)
#incspr(axebtl, "characters/axeunit.pcx")
#incpal(axebtlpal, "characters/axeunit.pcx",0,2)
#incspr(musketbtl, "map/sprites/archer.pcx")
#incpal(musketbtlpal, "map/sprites/archer.pcx",0,2)
#incspr(demonbtl, "map/sprites/demon.pcx")
#incpal(demonbtlpal, "map/sprites/demon.pcx",0,2)
#incspr(houndbtl, "map/sprites/hound_big.pcx")
#incpal(houndbtlpal, "map/sprites/hound_big.pcx",0,2)
#incspr(magebtl, "map/sprites/magebattle.pcx")
#incpal(magebtlpal, "map/sprites/magebattle.pcx",0,2)
#incspr(knightbtl, "map/sprites/knightbattle.pcx")
#incspr(monkbtl, "map/sprites/monkbtl.pcx")
#incpal(monkbtlpal, "map/sprites/monkbtl.pcx",0,2)
#incspr(lancerbtl, "map/sprites/lancerbattle.pcx")

#incchr(portraitgfx, "map/sprites/portrait.pcx", 0, 0, 4, 4)
#incchr(portraitm, "map/sprites/portraitm.pcx", 0, 0, 4, 4)
#incpal(portraitpal, "map/sprites/portrait.pcx")
#incpal(portraitmpal, "map/sprites/portraitm.pcx")

#define MAX_UNIT_COUNT 29

//attack types
#define NORMAL 0
#define PIERCE 1
#define AXE 2
#define MISSILE 3
#define MAGIC 4
#define UNARMED 5
#define NONE 6

//advantage types
#define NO_ADV 0
#define ADVANTAGE 1
#define DISADVANTAGE 2

#define NO_ATTACK 0
#define SINGLE_HIT 1
#define HEAL 2
#define MULTI_ROW 3
#define MULTI_COL_2 4
#define MULTI_COL_3 5
#define MULTI_ATTACK_AOE 6
#define MULTI_HEAL_AOE 7
#define PARALYZE 8
#define CONFUSE 9
#define ALL 10

#define MAX_BUYABLE_UNITS 20
#define TYPE_COUNT 6

typedef struct{
	unsigned char atk, def, mov, id, a_type,
                rng, ign, spd, points, bonus_col;

  char attacks[3];
	int hp;
} Unit;

typedef struct{
  Unit *unit;
  int hp;
} Unit_Entity;

const char buyable_units[] = { SWORD_UNIT, SPEAR_UNIT, AXE_UNIT, MAGE_UNIT, LANCER_UNIT, ARCHER_UNIT, HOUND_UNIT,
                              MONK_UNIT, BERSERKER};

Unit unit_list[MAX_UNIT_COUNT];
unsigned char unit_entity_count = 0;
char unlocked_units[MAX_UNIT_COUNT];
char buyable_unit_count = 0;

void initialize_units()
{
  char i;
  buyable_unit_count = 9;

	for(i=0; i<MAX_UNIT_COUNT; i++)
  {
    unit_list[i].hp = 25;
    unit_list[i].atk = 20;
    unit_list[i].def = 10;
    unit_list[i].spd = 15;
    unit_list[i].bonus_col = 0;
    unit_list[i].points = 1;
    unit_list[i].rng = 1;
    unit_list[i].mov = 3;
    unit_list[i].ign = 0;
    unit_list[i].id = i;
    unit_list[i].a_type = NONE;
    unit_list[i].attacks[0] = SINGLE_HIT;
    unit_list[i].attacks[1] = SINGLE_HIT;
    unit_list[i].attacks[2] = SINGLE_HIT;
    unlocked_units[i] = 0;
  }
  unit_list[SPEAR_UNIT].hp  = 27;
  unit_list[SPEAR_UNIT].atk = 23;
  unit_list[SPEAR_UNIT].def = 12;
  unit_list[SPEAR_UNIT].rng = 2;
  unit_list[SPEAR_UNIT].mov = 2;
  unit_list[SPEAR_UNIT].bonus_col = 1;
  unit_list[SPEAR_UNIT].spd = 15;
  unit_list[SPEAR_UNIT].ign = 0;
  unit_list[SPEAR_UNIT].id = SPEAR_UNIT;
  unit_list[SPEAR_UNIT].a_type = PIERCE;
  unlocked_units[SPEAR_UNIT] = 1;

  unit_list[LANCER_UNIT].hp  = 29;
  unit_list[LANCER_UNIT].atk = 25;
  unit_list[LANCER_UNIT].def = 14;
  unit_list[LANCER_UNIT].rng = 2;
  unit_list[LANCER_UNIT].mov = 3;
  unit_list[LANCER_UNIT].bonus_col = 1;
  unit_list[LANCER_UNIT].spd = 15;
  unit_list[LANCER_UNIT].ign = 0;
  unit_list[LANCER_UNIT].id = LANCER_UNIT;
  unit_list[LANCER_UNIT].a_type = PIERCE;
  unlocked_units[LANCER_UNIT] = 1;

  unit_list[SWORD_UNIT].hp  = 34; //0x2BCE
  unit_list[SWORD_UNIT].atk = 21;
  unit_list[SWORD_UNIT].def = 15;
  unit_list[SWORD_UNIT].mov = 3;
  unit_list[SWORD_UNIT].spd = 13;
  unit_list[SWORD_UNIT].rng = 1;
  unit_list[SWORD_UNIT].ign = 0;
  unit_list[SWORD_UNIT].id = SWORD_UNIT;
  unit_list[SWORD_UNIT].a_type = NORMAL;
  unit_list[SWORD_UNIT].bonus_col = 2;
  unit_list[SWORD_UNIT].attacks[2] = NO_ATTACK;
  unlocked_units[SWORD_UNIT] = 1;

  unit_list[ARCHER_UNIT].atk = 25;
  unit_list[ARCHER_UNIT].hp  = 24;
  unit_list[ARCHER_UNIT].def = 12;
  unit_list[ARCHER_UNIT].rng = 2;
  unit_list[ARCHER_UNIT].mov = 3;
  unit_list[ARCHER_UNIT].spd = 11;
  unit_list[ARCHER_UNIT].bonus_col = 2;
  unit_list[ARCHER_UNIT].ign = 0;
  unit_list[ARCHER_UNIT].id = ARCHER_UNIT;
  unit_list[ARCHER_UNIT].a_type = MISSILE;
  unlocked_units[ARCHER_UNIT] = 1;

	unit_list[HOUND_UNIT].atk = 23;
	unit_list[HOUND_UNIT].def = 13;
	unit_list[HOUND_UNIT].hp  = 23;
	unit_list[HOUND_UNIT].ign = 0;
  unit_list[HOUND_UNIT].bonus_col = 1;
  unit_list[HOUND_UNIT].mov = 4;
	unit_list[HOUND_UNIT].id = HOUND_UNIT;
	unit_list[HOUND_UNIT].a_type = NORMAL;
  unlocked_units[HOUND_UNIT] = 1;

	unit_list[BLOB_UNIT].atk = 20; //0x2C1B
	unit_list[BLOB_UNIT].def = 13;
	unit_list[BLOB_UNIT].hp  = 25;
	unit_list[BLOB_UNIT].ign = 0;
  unit_list[BLOB_UNIT].spd = 17;
  unit_list[BLOB_UNIT].mov = 3;
	unit_list[BLOB_UNIT].rng = 1;
	unit_list[BLOB_UNIT].id = BLOB_UNIT;
	unit_list[BLOB_UNIT].a_type = UNARMED;

	unit_list[AXE_UNIT].atk = 22;
	unit_list[AXE_UNIT].def = 13;
	unit_list[AXE_UNIT].hp  = 27;
	unit_list[AXE_UNIT].ign = 0;
  unit_list[AXE_UNIT].bonus_col = 0;
  unit_list[AXE_UNIT].mov = 3;
	unit_list[AXE_UNIT].id = AXE_UNIT;
	unit_list[AXE_UNIT].a_type = AXE;

  unit_list[BERSERKER].atk = 32;
  unit_list[BERSERKER].def = 14;
  unit_list[BERSERKER].hp  = 30;
  unit_list[BERSERKER].ign = 0;
  unit_list[BERSERKER].bonus_col = 0;
  unit_list[BERSERKER].mov = 3;
  unit_list[BERSERKER].id = BERSERKER;
  unit_list[BERSERKER].a_type = AXE;

  unit_list[MAGE_UNIT].atk = 18;
  unit_list[MAGE_UNIT].hp = 16;
  unit_list[MAGE_UNIT].def = 11;
  unit_list[MAGE_UNIT].hp = 24;
  unit_list[MAGE_UNIT].spd = 12;
  unit_list[MAGE_UNIT].bonus_col = 1;
  unit_list[MAGE_UNIT].ign = 0;
  unit_list[MAGE_UNIT].mov = 3;
  unit_list[MAGE_UNIT].id = MAGE_UNIT;
  unit_list[MAGE_UNIT].a_type = MAGIC;
  unit_list[MAGE_UNIT].attacks[1] = HEAL;
  unit_list[MAGE_UNIT].attacks[2] = HEAL;
  unit_list[MAGE_UNIT].attacks[0] = NO_ATTACK;

  unit_list[DEMON_UNIT].hp  = 22;
	unit_list[DEMON_UNIT].atk = 22;
	unit_list[DEMON_UNIT].def = 13;
	unit_list[DEMON_UNIT].rng = 1;
	unit_list[DEMON_UNIT].mov = 4;
  unit_list[DEMON_UNIT].bonus_col = 1;
  unit_list[DEMON_UNIT].ign = 1;
	unit_list[DEMON_UNIT].id = DEMON_UNIT;
	unit_list[DEMON_UNIT].a_type = NORMAL;

  unit_list[MONK_UNIT].hp  = 23;
  unit_list[MONK_UNIT].atk = 23;
  unit_list[MONK_UNIT].def = 15;
  unit_list[MONK_UNIT].rng = 1;
  unit_list[MONK_UNIT].mov = 3;
  unit_list[MONK_UNIT].bonus_col = 0;
  unit_list[MONK_UNIT].ign = 1;
  unit_list[MONK_UNIT].id = MONK_UNIT;
  unit_list[MONK_UNIT].a_type = UNARMED;

//  unlocked_units[DEMON_UNIT] = 1;

}

//0 = no advantage, 1 = advantage, 2 = disavantage
char check_advantage(char a_type, char d_type)
{
  return 0;
}

void print_unit_info(Unit_Entity *ue, char x, char y)
{
  int hp;
  if(!ue->hp)
  {
    put_string("   ",x,y);
    put_string("   ",x,y+1);
    put_string("   ",x,y+2);
    return;
  }
  hp = ue->hp * 100;
//  print_unit_attack_icon(ue->unit->id,x+1,y);
  print_unit_type(ue->unit->id,x,y);
//  put_char('M',x,y+1);
//  put_number(ue->unit->meter,2,x+1,y+1);
  put_char('%',x,y+2);
  put_number(hp/ue->unit->hp,2,x+1,y+2);
}

void print_unit_stats(char unit_id, char x, char y)
{
  put_string("HP ",x,y);
  put_number(unit_list[unit_id].hp,2,x+3,y);

  put_string("ATK ",x,y+1);
  put_number(unit_list[unit_id].atk,2,x+4,y+1);

  put_string("DEF ",x,y+2);
  put_number(unit_list[unit_id].def,2,x+4,y+2);

  put_string("SPD ",x,y+3);
  put_number(unit_list[unit_id].spd,2,x+4,y+3);

  put_string("PTS ",x, y+4);
  put_number(unit_list[unit_id].points,2,x+4,y+4);

  put_string("Type ",x, y+5);
  print_unit_attack_icon(unit_list[unit_id].id,x+5,y+5);
}

void print_attack_type(char unit_id, char row, char x, char y)
{
  set_font_pal(8);
  switch(unit_list[unit_id].attacks[row])
  {
    case SINGLE_HIT:
      put_char('[',x,y);
      put_string("Melee  ",x+1,y);
      break;

    case HEAL:
      put_char('[',x,y);
      put_string("Heal  ",x+1,y);
      break;

    case MULTI_ROW:
      put_char('\\',x,y);
      put_string("Melee  ",x+1,y);
      break;

    case NO_ATTACK:
      put_char('_',x,y);
      put_string("None  ",x+1,y);
      break;

    default:
      put_char('`',x,y);
      put_string("All   ",x+1,y);
      break;
  }
  set_font_pal(10);
}

void print_unit_advantage_position(char unit_id, char x, char y)
{
  set_font_pal(8);
  switch(unit_list[unit_id].bonus_col)
  {
    case 0:
      put_string("Front ",x,y);
      break;

    case 1:
      put_string("Middle",x,y);
      break;

    case 2:
      put_string("Rear  ",x,y);
      break;
  }
  set_font_pal(10);
}

void unlock_unit(char unit_id)
{
  unlocked_units[unit_id] = 1;
}