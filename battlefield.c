// #include "battlefield.h"
#include "battle_ai.c"

#incpal(bluepal,"map/sprites/bluetiles.pcx")
#incpal(redpal,"map/sprites/redtiles.pcx")
#incpal(greypal,"assets/faded_pal.pcx")

#define MENU_ATK_X 152
#define MENU_ATK_Y 16

int map_no;
int id;
int s_x_holder, s_y_holder,y_off_holder;
enum Direction cmdr_direction;
char exit_battlefield;
char t = 0;

//variables for update map routine
int squares, index, sprite_counter, entity_no;
char s;
//

void begin_battlefield(char map_id)
{
  char i;
  current_turn = 1;
  menu_mask = 0;
  map_no = map_id;
  map_offset = (464*map_no)-32;
  objective_pos = battle_map_metadata.event_positions[0];
  if(map_id == 1 || map_id == 2)
  {
    add_commander_to_party(name2,KING);
    // add_commander_to_party(name3,TINKER);
  }

  yOffset = 0;
  init_battlefield();

  selector_mode = 0;

  for(i=0; i<12; i++)
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
  int item_index;
  int position;
  gold_gained = 75;
  last_command = SELECT_MODE;
  selected_option = -1;

  // objective_pos = 200;
  // load_coords(0);

//  create_terrain_item(RED_CRYSTAL,8,9);
//  create_terrain_item(GREEN_CRYSTAL,13,10);
//  create_terrain_item(BLUE_CRYSTAL,6,5);

  s_x_relative = (s_x/8);
  s_y_relative = (s_y/8);

  disp_on();
  satb_update();
  vsync();
  turn = PLAYER;
  psgPlay(0);
  while(exit_battlefield)
  {
    if(turn == CPU)
    {
      ai();
    }
    else
    {
      position = graph_from_x_y(sx,sy);
      display_terrain_bonus(position);
      id = battle_grid[position];
      // display_position(14,1);
      display_id(id-1,13,1);
      swap_water_tiles();
      if(selector_mode != ARMY_MODE){ cycle_terrain_items(); }
      ctrls();
      cycle_animations();
      check_battle_complete();
      put_number(current_turn,2,3,1);
    }
    satb_update();
    vsync();
  }

  cleanup_battlefield();
  return 0;
  //whoops?
}

void battle_start()
{
  char i;
  selector_mode = 0;
  sx = 32;
  sy = 32;
  one_total = no_of_player_cmdrs;
  two_total = cpu_cmdr_count;

  reset_satb();
  init_units();
  draw_selector();
  update_map();
  // story(map_no,INBATTLE,0);
  load_battlefield_map();

  load_sprites_();
}

void check_battle_complete()
{
  if(battle_grid[objective_pos] != 0 && map_type == 2)
  {
    if(entities[battle_grid[objective_pos]-1].team == PLAYER)
    {
      win_condition();
      return;
    }
  }

  if(battle_grid[objective_pos] != 0 && map_type == 1)
  {
    if(entities[battle_grid[objective_pos]-1].team == CPU)
    {
      lose_condition();
      return;
    }
  }

  if(one_total > 0 && two_total > 0)
  {
    return;
  }

  if(one_total <= 0)
  {
    lose_condition();
    return;
  }

  win_condition();
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
    // //remove npc enemy group killed
    // selector_mode = EXPLORE_MODE;
  // }
  //becaue if the battle is over and we're at a castle, we just leave the battlefield
  // else if(battle_map_metadata[b_map_id].map_type == CASTLE)
  // {
    // post_battle_dialog();
  // }
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
  satb_update();
  vsync();
  exit_battlefield = 0;
  s_x = s_x_holder;
  s_y = s_y_holder;
  post_battle_dialog();
}

void lose_condition()
{
  satb_update();
  vsync();
  game_over = 0;
  exit_battlefield = 0;
  s_x = s_x_holder;
  s_y = s_y_holder;
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
  int i;
  for(i=0; i<party_size; i++)
  {
    load_group(i,PLAYER,battle_map_metadata.player_start_pos+i,party_commanders);//commander + max_army_size
  }

  for(i=0; i<cpu_cmdr_count; i++)
  {
    load_predefined_group_layout((char)battle_map_metadata.cpu_commander_ids[i],i);
    load_group(i,CPU,battle_map_metadata.cpu_start_pos+i,enemy_commanders);
  }
}

void load_group(char id, char team, int positions[7], struct Commander *commanders)
{
  add_entity(team,UNIT_PALS[commanders[id].sprite_type]+(team-1),id,positions[0],commanders);
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
            UNIT_PALS[party_commanders[entities[i].id].sprite_type]+(entities[i].team-1));
    }
    else
    {
          add_npc(entities[i].pos%16,entities[i].pos/16,
            party_commanders[entities[i].id].sprite_type,
            UNIT_PALS[party_commanders[entities[i].id].sprite_type]+(entities[i].team-1));
    }
  }
}

void init_battlefield()
{
  selector_mode = 0;
  num_of_entities = 0;
  sx = 80;
  sy = 48;
  cursor_x = -16;
  cursor_y = -16;
  menu_option = 0;

  exit_battlefield = 1;
  s_x_holder = s_x;
  s_y_holder = s_y;
  s_x = 0;
  s_y = 0;
  // no_of_npcs = 0;

  spr_set(SELECTOR);
  spr_y(-16);
  load_vram(0x68C0,selector,0x40);
//  load_vram(0x6900,cursor,0x40);
  load_battlefield_map();
}

void load_battlefield_map()
{
  set_screen_size(SCR_SIZE_32x64);
  // spr_make();
  scroll(1,0,0,0,32,0x80);
  scroll(0,s_x,s_y+32,32,224,0xC0);

  load_palette(0,overworldpal,7);
  load_palette(10,fontpal,2);
  set_font_pal(10);
	load_font(font,125,0x4800);
  load_vram(0x49A0,icons_gfx,0x60);

  load_palette(8,borderspal,1);
  load_palette(12,bluepal,1);
  load_palette(13,redpal,1);
  load_palette(31,greypal,1);
  load_terrains();

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
  menu_mask = 0x00;
  print_menu();
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
void display_terrain_bonus(int id)
{
  char t_type;
  t_type = terrain_type(battlefieldbat[map_offset+id]);
  put_terrain_icon(t_type,1,1);
  put_terrain_bonus(t_type,3,2);
  // put_terrain_atk_stat(t_type,3,2);
}

void load_sprites_() //load default sprites
{
  load_palette(12,bluepal,1);
  load_palette(13,redpal,1);

  load_vram(0x68C0,selector,0x40);
  load_palette(17,selectorpal,1);
//  load_vram(0x6900,cursor,0x40);

  display_selector();
}

void display_selector() //info sprites
{
  spr_make(0,sx,sy,0x68C0,0,NO_FLIP|SZ_16x16,17,1);
//  spr_make(1,cursor_x,cursor_y,0x6900,FLIP_MAS|SIZE_MAS,NO_FLIP|SZ_16x16,29,1);
}

void update_map()
{
  // int squares, i, sprite_counter, entity_no;
  // char s;
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
      if(entities[entity_no].actionable == 0)
      {
        spr_pal(31);
      }
    }
  }
  for(index=5; index<sprite_counter+1; index++)
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
  unsigned char j;
  int abs;
  j = joytrg(0);

  if(j & JOY_UP)
  {
    if(selector_mode == SELECT_MODE || selector_mode == PLACE_MODE || selector_mode == ATTACK_MODE || selector_mode == ATTACK_WITH_ART)
    {
      selector_up();
    }
    else if(selector_mode == MENU_MODE || selector_mode == ACTION_MODE || selector_mode == ARMY_MODE)
    {
      cursor_up();
    }
  }

  if(j & JOY_DOWN)
  {
    if(selector_mode == SELECT_MODE || selector_mode == PLACE_MODE || selector_mode == ATTACK_MODE || selector_mode == ATTACK_WITH_ART)
    {
      selector_down();
    }
    else if(selector_mode == MENU_MODE || selector_mode == ACTION_MODE || selector_mode == ARMY_MODE)
    {
      cursor_down();
    }
  }

  if(j & JOY_RIGHT)
  {
    if(selector_mode == SELECT_MODE || selector_mode == PLACE_MODE || selector_mode == ATTACK_MODE || selector_mode == ATTACK_WITH_ART)
    {
      selector_right();
    }
    else if(selector_mode == MENU_MODE || selector_mode == ACTION_MODE)
    {
      cursor_right();
    }
    else if(selector_mode == ARMY_MODE)
    {
      if(menu_option >= ((menu_rows*menu_columns)-menu_rows))
      {
        return;
      }
      menu_option += menu_rows;
      curs_right(5);
    }
  }

  if(j & JOY_LEFT)
  {
    if(selector_mode == SELECT_MODE || selector_mode == PLACE_MODE || selector_mode == ATTACK_MODE || selector_mode == ATTACK_WITH_ART)
    {
      selector_left();
    }
    else if(selector_mode == MENU_MODE || selector_mode == ACTION_MODE)
    {
      cursor_left();
    }
    else if(selector_mode == ARMY_MODE)
    {
      if(menu_option < menu_rows)
      {
        return;
      }
      menu_option -= menu_rows;
      curs_left(5);
    }
  }

  abs = graph_from_x_y(sx,sy);

  if(j & JOY_I)
  {
    if(selector_mode == SELECT_MODE)
    {
      if(battle_grid[abs] != 0)
      {
        if(entities[battle_grid[abs]-1].team == PLAYER)
        {
          last_command = selector_mode;
          id = battle_grid[abs];
          selector_mode = MENU_MODE;
          select_unit(id-1);
          set_menu_mask(id-1);
          get_unit_radius(abs,get_army_min_move(selected_entity->id),entities[id-1].team,1);
          display_menu(152,8);
        }
      }
    }
    else if(selector_mode == PLACE_MODE)
    {
      if(battle_grid[abs] == 0 && (is_traversable(abs)) && valid_map_square(abs))
      {
        remove_cursor();
        last_command = selector_mode;
        move_unit_new(abs);

        selector_mode = ACTION_MODE;

        set_menu_mask(battle_grid[abs]-1);
        display_menu(MENU_ATK_X,MENU_ATK_Y);
        draw_selector();

        unhighlight();
      }
    }
    else if(selector_mode == ATTACK_MODE)
    {
      if(battle_grid[abs] != 0 && valid_map_square(abs))
      {
        attack_unit(abs,selected_entity->pos,0);
      }
    }
    else if(selector_mode == ATTACK_WITH_ART)
    {
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
            get_unit_radius(selected_entity->pos,get_army_max_range(selected_entity->id),selected_entity->team,0);
            highlight(selected_entity->pos,0xD000);
          }
          break;

        case MENU_MOVE:
          if(menu_mask & 0x1)
          {
            hide_menu();
            display_move_range(selected_entity->pos);
          }
          break;

        case MENU_END:
          if(menu_mask & 0x4)
          {
            end_unit_turn(selected_entity->id);
          }
          break;

        case MENU_TURN:
          selector_mode = SELECT_MODE;
          menu_option = MENU_ATTACK;
          last_pos = 0;
          hide_cursor();
          start_turn(CPU);
          menu_mask = 0x00;
          print_menu();
          hide_cursor();
          break;

        case MENU_GRP:
          remove_cursor();
          scroll(0,0,s_y+32,32,224,0x80);
          display_abs_info_panel(7+(s_x),32+(s_y/2),18,14);
          cursor_x = 8;
          cursor_y = 11 + (s_y/8);
          display_cursor();
          selector_mode = ARMY_MODE;
          menu_option = 0;
          menu_rows = 3;
          menu_columns = 3;
          menu_vert_size = 4;
          list_commander_army(entities[id-1].bg,9,10);
          break;

        case MENU_TAKE://calling
          display_calling_background(0,s_y+32,selected_entity->id);
          set_calling(selected_entity->bg->calling_stone,selected_entity->team);
          animate_calling(calling_stones[selected_entity->bg->calling_stone].effect,PLAYER);
          display_selector();
          hide_menu();
          selector_mode = SELECT_MODE;
          load_palette(12,bluepal,1);
          break;
      }
    }
    else if(selector_mode == ARMY_MODE)
    {
      if(selected_option == -1)
      {
        set_font_pal(11);
        selected_option = menu_option;
        print_unit_info(&entities[id-1].bg->units[menu_option].unit,9+((menu_option/3)*5)+(s_x/8),10+((menu_option%3)*4)+(s_y/8));
        set_font_pal(10);
      }
      else
      {
        swap_commander_units(entities[id-1].id,selected_option,menu_option);
        list_commander_army(entities[id-1].bg,9,10);
        selected_option = -1;
      }
    }
  }

  if(j & JOY_II)
  {
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
      display_menu(152,8);
      unhighlight();
      get_unit_radius(selected_entity->pos,3,selected_entity->team,1);
    }
    else if(selector_mode == ACTION_MODE)
    {
      undo();
      selector_mode = PLACE_MODE;
    }
    else if(selector_mode == ARMY_MODE)
    {
      if(selected_option == -1) //haven't selected anything yet
      {
        remove_cursor();
        load_map(0,2,0,0,16,29);
        scroll(0,0,s_y+32,32,224,0xC0);
        selector_mode = SELECT_MODE;
      }
      else //unselect
      {
        print_unit_info(&entities[id-1].bg->units[selected_option].unit,10+((selected_option/3)*4),9+((selected_option%3)*4));
        selected_option = -1;
      }
    }
  }

  if(j & JOY_RUN)
  {
    // display_calling_background();
    // animate_calling(EFFECT_HEAL,PLAYER);
    // remove_effect(0);
    // display_selector();
    // draw_selector();
    // put_number(selected_entity->id,4,0,0);
//    create_terrain_item(RED_CRYSTAL,8,9);
//    create_terrain_item(GREEN_CRYSTAL,13,10);
//    create_terrain_item(BLUE_CRYSTAL,6,5);
  }

  if(j & JOY_SEL)
  {
//    put_number((terrain_items[1].y<<4)+terrain_items[1].x,3,0,0);
//    put_number(item_at_position(graph_from_x_y(sx,sy)),3,0,0);
//    remove_terrain_item(2);
//    put_tile(44,13,10);
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
      y = (entities[i].pos>>4)<<4;
      create_art_by_type(effect,x,y,0);
      animate_calling_effect(x,y);
      // remove_effect(0);
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
  sx = (pos % 16) * 16;
  sy = (((pos+yOffset) / 16) * 16);
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
  menu_option = y/12;
  cursor_x = x/8;
  cursor_y = y/8;

  print_menu();
  display_cursor();
  scroll(1,0,0,0,32,0x80);
}

void highlight(int square, int pal)
{
  int i;
  for(i=0; i<map_size+1; i++)
  {
    change_background_palette(map[i].ownPos,pal);
  }
}

void get_unit_radius(int square, int depth, char team, char ignore)
{
  get_path(square,999,path,battle_grid,team,depth,ignore);
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

// void pickup_item(int entity_id)
// {
//   char item_index;
//   item_index = item_at_position(entities[entity_id].pos);
//   item_gained_text(terrain_items[item_index].item_no);
//   remove_terrain_item(item_index);
// }

void display_camera_info()
{
  put_string("sy:",5,1);
  put_string("s_y:",5,2);
  put_string("yOff:",5,3);
  put_number(sy,3,8,1);
  put_number(s_y,3,9,2);
  put_number(yOffset,3,10,3);
}

void display_id(char id, int x, int y)
{
  if(id >= 0)
  {
    // put_number(entities[id].id,3,6,1);
    put_char('P',x,y);
    put_number(calculate_power(entities[id].id),3,x+1,y);
    put_char('M',x,y+1);
    put_number(0,3,x+1,y+1);
  }
  else
  {
    put_string("    ",x,y);
    put_string("    ",x,y+1);
  }
}

void display_position(int x, int y)
{
  put_number(graph_from_x_y(sx,sy),3,x,y);
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

char begin_battle(int attacker, int target, char range, char a_terrain, char t_terrain, char art)
{
  int i, battle_result, resolution;

  psgFadeOut(20);
  reset_satb();
  vsync();

  if(arts[art].target)//if targeting enemy
  {
    resolution = battle_loop(attacker,target,range,a_terrain,t_terrain,0,target);
  }
  else
  {
    resolution = battle_loop(attacker,target,range,a_terrain,t_terrain,0,attacker);
  }
    // resolution = battle_loop(attacker,target,range,a_terrain,t_terrain,LIGHTENING_ART,target);

  if(resolution == 0)//attacking team kills army
  {
    destroy_entity(target);
  }
  if(resolution == 1)//targeted team kills army
  {
    destroy_entity(attacker);
  }

  battle_result = -1;
  load_ents();

  load_battlefield_map();
  load_sprites_();
  update_map();
  disp_on();
  selector_mode = 0;
  vsync();

  psgPlay(0);
  return resolution;
}

void cleanup_battlefield()
{
  int i, j, z;

  for(i=0; i<464; i++)//MAP_SIZE; i++)
  {
    battle_grid[i] = 0;
  }
  for(i=0; i<MAX_ENTITIES; i++)
  {
    // entities[i].army_size = 0;
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
