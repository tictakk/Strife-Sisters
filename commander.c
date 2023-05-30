#define MAX_PARTY_COMMANDERS 8
#define MAX_ENEMY_COMMANDERS 15
#define MAX_ARMY_SIZE 9
#define MAX_METER 5
#define TOTAL_COMMANDERS (MAX_PARTY_COMMANDERS + MAX_ENEMY_COMMANDERS)

//names
const char *name0 = "Rei";
const char *name1 = "Violet";
const char *name2 = "King";
const char *name3 = "Yuri";
const char *name20 = "Generic";

typedef struct {
	Unit_Entity units[9];
  char calling_stone;
  char meter;
} Battlegroup;

struct Commander{
  // char meter;
	char *name;
	char sprite_type;
  char max_bp;
  // char medals[4];
	Battlegroup bg;
};

char party_size = 0;
struct Commander party_commanders[MAX_PARTY_COMMANDERS];
struct Commander enemy_commanders[MAX_ENEMY_COMMANDERS];

void clear_commander_battle_group(struct Commander *cmdr)
{
  char i;
  for(i=0; i<9; i++)
  {
    cmdr->bg.units[i].unit = &unit_list[NO_UNIT];
    cmdr->bg.units[i].hp = 0;
  }
}

void remove_unit_from_group(char cmdr_id, char position)
{
  party_commanders[cmdr_id].bg.units[position].unit = &unit_list[NO_UNIT];
  party_commanders[cmdr_id].bg.units[position].hp = 0;
}

void add_commander_to_party(char *name, char st)
{
  struct Commander *cmdr;
  cmdr = party_commanders + party_size;
  cmdr->name = name;
  cmdr->sprite_type = st;
  clear_commander_battle_group(cmdr);
  party_size++;
}

void clear_enemy_commander_ram()
{
  char i;
  for(i=0; i<MAX_ENEMY_COMMANDERS; i++)
  {
    enemy_commanders[i].sprite_type = 0;
    enemy_commanders[i].name = name20;
    clear_commander_battle_group(enemy_commanders+i);
  }
}

void list_commanders(char x, char y)
{
  char i;

  for(i=0; i<party_size; i++)
  {
    put_string(party_commanders[i].name,x,y+i);
  }
  for(i; i<8; i++)
  {
    put_string("---",x,y+i);
  }
}

void heal_commander_army(char cmdr_id)
{
  int x,y;
  char i;

  create_effect(EFFECT_HEAL,x,y,0);
  for(i=0; i<9; i++)
  {
    if(party_commanders[cmdr_id].bg.units[i].unit->id)
    {
      party_commanders[cmdr_id].bg.units[i].hp = party_commanders[cmdr_id].bg.units[i].unit->hp;
    }
  }
}

char get_commander_battle_points(char cmdr_id)
{
  char bp, i;
  bp = 0;

  for(i=0; i<9; i++)
  {
    if(party_commanders[cmdr_id].bg.units[i].unit->id)
    {
      bp += party_commanders[cmdr_id].bg.units[i].unit->points;
    }
  }
  return bp;
}

