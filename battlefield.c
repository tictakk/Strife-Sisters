// #include "battlefield.h"
#include "battle_ai.c"

#incpal(bluepal,"map/sprites/bluetiles.pcx")
#incpal(redpal,"map/sprites/redtiles.pcx")

#define MENU_MOVE_X 148
#define MENU_MOVE_Y 4
#define MENU_ATK_X 148
#define MENU_ATK_Y 12

// const int player_start_positions[6] = {87,67,75};
// const int cpu_start_positions[6] = {203,197,393};
const int army_positions[4] = {-16, -1, 1, 16};

int map_no;
int id;
int s_x_holder, s_y_holder,y_off_holder;
int walking;
enum Direction cmdr_direction;
int cmdr_x, cmdr_y;
char exit_battlefield;
int b_cycler = 0;
int b_framer = 0;
char t = 0;
char framer = 0;

void begin_explore(char map_id)
{
  no_of_player_cmdrs = 1;
  yOffset = 0;
  // map_no = battle_map_metadata.map_no;
  // init_npcs();
  clear_npcs();
  load_npcs(get_map_data_offset(map_no)+npc_level_data);
  init_battlefield();

  cmdr_direction = SOUTH;
  cmdr_x = 64;
  cmdr_y = 32;
  selector_mode = EXPLORE_MODE;

  // if(battle_map_metadata[map_id].map_type == WILD)
  // {
  //   randomize_enemy_npcs();
  // }
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

void begin_battlefield(char map_id)
{
  char i;
  map_no = map_id;
  if(map_id == 1)
  {
    add_commander_to_party(KING);
  }
  for(i=0; i<6; i++)
  {
    if(battle_map_metadata.player_start_pos[i])
    {
      no_of_player_cmdrs++;
    }
  }
  // no_of_player_cmdrs = min(6,party_size);
  yOffset = 0;
  // clear_npcs();
  init_battlefield();
  selector_mode = 0;

  battle_start();//battle_map_metadata[b_map_id].no_of_npcs);
  battlefield_loop(map_id);
}

void battlefield_loop(char map_id)
{
  char id;
  char collide;
  exp_gained = 75;
  gold_gained = 75;
  last_command = SELECT_MODE;
  objective_pos = 200;
  // ai_objective = battle_map_metadata.map_obj;
  // ai_objective = CAPTURE;
  // ai_objective = DEFEND;
  load_coords(0);
  display_sprites_();
  disp_on();
  satb_update();
  vsync();

  turn = PLAYER;
  while(exit_battlefield)
  {
    // put_hex(npc_vram[AXE_UNIT],5,0,0);
    if(turn == CPU)
    {
      ai();
    }
    else
    {
      id = battle_grid[graph_from_x_y(sx,sy)];
      if(id)
      {
        display_id(id-1,0,0);
        id--;
        display_army_size(id,1,2);
        display_stats(id,6,2);
        display_type(id,4,1);
      }
      else
      {
        put_string("  ",0,0);
        put_string("   ",3,0);
        put_string("   ",3,1);
        put_string("   ",3,2);

        put_string("   ",7,0);
        put_string("   ",7,1);
        put_string("   ",7,2);

        put_string("   ",11,0);
        put_string("   ",11,1);
        put_string("   ",11,2);
      }
      display_position(14,1);

      ctrls();
      cycle_animations();
      check_battle_complete();
    }
    satb_update();
    vsync();
  }

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
    u_type = range(HOUND_UNIT,AXE_UNIT);
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
      selector_mode = EXPLORE_MENU_MODE;
      display_menu(148,4);
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
  scroll(0,s_x,s_y+32,32,224,0xC0);
  draw_npcs(5);
}

void startup_spawn_battle(char u_type)
{
  char list[6];
  char j;
  for(j=0; j<3; j++)//j<range(3,6); j++)
  {
    // list[j] = u_type;
    list[j] = BLOB_UNIT;
  }
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
  one_total = no_of_player_cmdrs;
  two_total = cpu_cmdr_count;

  // for(i=0; i<no_of_player_cmdrs; i++)
  // {
  //   one_total += commanders[party[i]].no_of_units + 1;//units plus commander
  // }
  // for(i=0; i<cpu_cmdr_count; i++)
  // {
  //   two_total += commanders[battle_map_metadata.cpu_commander_ids].no_of_units + 1;//commanders[enemy_cmdrs[0]].no_of_units + 1;
  // }

  reset_satb();
  init_units();
  draw_selector();
  update_map();
  // story(b_map_id,INBATTLE,0);
  // load_battlefield_map();
  // init_battlefield();

  load_sprites_();
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
  // if(battle_map_metadata[b_map_id].map_type != CASTLE)
  // {
    // post_battle_dialog();
    // cleanup_battlefield();
    // reset_npcs();
    // load_visible_units();
    // randomize_enemy_npcs();
    // load_map(0,2,0,0,16,29);
    // spr_set(2);
    // spr_x(cmdr_x);
    // spr_y(cmdr_y+yOffset);
    // //remove npc enemy group killed
    // selector_mode = EXPLORE_MODE;
  // }
  //becaue if the battle is over and we're at a castle, we just leave the battlefield
  // else if(battle_map_metadata[b_map_id].map_type == CASTLE)
  // {
    post_battle_dialog();
    exit_battlefield = 0;
  // }
}
//this is awful. Since our sprites in the SATB don't have a reference to our
//entities, and vice versa, we don't know what sprite number our sprite is
//which entity. So we do a bunch of math on the sprite to figure out which
//pattern it has and if it's on the second or first cycle
void cycle_animations()
{
  // draw_npcs(5);
  char i;

  if(t++ == 10)
  {
    // t = 0;
    increment_frame();
    update_map();
  }
}

void init_units()
{
  char i;
  commanders[23].row_counts[0] = 0;
  commanders[23].row_counts[1] = 0;
  commanders[23].row_counts[2] = 0;

  commanders[24].row_counts[0] = 0;
  commanders[24].row_counts[1] = 0;
  commanders[24].row_counts[2] = 0;

  commanders[25].row_counts[0] = 0;
  commanders[25].row_counts[1] = 0;
  commanders[25].row_counts[2] = 0;

  // load_units_by_cmdr_id(24);
  // load_units_by_cmdr_id(23);
  // load_units_by_cmdr_id(25);

  for(i=0; i<no_of_player_cmdrs; i++)
  {
    load_group(party[i],party[i]-16,PLAYER,battle_map_metadata.player_start_pos+i);//commander + max_army_size
  }

  for(i=0; i<cpu_cmdr_count; i++)
  {
    load_units_by_cmdr_id((char)battle_map_metadata.cpu_commander_ids[i],20+i);
    load_group((char)battle_map_metadata.cpu_commander_ids[i]+16+i,
                20+i,
                CPU,battle_map_metadata.cpu_start_pos+i);
  }
}

void load_group(char cmdr_id, char id, char team, int positions[7])
{
  add_entity(team,UNIT_PALS[commanders[id].sprite_type],cmdr_id,id,positions[0]);
  battle_grid[positions[0]] = num_of_entities;
}

void load_ents()
{
  char i;
  reset_npcs();
  for(i=0; i<num_of_entities; i++)
  {
    add_npc(entities[i].pos%16,entities[i].pos/16,
            commanders[entities[i].id].sprite_type,
            UNIT_PALS[commanders[entities[i].id].sprite_type]);
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
  s_x = 0;
  s_y = 0;
  walking = 0;
  // no_of_npcs = 0;

  spr_set(SELECTOR);
  spr_y(-16);
  load_vram(0x68C0,selector,0x40);
  load_vram(0x6900,cursor,0x40);

  load_battlefield_map();
}

void load_battlefield_map()
{
  set_screen_size(SCR_SIZE_32x64);

  scroll(1,0,0,0,32,0x80);
  scroll(0,s_x,s_y+32,32,224,0xC0);

  load_palette(0,overworldpal,7);
  load_palette(10,fontpal,2);
  set_font_pal(10);
	load_font(font,125,0x4900);
  load_vram(0x4AA0,icons_gfx,0x60);
	load_vram(0x4CB0,icons_gfx+0x60,0x60);

  load_palette(8,borderspal,1);
  load_palette(12,bluepal,1);
  load_palette(13,redpal,1);

  // set_screen_size(SCR_SIZE_32x64);
  screen_dimensions = 32;

  load_vram(0x6400,borders,0x100);
  load_vram(0x30A0,icons_gfx,0x60);

  set_map_data(battlefieldbat+(464*map_no),16,29);
  set_tile_data(overworldtiles,147,overworldtilespal,TILE_WIDTH);

  load_tile(TILE_VRAM);
  load_map(0,2,0,0,16,29);

  display_abs_info_panel(0,0,18,4);
  display_abs_info_panel(18,0,14,4);

  print_menu();
}

void load_sprites_() //load default sprites
{
  // load_palette(29,selectorpal,1);
  // load_palette(30,bf_menu_pal,1);
  // load_palette(31,actionpal,1);

  load_palette(12,bluepal,1);
  load_palette(13,redpal,1);

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
  int squares, i, sprite_counter, entity_no, p;
  char s;

  t=0;
  squares = 224+s_y;//464;
  sprite_counter = 63;

  for(i=s_y+32; i<squares; i++)
  {
    if(battle_grid[i] != 0)
    {
      entity_no = battle_grid[i]-1;

      draw_npc(sprite_counter--,(i&15)*16,(((i/16)*16)-16),entity_no);
      if(entities[entity_no].actionable == 0)
      {
        spr_pal(31);
      }
    }
  }
  for(i=5; i<sprite_counter+1; i++)
  {
    spr_hide(i);
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
  spr_set(CURSOR);
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
  spr_set(CURSOR);
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
  spr_set(CURSOR);
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
  spr_set(CURSOR);
  spr_y(cursor_y);
}

void ctrls()
{
  unsigned char j;
  int abs;
  j = joytrg(0);

  if(j & JOY_UP)
  {
    if(selector_mode == SELECT_MODE || selector_mode == PLACE_MODE || selector_mode == ATTACK_MODE)
    {
      selector_up();
    }
    else if(selector_mode == MENU_MODE || selector_mode == ACTION_MODE)
    {
      cursor_up();
    }
  }

  if(j & JOY_DOWN)
  {
    if(selector_mode == SELECT_MODE || selector_mode == PLACE_MODE || selector_mode == ATTACK_MODE)
    {
      selector_down();
    }
    else if(selector_mode == MENU_MODE || selector_mode == ACTION_MODE)
    {
      cursor_down();
    }
  }

  if(j & JOY_RIGHT)
  {
    if(selector_mode == SELECT_MODE || selector_mode == PLACE_MODE || selector_mode == ATTACK_MODE)
    {
      selector_right();
    }
    else if(selector_mode == MENU_MODE || selector_mode == ACTION_MODE)
    {
      cursor_right();
    }
  }

  if(j & JOY_LEFT)
  {
    if(selector_mode == SELECT_MODE || selector_mode == PLACE_MODE || selector_mode == ATTACK_MODE)
    {
      selector_left();
    }
    else if(selector_mode == MENU_MODE || selector_mode == ACTION_MODE)
    {
      cursor_left();
    }
  }

  abs = graph_from_x_y(sx,sy);

  if(j & JOY_I)
  {
    // char i, size;
    // g=0;
    // size = get_path(abs,216,path,battle_grid,CPU,60,0);
    // for(i=0;i<size+1;i++)
    // {
    //   put_number(path[i],4,10,5+(g++));
    // }
    // return;
    // get_unit_radius(abs,3,CPU,0);
    // put_number(find_nearest_unoccupied_position(abs,objective_pos,map_size,map),4,10,5);
    // find_nearest_unoccupied_position(abs,objective_pos,map_size,map);
    if(selector_mode == SELECT_MODE)
    {
      if(battle_grid[abs] != 0)
      {
        last_command = selector_mode;
        unit_selected = abs;
        id = battle_grid[abs];
        selector_mode = MENU_MODE;
        // hide_selector();
        // spr_set(SELECTOR);
        // spr_hide();
        display_menu(148,4);
      }
    }
    else if(selector_mode == PLACE_MODE)
    {
      if(battle_grid[abs] == 0 && (is_traversable(abs) || 0))//entities[battle_grid[unit_selected]-1].unit->ign))
      {
        last_command = selector_mode;
        move_unit(abs,unit_selected);
        selector_mode = ACTION_MODE;
        // hide_selector();
        display_menu(MENU_ATK_X,MENU_ATK_Y);
        draw_selector();
        show_selectable_menu_options();
        unhighlight();
      }
    }
    else if(selector_mode == ATTACK_MODE)
    {
      if(battle_grid[abs] != 0)
      {
        // put_number(abs,3,15,15);
        // put_number(unit_selected,3,15,16);
        attack_unit(abs,unit_selected);
      }
    }
    else if(selector_mode == MENU_MODE || selector_mode == ACTION_MODE)
    {
      last_command = selector_mode;
      switch(menu_option)
      {
        case MENU_ATTACK:
        if(check_action_cost(ATTACK_COST))
        {
          selector_mode = ATTACK_MODE;
          hide_menu();
          draw_selector();
          // pattern(unit_selected,0xD000,coords,entities[battle_grid[unit_selected]-1].unit->ign);
          pattern(unit_selected,0xD000,coords,0);
        }
        break;

        case MENU_MOVE:
        if(check_action_cost(1))
        {
          hide_menu();
          select_unit(unit_selected);
        }
        break;

        case MENU_END:
        selector_mode = SELECT_MODE;
        menu_option = MENU_ATTACK;
        entities[battle_grid[unit_selected]-1].actionable = 0;

        print_menu();
        hide_cursor();
        break;

        case MENU_TURN:
        selector_mode = SELECT_MODE;
        menu_option = MENU_ATTACK;
        last_pos =

        hide_cursor();
        start_turn(CPU);
        break;
      }
    }
  }

  if(j & JOY_II)
  {
    // turn = CPU;
    // return;
    if(selector_mode == PLACE_MODE)
    {
      set_cursor_pos(unit_selected);
      selector_mode = last_command;
      selector_mode = MENU_MODE;
      menu_option = MENU_ATTACK;
      display_menu(148,4);
      unhighlight();
    }
    else if(selector_mode == MENU_MODE)
    {
      hide_menu();
      draw_selector();
      selector_mode = SELECT_MODE;
      // selector_mode = last_command;
    }
    else if(selector_mode == ATTACK_MODE)
    {
      set_cursor_pos(unit_selected);
      // pattern(unit_selected,0,coords,entities[battle_grid[unit_selected]-1].unit->ign);
      pattern(unit_selected,0,coords,0);
      // selector_mode = MENU_MODE;
      selector_mode = last_command;
      display_menu(148,4);
      // display_selector();
    }
    else if(selector_mode == ACTION_MODE)
    {
      undo();
      selector_mode = PLACE_MODE;
    }
    // if(selector_mode == OBSERVE_MODE)
    // {
    //   highlight(abs,0,3);
    //   pattern(abs,0,coords,entities[battle_grid[unit_selected]-1].unit->ign);
    //   selector_mode = SELECT_MODE;
    // }
  }

  if(j & JOY_RUN)
  {
    // if(selector_mode == SELECT_MODE)
    // {
    //   if(battle_grid[abs] != 0)
    //   {
    //     if(entities[battle_grid[abs]-1].team == CPU)
    //     {
    //       pattern(abs,0xC000,coords,entities[battle_grid[abs]-1].unit->ign);
    //       selector_mode = OBSERVE_MODE;
    //     }
    //   }
    // }

  }

  if(j & JOY_SEL)
  {
    // get_random_item_by_level(0);
    // get_random_item_by_level(0);
    // get_random_item_by_level(0);
    // get_random_item_by_level(0);

    // post_battle_dialog();
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
        // calc_move_cost(unit_selected,graph_from_x_y(sx-16,sy));
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
        // calc_move_cost(unit_selected,graph_from_x_y(sx+16,sy));
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
  if(s_y < 240)
  {
    if(selector_mode == PLACE_MODE)
    {
      if(valid_map_square(graph_from_x_y(sx,sy+16)))
      {
        // calc_move_cost(unit_selected,graph_from_x_y(sx,sy+16));
        update_selector_pos(0,16);
      }
    }
    else
    {
      spr_set(SELECTOR);
      update_selector_pos(0,16);
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
        // calc_move_cost(unit_selected,graph_from_x_y(sx,sy-16));
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
    // if((map[i].ownY*16)+map[i].ownX == desired) return 1;
    if(map[i].ownPos == desired) return 1;
  }
  return 0;
}

void set_cursor_pos(int pos)
{
  sx = (pos % 16) * 16;
  sy = (((pos+yOffset) / 16) * 16);
  spr_set(SELECTOR);
  spr_x(sx);
  spr_y(sy);
}

int abs_x_from_graph(int pos){ return graph_x_from_abs(pos)*16; }

int abs_y_from_graph(int pos){ return (graph_y_from_abs(pos)*16); }

int graph_x_from_abs(int pos){ return (pos % BF_X_TILES); }

int graph_y_from_abs(int pos){ return (pos / TILE_SIZE); }

int graph_from_x_y(int x, int y)
{
  return (x/TILE_SIZE)+((y/TILE_SIZE)*TILE_SIZE) - (yOffset);
}

void display_menu(int x, int y)
{
  menu_option = y/12;
  cursor_x = x;
  cursor_y = y;
  // spr_hide(SELECTOR);
  // hide_selector();
  spr_make(CURSOR,cursor_x,cursor_y,0x6900,FLIP_MAS|SIZE_MAS,NO_FLIP|SZ_16x16,20,1);
  spr_show(CURSOR);
  scroll(1,0,0,0,32,0xC0);
}

void highlight(int square, int pal, int depth)
{
  int i, p;
  p = get_path(square,999,path,battle_grid,entities[battle_grid[square]-1].team,depth,0);
  for(i=0; i<map_size+1; i++)
  {
    change_background_palette((map[i].ownPos),pal,(464*map_no));
  }
}

void get_unit_radius(int square, int distance, char team, char ignore)
{
  get_path(square,999,path,battle_grid,team,distance,ignore);
}

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
    if(is_traversable(pos) || ignore_terrain && pos < 464)
    {
      change_background_palette(pos,pal,(464*map_no));//change back to 224
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

void display_id(char id, int x, int y)
{
  char i;
  // char id;
  // id = battle_grid[graph_from_x_y(sx,sy)];
  put_number(id-1,2,x,y);
  // put_string("id",x,y);
  // if(id)
  // {

  for(i=0; i<commanders[entities[id].id].row_counts[0]; i++)
  {
    put_number(commanders[entities[id].id].row_one[i],3,x+3,y+i);
  }

  for(i=0; i<commanders[entities[id].id].row_counts[1]; i++)
  {
    put_number(commanders[entities[id].id].row_two[i],3,x+7,y+i);
  }

  for(i=0; i<commanders[entities[id].id].row_counts[2]; i++)
  {
    put_number(commanders[entities[id].id].row_three[i],3,x+11,y+i);
  }
}

void display_army_size(char id, int x, int y)
{
  // put_string("HP",x,y);
  // put_number(entities[id].army_size,2,x+2,y);
}

void display_position(int x, int y)
{
  put_number(graph_from_x_y(sx,sy),3,x,y);
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

void display_stats(char id, int x, int y)
{
  // print_unit_attack_icon(entities[id].id,x,y);
  // put_number(entities[id].unit->atk,2,x+1,y);
  // print_unit_defense_icon(entities[id].id,x+3,y);
  // put_number(entities[id].unit->def,2,x+4,y);
}

void display_type(char id, int x, int y)
{
  // print_unit_type(entities[id].unit->id,x,y);
}

char begin_battle(int attacker, int target, int attacker_pos, int target_pos)
{
  int i, battle_result, attacker_before, attacker_after, target_before, target_after, resolution;
  // satb_update();
  disp_off();
  // vsync();
  // for(i=0; i<64; i++)
  // {
  //   spr_hide(i);
  // }
  // cls();
  // satb_update();
  reset_satb();
  vsync();

  attacker_before = entities[attacker].army_size;
  target_before = entities[target].army_size;
  // hide_npcs(5);
  // resolution = battle_loop(attacker,target,attackable(target,attacker,coords));
  resolution = battle_loop(entities[attacker].id,entities[target].id,1);
  // resolution = battle_loop(0,24,0);

  if(resolution == 0)//attacking team kills army
  {
    destroy_entity(target);
  }
  if(resolution == 1)//targeted team kills army
  {
    destroy_entity(attacker);
  }
  //2 == neither army destroyed

  attacker_after = entities[attacker].army_size;
  target_after = entities[target].army_size;
  battle_result = -1;
  load_ents();

  load_battlefield_map();
  load_sprites_();
  update_map();
  disp_on();
  selector_mode = 0;
  vsync();
  // put_number(target,3,0,0);
  return battle_result;
}

void cleanup_battlefield()
{
  int i;

  for(i=0; i<464; i++)//MAP_SIZE; i++)
  {
    battle_grid[i] = 0;
  }
  for(i=0; i<MAX_ENTITIES; i++)
  {
    entities[i].army_size = 0;
    entities[i].team = 0;
    entities[i].id = 0;
    // entities[i].unit = 0;
    entities[i].pos = 0;
    entities[i].team = 0;
    // entities[i].hp = 0;
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
  yOffset = y_off_holder;
  clear_battle_items();
  reset_satb();
}
