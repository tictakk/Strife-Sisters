#define TILE_SIZE 16

#define SELECT_MODE 0
#define PLACE_MODE 1
#define MENU_MODE 2
#define SPLIT_MODE 3
#define ATTACK_MODE 4
#define ARMY_MODE 5
#define OBSERVE_MODE 6
#define DIALOG_MODE 7
#define EXPLORE_MODE 8
#define EXPLORE_MENU_MODE 9
#define ACTION_MODE 10
//117 per entity
#define MAX_ENTITIES 21
#define PLAYER 1
#define CPU 2

#define MENU_ATTACK 1
#define MENU_MOVE 0
#define MENU_TAKE 2
#define MENU_END 3
#define MENU_GRP 4
#define MENU_TURN 5
#define MENU_SPLIT 2
#define MENU_MERGE 99
#define MENU_BACK 4

#define ARCHER_ATTACK 8
#define SWORD_ATTACK 4
#define SPEAR_ATTACK 4

#define COMMANDER_FIGURE_BEGIN 0x6000

typedef struct{
  int pos;
  char team, id, actionable, movable;
  unsigned char hp;
  Battlegroup *bg;
} Entity;

int gold_gained;
unsigned char battle_grid[464];//MAP_SIZE];
Entity entities[MAX_ENTITIES];
char selector_mode;
int unit_selected, last_pos;
char num_of_entities, menu_option, menu_rows,
     menu_columns, menu_vert_size, selected_option, selected_entity;
char one_total;
char two_total;
char cost;
char turn;
int path[20];
char no_of_player_cmdrs;
char last_command;
char menu_mask;
// const char spear_attack[4] = { 2, 0, 2, 0};

const int range_3_coord1[2] = {1,-16};
const int range_3_coord2[2] = {0,-32};
const int range_3_coord3[2] = {1,16};
const int range_3_coord4[2] = {0,32};
const int range_3_coord5[2] = {-1, -16};
const int range_3_coord6[2] = {-1, 16};
const int range_3_coord7[2] = {-2, 0};
const int range_3_coord8[2] = {2, 0};
const int range_3_coord9[2] = {0, 16};
const int range_3_coord10[2] = {1, 0};
const int range_3_coord11[2] = {-1, 0};
const int range_3_coord12[2] = {0, -16};
const int range_3_coord13[2] = {0,-48};
const int range_3_coord14[2] = {-3,0};
const int range_3_coord15[2] = {3,0};
const int range_3_coord16[2] = {0,48};
const int range_3_coord17[2] = {1, 32};
const int range_3_coord18[2] = {-1, 32};
const int range_3_coord19[2] = {1, -32};
const int range_3_coord20[2] = {-1, -32};
const int range_3_coord21[2] = {-2,-16};
const int range_3_coord22[2] = {2,-16};
const int range_3_coord23[2] = {-2,16};
const int range_3_coord24[2] = {2,16};

const int range_2_coord1[2] = {1,-16};
const int range_2_coord2[2] = {0,-32};
const int range_2_coord3[2] = {1,16};
const int range_2_coord4[2] = {0,32};
const int range_2_coord5[2] = {-1, -16};
const int range_2_coord6[2] = {-1, 16};
const int range_2_coord7[2] = {-2, 0};
const int range_2_coord8[2] = {2, 0};
const int range_2_coord9[2] = {0, 16};
const int range_2_coord10[2] = {1, 0};
const int range_2_coord11[2] = {-1, 0};
const int range_2_coord12[2] = {0, -16};

const int range_1_coord1[2] = {0, 16};
const int range_1_coord2[2] = {1, 0};
const int range_1_coord3[2] = {-1, 0};
const int range_1_coord4[2] = {0, -16};

// struct Coords coords[9];
int coords[24];

void load_coords(char id)
{
  int *coord;
  int i, len;
  len = get_pattern_length(id);
  switch(len)
  {
    case 4: coord = range_1_coord1; break;
    case 12: coord = range_2_coord1; break;
    case 24: coord = range_3_coord1; break;
  }

  len*=2;

  for(i=0; i<len; i++)
  {
    coords[i] = coord[i];
  }
}

char get_pattern_length(char id)
{
  return 24;
}

char get_army_max_range(char entity_id)
{
  char i, max;
  max = 1;
  for(i=0; i<MAX_ARMY_SIZE; i++)
  {
    if(entities[entity_id].bg->units[i].hp && entities[entity_id].bg->units[i].unit->rng > max)
    {
      max = entities[entity_id].bg->units[i].unit->rng;
    }
  }
  return max;
}

char get_army_min_move(char entity_id)
{
  char i, min;
  min = MAX_MOVE_RANGE;
  for(i=0; i<MAX_ARMY_SIZE; i++)
  {
    if(entities[entity_id].bg->units[i].hp && entities[entity_id].bg->units[i].unit->mov < min)
    {
      min = entities[entity_id].bg->units[i].unit->mov;
    }
  }
  return min;
}

void add_entity(char team, char pal, char id, int pos, struct Commander *commanders)
{
  entities[num_of_entities].id = id;
  entities[num_of_entities].team = team;
  entities[num_of_entities].pos = pos;
  entities[num_of_entities].actionable = 1;
  entities[num_of_entities].movable = 1;
  entities[num_of_entities].hp = 255;
  // party_commanders[entities[num_of_entities]].meter = 0;

  add_npc(pos%16,pos/16,commanders[id].sprite_type,pal);
  entities[num_of_entities].bg = commanders[id].bg;
  num_of_entities++;
}

void update_selector_pos(int x, int y)
{
  if(sy == 208 && y > 0 && s_y < 240) //scroll down
  {
    yOffset -= 16;
    s_y += 16;
    update_map();
    scroll(0,s_x,s_y+32,32,224,0xC0);
  }
  else if(sy == 32 && y < 0 && s_y > 0) //scroll up
  {
    yOffset += 16;
    s_y -= 16;
    update_map();
    scroll(0,s_x,s_y+32,32,224,0xC0);
  }
  sx += x;
  sy = min(max((sy + y),32),208);
  draw_selector();
}

void draw_selector()
{
  spr_set(SELECTOR);
  spr_x(sx);
  spr_y(sy);
}

void hide_menu()
{
  unhighlight();
  // hide_selector();
  hide_cursor();
  scroll(1,0,0,0,32,0x80);
}

void unhighlight()
{
  load_map(0,2,0,0,16,29);
}

void hide_cursor()
{
  remove_cursor();
}

void hide_selector()
{
  spr_set(SELECTOR);
  spr_hide();
}

char check_action_cost(char required_cost){ return 1; }

char destroy_entity(int id)
{
  //NEXT THING TO DO
  int i, id_location;
  char entity_id, killed;
  killed = 0;
  entity_id = id;
    battle_grid[entities[id].pos] = 0;
    killed = 1;
    if(entities[entity_id].team == PLAYER)
    {
      one_total--;
    }
    else
    {
//      get_random_item_by_level(0);
//      gold_gained += range(0,20);
      two_total--;
    }
    for(i=0; i<464; i++)
    {
      if(battle_grid[i] > entity_id+1)
      {
        battle_grid[i] -= 1;
      }
    }
    destroy_npc(entity_id);
    for(i=entity_id; i<num_of_entities; i++)
    {
      memcpy(&entities[i],&entities[i+1],sizeof(Entity)); //cpy dst from src
      // entities[i].id--;
    }
    num_of_entities--;
  return killed;
}

void remove_unit_from_grid(int grid_pos)
{
  char entity_id, i;
  entity_id = battle_grid[grid_pos]-1;

  battle_grid[grid_pos] = 0;
  for(i=0; i<464; i++)
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

void move_unit(int to, int from)
{
  char id;
  // hide_menu();
  id = battle_grid[from] - 1;

  last_pos = from;
  battle_grid[to] = battle_grid[from];
  battle_grid[from] = 0;
  entities[id].pos = to;
  unit_selected = to;
  entities[id].movable = 0;
  // selector_mode = SELECT_MODE;
  update_map();
}

void select_unit(int unit)
{
  char mv;
  mv = get_army_min_move(battle_grid[unit]-1);
  selector_mode = PLACE_MODE;
  menu_option = MENU_ATTACK;

  draw_selector();
  highlight(unit,0xC000,mv,mv);
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
  char result;
  int attacker, target, range;//, result;
  attacker = battle_grid[dst]-1;
  target = battle_grid[src]-1;
  range = get_range_from_distance(dst,src);
  result = 2;
  if(entities[attacker].team != entities[target].team)
  {
    // if(attackable(attacker,target,coords))
    last_command = selector_mode;
    entities[attacker].actionable = 0;
    hide_menu();
    cursor_x = -32;
    cursor_y = -32;
    selector_mode = SELECT_MODE;
    result = begin_battle(attacker,target,range,battlefieldbat[map_offset+dst],battlefieldbat[map_offset+src]);
  }
  else
  {
    return -1;
  }
  return result;
}

char get_range_from_distance(int dist_1, int dist_2)
{
  char x_1, x_2, y_1, y_2;
  x_1 = dist_1 & 0xF; y_1 = dist_1 / 16;
  x_2 = dist_2 & 0xF; y_2 = dist_2 / 16;

  return abs(x_1 - x_2) + abs(y_1 - y_2);
}

void clear_text_field()
{
  put_string("                ",1,1);
  put_string("                ",1,2);
}

void post_battle_dialog()
{
  clear_text_field();
  put_string("Map won!",1,1);
  // print_items_gained();
  // print_post_battle_info("Gold received",gold_gained);
  // print_post_battle_info("EXP received",exp_gained);
  // distribute_exp();

  // print_post_battle_info("Rei exp",commanders[party[0]].exp);
  // print_post_battle_info();
  wait_for_I_input();
  clear_text_field();
  //items gained
  //money gained
  //exp gained
  //level ups
  //spells learned
}

void print_items_gained()
{
  char i=0;
  while(i<drop_count)
  {
    clear_text_field();
    write_text(1,1,"received ");
    write_text(10,1,items[battle_items[i]].name);
    i++;
    wait_for_I_input();
  }
}

void print_post_battle_info(char *str, int value)
{
  clear_text_field();
  write_text(1,1,str);
  put_number(value,4,1,2);
  wait_for_I_input();
}

void print_level_up(){}
void print_spells_learned(){}

void print_new_stats(char cmdr_id)
{
  int offset;
  char i;

  for(i=0; i<4; i++) //4 == num of attributes
  {
    clear_text_field();
    // scale_stat(cmdr_id,i);
    // switch(i)
    // {
    //   case ATK_ATTRIBUTE:
    //   offset = write_text(1,1,"Atk ");
    //   put_number(commanders[cmdr_id].unit->atk,3,1+offset,1);
    //   break;
    //
    //   case DEF_ATTRIBUTE:
    //   offset = write_text(1,1,"Def ");
    //   put_number(commanders[cmdr_id].unit->def,3,1+offset,1);
    //   break;
    //
    //   case SPD_ATTRIBUTE:
    //   offset = write_text(1,1,"Speed ");
    //   put_number(commanders[cmdr_id].unit->spd,3,1+offset,1);
    //   break;
    //
    //   case HP_ATTRIBUTE:
    //   offset = write_text(1,1,"HP ");
    //   put_number(commanders[cmdr_id].unit->hp,4,1+offset,1);
    //   break;
    // }
    wait_for_I_input();
  }
}

void undo()
{
  set_cursor_pos(last_pos);
  move_unit(last_pos,unit_selected);
  entities[battle_grid[unit_selected]-1].movable = 1;
  set_menu_mask(battle_grid[unit_selected]-1);
  print_menu();
  select_unit(unit_selected);
  hide_cursor();
}

void set_menu_mask(char entity_id)
{
  char item_index;

  menu_mask = 0x28;
  if((item_index = item_at_position(entities[entity_id].pos)) != -1)
  {
    put_hex(menu_mask,3,6,1);
    menu_mask |= 0x10;
    put_hex(menu_mask,3,6,2);
  }
  if(entities[entity_id].actionable)
  {
    menu_mask |= 0x0E;
    if(entities[entity_id].movable)
    {
      menu_mask |= 0x1;
    }
  }
}

void mask_menu()
{
  if(menu_mask & 0x01) { put_string("MOV",20,1); }
  if(menu_mask & 0x02) { put_string("ATK",20,2); }
  if(menu_mask & 0x04) { put_string("END",24,2); }
  if(menu_mask & 0x08) { put_string("TRN",28,2); }
  if(menu_mask & 0x10) { put_string("TKE",24,1); }
  if(menu_mask & 0x20) { put_string("GRP",28,1); }
}

void print_menu()
{
  set_font_pal(11);
  put_string("MOV",20,1);
  put_string("ATK",20,2);
  put_string("END",24,2);
  put_string("TRN",28,2);
  put_string("TKE",24,1);
  put_string("GRP",28,1);
  set_font_pal(10);
  mask_menu();
}

void center_camera(int selector_abs)
{
  int new_offset;
  new_offset = min(max((((selector_abs/16)<<4) - 128),0),240);
  s_y = new_offset;
  yOffset = 0 - new_offset;
  update_map();
  scroll(0,s_x,s_y+32,32,224,0xC0);
}

void item_gained_text(char item_no)
{
  clear_text_field();
  put_string("Gained 10",1,1);
  switch(item_no)
  {
    case 0x7:
      put_string("red gems",1,2);
      red_crystal_count += 10;
      break;

    case 0xA:
      put_string("blue gems",1,2);
      blue_crystal_count += 10;
      break;

    case 0xD:
      put_string("green gems",1,2);
      green_crystal_count += 10;
      break;

    case 0x10:
      put_char('0',10,1);
      put_string("gold coins",1,2);
      player_gold += 100;
      break;
  }

  wait_for_I_input();
  clear_text_field();
}

void end_unit_turn(char entity_id)
{
  selector_mode = SELECT_MODE;
  menu_option = MENU_ATTACK;
  entities[entity_id].actionable = 0;
  menu_mask = 0x00;
  print_menu();
  hide_cursor();
}

void update_map();
void begin_battle(int src, int dst, int dst_p, char a_terrain, char t_terrain);
void set_cursor_pos(int pos);

