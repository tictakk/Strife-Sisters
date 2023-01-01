#define MAX_PARTY_COMMANDERS 6
#define MAX_ENEMY_COMMANDERS 15
#define TOTAL_COMMANDERS (MAX_PARTY_COMMANDERS + MAX_ENEMY_COMMANDERS)

//names
const char *name0 = "Rei";
const char *name1 = "Violet";
const char *name20 = "Generic";

typedef struct {
	Unit_Entity units[9];
  char arts[3];
} Battlegroup;

struct Commander{
	char item;
  char max_meter;
  char meter;
  char max_army_pts;
	char *name;
	char sprite_type;
  char level;
  int exp;
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
    enemy_commanders[i].item = 0;
    enemy_commanders[i].sprite_type = 0;
    enemy_commanders[i].name = name20;
    clear_commander_battle_group(enemy_commanders+i);
  }
}

//TODO: I belive this can be removed, we are no longer "upgrading" units through exp
void upgrade_unit(struct Commander *cmdr, char pos, char upgrade_id)
{
  int hp, hp_p, new_hp;

  hp = cmdr->bg.units[pos].hp * 100;
  hp_p = hp / cmdr->bg.units[pos].unit->hp;

  new_hp = (unit_list[upgrade_id].hp * hp_p) / 100;

  cmdr->bg.units[pos].unit = &unit_list[upgrade_id];
  cmdr->bg.units[pos].hp = new_hp;
}

char level_up_commander(char cmdr_id)
{
  while(party_commanders[cmdr_id].exp > next_level(party_commanders[cmdr_id].level))
  {
    party_commanders[cmdr_id].level++;
    party_commanders[cmdr_id].max_army_pts++;
    party_commanders[cmdr_id].max_meter += 5;
  }
  return party_commanders[cmdr_id].level;
}

void give_commander_exp(char cmdr_id, char unit_id)
{
  party_commanders[cmdr_id].exp += (unit_list[unit_id].points << 3);
}

int next_level(char level)
{
  int lvl;
  lvl = (int)level;

  return ((lvl * lvl * lvl) * 5);
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

