// #include "tactic.c"
#include "commander.h"

#define MAX_PARTY_COMMANDERS 6
#define MAX_ENEMY_COMMANDERS 15
#define MAX_ARMY_SIZE 9
#define MAX_METER 5
#define TOTAL_COMMANDERS (MAX_PARTY_COMMANDERS + MAX_ENEMY_COMMANDERS)
#define MAX_LEVEL 20
#define MAX_COMMANDER_STAT 100

typedef struct {
	Unit_Entity units[9];
  char formation;
  char bonuses;
} Battlegroup;

struct Commander{
	char *name;
	char sprite_type;
  char max_bp;
  char tactic_id;
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
    cmdr->bg.units[i].id = 0;//&unit_list[NO_UNIT];
    cmdr->bg.units[i].hp = 0;
    cmdr->bg.units[i].exp = 0;
    cmdr->bg.units[i].level = 0;
    cmdr->bg.units[i].sta = 0;
  }
}

void remove_unit_from_group(char cmdr_id, char position)
{
  party_commanders[cmdr_id].bg.units[position].id = 0;// = &unit_list[NO_UNIT];
  party_commanders[cmdr_id].bg.units[position].hp = 0;
  party_commanders[cmdr_id].bg.units[position].exp = 0;
  party_commanders[cmdr_id].bg.units[position].level = 0;
  party_commanders[cmdr_id].bg.units[position].sta = 0;
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

void list_commanders(char x, char y)
{
  char i;

  for(i=0; i<party_size; i++)
  {
    put_string(party_commanders[i].name,x,y+i);
  }
  for(i; i<6; i++)
  {
    put_string("$$$",x,y+i);
  } 
}

char get_commander_battle_points(char cmdr_id)
{
  char bp, i;
  bp = 0;

  for(i=0; i<9; i++)
  {
    if(party_commanders[cmdr_id].bg.units[i].id)
    {
      load_unit_header(party_commanders[cmdr_id].bg.units[i].id,0);
      bp += unit_header[0].points;//party_commanders[cmdr_id].bg.units[i].unit->points;
    }
  }
  return bp;
}

void check_add_new_commander(char map_no)
{
  if(map_no == 1 && party_size < 2)
  {
    add_commander_to_party(name1, VIOLET);
    load_unit_to_cmdr(1,4,VIOLET,1,1);
  }
  if(map_no == 3 && party_size < 3)
  {
    add_commander_to_party(name2,KING);
    // set_commander_stats(2,1,8,8,10);
    // party_commanders[2].exp = 40;
    // level_up_commander(2);
    load_unit_to_cmdr(2,3,KING,1,3);
    // load_unit_to_cmdr(2,3,LANCER_UNIT,0);
    load_unit_to_cmdr(2,1,SWORD_UNIT,0,3);
    load_unit_to_cmdr(2,5,SPEAR_UNIT,0,3);
    load_unit_to_cmdr(2,7,CLERIC_UNIT,0,3);
    return;
  }

  if(map_no == 5)
  {
    //add tinker
    //return;
  }

  // if(map_no == )
}