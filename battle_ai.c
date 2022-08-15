#define PASS 0
#define ATTACK 1
#define MOVE 2
#define PASS_PTS 50

// int attackable_squares[4];
char ai_units[21];
char player_units[21];
char units_in_range[16];
char num_of_units_in_range;
char num_of_ai;
char num_of_players;
char done;
char target_unit;
char action;
char ai_tracker;
// char weakest_opp;

init_ai()
{
    char i,j,m;
    num_of_players = 0;
    num_of_ai = 0;
    j = 0;
    m = 0;
    for(i = 0; i<num_of_entities; i++)
    {
      if(entities[i].team == CPU)
      {
        ai_units[j++] = i;
      }
      else if(entities[i].team == PLAYER)
      {
        player_units[m++] = i;
      }
    }
    num_of_ai = j;
    num_of_players = m;
    // find_weakest_opp();
    // print_unit_type((*entities[weakest_opp].unit).unit_type,20,6);

    // put_number(weakest_opp,3,0,9);
}

int determine_unit_action(char id)
{
  char weakest, i, j;
  int target, len;
  target = -1;

  // if(entities[id].stamina < 0)
  // {
  //   action = PASS;
  //   return target;
  // }

  units_in_atk_zone(id);

  if(num_of_units_in_range != 0)
  {
    weakest = find_weakest_opp();
    action = ATTACK;
    return weakest;
  }
  switch(entities[id].stamina)
  {
    case 0:
    case 1:
    target = -1;
    action = PASS;
    break;

    case 2:
    target = find_weakest_opp_in_range(id,(*entities[id].unit).mov+(*entities[id].unit).rng);
    if(target == -1)
    {
      action = PASS;
    }
    else
    {
      action = MOVE;
    }
    break;

    case 3:
    target = find_weakest_opp_in_range(id,(*entities[id].unit).mov+(*entities[id].unit).rng);
    if(target == -1)
    {
      // target = find_weakest_opp_in_range(id,((*entities[id].unit).mov*2)+(*entities[id].unit).rng);
      target = find_critical_unit();
      if(target != -1)
      {
        len = get_path(entities[id].pos,entities[target].pos,path,battle_grid,2,6,entities[id].unit->ign);
        get_unit_radius(entities[id].pos,(*entities[id].unit).mov,CPU,0);

        for(i=0; i<len; i++)
        {
          for(j=0; j<map_size+1; j++)
          {
            if(path[i] == map[j].ownX + (map[j].ownY*16) && battle_grid[path[i]] == 0)
            {
              action = MOVE;
              return path[i];
            }
          }
        }
        action = PASS;
        return -1;
      }
      action = PASS;
      return -1;
    }
    action = MOVE;
    break;
  }
  // action = PASS;
  return target;
}

int find_weakest_opp_in_range(char id, char range)
{
  char weakest;
  int target_pos;
  target_pos = -1;
  get_opps_in_range(id,range);
  // put_number(num_of_units_in_range,3,0,9+(g++));

  while(target_pos == -1 && num_of_units_in_range > 0)
  {
    weakest = find_weakest_opp();
    get_attackable_squares(weakest,id);
    target_pos = get_attack_square(weakest,id);
    if(target_pos == -1)
    {
      pop_unit_from_range(weakest);
    }
  }
  // put_number(target_pos,3,0,9+(g++));
  return target_pos;
}

char find_critical_unit()
{
  int lowest, rating;
  char i, id;
  lowest = 9999;
  id = -1;
  for(i=0; i<num_of_players; i++)
  {
    rating = calc_unit_rating(player_units[i]);
    // put_number(rating,4,0,9+(g++));
    if (lowest > rating)
    {
      lowest = rating;
      id = player_units[i];
    }
  }

  if(lowest < 600)
  {
    return id;
  }
  return -1;
}

void units_in_atk_zone(char id)
{
  int square, pos;
  char weakest, len, i, opp_id;
  num_of_units_in_range = 0;
  pos = entities[id].pos;
  load_coords(id);
  len = get_pattern_length(id);
  for(i=0; i<len; i++)
  {
    square = pos+coords[i*2]+coords[(i*2)+1];
    opp_id = battle_grid[square];
    if(square < 224 && opp_id != 0 && entities[opp_id-1].team == PLAYER)
    {
      units_in_range[num_of_units_in_range++] = opp_id-1;
    }
  }
}

char units_in_move_zone(char id){}

char units_in_danger_zone(char id){}

// char calc_action_points(char ai_id, char player_id)
// {
//   int pts_total, distance;
//   pts_total = 0;
//   // distance = (calc_move_cost(entities[ai_id].pos,entities[player_id].pos) / 2)-1;
//   distance = has_path(ai_id,player_id);
//   // put_number(distance,3,5,10);
//   pts_total += get_distance_points(distance);
//   //unit stamina
//   // pts_total += (-15 + (entities[ai_id].stamina * 10));
//   pts_total += (entities[ai_id].stamina * 5);
//   //enemy health
//   pts_total += get_health_points(entities[player_id].army_size);
//   //enemy stamina
//   // pts_total += (15 - (entities[player_id].stamina * 5));
//   pts_total += (entities[player_id].stamina * 5) * -1;
//   //nearby engaged
//   pts_total += (entities[player_id].nearby_engaged * 15);
//   return pts_total;
// }
//
// char has_path(char ai_id, char player_id)
// {
//   char len;
//   len = get_path(entities[ai_id].pos,entities[player_id].pos,path,battle_grid,2,4);
//   if(len == -1)
//   {
//     return 99;
//   }
//   else
//   {
//     return len;
//   }
// }
//
// char get_distance_points(char distance)
// {
//   if(distance == 0)
//   {
//     return 30;
//   }
//   if(distance < 4) //move distance
//   {
//     return 15;
//   }
//   if(distance > 6) //outside move distance +2
//   {
//     return -30;
//   }
//   if(distance > 3) //outside move distance
//   {
//     return -15;
//   }
// }
//
// char get_health_points(char num_of_units)
// {
//   if(num_of_units > 11)
//   {
//     return 0;
//   }
//   if(num_of_units > 7)
//   {
//     return 5;
//   }
//   if(num_of_units > 4)
//   {
//     return 10;
//   }
//   return 15;
// }

ai()
{
  g = 0;
  for(ai_tracker=0; ai_tracker<num_of_ai; ai_tracker++)
  {
    set_cursor_pos(entities[ai_units[ai_tracker]].pos);
    satb_update();
    sync(45);
    ai_unit(ai_units[ai_tracker]);
  }
  start_turn(PLAYER);
}

void ai_unit(char id)
{
  int traveled, pos;
  char d, result, continue_move;
  continue_move = 1;
  // if(entities[id].actionable)
  while(continue_move && entities[id].stamina > 0)
  {
      pos = determine_unit_action(id);
      // put_number(pos,3,0,9+(g++));
      switch(action)
      {

        case PASS:
        continue_move = 0;
        update_map();
        draw_actions();
        break;

        case MOVE: //move a unit
        set_cursor_pos(pos);
        highlight(entities[id].pos,0xB000,3);
        satb_update();
        sync(30);
        move_unit(pos,entities[id].pos);
        update_map();
        draw_actions();
        satb_update();
        // ai_timer = 20;
        break;

        case ATTACK: //attack a unit
        pattern(entities[id].pos,0xC000,coords,1);
        set_cursor_pos(entities[pos].pos);
        satb_update();
        sync(60);
        result = attack_unit(entities[pos].pos,entities[id].pos);
        if(result == id)
        {
          ai_tracker--;
        }
        put_string("             ",6,2);
        continue_move = 0;
        init_ai();
        break;
      }
      // vysnc();
      satb_update();
      sync(30);
    }
}

void start_turn(char team)
{
  char i;
  init_ai();
  actions = ACTIONS_PER_TURN;
  turn = team;

  for(i=0; i<num_of_entities; i++)
  {
    if(team == entities[i].team)
    {
      entities[i].actionable = 1;
      replenish_stamina(i);
    }
  }
  init_ai();
  draw_actions();
  update_map();
  satb_update();
  vsync();
}

void replenish_stamina(char id)
{
  // if(entities[id].stamina < 3)
  // {
  //   entities[id].stamina++;
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

int furthest_can_travel(char path_length)
{
  char max_moves;
  max_moves = 6 / 2;
  if((path_length - max_moves)+1 > 0)
  {
    return path[(path_length - max_moves)+1];
  }
  else
  {
    // return path[max_moves - path_length];
    // put_number(path_length,3,5,10);
    return path[1];
  }
}

void get_opps_in_range(char id, char range)
{
  char i, pos;
  num_of_units_in_range = 0;
  get_unit_radius(entities[id].pos,range,entities[id].team,1);

  for(i=0; i<map_size+1; i++)
  {
    pos = map[i].ownX + (map[i].ownY*16);
    if(battle_grid[pos] != 0)
    {
      if(entities[battle_grid[pos]-1].team != CPU)
      {
        units_in_range[num_of_units_in_range++] = battle_grid[pos]-1;
      }
      // continue;
    }
    // if(entities[battle_grid[pos]-1].team != CPU)
    // {
    //   units_in_range[num_of_units_in_range++] = battle_grid[pos]-1;
    // }
  }
}

char find_weakest_opp()
{
  char i, weakest_id, id;
  int army_size, army_atk, army_def, army_total, weakest_total;
  weakest_id = -1;
  weakest_total = 9999;
  for(i=0; i<num_of_units_in_range; i++)
  {
    id = units_in_range[i];
    // army_size = entities[id].army_size;
    // army_atk = entities[id].unit.atk * army_size;
    // army_def = entities[id].unit.def * army_size;
    // army_total = army_atk + army_def;
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
  atk = (int)entities[id].unit->atk * army_size;
  def = (int)entities[id].unit->def * army_size;
  return atk + (def*2);
  // put_number(entities[id].unit->atk,4,0,9+(g++));

  // return ((((int)entities[id].unit->atk) * army_size)*2) + (((int)entities[id].unit->def) * army_size);
  // return (entities[id].unit.def * army_size)
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

void get_attackable_squares(char trgt, char atkr)
{
  // int pos;
  // pos = entities[trgt].pos;
  load_coords(atkr);
  // attackable_squares[0] = pos-1; //left
  // attackable_squares[1] = pos-16; //up
  // attackable_squares[2] = pos+1; //right
  // attackable_squares[3] = pos+16; //down
}

int get_attack_square(char id, char atkr)
{
  int pos, square;
  char i, j, len;
  square = entities[id].pos;
  len = get_pattern_length(atkr);
  for(i=0; i<map_size+1; i++)
  {
    for(j=0; j<len; j++)
    {
      pos = square+coords[j*2]+coords[(j*2)+1];
      if(pos == (map[i].ownX + (map[i].ownY*16)) && (battle_grid[pos] == 0) && (is_traversable(pos) || entities[atkr].unit->ign))
      {
        // put_number(pos,3,0,9+(g++));
        return pos;
      }
    }
  }
  return -1;
}

print_path(char path_length)
{
  int i;
  for(i=0; i<path_length; i++)
  {
    // put_number(path[i],3,3,3+i);
    put_number(map[i].ownX+(map[i].ownY*16),3,3,5+i);
  }
}
