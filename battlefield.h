#define TILE_SIZE 16
#define BF_X_TILES 16
#define BF_Y_TILES 12
#define MAP_SIZE 192

#define SELECT_MODE 0
#define PLACE_MODE 1
#define MENU_MODE 2
#define SPLIT_MODE 3
#define ATTACK_MODE 4
#define MERGE_MODE 5

#define MAX_ENTITIES 36
#define PLAYER 0
#define CPU 1
#define INF 0
#define SPEAR 1
#define MUSKET 2
#define CALV 3

#define ACTIONS_PER_TURN 6
#define MOVE_COST 2
#define SPLIT_COST 2
#define ATTACK_COST 2
#define MERGE_COST 2

#define MENU_ATTACK 0
#define MENU_MOVE 1
#define MENU_SPLIT 2
#define MENU_MERGE 3
#define MENU_BACK 4

#define COMMANDER_FIGURE_BEGIN 0x7C00

struct Map{
  char id;
  int address;
};

typedef struct{
  Unit *unit;
  char unit_type, army_size, team, pal, is_cmdr, id;
} Entity;

char battle_grid[223];
Entity entities[MAX_ENTITIES];
struct Map cmdr_vram_mapper[12];
char untraversable_terrain[15];
char selector_mode, actions;
int sel_x, sel_y, turn, menu_x, menu_y, cursor_x, cursor_y, unit_selected;
char num_of_entities, menu_option;
char size_one;
char size_two;
char one_total;
char two_total;
int cmdr_fig_current;
char cmdr_fig_pal;
char num_of_bad_terrains;
char current_turn;

void add_entity(char type, char size, char team, char pal, Unit *unit, char cmdr, char id)
{
  entities[num_of_entities].unit_type = type;
  entities[num_of_entities].army_size = size;
  entities[num_of_entities].team = team;
  entities[num_of_entities].pal = pal;
  entities[num_of_entities].unit = unit;
  entities[num_of_entities].is_cmdr = cmdr;
  entities[num_of_entities].id = id;
  num_of_entities += 1;
}

void display_error_message(char *str)
{
  // hide_unit_info();
  // put_string("cannot split unit",10,1);
  put_string(str,10,1);
}

void display_unit_info(char entity_id)
{
  put_string("unit id:",1,1);
  put_string("army_size:",1,2);
  put_number(entity_id,2,10,1);
  put_number(entities[entity_id-1].army_size,2,13,2);
}

void hide_unit_info()
{
  put_string("                               ",1,1);
  put_string("                               ",1,2);
}

void update_selector_pos(int x, int y)
{
  s_x += x;
  s_y += y;
}

void draw_selector()
{
  spr_set(0);
  spr_x(s_x);
  spr_y(s_y);
}

void hide_menu()
{
  spr_set(2);
  spr_hide();
  spr_set(3);
  spr_hide();
  hide_selector();
}

void hide_cursor()
{
  spr_set(0);
  spr_hide();
}

void hide_selector()
{
  spr_set(1);
  spr_hide();
}

void display_selector()
{
  spr_set(0);
  spr_show();
}

char check_action_cost(char required_cost)
{
  return actions >= required_cost;
}

void draw_actions()
{
  char i;
  for(i=4; i<4+actions; i++)
  {
    spr_set(i);
    spr_show();
  }
}

void load_actions()
{
  char i, x, y;
  for(i=4; i<10; i++)
  {
    x = 200 + (((i-4) / 2) * 18);
    y = 11 + (((i-4) % 2) * 8);
    spr_make(i,x,y,0x6600+(((i-4)%2)*0x40),FLIP_MAS|SIZE_MAS,NO_FLIP|SZ_16x16,31,1);
    if((i-4) >= actions)
    {
      spr_hide();
    }
  }
}

void destroy_entity(int grid_pos)
{
  //NEXT THING TO DO
  int i;
  char entity_id;
  entity_id = battle_grid[grid_pos]-1;

  if(entities[entity_id].army_size == 0)
  {
    battle_grid[grid_pos] = 0;
    if(entities[entity_id].team == PLAYER)
    {
      if(entities[entity_id].is_cmdr)
      {
        size_one--;
      }
      one_total--;
    }
    else
    {
      if(entities[entity_id].is_cmdr)
      {
        size_two--;
      }
      two_total--;
    }
    for(i=0; i<MAP_SIZE; i++)
    {
      if(battle_grid[i] > entity_id)
      {
        battle_grid[i] -= 1;
      }
    }
    // i = 0;
    for(i=entity_id; i<num_of_entities-1; i++)
    {
      memcpy(&entities[i],&entities[i+1],sizeof(Entity));
    }

    num_of_entities--;
  }
}

void remove_unit_from_grid(int grid_pos)
{
  char entity_id, i;
  entity_id = battle_grid[grid_pos]-1;

  battle_grid[grid_pos] = 0;
  for(i=0; i<MAP_SIZE; i++)
  {
    if(battle_grid[i] > entity_id)
    {
      battle_grid[i] -= 1;
    }
  }

  for(i=entity_id; i<num_of_entities-1; i++)
  {
    memcpy(&entities[i],&entities[i+1],sizeof(Entity));
  }
  reset_satb();
  display_sprites_();
  hide_menu();

  num_of_entities--;
}

char is_traversable(int pos)
{
  char i;
  for(i=0; i<num_of_bad_terrains; i++)
  {
    if(battlefieldbat[pos] == untraversable_terrain[i])
    {
      return 0;
    }
  }
  return 1;
}
