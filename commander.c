// #include "tactic.c"
#include "commander.h"

#define MAX_PARTY_COMMANDERS 6
#define MAX_ENEMY_COMMANDERS 15
#define MAX_ARMY_SIZE 9
#define MAX_METER 5
#define TOTAL_COMMANDERS (MAX_PARTY_COMMANDERS + MAX_ENEMY_COMMANDERS)
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
char tmp_party_size = 0;

struct Commander party_commanders[MAX_PARTY_COMMANDERS];
struct Commander enemy_commanders[MAX_ENEMY_COMMANDERS];

struct Commander temp_commanders[MAX_PARTY_COMMANDERS];

void copy_commanders(struct Commander *cmdr_to, struct Commander *cmdr_from)
{
  memcpy(cmdr_to,cmdr_from,(sizeof(struct Commander) * MAX_PARTY_COMMANDERS));
}

void clear_commander_battle_group(struct Commander *cmdr)
{
  char i;
  for(i=0; i<9; i++)
  {
    cmdr->bg.units[i].id = 0;//&unit_list[NO_UNIT];
    cmdr->bg.units[i].hp = 0;
    cmdr->bg.units[i].exp = 0;
    cmdr->bg.units[i].level = 0;
  }
}

char remove_unit_from_group(char cmdr_id, char position)
{
  char level;
  level = party_commanders[cmdr_id].bg.units[position].level;
  // put_number(level,4,s_x_relative,s_y_relative);
  // wait_for_I_input();
  party_commanders[cmdr_id].bg.units[position].id = 0;// = &unit_list[NO_UNIT];
  party_commanders[cmdr_id].bg.units[position].hp = 0;
  party_commanders[cmdr_id].bg.units[position].exp = 0;
  party_commanders[cmdr_id].bg.units[position].level = 0;
  return level;
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

void remove_party_commander_from_game(char cmdr_id)
{
  char i;
  for(i=cmdr_id; i<MAX_PARTY_COMMANDERS; i++)
  {
    // memcpy(&entities[i],&entities[i+1],sizeof(Entity));
    memcpy(&party_commanders[i],&party_commanders[i+1],sizeof(struct Commander));
  }
  party_size--;
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

char get_commander_bg_size(char cmdr_id)
{
  char i, count;
  count = 0;
  for(i=0; i<MAX_ARMY_SIZE; i++)
  {
    if(party_commanders[cmdr_id].bg.units[i].id)
    {
      count++;
    }
  }
  return count;
}