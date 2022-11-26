// #include "battlefield.h"
#include "battle_ai.c"

#incpal(bluepal,"map/sprites/bluetiles.pcx")
#incpal(redpal,"map/sprites/redtiles.pcx")

#define MENU_ATK_X 148
#define MENU_ATK_Y 12

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
char water = 0;
char attack_radius_toggle = 0;

void begin_battlefield(char map_id)
{
  char i;
  menu_mask = 0;
  map_no = map_id;
  map_offset = (464*map_no)-32;
  objective_pos = battle_map_metadata.event_positions[0];
  if(map_id == 1 || map_id == 2)
  {
    party[2] = KING;
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
  char collide;
  gold_gained = 75;
  last_command = SELECT_MODE;
  // objective_pos = 200;
  load_coords(0);
  display_sprites_();
  disp_on();
  satb_update();
  vsync();

  event_terrains[0] = 83;
  turn = PLAYER;
  while(exit_battlefield)
  {
    if(turn == CPU)
    {
      ai();
    }
    else
    {
      display_terrain_bonus(graph_from_x_y(sx,sy));
      // display_sprite_type(graph_from_x_y(sx,sy));
      // display_camera_info();
      // id = battle_grid[graph_from_x_y(sx,sy)];
      // if(id)
      // {
        // display_id(id-1,0,0);
      //   id--;
      //   // display_army_size(id,1,2);
      //   display_stats(id,6,2);
      //   display_type(id,4,1);
      // }
      // else
      // {
      //   put_string("  ",0,0);
      //   put_string("   ",3,0);
      //   put_string("   ",3,1);
      //   put_string("   ",3,2);
      //
      //   put_string("   ",7,0);
      //   put_string("   ",7,1);
      //   put_string("   ",7,2);
      //
      //   put_string("   ",11,0);
      //   put_string("   ",11,1);
      //   put_string("   ",11,2);
      // }
      display_position(14,1);
      put_number(check_terrain_triggered(),2,0,0);
      swap_water_tiles();
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
    // spr_x(cmdr_x);
    // spr_y(cmdr_y+yOffset);
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
  
  load_units_by_cmdr_id(10,10);

  for(i=0; i<no_of_player_cmdrs; i++)
  {
    load_group(party[i],party[i]-16,PLAYER,battle_map_metadata.player_start_pos+i);//commander + max_army_size
  }

  for(i=0; i<cpu_cmdr_count; i++)
  {
    load_units_by_cmdr_id((char)battle_map_metadata.cpu_commander_ids[i],11+i);
    load_group((char)battle_map_metadata.cpu_commander_ids[i]+16+i,
                // 20+i,
                11+i,
                CPU,battle_map_metadata.cpu_start_pos+i);
  }
}

void load_group(char cmdr_id, char id, char team, int positions[7])
{
  add_entity(team,UNIT_PALS[commanders[id].sprite_type]+(team-1),cmdr_id,id,positions[0]);
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
            UNIT_PALS[commanders[entities[i].id].sprite_type]+(entities[i].team-1));
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
  cursor_x = -16;
  cursor_y = -16;
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
  load_terrain_icons();

  // set_screen_size(SCR_SIZE_32x64);
  screen_dimensions = 32;

  load_vram(0x6400,borders,0x100);
  load_vram(0x30A0,icons_gfx,0x60);

  // set_map_data(battlefieldbat+(464*map_no),16,29);
  // memcpy(&map_data,&battlefieldbat,464);

  // set_map_data(map_data,16,29);
  set_map_data(battlefieldbat+(464*map_no),16,29);
  set_tile_data(overworldtiles,147,overworldtilespal,TILE_WIDTH);

  load_tile(TILE_VRAM);
  load_map(0,2,0,0,16,29);

  display_abs_info_panel(0,0,18,4);
  display_abs_info_panel(18,0,14,4);
  menu_mask = 0x00;
  print_menu();
}

display_sprite_type(int pos)
{
  char id;
  id = battle_grid[pos];
  if(id)
  {
    put_number(commanders[entities[id-1].id].sprite_type,3,0,0);
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
  put_terrain_def_stat(t_type,3,1);
  put_terrain_atk_stat(t_type,3,2);
}

void load_sprites_() //load default sprites
{
  load_palette(12,bluepal,1);
  load_palette(13,redpal,1);

  load_vram(0x68C0,selector,0x40);
  load_vram(0x6900,cursor,0x40);

  display_sprites_();
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
    if(selector_mode == SELECT_MODE)
    {
      if(battle_grid[abs] != 0)
      {
        if(entities[battle_grid[abs]-1].team == PLAYER)
        {
          last_command = selector_mode;
          unit_selected = abs;
          id = battle_grid[abs];
          selector_mode = MENU_MODE;
          set_menu_mask(id-1);
          display_menu(148,4);
        }
      }
    }
    else if(selector_mode == PLACE_MODE)
    {
      if(battle_grid[abs] == 0 && (is_traversable(abs)) && valid_map_square(abs))//entities[battle_grid[unit_selected]-1].unit->ign))
      {
        last_command = selector_mode;
        move_unit(abs,unit_selected);
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
        attack_unit(abs,unit_selected);
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
          highlight(unit_selected,0xD000,get_army_max_range(battle_grid[unit_selected]-1),0);
        }
        break;

        case MENU_MOVE:
        if(menu_mask & 0x1)
        {
          hide_menu();
          select_unit(unit_selected);
        }
        break;

        case MENU_END:
        if(menu_mask & 0x4)
        {
          selector_mode = SELECT_MODE;
          menu_option = MENU_ATTACK;
          entities[battle_grid[unit_selected]-1].actionable = 0;
          menu_mask = 0x00; 
          print_menu();
          hide_cursor();
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
      }
    }
  }

  if(j & JOY_II)
  {
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
      menu_mask = 0x00;

      print_menu();
    }
    else if(selector_mode == ATTACK_MODE)
    {
      set_cursor_pos(unit_selected);
      selector_mode = last_command;
      display_menu(148,4);
      unhighlight();
    }
    else if(selector_mode == ACTION_MODE)
    {
      undo();
      selector_mode = PLACE_MODE;
    }
  }

  if(j & JOY_RUN)
  {
    char mv = 0, rng = 0, id = 0;
    id = battle_grid[graph_from_x_y(sx,sy)];

    if(id--)
    {
      if(attack_radius_toggle ^= 1)
      {
        mv = get_army_min_move(id);
        rng = get_army_max_range(id);
        highlight(graph_from_x_y(sx,sy),0xD000,mv+rng,mv);
      }
      else
      {
        unhighlight();
      }
    }
  }

  if(j & JOY_SEL)
  {
    two_total = 0;
    // exit_battlefield = 0;
    // put_number(sizeof(),5,0,0);
    // swap_water_tiles();
  }
}

void selector_left()
{
  if(sx > 0)
  {
    // if(selector_mode == PLACE_MODE || selector_mode == ATTACK_MODE)
    // {
    //   if(valid_map_square(graph_from_x_y(sx-16,sy)))
    //   {
    //     update_selector_pos(-16,0);
    //   }
    // }
    // else
    // {
      update_selector_pos(-16,0);
    // }
  }
}

void selector_right()
{
  if(sx < 256 - 16)
  {
    // if(selector_mode == PLACE_MODE || selector_mode == ATTACK_MODE)
    // {
    //   if(valid_map_square(graph_from_x_y(sx+16,sy)))
    //   {
    //     update_selector_pos(16,0);
    //   }
    // }
    // else
    // {
      update_selector_pos(16,0);
    // }
  }
}

void selector_down()
{
  if(s_y <= 240)
  {
    // if(selector_mode == PLACE_MODE || selector_mode == ATTACK_MODE)
    // {
    //   if(valid_map_square(graph_from_x_y(sx,sy+16)))
    //   {
    //     update_selector_pos(0,16);
    //   }
    // }
    // else
    // {
      update_selector_pos(0,16);
    // }
  }
}

void selector_up()
{
  // if(sy > 0)
  // {
    // if(selector_mode == PLACE_MODE || selector_mode == ATTACK_MODE)
    // {
    //   if(valid_map_square(graph_from_x_y(sx,sy-16)))
    //   {
    //     update_selector_pos(0,-16);
    //   }
    // }
    // else
    // {
      update_selector_pos(0,-16);
    // }
  // }
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
  return (x/TILE_SIZE)+((y/TILE_SIZE)*TILE_SIZE) - (yOffset);
}

void display_menu(int x, int y)
{
  menu_option = y/12;
  cursor_x = x;
  cursor_y = y;

  print_menu();
  spr_make(CURSOR,cursor_x,cursor_y,0x6900,FLIP_MAS|SIZE_MAS,NO_FLIP|SZ_16x16,20,1);
  spr_show(CURSOR);
  scroll(1,0,0,0,32,0xC0);
}

void highlight(int square, int pal, int depth, char ign)
{
  int i, p;
  p = get_path(square,999,path,battle_grid,entities[battle_grid[square]-1].team,depth,ign);
  for(i=0; i<map_size+1; i++)
  {
    change_background_palette((map[i].ownPos),pal,(464*map_no));
  }
}

void get_unit_radius(int square, int depth, char team, char ignore)
{
  get_path(square,999,path,battle_grid,team,depth,ignore);
}

void pattern(int square, int pal, int coords[36], char ignore_terrain)
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

display_camera_info()
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
  put_number(id,2,x,y);
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

char begin_battle(int attacker, int target, char range, char a_terrain, char t_terrain)
{
  int i, battle_result, resolution;

  reset_satb();
  vsync();

  resolution = battle_loop(attacker,target,range,a_terrain,t_terrain);

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
  // put_number(target,3,0,0);
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
    for(j=0; j<3; j++)
    {
      for(z=0; z<3; z++)
      {
        entities[i].row_one[(j*3)+z].hp = 0;
        entities[i].row_one[(j*3)+z].hp = 0;
        entities[i].row_one[(j*3)+z].hp = 0;
        entities[i].row_one[(j*3)+z].unit = 0;
        entities[i].row_one[(j*3)+z].unit = 0;
        entities[i].row_one[(j*3)+z].unit = 0;
      }
    }
  }
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
  clear_battle_items();
  reset_satb();
}
