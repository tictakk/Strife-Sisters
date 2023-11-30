#include "battle_ai.c"

#incpal(bluepal,"map/sprites/bluetiles.pcx")
#incpal(redpal,"map/sprites/redtiles.pcx")
#incpal(greenpal,"map/sprites/greentiles.pcx")
#incpal(greypal,"assets/faded_pal.pcx")

#define MENU_ATK_X 152
#define MENU_ATK_Y 16
#define MENU_GROUP_X 216
#define MENU_GROUP_Y 8
#define MENU_DEFEND_X 216
#define MENU_DEFEND_Y 16
#define MENU_TACTIC_X 184
#define MENU_TACTIC_Y 8
#define WAIT_TIME 8

const char dumb_table[9] = {0,3,6,1,4,7,2,5,8};

char id;
int s_x_holder, s_y_holder, y_off_holder;
char exit_battlefield;
char t = 0;

char joy_left_held = 0;
char joy_right_held = 0;
char joy_down_held = 0;
char joy_up_held = 0;

char debug_info = 0;

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
  map_boundry_y = (map_y << 4) - 192; //would be 224 (screen size but 32 px are taken up by menu; 224-32 = 192)
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
  last_command = SELECT_MODE;
  map_result_status = MAP_RESULT_NONE;
  turns_count = 0;
  units_killed = 0;
  units_lost = 0;

  s_x_relative = 0;//(s_x/8);
  s_y_relative = 0;//(s_y/8);
  
  disp_on();
  satb_update();
  vsync();
  turn = PLAYER;
  select_unit(0);
  if(demo_flag)
  {
    play_story();
    psgPlay(3);
  }
  display_selector(SELECTOR,sx,sy,16);
  while(map_result_status == MAP_RESULT_NONE)
  {
    if(turn == CPU)
    {
      ai();
      check_battle_complete();
    }
    else
    {
      // put_number(map_type,3,0,0);
      g_abs = graph_from_x_y(sx,sy);
      t_type = terrain_type(tutorial_1[map_offset+g_abs]);
      id = battle_grid[g_abs];
      // put_number(id,4,0,0);
      display_position(14,1);
      display_bonuses(4,1);
      
      display_terrain_bonus();
      swap_water_tiles();

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

void battle_start()
{
  char i;
  selector_mode = 0;
  one_total = no_of_player_cmdrs;
  two_total = cpu_cmdr_count;

  reset_satb();
  init_units();

  update_map();
  load_battlefield_map();

  sx = (entities[0].pos&15) << 4;
  sy = (entities[0].pos>>4) << 4;
  load_sprz();
}

void check_battle_complete()
{
  // return;
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
  char has_cmdr;
  
  for(i=0; i<party_size; i++)
  {
    heal_commander_bg(i);
    load_group(i,PLAYER,battle_map_metadata.player_start_pos+i,party_commanders,1);//commander + max_army_size
  }

  //TODO: ADD IS_CMDR CHECK AND ADD A NEW PARAM TO ADD_ENTITY
  for(i=0; i<cpu_cmdr_count; i++)
  {
    has_cmdr = 0;
    load_predefined_group_layout(&battle_map_metadata.cpu_commander_ids[i*10],
    i+MAX_PARTY_COMMANDERS,(char)map_no);
    load_group(i,CPU,battle_map_metadata.cpu_start_pos+i,enemy_commanders,has_cmdr);
  }
}

void load_group(char id, char team, int positions[7], struct Commander *commanders, char has_cmdr)
{
  add_entity(team,id,positions[0],commanders,has_cmdr);
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
            CMDR_PALETTE);
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
      if(party_commanders[entities[i].id].sprite_type == DEMON_UNIT)
      {
        add_npc(entities[i].pos%16,entities[i].pos/16,
          party_commanders[entities[i].id].sprite_type,
          ENEMY_PALETTE+1);
      }
      else
      {
        add_npc(entities[i].pos%16,entities[i].pos/16,
          party_commanders[entities[i].id].sprite_type,
          ENEMY_PALETTE);
      }
          // UNIT_PALS[party_commanders[entities[i].id].sprite_type]);
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
  // load_battlefield_map();
}

void load_battlefield_map()
{
  set_screen_size(SCR_SIZE_32x64);
  load_terrains();
  // set_map_data(battlefieldbat+(320*map_no),16,20);
  set_map_data(tutorial_1+(320*map_no),16,20);
  set_tile_data(overworldtiles,162,overworldtilespal,TILE_WIDTH);

  load_tile(TILE_VRAM);
  load_map(0,2,0,0,16,20);

  disp_on();
  scroll(1,0,0,0,32,0x80);
  scroll(0,s_x,s_y+32,32,224,0xC0);
  
  load_palette(0,overworldpal,8);
  load_palette(10,fontpal,2);
  set_font_pal(10);
	// load_font(font,125,0x4800);
	// load_vram(0x4BB0,icons_gfx,0x60);
	// load_vram(0x49A0,icons_gfx+0x60,0x50);
  load_vram(0x4DF0,icons_gfx+0xC0,0x10);
  
  load_palette(9,borderspal,1);
  load_palette(12,bluepal,1);
  load_palette(13,redpal,1);
  load_palette(31,greypal,1);

  // set_screen_size(SCR_SIZE_32x64);
  screen_dimensions = 32;
  
  display_window_abs(0,0,18,4);
  display_window_abs(18,0,14,4);
  menu_mask = 0x00;
  print_menu();
}


void display_terrain_bonus()
{
  // terrain_type =  terrain_type(battlefieldbat[map_offset+id]);
  put_terrain_icon(t_type,1,1);
  // if(id-1 >= 0)
  // {
    // if(entities[id-1].has_cmdr)
    // {
    //   put_number(entities[id-1].tactic_meter,3,3,2);
    // }
  // }
  // else
  // {
  //   put_string("   ",3,2);
  // }
  // put_terrain_bonus(t_type,3,2);
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
      else if(selector_mode == MENU_MODE || selector_mode == RUN_MENU_MODE || selector_mode == ACTION_MODE)
      {
        cursor_up();
      }
      else if(selector_mode == PLAYER_ARMY_MODE)
      {
        cursor_up();
        update_unit_battle_info(entities[id-1].bg->units[dumb_table[menu_option]],20,9+(s_y/8));
      }
      else if(selector_mode == TACTIC_SELECT_MODE)
      {
        if(tactic_current == TACTIC_SWITCH || valid_map_square(g_abs-16))
        {
          selector_up();
        }
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
      else if(selector_mode == MENU_MODE || selector_mode == RUN_MENU_MODE || selector_mode == ACTION_MODE)
      {
        cursor_down();
      }
      else if(selector_mode == PLAYER_ARMY_MODE)
      {
        cursor_down();
        update_unit_battle_info(entities[id-1].bg->units[dumb_table[menu_option]],20,9+(s_y/8));  
      }
      else if(selector_mode == TACTIC_SELECT_MODE)
      {
        if(tactic_current == TACTIC_SWITCH || valid_map_square(g_abs+16))
        {
          selector_down();
        }
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
        update_unit_battle_info(entities[id-1].bg->units[dumb_table[menu_option]],20,9+(s_y/8));
        curs_right(5);
      }
      else if(selector_mode == TACTIC_SELECT_MODE)
      {
        if(tactic_current == TACTIC_SWITCH || valid_map_square(g_abs+1))
        {
          selector_right();
        }
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
        update_unit_battle_info(entities[id-1].bg->units[dumb_table[menu_option]],20,9+(s_y/8));
        curs_left(5);
      }
      else if(selector_mode == TACTIC_SELECT_MODE)
      {
        if(tactic_current == TACTIC_SWITCH || valid_map_square(g_abs-1))
        {
          selector_left();
        }
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

        case MENU_DEFEND: //MENU_DEFEND
          if(menu_mask & MASK_DEFEND)
          {
            selected_entity->defend = 1;
            hide_cursor();
            end_unit_turn(get_entity_id(selected_entity->pos));
          }
          break;

        case MENU_SQUAD:
          if(menu_mask & MASK_SQUAD)
          {
            remove_cursor();
            scroll(0,0,s_y+32,32,224,0x80);
            display_window_rel(2,8+(s_y/8),18,15);
            set_font_pal(9);
            put_string(" Front ",8,8+(s_y/8));
            put_string(" Rear  ",8,22+(s_y/8));
            set_font_pal(10);
            // selector_mode = CPU_ARMY_MODE;
            // if(entities[id-1].team == PLAYER)
            // {
              cursor_x = 3;
              cursor_y = 11 + (s_y/8);
              display_cursor();
              selector_mode = PLAYER_ARMY_MODE;
              menu_option = 0;
              menu_rows = 3;
              menu_columns = 3;
              menu_vert_size = 4;
            // }
            list_commander_army(entities[id-1].bg,4,10);
            display_unit_battle_info(entities[id-1].bg->units[0],20,9+(s_y/8));
          }
          break;

        case MENU_TAKE://calling
          // if(menu_mask & MASK_CALLING)
          // {
            setup_tactic();
          // }
          break;
      }
    }
    else if(selector_mode == RUN_MENU_MODE)
    {
      if(menu_option == 0)
      {
        hide_cursor();
        end_player_turn();
        return;
      }
      map_result_status = MAP_RESULT_LOSE;
    }
    else if(selector_mode == TACTIC_SELECT_MODE)
    {
      perform_tactic();
    }
  }

  if(jy & JOY_II)
  {
    if(selector_mode == TACTIC_SELECT_MODE)
    {
      selector_mode = MENU_MODE;
      menu_option = MENU_TAKE;
      set_cursor_pos(last_pos);
      display_menu(MENU_TACTIC_X,MENU_TACTIC_Y);
      unhighlight();
      get_unit_radius(last_pos,get_army_min_move(tactic_caster),entities[tactic_caster].team,get_army_min_move(tactic_caster));
    }
    else if(selector_mode == PLACE_MODE)
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
      remove_cursor();
      load_map(0,2,0,0,16,29);
      scroll(0,0,s_y+32,32,224,0xC0);
      selector_mode = MENU_MODE;
      set_menu_mask(id-1);
      display_menu(MENU_GROUP_X,MENU_GROUP_Y);
    }
    else if(selector_mode == RUN_MENU_MODE)
    {
      remove_cursor();
      selector_mode = SELECT_MODE;
      menu_mask = 0;
      print_menu();
    }
  }

  if(j & JOY_RUN)
  {
    if(selector_mode == SELECT_MODE)
    {
      last_command = selector_mode;
      selector_mode = RUN_MENU_MODE;
      selected_entity = 0;
      menu_mask = MASK_DEFEND;
      display_run_menu(152,8);
      // display_menu(MENU_TURN_X,MENU_TURN_Y);
    }
    // win_condition();
  }

  if(j & JOY_SEL)
  {
    // put_hex(battle_grid,6,0,0);
    // if(id > 0 && entities[id-1].has_cmdr)
    // {
    //   entities[id-1].tactic_meter = MAX_TACTIC_METER; 
    // }
    if(!demo_flag)
    {
      map_result_status = MAP_RESULT_WIN;
    }
    // map_result_status = MAP_RESULT_LOSE;
  }
}

void damage_group(char target_id, char attacker_id)
{
  unsigned char dmg;
  char i, level;

  load_unit_header(party_commanders[entities[attacker_id].id].sprite_type,0);
  for(i=0; i<MAX_ARMY_SIZE; i++)
  {
    if(party_commanders[entities[attacker_id].id].sprite_type == entities[attacker_id].bg->units[i].id)
    {
      level = entities[attacker_id].bg->units[i].level;
      apply_level_to_header(level,0);
      break;
    }
  }

  for(i=0; i<MAX_ARMY_SIZE; i++)
  {
    if(entities[target_id].bg->units[i].hp)
    {
      load_unit_header(entities[target_id].bg->units[i].id,1);
      apply_level_to_header(entities[target_id].bg->units[i].level,1);
      dmg = calc_damage(0,
                        0,
                        level,
                        unit_header[0].intel,
                        0,
                        0,
                        unit_header[1].res,
                        20); //TODO: m_power should vary based on move
      entities[target_id].bg->units[i].hp = max(entities[target_id].bg->units[i].hp - dmg, 1);
    }
  } 
}

void perform_tactic()
{
  if(is_valid_tactic_tile())
  {
    selector_mode = SELECT_MODE;
    remove_cursor();
    unhighlight();
    
    if(!get_tactic_perform_status())
    {
      end_unit_turn(tactic_caster);
      // entities[tactic_caster].actionable = 0;
      // entities[tactic_caster].movable = 0;
    }
    switch(tactic_current)
    {
      case TACTIC_SCORCH: scorch_tactic(); break;
    
      case TACTIC_LEAP: leap_tactic(); break;
    
      case TACTIC_RAGE: tactic_rage(); break;

      case TACTIC_DASH: dash_tactic(); break;

      case TACTIC_SWITCH: switch_tactic(); break;
    }
    menu_mask = 0x00;
    print_menu();
    entities[tactic_caster].tactic_meter = 0;
    sync(30);
    display_selector(SELECTOR,sx,sy,16);
  }
}

void switch_tactic()
{
  int tmp_pos;
  char tmp_id;
  
  create_art_by_type(EFFECT_POOF,(g_abs&15)<<4,((g_abs>>4)<<4)-s_y,0);
  create_art_by_type(EFFECT_POOF,(selected_entity->pos&15)<<4,((selected_entity->pos>>4)<<4)-s_y,0);

  animate_calling_effect(0,0);
  remove_effects();

  tmp_id = id;
  tmp_pos = selected_entity->pos;

  battle_grid[g_abs] = battle_grid[selected_entity->pos];
  selected_entity->pos = g_abs;
  
  battle_grid[tmp_pos] = tmp_id;
  entities[tmp_id-1].pos = tmp_pos;

  display_selector(SELECTOR,sx,sy,16);
  entities[tmp_id-1].actionable = 1;
  entities[tmp_id-1].movable = 1;
  tactic_current = 0;
  selected_entity = 0;
}

void tactic_rage()
{
  //rage doesn't really do anything here innit?
  animate_calling_single(EFFECT_POWER_UP,id-1);
  animate_calling_single(EFFECT_POWER_UP,id-1);
  tactic_current = TACTIC_RAGE;
  remove_effects();
}

void scorch_tactic()
{
  animate_calling_single(EFFECT_FIRE,id-1);
  damage_group(id-1,tactic_caster);
  shake_entity(id-1);
  tactic_current = 0;
}

void dash_tactic()
{
  char i, size, dashed_id;

  size = get_path(entities[tactic_caster],g_abs,path,battle_grid,PLAYER,999,0);
  walk_entity(tactic_caster,g_abs);

  for(i=0; i<size+1; i++)
  {
    dashed_id = battle_grid[path[i]];
    if(dashed_id && entities[dashed_id-1].team != entities[tactic_caster].team)
    { 
      damage_group(dashed_id-1,tactic_caster);
      shake_entity(dashed_id-1);
    }
  }

  display_selector(SELECTOR,sx,sy,16);
  // tactic_current = 0;
}

void leap_tactic()
{
  int end_x, end_y;

  end_x = (g_abs&15)<<4;
  end_y = ((g_abs>>4)<<4)-s_y;

  walk_entity(tactic_caster,g_abs);
  create_art_by_type(EFFECT_POOF,end_x+16,end_y,0);
  create_art_by_type(EFFECT_POOF,end_x-16,end_y,0);
  create_art_by_type(EFFECT_POOF,end_x,end_y+16,0);
  create_art_by_type(EFFECT_POOF,end_x,end_y-16,0);

  animate_calling_effect(0,0);
  remove_effects();

  determine_map_target(CPU,1);
  while(tactic_target_count > 0)
  {
    damage_group(target_target_ids[--tactic_target_count],tactic_caster);
    shake_entity(target_target_ids[tactic_target_count]);
  }

  display_selector(SELECTOR,sx,sy,16);
  tactic_current = 0;
}

void shake_entity(char ent_id)
{
  char i, id;
  id = get_entity_sprite_no(ent_id);
  spr_set(id);
  for(i=0; i<16; i++)
  {
    if(i%2 == 0)
    {
      spr_x(spr_get_x()+1);
    }
    else
    {
      spr_x(spr_get_x()-1);
    }
    satb_update();
    vsync();
  }
}

void setup_tactic()
{
  last_pos = g_abs;
  set_tactic(id-1,party_commanders[entities[id-1].id].tactic_id);
  // if(tactic_distance())
  // {
  if(tactic_current == TACTIC_SWITCH)
  {
    char i;
    for(i=0; i<num_of_entities; i++)
    {
      if(entities[i].team == PLAYER && party_commanders[entities[i].id].tactic_id != tactic_current)
      {
        change_background_palette(entities[i].pos,0xC000);
        target_target_ids[i] = i;
      }
    }
    selector_mode = TACTIC_SELECT_MODE;
    return;
  }
  if(tactic_current == TACTIC_DASH)
  {
    get_dash_radius();
  }
  else
  {
    get_unit_radius(entities[id-1].pos,tactic_distance(),PLAYER,0);
  }
  highlight(entities[id-1].pos,ACTION_ATTACK);
  selector_mode = TACTIC_SELECT_MODE;
  // }
}

void animate_calling_single(char effect, char target)
{
  int x, y;

  x = (entities[target].pos&15)<<4;
  y = ((entities[target].pos>>4)<<4)-s_y;
  create_art_by_type(effect,x,y,0);
  animate_calling_effect(x,y);
  remove_effect(0);
}

void animate_calling_effect(int x, int y)
{
  char i;
  spr_show();
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

void display_run_menu(int x, int y)
{
  menu_vert_size = 1;
  menu_columns = 1;
  menu_rows = 2;
  menu_option = y/12 + ((x-152)/16);
  cursor_x = x/8;
  cursor_y = y/8;

  display_window_abs(18,0,14,4);
  print_run_menu();
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

void display_position(int x, int y)
{
  put_number(g_abs,3,x,y);
  // if(battle_grid[g_abs])
  // {
    // put_number(entities[battle_grid[g_abs]-1].id,4,0,0);
  // }
}

void display_bonuses(char x, char y)
{
  char i, j, bonus, bonuses;
  put_string("    ",x,y);
  
  if(id>0)
  {
    j=0;
    i=0;
    bonuses = entities[id-1].bg->bonuses;
    while(bonuses)
    {
      bonus = 1 << i++;
      if(bonus & bonuses)
      {
        print_unit_attack_icon(bonus,x+(j++),y);
        bonuses ^= bonus;
      }
    }
    if(entities[id-1].defend)
    {
      put_char('D',x+3,y);
    }
  }
}

char attack_unit(int src, int dst, char art)
{
  char result;
  unsigned char attacker, target, range;
  
  attacker = battle_grid[dst]-1;
  target = battle_grid[src]-1;
  range = (unsigned char) get_range_from_distance(dst,src);
  result = NO_DEFEAT;
  if(entities[attacker].team != entities[target].team)
  {
    last_command = selector_mode;
    entities[attacker].actionable = 0;

    hide_menu();
    cursor_x = -32;
    cursor_y = -32;
    selector_mode = SELECT_MODE;
    result = begin_battle(attacker,target,range,tutorial_1[map_offset+dst],terrain_type(tutorial_1[map_offset+src]));//tutorial_1[map_offset+src]);
    if(result == TARGET_DEFEAT && tactic_current == TACTIC_RAGE && attacker == tactic_caster)
    {
      entities[attacker].actionable = 1;
      entities[attacker].movable = 1;
      load_commander_palette(party_commanders[entities[attacker].id].sprite_type);
    }
  }
  return result;
}

char begin_battle(unsigned char attacker, unsigned char target, unsigned char range, char a_terrain, char t_terrain)
{
  int resolution;

  // psgFadeOut(20);
  fade_screen();
  disp_off();
  reset_satb();
  // vsync();

  resolution = battle_loop(attacker,target,range,a_terrain,t_terrain,STANDARD_MODE);
    // resolution = battle_loop(attacker,target,range,a_terrain,t_terrain,LIGHTENING_ART,target);

  if(resolution == TARGET_DEFEAT)//attacking team kills army
  {
    destroy_entity(target);
  }
  if(resolution == ATTACKER_DEFEAT)//targeted team kills army
  {
    destroy_entity(attacker);
  }
  
  load_battlefield_map();
  load_sprz();
  update_map();
  
  load_ents();
  // sync(30);
  vsync();

  selector_mode = 0;
  // vsync();
  return resolution;
}

void cleanup_battlefield()
{
  cleanup_entities();
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

void cleanup_entities()
{
  int i;
  char j, z;

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

  for(j=0; j<MAX_ENEMY_COMMANDERS; j++)
  {
    for(z=0; z<MAX_ARMY_SIZE; z++)
    {
      enemy_commanders[j].bg.units[z].id = 0;
      enemy_commanders[j].bg.units[z].hp = 0;
    }
  }

  num_of_entities = 0;
  for(i=0; i<64; i++)
  {
    spr_hide(i);
  }
}
