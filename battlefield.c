// #include "battlefield.h"
#include "battle_ai.c"

#incpal(bluepal,"map/sprites/bluetiles.pcx")
#incpal(redpal,"map/sprites/redtiles.pcx")

const int player_start_positions[3] = {71,51,60};
const int cpu_start_positions[3] = {198,210,219};

int map_no;
//PRETTY SURE THESE TWO CAN GO BYE BYE NOW, ALL UNITS (COMMANDERS INCLUDED ARE A PART OF THE UNIT_LIST)
char player_cmdr_list[3];
char cpu_cmdr_list[3];
int id;
int s_x_holder, s_y_holder;
int walking;
enum Direction cmdr_direction;
int cmdr_x, cmdr_y;
char exit_battlefield;
int b_cycler = 0;
int b_framer = 0;
char t = 0;
// char no_of_cpu_cmdrs = 0;
// char no_of_player_cmdrs = 0;
char framer = 0;

void begin_explore(char map_id)
{
  no_of_player_cmdrs = 1;
  no_of_cpu_cmdrs = 0;
  yOffset = 0;
  b_map_id = map_id;
  map_no = battle_map_metadata[map_id].map_no;
  // init_npcs();
  clear_npcs();
  load_npcs(get_map_data_offset(map_no)+npc_level_data);
  init_battlefield();

  cmdr_direction = SOUTH;
  cmdr_x = 64;
  cmdr_y = 32;
  selector_mode = EXPLORE_MODE;

  if(battle_map_metadata[map_id].map_type == WILD)
  {
    randomize_enemy_npcs();
  }
  load_visible_units();
  battlefield_loop(map_id);
}

int get_map_data_offset(int level)
{
  int i, current_level;
  i = 0;
  current_level = 0;
  if(level == 0)
  {
    return 0;
  }
  for(;;)
  {
    if(npc_level_data[i++] == -1)
    {
      if(++current_level == level)
      {
        return i;
      }
    }
  }
}

void begin_battlefield(char map_id, char enemy_cmdrs[3], char no_of_cmdrs)
{
  b_map_id = map_id;
  map_no = castles[map_id].map_no;
  no_of_cpu_cmdrs = no_of_cmdrs;
  no_of_player_cmdrs = min(3,party_size);
  yOffset = 0;
  // init_npcs();
  clear_npcs();
  init_battlefield();
  selector_mode = 0;
  memcpy(cpu_cmdr_list,enemy_cmdrs,3);
  battle_start(no_of_player_cmdrs,no_of_cpu_cmdrs);//battle_map_metadata[b_map_id].no_of_npcs);
  battlefield_loop(map_id);
}

// void battlefield_loop(char *deploy_one, char s_one, char *deploy_two, char s_two, char start_mode)
void battlefield_loop(char map_id)
{
  char collide;
  exp_gained = 75;
  gold_gained = 75;
  display_sprites_();
  disp_on();
  satb_update();
  vsync();

  turn = PLAYER;
  spr_hide(0);
  while(exit_battlefield)
  {
    // put_number(unit_list[entities[2].id].id,3,7,6);
    // put_number(battle_grid[0].type,3,7,5);

    // put_number(npcs[0].pos_y,3,5,6);
    // put_number(npcs[0].type,3,5,7);
    // put_number(npcs[0].pal,3,5,8);
    // put_hex(npc_vram[0],6,12,4);
    // put_hex(unit_list[npcs[0]].address+(0x40*framer),6,5,5);
    put_number(cmdr_y,4,1,1);
    put_number(s_y,4,6,1);
    tick();
    if(turn == CPU)
    {
      ai();
    }
    else
    {
      if(selector_mode == EXPLORE_MODE)
      {
        collide = check_collision(cmdr_x, cmdr_y);
        explore();
        walk();

        if(collide != -1)
        {
          reset_npcs();
          startup_spawn_battle(collide);
        }
        // if(tic==0)
        // {
          // draw_npcs(5);
        // }
      }
      else if(selector_mode == EXPLORE_MENU_MODE)
      {
        explore_menu();
      }
      else if(selector_mode == DIALOG_MODE)
      {
        if(joytrg(0) & JOY_I)
        {
          get_next_dialog(0);
        }
      }
      else
      {
        if(tic==0)
        {
          // draw_npcs(10);
          // update_map();
        }
        display_army_size(1,2);
        display_stats(6,2);
        display_type(4,1);
        // display_id(4,1);
        ctrls();
        draw_selector();
        cycle_animations();
        check_battle_complete();
      }
    }
    satb_update();
    vsync();
  }
  // spr_hide(2);
  // put_string("over",5,5);
  cleanup_battlefield();
  return 0;
  //whoops?
}

void randomize_enemy_npcs()
{
  int x, y;
  char i, u_type;
  for(i=0; i<range(1,3); i++)
  {
    x = range(0,15);
    y = range(6,12);
    u_type = range(HOUND_UNIT,BANDIT_UNIT);
    if(x*16 != cmdr_x && y*16 != cmdr_y)
    {
      add_npc(x,y,u_type,UNIT_PALS[u_type]);
    }
    else
    {
      --i;
    }
  }
}

void load_visible_units()
{
  /*
  * Load commander sprite
  */
  load_vram(0x6A00,rei_walk,0x300);
  load_palette(16,rei_walk_pal,1);
  spr_make(2,cmdr_x,cmdr_y,0x6A00,FLIP_MAS|SIZE_MAS,NO_FLIP|SZ_16x32,0,1);//player sprite
  spr_pri(1);
  // display_spawn(current_global_units-1,enemy_positions[0],enemy_positions[1],1,1);
  /*
  * Load enemy spawns
  */
}

void explore_menu()
{
  int value;
  unsigned char joy1;
  joy1 = joytrg(0);

  switch(joy1)
  {
    case JOY_DOWN:
    cursor_down();
    break;

    case JOY_UP:
    cursor_up();
    break;

    case JOY_RIGHT:
    cursor_right();
    break;

    case JOY_LEFT:
    cursor_left();
    break;

    case JOY_I:
    if(menu_option == 5)
    {
      exit_battlefield = 0;
    }
    else if(menu_option == 4)
    {
      char npc_type = -1;
      switch(cmdr_direction)
      {
        case EAST:
        npc_type = check_collision(cmdr_x+16,cmdr_y);
        break;
        case NORTH:
        npc_type = check_collision(cmdr_x,cmdr_y-16);
        break;
        case SOUTH:
        npc_type = check_collision(cmdr_x,cmdr_y+16);
        break;
        case WEST:
        npc_type = check_collision(cmdr_x-16,cmdr_y);
        break;
      }
      if(npc_type != -1)
      {
        spr_hide(1);
        satb_update();
        selector_mode = DIALOG_MODE;
        get_next_dialog(npc_type);
      }
    }
    break;

    case JOY_II:
    spr_hide(1);
    selector_mode = EXPLORE_MODE;
    break;
  }
}

void explore()
{
  unsigned char joy1;
  joy1 = joy(0);

  if(walking == 0)
  {
    switch(joy1)
    {
      case JOY_DOWN:
      if(check_collision(cmdr_x,cmdr_y+16) < MAN_NPC && cmdr_y < 432)
      {
        walking = 16;
      }
      cmdr_direction = SOUTH;
      break;

      case JOY_UP:
      if(cmdr_y>32 && check_collision(cmdr_x,cmdr_y-16) < MAN_NPC)
      {
        walking = 16;
      }
      cmdr_direction = NORTH;
      break;

      case JOY_LEFT:
      if(check_collision(cmdr_x-16,cmdr_y) < MAN_NPC)
      {
        walking = 16;
      }
      cmdr_direction = WEST;
      break;

      case JOY_RIGHT:
      if(check_collision(cmdr_x+16,cmdr_y) < MAN_NPC)
      {
        walking = 16;
      }
      cmdr_direction = EAST;
      break;

      case JOY_I:
      // if(joytrg(0)&JOY_I)
      // {
      //   add_npc(128,64,REI,20);
      // }
      selector_mode = EXPLORE_MENU_MODE;
      display_menu();
      break;
    }
  }
}

void walk()
{
  b_cycler = (b_cycler+1)%8;
  if(b_cycler == 0)
  {
    b_framer = (b_framer+1)%2;
  }
  spr_set(2);

  switch(cmdr_direction)
  {
    case SOUTH:
    // spr_pattern(0x6A00 + (0x40*b_framer));
    spr_pattern(0x6A00);
    break;

    case NORTH:
    spr_pattern(0x6A00 + 0x100);
    // spr_pattern(0x6A00 + (0x40*b_framer) + 0x100);
    break;

    case EAST:
    // spr_pattern(0x6A00 + (0x40*b_framer) + 0x200);
    spr_pattern(0x6A00 + 0x200);
    spr_ctrl(FLIP_MAS|SIZE_MAS,NO_FLIP_X|SZ_16x32);
    break;

    case WEST:
    // spr_pattern(0x6A00 + (0x40*b_framer) + 0x200);
    spr_pattern(0x6A00 + 0x200);
    spr_ctrl(FLIP_MAS,FLIP_X|SZ_16x32);
    break;
  }
  if(walking > 0)
  {
    switch(cmdr_direction)
    {
      case SOUTH:
      spr_pattern(0x6A00 + (0x40*b_framer));
      cmdr_y++;
      if(cmdr_y>96 && cmdr_y<336){
        yOffset--;
        s_y++;
      }
      spr_y(cmdr_y+yOffset);
      walking--;
      break;

      case NORTH:
      spr_pattern(0x6A00 + (0x40*b_framer) + 0x100);
      cmdr_y--;
      if(cmdr_y > 96 && cmdr_y < 336)
      {
        yOffset++;
        s_y--;
      }
      spr_y(cmdr_y+yOffset);
      walking--;
      break;

      case EAST:
      spr_pattern(0x6A00 + (0x40*b_framer) + 0x200);
      spr_x(++cmdr_x);
      walking--;
      break;

      case WEST:
      spr_x(--cmdr_x);
      spr_pattern(0x6A00 + (0x40*b_framer) + 0x200);
      walking--;
      break;
    }
  }
  scroll(0,s_x,s_y+32,32, 223, 0xC0);
  draw_npcs(5);
}

void startup_spawn_battle(char u_type)
{
  char list[6];
  char j;
  for(j=0; j<range(3,6); j++)
  {
    list[j] = u_type;
    // list[j] = BANDIT_UNIT;
  }
  load_group(&party[0],party[0]-16,PLAYER,player_start_positions[0],15);
  load_entities(15,CPU,list,j,cpu_start_positions[0],-17);
  one_total = commanders[party[0]].no_of_units + 1;
  two_total = j;
  // reset_npcs();
  reset_satb();
  load_sprites_();
  draw_selector();
  update_map();
  // load_commanders_to_vram();
  // load_palette_groups();
  selector_mode = 0;
}

void battle_start()
{
  char i;
  selector_mode = 0;
  sx = 32;
  sy = 32;
  cmdr_x += 16;
  one_total = 0;
  two_total = 0;

  for(i=0; i<no_of_player_cmdrs; i++)
  {
    one_total += commanders[party[i]].no_of_units + 1;//units plus commander
  }
  for(i=0; i<no_of_cpu_cmdrs; i++)
  {
    two_total += commanders[castles[b_map_id].commanders[i]].no_of_units + 1;//commanders[enemy_cmdrs[0]].no_of_units + 1;
  }

  reset_satb();
  init_units();
  // load_commanders_to_vram();
  load_sprites_();
  draw_selector();
  update_map();
}

void check_battle_complete()
{
  if(one_total > 0 && two_total > 0)
  {
    return;
  }
  satb_update();
  vsync();
  //if the battle is over and we're not at a castle, we go back to exploring
  if(battle_map_metadata[b_map_id].map_type != CASTLE)
  {
    post_battle_dialog();
    cleanup_battlefield();
    clear_npcs();
    load_visible_units();
    randomize_enemy_npcs();
    //remove npc enemy group killed
    selector_mode = EXPLORE_MODE;
  }
  //becaue if the battle is over and we're at a castle, we just leave the battlefield
  else if(battle_map_metadata[b_map_id].map_type == CASTLE)
  {
    post_battle_dialog();
    cleanup_battlefield();
    exit_battlefield = 0;
  }
}
//this is awful. Since our sprites in the SATB don't have a reference to our
//entities, and vice versa, we don't know what sprite number our sprite is
//which entity. So we do a bunch of math on the sprite to figure out which
//pattern it has and if it's on the second or first cycle
void cycle_animations()
{
  char i;
  int pattern, p_start;
  p_start = 0x3800;
  if(t++ == 30)
  {
    t = 0;
    b_framer = (b_framer+1)%2;
    for(i=0; i<num_of_entities; i++)//this is awful
    {
      spr_set(63-i);
      if((spr_get_pattern() % 0x100))
      {
        spr_pattern((spr_get_pattern() - 0x40));
      }
      else
      {
        spr_pattern((spr_get_pattern() + 0x40));
      }
    }
  }
}

void init_units()
{
  char i;
  for(i=0; i<no_of_player_cmdrs; i++)
  {
    load_group(&party[i],party[i]-16,PLAYER,player_start_positions[i],15);
  }

  for(i=0; i<no_of_cpu_cmdrs; i++)
  {
    load_group(&cpu_cmdr_list[i],castles[b_map_id].commanders[i]-16,CPU,cpu_start_positions[i],-17);
  }
}

void load_group(char *cmdr_id, char id, char team, int start, int front)
{
  load_entities(1,team,cmdr_id,1,start,front);
  load_entities(MAX_ARMY_SIZE,team,commanders[id].units,commanders[id].no_of_units,start+front,front);
}

//should rename to initialize_entities
void load_entities(char size, char team, char *list, char list_length, int starting_location, int direction)
{
  char *u;
  char i, value;
  int location;

  for(i=0, u=list; i<list_length; i++, u++)
  {
    value = *u;
    location = (starting_location)+(i%3)+((i/3)*(direction+1));
    add_entity(0,size,team,UNIT_PALS[value],value,1,value,location);//simply changing this fucks up where the units come up on the map
    battle_grid[location] = num_of_entities;
  }
}

void load_ents()
{
  char i;
  clear_npcs();
  for(i=0; i<num_of_entities; i++)
  {
    add_npc(entities[i].pos%16,entities[i].pos/16,unit_list[entities[i].id].id,UNIT_PALS[entities[i].id]);
  }
}

void init_battlefield()
{
  g=0;
  unit_selected = 0;
  selector_mode = 0;
  num_of_entities = 0;
  sx = 80;
  sy = 48;
  menu_x = -64;
  menu_y = -32;
  cursor_x = -16;
  cursor_y = -16;
  actions = ACTIONS_PER_TURN;
  menu_option = 0;
  exit_battlefield = 1;
  s_x_holder = s_x;
  s_y_holder = s_y;
  walking = 0;
  // no_of_npcs = 0;

  spr_set(0);
  spr_y(-16);
  load_vram(0x68C0,selector,0x40);
  load_vram(0x6900,cursor,0x40);

  load_battlefield_map();
}

void load_battlefield_map()
{
  s_x = 0;
  s_y = 0;

  scroll(1,0,0,0,32,0xC0);
  scroll(0,0,32,32,223,0xC0);

  load_palette(0,overworldpal,7);
  load_palette(10,fontpal,2);
  set_font_pal(10);
	load_font(font,125,0x2F00);

  load_palette(8,borderspal,1);
  load_palette(11,bluepal,1);
  load_palette(12,redpal,1);

  set_screen_size(SCR_SIZE_32x64);
  screen_dimensions = 32;

  load_vram(0x6400,borders,0x100);
  load_vram(0x30A0,icons_gfx,0x20);

  set_map_data(battlefieldbat+(224*map_no),16,14);

  load_tile(TILE_VRAM);
  load_map(0,2,0,0,16,14);

  display_info_panel(0,0,18,4);
  display_info_panel(0,18,14,4);

  put_string("ATK",20,1);
  put_string("MOV",20,2);
  put_string("ART",24,1);
  put_string("End",24,2);
  put_string("TLK",28,1);
  put_string("EXT",28,2);
}

void load_sprites_() //load default sprites
{
  // load_palette(29,selectorpal,1);
  // load_palette(30,bf_menu_pal,1);
  // load_palette(31,actionpal,1);

  load_palette(11,bluepal,1);
  load_palette(12,redpal,1);

  load_vram(0x68C0,selector,0x40);
  load_vram(0x6900,cursor,0x40);

  display_sprites_();
  // load_palette_groups();
  // load_units(0x3800);
}

void display_sprites_() //info sprites
{
  spr_make(0,sx,sy,0x68C0,0,NO_FLIP|SZ_16x16,29,1);
  spr_make(1,cursor_x,cursor_y,0x6900,FLIP_MAS|SIZE_MAS,NO_FLIP|SZ_16x16,29,1);
}

void update_map()
{
  int squares, i, sprite_counter, entity_no, p, z;
  char s;
  z=0;
  squares = MAP_SIZE;
  sprite_counter = 63;

  for(i = 0; i < squares; i++)
  {
    if(battle_grid[i] != 0)
    {
      entity_no = battle_grid[i]-1;
      draw_npc(sprite_counter--,(i%16)*16,((i/16)*16)-16,entity_no);
      // spr_make(sprite_counter--
      //         ,abs_x_from_graph(i)
      //         ,abs_y_from_graph(i)-16
      //         // ,entities[entity_no].unit->address
      //         ,npc_vram[entities[entity_no].id]
      //         ,FLIP_MAS|SIZE_MAS
      //         ,SZ_16x32
      //         ,entities[entity_no].unit->p_group+(entities[entity_no].team-1)
      //         ,1);
    }
  }
}

void cursor_left()
{
  if(menu_option == 0 || menu_option == 1)
  {
    return;
  }
  cursor_x -= 32;
  menu_option -= 2;
  spr_set(1);
  spr_x(cursor_x);
}

void cursor_right()
{
  if(menu_option == 4 || menu_option == 5)
  {
    return;
  }
  cursor_x += 32;
  menu_option += 2;
  spr_set(1);
  spr_x(cursor_x);
}

void cursor_down()
{
  if(menu_option == 1 || menu_option == 3 || menu_option == 5)
  {
    return;
  }
  cursor_y += 8;
  menu_option += 1;
  spr_set(1);
  spr_y(cursor_y);
}

void cursor_up()
{
  if(menu_option == 0 || menu_option == 2 || menu_option == 4)
  {
    return;
  }
  cursor_y -= 8;
  menu_option -= 1;
  spr_set(1);
  spr_y(cursor_y);
}

void ctrls()
{
  unsigned char j;
  int abs;
  j = joytrg(0);

  if(j & JOY_UP)
  {
    if(selector_mode == SELECT_MODE || selector_mode == PLACE_MODE || selector_mode == SPLIT_MODE || selector_mode == ATTACK_MODE || selector_mode == MERGE_MODE)
    {
      selector_up();
    }
    else if(selector_mode == MENU_MODE)
    {
      cursor_up();
    }
  }

  if(j & JOY_DOWN)
  {
    if(selector_mode == SELECT_MODE || selector_mode == PLACE_MODE || selector_mode == SPLIT_MODE || selector_mode == ATTACK_MODE || selector_mode == MERGE_MODE)
    {
      selector_down();
    }
    else if(selector_mode == MENU_MODE)
    {
      cursor_down();
    }
  }

  if(j & JOY_RIGHT)
  {
    if(selector_mode == SELECT_MODE || selector_mode == PLACE_MODE || selector_mode == SPLIT_MODE || selector_mode == ATTACK_MODE || selector_mode == MERGE_MODE)
    {
      selector_right();
    }
    else if(selector_mode == MENU_MODE)
    {
      cursor_right();
    }
  }

  if(j & JOY_LEFT)
  {
    if(selector_mode == SELECT_MODE || selector_mode == PLACE_MODE || selector_mode == SPLIT_MODE || selector_mode == ATTACK_MODE || selector_mode == MERGE_MODE)
    {
      selector_left();
    }
    else if(selector_mode == MENU_MODE)
    {
      cursor_left();
    }
  }

  abs = graph_from_x_y(sx,sy);

  if(j & JOY_I)
  {
    if(selector_mode == SELECT_MODE)
    {
      if(battle_grid[abs] != 0)
      {
          unit_selected = abs;
          id = battle_grid[abs];
          selector_mode = MENU_MODE;
          spr_set(0);
          spr_hide();
          display_menu();
      }
    }
    else if(selector_mode == PLACE_MODE)
    {
      if(battle_grid[abs] == 0 && (is_traversable(abs) || entities[battle_grid[unit_selected]-1].unit->ign))
      {
        move_unit(abs,unit_selected);
      }
    }
    else if(selector_mode == ATTACK_MODE)
    {
      if(battle_grid[abs] != 0)
      {
        attack_unit(abs,unit_selected);
      }
    }
    else if(selector_mode == MENU_MODE)
    {
      switch(menu_option)
      {
        case MENU_ATTACK:
        if(check_action_cost(ATTACK_COST))
        {
          selector_mode = ATTACK_MODE;
          hide_menu();
          pattern(unit_selected,0xC000,coords,entities[battle_grid[unit_selected]-1].unit->ign);
          // display_selector();
        }
        break;

        case MENU_MOVE:
        if(check_action_cost(1))
        {
          hide_menu();
          select_unit(unit_selected);
          // hide_menu();
        }
        else
        {
          display_error_message("insufficient actions");
        }
        break;

        case MENU_MERGE:
        if(check_action_cost(MERGE_COST))
        {
          selector_mode = MERGE_MODE;
          menu_option = MENU_ATTACK;
          // highlight_near_squares(unit_selected,0x1000,2,0,2,0);
          hide_menu();
          display_selector();
        }
        else
        {
            display_error_message("not enough actions");
        }
        break;

        case 3:
        selector_mode = SELECT_MODE;
        menu_option = MENU_ATTACK;
        hide_menu();
        // highlight_near_squares(unit_selected,0,2,0,2,0);
        // hide_menu();
        display_selector();
        start_turn(CPU);
        break;
      }
    }
  }

  if(j & JOY_II)
  {
    if(selector_mode == PLACE_MODE)
    {
      set_cursor_pos(unit_selected);
      highlight(unit_selected,0,(*entities[battle_grid[unit_selected]-1].unit).mov);
      selector_mode = MENU_MODE;
      menu_option = MENU_ATTACK;
      display_menu();
      display_selector();
      // hide_menu();
      unhighlight();
    }
    else if(selector_mode == MENU_MODE)
    {
      hide_menu();
      // highlight_near_squares(unit_selected,0,2,0,2,0);
      selector_mode = SELECT_MODE;
      display_selector();
    }
    else if(selector_mode == SPLIT_MODE || selector_mode == ATTACK_MODE || selector_mode == MERGE_MODE)
    {
      set_cursor_pos(unit_selected);
      pattern(unit_selected,0,coords,entities[battle_grid[unit_selected]-1].unit->ign);
      selector_mode = MENU_MODE;
      display_menu();
      display_selector();
    }
    if(selector_mode == OBSERVE_MODE)
    {
      highlight(abs,0,3);
      pattern(abs,0,coords,entities[battle_grid[unit_selected]-1].unit->ign);
      selector_mode = SELECT_MODE;
    }
  }

  if(j & JOY_RUN)
  {
    if(selector_mode == SELECT_MODE)
    {
      if(battle_grid[abs] != 0)
      {
        if(entities[battle_grid[abs]-1].team == CPU)
        {
          pattern(abs,0xC000,coords,entities[battle_grid[abs]-1].unit->ign);
          selector_mode = OBSERVE_MODE;
        }
      }
    }
    // actions = ACTIONS_PER_TURN;
    // draw_actions();
  }

  if(j & JOY_SEL)
  {
    get_random_item_by_level(0);
    get_random_item_by_level(0);
    get_random_item_by_level(0);
    get_random_item_by_level(0);

    post_battle_dialog();
    // put_number(drop_count,3,1,1);
  }
}

void get_next_dialog(char n_type)
{
  story(2,EXPLORE,n_type);

  load_battlefield_map();
  selector_mode = EXPLORE_MODE;
}

void selector_left()
{
  if(sx > 0)
  {
    if(selector_mode == PLACE_MODE)
    {
      if(valid_map_square(graph_from_x_y(sx-16,sy)))
      {
        calc_move_cost(unit_selected,graph_from_x_y(sx-16,sy));
        update_selector_pos(-16,0);
      }
    }
    else if(selector_mode == SPLIT_MODE || selector_mode == MERGE_MODE)
    {
      if(valid_distance(sx-16,sy+0,unit_selected,2) && is_traversable(graph_from_x_y(sx-16,sy)))
      {
        update_selector_pos(-16,0);
      }
    }
    else
    {
      update_selector_pos(-16,0);
    }
  }
}

void selector_right()
{
  if(sx < 256 - 16)
  {
    if(selector_mode == PLACE_MODE)
    {
      if(valid_map_square(graph_from_x_y(sx+16,sy)))
      {
        calc_move_cost(unit_selected,graph_from_x_y(sx+16,sy));
        update_selector_pos(16,0);
      }
    }
    else if(selector_mode == SPLIT_MODE || selector_mode == MERGE_MODE)
    {
      if(valid_distance(sx+16,sy+0,unit_selected,2) && is_traversable(graph_from_x_y(sx+16,sy)))
      {
        update_selector_pos(16,0);
      }
    }
    else
    {
      update_selector_pos(16,0);
    }
  }
}

void selector_down()
{
  if(sy < 256)
  {
    if(selector_mode == PLACE_MODE)
    {
      if(valid_map_square(graph_from_x_y(sx,sy+16)))
      {
        calc_move_cost(unit_selected,graph_from_x_y(sx,sy+16));
        update_selector_pos(0,16);
      }
    }
    else if(selector_mode == SPLIT_MODE || selector_mode == MERGE_MODE)
    {
      if(valid_distance(sx+0,sy+16,unit_selected,2) && is_traversable(graph_from_x_y(sx,sy+16)))
      {
        update_selector_pos(0,16);
      }
    }
    else
    {
      // s_y += 8;
      spr_set(0);
      update_selector_pos(0,16);
      // scroll(0,s_x,s_y+32,32, 223, 0xC0);
    }
  }
}

void selector_up()
{
  if(sy > 0)
  {
    if(selector_mode == PLACE_MODE)
    {
      if(valid_map_square(graph_from_x_y(sx,sy-16)))
      {
        calc_move_cost(unit_selected,graph_from_x_y(sx,sy-16));
        update_selector_pos(0,-16);
      }
    }
    else if(selector_mode == SPLIT_MODE || selector_mode == MERGE_MODE)
    {
      if(valid_distance(sx+0,sy-16,unit_selected,2) && is_traversable(graph_from_x_y(sx,sy-16)))
      {
        update_selector_pos(0,-16);
      }
    }
    else
    {
      update_selector_pos(0,-16);
    }
  }
}

char valid_map_square(int desired)
{
  char i;
  for(i=0; i<map_size+1; i++)
  {
    if((map[i].ownY*16)+map[i].ownX == desired) return 1;
  }
  return 0;
}

char valid_distance(int d_x, int d_y, int pos, int dist)
{
    int total;
    total = (abs(abs_x_from_graph(pos)/16 - (d_x/16))) + (abs((abs_y_from_graph(pos))/16 - (d_y/16)));
    if(total <= dist)
    {
      return 1;
    }
    return 0;
}

void set_cursor_pos(int pos)
{
  sx = abs_x_from_graph(pos);
  sy = abs_y_from_graph(pos);
  spr_set(0);
  spr_x(sx);
  spr_y(sy);
}

int abs_x_from_graph(int pos){ return graph_x_from_abs(pos)*16; }

int abs_y_from_graph(int pos){ return (graph_y_from_abs(pos)*16); }

int graph_x_from_abs(int pos){ return (pos % BF_X_TILES); }

int graph_y_from_abs(int pos){ return (pos / TILE_SIZE); }

int graph_from_x_y(int x, int y){ return (x/TILE_SIZE)+((y/TILE_SIZE)*TILE_SIZE); }

void display_menu()
{
  menu_option = 0;
  cursor_x = 148;
  cursor_y = 4;

  spr_make(1,cursor_x,cursor_y,0x6900,FLIP_MAS|SIZE_MAS,NO_FLIP|SZ_16x16,20,1);
}

void highlight(int square, int pal, int depth)
{
  int i, p;
  p = get_path(square,999,path,battle_grid,entities[battle_grid[square]-1].team,depth,entities[battle_grid[square]-1].unit->ign);
  for(i=0; i<map_size+1; i++)
  {
      change_background_palette((map[i].ownY*16)+map[i].ownX,pal,(224*map_no));
  }
}

void get_unit_radius(int square, int distance, char team, char ignore){ get_path(square,999,path,battle_grid,team,distance,ignore); }

void pattern(int square, int pal, int coords[18], char ignore_terrain)
{
  int pos;
  char j, id;
  id = battle_grid[square]-1;
  // print_unit_type((*entities[id].unit).unit_type,20,6);
  load_coords(id);
  for(j=0; j<get_pattern_length(id); j++)
  {
    pos = square+coords[j*2]+coords[(j*2)+1];
    if(is_traversable(pos) || ignore_terrain && pos < 224)
    {
      change_background_palette(pos,pal,(224*map_no));
    }
  }
}

char attackable(int attacker, int target, int coords[18])
{
  int pos;
  char j;
  load_coords(attacker);
  for(j=0; j<get_pattern_length(attacker); j++)
  {
    pos = entities[attacker].pos+coords[j*2]+coords[(j*2)+1];
    // if(is_traversable(pos)) why does it matter if the pos we're trying to attack is traversable?
    {
      if(pos == entities[target].pos)
      {
        return 1;
      }
      // change_background_palette(pos,pal);
    }
  }
  return 0;
}

char square_adjacent(int origin, int desired)
{
  int absolute;
  absolute = abs(origin - desired);

  if(absolute == 16) { return 1; }
  if(absolute == 1)
  {
    if(origin > desired) //moving left
    {
      if(desired%16 != 15)
      {
        return 1;
      }
    }
    else //moving right
    {
      if(desired%16 != 0)
      {
        return 1;
      }
    }
  }
    return 0;
}

void display_selector_pos()
{
  put_string("x:",1,1);
  put_number(sx,3,3,1);
  put_string("y:",1,2);
  put_number(sy,3,3,2);
}

void display_actions()
{
  put_string("actions:",10,1);
  put_number(actions,2,18,1);
}

void display_cost()
{
  put_string("cost:",10,2);
  put_number(cost,2,15,2);
  // actions -= (cost*2);
}

void display_abs(int x, int y)
{
  put_string("abs:",5,1);
  put_number(graph_from_x_y(sx,sy),3,x,y);
}

void display_id(int x, int y)
{
  char id;
  id = battle_grid[graph_from_x_y(sx,sy)];
  put_string("id",x,y);
  if(id)
  {
    put_number(entities[id-1].id,3,x+3,y);
  }
  else
  {
    put_number(0,3,x+3,y);
  }
}

void display_army_size(int x, int y)
{
  char id;
  id = battle_grid[graph_from_x_y(sx,sy)];
  put_string("HP",x,y);
  if(id)
  {
    put_number(entities[id-1].army_size,2,x+2,y);
  }
  else
  {
    put_string("    ",x,y);
  }
}

void display_team(int x, int y)
{
  char id;
  id = battle_grid[graph_from_x_y(sx,sy)];
  put_string("team:",x,y);
  if(id)
  {
    put_number(entities[id-1].team,2,x+5,y);
  }
  else
  {
    put_number(0,2,x+5,y);
  }
}

void display_stats(int x, int y)
{
  char id;
  id = battle_grid[graph_from_x_y(sx,sy)];
  if(id)
  {
    put_string(":",x,y);
    put_number(entities[id-1].unit->atk,2,x+1,y);
    put_string(";",x+4,y);
    put_number(entities[id-1].unit->def,2,x+5,y);
    // change_palette(0,0);
  }
  else
  {
    put_string("         ",x,y);
  }
}

void display_type(int x, int y)
{
  char id;
  id = battle_grid[graph_from_x_y(sx,sy)];
  if(id)
  {
    if(entities[id-1].unit->unit_type == COMMANDER)
    {
      put_string(commanders[entities[id-1].unit->id - 16].name,x,y);
    }
    else
    {
      print_unit_type((*entities[id-1].unit).unit_type,x,y);
      put_string("    ",x+3,y);
    }
  }
  else
  {
    put_string("       ",x,y);
  }
}

char begin_battle(int attacker, int target, int attacker_pos, int target_pos)
{
  int i, battle_result, attacker_before, attacker_after, target_before, target_after, resolution;
  // deduct_actions(ATTACK_COST);
  satb_update();
  disp_off();
  vsync();
  for(i=0; i<64; i++)
  {
    spr_hide(i);
  }
  cls();
  satb_update();
  reset_satb();
  vsync();

  attacker_before = entities[attacker].army_size;
  target_before = entities[target].army_size;

  resolution = battle_loop(attacker,target,attackable(target,attacker,coords));

  // load_commanders_to_vram();
  load_ents();
  attacker_after = entities[attacker].army_size;
  target_after = entities[target].army_size;

  // commanders[entities[attacker].id].army_size -= (attacker_before - attacker_after);
  // commanders[entities[target].id].army_size -= (target_before - target_after);
  // entities[attacker].army_size -= (attacker_before - attacker_after);
  // entities[target].army_size -= (target_before - target_after);
  battle_result = -1;
  load_sprites_();

  switch(resolution)
  {
    case 0:
    if(destroy_entity(attacker))
    {
      battle_result = attacker;
    }
    break;

    case 1:
    if(destroy_entity(target))
    {
      battle_result = target;
    }
    break;

    case 2: //nothing
    break;

    default:
    break;
  }

  load_battlefield_map();
  load_sprites_();
  // load_units(0x3800);

  update_map();
  disp_on();
  selector_mode = 0;
  // disp_on();
  return battle_result;
}

void cleanup_battlefield()
{
  int i; //exp_per=0;

  for(i=0; i<MAP_SIZE; i++)
  {
    battle_grid[i] = 0;
  }
  for(i=0; i<MAX_ENTITIES; i++)
  {
    entities[i].army_size = 0;
    entities[i].team = 0;
    entities[i].id = 0;
    entities[i].unit = 0;
    entities[i].pos = 0;
    entities[i].team = 0;
    entities[i].hp = 0;
  }
  num_of_entities = 0;
  for(i=0; i<64; i++)
  {
    spr_hide(i);
  }
  satb_update();
  one_total = 0;
  two_total = 0;

  s_x = s_x_holder;
  s_y = s_y_holder;
  clear_battle_items();
  reset_satb();
}
