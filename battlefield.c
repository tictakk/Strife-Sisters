// #include "battlefield.h"
#include "battle_ai.c"

#incpal(bluepal,"map/sprites/bluetiles.pcx")
#incpal(redpal,"map/sprites/redtiles.pcx")
#incpal(greenpal,"map/sprites/greentiles.pcx")
#incpal(greypal,"assets/faded_pal.pcx")

#define MENU_ATK_X 152
#define MENU_ATK_Y 16
#define MENU_GROUP_X 216
#define MENU_GROUP_Y 8
#define MENU_TURN_X 216
#define MENU_TURN_Y 16
#define WAIT_TIME 8

int position_1 = 0, position_2 = 0, position_len = 0;
const char dumb_table[9] = {0,3,6,1,4,7,2,5,8};
int g_abs;
int id;
int s_x_holder, s_y_holder, y_off_holder;
enum Direction cmdr_direction;
char exit_battlefield;
char t = 0;
char wait = 0;

char toggle_enemy_higlight = 0;
char joy_left_held = 0;
char joy_right_held = 0;
char joy_down_held = 0;
char joy_up_held = 0;

//variables for update map routine
int index, sprite_counter, entity_no;
char t_type;

void play_story()
{
  menu_height = 32;
  story(map_no,INBATTLE,0);
  psgFadeOut(10);
  scroll(0,s_x,s_y+32,32,224,0xC0);
  scroll(1,0,0,0,32,0x80);
  display_window_abs(0,0,18,4);
  display_window_abs(18,0,14,4);
  menu_mask = 0x00;
  print_menu();
  load_map(0,2,0,0,16,20);
  load_palette(12,bluepal,1);
  load_palette(13,redpal,1);
  menu_height = 32;
  pan_camera_y(entities[0].pos);
  sx = (entities[0].pos&15) << 4;
  sy = ((entities[0].pos>>4) << 4) - s_y;
  display_selector(SELECTOR,sx,sy,16);
  clear_joy_events(0x1F);
  npcs[1].pos_y = 0;
}

void begin_battlefield(char map_id)
{
  char i;
  current_turn = 1;
  menu_mask = 0;
  map_no = map_id;
  map_offset = (320*map_no)-32;
  objective_pos = battle_map_metadata.event_positions[0];
  check_add_new_commander(map_no);

  yOffset = 0;
  init_battlefield();

  selector_mode = 0;

  for(i=0; i<8; i++)
  {
    if(battle_map_metadata.player_start_pos[i] != 0)
    {
      no_of_player_cmdrs++;
    }
  }
  battle_start();
  battlefield_loop(map_id);
}

void battlefield_loop(char map_id)
{
  char id;
  last_command = SELECT_MODE;
  map_result_status = MAP_RESULT_NONE;
  selected_option = -1;
  turns_count = 0;
  units_killed = 0;
  units_lost = 0;
  materials_collected = 0;
  wait = 0;

  s_x_relative = 0;//(s_x/8);
  s_y_relative = 0;//(s_y/8);
  // ss = script[1];

  disp_on();
  satb_update();
  vsync();
  turn = PLAYER;
  select_unit(0);
  // play_story();
  display_selector(SELECTOR,sx,sy,16);
  // psgPlay(3);
  // while(exit_battlefield)
  // put_number((char)unit_header[0].hp,3,0,0);
  while(map_result_status == MAP_RESULT_NONE)
  {
    if(turn == CPU)
    {
      ai();
      check_battle_complete();
    }
    else
    {
      // put_number(map_result_status,3,0,0);
      // check_battle_complete();
      // position = graph_from_x_y(sx,sy);
      g_abs = graph_from_x_y(sx,sy);
      t_type =  terrain_type(battlefieldbat[map_offset+g_abs]);
      id = battle_grid[g_abs];
      display_position(14,1);
      put_number(map_no,2,14,2);
      // display_commander_stats(9,1);
      // display_level(4,1);
      // display_enemy_remaining(13,1);
      display_terrain_bonus();
      swap_water_tiles();
      // if(selector_mode != ARMY_MODE){ cycle_terrain_items(); }
      ctrls();
      cycle_animations();
      check_battle_complete();
    }
    satb_update();
    vsync();
  }
  if(map_result_status == MAP_RESULT_WIN)
  {
    win_condition();
  }
  else
  {
    lose_condition();
  }

  cleanup_battlefield();
  // return 0;
  //whoops?
}

char check_item_pickup()
{
  char item_no;
  if((item_no = item_at_position(selected_entity->pos)) > -1)
  {
    collect_item(item_no);
  }
}

void battle_start()
{
  char i;
  selector_mode = 0;
  one_total = no_of_player_cmdrs;
  two_total = cpu_cmdr_count;

  reset_satb();
  init_units();
  load_map_items();
  update_map();
  load_battlefield_map();
  // load_map_items();
  sx = (entities[0].pos&15) << 4;
  sy = (entities[0].pos>>4) << 4;
  load_sprz();
}

void check_battle_complete()
{
  if(battle_grid[objective_pos] != 0 && map_type == 2)
  {
    if(entities[battle_grid[objective_pos]-1].team == PLAYER)
    {
      map_result_status = MAP_RESULT_WIN;
      return;
    }
  }

  if(battle_grid[objective_pos] != 0 && map_type == 1)
  {
    if(entities[battle_grid[objective_pos]-1].team == CPU)
    {
      map_result_status = MAP_RESULT_LOSE;
      return;
    }
  }

  if(one_total > 0 && two_total > 0)
  {
    return;
  }

  if(one_total <= 0)
  {
    map_result_status = MAP_RESULT_LOSE;
    return;
  }
  map_result_status = MAP_RESULT_WIN;
}

char check_terrain_triggered()
{
  char i;
  for(i=0; i<MAX_EVENT_TERRAIN; i++)
  {
    if(battle_grid[event_terrains[i]])
    {
      return 1;
    }
  }
  return 0;
}

void win_condition()
{
  post_battle_screen();
  satb_update();
  vsync();
  exit_battlefield = 0;
  s_x = s_x_holder;
  s_y = s_y_holder;
}

void lose_condition()
{
  post_battle_screen();
  satb_update();
  vsync();
  game_over = 0;
  exit_battlefield = 0;
  s_x = s_x_holder;
  s_y = s_y_holder;
}

void cycle_animations()
{
  if(t++ == 10)
  {
    increment_frame();
    update_map();
    cycle_selector();
  }
}

void init_units()
{
  int i;
  
  for(i=0; i<party_size; i++)
  {
    heal_commander_bg(i);
    load_group(i,PLAYER,battle_map_metadata.player_start_pos+i,party_commanders);//commander + max_army_size
  }

  for(i=0; i<cpu_cmdr_count; i++)
  {
    load_predefined_group_layout((char)battle_map_metadata.cpu_commander_ids[i*4], //formation
    (char)battle_map_metadata.cpu_commander_ids[i*4+1], //row one units
    (char)battle_map_metadata.cpu_commander_ids[i*4+2], //row two units
    (char)battle_map_metadata.cpu_commander_ids[i*4+3], //row three units
    i+MAX_PARTY_COMMANDERS,(char)map_no);
    load_group(i,CPU,battle_map_metadata.cpu_start_pos+i,enemy_commanders);
  }
}

void load_group(char id, char team, int positions[7], struct Commander *commanders)
{
  // put_number(positions[0],5,0,0);
  // wait_for_I_input();
  add_entity(team,UNIT_PALS[commanders[id].sprite_type],id,positions[0],commanders);
  battle_grid[positions[0]] = num_of_entities;
}

void load_ents()
{
  char i;
  reset_npcs();
  for(i=0; i<num_of_entities; i++)
  {
    if(entities[i].team == PLAYER)
    {
      add_npc(entities[i].pos%16,entities[i].pos/16,
            party_commanders[entities[i].id].sprite_type,
            0);
      // put_number(UNIT_PALS[party_commanders[entities[i].id].sprite_type],3,0,0);
      // wait_for_I_input();
      sync(1);
      if(entities[i].actionable == 0)
      {
        darken_palette(get_commander_palette(party_commanders[entities[i].id].sprite_type));
      }
    }
    else
    {
      add_npc(entities[i].pos%16,entities[i].pos/16,
          party_commanders[entities[i].id].sprite_type,
          UNIT_PALS[party_commanders[entities[i].id].sprite_type]);
    }
  }
}

void init_battlefield()
{
  selector_mode = 0;
  num_of_entities = 0;
  cursor_x = -16;
  cursor_y = -16;
  menu_option = 0;

  exit_battlefield = 1;
  s_x_holder = s_x;
  s_y_holder = s_y;
  s_x = 0;
  s_y = 0;
  // no_of_npcs = 0;

  // spr_set(SELECTOR);
  // spr_y(-16);
  load_vram(0x68C0,selector,0x40);
//  load_vram(0x6900,cursor,0x40);
  load_battlefield_map();
}

void load_battlefield_map()
{
  set_screen_size(SCR_SIZE_32x64);
  scroll(1,0,0,0,32,0x80);
  scroll(0,s_x,s_y+32,32,224,0xC0);

  load_palette(0,overworldpal,8);
  load_palette(10,fontpal,2);
  set_font_pal(10);
	load_font(font,125,0x4800);
	load_vram(0x4BB0,icons_gfx,0x60);

  load_palette(9,borderspal,1);
  load_palette(12,bluepal,1);
  load_palette(13,redpal,1);
  load_palette(31,greypal,1);
  load_terrains();

  // set_screen_size(SCR_SIZE_32x64);
  screen_dimensions = 32;

  load_vram(0x6400,borders,0x100);
  load_vram(0x30A0,icons_gfx,0x60);
  
  // set_map_data(battlefieldbat+(464*map_no),16,29);
  set_map_data(battlefieldbat+(320*map_no),16,20);
  set_tile_data(overworldtiles,162,overworldtilespal,TILE_WIDTH);

  load_tile(TILE_VRAM);
  load_map(0,2,0,0,16,20);

  // display_abs_info_panel(0,0,18,4);
  // display_abs_info_panel(18,0,14,4);
  display_window_abs(0,0,18,4);
  display_window_abs(18,0,14,4);
  menu_mask = 0x00;
  print_menu();
  // load_map_items();
}

void load_map_items()
{
  char i;
  terrain_item_count = 0;
  for(i=0; i<MAX_TERRAIN_ITEMS; i++)
  {
    if(battle_map_metadata.map_items[i])
    {
      create_terrain_item(battle_map_metadata.map_items[i],
                          battle_map_metadata.item_positions[i]&15,
                          battle_map_metadata.item_positions[i]/16,4);
    }
  }
}

void display_sprite_type(int pos)
{
  char id;
  id = battle_grid[pos];
  if(id)
  {
    put_number(party_commanders[entities[id-1].id].sprite_type,3,0,0);
  }
  else
  {
    put_string("   ",0,0);
  }
}

void display_enemy_remaining(char x, char y)
{
  put_string("Foes",x,y);
  put_number(two_total,2,x+2,y+1);
}

void display_terrain_bonus()
{
  // terrain_type =  terrain_type(battlefieldbat[map_offset+id]);
  put_terrain_icon(t_type,1,1);
  put_terrain_bonus(t_type,3,2);
  // put_terrain_atk_stat(t_type,3,2);
}

void load_sprz() //load default sprites
{
  load_palette(12,bluepal,1);
  load_palette(13,redpal,1);
  load_vram(0x68C0,selector,0x40*3);
  load_palette(16,selectorpal,1);
  display_selector(0,sx,sy,16);
}

void update_map()
{
  t=0;
  squares = 224+s_y;//464;
  sprite_counter = 63;

  for(index=s_y+32; index<squares; index++)
  {
    if(battle_grid[index] != 0)
    {
      entity_no = battle_grid[index]-1;

      // draw_npc(sprite_counter--,(i&15)*16,(((i/16)*16)-16),entity_no);
      draw_npc(sprite_counter--,(index&15)<<4,(((index>>4)<<4)-16),entity_no);
      // if(entities[entity_no].actionable == 0 && entities[entity_no].team == PLAYER)
      // {
        // darken_palette(entity_no+26);
      //   spr_pal(31);
      // }
    }
  }
  cycle_terrain_items();
  for(index=6; index<sprite_counter+1; index++)
  {
    spr_hide(index);
  }
}

void cursor_left()
{
  if(menu_option < menu_rows)
  {
    return;
  }
  remove_cursor();
  cursor_x -= 4;
  menu_option -= (menu_rows);
  display_cursor();
}

void cursor_right()
{
  if(menu_option >= ((menu_rows*menu_columns)-menu_rows))
  {
    return;
  }
  remove_cursor();
  cursor_x += 4;
  menu_option += (menu_rows);
  display_cursor();
}

void cursor_down()
{
  if(menu_option == (menu_rows*1 - 1) || menu_option == (menu_rows*2 - 1) || menu_option == (menu_rows*3 - 1))
  {
    return;
  }
  remove_cursor();
  cursor_y += (menu_vert_size);
  menu_option += 1;
  display_cursor();
}

void cursor_up()
{
  if(!(menu_option%menu_rows))
  {
    return;
  }
  remove_cursor();
  cursor_y -= menu_vert_size;
  menu_option -= 1;
  display_cursor();
}

void ctrls()
{
  unsigned char j, jy;

  j = joy(0);
  jy = joytrg(0);

  if(selector_mode == SELECT_MODE || selector_mode == PLACE_MODE || selector_mode == ATTACK_MODE)
  {
    j = joy(0);
  }
  else
  {
    j = joytrg(0);
  }

  if(j & JOY_UP)
  {
    joy_up_held++;
    
    if(joy_up_held == 1 || joy_up_held > WAIT_TIME)
    {
      if(selector_mode == SELECT_MODE || selector_mode == PLACE_MODE || selector_mode == ATTACK_MODE)
      {
        selector_up();
        joy_up_held = 2;
      }
      else if(selector_mode == MENU_MODE || selector_mode == ACTION_MODE || selector_mode == PLAYER_ARMY_MODE)
      {
        cursor_up();
      }
    }
  }
  else { joy_up_held = 0;}

  if(j & JOY_DOWN)
  {
    joy_down_held++;
    if(joy_down_held == 1 || joy_down_held > WAIT_TIME)
    {
      if(selector_mode == SELECT_MODE || selector_mode == PLACE_MODE || selector_mode == ATTACK_MODE)
      {
        selector_down();
        joy_down_held = 2;
      }
      else if(selector_mode == MENU_MODE || selector_mode == ACTION_MODE || selector_mode == PLAYER_ARMY_MODE)
      {
        cursor_down();
      }
    }
  }else { joy_down_held = 0; }

  if(j & JOY_RIGHT)
  {
    joy_right_held++;
    if(joy_right_held == 1 || joy_right_held > WAIT_TIME)
    {
      if(selector_mode == SELECT_MODE || selector_mode == PLACE_MODE || selector_mode == ATTACK_MODE)
      {
        selector_right();
        joy_right_held = 2;
      }
      else if(selector_mode == MENU_MODE || selector_mode == ACTION_MODE)
      {
        cursor_right();
      }
      else if(selector_mode == PLAYER_ARMY_MODE)
      {
        if(menu_option >= ((menu_rows*menu_columns)-menu_rows))
        {
          return;
        }
        menu_option += menu_rows;
        curs_right(5);
      }
    }
  } else { joy_right_held = 0; }

  if(j & JOY_LEFT)
  {
    joy_left_held++;
    if(joy_left_held == 1 || joy_left_held > WAIT_TIME)
    {
      if(selector_mode == SELECT_MODE || selector_mode == PLACE_MODE || selector_mode == ATTACK_MODE)
      {
        selector_left();
        joy_left_held = 2;
      }
      else if(selector_mode == MENU_MODE || selector_mode == ACTION_MODE)
      {
        cursor_left();
      }
      else if(selector_mode == PLAYER_ARMY_MODE)
      {
        if(menu_option < menu_rows)
        {
          return;
        }
        menu_option -= menu_rows;
        curs_left(5);
      }
    }
  } else { joy_left_held = 0; }

  // abs = graph_from_x_y(sx,sy);

  if(jy & JOY_I)
  {
    if(selector_mode == SELECT_MODE)
    {
      if(battle_grid[g_abs] != 0)
      {
        // if(entities[battle_grid[g_abs]-1].team == PLAYER)
        // {
          last_command = selector_mode;
          id = battle_grid[g_abs];
          selector_mode = MENU_MODE;
          select_unit(id-1);
          set_menu_mask(id-1);
          get_unit_radius(g_abs,get_army_min_move(id-1),entities[id-1].team,get_army_min_move(id-1));
          // put_number(map_size,3,0,0);
          display_menu(152,8);
          if(entities[battle_grid[g_abs]-1].team == CPU)
          {
            display_move_range(selected_entity->pos);
          }
        // }
      }
    }
    else if(selector_mode == PLACE_MODE)
    {
      if(battle_grid[g_abs] == 0 && (is_traversable(g_abs)) && valid_map_square(g_abs))
      {
        remove_cursor();
        last_command = selector_mode;
        // move_unit_new(abs);
        walk_entity(get_entity_id(selected_entity->pos),g_abs);

        selector_mode = ACTION_MODE;
        selected_entity->movable = 0;
        set_menu_mask(battle_grid[g_abs]-1);
        display_menu(MENU_ATK_X,MENU_ATK_Y);
        draw_selector();

        unhighlight();
      }
    }
    else if(selector_mode == ATTACK_MODE)
    {
      if(battle_grid[g_abs] != 0 && valid_map_square(g_abs))
      {
        if(entities[battle_grid[g_abs]-1].team != selected_entity->team)
        {
          check_item_pickup();
          attack_unit(g_abs,selected_entity->pos,0);
          check_end_turn();
        }
      }
    }
    else if(selector_mode == MENU_MODE || selector_mode == ACTION_MODE)
    {
      last_command = selector_mode;
      switch(menu_option)
      {
        case MENU_ATTACK:
          if(menu_mask & 0x2)
          {
            selector_mode = ATTACK_MODE;
            hide_menu();
            draw_selector();
            get_unit_radius(selected_entity->pos,get_army_max_range(get_entity_id(selected_entity->pos)),selected_entity->team,0);
            highlight(selected_entity->pos,ACTION_ATTACK);
          }
          break;

        case MENU_MOVE:
          if(menu_mask & 0x1)
          {
            hide_menu();
            display_move_range(selected_entity->pos);
            selector_mode = PLACE_MODE;
            menu_option = MENU_ATTACK;
          }
          break;

        case MENU_END:
          if(menu_mask & 0x4)
          {
            end_unit_turn(get_entity_id(selected_entity->pos));
          }
          break;

        case MENU_TURN:
          if(menu_mask & MASK_TURN)
          {
            check_item_pickup();
            end_player_turn();
          }
          break;

        case MENU_GRP:
          if(menu_mask & MASK_GROUP)
          {
            remove_cursor();
            scroll(0,0,s_y+32,32,224,0x80);
            // display_abs_info_panel(7+(s_x),32+(s_y/2),18,15); //find a way to remove
            display_window_rel(7,8+(s_y/8),18,15);
            set_font_pal(9);
            put_string("Front",14,9+(s_y/8));
            put_string("Rear ",14,21+(s_y/8));
            set_font_pal(10);
            selector_mode = CPU_ARMY_MODE;
            if(entities[id-1].team == PLAYER)
            {
              cursor_x = 8;
              cursor_y = 11 + (s_y/8);
              display_cursor();
              selector_mode = PLAYER_ARMY_MODE;
              menu_option = 0;
              menu_rows = 3;
              menu_columns = 3;
              menu_vert_size = 4;
              // list_commander_army(entities[id-1].bg,9,10);
            }
            list_commander_army(entities[id-1].bg,9,10);
          }
          break;

        case MENU_TAKE://calling
          // if(menu_mask & MASK_CALLING)
          // {
            // check_item_pickup();
            // display_calling_background(0,s_y+32,get_entity_id(selected_entity->pos));
            // set_calling(selected_entity->bg->calling_stone,selected_entity->team);
            // animate_calling(calling_stones[selected_entity->bg->calling_stone].effect,PLAYER);
            // display_selector(0,sx,sy,16);
            // hide_menu();
            // selector_mode = SELECT_MODE;
            // display_selector(SELECTOR,sx,sy,16);
            // display_turn(turn);
            // sync(100);
          // }
          break;
      }
    }
    else if(selector_mode == PLAYER_ARMY_MODE)
    {
      if(selected_option == -1)
      {
        set_font_pal(11);
        selected_option = dumb_table[menu_option];
        print_unit_info(&entities[id-1].bg->units[selected_option],9+((selected_option%3)*5)+(s_x/8),10+((selected_option/3)*4)+(s_y/8));
        set_font_pal(10);
      }
      else
      {
        swap_commander_units(entities[id-1].id,selected_option,dumb_table[menu_option]);
        list_commander_army(entities[id-1].bg,9,10);
        selected_option = -1;
      }
    }
  }

  if(jy & JOY_II)
  {
    // if(selector_mode == SELECT_MODE)
    // {
    //   if(position_1 == 0)
    //   {
    //     position_1 = g_abs;
    //   }
    //   else
    //   {
    //     position_2 = g_abs;
    //     put_number(find_attackable_position(position_1,position_2,2,3),4,0,0);
    //     // position_len = get_path(position_1,position_2,path,battle_grid,PLAYER,5,0);
    //     // put_number(position_len,5,0,0);
    //     position_1 = 0;
    //     position_2 = 0;
    //   }
    // }
    if(selector_mode == PLACE_MODE)
    {
      set_cursor_pos(selected_entity->pos);
      selector_mode = last_command;
      selector_mode = MENU_MODE;
      menu_option = MENU_ATTACK;

      display_menu(152,8);
      unhighlight();
    }
    else if(selector_mode == MENU_MODE)
    {
      hide_menu();
      draw_selector();
      selector_mode = SELECT_MODE;
      menu_mask = 0x00;
      print_menu();
    }
    else if(selector_mode == ATTACK_MODE)
    {
      set_cursor_pos(selected_entity->pos);
      selector_mode = last_command;
      // set_menu_mask();
      display_menu(152,8);
      unhighlight();
      get_unit_radius(selected_entity->pos,3,selected_entity->team,1);
    }
    else if(selector_mode == ACTION_MODE)
    {
      undo();
      selector_mode = PLACE_MODE;
      set_menu_mask(id-1);
      display_menu(152,8);
    }
    else if(selector_mode == PLAYER_ARMY_MODE)
    {
      if(selected_option == -1) //haven't selected anything yet
      {
        remove_cursor();
        load_map(0,2,0,0,16,29);
        scroll(0,0,s_y+32,32,224,0xC0);
        selector_mode = MENU_MODE;
        set_menu_mask(id-1);
        display_menu(MENU_GROUP_X,MENU_GROUP_Y);
        // display_cursor();
        // menu_mask = 0x00;
        // print_menu();
      }
      else //unselect
      {
        print_unit_info(&entities[id-1].bg->units[selected_option],9+((selected_option%3)*5)+(s_x/8),10+((selected_option/3)*4)+(s_y/8));
        selected_option = -1;
      }
    }
    else if(selector_mode == CPU_ARMY_MODE)
    {
      load_map(0,2,0,0,16,29);
      scroll(0,0,s_y+32,32,224,0xC0);
      selector_mode = MENU_MODE;
      set_menu_mask(id-1);
      display_cursor();
      // menu_mask = 0x00;
      print_menu();
      display_move_range(selected_entity->pos);
    }
  }

  if(j & JOY_RUN)
  {
    if(selector_mode == SELECT_MODE)
    {
      last_command = selector_mode;
      selector_mode = MENU_MODE;
      selected_entity = 0;
      menu_mask = MASK_TURN;
      display_menu(MENU_TURN_X,MENU_TURN_Y);
    }
    // win_condition();
  }

  if(j & JOY_SEL)
  {
    map_result_status = MAP_RESULT_WIN;
  }
}

void animate_calling(char effect, char target_team)
{
  char i;
  int x, y;

  for(i=0; i<num_of_entities; i++)
  {
    if(entities[i].team == target_team)
    {
      x = (entities[i].pos&15)<<4;
      y = ((entities[i].pos>>4)<<4)-s_y;
      create_art_by_type(effect,x,y,0);
      // create_effect(effect,x,y,0);
      animate_calling_effect(x,y);
      remove_effect(0);
    }
  }
}

void animate_calling_effect(int x, int y)
{
  char i;

  for(i=0; i<11; i++)
  {
    animate_effects();
    satb_update();
    sync(4);
  }
}

void selector_left()
{
  if(sx > 0)
  {
    update_selector_pos(-16,0);
  }
}

void selector_right()
{
  if(sx < 256 - 16)
  {
    update_selector_pos(16,0);
  }
}

void selector_down()
{
  if(s_y <= 240)
  {
    update_selector_pos(0,16);
  }
}

void selector_up()
{
  update_selector_pos(0,-16);
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
  sx = (pos % 16) << 4;
  sy = (((pos+yOffset) >> 4) << 4);
  spr_set(SELECTOR);
  spr_x(sx);
  spr_y(sy);
}

int graph_from_x_y(int x, int y)
{
  return (x>>4)+((y>>4)<<4) - (yOffset);
}

void display_menu(int x, int y)
{
  menu_vert_size = 1;
  menu_columns = 3;
  menu_rows = 2;
  menu_option = y/12 + ((x-152)/16);
  cursor_x = x/8;
  cursor_y = y/8;

  // put_number(menu_option,3,0,0);
  print_menu();
  display_cursor();
  scroll(1,0,0,0,32,0x80);
}

void highlight(int square, char action_type)
{
  int i;
  char pal;
  switch(action_type)
  {
    case ACTION_MOVE: load_palette(12,bluepal,1);break;
    case ACTION_ATTACK: load_palette(12,redpal,1);break;
    case ACTION_CALLING: load_palette(12,greenpal,1);break;
  }
  for(i=0; i<map_size+1; i++)
  {
    change_background_palette(map[i].ownPos,0xC000);
  }
}

void get_unit_radius(int square, int depth, char team, char ignore)
{
  get_path(square,999,path,battle_grid,team,depth,ignore);
}

void display_selector_pos()
{
  put_string("x:",1,1);
  put_number(sx,3,3,1);
  put_string("y:",1,2);
  put_number(sy,3,3,2);
}

void display_camera_info()
{
  put_string("sy:",5,1);
  put_string("s_y:",5,2);
  put_string("yOff:",5,3);
  put_number(sy,3,8,1);
  put_number(s_y,3,9,2);
  put_number(yOffset,3,10,3);
}

void display_position(int x, int y)
{
  put_number(graph_from_x_y(sx,sy),3,x,y);
}

// void display_commander_stats(char x, char y)
// {
//   char id;
//   id = battle_grid[graph_from_x_y(sx,sy)]-1;
//   if(id >= 0)
//   {
//     put_string("tac",x+2,y);
//     put_number(party_commanders[entities[id].id].tac,2,x,y);

//     put_string("wis",x+2,y+1);
//     put_number(party_commanders[entities[id].id].wis,2,x,y+1);

//     put_string("frt",x+2,y+2);
//     put_number(party_commanders[entities[id].id].fort,2,x,y+2);
//   }
// }

void display_level(char x, char y)
{
  // char id;
  // id = battle_grid[graph_from_x_y(sx,sy)]-1;
  // if(id >= 0)
  // {
  //   put_string("LV",x,y);
  //   put_number(party_commanders[entities[id].id].level,2,x+2,y);
  // }
  // else
  // {
  //   put_string("    ",x,y);
  // }
}

void display_unit_menu_mask(int x, int y)
{
  char id;
  id = battle_grid[graph_from_x_y(sx,sy)];
  if(id)
  {
    set_menu_mask(id-1);
    put_hex(menu_mask,3,x,y);
  }
  else
  {
    put_hex(0,3,x,y);
  }
}

char begin_battle(unsigned char attacker, unsigned char target, unsigned char range, char a_terrain, char t_terrain)
{
  int resolution;

  // psgFadeOut(20);
  reset_satb();
  vsync();

  resolution = battle_loop(attacker,target,range,a_terrain,t_terrain);
    // resolution = battle_loop(attacker,target,range,a_terrain,t_terrain,LIGHTENING_ART,target);

  if(resolution == 0)//attacking team kills army
  { 
    destroy_entity(target);
  }
  if(resolution == 1)//targeted team kills army
  {
    destroy_entity(attacker);
  }
  
  load_battlefield_map();
  load_sprz();
  update_map();
  load_ents();
  disp_on();

  selector_mode = 0;

  vsync();

  return resolution;
}

void cleanup_battlefield()
{
  int i, j, z;

  for(i=0; i<352; i++)
  {
    battle_grid[i] = 0;
  }
  for(i=0; i<MAX_ENTITIES; i++)
  {
    entities[i].team = 0;
    entities[i].id = 0;
    entities[i].pos = 0;
    entities[i].team = 0;
    entities[i].bg = 0;
  }
  terrain_item_count = 0;
  num_of_entities = 0;
  for(i=0; i<64; i++)
  {
    spr_hide(i);
  }
  satb_update();
  one_total = 0;
  two_total = 0;
  no_of_player_cmdrs = 0;
  s_x = s_x_holder;
  s_y = s_y_holder;
  yOffset = y_off_holder;
  selected_entity = 0;
//  clear_battle_items();
  reset_satb();
}
