#define TARGET_SELF 1
#define TARGET_ALLY 2
#define TARGET_ENEMY 3
#define TARGET_ENEMIES 4

#define TARGET_TILE_EMPTY 1
#define TARGET_TILE_SELF 2
#define TARGET_TILE_ALLY 3
#define TARGET_TILE_ENEMY 4

#define MAX_TACTIC_METER 50

char tactic_loop = 0;
char tactic_current = 0;
char tactic_caster = 0;
char tactic_target_count = 0;

char target_target_ids[16];

void set_tactic(char id, char tactic_id)
{
  tactic_caster = id;
  tactic_current = tactic_id;
}

char tactic_distance()
{
  if(tactic_current == TACTIC_SCORCH)
  {
    return 4;
  }
  else if(tactic_current == TACTIC_DASH)
  {
    return 4;
  }
  else if(tactic_current == TACTIC_LEAP)
  {
    return 3;
  }
  return 0;
}

char tactic_tile_option()
{
  if(tactic_current == TACTIC_SCORCH)
  {
    return TARGET_TILE_ENEMY;
  }
  else if(tactic_current == TACTIC_DASH)
  {
    return TARGET_TILE_EMPTY;
  }
  else if(tactic_current == TACTIC_LEAP)
  {
    return TARGET_TILE_EMPTY;
  }
  else if(tactic_current == TACTIC_RAGE)
  {
    return TARGET_TILE_SELF;
  }
  return 0;
}

char get_target_tile_options(char tile)
{
  if(tile == 0 && tactic_tile_option() != TARGET_TILE_EMPTY){ return 0;}
  switch(tactic_tile_option())
  {
    case TARGET_TILE_EMPTY:
    return is_empty_tile(tile);

    case TARGET_TILE_SELF:
    return is_self_tile(tile);

    case TARGET_TILE_ALLY:
    return is_ally_tile(tile);

    case TARGET_TILE_ENEMY:
    return is_enemy_tile(tile);
  }

  return 0;
}

char get_tactic_cost(char t_id)
{
  switch(t_id)
  {
    case TACTIC_DASH: return 20;
    case TACTIC_LEAP: return 20;
    case TACTIC_RAGE: return 20;
    case TACTIC_SCORCH: return 30;
    case TACTIC_NONE: return 0;
  }
}

char get_tactic_perform_status()
{
  switch(tactic_current)
  {
    case TACTIC_DASH: return 1;
    case TACTIC_LEAP: return 1;
    case TACTIC_RAGE: return 1;
    case TACTIC_SCORCH: return 0;
    case TACTIC_NONE: return 0;
  }
}


char is_self_tile(char tile)
{
  return tactic_caster == tile-1;
}

char is_enemy_tile(char tile)
{
  return entities[tactic_caster].team != entities[tile-1].team;
}

char is_ally_tile(char tile)
{
  return entities[tactic_caster].team == entities[tile-1].team;
}

char is_empty_tile(char tile)
{
  tile == 0;
}

char is_valid_tactic_tile()
{
  char tile;
  tile = battle_grid[g_abs];
  return get_target_tile_options(tile);
}

void determine_map_target(char target_team, char radius)
{
  char id;

  get_unit_radius(g_abs,radius,(target_team == PLAYER)? PLAYER : CPU,0);
  for(tactic_loop=0, tactic_target_count=0; tactic_loop<map_size+1; tactic_loop++)
  {
    id = battle_grid[map[tactic_loop].ownPos] - 1;
    if(id >= 0 && entities[id].team != entities[tactic_caster].team)
    {
      target_target_ids[tactic_target_count++] = id;
    }
  }
}

void get_dash_radius()
{
  map_size = 0;
  map[map_size++] = g_abs;
  get_left();
  get_right();
  get_up();
  get_down();
  if(map_size > 0)
  {
    map_size--;
  }
}

void get_left()
{
  int current;
  char i;
  current = g_abs;
  for(i=0; i<4; i++)
  {
    if((((current&0xF) - 1) > -1 ) && is_traversable(current-1))
    {
      map[map_size++] = --current;
    }
    else
    {
      return;
    }
  }
}

void get_right()
{
  int current;
  char i;
  current = g_abs;
  for(i=0; i<4; i++)
  {
    if(((current&15) + 1) < 16 && is_traversable(current+1))
    {
      map[map_size++] = ++current;
    }
    else
    {
      return;
    }
  }
}

void get_up()
{
  int current;
  char i;
  current = g_abs;
  for(i=0; i<4; i++)
  {
    if((current - 16) > -1 && is_traversable(current-16))
    {
      current -= 16;
      map[map_size++] = current;
    }
    else
    {
      return;
    }
  }
}

void get_down()
{
  int current;
  char i;
  current = g_abs;
  for(i=0; i<4; i++)
  {
    if((current + 16) < 352 && is_traversable(current+16))
    {
      current += 16;
      map[map_size++] = current;
    }
    else
    {
      return;
    }
  }
}

