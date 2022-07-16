// #include "battlefield.h"
#include "battle_ai.c"
#include "script.h"

// #incspr(bf_menu, "map/sprites/bf_menu.pcx",0,0,2,4)
// #incspr(be_menu2, "map/sprites/bf_menu.pcx",32,0,2,4)
// #incpal(bf_menu_pal, "map/sprites/bf_menu.pcx")
// #incspr(pad, "map/sprites/direction_pad.pcx")

#incpal(bluepal,"map/sprites/bluetiles.pcx")
#incpal(redpal,"map/sprites/redtiles.pcx")

char enemy_positions[6];
char t, swap;
int id;
int s_x_holder, s_y_holder;
char first;
int walking;
enum Direction cmdr_direction;
char *first_army;
char *second_army;
int b_cycler = 0;
int b_framer = 0;
char s;
int cmdr_x, cmdr_y;
char exit_battlefield;

void battlefield_loop(char *deploy_one, char s_one, char *deploy_two, char s_two, char start_mode)
{
  // turn = CPU;
  exit_battlefield = 1;
  ai_timer = 20;
  cmdr_fig_pal = 16;
  s_x_holder = s_x;
  s_y_holder = s_y;
  walking = 0;
  cmdr_direction = SOUTH;
  cmdr_x = 64;
  cmdr_y = 32;
  spr_set(0);
  spr_y(-16);
  load_vram(0x68C0,selector,0x40);
  load_vram(0x6900,cursor,0x40);

  // u = 198;
  // done = 0;
  // memcpy(highlight,spear_attack,4);
  // put_number(script[0],3,1,1);
  s = script[0];

  first_army = deploy_one;
  second_army = deploy_two;
  memcpy(army_one,deploy_one,s_one);
  memcpy(army_two,deploy_two,s_two);
  size_one = s_one;
  size_two = s_two;
  one_total = s_one;
  two_total = s_two;
  mapper_size = size_one+size_two;
  cmdr_fig_current = COMMANDER_FIGURE_BEGIN;
  // cmdr_fig_pal = 19;
  num_of_bad_terrains = 7;
  t = 0;
  swap = 0;
  enemy_positions[0] = 32;
  enemy_positions[1] = 160;

  untraversable_terrain[0] = 89;
  untraversable_terrain[1] = 90;
  untraversable_terrain[3] = 91;
  untraversable_terrain[4] = 92;
  untraversable_terrain[5] = 93;
  untraversable_terrain[6] = 94;

  init_battlefield();
  load_battlefield_map();

  // selector_mode = 0;
  if(start_mode)
  {
    selector_mode = 0;
    battle_start();
  }
  else
  {
    selector_mode = EXPLORE_MODE;
    load_visible_units();
  }
  // init_units();
  // load_sprites_();
  // update_map();
  display_sprites_();
  scroll(0, 0, 0, 0, 223, 0xC0);
  // display_selector();
  disp_on();
  satb_update();
  // load_palette(16,rei_walk_pal,1);
  // spr_make(2,cmdr_x,cmdr_y,0x6A00,FLIP_MAS|SIZE_MAS,NO_FLIP|SZ_16x32,0,1);
  // load_visible_units();
  vsync();

  turn = PLAYER;
  // selector_mode = EXPLORE_MODE;
  spr_hide(0);
  // while(one_total > 0 && two_total > 0)
  while(exit_battlefield)
  {
    if(turn == CPU)
    {
      ai();
    }
    else
    {
      if(selector_mode == EXPLORE_MODE)
      {
        check_interaction();
        explore();
        walk();
      }
      else if(selector_mode == EXPLORE_MENU_MODE)
      {
        explore_menu();
      }
      else
      {
        display_army_size(1,2);
        display_stats(6,2);
        display_type(4,1);
        ctrls();
        draw_selector();
        cycle_animations();
        check_battle_complete(start_mode);
      }
    }
    satb_update();
    vsync();
  }
  // put_string("over",5,5);
  cleanup_battlefield();
  if(size_one==0)
  {
    return 0;
  }
  if(size_two==0)
  {
    return 1;
  }
  //whoops?
}

void load_visible_units()
{
  load_vram(0x6A00,rei_walk,0x300);
  // add_gentity(1,15,1,&hounds,0,0,0,0);
  load_palette(16,rei_walk_pal,1);
  spr_make(2,cmdr_x,cmdr_y,0x6A00,FLIP_MAS|SIZE_MAS,NO_FLIP|SZ_16x32,0,1);
  // display_spawn(current_global_units-1,enemy_positions[0],enemy_positions[1],1,1);
  if(two_total > 0)
  {
    display_spawn(second_army[0],enemy_positions[0],enemy_positions[1],1,1);
  }
}

void explore_menu()
{
  char joy1;
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
    //do stuff
    if(menu_option == 5)
    {
      exit_battlefield = 0;
    }
    break;

    case JOY_II:
    // spr_set(0);
    spr_hide(1);
    selector_mode = EXPLORE_MODE;
    break;
  }
}

void explore()
{
  char joy1;
  joy1 = joy(0);

  if(walking == 0)
  {
    switch(joy1)
    {
      case JOY_DOWN:
      walking = 16;
      cmdr_direction = SOUTH;
      break;

      case JOY_UP:
      if(cmdr_y>32)
      {
        walking = 16;
        cmdr_direction = NORTH;
      }
      break;

      case JOY_RIGHT:
      walking = 16;
      cmdr_direction = EAST;
      break;

      case JOY_LEFT:
      walking = 16;
      cmdr_direction = WEST;
      break;

      case JOY_I:
      // get_next_dialog();
      selector_mode = EXPLORE_MENU_MODE;
      display_menu();
      break;
    }
  }
}

void walk()
{
  char i;
  b_cycler = (b_cycler+1)%8;
  if(b_cycler == 0)
  {
    b_framer = (b_framer+1)%2;
  }
  spr_set(2);
  if(walking > 0)
  {
    switch(cmdr_direction)
    {
      case SOUTH:
      spr_set(2);
      spr_pattern(0x6A00 + (0x40*b_framer));
      spr_y(++cmdr_y);
      walking--;
      break;

      case NORTH:
      spr_set(2);
      spr_pattern(0x6A00 + (0x40*b_framer) + 0x100);
      spr_y(--cmdr_y);
      walking--;
      break;

      case EAST:
      spr_set(2);
      spr_x(++cmdr_x);
      spr_pattern(0x6A00 + (0x40*b_framer) + 0x200);
      spr_ctrl(FLIP_MAS|SIZE_MAS,NO_FLIP_X|SZ_16x32);
      walking--;
      break;

      case WEST:
      spr_set(2);
      spr_pattern(0x6A00 + (0x40*b_framer) + 0x200);
      spr_ctrl(FLIP_MAS,FLIP_X|SZ_16x32);
      spr_x(--cmdr_x);
      walking--;
      break;
    }
  }
}

void check_interaction()
{
  char i=0;
  for(i; i<3; i++)
  {
    if(enemy_positions[i*2] == cmdr_x && enemy_positions[i*2+1] == cmdr_y)
    {
      battle_start();
    }
  }
}

void battle_start()
{
  // spr_hide(1);
  // spr_hide(10);
  selector_mode = 0;
  sx = 32;
  sy = 32;
  cmdr_x += 16;
  reset_satb();
  init_units();
  load_sprites_();
  draw_selector();

  update_map();
}

void check_battle_complete(char start_mode)
{
  if(one_total > 0 && two_total > 0)
  {
    return;
  }
  if(start_mode == 0)
  {
    reset_satb();
    load_visible_units();
    enemy_positions[0] = 0;
    enemy_positions[1] = 0;
    enemy_positions[2] = 0;
    enemy_positions[3] = 0;
    enemy_positions[4] = 0;
    enemy_positions[5] = 0;

    selector_mode = EXPLORE_MODE;
  }
  else if(start_mode == 1)
  {
    exit_battlefield = 0;
  }
}
//this is awful. Since our sprites in the SATB don't have a reference to our
//entities, and vice versa, we don't know what sprite number our sprite is
//which entity. So we do a bunch of math on the sprite to figure out which
//pattern it has and if it's one the second or first cycle
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
      pattern = (spr_get_pattern() - 3800) / 0x100;
      if((pattern % 2))
      {
        spr_pattern((spr_get_pattern() + 0x100));
      }
      else
      {
        spr_pattern((spr_get_pattern() - 0x100));
      }
    }
  }
}

void init_units()
{
  char *u;
  char i;

  for(i=0, u=army_one; i<size_one; i++, u++)
  {
    switch((unit_list[*u]).unit_type)
    {
      case INFANTRY:
      add_entity(INF,MAX_ARMY_SIZE,PLAYER,cmdr_fig_pal+i,SWORD_UNIT,1,*u,53+i);
      battle_grid[53+i] = num_of_entities;
      break;

      case SPEARS:
      add_entity(SPEAR,MAX_ARMY_SIZE,PLAYER,cmdr_fig_pal+i,SPEAR_UNIT,1,*u,53+i);
      battle_grid[53+i] = num_of_entities;
      break;

      case MUSKETS:
      add_entity(MUSKET,MAX_ARMY_SIZE,PLAYER,cmdr_fig_pal+i,ARCHER_UNIT,1,*u,53+i);
      battle_grid[53+i] = num_of_entities;
      break;

      case FLYERS:
      add_entity(FLYERS,MAX_ARMY_SIZE,PLAYER,cmdr_fig_pal+i,DEMON_UNIT,1,*u,53+i);
      battle_grid[53+i] = num_of_entities;
      break;

      case HOUNDS:
      add_entity(HOUNDS,MAX_ARMY_SIZE,PLAYER,cmdr_fig_pal+i,HOUND_UNIT,1,*u,53+i);
      battle_grid[53+i] = num_of_entities;
      break;

      case COMMANDER:
      add_entity(INF,1,PLAYER,cmdr_fig_pal+i,SWORD_UNIT,1,*u,53+i);
      battle_grid[53+i] = num_of_entities;
      break;

      default:
      add_entity(INF,MAX_ARMY_SIZE,PLAYER,cmdr_fig_pal+i,SWORD_UNIT,1,*u,53+i);
      battle_grid[53+i] = num_of_entities;
      break;
    }
  }
  // entities = ai_entities;
  for(i=0, u=army_two; i<size_two; i++,u++)
  {
    // switch(commanders[*u].army_type)
    // switch((*commanders[*u].unit).unit_type)
    switch((unit_list[*u]).unit_type)
    {
      case INFANTRY:
      add_entity(INF,MAX_ARMY_SIZE,CPU,cmdr_fig_pal+i+size_one,SWORD_UNIT,1,*u,197+i);
      battle_grid[197+i] = num_of_entities;
      break;

      case SPEARS:
      add_entity(SPEAR,MAX_ARMY_SIZE,CPU,cmdr_fig_pal+i+size_one,SPEAR_UNIT,1,*u,197+i);
      battle_grid[197+i] = num_of_entities;
      break;

      case MUSKETS:
      add_entity(MUSKET,MAX_ARMY_SIZE,CPU,cmdr_fig_pal+i+size_one,ARCHER_UNIT,1,*u,197+i);
      battle_grid[197+i] = num_of_entities;
      break;

      case FLYERS:
      add_entity(FLYERS,MAX_ARMY_SIZE,CPU,cmdr_fig_pal+i+size_one,DEMON_UNIT,1,*u,197+i);
      battle_grid[197+i] = num_of_entities;
      break;

      case HOUNDS:
      add_entity(HOUNDS,MAX_ARMY_SIZE,CPU,cmdr_fig_pal+i+size_one,HOUND_UNIT,1,*u,197+i);
      battle_grid[197+i] = num_of_entities;
      break;

      case COMMANDER:
      add_entity(INF,1,CPU,cmdr_fig_pal+i+size_one,SWORD_UNIT,1,*u,197+i);
      battle_grid[197+i] = num_of_entities;

      default:
      add_entity(INF,MAX_ARMY_SIZE,CPU,cmdr_fig_pal+i+size_one,SWORD_UNIT,1,*u,197+i);
      battle_grid[197+i] = num_of_entities;
      break;
    }
  }
  load_units(0x3800);
  // entities = player_entities;
  // cmdr_fig_pal = 19;
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
}

void load_battlefield_map()
{
  load_palette(0,overworldpal,7);
  load_palette(10,fontpal,2);
  set_font_pal(10);
	load_font(font,125,0x2F00);

  // load_palette(10,fontpal,2);
  load_palette(8,borderspal,1);
  load_palette(11,bluepal,1);
  load_palette(12,redpal,1);

  set_screen_size(SCR_SIZE_32x32);

  load_vram(0x6400,borders,0x100);
  // load_vram(0x9A0,icons_gfx,0x20);
  load_vram(0x30A0,icons_gfx,0x20);

  set_map_data(battlefieldbat,16,14);
  // set_tile_data(battlefield,28,overworldtilespal,16);
  load_tile(TILE_VRAM);
  load_map(0,2,0,0,16,14);
  s_x = 0;
  s_y = 0;
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
  load_palette(29,selectorpal,1);
  // load_palette(30,bf_menu_pal,1);
  // load_palette(31,actionpal,1);

  load_palette(11,bluepal,1);
  load_palette(12,redpal,1);

  load_palette_groups();

  load_vram(0x68C0,selector,0x40);
  load_vram(0x6900,cursor,0x40);

  display_sprites_();
  load_units(0x3800);
}

void display_sprites_() //info sprites
{
  spr_make(0,sx,sy,0x68C0,0,NO_FLIP|SZ_16x16,29,1);
  spr_make(1,cursor_x,cursor_y,0x6900,FLIP_MAS|SIZE_MAS,NO_FLIP|SZ_16x16,29,1);
}

int get_address_from_id(char id)
{
  char i;
  for(i=0; i<mapper_size; i++)
  {
    if(cmdr_vram_mapper[i*2] == id)
    {
      return cmdr_vram_mapper[i*2+1];
    }
  }
}

void update_map()
{
  int squares, i, sprite_counter, entity_no, p, z;
  char s;
  z=0;
  squares = 224;
  sprite_counter = 63;

  for(i = 0; i < squares; i++)
  {
    if(battle_grid[i] != 0)
    {
      entity_no = battle_grid[i]-1;
      spr_make(sprite_counter--
              ,abs_x_from_graph(i)
              ,abs_y_from_graph(i)-16
              ,entities[entity_no].unit->address
              ,FLIP_MAS|SIZE_MAS
              ,SZ_16x32
              ,entities[entity_no].unit->p_group+(entities[entity_no].team)
              ,1);
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
  char j;
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
        if(check_action_cost(MOVE_COST))
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
    else if(selector_mode == DIALOG_MODE)
    {
      get_next_dialog();
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
}

void set_dialog_mode()
{
  // selector_mode = DIALOG_MODE;
  // sx = -32;
  // sy = -32;
  // load_portrait(2);
  // display_item(2,32,0);
  //
  // set_font_color(1,2);
  // set_font_pal(5);
  // load_default_font(0,0x800);
}

void get_next_dialog(int index)
{
  // put_char(script[0],1,1);
  s_x = 0;
  s_y = 0;
  display_info_panel(0,0,32,6);
  load_portrait(0);
  display_item(0,0,1,1);
  write_text(5,2,script);
  // write_text(5,2,script+5);
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
  if(sy < 224 - 16)
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
      spr_set(0);
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
  char i;
  get_path(square,999,path,battle_grid,entities[battle_grid[square]-1].team,depth,entities[battle_grid[square]-1].unit->ign);
  for(i=0; i<map_size+1; i++)
  {
      change_background_palette((map[i].ownY*16)+map[i].ownX,pal);
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
      change_background_palette(pos,pal);
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
    put_number(id,3,x+3,y);
  }
  else
  {
    put_number(0,3,x+3,y);
  }
}

void display_stamina(int x, int y)
{
  char id;
  id = battle_grid[graph_from_x_y(sx,sy)];
  put_string("stam:",x,y);
  if(id)
  {
    put_number(entities[id-1].stamina,2,x+5,y);
  }
  else
  {
    put_number(0,2,x+5,y);
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
    put_number((*entities[id-1].unit).atk,2,x+1,y);
    put_string(";",x+4,y);
    put_number((*entities[id-1].unit).def,2,x+5,y);
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
    print_unit_type((*entities[id-1].unit).unit_type,x,y);
  }
  else
  {
    put_string("        ",x,y);
  }
}

char begin_battle(char attacker, char target, int attacker_pos, int target_pos)
{
  char i, battle_result, attacker_before, attacker_after, target_before, target_after, resolution;
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
    // put_number(8,1,5,1);
    // entities[attacker].actionable = 0;
    break;

    default:
    // put_number(9,1,5,1);
    // entities[attacker].actionable = 0;
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
  disp_off();

  for(i=0; i<MAP_SIZE; i++)
  {
    battle_grid[i] = 0;
  }
  for(i=0; i<MAX_ENTITIES; i++)
  {
    // entities[i]->unit = 0;
    // entities[i].unit_type = 0;
    // global_units[entities[i].id].exp = 25;
    entities[i].army_size = 0;
    entities[i].team = 0;
    // entities[i].pal = 0;
    entities[i].id = 0;
  }

  for(i=0; i<mapper_size; i++)
  {
    cmdr_vram_mapper[i*2] = 0;
    cmdr_vram_mapper[i*2+1] = 0;
  }

  for(i=0; i<64; i++)
  {
    spr_hide(i);
  }
  satb_update();
  mapper_size = 0;
  size_one = 0;
  size_two = 0;
  one_total = 0;
  two_total = 0;
  // army_one = 0;
  // army_two = 0;
  //sy = -1 * y_offset; //why did I decide to use sy and sx varibles here???
  //sx = -1 * x_offset;

  s_x = s_x_holder;
  s_y = s_y_holder;
  reset_satb();
  // satb_update();
  cls();
}
