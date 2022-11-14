#define PASS 0
#define ATTACK 1
#define MOVE 2
#define PASS_PTS 50

//Objective
#define DEFEND 0
#define CAPTURE 1
#define TARGET 2

//AI states
#define READY 0
#define MOVING 1
#define ATTACKING 2
#define CAPTURING 3
#define PASSING 4

struct Ai_Entity{
  char state, entity_id, id, action, target;
  int dest;
};

char ai_units[21];
char player_units[21];
char units_in_range[16];
char num_of_units_in_range;
char num_of_ai;
char num_of_players;
char target_unit;
char action;
char ai_tracker;
int objective_pos;

struct Ai_Entity ai_entities[20]; //should be 15 but gave an extra 5 for possible, non-enemy ais

init_ai()
{
    char i,j,m;
    num_of_players = 0;
    num_of_ai = 0;
    j = 0;
    m = 0;
    for(i=0; i<num_of_entities; i++)
    {
      if(entities[i].team == CPU)
      {
        ai_entities[j].entity_id = i;
        ai_entities[j].id = j;
        ai_entities[j].target = 0;
        ai_entities[j].dest = 0;
        ai_entities[j++].state = READY;
      }
      else if(entities[i].team == PLAYER)
      {
        player_units[m++] = i;
      }
    }
    num_of_ai = j;
    num_of_players = m;
}

int find_weakest_opp_in_range(char id, char range)
{
  char weakest;
  int target_pos;
  weakest = -1;
  target_pos = -1;
  get_opps_in_range(id,range);

  while(target_pos == -1 && num_of_units_in_range > 0)
  {
    weakest = find_weakest_opp();

    target_pos = get_attack_square(weakest,id);
    if(target_pos == -1)
    {
      pop_unit_from_range(weakest);
    }
  }
  return weakest;
}

ai()
{
  char f=0;
  g=0;
  for(ai_tracker=0; ai_tracker<num_of_ai; ai_tracker++)
  {
    // set_cursor_pos(entities[ai_units[ai_tracker]].pos);
    // set_cursor_pos(entities[ai_entities[ai_tracker].entity_id].pos);
    // display_position(14,1);
    // satb_update();
    // sync(30);
    ai_do_state(ai_entities[ai_tracker].id);
    update_map();
    satb_update();
  }
  start_turn(PLAYER);
}

void start_turn(char team)
{
  char i, unit_type;
  init_ai();
  turn = team;

  for(i=0; i<num_of_entities; i++)
  {
    if(entities[i].team)
    {
      unit_type = entities[i].id;
      entities[i].actionable = 1;
    }
  }
  update_map();
  // init_ai();
  // draw_actions();
  // update_map();
  // cycle_animations();
  satb_update();
  vsync();
}

void get_opps_in_range(char id, char range)
{
  int i, pos;
  num_of_units_in_range = 0;
  get_unit_radius(entities[id].pos,range,CPU,1);

  for(i=0; i<map_size+1; i++)
  {
    // pos = map[i].ownX + (map[i].ownY*16);
    pos = map[i].ownPos;
    if(battle_grid[pos] != 0)
    {
      if(entities[battle_grid[pos]-1].team != CPU)
      {
        units_in_range[num_of_units_in_range++] = battle_grid[pos]-1;
      }
    }
  }
  g = 0;
}

char find_weakest_opp()
{
  char i, weakest_id, id;
  int army_size, army_atk, army_def, army_total, weakest_total;
  weakest_id = units_in_range[0];
  weakest_total = calc_unit_rating(units_in_range[0]);
  for(i=0; i<num_of_units_in_range; i++)
  {
    id = units_in_range[i];
    army_total = calc_unit_rating(id);
    if(army_total < weakest_total)
    {
      weakest_total = army_total;
      weakest_id = id;
    }
  }
  return weakest_id;
}

int calc_unit_rating(char id)
{
  int army_size, atk, def;
  army_size = entities[id].army_size;
  atk = 15;
  def = 15;
  return atk + (def*2);
}

void pop_unit_from_range(char id)
{
  char i, location;
  location = 0;
  for(i=0; i<num_of_units_in_range; i++)
  {
    if(units_in_range[i] == id)
    {
      location = i;
    }
  }

  for(i=location; i<num_of_units_in_range; i++)
  {
    units_in_range[i] = units_in_range[i+1];
  }
  num_of_units_in_range--;
}

int get_attack_square(char id, char atkr)
{
  int pos, square;
  char i, j, len;
  square = entities[id].pos;
  // len = get_pattern_length(atkr);
  for(i=0; i<map_size+1; i++)
  {
    for(j=0; j<len; j++)
    {
      pos = square+coords[j*2]+coords[(j*2)+1];
      if(pos == (map[i].ownPos) && (battle_grid[pos] == 0) && (is_traversable(pos) || 0))//entities[atkr].unit->ign))
      {
        return pos;
      }
    }
  }
  return -1;
}

void do_defend_objective(char ai_id)
{
  char i = 0, obj_in_range = 0;
  int p = 0;

  search_in_radius(entities[ai_entities[ai_id].entity_id].pos,4);
  if(num_of_units_in_range > 0)
  {
    ai_entities[ai_id].target = find_weakest_opp();
    // put_number(ai_entities[ai_id].target,3,1,1);
    p = get_path(entities[ai_entities[ai_id].entity_id].pos,entities[ai_entities[ai_id].target].pos,path,battle_grid,CPU,60,0);
    // put_number(battle_grid[path[0]],3,1,1);
    for(i=p; i>=0; i--)
    {
      // put_number(path[p-i],3,1,1+(g++));
      if(battle_grid[path[p-i]] == 0)
      {
        ai_entities[ai_id].state = MOVING;
        ai_entities[ai_id].dest = path[p-i];
        // break;
        return;
      }
      // ai_entities[ai_id].state = PASSING;
      ai_entities[ai_id].state = ATTACKING;
    }
  }
  else
  {
    ai_entities[ai_id].state = PASSING;
  }
}

void do_capture_objective(char ai_id)
{
  char i = 0, obj_in_range = 0;
  int p = 0;

  obj_in_range = search_in_radius(entities[ai_entities[ai_id].entity_id].pos,3);

  if(obj_in_range && battle_grid[objective_pos] == 0)
  {
    ai_entities[ai_id].state = MOVING;
    ai_entities[ai_id].dest = objective_pos;
    return;
  }
  search_in_radius(entities[ai_entities[ai_id].entity_id].pos,4);
  if(num_of_units_in_range == 1)
  {
    // wait_for_I_input();
    p = get_path(entities[ai_entities[ai_id].entity_id].pos,entities[units_in_range[0]].pos,path,battle_grid,CPU,60,0);
    if(p == 0)
    {
      ai_entities[ai_id].state = ATTACKING;
      ai_entities[ai_id].target = units_in_range[0];
    }
    else
    {
      for(i=p; i>=0; i--)
      {
        if(battle_grid[path[p-i]] == 0)
        {
          ai_entities[ai_id].state = MOVING;
          ai_entities[ai_id].dest = path[p-i];
          ai_entities[ai_id].target = units_in_range[0];
          return;
        }
      }
    }
  }
  else
  {
    p = find_nearest_unoccupied_position(entities[ai_entities[ai_id].entity_id].pos,objective_pos,map_size,map);
    ai_entities[ai_id].state = MOVING;
    ai_entities[ai_id].dest = p;
  }
}

void do_target_objective(char ai_id)
{
  int location = 0;
}

char search_in_radius(int position, char radius_size)
{
  char i=0, found=0;
  int pos=0, id=0;
  get_unit_radius(position,radius_size,CPU,1);
  num_of_units_in_range = 0;

  for(i=0; i<map_size+1; i++)
  {
    pos = map[i].ownPos;
    id = battle_grid[pos];
    if(pos == objective_pos)
    {
      found = 1;
    }
    if(id)
    {
      if(entities[id-1].team != CPU)
      {
        units_in_range[num_of_units_in_range] = id-1;
        num_of_units_in_range++;
      }
    }
  }
  return found;
}

void move_ai_unit(char ai_id, int dest)
{
  if(battle_grid[dest] != 0)
  {
    return;
  }
  highlight(entities[ai_entities[ai_id].entity_id].pos,0xC000,3);
  satb_update();
  sync(60);
  move_unit(dest,entities[ai_entities[ai_id].entity_id].pos);
  set_cursor_pos(entities[ai_entities[ai_id].entity_id].pos);
  unhighlight();
  satb_update();
  sync(30);
}

void ai_do_state(char ai_id)
{
  set_cursor_pos(entities[ai_entities[ai_id].entity_id].pos);
  display_position(14,1);
  satb_update();
  sync(30);
  if(ai_entities[ai_id].state == READY){ do_ready(ai_id); }
  if(ai_entities[ai_id].state == MOVING){ do_move(ai_id); }
  if(ai_entities[ai_id].state == ATTACKING){ do_attack(ai_id); }
  if(ai_entities[ai_id].state == PASSING){ do_pass(ai_id); }
}

void do_ready(char ai_id)
{
  switch(ai_objective)
  {
    case DEFEND: do_defend_objective(ai_id); break;
    case CAPTURE: do_capture_objective(ai_id); break;
    case TARGET: do_target_objective(ai_id); break;
  }
}

void do_move(char ai_id)
{
  if(ai_entities[ai_id].dest && ai_entities[ai_id].dest != entities[ai_entities[ai_id].entity_id].pos)
  {
    move_ai_unit(ai_id,ai_entities[ai_id].dest);
  }
  ai_entities[ai_id].target = find_weakest_opp_in_range(ai_entities[ai_id].entity_id,1);

  if(ai_entities[ai_id].target != -1)
  {
    ai_entities[ai_id].state = ATTACKING;
    ai_entities[ai_id].target;
  }
  else
  {
    ai_entities[ai_id].state = PASSING;
  }
}

void do_attack(char ai_id)
{
  char result, track_tracker;
  // track_tracker = ai_tracker;
  pattern(entities[ai_entities[ai_id].entity_id].pos,0xD000,coords,1);
  satb_update();
  sync(30);
  set_cursor_pos(entities[ai_entities[ai_id].target].pos);
  satb_update();
  sync(30);
  result = attack_unit(entities[ai_entities[ai_id].target].pos,entities[ai_entities[ai_id].entity_id].pos);
  if(result == 1)
  {
    // ai_id--;
    // init_ai();
    // ai_tracker = track_tracker;
    return;
  }
  // ai_entities[ai_id].state = PASSING;
}

void do_pass(char ai_id)
{
  entities[ai_entities[ai_id].entity_id].actionable = 0;
}

int find_nearest_unoccupied_position(int position, int destination, char range, struct Node *grid)
{
  char i;
  int closest, distance, distance_x, distance_y, distance_diff,
      closest_x, closest_y, closest_diff;

  closest = position;
  closest_diff = abs(position - destination);
  closest_x = closest_diff % 16;
  closest_y = closest_diff / 16;

  for(i=0; i<range; i++)
  {
    distance = grid[i].ownPos;

    if(battle_grid[grid[i].ownPos] == 0)
    {
      distance_diff = abs(distance - destination);
      distance_x = distance_diff % 16;
      distance_y = distance_diff / 16;

      if((distance_x + distance_y) < (closest_x + closest_y))
      {
        closest = distance;
        closest_diff = distance_diff;
        closest_x = distance_x;
        closest_y = distance_y;
      }
    }
  }
  return closest;
}
