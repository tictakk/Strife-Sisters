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
#define NONE    0b00000000

//advantage types
#define NO_ADV 0
#define ADVANTAGE 1
#define DISADVANTAGE 2

#define MAX_BUYABLE_UNITS 20
#define TYPE_COUNT 6

typedef struct{
	unsigned char atk, def, mov, id, a_type, art,
                rng, spd, points, pow;
	int hp;
} Unit;

typedef struct{
  Unit *unit;
  int hp;
} Unit_Entity;

const char buyable_units[] = { SWORD_UNIT, SPEAR_UNIT, AXE_UNIT, MAGE_UNIT, LANCER_UNIT, ARCHER_UNIT, STALKER_UNIT, KNIGHT_UNIT,
                              PALADIN_UNIT, MONK_UNIT, FIGHTER_UNIT, BRAWLER_UNIT, BERSERKER_UNIT, CLERIC_UNIT, WITCH_UNIT,
                              BLACK_MAGE_UNIT
                              };

Unit unit_list[MAX_UNIT_COUNT];
char unlocked_units[MAX_UNIT_COUNT];
unsigned char unit_entity_count = 0;
char buyable_unit_count = 0;
int upgrade_cost = 0;
int unit_cost = 0;

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
  print_unit_attack_icon(ue->unit->id,x,y);
  print_unit_type(ue->unit->id,x+1,y);
  put_char('M',x,y+1);
  put_number(0,3,x+1,y+1);
  put_char('%',x,y+2);
  put_number(hp/ue->unit->hp,3,x+1,y+2);
}

void print_unit_stats(char unit_id, char x, char y)
{
  if(unit_id)
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
  else
  {
    put_string("      ",x,y);

    put_string("      ",x,y+1);

    put_string("      ",x,y+2);

    put_string("      ",x,y+3);

    put_string("      ",x, y+4);

    put_string("      ",x, y+5);
  }
}

void print_attack_type(char attack_type, char row, char x, char y)
{
  set_font_pal(10);
  switch(attack_type)
  {
    case SINGLE_HIT:
      put_char(':',x,y);
      put_string("Single ",x+1,y);
      break;

    case MULTI_ROW:
      put_char('<',x,y);
      put_string("Column ",x+1,y);
      break;

    case MULTI_COL_2:
    case MULTI_COL_3:
      put_char(';',x,y);
      put_string("Row    ",x+1,y);
      break;

    case NO_TARGET:
      put_char('=',x,y);
      put_string("None  ",x+1,y);
      break;

    default:
      put_char('>',x,y);
      put_string("All   ",x+1,y);
      break;
  }
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

    case MULTI_COL_2:
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
  unlocked_units[unit_id] = 1;
}

void initialize_units()
{
  char i;
  buyable_unit_count = 16;

	for(i=0; i<MAX_UNIT_COUNT; i++)
  {
    unit_list[i].hp = 50;
    unit_list[i].atk = 20;
    unit_list[i].def = 10;
    unit_list[i].spd = 15;
    unit_list[i].points = 2;
    unit_list[i].rng = 1;
    unit_list[i].mov = 3;
    unit_list[i].id = i;
    // unit_list[i].art = POWER_WAVE_ART;
    // unit_list[i].art = ICE_ART;
    unit_list[i].art = NO_ART;
    unit_list[i].a_type = NONE;
    unlocked_units[i] = 0;
    // unit_list[i].exp = 4;
  }

  unit_list[0].hp  = 1;
  unit_list[0].atk = 1;
  unit_list[0].def = 1;
  unit_list[0].rng = 1;
  unit_list[0].mov = 1;
  unit_list[0].spd = 1;
  unit_list[0].points = 0;
  unit_list[0].art = NO_ART;
  unit_list[0].id = 0;
  unit_list[0].a_type = NONE;

  unit_list[SPEAR_UNIT].hp  = 45;
  unit_list[SPEAR_UNIT].atk = 23;
  unit_list[SPEAR_UNIT].def = 8;
  unit_list[SPEAR_UNIT].rng = 2;
  unit_list[SPEAR_UNIT].mov = 3;
  unit_list[SPEAR_UNIT].spd = 15;
  unit_list[SPEAR_UNIT].points = 3;
  unit_list[SPEAR_UNIT].art = ZAP_ART;
  unit_list[SPEAR_UNIT].id = SPEAR_UNIT;
  unit_list[SPEAR_UNIT].a_type = PIERCE;
  unit_list[SPEAR_UNIT].art = RAPID_THRUST_ART;
  unlocked_units[SPEAR_UNIT] = 1;

  unit_list[LANCER_UNIT].hp  = 55;
  unit_list[LANCER_UNIT].atk = 25;
  unit_list[LANCER_UNIT].def = 11;//180
  unit_list[LANCER_UNIT].rng = 2;
  unit_list[LANCER_UNIT].mov = 3;
  unit_list[LANCER_UNIT].spd = 14;
  unit_list[LANCER_UNIT].id = LANCER_UNIT;
  unit_list[LANCER_UNIT].a_type = PIERCE;
  unit_list[LANCER_UNIT].art = RUN_THROUGH_ART;
  unlocked_units[LANCER_UNIT] = 1;

  unit_list[SWORD_UNIT].hp  = 50; //0x2BCE
  unit_list[SWORD_UNIT].atk = 20;
  unit_list[SWORD_UNIT].def = 10;
  unit_list[SWORD_UNIT].mov = 3;
  unit_list[SWORD_UNIT].spd = 11;
  unit_list[SWORD_UNIT].rng = 1;
  unit_list[SWORD_UNIT].a_type = NORMAL;
  unit_list[SWORD_UNIT].art = CLEAVE_ART;
  unlocked_units[SWORD_UNIT] = 1;

  unit_list[ARCHER_UNIT].atk = 26;
  unit_list[ARCHER_UNIT].hp  = 35;
  unit_list[ARCHER_UNIT].def = 8;
  unit_list[ARCHER_UNIT].rng = 2;
  unit_list[ARCHER_UNIT].mov = 3;
  unit_list[ARCHER_UNIT].spd = 11;
  unit_list[ARCHER_UNIT].id = ARCHER_UNIT;
  unit_list[ARCHER_UNIT].a_type = MISSILE;
  unit_list[ARCHER_UNIT].art = RAIN_ARROW_ART;
  unlocked_units[ARCHER_UNIT] = 1;

  unit_list[SNIPER_UNIT].atk = 28;
  unit_list[SNIPER_UNIT].hp  = 35;
  unit_list[SNIPER_UNIT].def = 9;
  unit_list[SNIPER_UNIT].rng = 2;
  unit_list[SNIPER_UNIT].mov = 3;
  unit_list[SNIPER_UNIT].spd = 11;
  unit_list[SNIPER_UNIT].id = SNIPER_UNIT;
  unit_list[SNIPER_UNIT].a_type = MISSILE;
  unit_list[SNIPER_UNIT].art = FATAL_SHOT_ART;
  unlocked_units[SNIPER_UNIT] = 1;

  unit_list[STALKER_UNIT].atk = 26;
  unit_list[STALKER_UNIT].hp  = 35;
  unit_list[STALKER_UNIT].def = 8;
  unit_list[STALKER_UNIT].rng = 2;
  unit_list[STALKER_UNIT].mov = 3;
  unit_list[STALKER_UNIT].spd = 11;
  unit_list[STALKER_UNIT].id = STALKER_UNIT;
  unit_list[STALKER_UNIT].a_type = MISSILE;
  unit_list[STALKER_UNIT].art = TRACK_ART;
  unlocked_units[STALKER_UNIT] = 1;

	unit_list[HOUND_UNIT].atk = 28;
	unit_list[HOUND_UNIT].def = 12;
	unit_list[HOUND_UNIT].hp  = 60;
  unit_list[HOUND_UNIT].mov = 4;
  unit_list[HOUND_UNIT].spd = 20;
  unit_list[HOUND_UNIT].rng = 1;
	unit_list[HOUND_UNIT].id = HOUND_UNIT;
	unit_list[HOUND_UNIT].a_type = NONE;

	unit_list[BLOB_UNIT].atk = 20;
	unit_list[BLOB_UNIT].def = 14;
	unit_list[BLOB_UNIT].hp  = 50;
  unit_list[BLOB_UNIT].spd = 17;
  unit_list[BLOB_UNIT].mov = 3;
	unit_list[BLOB_UNIT].rng = 1;
	unit_list[BLOB_UNIT].id = BLOB_UNIT;
	unit_list[BLOB_UNIT].a_type = NONE;

	unit_list[AXE_UNIT].atk = 22;
	unit_list[AXE_UNIT].def = 10;
	unit_list[AXE_UNIT].hp  = 50;
  unit_list[AXE_UNIT].mov = 3;
	unit_list[AXE_UNIT].id = AXE_UNIT;
	unit_list[AXE_UNIT].a_type = AXE;
	unit_list[AXE_UNIT].art = POWER_WAVE_ART;
  unit_list[AXE_UNIT].points = 3;

  unit_list[BERSERKER_UNIT].atk = 32;
  unit_list[BERSERKER_UNIT].def = 13;
  unit_list[BERSERKER_UNIT].hp  = 60;
  unit_list[BERSERKER_UNIT].mov = 3;
  unit_list[BERSERKER_UNIT].id = BERSERKER_UNIT;
  unit_list[BERSERKER_UNIT].a_type = AXE;
  unit_list[BERSERKER_UNIT].art = FRENZY_ART;

  unit_list[MAGE_UNIT].atk = 20;
  unit_list[MAGE_UNIT].def = 7;
  unit_list[MAGE_UNIT].hp = 35;
  unit_list[MAGE_UNIT].spd = 12;
  unit_list[MAGE_UNIT].rng = 2;
  unit_list[MAGE_UNIT].mov = 3;
  unit_list[MAGE_UNIT].id = MAGE_UNIT;
  unit_list[MAGE_UNIT].a_type = MAGIC;
  unit_list[MAGE_UNIT].art = ZAP_ART;
  unit_list[MAGE_UNIT].points = 3;

  unit_list[WITCH_UNIT].atk = 18;
  unit_list[WITCH_UNIT].def = 8;
  unit_list[WITCH_UNIT].hp = 35;
  unit_list[WITCH_UNIT].spd = 12;
  unit_list[WITCH_UNIT].rng = 2;
  unit_list[WITCH_UNIT].mov = 3;
  unit_list[WITCH_UNIT].id = WITCH_UNIT;
  unit_list[WITCH_UNIT].a_type = MAGIC;
  unit_list[WITCH_UNIT].art = SEDUCE_ART;

  unit_list[PRIEST_UNIT].atk = 18;
  unit_list[PRIEST_UNIT].hp = 16;
  unit_list[PRIEST_UNIT].def = 8;
  unit_list[PRIEST_UNIT].hp = 35;
  unit_list[PRIEST_UNIT].spd = 12;
  unit_list[PRIEST_UNIT].rng = 2;
  unit_list[PRIEST_UNIT].mov = 3;
  unit_list[PRIEST_UNIT].id = PRIEST_UNIT;
  unit_list[PRIEST_UNIT].a_type = MAGIC;
  unit_list[PRIEST_UNIT].art = ZAP_ART;

  unit_list[BLACK_MAGE_UNIT].atk = 18;
  unit_list[BLACK_MAGE_UNIT].hp = 16;
  unit_list[BLACK_MAGE_UNIT].def = 8;
  unit_list[BLACK_MAGE_UNIT].hp = 35;
  unit_list[BLACK_MAGE_UNIT].spd = 12;
  unit_list[BLACK_MAGE_UNIT].rng = 2;
  unit_list[BLACK_MAGE_UNIT].mov = 3;
  unit_list[BLACK_MAGE_UNIT].id = BLACK_MAGE_UNIT;
  unit_list[BLACK_MAGE_UNIT].a_type = MAGIC;
  unit_list[BLACK_MAGE_UNIT].art = CAPTURE_ART;
  unlocked_units[BLACK_MAGE_UNIT] = 1;

  unit_list[CLERIC_UNIT].atk = 16;
  unit_list[CLERIC_UNIT].def = 7;
  unit_list[CLERIC_UNIT].hp = 29;
  unit_list[CLERIC_UNIT].spd = 16;
  unit_list[CLERIC_UNIT].mov = 3;
  unit_list[CLERIC_UNIT].id = CLERIC_UNIT;
  unit_list[CLERIC_UNIT].a_type = MAGIC;
  unit_list[CLERIC_UNIT].art = INNVIGORATE_ART;
  unit_list[CLERIC_UNIT].points = 3;

	unit_list[DEMON_UNIT].atk = 22;
	unit_list[DEMON_UNIT].def = 13;
	unit_list[DEMON_UNIT].rng = 1;
	unit_list[DEMON_UNIT].mov = 4;
	unit_list[DEMON_UNIT].id = DEMON_UNIT;
	unit_list[DEMON_UNIT].a_type = NORMAL;

  unit_list[MONK_UNIT].hp  = 60;
  unit_list[MONK_UNIT].atk = 23;
  unit_list[MONK_UNIT].def = 12;
  unit_list[MONK_UNIT].rng = 1;
  unit_list[MONK_UNIT].mov = 3;
  unit_list[MONK_UNIT].id = MONK_UNIT;
  unit_list[MONK_UNIT].a_type = UNARMED;
  unit_list[MONK_UNIT].art = CLEAR_EYES_ART;

  unit_list[FIGHTER_UNIT].hp  = 45;
  unit_list[FIGHTER_UNIT].atk = 24;
  unit_list[FIGHTER_UNIT].def = 8;
  unit_list[FIGHTER_UNIT].rng = 1;
  unit_list[FIGHTER_UNIT].mov = 3;
  unit_list[FIGHTER_UNIT].spd = 19;
  unit_list[FIGHTER_UNIT].id = FIGHTER_UNIT;
  unit_list[FIGHTER_UNIT].a_type = UNARMED;
  unit_list[FIGHTER_UNIT].art = RED_EYE_ART;

  unit_list[BRAWLER_UNIT].hp  = 60;
  unit_list[BRAWLER_UNIT].atk = 23;
  unit_list[BRAWLER_UNIT].def = 9;
  unit_list[BRAWLER_UNIT].rng = 1;
  unit_list[BRAWLER_UNIT].mov = 3;
  unit_list[BRAWLER_UNIT].id = BRAWLER_UNIT;
  unit_list[BRAWLER_UNIT].a_type = UNARMED;
  unit_list[BRAWLER_UNIT].art = BLACK_EYE_ART;
  
  unit_list[RAIDER_UNIT].hp  = 45;
  unit_list[RAIDER_UNIT].atk = 23;
  unit_list[RAIDER_UNIT].def = 9;
  unit_list[RAIDER_UNIT].rng = 1;
  unit_list[RAIDER_UNIT].mov = 3;
  unit_list[RAIDER_UNIT].spd = 14;
  unit_list[RAIDER_UNIT].id = RAIDER_UNIT;
  unit_list[RAIDER_UNIT].a_type = NORMAL;
  unit_list[RAIDER_UNIT].art = PILLAGE_ART;

  unit_list[KNIGHT_UNIT].hp  = 60;
  unit_list[KNIGHT_UNIT].atk = 23;
  unit_list[KNIGHT_UNIT].def = 13;
  unit_list[KNIGHT_UNIT].rng = 1;
  unit_list[KNIGHT_UNIT].mov = 3;
  unit_list[KNIGHT_UNIT].id = KNIGHT_UNIT;
  unit_list[KNIGHT_UNIT].a_type = NORMAL;
  unit_list[KNIGHT_UNIT].art = BLOW_BACK_ART;

  unit_list[PALADIN_UNIT].hp  = 60;
  unit_list[PALADIN_UNIT].atk = 23;
  unit_list[PALADIN_UNIT].def = 14;
  unit_list[PALADIN_UNIT].rng = 1;
  unit_list[PALADIN_UNIT].mov = 3;
  unit_list[PALADIN_UNIT].id = PALADIN_UNIT;
  unit_list[PALADIN_UNIT].a_type = NORMAL;
  unit_list[PALADIN_UNIT].art = JUDGEMENT_ART;

  unit_list[GOLEM_UNIT].hp  = 200;
  unit_list[GOLEM_UNIT].atk = 15;
  unit_list[GOLEM_UNIT].def = 19;
  unit_list[GOLEM_UNIT].rng = 1;
  unit_list[GOLEM_UNIT].mov = 3;
  unit_list[GOLEM_UNIT].id = GOLEM_UNIT;
  unit_list[GOLEM_UNIT].a_type = NONE;
  unit_list[GOLEM_UNIT].art = NO_ART;

  for(i=0; i<MAX_UNIT_COUNT; i++)
  {
    unit_list[i].pow = 0;
    unit_list[i].pow += unit_list[i].hp;
    unit_list[i].pow += unit_list[i].atk;
    unit_list[i].pow += unit_list[i].def;
    unit_list[i].pow /= 7;
    unit_list[i].pow += unit_list[i].rng * 2;
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
    case PIRATE_UNIT: upgrade_cost = 150; break;
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
    case PIRATE_UNIT: unit_cost = 200; break;
    default: unit_cost = 100;
  }
}