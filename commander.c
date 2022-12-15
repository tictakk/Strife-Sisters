#define MAX_PARTY_COMMANDERS 6
#define MAX_ENEMY_COMMANDERS 15
#define TOTAL_COMMANDERS (MAX_PARTY_COMMANDERS + MAX_ENEMY_COMMANDERS)

//names
const char *name0 = "Rei";
const char *name1 = "Violet";
const char *name20 = "Generic";

typedef struct {
	Unit_Entity units[9];
} Battlegroup;

struct Commander{
	char item;
	char *name;
	char sprite_type;
	Battlegroup bg;
};

char party_size = 0;
struct Commander party_commanders[MAX_PARTY_COMMANDERS];
struct Commander enemy_commanders[MAX_ENEMY_COMMANDERS];

void add_commander_to_party(char *name, char st)
{
  struct Commander *cmdr;
  cmdr = party_commanders + party_size;
  cmdr->name = name;
  cmdr->sprite_type = st;
  clear_commander_battle_group(cmdr);
  party_size++;
}

void clear_commander_battle_group(struct Commander *cmdr)
{
  char i;
  for(i=0; i<9; i++)
  {
    cmdr->bg.units[i].unit = 0;
    cmdr->bg.units[i].hp = 0;
    cmdr->bg.units[i].exp = 0;
  }
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
