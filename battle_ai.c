#define PASS 0
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

char player_units[6];
char units_in_range[6];
char num_of_units_in_range;
char num_of_ai;
char num_of_players;
char target_unit;
char action;
char ai_tracker;
int hit_radius_size;
int opp_hit_radius[24];

struct Ai_Entity ai_entities[15]; //should be 15 but gave an extra 5 for possible, non-enemy ais

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
    ai_do_state(ai_entities[ai_tracker].id);
    update_map();
    satb_update();
  }
  start_turn(PLAYER);
}

void start_turn(char team)
{
  int camera_pos;
  char i, first;
  first = 1;
  init_ai();
  turn = team;
  for(i=0; i<num_of_entities; i++)
  {
    if(entities[i].team)
    {
      if(first)
      {
        first = 0;
        camera_pos = entities[i].pos;
      }
      entities[i].actionable = 1;
      entities[i].movable = 1;
    }
  }
  if(team == PLAYER)
  {
    current_turn++;
  }
  reset_calling(team);
  center_camera(camera_pos);
  set_cursor_pos(camera_pos);
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
  for(i=1; i<num_of_units_in_range; i++)
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
  int i, total;
  total = 0;
  for(i=0; i<MAX_ARMY_SIZE; i++)
  {
    if(entities[id].bg->units[i].hp>0)
    {
      total += (int)entities[id].bg->units[i].unit->def;
    }
  }
  return total;
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
  char i = 0, j = 0, mv = 0, rng = 0, obj_in_range = 0;
  int p = 0;

  mv = get_army_min_move(ai_entities[ai_id].entity_id);
  rng = get_army_max_range(ai_entities[ai_id].entity_id);
  search_in_radius(entities[ai_entities[ai_id].entity_id].pos,mv+rng,mv);

  if(num_of_units_in_range > 0)
  {
    ai_entities[ai_id].target = find_weakest_opp();

    get_unit_radius(entities[ai_entities[ai_id].target].pos,rng,PLAYER,0);
    convert_map_to_array();
    get_unit_radius(entities[ai_entities[ai_id].entity_id].pos,mv,CPU,0);
    for(i=0; i<hit_radius_size; i++)
    {
      for(j=0; j<map_size; j++)
      {
        if(opp_hit_radius[i] == map[j].ownPos && 
           opp_hit_radius[i] != entities[ai_entities[ai_id].target].pos &&
           battle_grid[opp_hit_radius[i]] == 0
           )
        {
          ai_entities[ai_id].state = MOVING;
          ai_entities[ai_id].dest = opp_hit_radius[i];
          break;
        }
      }
    }
  }
  else
  {
    ai_entities[ai_id].state = PASSING;
  }
}

void convert_map_to_array()
{
  int i;
  hit_radius_size = map_size+1;
  for(i=0; i<hit_radius_size; i++)
  {
    opp_hit_radius[i] = map[i].ownPos;
  }
}

void do_capture_objective(char ai_id)
{
  char i = 0, mv = 0, rng = 0, obj_in_range = 0;
  int p = 0;

  mv = get_army_min_move(ai_entities[ai_id].entity_id);
  rng = get_army_max_range(ai_entities[ai_id].entity_id);
  search_in_radius(entities[ai_entities[ai_id].entity_id].pos,mv,0);

  if(obj_in_range && battle_grid[objective_pos] == 0)
  {
    // put_number(mv,2,1,1);
    // wait_for_I_input();
    ai_entities[ai_id].state = MOVING;
    ai_entities[ai_id].dest = objective_pos;
    return;
  }
  search_in_radius(entities[ai_entities[ai_id].entity_id].pos,mv+rng,mv);
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
    search_in_radius(entities[ai_entities[ai_id].entity_id].pos,mv,mv);
    p = find_nearest_unoccupied_position(entities[ai_entities[ai_id].entity_id].pos,objective_pos,map_size,map);
    // put_number(p,3,1,2);
    // wait_for_I_input();
    ai_entities[ai_id].state = MOVING;
    ai_entities[ai_id].dest = p;
  }
}

void do_target_objective(char ai_id)
{
  int location = 0;
}

char search_in_radius(int position, char radius_size, char ignore_depth)
{
  char i=0, found=0;
  int pos=0, id=0;
  get_unit_radius(position,radius_size,CPU,ignore_depth);
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
  highlight(entities[ai_entities[ai_id].entity_id].pos,0xC000);
  satb_update();
  sync(10);
  move_unit(dest,entities[ai_entities[ai_id].entity_id].pos);
  set_cursor_pos(entities[ai_entities[ai_id].entity_id].pos);
  unhighlight();
  satb_update();
  sync(10);
}

void ai_do_state(char ai_id)
{
  // center_camera(graph_from_x_y(sx,sy));
  center_camera(entities[ai_entities[ai_id].entity_id].pos);
  set_cursor_pos(entities[ai_entities[ai_id].entity_id].pos);
  display_position(14,1);
  satb_update();
  sync(60);
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
  // put_number(ai_entities[ai_id].dest,3,0,0);
  // put_number(ai_entities[ai_id].target,3,0,1);
  // wait_for_I_input();
  if(ai_entities[ai_id].dest && ai_entities[ai_id].dest != entities[ai_entities[ai_id].entity_id].pos)
  {
    move_ai_unit(ai_id,ai_entities[ai_id].dest);
  }
  if(ai_entities[ai_id].target)
  {
    ai_entities[ai_id].state = ATTACKING;
    return;
  }
  //we should really figure out WHICH units range we want to use
  //what if our ai army one has one ranged unit out of 9, do we still want to
  //use the max range? Probably not
  ai_entities[ai_id].target = find_weakest_opp_in_range(ai_entities[ai_id].entity_id,get_army_max_range(ai_entities[ai_id].entity_id));

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

  satb_update();
  sync(30);
  set_cursor_pos(entities[ai_entities[ai_id].target].pos);
  satb_update();
  sync(30);
  result = attack_unit(entities[ai_entities[ai_id].target].pos,entities[ai_entities[ai_id].entity_id].pos,0);
  if(result == 1)
  {
    init_ai();
    ai_tracker = max(ai_id-1,0);
    // ai_tracker--;
    // num_of_ai--;
    // return;
  }
}

void do_pass(char ai_id)
{
  entities[ai_entities[ai_id].entity_id].actionable = 0;
}

int find_nearest_unoccupied_position(int position, int destination, char range, struct Node *grid)
{
  char i;
  int closest_total, closest_x, closest_y, dest_x, dest_y,
      closest, diff_x, diff_y, diff_total, pos_x, pos_y;

  pos_x = position & 15;
  pos_y = position / 16;

  closest = position;
  closest_x = pos_x;
  closest_y = pos_y;

  dest_x = destination & 15;
  dest_y = destination / 16;

  diff_x = abs(closest_x - dest_x);
  diff_y = abs(closest_y - dest_y);
  closest_total = diff_x + diff_y;

  for(i=0; i<range; i++)
  {
    position = grid[i].ownPos;

    if(battle_grid[position] == 0)
    {
      pos_x = position & 15;
      pos_y = position / 16;

      diff_x = abs(pos_x - dest_x);
      diff_y = abs(pos_y - dest_y);
      diff_total = diff_x + diff_y;

      if(diff_total < closest_total)
      {
        closest_total = diff_total;
        closest = position;
      }
    }
  }
  return closest;
}
