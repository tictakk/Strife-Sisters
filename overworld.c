#include "npc.c"
#include "pathing.c"
#include "battlefield.c"
#include "overworld.h"

// char first_press;

void overworld_loop(int x, int y)
{
  char k;
  k = 0;
  // first_press = 1;
	party_direction = NORTH;

	s_y = 288;
	s_x = ((x*16)/256)*256;

  party_pos_x = x;
	party_pos_y = y;
	selector_y = (y*16)-s_y-16;//208-16;
	selector_x = x*16-s_x;//80;
	game_over = 1;
	menu_cols = 3;

	// disp_off();
  load_overworld_bg();
  // for(k=0; k<get_map_id_by_pos(get_absolute_pos())+1; k++)
  // {
  //   unlock_units_by_castle(k);
  // }
//	draw_npcs(5);
	satb_update();
	arrived(get_absolute_pos());
	while(game_over)
	{
		vsync();
		swap_water_tiles();

		if(menu_state == 0)
		{
				int pos;
				pos = (party_pos_y * 32) + party_pos_x;
//				draw_npcs(5);
				// cycle_spawn_animations(enemy_spawns,num_of_spawns);
				set_direction(0,party_direction);
				if(party_moving)
				{
					redowalk:
					if(party_walk)
					{
						spr_set(0);
						move_towards_direction();

						spr_y(selector_y);
						spr_x(selector_x);
					}
					else
					{
						if(overworld[pos] != ROAD)
						{
							party_moving = 0;
							party_walk = 0;
							arrived(pos);
						}
						else
						{
							if(get_next_pos())
							{
								party_walk = 16;
								goto redowalk;
							}
						}
					}
				}
				if(selector_x == 0 && s_x > 0)
				{
					s_x = 0;
					scroll(0,s_x,s_y,0, 223, 0xC0);
					selector_x = 256;
          s_x_relative = s_x / 8;
          s_y_relative = s_y / 8;
				}
				else if(selector_x == 256 && s_x < 224)
				{
					s_x = 256;
					scroll(0,s_x,s_y,0, 223, 0xC0);
					selector_x = 0;
          s_x_relative = s_x / 8;
          s_y_relative = s_y / 8;
				}
				else if(selector_y == -16 && s_y > 224)
				{
					s_y = 64;
					scroll(0,s_x,s_y,0, 223, 0xC0);
					selector_y = 224;
					spr_y(selector_y);
          s_x_relative = s_x / 8;
          s_y_relative = s_y / 8;
				}
				else if(selector_y == 224 && s_y < 288)
				{
					s_y = 288;
					scroll(0,s_x,s_y,0,223,0xC0);
					selector_y = 16;
          s_x_relative = s_x / 8;
          s_y_relative = s_y / 8;
				}
		}
		if(!party_moving)
		{
			overworld_controls();
		}
    // cycle_animations();
    if(g++ == 10)
    {
      char xi;
      increment_frame();

      for(xi=0; xi<npc_count-training_enemies_count; xi++)
      {
        draw_iso_npc(xi+5,1,17,npcs[xi].pos_x,npcs[xi].pos_y,xi);
      }
      g=0;
    }
		satb_update();
	}
}

void arrived(int pos)
{
//   if(prebattle_flag == get_map_id_by_pos(pos))
//   {
  if(demo_flag)
  {
    story(get_map_id_by_pos(pos),PREBATTLE,0);
  }
//   }
  map_no = get_map_id_by_pos(pos);
  unlock_units_by_castle(map_no);
  scroll(0,s_x,288,0, 223, 0xC0);

	load_map(0,0,0,0,MAP_WIDTH,OVERWORLD_MAP_HEIGHT);
	commander_select_cursor = 0;
	display_castle_menu();
}

void set_direction(int sprite_no, enum Direction direction)
{
	switch(direction)
	{
		case NORTH:
		spr_set(sprite_no);
		spr_pattern(0x6A00 + 0x100);
		break;
		case NORTHEAST:
		case SOUTHEAST:
		case EAST:
		spr_set(sprite_no);
		spr_pattern(0x6A00 + 0x200);
		spr_ctrl(FLIP_MAS|SIZE_MAS,NO_FLIP_X|SZ_16x32);
		break;
		case NORTHWEST:
		case WEST:
		case SOUTHWEST:
		spr_set(sprite_no);
		spr_pattern(0x6A00 + 0x200);
		spr_ctrl(FLIP_MAS,FLIP_X|SZ_16x32);
		break;
		case SOUTH:
		spr_set(sprite_no);
		spr_pattern(0x6A00);
		break;
	}
}

void move_towards_direction()
{
	switch(party_direction)
	{
		case NORTH:
		selector_y -= 4;
		break;

		case NORTHEAST:
		selector_x += 4;
		selector_y -= 4;
		break;

		case NORTHWEST:
		selector_x -= 4;
		selector_y -= 4;
		break;

		case EAST:
		selector_x += 4;
		break;

		case SOUTHEAST:
		selector_x += 4;
		selector_y += 4;
		break;

		case SOUTH:
		selector_y += 4;
		break;

		case SOUTHWEST:
		selector_y += 4;
		selector_x -= 4;
		break;

		case WEST:
		selector_x -= 4;
		break;
	}
	party_walk -= 4;
}

int get_next_pos()
{
	enum Direction direct;
	int tile1, tile2;
	tile1 = 0; tile2 = 0;

	switch(party_direction)
	{
		case EAST:
		tile1 = check_east();
		direct = EAST;
		if((tile2 = check_southeast()) > tile1)
		{
			tile1 = tile2;
			direct = SOUTHEAST;
		}
		if((tile2 = check_northeast()) > tile1)
		{
			direct = NORTHEAST;
		}
		break;

		case SOUTHEAST:
		tile1 = check_southeast();
		direct = SOUTHEAST;
		if((tile2 = check_east()) > tile1)
		{
			tile1 = tile2;
			direct = EAST;
		}
		if((tile2 = check_south()) > tile1)
		{
			direct = SOUTH;
		}
		break;

		case NORTHEAST:
		tile1 = check_northeast();
		direct = NORTHEAST;
		if((tile2 = check_north()) > tile1)
		{
			tile1 = tile2;
			direct = NORTH;
		}
		if((tile2 = check_east()) > tile1)
		{
			direct = EAST;
		}
		break;

		case NORTH:
		tile1 = check_north();
		direct = NORTH;
		if((tile2 = check_northeast()) > tile1)
		{
			tile1 = tile2;
			direct = NORTHEAST;
		}
		if((tile2 = check_northwest()) > tile1)
		{
			direct = NORTHWEST;
		}
		break;

		case NORTHWEST:
		tile1 = check_northwest();
		direct = NORTHWEST;
		if((tile2 = check_north()) > tile1)
		{
			tile1 = tile2;
			direct = NORTH;
		}
		if((tile2 = check_west()) > tile1)
		{
			direct = WEST;
		}
		break;

		case WEST:
		tile1 = check_west();
		direct = WEST;
		if((tile2 = check_northwest()) > tile1)
		{
			tile1 = tile2;
			direct = NORTHWEST;
		}
		if((tile2 = check_southwest()) > tile1)
		{
			direct = SOUTHWEST;
		}
		break;

		case SOUTHWEST:
		tile1 = check_southwest();
		direct = SOUTHWEST;
		if((tile2 = check_west()) > tile1)
		{
			tile1 = tile2;
			direct = WEST;
		}
		if((tile2 = check_south()) > tile1)
		{
			direct = SOUTH;
		}
		break;

		case SOUTH:
		tile1 = check_south();
		direct = SOUTH;
		if((tile2 = check_southeast()) > tile1)
		{
			tile1 = tile2;
			direct = SOUTHEAST;
		}
		if((tile2 = check_southwest()) > tile1)
		{
			direct = SOUTHWEST;
		}
		break;
	}
	
  if(set_direct(direct))
  {
    return 1;
  }

	party_moving = 0;
	party_walk = 0;
	return 0;
}

void set_direct(enum Direction direction)
{
	switch(direction)
	{
		case NORTH: set_north(); return 1;
		case NORTHWEST: set_northwest(); return 1;
		case NORTHEAST: set_northeast(); return 1;
		case EAST: set_east(); return 1;
		case SOUTHEAST: set_southeast(); return 1;
		case SOUTH: set_south(); return 1;
		case SOUTHWEST: set_southwest(); return 1;
		case WEST: set_west(); return 1;
	}
  return 0;
}

void load_overworld_bg()
{
	set_screen_size(SCR_SIZE_64x64);
	screen_dimensions = 64;
	s_x_relative = s_x / 8;
	s_y_relative = s_y / 8;

	set_map_data(overworldmap,MAP_WIDTH,32);
	set_tile_data(overworldtiles,166,overworldtilespal,TILE_WIDTH);

	load_tile(TILE_VRAM);

	load_palette(0,overworldpal,8);
	load_palette(10,fontpal,2);
	load_palette(9,borderspal,1);

	load_map(0,0,0,0,MAP_WIDTH,OVERWORLD_MAP_HEIGHT);
	// load_vram(0x6400,borders,0x100);

	set_font_pal(10);
	load_font(font,125,0x4800);
  load_terrains();
  load_vram(0x4BB0,icons_gfx,0x60);
	load_vram(0x49A0,icons_gfx+0x60,0x50);
  load_vram(0x4DF0,icons_gfx+0xC0,0x10);

	load_vram(0x6A00,rei_walk,0x300);
	load_palette(16,rei_walk_pal,1);
  load_palette(17,soldierpal,1);

	spr_make(0,selector_x,selector_y,0x6A00,FLIP_MAS|SIZE_MAS,SZ_16x32,0,1);

	init_overworld_data();
	disp_on();
	scroll(0,s_x,288,0, 223, 0xC0);
	satb_update();
}

void init_overworld_data()
{
	load_castle_data(997,0,0,0,0,NORTHWEST);//tutorial map 1
	load_castle_data(867,1,0,0,0,NORTH);//tutorial map 2
	load_castle_data(737,2,SWORD_UNIT,ARCHER_UNIT,CLERIC_UNIT,NORTH);//tutorial map 3
  load_castle_data(613,3,BRAWLER_UNIT,0,0,EAST); //map 4
  load_castle_data(617,4,SPEAR_UNIT,RAIDER_UNIT,0,SOUTH);//map 5
  load_castle_data(713,5,GOLEM_UNIT,HOUND_UNIT,0,SOUTHEAST);//map 6
  load_castle_data(811,6,DANCER_UNIT,WITCH_UNIT,0,SOUTH);//map 7
  load_castle_data(970,7,0,0,0,EAST); //map 8
  load_castle_data(946,8,0,0,0,NORTH); //map 9
  load_castle_data(852,9,0,0,0,EAST); //map 10
  load_castle_data(888,10,0,0,0,EAST); //map 11
  load_castle_data(891,11,0,0,0,NORTHEAST); //map 12
  load_castle_data(796,12,0,0,0,NORTH); //map 13
  load_castle_data(730,13,0,0,0,WEST); //map 14
  load_castle_data(727,14,0,0,0,WEST); //map 15
  load_castle_data(692,15,0,0,0,0); //map 16
}

void load_castle_data(int pos, int map_id, char unlock_one, char unlock_two, char unlock_three, enum Direction direction)
{
	castles[map_id].pos = pos;
	castles[map_id].map_id = map_id;
  castles[map_id].unlocked[0] = unlock_one;
  castles[map_id].unlocked[1] = unlock_two;
  castles[map_id].unlocked[2] = unlock_three;
  castles[map_id].direction = direction;
}

void display_castle_menu()
{
	hide_npcs(5);
  spr_show(0);
  // display_window_rel(0,0,10,7);
  display_window_abs(s_x_relative,s_y_relative,10,7);
  set_font_pal(11);
  //Get rid of shop, add "gems" menu for upgrading and 
  //Menu for upgrading Gems, "gems"

	put_string("Train",2+s_x_relative,1+s_y/8);
  set_font_pal(10);
  if(map_no < 2)
  {
    set_font_pal(11);
  }
	put_string("Hire",2+s_x/8,2+s_y/8);
	put_string("Army",2+s_x/8,3+s_y/8);
  set_font_pal(11);
	put_string("Info",2+s_x/8,4+s_y/8);
  set_font_pal(10);
	put_string("Begin",2+s_x/8,5+s_y/8);

  load_cursor(1,1,SLIDER_ONE);
	menu_state = CASTLE_MENU;
	menu_rows = 5;
}

void print_unit_row_by_type(char type, char x, char y)
{
  char i, offset;
  offset = 0;

  menu_option = 0;
  for(i=0; i<buyable_unit_count; i++)
  {
    load_unit_header(unlocked_units[i],0);

    if(unit_header[0].a_type == type)
    {
      set_font_pal(10);
      print_unit_type(unit_header[0].id,x,y+(s_y/8)+menu_option++);
    }
  }
  set_font_pal(10);
  menu_option -= 1;
}

char get_unit_by_type(char type, char index)
{
  char i, j;
  j=0;

  for(i=0; i<buyable_unit_count; i++)
  {
    load_unit_header(unlocked_units[i],0);
    if(unit_header[0].a_type == type)
    {
      if(j++ == index)
      {
        return unlocked_units[i];
      }
        // set_font_pal(10);
        // print_unit_type(unit_header[0].id,x,y+(s_y/8)+offset);
        // menu_option++;
    }
  }
  // put_number(menu_option,4,s_x_relative,s_y_relative+1);
  return -1;
}

void buy_unit()
{
  player_gold -= unit_cost;
  add_unit_to_convoy(selected_unit,1);
}

char get_map_id_by_pos(int pos)
{
	char i;
	for(i=0; i<NO_OF_CASTLES; i++)
	{
		if(castles[i].pos == pos)
		{
			return i;
		}
	}
	return 0;
}

char check_north(){ return overworld[((party_pos_y*32)+party_pos_x)-32];}
char check_northeast(){ return overworld[((party_pos_y*32)+party_pos_x)-32+1];}
char check_east(){ return overworld[((party_pos_y*32)+party_pos_x)+1];}
char check_southeast(){ return overworld[((party_pos_y*32)+party_pos_x)+32+1];}
char check_south(){ return overworld[((party_pos_y*32)+party_pos_x)+32];}
char check_southwest(){ return overworld[((party_pos_y*32)+party_pos_x)+32-1];}
char check_west(){ return overworld[((party_pos_y*32)+party_pos_x)-1];}
char check_northwest(){ return overworld[((party_pos_y*32)+party_pos_x)-32-1];}

void set_north()
{
	party_direction = NORTH;
	party_pos_last = (party_pos_y*32)+party_pos_x;
	party_pos_y -= 1;
	party_moving = 1;
	party_walk = 16;
}

int get_absolute_pos(){ return (party_pos_y*32)+party_pos_x; }

void set_northeast()
{
	party_direction = NORTHEAST;
	party_pos_last = (party_pos_y*32)+party_pos_x;
	party_pos_x += 1;
	party_pos_y -= 1;
	party_moving = 1;
	party_walk = 16;
}

int get_northeast(){ return (party_pos_y*32)+party_pos_x; }

void set_east()
{
	party_direction = EAST;
	party_pos_last = (party_pos_y*32)+party_pos_x;
	party_pos_x += 1;
	party_moving = 1;
	party_walk = 16;
}

int get_east() { return (party_pos_y*32)+party_pos_x; }

void set_southeast()
{
	party_direction = SOUTHEAST;
	party_pos_last = (party_pos_y*32)+party_pos_x;
	party_pos_x += 1;
	party_pos_y += 1;
	party_moving = 1;
	party_walk = 16;
}

int get_southeast() { return (party_pos_y*32)+party_pos_x; }

void set_south()
{
	party_direction = SOUTH;
	party_pos_last = (party_pos_y*32)+party_pos_x;
	party_pos_y += 1;
	party_moving = 1;
	party_walk = 16;
}

int get_south(){ return (party_pos_y*32)+party_pos_x; }

void set_southwest()
{
	party_direction = SOUTHWEST;
	party_pos_last = (party_pos_y*32)+party_pos_x;
	party_pos_x -= 1;
	party_pos_y += 1;
	party_moving = 1;
	party_walk = 16;
}

int get_southwest(){ return (party_pos_y*32)+party_pos_x; }

void set_west()
{
	party_direction = WEST;
	party_pos_last = (party_pos_y*32)+party_pos_x;
	party_pos_x -= 1;
	party_moving = 1;
	party_walk = 16;
}

int get_west(){ return (party_pos_y*32)+party_pos_x; }

void set_northwest()
{
	party_direction = NORTHWEST;
	party_pos_last = (party_pos_y*32)+party_pos_x;
	party_pos_x -= 1;
	party_pos_y -= 1;
	party_moving = 1;
	party_walk = 16;
}

int get_northwest(){ return (party_pos_y*32)+party_pos_x; }

void load_cmdr_army_to_npcs(char cmdr_id)
{
  char i;
  // hide_npcs(5);
  // reset_npcs();
  for(i=0; i<MAX_ARMY_SIZE; i++)
  {
    if(party_commanders[cmdr_id].bg.units[i].id)
    {
      add_npc(i/3,i%3,party_commanders[cmdr_id].bg.units[i].id,CMDR_PALETTE+1);
    }
  }
}

void overworld_controls(){
	char i, move_screen;
	move_screen = 16;
	j_1 = joy(0);
	j_2 = joytrg(0);

	if (j_2 & JOY_LEFT)
	{
    if(menu_state == SHOP_MENU && cursor_column > 0)
    {
      commander_select_cursor = 0;
      print_unit_row_by_type(attack_types[--cursor_column],2+(cursor_column*4),4);
      selected_unit = get_unit_by_type(attack_types[cursor_column],commander_select_cursor);
      remove_cursor();
      cursor_x -= 4;
      cursor_y = 4;
      display_cursor();
      update_hire_menu(selected_unit);
      return;
    }
    if(menu_state == FORMATION_MENU)
    {
      if(formation_cursor>0)
      {
        // formation_cursor--;
        // set_formation_menu();
        // return;
        animate_formation_left();
        return;
      }
    }
    if(is_selector_menu())
    {
      if(cursor_column > 0)
      {
        move_selector(-1,0);
      }
      return;
    }
    if(menu_state == DEPLOY_SELECT_MENU || menu_state == DISMISS_SELECT_MENU)
    {
      if(cursor_column > 0)
      {
        cursor_column--;
        curs_left(4);
        update_unit_stats_window(party_units[(cursor_column*8)+commander_select_cursor]&0xFF,ARMY_STATS_X,ARMY_STATS_Y,1);
      }
      return;
    }
		if(menu_state != OVERWORLD)
		{
			return;
		}
		if(party_moving != 0){ return; }
		if(check_west())
		{
			set_west();
		}
		else if(check_northwest())
		{
			set_northwest();
		}
		else if(check_southwest())
		{
			set_southwest();
		}
		load_map(9,18,9,18,10,4);
	}
	if (j_2 & JOY_RIGHT)
	{
    if(menu_state == SHOP_MENU && cursor_column < 6)
    {
      commander_select_cursor = 0;
      print_unit_row_by_type(attack_types[++cursor_column],2+(cursor_column*4),4);
      selected_unit = get_unit_by_type(attack_types[cursor_column],commander_select_cursor);
      remove_cursor();
      cursor_x += 4;
      cursor_y = 4;
      display_cursor();
      update_hire_menu(selected_unit);
      return;
    }
    if(menu_state == FORMATION_MENU)
    {
      animate_formation_right();
      return;
    }
    if(is_selector_menu())
    {
      if(cursor_column < menu_cols)
      {
        move_selector(1,0);
      }
      return;
    }
    if(menu_state == DEPLOY_SELECT_MENU || menu_state == DISMISS_SELECT_MENU)
    {
      if(cursor_column < menu_cols)
      {
        cursor_column++;
        curs_right(4);
        update_unit_stats_window(party_units[(cursor_column*8)+commander_select_cursor]&0xFF,ARMY_STATS_X,ARMY_STATS_Y,1);
      }
      return;
    }
		if(menu_state != OVERWORLD)
		{
				return;
		}
		if(party_moving != 0){ return; }
		if(check_east())
		{
			set_east();
		}
		else if(check_northeast())
		{
			set_northeast();
		}
		else if(check_southeast())
		{
			set_southeast();
		}
		load_map(9,18,9,18,10,4);
	}
	if (j_2 & JOY_UP)
	{
		if(menu_state != 0)
		{
      if(menu_state == SHOP_MENU)
      {
        if(commander_select_cursor > 0)
        {
          remove_cursor();
          commander_select_cursor--;
          selected_unit = get_unit_by_type(attack_types[cursor_column],commander_select_cursor);
          cursor_y--;
          display_cursor();
          // print_unit_type(selected_unit,s_x_relative,s_y_relative);
          update_hire_menu(selected_unit);
        }
        return;
      }
      if(is_selector_menu())
      {
        if(commander_select_cursor < (menu_rows - 1))
        {
          move_selector(0,1);
          return;
        }
      }
			else if(commander_select_cursor > 0)
			{
				commander_select_cursor--;
        curs_up();

				if(menu_state == ARMY_MENU)
				{
          selected_cmdr = commander_select_cursor;
          selected_formation = party_commanders[selected_cmdr].bg.formation;
          // update_party_commander_window(ARMY_GROUP_UNITS_WINDOW_X,ARMY_GROUP_UNITS_WINDOW_Y);
				}
				else if(menu_state == DEPLOY_SELECT_MENU  || menu_state == DISMISS_SELECT_MENU)
				{
          update_unit_stats_window(party_units[(cursor_column*8)+commander_select_cursor]&0xFF,ARMY_STATS_X,ARMY_STATS_Y,1);
				}
			}
			return;
		}
		if(party_moving != 0){ return; }
		if(check_north())
		{
			set_north();
		}
		load_map(9,18,9,18,10,4);
	}
	if (j_2 & JOY_DOWN)
	{
    if(menu_state != 0)
    {
      if(menu_state == SHOP_MENU)
      {
        if(commander_select_cursor < menu_option)
        {
          commander_select_cursor++;
          selected_unit = get_unit_by_type(attack_types[cursor_column],commander_select_cursor);
          remove_cursor();
          cursor_y++;
          display_cursor();
          update_hire_menu(selected_unit);
        }
        return;
      }
      if(is_selector_menu())
      {
        if(commander_select_cursor > 0)
        {
          move_selector(0,-1);
          return;
        }
      }
      else if(commander_select_cursor < (menu_rows-1))
      {
        commander_select_cursor++;
        curs_down();

        if(menu_state == ARMY_MENU) 
        {
          selected_cmdr = commander_select_cursor;
          selected_formation = party_commanders[selected_cmdr].bg.formation;
        }
        if(menu_state == DEPLOY_SELECT_MENU || menu_state == DISMISS_SELECT_MENU)
        {
          update_unit_stats_window(party_units[(cursor_column*8)+commander_select_cursor]&0xFF,ARMY_STATS_X,ARMY_STATS_Y,1);
        }
      }
      return;
    }
	  if(party_moving != 0){ return; }
    if(menu_state == 0)
    {
      if(check_south())
      {
        set_south();
      }
    }
	  load_map(9,18,9,18,10,4);
	}

	if(j_2 & JOY_I)
	{
    if(menu_state == FORMATION_MENU)
    {
      formation_menu_I();
      return;
    }
		if(menu_state == TRAIN_MENU)
		{
      selected_cmdr = commander_select_cursor;
      selected_formation = party_commanders[selected_cmdr].bg.formation;
      set_train_options_menu();
		}
    else if(menu_state == TRAIN_OPTIONS_MENU)
    {
      option_select_menu_buttons();
    }
		else if(menu_state == SHOP_MENU)//actually the buy units menu...
		{
      clear_joy_events(0x1F);
      // put_number(selected_unit,3,s_x_relative+14,s_y_relative);
			if(selected_unit > 0)
			{
        if(party_units_size < MAX_PARTY_UNIT_SIZE)
        {
				  if((player_gold >= get_unit_cost(selected_unit) && affirmative_question("Buy unit",22,14)))
				  {
				    buy_unit();
				  }
        }
        else
        {
          affirm_statement(" Convoy","  Full",22,14);
        }
			}
      // 2+(cursor_column*5),4
      update_hire_menu(selected_unit);
      load_cursor(1+(cursor_column*4),4+commander_select_cursor,SLIDER_ONE);
		}
    else if(menu_state == RETURN_SELECT_MENU)
    {
      char unit_id;
      unit_id = party_commanders[selected_cmdr].bg.units[selected_unit].id;

      if(unit_id)
      {
        if(party_units_size < MAX_PARTY_UNIT_SIZE-1)
        {
          if(unit_id < REI)
          {
            add_unit_to_convoy(unit_id,remove_unit_from_group(selected_cmdr,selected_unit));
            update_convoy_window(ARMY_CONVOY_WINDOW_X,ARMY_CONVOY_WINDOW_Y);
            update_unit_stats_window(NO_UNIT,ARMY_STATS_X,ARMY_STATS_Y,1);
            reset_npcs();
            load_cmdr_army_to_npcs(selected_cmdr);
            refresh_battle_units();
            spr_show(63);
          }
          else
          {
            affirm_statement("Cant ret","commndr",22,14);
            display_window_rel(ARMY_GROUP_UNITS_WINDOW_X,ARMY_GROUP_UNITS_WINDOW_Y,10,14);
          }
        }
        else
        {
          affirm_statement("Convoy","full",22,14);
        }
      }
      else
      {
        affirm_statement("No unit","selected",22,14);
        display_window_rel(ARMY_GROUP_UNITS_WINDOW_X,ARMY_GROUP_UNITS_WINDOW_Y,10,14);
      }
    }
    else if(menu_state == DISMISS_SELECT_MENU)
    {
      dismiss_select_I();
    }
		else if(menu_state == DEPLOY_SWAP_MENU)
		{
      char *formation;
      // put_number(party_commanders[selected_cmdr].bg.units[selected_unit].unit->id,3,0,36);
      if(party_commanders[selected_cmdr].bg.units[selected_unit].id >= COMMANDER_ID_START)
      {
        affirm_statement("Cant ret","Commandr",22,14);
        display_window_rel(22,14,10,14);

        return;
      }
      load_unit_header(party_commanders[selected_cmdr].bg.units[selected_unit].id,0);
      load_unit_header(party_units[swap_unit]&0xFF,1);
      // if(party_commanders[selected_cmdr].max_bp >= get_commander_battle_points(selected_cmdr) - unit_header[0].points + unit_header[1].points)
      formation = get_formation(party_commanders[selected_cmdr].bg.formation);
      if(formation[selected_unit])
      {
        swap_convoy_units(selected_cmdr,selected_unit,swap_unit);
        swap_unit = 0;
        display_deploy_select_menu();
        spr_hide(63);
        hide_npcs(5);
        reset_npcs();

        load_cmdr_army_to_npcs(selected_cmdr);
        load_cmdr_army_to_npcs(MAX_PARTY_COMMANDERS);
        refresh_battle_units();
      }
      else
      {
        affirm_statement("Invlaid","placemnt",22,14);
        display_window_rel(22,14,10,14);
      }
      // update_battle_group_window(selected_cmdr,1,17,0,training_enemies_count);
		}
    else if(menu_state == ORGANIZE_SELECT_MENU)
		{
			menu_state = SWAP_MENU;
			swap_unit = (cursor_column % 3)+(commander_select_cursor * 3);
      tile_highlight(0xE,swap_unit,TILE_HIGHLIGHT);
      // put_number(swap_unit,4,s_x_relative,s_y_relative);
		}
		else if(menu_state == SWAP_MENU)
		{
      if(!valid_formation_placement(selected_unit) || !valid_formation_placement(swap_unit))
      {
        spr_hide(1);
        satb_update();
        affirm_statement("Invalid","Tile",22,0);
        display_window_rel(22,0,10,14);
        if(primary_menu == TRAIN_MENU)
        {
          update_unit_stats_window(party_commanders[selected_cmdr].bg.units[selected_unit].id,23,0,1);
        }
        else
        {
          update_unit_stats_window(party_commanders[selected_cmdr].bg.units[selected_unit].id,ARMY_STATS_X,ARMY_STATS_Y,1);
        }
        return;
      }
        reset_npcs();
        load_cmdr_army_to_npcs(selected_cmdr);
        load_cmdr_army_to_npcs(MAX_PARTY_COMMANDERS);
      
        swap_commander_units(selected_cmdr,selected_unit,swap_unit);
        menu_state = ORGANIZE_SELECT_MENU;

        reset_npcs();
        load_cmdr_army_to_npcs(selected_cmdr);
        load_cmdr_army_to_npcs(MAX_PARTY_COMMANDERS);
        
        refresh_battle_units();
      

      if(primary_menu == TRAIN_MENU)
      {
        update_unit_stats_window(party_commanders[selected_cmdr].bg.units[selected_unit].id,23,0,1);
      }
      else
      {
        update_unit_stats_window(party_commanders[selected_cmdr].bg.units[selected_unit].id,ARMY_STATS_X,ARMY_STATS_Y,1);
      }
			swap_unit = 0;

      display_selector(63,get_iso_x(1,17,commander_select_cursor,cursor_column),get_iso_y(1,17,commander_select_cursor,cursor_column)+14,31);
      draw_formation(selected_formation);
		}
		else if(menu_state == DEPLOY_SELECT_MENU)
		{
      menu_state = DEPLOY_SWAP_MENU;
      swap_unit = (cursor_column*8) + commander_select_cursor;

      last_selected_col = cursor_column;
      last_selected_cursor = commander_select_cursor;

      commander_select_cursor = 0;
      cursor_column = 0;  
      menu_select_unit();
      set_menu_state(menu_state,3,2);
      remove_cursor();
      reset_npcs();
      load_selector(0x68C0,31);
      display_selector(63,get_iso_x(1,17,commander_select_cursor,cursor_column),get_iso_y(1,17,commander_select_cursor,cursor_column)+14,31);
      update_unit_stats_window(party_commanders[selected_cmdr].bg.units[0].id,ARMY_STATS_X,ARMY_STATS_Y,1);
		}
    else if(menu_state == ARMY_MENU)
		{
			selected_cmdr = commander_select_cursor;
      selected_formation = party_commanders[selected_cmdr].bg.formation;
      reset_npcs();

      load_cmdr_army_to_npcs(selected_cmdr);
      load_cmdr_army_to_npcs(MAX_PARTY_COMMANDERS);
      refresh_battle_units();
			commander_select_cursor = 0;
			cursor_column = 0;
			display_party_options_window(10,0);
		}
		else if(menu_state == CASTLE_MENU)
		{
      map_no = get_map_id_by_pos(get_absolute_pos());
      // spr_hide(0);
			switch(commander_select_cursor)
			{
        case 0:
          primary_menu = TRAIN_MENU;
          display_train_select_menu();
          // start_training_battle(0);
          // reload_overworld();
          // display_castle_menu();
          break;

				case 1:
          spr_hide(0);
          cursor_column = 0;
          commander_select_cursor = 0;
          display_hire_window(SWORD_UNIT);
          break;

        case 2:
          spr_hide(0);
          commander_select_cursor = 0;
          cursor_column = 0;
          display_party_commanders_window(0,0);
          break;

				case 4:
          copy_commanders(temp_commanders,party_commanders);
				  reset_npcs();
          init_map_data(get_map_id_by_pos(get_absolute_pos()));
	        begin_battlefield(get_map_id_by_pos(get_absolute_pos()));
					reload_overworld();
          
          // unlock_units_by_castle(map_no);
          // load_map(0,0,0,0,MAP_WIDTH,OVERWORLD_MAP_HEIGHT);
          if(demo_flag)
          {
            story(map_no,POSTBATTLE,0);
          }
          load_overworld_bg();
          load_map(0,0,0,0,MAP_WIDTH,OVERWORLD_MAP_HEIGHT);
          // if(game_over == 0)
          // {
          //   copy_commanders(party_commanders,temp_commanders);
          //   game_over = 1;
          // }
          
          // if(map_no == 3 || game_over == 0)
          // {
          //   game_over = 0;
          // }
          // else
          // {
            if(castles[map_no].direction)
            {
              set_direct(castles[map_no].direction);
            }
          // }
					break;
			}
		}
		else if(menu_state == PARTY_OPTION_MENU)
		{
      switch(commander_select_cursor)
      {
        case 0:
          display_deploy_select_menu();
          break;

        case 1:
          display_oraganize_select_menu(1,17);
          break;

        case 2:
          display_formation_menu();
          break;

        case 3:
          display_return_select_menu();
          break;

        case 4:
          display_dismiss_select_menu();
          break;
      }
		}
	}

	if(j_2 & JOY_II)
	{
    menu_back_button();
  }
	if(j_2 & JOY_RUN)
	{
    put_green_square(1,17);
  }
	if(j_2 & JOY_SEL)
	{
    // tile_highlight(0xE,11,TILE_FORMATION);
    // tile_highlight(0xE,8,TILE_HIGHLIGHT);
    // tile_highlight(0xE,8,TILE_GRAY);
    // for(;;)
    // {
    //   char zi;

    //   for(zi=0; zi<9; zi++)
    //   {
    //     tile_highlight(0xE,5+zi,range(0,512));
    //   }

    //   vsync(10);
    // }
  }
}

char find_next_empty_convoy_location()
{
  char i;
  for(i=0; i<MAX_PARTY_UNIT_SIZE; i++)
  {
    if(party_units[i] == 0)
    {
      return i;
    }
  }
  return -1;
}

void formation_menu_I()
{
  char i, cmdr_location;
  
  if(party_units_size + (get_commander_bg_size()) < MAX_PARTY_UNIT_SIZE)
  {
    party_commanders[selected_cmdr].bg.formation = get_next_formation(formation_cursor);
    draw_formation(party_commanders[selected_cmdr].bg.formation);

    for(i=0; i<MAX_ARMY_SIZE; i++)
    {
      if(party_commanders[selected_cmdr].bg.units[i].id)
      {
        if(party_commanders[selected_cmdr].bg.units[i].id < REI)
        {
          swap_convoy_units(selected_cmdr,i,find_next_empty_convoy_location());
        }
        else
        {
          cmdr_location = i;
        }
      }
    }
  }

  for(i=0; i<MAX_ARMY_SIZE; i++)
  {
    if(valid_formation_placement(i))
    {
      swap_commander_units(selected_cmdr,i,cmdr_location);
      display_party_options_window(10,0);
      load_cmdr_army_to_npcs(selected_cmdr);
      refresh_battle_units();
      put_string("        ",s_x_relative+13,s_y_relative+15);
      put_string("        ",s_x_relative+13,s_y_relative+16);
      update_convoy_window(ARMY_CONVOY_WINDOW_X,ARMY_CONVOY_WINDOW_Y);
      selected_formation = party_commanders[selected_cmdr].bg.formation;
      formation_cursor = 0;
      return;
    }
  }
}

void animate_formation_right()
{
  char zi, j;
  if(formation_cursor < owned_formation_count-1)
  {
    for(zi=0; zi<9; zi++)
    {
      for(j=0; j<6; j++)
      {
        put_string("         ",s_x_relative+1,s_y_relative+17+j);
      }
      put_green_square(1+zi,17);
      sync(1);
    }
    for(j=0; j<9;j++)
    {
      put_string("             ",s_x_relative+8,s_y_relative+17+j);
    }
    put_green_square(1,17);
    formation_cursor++;
    set_formation_menu();
  }
}

void animate_formation_left()
{
  char i,j;
  display_window_rel(0,14,22,14);
  if(formation_cursor > 0)
  {
    for(i=6; i>0; i--)
    {
      // put_green_square(1+i,17);
      for(j=0; j<9; j++)
      {
        put_string("              ",s_x_relative+1+i,s_y_relative+17+j);
      }
      put_green_square(1+i,17);
      sync(1);
    }
    display_window_rel(0,14,22,14);
    put_green_square(1,17);
    formation_cursor--;
    set_formation_menu();
  }
}

void display_formation_info()
{

}

void dismiss_select_I()
{
  char unit_pos = 0;

  clear_joy_events(0x1F);
  unit_pos = (cursor_column*8)+commander_select_cursor;
  if(party_units[unit_pos])
  {
    if(affirmative_question("Dismiss",22,14))
    {
      remove_unit_from_convoy(unit_pos);
      update_unit_stats_window(NO_UNIT,ARMY_STATS_X,ARMY_STATS_Y,1);
      update_convoy_window(ARMY_CONVOY_WINDOW_X,ARMY_CONVOY_WINDOW_Y);
    }
    update_unit_stats_window(party_units[(cursor_column*8)+commander_select_cursor]&0xFF,ARMY_STATS_X,ARMY_STATS_Y,1);
    display_window_rel(ARMY_GROUP_UNITS_WINDOW_X,ARMY_GROUP_UNITS_WINDOW_Y,10,14);
    load_cursor(23+(cursor_column*4),3+commander_select_cursor,SLIDER_ONE);
  }
}

void reload_overworld()
{
	load_overworld_bg();
	reset_npcs();
	satb_update();
	menu_state = OVERWORLD;
          
  load_map(0,0,0,0,MAP_WIDTH,OVERWORLD_MAP_HEIGHT);
}

void unlock_units_by_castle(char castle_no)
{
  char words[17];
  char p;
  for(p=0; p<3; p++)
  {
    if(castles[castle_no].unlocked[p])
    {
      unlock_unit(castles[castle_no].unlocked[p]);
      // strcpy(words,get_unit_fullname(castles[castle_no].unlocked[p]));
      // strcpy(words+7,unlock_text);
      // display_popup(words,1);
    }
  }
}

char is_selector_menu()
{
  return (menu_state == ORGANIZE_SELECT_MENU
         || menu_state == DEPLOY_SWAP_MENU
         || menu_state == SWAP_MENU
         || menu_state == RETURN_SELECT_MENU)? 1 : 0;
}

void move_selector(char row, char col)
{
  commander_select_cursor += col;
  cursor_column += row;
  menu_select_unit();
  // spr_make(63,(cursor_column*24)+20,(commander_select_cursor*24)+144-4,0x68C0,0,NO_FLIP|SZ_16x16,28,1);
  spr_make(63,get_iso_x(1,17,commander_select_cursor,cursor_column),get_iso_y(1,17,commander_select_cursor,cursor_column)+14,0x68C0,0,NO_FLIP|SZ_16x16,31,1);

  if(primary_menu == ARMY_MENU)
  {
    update_unit_stats_window(party_commanders[selected_cmdr].bg.units[selected_unit].id,ARMY_STATS_X,ARMY_STATS_Y,party_commanders[selected_cmdr].bg.units[selected_unit].level);
  }
  else
  {
    update_unit_stats_window(party_commanders[selected_cmdr].bg.units[selected_unit].id,23,0,1);
  }
}

void clear_commander_select()
{
	load_map(0,0,0,0,MAP_WIDTH,OVERWORLD_MAP_HEIGHT);
	spr_hide(1);
	menu_state = 0;
	commander_select_cursor = 0;
	// satb_update();
}

void menu_select_unit()
{
  selected_unit = (cursor_column % 3) + (commander_select_cursor * 3);
}