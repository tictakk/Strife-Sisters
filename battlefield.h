#define TILE_SIZE 16

#define SELECT_MODE 0
#define PLACE_MODE 1
#define MENU_MODE 2
#define SPLIT_MODE 3
#define ATTACK_MODE 4
#define PLAYER_ARMY_MODE 5
#define CPU_ARMY_MODE 6
#define ATTACK_WITH_ART 7
#define EXPLORE_MODE 8
#define EXPLORE_MENU_MODE 9
#define ACTION_MODE 10
//117 per entity
#define MAX_ENTITIES 21

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

#define ACTION_MOVE 0
#define ACTION_ATTACK 1
#define ACTION_CALLING 2

#define BATTLEFIELD_CENTER_Y 128

#define MASK_MOVE 0x01
#define MASK_ATTACK 0x02
#define MASK_END 0x04
#define MASK_TURN 0x08
#define MASK_CALLING 0x10
#define MASK_GROUP 0x20

typedef struct{
  int pos;
  char team, id, actionable, movable;
  Battlegroup *bg;
} Entity;

// unsigned char battle_grid[464];
unsigned char battle_grid[352];
Entity entities[MAX_ENTITIES];
char selector_mode;
int last_pos,squares;
char num_of_entities, menu_option,
     menu_columns, menu_vert_size, selected_option;
char one_total;
char two_total;
char turn, turns_count;
int path[10];
char no_of_player_cmdrs = 0;
char last_command;
char menu_mask;
char current_turn;
char enemy_count;
char units_lost, units_killed;
int turn_bonus, killed_bonus, lost_bonus, total_bonus, materials_collected;
char map_result_status;

int coords[24];
Entity *selected_entity;

char get_army_max_range(char entity_id)
{
  char i, max;
  max = 1;
  //TODO: can make this faster by next if statements. If hp is zero, don't bother getting range

  for(i=0; i<MAX_ARMY_SIZE; i++)
  {
    load_unit_header(entities[entity_id].bg->units[i].id,0);
    if(entities[entity_id].bg->units[i].hp && unit_header[0].rng > max)
    {
      max = unit_header[0].rng;
    }
  }
  return max;
}

char get_army_min_move(char entity_id)
{
  // char i, min;
  // min = MAX_MOVE_RANGE;

  // for(i=0; i<MAX_ARMY_SIZE; i++)
  // {
  //   if(entities[entity_id].bg->units[i].hp && entities[entity_id].bg->units[i].unit->mov < min)
  //   {
  //     min = entities[entity_id].bg->units[i].unit->mov;
  //   }
  // }

  // return min;
  return 3;
}

void add_entity(char team, char id, int pos, struct Commander *commanders)
{
  if(team == CPU)
  {
    entities[num_of_entities].id = id + MAX_PARTY_COMMANDERS;
  }
  else
  {
    entities[num_of_entities].id = id;
  }
  entities[num_of_entities].team = team;
  entities[num_of_entities].pos = pos;
  entities[num_of_entities].actionable = 1;
  entities[num_of_entities].movable = 1;
  // party_commanders[entities[num_of_entities]].meter = 0;
  if(team == PLAYER)
  {
    add_npc(pos%16,pos/16,commanders[id].sprite_type,CMDR_PALETTE);
  }
  else
  {
    add_npc(pos%16,pos/16,commanders[id].sprite_type,ENEMY_PALETTE);
  }
  entities[num_of_entities].bg = commanders[id].bg;
  set_bonuses(&commanders[id]);
  num_of_entities++;
}

void set_bonuses(struct Commander *commander)
{
  char i, j, type_counter, type;

  commander->bg.bonuses = 0;
  for(i=0; i<TYPE_COUNT; i++)
  {
    type_counter = 0;
    type = 1 << i;
    for(j=0; j<MAX_ARMY_SIZE; j++)
    {
      // if(commander->bg.units[j].id)
      // {
        load_unit_header(commander->bg.units[j].id,1);
      
        if(unit_header[1].a_type == type)
        {
          type_counter++;
        }
      // }
    }
    if(type_counter >= 3)
    {
      commander->bg.bonuses |= type;
    }
  }
}

void update_selector_pos(int x, int y)
{
  if(sy == 208 && y > 0 && s_y < 116) //scroll down
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

void destroy_entity(char id)
{
  int i, id_location;
  char entity_id;
  entity_id = id;
  battle_grid[entities[id].pos] = 0;
  if(entities[entity_id].team == PLAYER)
  {
    units_lost++;
    one_total--;
  }
  else
  {
    units_killed++;
    two_total--;
  }
  for(i=0; i<352; i++)
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

  select_unit(0);
}

void remove_unit_from_grid(int grid_pos)
{
  char entity_id, i;
  entity_id = battle_grid[grid_pos]-1;

  battle_grid[grid_pos] = 0;
  for(i=0; i<352; i++)
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
  // unit_selected = to;
  entities[id].movable = 0;
  // selector_mode = SELECT_MODE;
  // update_map();
}

void move_unit_new(int to)
{
  last_pos = selected_entity->pos;
  battle_grid[to] = battle_grid[last_pos];
  battle_grid[last_pos] = 0;
  selected_entity->pos = to;
  update_map();
}

void select_unit(char entity_id)
{
  // put_number(entity_id,4,0,0);
  selected_entity = &entities[entity_id];
  // selector_mode = PLACE_MODE;
  // menu_option = MENU_ATTACK;
  // draw_selector();
  // highlight(pos,0xC000);
}

void display_move_range(int pos)
{
  // put_number(pos,4,0,0);
  // selector_mode = PLACE_MODE;
  // menu_option = MENU_ATTACK;
  draw_selector();
  highlight(pos,ACTION_MOVE);
}

char attack_unit(int src, int dst, char art)
{
  char result;
  unsigned char attacker, target, range;
  // int range; //,item_no;//, result;

  attacker = battle_grid[dst]-1;
  target = battle_grid[src]-1;
  range = (unsigned char) get_range_from_distance(dst,src);
  result = 2;
  if(entities[attacker].team != entities[target].team)
  {
    last_command = selector_mode;
    entities[attacker].actionable = 0;
    // if(entities[attacker].team == PLAYER)
    // {
    //   darken_palette(26+attacker);
    // }
    hide_menu();
    cursor_x = -32;
    cursor_y = -32;
    selector_mode = SELECT_MODE;
    result = begin_battle(attacker,target,range,battlefieldbat[map_offset+dst],battlefieldbat[map_offset+src]);
  }
  return result;
}

char get_range_from_distance(int dist_1, int dist_2)
{
  char x_1, x_2, y_1, y_2;
  x_1 = dist_1 & 0xF; y_1 = dist_1 >> 4;
  x_2 = dist_2 & 0xF; y_2 = dist_2 >> 4;

  return abs(x_1 - x_2) + abs(y_1 - y_2);
}

void clear_text_field()
{
  put_string("                ",1,1);
  put_string("                ",1,2);
}

void post_battle_screen()
{
  s_y_relative = (s_y>>3);
  s_x_relative = (s_x>>3);
  // display_abs_black_panel(0,((s_y+32)/8),32,16);
  hide_npcs(5);
  spr_hide(0);
  remove_terrain_items();
  satb_update();

  display_window_rel(4,6,22,18);
  if(map_result_status == MAP_RESULT_WIN)
  {
    put_string("Victory",s_x_relative+12,s_y_relative+8);
  }
  else if(map_result_status == MAP_RESULT_LOSE)
  {
    put_string("Defeat",s_x_relative+12,s_y_relative+8);
  }
  else
  {
    put_string("Error result",s_x_relative+12,s_y_relative+8);
  }

  get_units_kill_bonus();
  get_units_lost_bonus();
  get_turn_bonus();
  get_total_bonus();

  write_text(s_x_relative+6,s_y_relative+12,"Turns        -");
  put_number(turns_count/2,3,s_x_relative+21,s_y_relative+12);
  write_text(s_x_relative+6,s_y_relative+13,"Turn bonus   -");
  display_number_incrementing(s_x_relative+21,s_y_relative+13,turn_bonus,3);

  write_text(s_x_relative+6,s_y_relative+14,"Units killed -");
  put_number(units_killed,2,s_x_relative+22,s_y_relative+14);
  write_text(s_x_relative+6,s_y_relative+15,"Killed Bonus -");
  display_number_incrementing(s_x_relative+21,s_y_relative+15,killed_bonus,3);

  write_text(s_x_relative+6,s_y_relative+16,"Units lost   -");
  put_number(units_lost,2,s_x_relative+22,s_y_relative+16);
  write_text(s_x_relative+6,s_y_relative+17,"Lost bonus   -");
  display_number_incrementing(s_x_relative+21,s_y_relative+17,lost_bonus,3);

  put_string("$$$$$$$$$$$$$$$$$$",s_x_relative+6,s_y_relative+18);

  write_text(s_x_relative+6,s_y_relative+19,"Total");
  put_number(total_bonus,4,s_x_relative+20,s_y_relative+19);
  // write_text(s_x_relative+6,s_y_relative+20,"Ore       ");
  // display_number_incrementing(s_x_relative+24,s_y_relative+20,materials_collected,3);

  write_text(s_x_relative+12,s_y_relative+22,"Grade");
  set_font_pal(9);
  put_char(get_map_grade_result(map_no,total_bonus),s_x_relative+18,s_y_relative+22);
  set_font_pal(10);
  // put_char('S',s_x_relative+18,s_y_relative+22);
  // put_number(map_grades[3],5,0,0);
  player_gold += total_bonus;
  materials_count += materials_collected;
  wait_for_I_input();
}

void get_units_kill_bonus()
{
  //max == 525
  if(map_result_status == MAP_RESULT_LOSE)
  {
    killed_bonus = 0;
    return;
  }
  killed_bonus = min(525,units_killed * 35);
}

void get_units_lost_bonus()
{
  //max == 500
  if(map_result_status == MAP_RESULT_LOSE)
  {
    lost_bonus = 0;
    return;
  }
  lost_bonus = max(500-(units_lost * 65),0);
}

void get_turn_bonus()
{
  if(map_result_status == MAP_RESULT_LOSE)
  {
    turn_bonus = 0;
    return;
  }
  turn_bonus = max(8-(turns_count>>2),0) * 100;

  //max == 900 I guess? Not really possible
}

void get_total_bonus()
{
  total_bonus = killed_bonus + lost_bonus + turn_bonus;
}

void print_post_battle_info(char *str, int value)
{
  clear_text_field();
  write_text(1,1,str);
  put_number(value,4,1,2);
  wait_for_I_input();
}

void undo()
{
  set_cursor_pos(last_pos);
  get_unit_radius(last_pos,get_army_min_move(get_entity_id(selected_entity->pos)),selected_entity->team,0);
  move_unit(last_pos,selected_entity->pos);
  // move_unit_new(last_pos);
  selected_entity->movable = 1;
  set_menu_mask(selected_entity->pos);
  print_menu();
  display_move_range(selected_entity->pos);
  selector_mode = PLACE_MODE;
  menu_option = MENU_ATTACK;
  hide_cursor();
}

void set_menu_mask(char entity_id)
{
  menu_mask = MASK_GROUP;
  if(entities[entity_id].team == CPU)
  {
    return;
  }
  menu_mask |= MASK_TURN;
  // if(arts[entities[entity_id].bg->art].cost <= party_commanders[entities[entity_id].id].meter && entities[entity_id].bg->art)
  // {
  //   menu_mask |= 0x10;
  // }
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
  if(menu_mask & 0x10) { put_string("TAC",24,1); }
  if(menu_mask & 0x20) { put_string("ARM",28,1); }
}

void print_menu()
{
  set_font_pal(11);
  put_string("MOV",20,1);
  put_string("ATK",20,2);
  put_string("END",24,2);
  put_string("TRN",28,2);
  put_string("TAC",24,1);
  put_string("ARM",28,1);
  set_font_pal(10);
  mask_menu();
}

void center_camera(int position)
{
  int new_offset;
  new_offset = get_valid_map_s_y(position);
  s_y = new_offset;
  yOffset = 0 - new_offset;
  update_map();
  scroll(0,s_x,s_y+32,32,224,0xC0);
}

void pan_camera_y(int position)
{
  int desired_s_y;
  desired_s_y = get_valid_map_s_y(position);

  // hide_npcs(6);
  spr_hide(0);
  spr_hide(1);
  spr_hide(2);
  spr_hide(3);
  spr_hide(5);
  spr_hide(6);
  draw_npcs(6);
  satb_update();

  while(desired_s_y != s_y)
  {
    if(desired_s_y - s_y < 0)
    {
      ++yOffset;
      scroll(0,s_x,--s_y+menu_height,menu_height,224,0xC0);
      draw_npcs(6);
      cycle_terrain_items();
    }
    else
    {
      --yOffset;
      scroll(0,s_x,++s_y+menu_height,menu_height,224,0xC0);
      draw_npcs(6);
      cycle_terrain_items();
    }
    satb_update();
    vsync();
  }
}

int get_valid_map_s_y(int position)
{
  return min(max((((position>>4)<<4) - 128),0),112);
}

void check_end_turn()
{
  if(!remaining_unit_turns())
  {
    end_player_turn();
  }
}

void end_player_turn()
{
  selector_mode = SELECT_MODE;
  menu_option = MENU_ATTACK;
  last_pos = 0;
  // hide_cursor();
  check_battle_complete();
  start_turn(CPU);
  menu_mask = 0x00;
  print_menu();
  // hide_cursor();
}

void end_unit_turn(char entity_id)
{
  char item_no;
  selector_mode = SELECT_MODE;
  menu_option = MENU_ATTACK;
  entities[entity_id].actionable = 0;
  darken_palette(26+entity_id);
  menu_mask = 0x00;
  print_menu();
  hide_cursor();
  check_end_turn();
}

char remaining_unit_turns()
{
  char i, remain;
  remain = 0;
  for(i=0; i<MAX_ENTITIES; i++)
  {
    if(entities[i].team == PLAYER && entities[i].actionable)
    {
      remain++;
    }
  }
  return remain;
}

void walk_sprite(char entity_id, int location, int distance)
{
  char i;
  int x, y, desired_x, desired_y, loc_x, loc_y;
  
  spr_set(get_entity_sprite_no(entity_id));
  spr_show();
  satb_update();
  x = spr_get_x();
  y = spr_get_y();
  loc_y = ((location>>4)<<4);
  loc_x = (location&15)<<4;
  npcs[entity_id].pos_y = loc_y>>4;
  npcs[entity_id].pos_x = location&15;
  desired_x = x - loc_x;
  desired_y = y - loc_y+16+s_y;

  while(desired_x != 0 || desired_y != 0)
  {
    if(desired_x < 0)
    {
      spr_x(spr_get_x()+2);
      desired_x+=2;
    }
    if(desired_x > 0)
    {
      spr_x(spr_get_x()-2);
      desired_x-=2;
    }
    if(desired_y < 0)
    {
      spr_y(spr_get_y()+2);
      desired_y+=2;
    }
    if(desired_y > 0)
    {
      spr_y(spr_get_y()-2);
      desired_y-=2;
    }    
    satb_update();
    vsync();
  }
}

void walk_entity(char entity_id, int position)
{
  select_unit(entity_id);
  walk_sprite(entity_id,position,0);
  move_unit_new(position);
}

char get_entity_sprite_no(char entity_id)
{
  char sprite_counter;
  int index;
  sprite_counter = 63;
  squares = 224+s_y;

  for(index=s_y+32; index<squares; index++)
  {
    if(battle_grid[index] != 0)
    {
      if(battle_grid[index]-1 == entity_id)
      {
        return sprite_counter;
      }
      sprite_counter--;
    }
  }
  return -1;
}

char get_entity_id(int position)
{
  return battle_grid[position]-1;
}

void update_map();
char begin_battle(char src, char dst, unsigned char dst_p, char a_terrain, char t_terrain);
void set_cursor_pos(int pos);

