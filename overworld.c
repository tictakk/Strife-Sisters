#include "npc.c"
#include "pathing.c"
#include "battlefield.c"
#include "overworld.h"

char first_press;

void overworld_loop(int x, int y)
{
  char k;
  k = 0;
  first_press = 1;
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
  for(k=0; k<get_map_id_by_pos(get_absolute_pos()); k++)
  {
    // put_number(k,4,s_x_relative,s_y_relative);
    unlock_units_by_castle(k);
  }
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
		satb_update();
	}
}

void arrived(int pos)
{
  // if(prebattle_flag == get_map_id_by_pos(pos))
  // {
	// story(get_map_id_by_pos(pos),PREBATTLE,0);
  // }
  scroll(0,s_x,288,0, 223, 0xC0);
  // display_popup("Brawler \nunlocked",1);
	load_map(0,0,0,0,MAP_WIDTH,OVERWORLD_MAP_HEIGHT);
	commander_select_cursor = 0;
	display_castle_menu(get_map_id_by_pos(pos));
  // spr_hide(0);
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
	switch(direct)
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

	party_moving = 0;
	party_walk = 0;
	return 0;
}

void load_overworld_bg()
{
	total_sprites = 0;
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
	load_vram(0x6400,borders,0x100);

	set_font_pal(10);
	load_font(font,125,0x4800);
  load_terrains();
  load_vram(0x4BB0,icons_gfx,0x60);
	load_vram(0x49A0,icons_gfx+0x60,0x50);
	// load_vram(0x49A0,icons_gfx,0x60);
	// load_vram(0x4BB0,icons_gfx+0x60,0x50);

	load_vram(0x6A00,rei_walk,0x300);
	load_palette(16,rei_walk_pal,1);

	spr_make(0,selector_x,selector_y,0x6A00,FLIP_MAS|SIZE_MAS,SZ_16x32,0,1);

	init_overworld_data();

	disp_on();
	scroll(0,s_x,288,0, 223, 0xC0);

	satb_update();
}

void init_overworld_data()
{
	load_castle_data(997,0,BRAWLER_UNIT,HOUND_UNIT,0);//formation of 3
	load_castle_data(867,1,RAIDER_UNIT,SPEAR_UNIT,0);
	load_castle_data(737,2,GOLEM_UNIT,0,0);//formation of 4
  load_castle_data(613,3,THIEF_UNIT,KNIGHT_UNIT,0);
  load_castle_data(617,4,MAGE_UNIT,0,0);//formation of 5
  load_castle_data(713,5,LANCER_UNIT,STALKER_UNIT, 0);
  load_castle_data(811,6,DEMON_UNIT,0,0);//formation of 6
  load_castle_data(970,7,BERSERKER_UNIT,WITCH_UNIT,0);
  load_castle_data(946,8,PRIEST_UNIT,0,0);//formation of 7
  load_castle_data(852,9,0,0,0);//dancer
  load_castle_data(888,10,0,0,0);//dancer //formation of 8
  load_castle_data(891,11,0,0,0);//rogue
  load_castle_data(796,12,0,0,0);//formation of 9
  load_castle_data(730,13,0,0,0);
  load_castle_data(727,14,0,0,0);
  load_castle_data(692,15,0,0,0);
}

void load_castle_data(int pos, int map_id, char unlock_one, char unlock_two, char unlock_three)
{
	castles[map_id].pos = pos;
	castles[map_id].map_id = map_id;
  castles[map_id].unlocked[0] = unlock_one;
  castles[map_id].unlocked[1] = unlock_two;
  castles[map_id].unlocked[2] = unlock_three;
}

void display_castle_menu(char castle_no)
{
	hide_npcs(5);
  // display_window_rel(0,0,10,7);
  display_window_abs(s_x_relative,s_y_relative,10,7);
  set_font_pal(11);
  //Get rid of shop, add "gems" menu for upgrading and 
  //Menu for upgrading Gems, "gems"

	put_string("Heroes",2+s_x_relative,1+s_y/8);
	put_string("Hire",2+s_x/8,2+s_y/8);
	put_string("Army",2+s_x/8,3+s_y/8);
	put_string("Info",2+s_x/8,4+s_y/8);
  set_font_pal(10);
	put_string("Begin",2+s_x/8,5+s_y/8);

  load_cursor(1,1,SLIDER_ONE);
	menu_state = CASTLE_MENU;
	menu_rows = 5;
	current_selected_castle = castle_no;
}

void print_unit_row_by_type(char type, char x, char y)
{
  char i, offset;
  offset = 0;

  menu_option = 0;
  // put_number(type,3,s_x_relative,s_y_relative);
  for(i=0; i<buyable_unit_count; i++)
  {
    load_unit_header(unlocked_units[i],0);
    // put_number(buyable_unit_count,4,s_x_relative+7,s_y_relative);
    // wait_for_I_input();
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

    // put_number(unit_header[0].a_type,3,s_x_relative+5,s_y_relative);
    // put_number(type,3,s_x_relative,s_y_relative);
    // wait_for_I_input();
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
  add_unit_to_convoy(selected_unit);
}

void select_buyable_item(char item_index)
{
	selected_item = item_index;
	menu_state = BUY_ITEM_MENU;
	menu_rows = party_size;
	commander_select_cursor = 0;

	reset_npcs();
  remove_cursor();
  load_cursor(23,2,SLIDER_ONE);
}

void load_cmdr_army_to_npcs(char cmdr_id, char x, char y)
{
  char i, j;

  hide_npcs(5);
  reset_npcs();

  for(j=0; j<3; j++, y+=2)
  {
    for(i=0; i<3; i++)
    {
      if(party_commanders[cmdr_id].bg.units[(j*3)+i].id)
      {
        add_npc((x+(i%3)),y,party_commanders[cmdr_id].bg.units[(j*3)+i].id,
                UNIT_PALS[party_commanders[cmdr_id].bg.units[(j*3)+i].id]);
      }
	 	}
  }
  satb_update();
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
      put_number(selected_unit,4,s_x_relative,s_y_relative);
      remove_cursor();
      cursor_x -= 4;
      cursor_y = 4;
      display_cursor();
      // print_unit_type(selected_unit,s_x_relative,s_y_relative);
      update_hire_menu(selected_unit);
      return;
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
        update_unit_stats_window(party_units[(cursor_column*8)+commander_select_cursor],13,14,1);
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
      put_number(selected_unit,4,s_x_relative,s_y_relative);
      remove_cursor();
      cursor_x += 4;
      cursor_y = 4;
      display_cursor();
      // put_number(selected_unit,4,s_x_relative,s_y_relative);
      // print_unit_type(selected_unit,s_x_relative,s_y_relative);
      update_hire_menu(selected_unit);
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
        update_unit_stats_window(party_units[(cursor_column*8)+commander_select_cursor],13,14,1);
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

				if(menu_state == PARTY_MENU)
				{
          selected_cmdr = commander_select_cursor;
          update_party_commander_window(ARMY_GROUP_UNITS_WINDOW_X,ARMY_GROUP_UNITS_WINDOW_Y);
				}
				else if(menu_state == DEPLOY_SELECT_MENU  || menu_state == DISMISS_SELECT_MENU)
				{
          update_unit_stats_window(party_units[(cursor_column*8)+commander_select_cursor],13,14,1);
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
    if(first_press)
    {
      srand(clock_tt());
      first_press = 0;
    }
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

        if(menu_state == PARTY_MENU)
        {
          selected_cmdr = commander_select_cursor;
          update_party_commander_window(ARMY_GROUP_UNITS_WINDOW_X,ARMY_GROUP_UNITS_WINDOW_Y);
        }
        if(menu_state == DEPLOY_SELECT_MENU || menu_state == DISMISS_SELECT_MENU)
        {
          update_unit_stats_window(party_units[(cursor_column*8)+commander_select_cursor],13,14,1);
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
		if(menu_state == RECRUIT_MENU) //actually the shop items menu
		{
			last_selected_cursor = commander_select_cursor;
      set_font_pal(11);
			set_font_pal(10);

			load_portrait(0);
			display_item(0,0,2,14);

			display_cmdr_info(0,2,16);
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
      // put_number(party_units_size,4,s_x_relative,s_y_relative);
		}
    else if(menu_state == RETURN_SELECT_MENU)
    {
      char unit_id;
      unit_id = party_commanders[selected_cmdr].bg.units[selected_unit].id;
      if(unit_id)
      {
        remove_unit_from_group(selected_cmdr,selected_unit);
        add_unit_to_convoy(unit_id);
        update_battle_group_window(ARMY_BATTLE_GROUP_WINDOW_X,ARMY_BATTLE_GROUP_WINDOW_Y);
        update_party_commander_window(ARMY_GROUP_UNITS_WINDOW_X,ARMY_GROUP_UNITS_WINDOW_Y);
        update_convoy_window(ARMY_CONVOY_WINDOW_X,ARMY_CONVOY_WINDOW_Y);
        update_unit_stats_window(NO_UNIT,ARMY_STATS_X,ARMY_STATS_Y,1);
        spr_show(63);
      }
    }
    else if(menu_state == DISMISS_SELECT_MENU)
    {
      char unit_pos = 0;

      clear_joy_events(0x1F);
      unit_pos = (cursor_column*8)+commander_select_cursor;
      if(party_units[unit_pos])
      {
        if(affirmative_question("Dismiss",22,18))
        {
          remove_unit_from_convoy(unit_pos);
          update_unit_stats_window(NO_UNIT,11,14,1);
          update_convoy_window(ARMY_CONVOY_WINDOW_X,ARMY_CONVOY_WINDOW_Y);
        }
        update_unit_stats_window(party_units[(cursor_column*8)+commander_select_cursor],ARMY_STATS_X,ARMY_STATS_Y,1);
        display_commander_window(selected_cmdr,ARMY_GROUP_UNITS_WINDOW_X,ARMY_GROUP_UNITS_WINDOW_Y);
        load_cursor(23+(cursor_column*4),3+commander_select_cursor,SLIDER_ONE);
      }
    }
		else if(menu_state == DEPLOY_SWAP_MENU)
		{
      // put_number(party_commanders[selected_cmdr].bg.units[selected_unit].unit->id,3,0,36);
      load_unit_header(party_commanders[selected_cmdr].bg.units[selected_unit].id,0);
      load_unit_header(party_units[swap_unit],1);
      if(party_commanders[selected_cmdr].max_bp >= get_commander_battle_points(selected_cmdr) - unit_header[0].points + unit_header[1].points)
      {
        swap_convoy_units(selected_cmdr,selected_unit,swap_unit);
        swap_unit = 0;
        display_deploy_select_menu();
        update_battle_group_window(ARMY_BATTLE_GROUP_WINDOW_X,ARMY_BATTLE_GROUP_WINDOW_Y);
        update_party_commander_window(ARMY_GROUP_UNITS_WINDOW_X,ARMY_GROUP_UNITS_WINDOW_Y);
        spr_hide(63);
        satb_update();
      }
		}
    else if(menu_state == ORGANIZE_SELECT_MENU)
		{
      // char q,r;
      // r=0;
			menu_state = SWAP_MENU;
			swap_unit = (cursor_column % 3)+(commander_select_cursor * 3);
      // put_number(swap_unit,5,s_x_relative,s_y_relative);
      // for(q=0; q<9; q++)
      // {
      //   if(party_commanders[selected_cmdr].bg.units[q].id)
      //   {
      //     if(q==swap_unit)
      //     {
      //       highlight_npc(r);
      //     }
      //     r++;
      //   }
      // }
		}
		else if(menu_state == SWAP_MENU)
		{
			swap_commander_units(selected_cmdr,selected_unit,swap_unit);
			swap_unit = 0;
			menu_state = ORGANIZE_SELECT_MENU;

			display_battle_group_window(ARMY_BATTLE_GROUP_WINDOW_X,ARMY_BATTLE_GROUP_WINDOW_Y);
		}
		else if(menu_state == DEPLOY_SELECT_MENU)
		{
      menu_state = DEPLOY_SWAP_MENU;
      swap_unit = (cursor_column*8) + commander_select_cursor;
      commander_select_cursor = 0;
      cursor_column = 0;
      menu_select_unit();
      display_battle_group_window(ARMY_BATTLE_GROUP_WINDOW_X,ARMY_BATTLE_GROUP_WINDOW_Y); 
		}
    else if(menu_state == PARTY_MENU)
		{
			selected_cmdr = commander_select_cursor;
			commander_select_cursor = 0;
			cursor_column = 0;
			display_party_options_window(10,0);
		}
		else if(menu_state == CASTLE_MENU)
		{
      spr_hide(0);
			switch(commander_select_cursor)
			{
        case 0:
          // display_shop_menu(current_selected_castle);
          break;

				case 1:
          cursor_column = 0;
          commander_select_cursor = 0;
          display_hire_window(SWORD_UNIT);
          break;

        case 2:
          commander_select_cursor = 0;
          cursor_column = 0;
          display_party_commanders_window(0,0);
          break;

				case 4:
				  reset_npcs();
					init_map_data(get_map_id_by_pos(get_absolute_pos()));
					begin_battlefield(get_map_id_by_pos(get_absolute_pos()));

					load_overworld_bg();
					reset_npcs();
					satb_update();
					menu_state = OVERWORLD;
          unlock_units_by_castle(map_no);
          load_map(0,0,0,0,MAP_WIDTH,OVERWORLD_MAP_HEIGHT);
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
          display_oraganize_select_menu();
          break;

        case 2:
          // display_dismiss_select_menu();
          display_return_select_menu();
          break;

        case 3:
          // display_return_select_menu();
          display_dismiss_select_menu();
          break;
      }
		}
	}

	if(j_2 & JOY_II)
	{
		// return;
		if(menu_state == RECRUIT_MENU || menu_state == SHOP_MENU)
		{
			clear_commander_select();
			display_castle_menu(get_absolute_pos());
			return;
		}
		if(menu_state == CASTLE_MENU)
		{
			load_map(0,0,0,0,MAP_WIDTH,OVERWORLD_MAP_HEIGHT);
      spr_show(0);
			satb_update();
      menu_state = OVERWORLD;
      menu_rows = 0;
      commander_select_cursor = 0;
      last_selected_cursor = 0;
			return;
		}
		if(menu_state == BUY_ITEM_MENU)
		{
			commander_select_cursor = last_selected_cursor;
			display_item(0,1,18,14);
			menu_state = RECRUIT_MENU;
			menu_rows = num_of_buyable_items;
      remove_cursor();
      load_cursor(1,3+commander_select_cursor,SLIDER_ONE);
		}
    if(menu_state == ORGANIZE_SELECT_MENU || menu_state == DEPLOY_SELECT_MENU
     || menu_state == RETURN_SELECT_MENU || menu_state == DISMISS_SELECT_MENU)
    {
      spr_hide(1);
      hide_npcs(5);
      spr_hide(63);
      display_party_commanders_window(0,0);
      display_party_options_window(10,0);
      // spr_hide(1);
      // spr_hide(63);
      // commander_select_cursor = selected_cmdr;
      // cursor_column = 0;
      // display_party_commanders_window(0,0);
      return;
    }
    if(menu_state == PARTY_OPTION_MENU)
    {
      cursor_column = 0;
      commander_select_cursor = selected_cmdr;
      display_party_commanders_window(0,0);
      return;
    }
    if(menu_state == DEPLOY_SWAP_MENU)
    {
      commander_select_cursor = swap_unit % 8;
      cursor_column = swap_unit / 8;
      set_deploy_select_state();
      update_unit_stats_window(party_units[swap_unit],13,14,1);
      display_cursor();
      spr_hide(63);
      satb_update();
      return;
    }
		if(menu_state == BUY_SELECT_MENU)
		{
			commander_select_cursor = last_selected_cursor;
      cursor_column = last_selected_col;

			menu_state = SHOP_MENU;
			menu_rows = 6;

			load_cursor(1+(cursor_column*4),3+commander_select_cursor,SLIDER_ONE);
      check_unit_in_row(0,0);
			print_unit_row_by_type(unit_header[0].a_type,2,cursor_y);

			display_cursor();
			display_window_rel(0,12,16,16);
			display_window_rel(16,12,16,16);
			hide_npcs(5);
			reset_npcs();
			return;
		}
    if(menu_state == PARTY_MENU)
    {
      clear_commander_select();
      display_castle_menu(get_absolute_pos());
      return;
    }
    if(menu_state == SWAP_MENU)
    {
      swap_unit = 0;
      display_battle_group_window(ARMY_BATTLE_GROUP_WINDOW_X,ARMY_BATTLE_GROUP_WINDOW_Y);
      menu_state = ORGANIZE_SELECT_MENU;
      return;
    }
  }
	if(j_2 & JOY_RUN)
	{

  }
	if(j_2 & JOY_SEL)
	{
    // put_number(sizeof(Unit_Entity),5,s_x_relative,s_y_relative);
    put_number(hits,4,s_x_relative,s_y_relative);
    put_number(misses,4,s_x_relative,s_y_relative+1);
  }
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
  spr_make(63,get_iso_x(1,17,commander_select_cursor,cursor_column),get_iso_y(1,17,commander_select_cursor,cursor_column)+14,0x68C0,0,NO_FLIP|SZ_16x16,28,1);
  update_unit_stats_window(party_commanders[selected_cmdr].bg.units[selected_unit].id,ARMY_STATS_X,ARMY_STATS_Y,party_commanders[selected_cmdr].bg.units[selected_unit].level);
}

char check_unit_in_row(char curs_col, char curs_row)
{
  unsigned char i, desired_col, desired_row, row_count, unit_type, current_row;
  row_count = 0;

  desired_col = curs_col + cursor_column;
  current_row = 1 << (commander_select_cursor * 1);
  desired_row = current_row;

  if(curs_row > 0)
  {
    desired_row = current_row << 1;
  }
  if(curs_row < 0)
  {
    desired_row = current_row >> 1;
  }

  if(desired_col < 0 || desired_col > 6){ return -1; }

  for(i=0; i<buyable_unit_count; i++)
  {
    load_unit_header(buyable_units[i],0);
    if(unit_header[0].a_type == desired_row)
    {
      if(row_count++ == desired_col)
      {
        return buyable_units[i];
      }
    }
  }
  return -1;
}

void clear_commander_select()
{
	load_map(0,0,0,0,MAP_WIDTH,OVERWORLD_MAP_HEIGHT);
	// display_castles();
	spr_hide(1);
	// spr_hide(DEFAULT_SPRITE_NUMBER);
	menu_state = 0;
	commander_select_cursor = 0;
	satb_update();
}

void menu_select_unit()
{
  selected_unit = (cursor_column % 3) + (commander_select_cursor * 3);
}