#define TILE_SIZE 16
#define BF_X_TILES 16
#define BF_Y_TILES 12
#define MAP_SIZE 224

#define SELECT_MODE 0
#define PLACE_MODE 1
#define MENU_MODE 2
#define SPLIT_MODE 3
#define ATTACK_MODE 4
#define MERGE_MODE 5
#define OBSERVE_MODE 6 //has to do with seeing AI's move space
#define DIALOG_MODE 7
#define EXPLORE_MODE 8
#define EXPLORE_MENU_MODE 9

#define MAX_ENTITIES 18
#define PLAYER 1
#define CPU 2
#define INF 0
#define SPEAR 1
#define MUSKET 2
#define FLYER 3
#define HOUND 4

#define ACTIONS_PER_TURN 6
#define MOVE_COST 2
#define SPLIT_COST 2
#define ATTACK_COST 2
#define MERGE_COST 2

#define MENU_ATTACK 0
#define MENU_MOVE 1
#define MENU_SPLIT 2
#define MENU_MERGE 99
#define MENU_BACK 4

#define ARCHER_ATTACK 8
#define SWORD_ATTACK 4
#define SPEAR_ATTACK 4

#define COMMANDER_FIGURE_BEGIN 0x6000

typedef struct{
  Unit *unit;
  int pos, stamina;
  char army_size, team, id, actionable, nearby_engaged;
} Entity;

char battle_grid[MAP_SIZE];
Entity entities[MAX_ENTITIES];
int cmdr_vram_mapper[24];
// struct Map cmdr_vram_mapper[12];
// char untraversable_terrain[15];
char selector_mode, actions;
int sel_x, sel_y, turn, menu_x, menu_y, cursor_x, cursor_y, unit_selected;
char num_of_entities, menu_option;
char size_one;
char size_two;
char one_total;
char two_total;
char cost;
int cmdr_fig_current;
char cmdr_fig_pal;
char current_turn;
int ai_timer;
int path[20];
char target;
char mapper_size;
char g;

char army_one[8]; //copy of army one
char army_two[10]; //copy of army two

// const char spear_attack[4] = { 2, 0, 2, 0};

const int archer_coord1[2] = {1,-16};
const int archer_coord2[2] = {0,-32};
const int archer_coord3[2] = {1,16};
const int archer_coord4[2] = {0,32};
const int archer_coord5[2] = {-1, -16};
const int archer_coord6[2] = {-1, 16};
const int archer_coord7[2] = {-2, 0};
const int archer_coord8[2] = {2, 0};

const int spear_coord1[2] = {0, -16};
const int spear_coord2[2] = {0, -32};
const int spear_coord3[2] = {0, 16};
const int spear_coord4[2] = {0, 32};
// const int spear_coord5[2] = {1, 0};
// const int spear_coord6[2] = {-1, 0};

const int sword_coord1[2] = {0, 16};
const int sword_coord2[2] = {1, 0};
const int sword_coord3[2] = {-1, 0};
const int sword_coord4[2] = {0, -16};

// struct Coords coords[9];
int coords[18];

void load_coords(char id)
{
  switch((*entities[id].unit).unit_type)
  {
    default:
    case FLYERS:
    case INFANTRY:
    coords[0] = sword_coord1[0];
    coords[1] = sword_coord1[1];

    coords[2] = sword_coord2[0];
    coords[3] = sword_coord2[1];

    coords[4] = sword_coord3[0];
    coords[5] = sword_coord3[1];

    coords[6] = sword_coord4[0];
    coords[7] = sword_coord4[1];
    break;

    case SPEARS:
    coords[0] = spear_coord1[0];
    coords[1] = spear_coord1[1];

    coords[2] = spear_coord2[0];
    coords[3] = spear_coord2[1];

    coords[4] = spear_coord3[0];
    coords[5] = spear_coord3[1];

    coords[6] = spear_coord4[0];
    coords[7] = spear_coord4[1];

    // coords[4].x = spear_coord5[0];
    // coords[4].y = spear_coord5[1];
    //
    // coords[5].x = spear_coord6[0];
    // coords[5].y = spear_coord6[1];
    break;

    case MUSKETS:
    coords[0] = archer_coord1[0];
    coords[1] = archer_coord1[1];

    coords[2] = archer_coord2[0];
    coords[3] = archer_coord2[1];

    coords[4] = archer_coord3[0];
    coords[5] = archer_coord3[1];

    coords[6] = archer_coord4[0];
    coords[7] = archer_coord4[1];

    coords[8] = archer_coord5[0];
    coords[9] = archer_coord5[1];

    coords[10] = archer_coord6[0];
    coords[11] = archer_coord6[1];

    coords[12] = archer_coord7[0];
    coords[13] = archer_coord7[1];

    coords[14] = archer_coord8[0];
    coords[15] = archer_coord8[1];
    break;
  }
}

char get_pattern_length(char id)
{
  switch((*entities[id].unit).unit_type)
  {
    case FLYERS:
    case INFANTRY: return SWORD_ATTACK;
    case SPEARS: return SPEAR_ATTACK;
    case MUSKETS: return ARCHER_ATTACK;
    default: return SWORD_ATTACK;
  }
}

void add_entity(char type, char size, char team, char pal, char unit_id, char cmdr, char id, int pos)
{
  entities[num_of_entities].army_size = size;
  entities[num_of_entities].team = team;
  // entities[num_of_entities].pal = pal;
  entities[num_of_entities].unit = &unit_list[unit_id];
  entities[num_of_entities].id = id;
  entities[num_of_entities].pos = pos;
  entities[num_of_entities].actionable = 1;
  entities[num_of_entities].stamina = 3;
  entities[num_of_entities].nearby_engaged = 1; //should start at 0
  num_of_entities += 1;
}

void display_error_message(char *str)
{
  // hide_unit_info();
  // put_string("cannot split unit",10,1);
  put_string(str,10,1);
  clear_error_message();
}

void display_unit_info(char entity_id)
{
  put_string("unit id:",1,1);
  put_string("army_size:",1,2);
  put_number(entity_id,2,10,1);
  put_number(entities[entity_id-1].army_size,2,13,2);
}

void update_selector_pos(int x, int y)
{
  sx += x;
  sy = max((sy + y),32);
}

void draw_selector()
{
  spr_set(0);
  spr_x(sx);
  spr_y(sy);
}

void hide_menu()
{
  // spr_set(2);
  // spr_hide();
  // spr_set(3);
  // spr_hide();
  unhighlight();
  hide_selector();
}

void unhighlight()
{
  load_map(0,2,0,0,16,14);
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
  // spr_set(0);
  // load_vram(0x5000,selector,0x40);
  // load_palette(16,selectorpal,1);
  spr_make(0,selector_x,selector_y,0x68C0,FLIP_MAS|SIZE_MAS,SZ_16x16,3,1);
  // spr_make()
  spr_show(0);
}

char check_action_cost(char required_cost)
{
  return actions >= required_cost;
}

void draw_actions()
{
  // char i;
  // for(i=4; i<4+actions; i++)
  // {
  //   spr_set(i);
  //   spr_show();
  // }
}

void load_actions()
{
  // char i, x, y;
  // for(i=4; i<10; i++)
  // {
  //   x = 200 + (((i-4) / 2) * 18);
  //   y = 11 + (((i-4) % 2) * 8);
  //   spr_make(i,x,y,0x6600+(((i-4)%2)*0x40),FLIP_MAS|SIZE_MAS,NO_FLIP|SZ_16x16,31,1);
  //   if((i-4) >= actions)
  //   {
  //     spr_hide();
  //   }
  // }
}

char destroy_entity(int id)
{
  //NEXT THING TO DO
  int i, id_location;
  char entity_id, killed;
  killed = 0;
  entity_id = id;
  // entity_id = battle_grid[grid_pos]-1;

  if(entities[entity_id].army_size == 0)
  {
    // battle_grid[grid_pos] = 0;
    battle_grid[entities[id].pos] = 0;
    killed = 1;
    if(entities[entity_id].team == PLAYER) //shoul we reduce size...?
    {
      one_total--;
    }
    else
    {
      two_total--;
    }
    for(i=0; i<MAP_SIZE; i++)
    {
      if(battle_grid[i] > entity_id+1)
      {
        battle_grid[i] -= 1;
      }
    }

    for(i=entity_id; i<num_of_entities; i++)
    {
      memcpy(&entities[i],&entities[i+1],sizeof(Entity)); //cpy dst from src
    }
    num_of_entities--;

  }
  return killed;
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

void deduct_actions(char num_of_actions)
{
  char i, action_offset, actions_left;
  actions_left = 6 - actions;
  action_offset = 10 - actions_left;
  actions -= num_of_actions;

  for(i=num_of_actions; i>0; i--)
  {
    spr_set(action_offset-i);
    spr_hide();
  }
}

void move_unit(int to, int from)
{
  char id;
  if(entities[battle_grid[from]-1].stamina >= 0)
  {
    //highlight(from,0,entities[battle_grid[from]-1].unit->mov);//actions/MOVE_COST
    hide_menu();
    id = battle_grid[from] - 1;
    // if(entities[id].stamina < 0){ return;}

    battle_grid[to] = battle_grid[from];
    battle_grid[from] = 0;
    entities[id].pos = to;
    deduct_stamina(id);

    selector_mode = SELECT_MODE;
    // put_number(battle_grid[to],3,5,5);
    update_map();
  }
  else
  {
    display_error_message("not enough stamina");
  }

}

void deduct_stamina(char id)
{
  // if(entities[id].stamina > -3) //if stamina is any number over 0
  // {
  //   entities[id].stamina--;
  // }
  // if(entities[id].stamina > 0)
  // {
  //   set_color_rgb((entities[id].pal*16)+15,0,7,0);
  // }
  // else
  // {
  //   set_color_rgb((entities[id].pal*16)+15,7,0,0);
  // }
}

void select_unit(char unit)
{
  selector_mode = PLACE_MODE;
  menu_option = MENU_ATTACK;
  // hide_menu();
  // highlight_near_squares(unit,0x1000,2,0,2,0);
  highlight(unit,0xB000,(*entities[battle_grid[unit]-1].unit).mov);
  display_selector();
}

int calc_move_cost(int origin, int dest)
{
  int o_x, o_y, d_x, d_y, x, y;
  o_x = origin % 16;
  o_y = origin / 16;
  d_x = dest % 16;
  d_y = dest / 16;
  return (char)(abs(o_x - d_x) + abs(o_y - d_y)) * 2;
}

char attack_unit(int src, int dst)
{
  char attacker, target;//, result;
  attacker = battle_grid[dst]-1;
  target = battle_grid[src]-1;
  if(entities[attacker].stamina < 0)
  {
    display_error_message("not enough stamina");
    return 2;
  }
  if(entities[attacker].actionable == 0)
  {
    display_error_message("unit has attacked");
    return 2;
  }
  if(entities[attacker].team != entities[target].team)
  {
    if(attackable(attacker,target,coords))
    {
      entities[attacker].actionable = 0;
      deduct_stamina(attacker);
      deduct_stamina(target);
      hide_menu();
      menu_x = -64;
      menu_y = -64;
      cursor_x = -32;
      cursor_y = -32;
      selector_mode = SELECT_MODE;
      return begin_battle(attacker,target,dst,src);
      // selector_mode = SELECT_MODE;
      // return result;
      // entities[attacker].actionable = 0;
    }
    else
    {
      display_error_message("unattackable");
      // put_number(attacker,4,23,1);
      return -1;
    }
  }
  else
  {
    display_error_message("invalid target");
    return -1;
  }
}

void clear_error_message()
{
  sync(60);
  put_string("                    ",10,1);
}

void update_map();
void begin_battle(int src, int dst);
void set_cursor_pos(int pos);
char square_adjacent(int origin, int desired);
