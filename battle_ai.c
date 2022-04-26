#define PASS 0
#define ATTACK 1
#define MOVE 2
#define PASS_PTS 50

char ai_units[18];
char player_units[18];
char num_of_ai;
char num_of_players;
char done;
char current_id;
char target_unit;
char action;

init_ai()
{
    char i,j,m;
    num_of_players = 0;
    j = 0;
    m = 0;
    for(i = 0; i<MAX_ENTITIES; i++)
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
    done = 1;
    current_id = ai_units[0];
}

void determine_unit_action(char id)
{
  char i;
  int points, most_points;
  most_points = PASS_PTS;
  action = PASS;
  // most_points = -50;
  // shortest_distance = calc_move_cost(entities[id],entities[player_units[0]]);
  for(i=0; i<num_of_players; i++)
  {
    // distance = calc_move_cost(entities[id],entities[player_units[0]]);
    // if(distance < shortest_distance)
    // {
    //   shortest_distance = distance;
    // }
    // put_number(entities[player_units[i]].stamina,3,5,10+i);

    points = calc_action_points(id,player_units[i]);

    if(points > most_points)
    {
      most_points = points;
      target_unit = player_units[i];
      if(((calc_move_cost(entities[id].pos,entities[player_units[i]].pos) / 2)-1) == 0)
      {
        action = ATTACK;
      }
      else
      {
        action = MOVE;
      }
    }
  }
  if(most_points == PASS_PTS)
  {
    action = PASS;
  }
  put_number(most_points,3,5,10+id);
}

char calc_action_points(char ai_id, char player_id)
{
  int pts_total, distance;
  pts_total = 0;
  distance = (calc_move_cost(entities[ai_id].pos,entities[player_id].pos) / 2)-1;
  // put_number(distance,3,5,10);
  pts_total += get_distance_points(distance);
  //unit sta
  pts_total += (-15 + (entities[ai_id].stamina * 10));
  //enemy health
  pts_total += get_health_points(entities[player_id].army_size);
  //enemy sta
  pts_total += (15 - (entities[player_id].stamina * 5));
  //nearby engaged
  pts_total += (entities[player_id].nearby_engaged * 15);
  return pts_total;
}

char get_distance_points(char distance)
{
  if(distance == 0)
  {
    return 30;
  }
  if(distance < 4) //move distance
  {
    return 15;
  }
  if(distance > 6) //outside move distance +2
  {
    return -30;
  }
  if(distance > 3) //outside move distance
  {
    return -15;
  }
}

char get_health_points(char num_of_units)
{
  if(num_of_units > 11)
  {
    return 0;
  }
  if(num_of_units > 7)
  {
    return 5;
  }
  if(num_of_units > 4)
  {
    return 10;
  }
  return 15;
}

ai()
{
  char i;
  for(i=0; i<num_of_ai; i++)
  {
    ai_timer = 20;
    ai_unit(ai_units[i]);
  }
  start_turn(PLAYER);
}

void ai_unit(char id)
{
  int traveled;
  char d, i;
  while(entities[id].actionable)
  {
    ai_timer--;

    if(ai_timer < 0)
    {
      determine_unit_action(id);
      // put_number(id,3,10,10);
      switch(action)
      {

        case PASS:
        ai_timer = 20;
        entities[id].actionable = 0;
        update_map();
        draw_actions();
        break;

        case MOVE: //move a unit
        d = get_path(entities[id].pos,entities[target_unit].pos,path,battle_grid,1);
        traveled = furthest_can_travel(d);
        move_unit(traveled,entities[id].pos);
        update_map();
        draw_actions();
        satb_update();
        ai_timer = 20;
        break;

        case ATTACK: //attack a unit
        attack_unit(entities[id].pos,entities[target_unit].pos);
        ai_timer = 20;
        entities[id].actionable = 0;
        break;
      }
    }
  }
}

void start_turn(char team)
{
  char i;
  actions = ACTIONS_PER_TURN;
  turn = team;
  s_y = 32;
  s_x = 32;
  ai_timer = 90;
  for(i=0; i<num_of_entities; i++)
  {
    if(team == entities[i].team)
    {
      entities[i].actionable = 1;
    }
  }
  init_ai();
  spr_set(0);
  spr_y(s_y);
  spr_x(s_x);
  draw_actions();
  satb_update();
  vsync();
}

char calc_nearest_opp(int selected)
{
  int i, j, opp_entities;
  char distance, shortest, shortest_opp;
  int opponent[18]; //18 is the max a team can have
  shortest = 127;
  opp_entities = 0;
  for(i=0; i<num_of_entities; i++)
  {
    if(entities[i].team == PLAYER)
    {
      opponent[opp_entities++] = i;
    }
  }
  for(i=0; i<opp_entities; i++)
  {
    distance = get_path(selected,entities[opponent[i]].pos,path,battle_grid,1);
    if(distance < shortest)
    {
      shortest = distance;
      shortest_opp = i;
    }
  }
  get_path(selected,entities[opponent[shortest_opp]].pos,path,battle_grid,1);
  return shortest;
}

int furthest_can_travel(char path_length)
{
  char max_moves;
  max_moves = 6 / 2;
  // put_number(path_length,3,5,10);
  if((path_length - max_moves)+1 > 0)
  {
    // put_number(path[(path_length - max_moves)+1],3,5,10);
    return path[(path_length - max_moves)+1];
  }
  else
  {
    // return path[max_moves - path_length];
    // put_number(path_length,3,5,10);
    return path[1];
  }
}

print_path(char path_length)
{
  int i;
  for(i=0; i<path_length; i++)
  {
    put_number(path[i],3,3,3+i);
  }
}
