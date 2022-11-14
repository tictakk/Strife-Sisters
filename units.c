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

#define MAX_UNIT_ENTITIES 180

enum Unit_Type{
	INFANTRY, FLYERS, SPEARS, MUSKETS, MAGES, HOUNDS, COMMANDER, BLOBS, AXES
};

typedef struct{
	unsigned char atk, def, mov, rng, ign, spd, exp, id, a_type, d_type;
	int hp;
	enum Unit_Type unit_type;
} Unit;

typedef struct{
	char unit_type, cmdr_id;
	int hp;
} Unit_Entity;

Unit_Entity unit_entities[MAX_UNIT_ENTITIES];
Unit unit_list[16];//12 units, 4 ncpcs, 25 commanders
unsigned char unit_entity_count = 0;

void initialize_units()
{
  char i;
  // for(i=0; i<16+TOTAL_COMMANDERS; i++)
	for(i=0; i<16; i++)
  {
    unit_list[i].hp = 50;
    unit_list[i].atk = 10;
    unit_list[i].def = 10;
    unit_list[i].rng = 1;
    unit_list[i].mov = 3;
    unit_list[i].ign = 0;
    unit_list[i].spd = 10;
    unit_list[i].exp = 25;
    unit_list[i].unit_type = COMMANDER;
    unit_list[i].id = i;
    unit_list[i].a_type = UNARMED;
    unit_list[i].d_type = MEDIUM;
  }
  unit_list[SPEAR_UNIT].hp  = 16;
  unit_list[SPEAR_UNIT].atk = 14;
  unit_list[SPEAR_UNIT].def = 7;
  unit_list[SPEAR_UNIT].rng = 2;
  unit_list[SPEAR_UNIT].mov = 3;
  unit_list[SPEAR_UNIT].ign = 0;
  unit_list[SPEAR_UNIT].spd = 10;
  unit_list[SPEAR_UNIT].exp = 25;
  unit_list[SPEAR_UNIT].unit_type = SPEARS;
  unit_list[SPEAR_UNIT].id = SPEAR_UNIT;
  unit_list[SPEAR_UNIT].a_type = PIERCE;
  unit_list[SPEAR_UNIT].d_type = HEAVY;

  unit_list[SWORD_UNIT].hp  = 1;
  unit_list[SWORD_UNIT].atk = 12;
  unit_list[SWORD_UNIT].def = 8;
  unit_list[SWORD_UNIT].mov = 3;
  unit_list[SWORD_UNIT].rng = 1;
  unit_list[SWORD_UNIT].ign = 0;
  unit_list[SWORD_UNIT].spd = 12;
  unit_list[SWORD_UNIT].exp = 25;
  unit_list[SWORD_UNIT].unit_type = INFANTRY;
  unit_list[SWORD_UNIT].id = SWORD_UNIT;
  unit_list[SWORD_UNIT].a_type = NORMAL;
  unit_list[SWORD_UNIT].d_type = HEAVY;

  unit_list[ARCHER_UNIT].hp  = 13;
  unit_list[ARCHER_UNIT].atk = 15;
  unit_list[ARCHER_UNIT].def = 5;
  unit_list[ARCHER_UNIT].rng = 2;
  unit_list[ARCHER_UNIT].mov = 3;
  unit_list[ARCHER_UNIT].ign = 0;
  unit_list[ARCHER_UNIT].spd = 8;
  unit_list[ARCHER_UNIT].exp = 25;
  unit_list[ARCHER_UNIT].unit_type = MUSKETS;
  unit_list[ARCHER_UNIT].id = ARCHER_UNIT;
  unit_list[ARCHER_UNIT].a_type = MISSILE;
  unit_list[ARCHER_UNIT].d_type = LIGHT;

	unit_list[HOUND_UNIT].atk = 12;
	unit_list[HOUND_UNIT].def = 5;
	unit_list[HOUND_UNIT].hp  = 13;
	unit_list[HOUND_UNIT].ign = 0;
	unit_list[HOUND_UNIT].mov = 4;
	unit_list[HOUND_UNIT].spd = 11;
	unit_list[HOUND_UNIT].exp = 25;
	unit_list[HOUND_UNIT].unit_type = HOUNDS;
	unit_list[HOUND_UNIT].id = HOUND_UNIT;
	unit_list[HOUND_UNIT].a_type = NORMAL;
	unit_list[HOUND_UNIT].d_type = MEDIUM;

	unit_list[BLOB_UNIT].atk = 12;
	unit_list[BLOB_UNIT].def = 6;
	unit_list[BLOB_UNIT].hp  = 11;
	unit_list[BLOB_UNIT].ign = 0;
	unit_list[BLOB_UNIT].mov = 3;
	unit_list[BLOB_UNIT].spd = 16;
	unit_list[BLOB_UNIT].exp = 25;
	unit_list[BLOB_UNIT].unit_type = BLOBS;
	unit_list[BLOB_UNIT].id = BLOB_UNIT;
	unit_list[BLOB_UNIT].a_type = NORMAL;
	unit_list[BLOB_UNIT].d_type = LIGHT;

	unit_list[AXE_UNIT].atk = 12;
	unit_list[AXE_UNIT].def = 6;
	unit_list[AXE_UNIT].hp  = 20;
	unit_list[AXE_UNIT].ign = 0;
	unit_list[AXE_UNIT].mov = 3;
	unit_list[AXE_UNIT].spd = 13;
	unit_list[AXE_UNIT].exp = 25;
	unit_list[AXE_UNIT].unit_type = AXES;
	unit_list[AXE_UNIT].id = AXE_UNIT;
	unit_list[AXE_UNIT].a_type = BLUNT;
	unit_list[AXE_UNIT].d_type = MEDIUM;

  unit_list[MAGE_UNIT].atk = 17;
  unit_list[MAGE_UNIT].def = 10;
  unit_list[MAGE_UNIT].hp  = 20;
  unit_list[MAGE_UNIT].ign = 0;
  unit_list[MAGE_UNIT].spd = 5;
  unit_list[MAGE_UNIT].mov = 3;
  unit_list[MAGE_UNIT].exp = 25;
  unit_list[MAGE_UNIT].unit_type = MAGES;
  unit_list[MAGE_UNIT].id = MAGE_UNIT;
  unit_list[MAGE_UNIT].a_type = MAGIC;
  unit_list[MAGE_UNIT].d_type = LIGHT;

	unit_list[DEMON_UNIT].hp  = 70;
	unit_list[DEMON_UNIT].atk = 16;
	unit_list[DEMON_UNIT].def = 12;
	unit_list[DEMON_UNIT].rng = 1;
	unit_list[DEMON_UNIT].mov = 4;
	unit_list[DEMON_UNIT].ign = 1;
	unit_list[DEMON_UNIT].spd = 15;
	unit_list[DEMON_UNIT].exp = 25;
	unit_list[DEMON_UNIT].unit_type = FLYERS;
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

unsigned char add_unit_entity(char unit_type, char id)
{
	unit_entities[unit_entity_count].unit_type = unit_type;
	unit_entities[unit_entity_count].hp = unit_list[unit_type].hp;
	unit_entities[unit_entity_count].cmdr_id = id;
	return unit_entity_count++;
}

void delete_unit_entity(unsigned char unit_id)
{
	int i;
	for(i=unit_id; i<unit_entity_count+1; i++)
	{
	  memcpy(&unit_entities[i],&unit_entities[i+1],sizeof(Unit_Entity));
	}
	unit_entity_count--;
}
