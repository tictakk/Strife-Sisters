#define PASS 0
//Objective
#define MAP_DEFEND 1
#define MAP_CAPTURE 0
#define MAP_TARGET 2

//AI states
#define READY 0
#define MOVING 1
#define ATTACKING 2
#define CAPTURING 3
#define PASSING 4

struct Ai_Entity{
  char state, entity_id, id, target, mv_rng, atk_rng;
  int dest;
};
char units_in_range[6];
char targetable_units[6];
char num_of_units_in_range;
char num_of_ai;
char num_of_targetable_units;
char target_unit;
char ai_tracker;
int hit_radius_size;
int opp_hit_radius[26];
struct Ai_Entity ai_entities[15];

init_ai()
{
  char i,j;
  num_of_ai = 0;
  j = 0;
  for(i=0; i<num_of_entities; i++)
  {
    if(entities[i].team == CPU)
    {
      ai_entities[j].entity_id = i;
      ai_entities[j].id = j;
      ai_entities[j].target = 0;
      ai_entities[j].dest = 0;
      ai_entities[j].state = READY;
      ai_entities[j].mv_rng = get_army_min_move(i);
      ai_entities[j++].atk_rng = get_army_max_range(i);
    }
  }
  num_of_ai = j;
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
    weakest = find_weakest_opp(units_in_range,num_of_units_in_range);

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
  for(ai_tracker=0; ai_tracker<num_of_ai; ai_tracker++)
  {

    ai_do_state(ai_entities[ai_tracker].id);
    check_battle_complete();
    if(map_result_status != MAP_RESULT_NONE)
    {
      break;
    }
    update_map();
    satb_update();
  }
  // if(map_result_status == MAP_RESULT_NONE)
  // {
    // break;
    start_turn(PLAYER);
  // }
}

void start_turn(char team)
{
  int camera_pos;
  char i, first;
  if(map_result_status != MAP_RESULT_NONE)
  {
    return;
  }
  first = 1;
  init_ai();
  turn = team;
  turns_count++;
  tactic_current = TACTIC_NONE;
  // display_turn(team);
  if(team == PLAYER)
  {
    s_y_relative = (s_y/8);
    display_popup("Player\n  Turn",0);
    load_map(0,2,0,0,16,29);
    s_y_relative = 0;
  }
  else
  {
    tactic_current = 0;
    s_y_relative = (s_y/8);
    display_popup("Enemy\n  Turn",0);
    load_map(0,2,0,0,16,29);
    s_y_relative = 0;
  }
  for(i=0; i<num_of_entities; i++)
  {
    if(entities[i].team == team)
    {
      entities[i].actionable = 1;
      entities[i].movable = 1;
      if(entities[i].has_cmdr)
      {
        entities[i].tactic_meter = min(entities[i].tactic_meter+3,MAX_TACTIC_METER);
      }
      // increment_meter(i);
    }
    load_commanders_palettes();
    // if(entities[i].team == PLAYER)
    // {
      // load_commander_palette(party_commanders[entities[i].id].sprite_type);
      // load_palette(get_commander_palette(party_commanders[entities[i].id].sprite_type))
    // }
  }

  camera_pos = entities[0].pos;
  if(team == PLAYER)
  {
    current_turn++;
  }
  center_camera(camera_pos);
  set_cursor_pos(camera_pos);
  satb_update();
  vsync();
}

void get_opps_in_range(char id, char range)
{
  int i, pos;
  num_of_units_in_range = 0;
  get_unit_radius(entities[id].pos,range,CPU,0);

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
}

char find_weakest_opp(char *units, char count)
{
  char i, weakest_id, id;
  int army_total, weakest_total;
  weakest_id = units[0];
  weakest_total = calc_unit_rating(units[0]);


  for(i=1; i<count; i++)//why did I put i=1?
  {
    id = units[i];
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
      load_unit_header(entities[id].bg->units->id,0);
      total += unit_header[0].def;
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
  //TODO:BUG HERE, len is never initialized
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
  int result = 0;

  num_of_targetable_units = 0;
  mv = get_army_min_move(ai_entities[ai_id].entity_id);
  rng = get_army_max_range(ai_entities[ai_id].entity_id);
  
  search_in_radius(entities[ai_entities[ai_id].entity_id].pos,mv+rng,0);
  highlight(entities[ai_entities[ai_id].entity_id].pos,0xC000);

  for(i=0; i<num_of_units_in_range; i++)
  {
    result = determine_attackable_targets(ai_id,units_in_range[i],mv,rng);

    if(result)
    {
      targetable_units[num_of_targetable_units++] = units_in_range[i];
    }
  }

  ai_entities[ai_id].target = find_weakest_opp(targetable_units,num_of_targetable_units);

  result = determine_attackable_targets(ai_id,ai_entities[ai_id].target,mv,rng);

  if(result == 0)
  {
    unhighlight();
    ai_entities[ai_id].state = PASSING;
    return;
  }
  if(result == 2)
  {
    unhighlight();
    ai_entities[ai_id].state = ATTACKING;
    return;
  }
  unhighlight();
  ai_entities[ai_id].state = MOVING;
  ai_entities[ai_id].dest = result;
}

char determine_attackable_targets(char ai_id, char target_id, char mv, char rng)
{
  char i,j;
  get_unit_radius(entities[target_id].pos,rng,PLAYER,0);
  convert_map_to_array();
  get_unit_radius(entities[ai_entities[ai_id].entity_id].pos,mv,CPU,mv);

  for(i=0; i<hit_radius_size; i++)
  {
    for(j=0; j<map_size+1; j++)
    {
      if(opp_hit_radius[i] == map[j].ownPos && 
         opp_hit_radius[i] != entities[ai_entities[ai_id].target].pos &&
         is_traversable(opp_hit_radius[i])
         )
      {
        if(battle_grid[opp_hit_radius[i]]-1 == ai_entities[ai_id].entity_id)
        {
          return 2;
        }
        if(battle_grid[opp_hit_radius[i]] == 0)
        {
          // ai_entities[ai_id].state = MOVING;
          // ai_entities[ai_id].dest = opp_hit_radius[i];
          return opp_hit_radius[i];
        }
      }
    }
  }
  return 0;
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
  obj_in_range = search_in_radius(entities[ai_entities[ai_id].entity_id].pos,mv,mv);
  highlight(entities[ai_entities[ai_id].entity_id].pos,0xC000);
  if(obj_in_range && battle_grid[objective_pos] == 0)
  {
    ai_entities[ai_id].state = MOVING;
    ai_entities[ai_id].dest = objective_pos;
    return;
  }
  search_in_radius(entities[ai_entities[ai_id].entity_id].pos,mv+rng,mv);
  if(num_of_units_in_range == 1)
  {
    // if(is_adjacent(entities[ai_entities[ai_id].entity_id].pos))
    // {
    //   return;
    // }
    p = get_path(entities[ai_entities[ai_id].entity_id].pos,entities[units_in_range[0]].pos,path,battle_grid,CPU,mv,mv);
    // p = find_attackable_position(entities[ai_entities[ai_id].entity_id].pos,entities[units_in_range[0]].pos,rng,mv);
    if(p == 0)
    {
      ai_entities[ai_id].state = ATTACKING;
      ai_entities[ai_id].target = units_in_range[0];
      return;
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
  //doesn't even get here
  search_in_radius(entities[ai_entities[ai_id].entity_id].pos,mv,mv);
  p = find_nearest_unoccupied_position(entities[ai_entities[ai_id].entity_id].pos,objective_pos,map_size+1,map);

  ai_entities[ai_id].state = MOVING;
  ai_entities[ai_id].dest = p;
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
  // highlight(entities[ai_entities[ai_id].entity_id].pos,0xC000);
  if(battle_grid[dest] != 0)
  {
    sync(60);
    unhighlight();
    return;
  }
  satb_update();
  sync(10);
  move_unit(dest,entities[ai_entities[ai_id].entity_id].pos);
  set_cursor_pos(entities[ai_entities[ai_id].entity_id].pos);
  update_map();
  unhighlight();
  satb_update();
  sync(10);
}

void ai_do_state(char ai_id)
{
  center_camera(entities[ai_entities[ai_id].entity_id].pos);
  set_cursor_pos(entities[ai_entities[ai_id].entity_id].pos);
  satb_update();
  sync(60);
  if(ai_entities[ai_id].state == READY){ do_ready(ai_id); }
  if(ai_entities[ai_id].state == MOVING){ do_move(ai_id); }
  if(ai_entities[ai_id].state == ATTACKING){ do_attack(ai_id); }
  if(ai_entities[ai_id].state == PASSING){ do_pass(ai_id); }
}

void do_ready(char ai_id)
{
  switch(map_type)
  {
    case MAP_DEFEND: do_capture_objective(ai_id); break;
    default: do_defend_objective(ai_id); break;
    // case MAP_DEFEND: do_capture_objective(ai_id); break;
    // case MAP_TARGET: do_target_objective(ai_id); break;
  }
}

void do_move(char ai_id)
{
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
  char result, i;

  result = -1;
  satb_update();
  sync(30);
  set_cursor_pos(entities[ai_entities[ai_id].target].pos);
  satb_update();
  sync(30);
  result = attack_unit(entities[ai_entities[ai_id].target].pos,entities[ai_entities[ai_id].entity_id].pos,0);
  ai_entities[ai_id].target = 0;
  if(result == ATTACKER_DEFEAT)//ai died attacking
  {
    init_ai();
    // ai_tracker = max(ai_id-1,0);
    ai_tracker--;
    // num_of_ai--;
    // return;
  }
  else if(result == TARGET_DEFEAT)//killed target attacking, this should probably just be running init_ai all over again?
  {
    for(i=0; i<num_of_ai; i++)
    {
      ai_entities[i].entity_id -= 1;
    }
  }
}

// char is_adjacent(int position)
// {
//   if(((position&0xF) - 1) > -1 && battle_grid[position-1] == units_in_range[0])
// 	{
//     return 1;
// 	}
// 	if((position - 16) > -1 && battle_grid[position-16] == units_in_range[0])
// 	{
//     return 1;
// 	}
// 	if((position + 16) < 352 && battle_grid[position+16] == units_in_range[0])
// 	{
//     return 1;
// 	}
// 	if(((position&15) + 1) < 16 && battle_grid[position+1] == units_in_range[0])
// 	{
//     return 1;
// 	}
//   return 0;
// }

void do_pass(char ai_id)
{
  entities[ai_entities[ai_id].entity_id].actionable = 0;
  unhighlight();
  // check_battle_complete();
}

int find_nearest_unoccupied_position(int position, int destination, char range, struct Node *grid)
{
  char i;
  int closest_total, closest_x, closest_y, dest_x, dest_y,
      closest, diff_x, diff_y, diff_total, pos_x, pos_y;

  pos_x = position & 15;
  pos_y = position >> 4;

  closest = position;
  closest_x = pos_x;
  closest_y = pos_y;

  dest_x = destination & 15;
  dest_y = destination >> 4;

  diff_x = abs(closest_x - dest_x);
  diff_y = abs(closest_y - dest_y);
  closest_total = diff_x + diff_y;

  for(i=0; i<range; i++)
  {
    position = grid[i].ownPos;

    if(battle_grid[position] == 0 && is_traversable(position))
    {
      pos_x = position & 15;
      pos_y = position >> 4;

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
