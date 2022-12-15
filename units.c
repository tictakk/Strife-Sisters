/* sprites */
#incspr(attack, "map/sprites/swordy.pcx")
#incpal(soldierpal, "map/sprites/swordy.pcx",0,2)
#incspr(blobbattle, "map/sprites/blobbattle.pcx")
#incpal(blobbattlepal, "map/sprites/blobbattle.pcx",0,2)
#incspr(attack2, "map/sprites/spear_anim.pcx")
#incpal(spearpal, "map/sprites/spear_anim.pcx",0,2)
#incspr(bandit, "characters/axeunit.pcx")
#incpal(banditpal, "characters/axeunit.pcx",0,2)
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
#define NORMAL 0b00010001
#define PIERCE 0b00100010
#define MISSILE 0b00001100
#define MAGIC 0b00001010
#define UNARMED 0b00100001
#define BLUNT 0b00010100

//defense types
#define HEAVY 0b00100100
#define MEDIUM 0b00010010
#define LIGHT 0b00001001
#define NO_ARMOR 0b00000000

//advantage types
#define NO_ADV 0
#define ADVANTAGE 1
#define DISADVANTAGE 2

typedef struct{
	unsigned char atk, def, mov, id, a_type, d_type,
                rng, ign, spd, exp, points, bonus_col;
	int hp;
} Unit;

typedef struct{
  Unit *unit;
  char exp;
  int hp;
} Unit_Entity;

Unit unit_list[MAX_UNIT_COUNT];
unsigned char unit_entity_count = 0;

void initialize_units()
{
  char i;

	for(i=0; i<MAX_UNIT_COUNT; i++)
  {
    unit_list[i].hp = 25;
    unit_list[i].atk = 20;
    unit_list[i].def = 10;
    unit_list[i].spd = 10;
    unit_list[i].bonus_col = 1;
    unit_list[i].rng = 1;
    unit_list[i].mov = 3;
    unit_list[i].ign = 0;
    unit_list[i].id = i;
    unit_list[i].a_type = UNARMED;
    unit_list[i].d_type = MEDIUM;
  }

  unit_list[SPEAR_UNIT].hp  = 27;
  unit_list[SPEAR_UNIT].atk = 23;
  unit_list[SPEAR_UNIT].def = 12;
  unit_list[SPEAR_UNIT].rng = 2;
  unit_list[SPEAR_UNIT].mov = 2;
  unit_list[SPEAR_UNIT].ign = 0;
  unit_list[SPEAR_UNIT].id = SPEAR_UNIT;
  unit_list[SPEAR_UNIT].a_type = PIERCE;
  unit_list[SPEAR_UNIT].d_type = HEAVY;

  unit_list[SWORD_UNIT].hp  = 36; //0x2BCE
  unit_list[SWORD_UNIT].atk = 20;
  unit_list[SWORD_UNIT].def = 15;
  unit_list[SWORD_UNIT].mov = 3;
  unit_list[SWORD_UNIT].rng = 1;
  unit_list[SWORD_UNIT].ign = 0;
  unit_list[SWORD_UNIT].id = SWORD_UNIT;
  unit_list[SWORD_UNIT].a_type = NORMAL;
  unit_list[SWORD_UNIT].d_type = HEAVY;

  unit_list[ARCHER_UNIT].atk = 28;
  unit_list[ARCHER_UNIT].hp  = 24;
  unit_list[ARCHER_UNIT].def = 12;
  unit_list[ARCHER_UNIT].rng = 2;
  unit_list[ARCHER_UNIT].mov = 3;
  unit_list[ARCHER_UNIT].ign = 0;
  unit_list[ARCHER_UNIT].id = ARCHER_UNIT;
  unit_list[ARCHER_UNIT].a_type = MISSILE;
  unit_list[ARCHER_UNIT].d_type = LIGHT;

	unit_list[HOUND_UNIT].atk = 23;
	unit_list[HOUND_UNIT].def = 13;
	unit_list[HOUND_UNIT].hp  = 23;
	unit_list[HOUND_UNIT].ign = 0;
	unit_list[HOUND_UNIT].mov = 4;
	unit_list[HOUND_UNIT].id = HOUND_UNIT;
	unit_list[HOUND_UNIT].a_type = NORMAL;
	unit_list[HOUND_UNIT].d_type = MEDIUM;

	unit_list[BLOB_UNIT].atk = 22; //0x2C1B
	unit_list[BLOB_UNIT].def = 14;
	unit_list[BLOB_UNIT].hp  = 20;
	unit_list[BLOB_UNIT].ign = 0;
	unit_list[BLOB_UNIT].mov = 3;
	unit_list[BLOB_UNIT].rng = 1;
	unit_list[BLOB_UNIT].id = BLOB_UNIT;
	unit_list[BLOB_UNIT].a_type = NORMAL;
	unit_list[BLOB_UNIT].d_type = LIGHT;

	unit_list[AXE_UNIT].atk = 22;
	unit_list[AXE_UNIT].def = 13;
	unit_list[AXE_UNIT].hp  = 30;
	unit_list[AXE_UNIT].ign = 0;
	unit_list[AXE_UNIT].mov = 3;
	unit_list[AXE_UNIT].id = AXE_UNIT;
	unit_list[AXE_UNIT].a_type = BLUNT;
	unit_list[AXE_UNIT].d_type = MEDIUM;

  unit_list[MAGE_UNIT].atk = 23;
  unit_list[MAGE_UNIT].hp = 16;
  unit_list[MAGE_UNIT].def = 11;
  unit_list[MAGE_UNIT].hp  = 24;
  unit_list[MAGE_UNIT].spd  = 23;
  unit_list[MAGE_UNIT].ign = 0;
  unit_list[MAGE_UNIT].mov = 3;
  unit_list[MAGE_UNIT].id = MAGE_UNIT;
  unit_list[MAGE_UNIT].a_type = MAGIC;

	unit_list[DEMON_UNIT].hp  = 22;
	unit_list[DEMON_UNIT].atk = 22;
	unit_list[DEMON_UNIT].def = 13;
	unit_list[DEMON_UNIT].rng = 1;
	unit_list[DEMON_UNIT].mov = 4;
	unit_list[DEMON_UNIT].ign = 1;
	unit_list[DEMON_UNIT].id = DEMON_UNIT;
	unit_list[DEMON_UNIT].a_type = NORMAL;
}
//0 = no advantage, 1 = advantage, 2 = disavantage
char check_advantage(char a_type, char d_type)
{
  if(a_type & d_type)//either atk or def has advantage
  {
    //shift right 3 to ignore attack advantage
    if((a_type>>3) & (d_type>>3))
    {
      return DISADVANTAGE;
    }
    else
    {
      return ADVANTAGE;
    }
  }
  else
  {
    return NO_ADV;
  }
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
  hp = ue->unit->hp * 100;
  print_unit_attack_icon(ue->unit->id,x+1,y);
  print_unit_type(ue->unit->id,x,y+1);
  put_char('%',x,y+2);
  put_number(hp/ue->hp,2,x+1,y+2);
}