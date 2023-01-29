#include "pathing.c"
#include "npc.c"
#include "battlefield.c"
#include "overworld.h"

void overworld_loop()
{
	char s;
	row_selected = ROW_ONE;
	s = script[0];
	party_direction = NORTH;
	s_y = 288;
	s_x = 0;
	// s_y_relative = s_y / 8;
	// s_x_relative = s_x / 8;
	selector_y = 208-16;
	selector_x = 80;
	game_over = 1;
	hz = 60;
	secs = 180;
	party_pos_x = 5;
	party_pos_y = 31;
	menu_cols = 3;

	disp_off();
  load_overworld_bg();

//	draw_npcs(5);
	satb_update();
	arrived(get_absolute_pos());

	while(game_over)
	{
		vsync();
		swap_water_tiles();

		if(menu_state == 0)
		{
			tick();
			if(tic == 0)
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
				}
				else if(selector_x == 256 && s_x < 224)
				{
					s_x = 256;
					scroll(0,s_x,s_y,0, 223, 0xC0);
					selector_x = 0;
				}
				else if(selector_y == -16 && s_y > 224)
				{
					s_y = 64;
					scroll(0,s_x,s_y,0, 223, 0xC0);
					selector_y = 224;
					spr_y(selector_y);
				}
				else if(selector_y == 224 && s_y < 288)
				{
					s_y = 288;
					scroll(0,s_x,s_y,0,223,0xC0);
					selector_y = 16;
				}
			}
			else
			{
				rand();
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
//  story(get_map_id_by_pos(pos),PREBATTLE,0);
	load_map(0,0,0,0,MAP_WIDTH,OVERWORLD_MAP_HEIGHT);
	commander_select_cursor = 0;

	display_castle_menu(get_map_id_by_pos(pos));
  spr_hide(0);
}

void set_direction(int sprite_no, enum Direction direction)
{
	switch(direction)
	{
		case NORTH:
		spr_set(sprite_no);
		spr_pattern(0x6A00 + (0x40*framer) + 0x100);
		break;
		case NORTHEAST:
		case SOUTHEAST:
		case EAST:
		spr_set(sprite_no);
		spr_pattern(0x6A00 + (0x40*framer) + 0x200);
		spr_ctrl(FLIP_MAS|SIZE_MAS,NO_FLIP_X|SZ_16x32);
		break;
		case NORTHWEST:
		case WEST:
		case SOUTHWEST:
		spr_set(sprite_no);
		spr_pattern(0x6A00 + (0x40*framer) + 0x200);
		spr_ctrl(FLIP_MAS,FLIP_X|SZ_16x32);
		break;
		case SOUTH:
		spr_set(sprite_no);
		spr_pattern(0x6A00 + (0x40*framer));
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
	set_tile_data(overworldtiles,147,overworldtilespal,TILE_WIDTH);

	load_tile(TILE_VRAM);

	load_palette(0,overworldpal,7);
	load_palette(10,fontpal,2);
	load_palette(8,borderspal,1);

	load_map(0,0,0,0,MAP_WIDTH,OVERWORLD_MAP_HEIGHT);
	load_vram(0x6400,borders,0x100);

	set_font_pal(10);
	load_font(font,125,0x4800);
  load_terrains();
	load_vram(0x49A0,icons_gfx,0x60);
	load_vram(0x4BB0,icons_gfx+0x60,0x60);

	load_vram(0x6A00,rei_walk,0x300);
	load_palette(16,rei_walk_pal,1);

	spr_make(0,selector_x,selector_y,0x6A00,FLIP_MAS|SIZE_MAS,SZ_16x32,0,1);

	init_overworld_data();

	disp_on();
	scroll(0,s_x,288,0, 223, 0xC0);

	satb_update();
}

void display_inventory(int x, int y, char item_type)
{
	int j=0, i=0;
  display_window(y,x,24,12);
	put_string("Inventory",y+6+(s_x/8),x+1+(s_y/8));

	for(j=0; j<no_of_party_items; j++)
	{
		if(item_type == ANY_ITEM || items[party_items[j]].type == item_type)
		{
			print_item_name(j,y+2,x+3,i++);
		}
	}

	menu_options = i;
	commander_select_cursor = 0;
	// menu_state = CONSUME_MENU + (item_type & 1);
}

void init_overworld_data()
{
	load_castle_data(994,0);
	load_castle_data(867,1);
	load_castle_data(737,2);
}

void load_castle_data(int pos, int map_id)
{
	castles[map_id].pos = pos;
	castles[map_id].map_id = map_id;
}

void display_shop_menu(char castle_no)
{
	char i;
  display_window(0,0,22,12);
	put_string("Shop",9+(s_x/8),1+(s_y/8));

  display_window(22,0,10,12);
  display_window(0,12,16,16);
  display_window(16,12,16,16);

	load_item(buyable_items[0],1);
	display_item(0,1,18,14);
	display_attribute_string(buyable_items[0],(s_x/8)+18,(s_y/8)+18);

	put_string("Gold",1+(s_x/8),9+(s_y/8));
	put_number(player_gold,5,(s_x/8)+1,10+(s_y/8));

	for(i=0; i<num_of_buyable_items; i++)
	{
		print_commander_stats(buyable_items[i],i);
	}

	for(i=0; i<party_size; i++)
	{
		put_string(party_commanders[i].name,24,(s_y/8)+2+i);
	}

  load_cursor(1,3,SLIDER_ONE);

  menu_options = num_of_buyable_items;
	menu_state = RECRUIT_MENU;
	commander_select_cursor = 0;
}

void display_castle_menu(char castle_no)
{
	hide_npcs(5);

  display_window(0,0,10,12);

  set_font_pal(11);
	put_string("Shop",2+s_x/8,3+s_y/8);
  set_font_pal(10);
	put_string("Units",2+s_x/8,4+s_y/8);
	put_string("Army",2+s_x/8,5+s_y/8);
  set_font_pal(11);
	put_string("Upgrade",2+s_x/8,6+s_y/8);
  set_font_pal(10);
	put_string("Begin",2+s_x/8,7+s_y/8);

  load_cursor(1,3,SLIDER_ONE);
	menu_state = CASTLE_MENU;
	menu_options = 5;
	current_selected_castle = castle_no;
}

void print_item_name(int i, int x_off, int y_off, char index)
{
	int x, y;
	x = x_off+(s_x/8);
	y = y_off+(s_y/8);

	put_string(items[i].name,x+((index/8) * 8),y+(index%8));
}

void print_commander_stats(char item_no, char i)
{
	int x, y;
	x = 2+(s_x/8);
	y = 3+(s_y/8);

	put_string(items[item_no].name,x,y+i);
	put_number(100,3,x+9,y+i);
}

void print_unit_row_by_type(char type, char x, char y)
{
  char i, offset;
  offset = 0;
  for(i=0; i<buyable_unit_count; i++)
  {
    if(unit_list[buyable_units[i]].a_type == type)
    {
      if(unlocked_units[buyable_units[i]])
      {
        set_font_pal(10);
        print_unit_type(unit_list[buyable_units[i]].id,x+offset,y+(s_y/8));
      }
      else
      {
        set_font_pal(11);
        print_unit_type(unit_list[buyable_units[i]].id,x+offset,y+(s_y/8));
      }
      offset += 4;
    }
  }
  set_font_pal(10);
}

void buy_item(char item_index, char cmdr_index)
{
	player_gold -= 100;
	party_commanders[cmdr_index].item = item_index;
  select_buyable_item(item_index);
}

void buy_unit()
{
  char i;
  player_gold -= 100;

  for(i=0; i<MAX_PARTY_UNIT_SIZE; i++)
  {
    if(party_units[i] == 0)
    {
      party_units[i] = selected_unit;
      party_units_size++;
      return;
    }
  }
}

void select_buyable_item(char item_index)
{
	selected_item = item_index;
	menu_state = BUY_ITEM_MENU;
	menu_options = party_size;
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
      if(party_commanders[cmdr_id].bg.units[(j*3)+i].unit->id)
      {
        add_npc((x+(i%3)),y,party_commanders[cmdr_id].bg.units[(j*3)+i].unit->id,
                UNIT_PALS[party_commanders[cmdr_id].bg.units[(j*3)+i].unit->id]);
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

void reset_menu_state(char ms, char options, char cursor_pos, char last_cursor)
{
	menu_state = ms;
	menu_options = options;
	commander_select_cursor = cursor_pos;
	last_selected_cursor = last_cursor;
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
    if(menu_state == SHOP_MENU)
    {
      char u_id;
      if((u_id = check_unit_in_row(-1,0)) >= 0)
      {
        selected_unit = u_id;
        cursor_column--;
        remove_cursor();
        cursor_x -= 4;
        display_cursor();

        update_recruit_menu(selected_unit);
      }
      return;
    }
    // if(menu_state == CMDR_MENU)
    if(menu_state == ASSIGN_MENU || menu_state == ARRANGE_MENU || menu_state == SWAP_MENU)
    {
      if(cursor_column > 0)
      {
        cursor_column--;
        selected_unit = (cursor_column % 3) + (commander_select_cursor * 3);
        // update_battle_group_window(selected_cmdr,0,14);
        spr_make(63,(cursor_column*24)+20,(commander_select_cursor*24)+144-4,0x68C0,0,NO_FLIP|SZ_16x16,28,1);
        display_unit_stats_window(party_commanders[selected_cmdr].bg.units[selected_unit].unit->id,22,14);
      }
      return;
    }
    if(menu_state == UNIT_MENU)
    {
      if(cursor_column > 0)
      {
        cursor_column--;
        curs_left(4);
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
    if(menu_state == SHOP_MENU)
    {
      char u_id;
      if((u_id = check_unit_in_row(1,0)) >= 0)
      {
        selected_unit = u_id;
        cursor_column++;
        remove_cursor();
        cursor_x += 4;
        display_cursor();

        update_recruit_menu(selected_unit);
      }
      return;
    }
    if(menu_state == ARRANGE_MENU || menu_state == ASSIGN_MENU || menu_state == SWAP_MENU)
    {
      if(cursor_column < menu_cols)
      {
        cursor_column++;
        selected_unit = (cursor_column % 3) + (commander_select_cursor * 3);
        // update_battle_group_window(selected_cmdr,0,14);
        spr_make(63,(cursor_column*24)+20,(commander_select_cursor*24)+144-4,0x68C0,0,NO_FLIP|SZ_16x16,28,1);
        display_unit_stats_window(party_commanders[selected_cmdr].bg.units[selected_unit].unit->id,22,14);
      }
      return;
    }
    if(menu_state == UNIT_MENU)
    {
      if(cursor_column < menu_cols)
      {
        cursor_column++;
        curs_right(4);
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
        char u_id;
        if((u_id = check_unit_in_row(0,-1)) >= 0)
        {
          selected_unit = u_id;
          commander_select_cursor--;
          remove_cursor();
          cursor_y -= 1;
          display_cursor();

          update_recruit_menu(selected_unit);
        }
        return;
      }
			if(commander_select_cursor > 0)
				{
					commander_select_cursor--;
          if(menu_state == ARRANGE_MENU || menu_state == ASSIGN_MENU || menu_state == SWAP_MENU)
          {
            selected_unit = (cursor_column % 3) + (commander_select_cursor * 3);
            // update_battle_group_window(selected_cmdr,0,14);
            spr_make(63,(cursor_column*24)+20,(commander_select_cursor*24)+144-4,0x68C0,0,NO_FLIP|SZ_16x16,28,1);
            display_unit_stats_window(party_commanders[selected_cmdr].bg.units[selected_unit].unit->id,22,14);
            return;
          }
          curs_up();

					if(menu_state == RECRUIT_MENU)
					{
						load_item(buyable_items[commander_select_cursor],1);
						display_attribute_string(buyable_items[commander_select_cursor],(s_x/8)+18,(s_y/8)+18);
					}
					else if(menu_state == BUY_SELECT_MENU)
					{
//						display_army_info(commander_select_cursor,17,13);
//						item_purchase_select();
					}
					else if(menu_state == BUY_ITEM_MENU)
					{
//						item_purchase_select();
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
      char u_id;
      if((u_id = check_unit_in_row(0,1)) >= 0)
      {
        selected_unit = u_id;
        commander_select_cursor++;
        remove_cursor();
        cursor_y += 1;
        display_cursor();

        update_recruit_menu(selected_unit);
      }
      return;
    }
		if(commander_select_cursor < (menu_options-1))
		{
			commander_select_cursor++;
      if(menu_state == ARRANGE_MENU || menu_state == ASSIGN_MENU || menu_state == SWAP_MENU)
      {
        selected_unit = (cursor_column % 3) + (commander_select_cursor * 3);
        // update_battle_group_window(selected_cmdr,0,14);
        spr_make(63,(cursor_column*24)+20,(commander_select_cursor*24)+144-4,0x68C0,0,NO_FLIP|SZ_16x16,28,1);
        display_unit_stats_window(party_commanders[selected_cmdr].bg.units[selected_unit].unit->id,22,14);
        return;
      }
      curs_down();

			if(menu_state == RECRUIT_MENU)
			{
				load_item(buyable_items[commander_select_cursor],1);
				display_attribute_string(buyable_items[commander_select_cursor],(s_x/8)+18,(s_y/8)+18);
			}
			if(menu_state == BUY_SELECT_MENU)
			{
//				display_army_info(commander_select_cursor,17,13);
//				item_purchase_select();
			}
			else if(menu_state == BUY_ITEM_MENU)
			{
//				item_purchase_select();
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
			print_item_name(buyable_items[commander_select_cursor],2,3,commander_select_cursor);
			set_font_pal(10);
			select_buyable_item(buyable_items[commander_select_cursor]);

			load_portrait(0);
			display_item(0,0,2,14);

			display_cmdr_info(0,2,16);
			if(party_commanders[0].item)
			{
				put_string("Item",(s_x/8)+9,(s_y/8)+20);
				load_item(party_commanders[0].item,2);
				display_item(0,2,9,22);
				display_attribute_string(party_commanders[0].item,(s_x/8)+9,(s_y/8)+26);
			}
		}
		else if(menu_state == SHOP_MENU)//actually the buy units menu...
		{
      clear_joy_events(0x1F);

			if(unlocked_units[selected_unit])
			{
				if(affirmative_question("Buy unit",22,18))
				{
				buy_unit();
				}
			}
      		else
			{
				if(affirmative_question("Upgrade",22,18))
				{
				unlock_unit(selected_unit);
				}
			}
      		display_recruit_window(selected_unit);
		}
		else if(menu_state == ASSIGN_MENU)
		{
			selected_unit = (cursor_column % 3)+(commander_select_cursor * 3);
			display_convoy_select_window(22,0);
		}
    	else if(menu_state == ARRANGE_MENU)
		{
			menu_state = SWAP_MENU;
			swap_unit = (cursor_column % 3)+(commander_select_cursor * 3);
			put_white_square(2+(cursor_column*3),17+(commander_select_cursor*3));
		}
		else if(menu_state == SWAP_MENU)
		{
			swap_commander_units(selected_cmdr,selected_unit,swap_unit);
			swap_unit = 0;
			menu_state = ARRANGE_MENU;

			display_battle_group_window(selected_cmdr,0,14);
			display_unit_stats_window(party_commanders[selected_cmdr].bg.units[selected_unit].unit->id,22,14);
		}
		else if(menu_state == UNIT_MENU)
		{
			swap_convoy_units(selected_cmdr,selected_unit,(cursor_column*8)+commander_select_cursor);
			commander_select_cursor = (selected_unit) / 3;
			cursor_column = selected_unit % 3;

			display_battle_group_window(selected_cmdr,0,14);
			update_convoy_select(22,0);
			display_unit_stats_window(party_commanders[selected_cmdr].bg.units[selected_unit].unit->id,22,14);
			menu_state = ASSIGN_MENU;
		}
    	else if(menu_state == PARTY_MENU)
		{
			selected_cmdr = commander_select_cursor;
			commander_select_cursor = 0;
			cursor_column = 0;
			// display_battle_group_window(selected_cmdr,0,14);
			display_party_options_window(10,0);
			// display_unit_stats_window(party_commanders[selected_cmdr].bg.units[0].unit->id,22,14);
		}
		else if(menu_state == CASTLE_MENU)
		{
			switch(commander_select_cursor)
			{
        case 0:
          // display_shop_menu(current_selected_castle);
          break;

				case 1:
          cursor_column = 0;
          commander_select_cursor = 0;
          display_recruit_window(SWORD_UNIT);
          break;

        case 2:
          commander_select_cursor = 0;
          cursor_column = 0;
          display_party_commanders_window(0,0,0);
          break;

				case 4:
				  reset_npcs();
					init_map_data(get_map_id_by_pos(get_absolute_pos()));
					begin_battlefield(get_map_id_by_pos(get_absolute_pos()));
					disp_off();
					if(get_map_id_by_pos(get_absolute_pos()) == 2)
					{
						game_over = 0;
						reset_npcs();
						reset_satb();
					}
					else
					{
						load_overworld_bg();
						reset_npcs();
						satb_update();
						menu_state = OVERWORLD;
					}
					break;
			}
		}
		else if(menu_state == OVERWORLD)
		{
		}
		else if(menu_state == PARTY_MENU)
		{
		}
		else if(menu_state == PARTY_OPTION_MENU)
		{
      switch(commander_select_cursor)
      {
        case 0:
          remove_cursor();
          menu_state = ASSIGN_MENU;
          commander_select_cursor = 0;
          break;

        case 1:
          remove_cursor();
          menu_state = ARRANGE_MENU;
          commander_select_cursor = 0;
          break;

        // case 2:
          // remove_cursor();
          // menu_state = ARTS_MENU;
          // break;
      }
      display_battle_group_window(selected_cmdr,0,14);
		}
		else if(menu_state == ASSIGN_MENU)
		{
		}
		else if(menu_state == ARRANGE_MENU)
		{
		}
	}

	if(j_2 & JOY_II)
	{
		// return;
		if(menu_state == RECRUIT_MENU || menu_state == SHOP_MENU)
		{
			clear_commander_select();
			display_castle_menu(0);
			return;
		}
		if(menu_state == CASTLE_MENU)
		{
			load_map(0,0,0,0,MAP_WIDTH,OVERWORLD_MAP_HEIGHT);
      spr_show(0);
			satb_update();
			reset_menu_state(OVERWORLD,0,0,0);
			return;
		}
		if(menu_state == BUY_ITEM_MENU)
		{
			commander_select_cursor = last_selected_cursor;
			print_item_name(buyable_items[commander_select_cursor],2,3,commander_select_cursor);
			load_item(buyable_items[commander_select_cursor],1);
			display_item(0,1,18,14);
			display_attribute_string(buyable_items[commander_select_cursor],(s_x/8)+18,(s_y/8)+18);
			menu_state = RECRUIT_MENU;
			menu_options = num_of_buyable_items;
      remove_cursor();
      load_cursor(1,3+commander_select_cursor,SLIDER_ONE);
		}
    if(menu_state == ARRANGE_MENU || menu_state == ASSIGN_MENU)
    {
      hide_npcs(5);
      spr_hide(1);
      spr_hide(63);
      commander_select_cursor = 0;
      cursor_column = 0;
      display_party_commanders_window(0,0,0);
      return;
    }
    if(menu_state == UNIT_MENU)
    {
      commander_select_cursor = selected_unit / 3;
      cursor_column = selected_unit % 3;
      display_battle_group_window(selected_cmdr,0,14);
      return;
    }
		if(menu_state == BUY_SELECT_MENU)
		{
			commander_select_cursor = last_selected_cursor;
      cursor_column = last_selected_col;

			menu_state = SHOP_MENU;
			menu_options = 6;

			load_cursor(1+(cursor_column*4),3+commander_select_cursor,SLIDER_ONE);
			print_unit_row_by_type(unit_list[check_unit_in_row(0,0)].a_type,2,cursor_y);

			display_cursor();
			display_window(0,12,16,16);
			display_window(16,12,16,16);
			hide_npcs(5);
			reset_npcs();
			return;
		}
    if(menu_state == PARTY_MENU)
    {
      clear_commander_select();
      display_castle_menu(0);
      return;
    }
    if(menu_state == SWAP_MENU)
    {
      swap_unit = 0;
      display_battle_group_window(selected_cmdr,0,14);
      menu_state = ARRANGE_MENU;
      return;
    }
	}

	 if(j_2 & JOY_RUN)
	 {

	 }
	 if(j_2 & JOY_SEL)
	 {
    // display_party_commanders_window(0,0,0);
//     display_player_resources_window(0,0);
//     display_convoy_window(22,0);
//     display_window(10,0,12,14);
//     display_window(0,14,22,14);
//     display_unit_stats_window(SPEAR_UNIT,22,14);
   }
}

char check_unit_in_row(char curs_col, char curs_row)
{
  unsigned char i, desired_col, desired_row, row_count, unit_type, current_row;
  row_count = 0;

  desired_col = curs_col + cursor_column;
  current_row = 1 << (commander_select_cursor * 1);
  desired_row = current_row;
  // desired_row = commander_select_cursor + curs_row;

  if(curs_row > 0)
  {
    desired_row = current_row << 1;
  }
  if(curs_row < 0)
  {
    desired_row = current_row >> 1;
  }
  // desired_row = 1 <<  (curs_row * (commander_select_cursor+1));

  put_number(desired_row,3,0,36);

  if(desired_col < 0 || desired_col > 6){ return -1; }

  for(i=0; i<buyable_unit_count; i++)
  {
    if(unit_list[buyable_units[i]].a_type == desired_row)
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

timer()
{
	if(hz-- == 0)
	{
		put_number(--secs/SECONDS,1,26,2);
		put_string(":",27,2);
		put_number(secs%SECONDS,2,28,2);
		hz = 60;

		if(secs%SECONDS < 10)
		{
			put_number(0,1,28,2);
		}
		if(secs < 1)
		{
			secs = 180;
		}
	}
}
