#include "pathing.c"
#include "npc.c"
#include "battlefield.c"

// extern void wave_1(void);
// extern void wave_2(void);
extern const char wave_1[32];
extern const char wave_2[32];

#define PANEL_VRAM 0x2000
#define OVERWORLD 0
#define SHOP_MENU 1
#define RECRUIT_MENU 2
#define CASTLE_MENU 3
#define OVERWORLD_MENU 4
#define CONSUME_MENU 5
#define EQUIP_MENU 6
#define UNIT_MENU 7
#define CMDR_MENU 8
#define CONSUME_ITEM_MENU 9
#define EQUIP_ITEM_MENU 10
#define ORDER_MENU 11
#define BUY_SELECT_MENU 12
#define BUY_CMDR_UNIT_MENU 13
#define BUY_ITEM_MENU 14
#define UNIT_SLIDER_MENU 15
#define ROW_SELECT_MENU 16
#define SECONDS 60

char menu_state = 0;
char menu_options = 0;
char submenu_state = 0;
char row_selected = 0;

//party
enum Direction party_direction;
char party_moving = 0;
int party_pos_x = 5;
int party_pos_y = 31;
int party_pos_last = 0;
char party_walk = 0;
int party_last_pos = 0;

char selected_item = 0;
char selected_unit = 0;
char cmdr_selected = 0;
char cycler = 0;
char current = 0;
char current_selected_castle = 0;
int commander_select_cursor = 0;
int last_selected_cursor = 0;
int aahh = 0;
char bbhh = 0;
char chan = 0;

char hz = 60;
int secs = 180;

void overworld_loop()
{
	char s;
	row_selected = ROW_ONE;
	aahh = 0b0000011111111111;
	// bbhh = 0b0000011111111111;
	s = script[0];
	party_direction = NORTH;
	s_y = 288;
	selector_y = 208-16;
	selector_x = 80;
	game_over = 1;

	disp_off();
  	load_overworld_bg();

	spawn_starting_enemies();
	draw_npcs(5);
	satb_update();
	arrived(get_absolute_pos());

	// load_wavetable(wave_2);
	// chirp(76);
	// set_channel(1);
	// chirp(76);
	// set_channel(chan);
	// chirp(76);

	while(game_over)
	{
		vsync();
		swap_water_tiles();
		// put_hex(raw_map_data,5,1,36);
		if(menu_state == 0)
		{
			tick();
			if(tic == 0)
			{
				int pos;
				pos = (party_pos_y * 32) + party_pos_x;
				draw_npcs(5);
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
	// story(get_map_id_by_pos(pos),PREBATTLE,0);
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

// atk | def | hp | spd | level
void spawn_starting_enemies()
{
	init_npcs();
	// load_npcs(area_one);
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

	// put_hex(&capture_position,4,10,40);
	// put_number(selector_y,4,10,41);
	// vsync();
	satb_update();
}

void display_inventory(int x, int y, char item_type)
{
	int j=0, i=0;
	display_info_panel(x,y,24,12);
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
	menu_state = CONSUME_MENU + (item_type & 1);
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

void display_overworld_menu()
{
	int x,y, i;
	x = (s_x/8);
	y = (s_y/8);
	spr_hide(0);

	display_info_panel(0,0,8,12);
	display_info_panel(0,8,24,12);
	display_info_panel(12,0,32,16);
	put_string("Items",x+2,y+1);
	put_string("Equip",x+2,y+2);
	put_string("Shop",x+2,y+3);
	put_string("Enlist",x+2,y+4);
	put_string("Begin",x+2,y+5);

	menu_state = OVERWORLD_MENU;
	menu_options = 5;
	commander_select_cursor = 0;
}

void display_commanders_in_castle(char castle_no)
{
	char j;
	display_info_panel(0,0,23,12);
	put_string("Recruit",10+(s_x/8),1+(s_y/8));
	put_string("Unit",(s_x/8)+2,(s_y/8)+3);
	put_string("Cost",(s_x/8)+11,(s_y/8)+3);
	put_string("Type",(s_x/8)+17,(s_y/8)+3);

	display_info_panel(0,23,9,12);
	// display_info_panel(12,4,24,8);
	display_info_panel(12,0,16,16);
	display_info_panel(12,16,16,16);
	put_string("Gold",1+(s_x/8),9+(s_y/8));
	put_number(player_gold,5,(s_x/8)+1,10+(s_y/8));

	spr_set(1);
	spr_x(4);
	spr_y(28);

	for(j=0; j<num_of_buyable_units; j++)
	{
		print_unit_data(buyable_units[j],j,0,1,1);
	}

	for(j=0; j<party_size; j++)
	{
		put_string(party_commanders[j].name,25,2+(s_y/8)+j);
	}

	menu_state = SHOP_MENU;
	commander_select_cursor = 0;
	menu_options = num_of_buyable_units;
}

void display_shop_menu(char castle_no)
{
	char i;
	display_info_panel(0,0,23,12);
	put_string("Shop",9+(s_x/8),1+(s_y/8));

	display_info_panel(0,23,9,12);
	display_info_panel(12,0,16,16);
	display_info_panel(12,16,16,16);

	load_item(buyable_items[0],1);
	display_item(0,1,18,14);
	display_attribute_string(buyable_items[0],(s_x/8)+18,(s_y/8)+18);

	// put_string("Gold",17+(s_x/8),9+(s_y/8));
	put_string("Gold",1+(s_x/8),9+(s_y/8));
	put_number(player_gold,5,(s_x/8)+1,10+(s_y/8));
	// put_number(player_gold,6,16+(s_x/8),10+(s_y/8));
	spr_set(1);
	spr_y(20);

	for(i=0; i<num_of_buyable_items; i++)
	{
		print_commander_stats(buyable_items[i],i);
	}

	for(i=0; i<party_size; i++)
	{
		put_string(party_commanders[i].name,25,(s_y/8)+2+i);
	}

	menu_options = num_of_buyable_items;
	menu_state = RECRUIT_MENU;
	commander_select_cursor = 0;
	satb_update();
}

void display_castle_menu(char castle_no)
{
	hide_npcs(5);

	display_info_panel(0,0,8,12);
	set_font_pal(11);
	put_string("Shop",2+s_x/8,1+s_y/8);
	put_string("Units",2+s_x/8,2+s_y/8);
	put_string("Army",2+s_x/8,3+s_y/8);
	put_string("Cmdrs",2+s_x/8,4+s_y/8);
	set_font_pal(10);
	put_string("Begin",2+s_x/8,5+s_y/8);
  cursor_x = 1;
  cursor_y = 1;
	load_cursor(cursor_x,1,SLIDER_ONE);
	menu_state = CASTLE_MENU;
	menu_options = 5;
	current_selected_castle = castle_no;
	// hide_npcs(5);
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

void print_unit_data(char unit_id, char i, int x_off, int y_off, char disp_cost)
{
	int x, y;
	x = x_off+2+(s_x/8);
	y = y_off+3+(s_y/8);
	print_unit_type(unit_id,x+((i/7) * 7),y+(i%7));

	if(disp_cost)
	{
		put_number(get_unit_cost(buyable_units[unit_id]),3,x+9,y+i);
		print_unit_attack_icon(unit_id,(s_x/8)+18,(s_y/8)+4+i);
		print_unit_defense_icon(unit_id,(s_x/8)+19,(s_y/8)+4+i);
	}
}

void print_commander_data(char cmdr_id)
{
	int x, y;
	x = 23+((s_x>>1)/8);
	y = 5+((s_y>>1)/8);
	put_string("       ",x+2,y);
	put_string(party_commanders[cmdr_id].name,x+2,y);
	put_string("HP",x+4,y+1);
	put_char('/',x+4,y+2);

	put_string("AP",x+4,y+3);
	// put_number(commanders[cmdr_id].ap,3,x+1,y+4);
	put_char('/',x+4,y+4);
	// put_number(commanders[cmdr_id].ap,3,x+5,y+4);
}

char buy_item(char item_index, char cmdr_index)
{
	// int cost;
	// char item_id;
	// item_id = buyable_items[item_index];
	// cost = items[item_id].cost;
	player_gold -= 100;
	party_commanders[cmdr_index].item = item_index;
	// party_items[no_of_party_items++] = item_id;
	return 1;
}

void select_buyable_unit(char unit_index)
{
	selected_unit = unit_index;
	menu_state = BUY_SELECT_MENU;
	menu_options = party_size;
	commander_select_cursor = 0;
	reset_npcs();
	display_army_info(party_commanders[0].sprite_type-16,17,13);
	item_purchase_select();

	spr_set(1);
	spr_y(12);
	spr_x(186);
	satb_update();
}

void select_cmdr_for_unit()
{
	last_selected_cursor = commander_select_cursor;
	menu_state = BUY_CMDR_UNIT_MENU;
	menu_options = 3;
	commander_select_cursor = 0;
	row_selected = ROW_ONE;

	set_font_pal(11);
	put_string(party_commanders[commander_select_cursor].name,25,(s_y/8)+2+commander_select_cursor);
	set_font_pal(10);

	load_cursor(225+12,112,SLIDER_ONE);
	load_cursor(145+32,112,SLIDER_TWO);
}

void select_buyable_item(char item_index)
{
	selected_item = item_index;
	menu_state = BUY_ITEM_MENU;
	menu_options = party_size;
	commander_select_cursor = 0;

	reset_npcs();
	spr_set(1);
	spr_y(12);
	spr_x(186);
	satb_update();
}

void remove_unit_from_cmdr()
{
	int cost;
	char cmdr_id, min, row_count;
	cost = get_unit_cost(selected_unit);
	cmdr_id = cmdr_selected;

	min = 0;
	// row_count = commanders[cmdr_id].row_counts[row_selected];
	// unit_count = commanders[cmdr_id].no_of_units;
	// if(row_count > min)
	// {
	// 	player_gold += cost;
	// 	commanders[cmdr_id].row_counts[row_selected]--;
	// 	display_cmdr_army_info(cmdr_id,17,13);
	// 	load_cmdr_army_to_npcs(cmdr_id);
	// 	draw_npcs(5);
	// 	put_number(player_gold,5,(s_x/8)+1,10+(s_y/8));
	// }
}

void add_unit_to_cmdr()
{
	// int cost;
	// char cmdr_id, row_count;
	// char *row;
	// cost = get_unit_cost(selected_unit);
	// cmdr_id = cmdr_selected;
	// row = commanders[cmdr_id].row_one + (row_selected*3);

	// row_count = commanders[cmdr_id].row_counts[row_selected];
	// if(row_count < MAX_UNITS_PER_ROW && player_gold > cost)
	// {
	// 	player_gold -= cost;
	// 	display_cmdr_army_info(cmdr_id,17,13);
	// 	put_number(player_gold,5,(s_x/8)+1,10+(s_y/8));
	// 	load_cmdr_army_to_npcs(cmdr_id);
	// 	draw_npcs(5);
	// }
}

void display_army_info(char cmdr_id, int x, int y)
{
	cmdr_selected = cmdr_id;
	last_selected_cursor = commander_select_cursor;
	display_cmdr_army_info(cmdr_id,x,y);
	load_cmdr_army_to_npcs(cmdr_id);
	load_portrait(cmdr_id+16);
	draw_npcs(5);
}

void load_cmdr_army_to_npcs(char cmdr_id)
{
	// char i, j;
	// int x,y;
	// char *row;
	// hide_npcs(5);
	// reset_npcs();
	// row = commanders[cmdr_id].row_one;
	// x = 12;
	// y = 6;
	// for(j=0; j<3; j++, y+=2, row+=3)
	// {
	// 	for(i=0; i<commanders[cmdr_id].row_counts[j]; i++)
	// 	{
	// 		add_npc(x+i,y,unit_list[row[i]].unit_type,UNIT_PALS[unit_list[row[i]].unit_type]);
	// 	}
	// }
}

void item_swap_menu()
{
	int z;
	load_portrait(0);

	for(z=0; z<party_size; z++)
	{
		put_string(party_commanders[z].name,3,49+z);
	}
	display_item(0,0,11,13);
	display_item(0,1,21,13);
	if(party_commanders[0].item)
	{
		load_item(party_commanders[0].item,2);
		display_item(0,2,11,18);
		display_attribute_string(party_commanders[0].item,(s_x/8)+15,(s_y/8)+18);
	}
	// display_cmdr_info(party[0]-16,15,10);
	display_cmdr_info(0,15,10);
	display_attribute_string(party_items[selected_item],(s_x/8)+25,(s_y/8)+13);

	load_cursor(12,100,SLIDER_ONE);
	reset_menu_state(EQUIP_ITEM_MENU,party_size,0,commander_select_cursor);
}

void item_consume_select()
{
	selected_item = get_item_real_index(commander_select_cursor,menu_state - 5);
	load_item(party_items[selected_item],1);
}

void item_purchase_select()
{
	load_portrait(party_commanders[commander_select_cursor]);
	display_cmdr_info(party_commanders[commander_select_cursor]-16,2,16);
	put_string("Item",(s_x/8)+9,(s_y/8)+20);
	load_item(party_commanders[commander_select_cursor].item,2);
	display_item(0,2,9,22);
	display_attribute_string(party_commanders[commander_select_cursor].item,(s_x/8)+9,(s_y/8)+26);
}

void commander_consume_select()
{
	load_item(party_commanders[commander_select_cursor].item,2);
	display_attribute_string(party_commanders[commander_select_cursor].item,(s_x/8)+15,(s_y/8)+18);
	display_item(0,2,11,18);
	load_portrait(0);
	display_cmdr_info(0,15,10);
	// load_portrait(party[commander_select_cursor]);
	// display_cmdr_info(party[commander_select_cursor]-16,15,10);
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
	// map_get_tile(1,1);
	// put_tile(map_get_tile(1,1),4,20);

	if (j_2 & JOY_LEFT)
	{
		if(menu_state == BUY_CMDR_UNIT_MENU)
		{
			remove_unit_from_cmdr();
		}

		if(menu_state != OVERWORLD)
		{
			// spr_x(spr_get_x()-32);
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
		if(menu_state == BUY_CMDR_UNIT_MENU)
		{
			add_unit_to_cmdr();
		}
		if(menu_state != OVERWORLD)
		{
				return;
				// spr_x(spr_get_x()+32);
				// return;
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
				// put_number(commander_select_cursor,4,10,40);
				spr_set(1);
				if(commander_select_cursor > 0) //&& menu_state != BUY_CMDR_UNIT_MENU)
				{
					commander_select_cursor--;
          curs_up();
//					spr_y(spr_get_y()-8);

					if(menu_state == RECRUIT_MENU)
					{
						load_item(buyable_items[commander_select_cursor],1);
						display_attribute_string(buyable_items[commander_select_cursor],(s_x/8)+18,(s_y/8)+18);
					}
					if(menu_state == BUY_CMDR_UNIT_MENU)
					{
						// add_unit_to_cmdr();
						int ypos;
						ypos = spr_get_y() - 24;
						spr_y(ypos);
						spr_set(2);
						spr_y(ypos);
						row_selected--;
					}
					else if(menu_state == CONSUME_ITEM_MENU || menu_state == EQUIP_ITEM_MENU)
					{
						commander_consume_select();
					}
					else if(menu_state == UNIT_MENU)
					{
						// display_unit_stats(troops[commander_select_cursor],5,14,submenu_state);
					}
					else if(menu_state == BUY_SELECT_MENU)
					{
						// display_army_info(party[commander_select_cursor]-16,17,13);
						display_army_info(0,17,13);
						item_purchase_select();
					}
					else if(menu_state == BUY_ITEM_MENU)
					{
						item_purchase_select();
					}
					else if(menu_state == CONSUME_MENU || menu_state == EQUIP_MENU)
					{
						item_consume_select();
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
				spr_set(1);
				if(commander_select_cursor < (menu_options-1)) //&& menu_state != BUY_CMDR_UNIT_MENU)
				{
					commander_select_cursor++;
          curs_down();
//					spr_y(spr_get_y()+8);

					if(menu_state == RECRUIT_MENU)
					{
						load_item(buyable_items[commander_select_cursor],1);
						display_attribute_string(buyable_items[commander_select_cursor],(s_x/8)+18,(s_y/8)+18);
					}
					else if(menu_state == CONSUME_ITEM_MENU || menu_state == EQUIP_ITEM_MENU)
					{
						commander_consume_select();
					}
					if(menu_state == BUY_CMDR_UNIT_MENU)
					{
						int ypos;
						ypos = spr_get_y() + 24;
						spr_y(ypos);
						spr_set(2);
						spr_y(ypos);
						row_selected++;
					}
					else if(menu_state == BUY_SELECT_MENU)
					{
						// display_army_info(party[commander_select_cursor]-16,17,13);
						display_army_info(0,17,13);
						item_purchase_select();
					}
					else if(menu_state == BUY_ITEM_MENU)
					{
						item_purchase_select();
					}
					else if(menu_state == CONSUME_MENU || menu_state == EQUIP_MENU)
					{
						item_consume_select();
					}
				}
			return;
		}
		return;
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
			spr_set(1);
			set_font_pal(11);
			print_item_name(buyable_items[commander_select_cursor],2,3,commander_select_cursor);
			set_font_pal(10);
			select_buyable_item(buyable_items[commander_select_cursor]);
			// load_portrait(party[0]);
			load_portrait(0);

			display_item(0,0,2,14);
			// display_cmdr_info(party[0]-16,2,16);
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
			last_selected_cursor = commander_select_cursor;
			spr_set(1);
			set_font_pal(11);
			print_unit_data(buyable_units[commander_select_cursor],commander_select_cursor,0,1,1);
			set_font_pal(10);
			select_buyable_unit(buyable_units[commander_select_cursor]);
			load_portrait(0);
			display_item(0,0,2,14);
		}
		else if(menu_state == BUY_SELECT_MENU)
		{
			select_cmdr_for_unit();
		}
		else if(menu_state == CASTLE_MENU)
		{
			switch(commander_select_cursor)
			{
				case 0:
				// spr_set(1);
				// spr_y(-32);
				// satb_update();
				// display_shop_menu(current_selected_castle);
				break;

				case 1:
				// display_commanders_in_castle(current_selected_castle);
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
		else if(menu_state == CONSUME_MENU)
		{
			item_swap_menu();
		}
		else if(menu_state == CONSUME_ITEM_MENU)
		{
			// consume_item(commander_select_cursor,last_selected_cursor,CONSUMABLE);
			display_inventory(0,8,CONSUMABLE);
			load_cursor(66,20,SLIDER_ONE);
			last_selected_cursor = 0;
		}
		else if(menu_state == EQUIP_MENU)
		{
			item_swap_menu();
		}
		else if(menu_state == EQUIP_ITEM_MENU)
		{
			equip_item(party_commanders[commander_select_cursor].sprite_type-16,last_selected_cursor,EQUIPABLE);
			display_inventory(0,8,EQUIPABLE);
			load_cursor(66,20,SLIDER_ONE);
			last_selected_cursor = 0;
		}
		else if(menu_state == BUY_ITEM_MENU)
		{
			// buy_item(selected_item,party[commander_select_cursor]-16);
			buy_item(selected_item,0);
			load_item(party_commanders[commander_select_cursor].item,2);
			display_item(0,2,9,22);
			display_attribute_string(party_commanders[commander_select_cursor].item,(s_x/8)+9,(s_y/8)+26);
			put_number(player_gold,6,16+(s_x/8),10+(s_y/8));
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
			spr_set(1);
			spr_hide();
			spr_show(0);
			hide_npcs(5);
			reset_npcs();
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
			spr_set(1);
			spr_y(20+(8*commander_select_cursor));
			spr_x(4);
		}
		if(menu_state == EQUIP_MENU || menu_state == CONSUME_MENU)
		{
			display_info_panel(0,8,24,12);
			load_cursor(0,4,SLIDER_ONE);
			reset_menu_state(OVERWORLD_MENU,5,0,0);
			display_info_panel(12,0,32,16);
			return;
		}
		if(menu_state == EQUIP_ITEM_MENU)
		{
			commander_select_cursor = last_selected_cursor;
			display_inventory(0,8,EQUIPABLE);
			display_info_panel(12,0,32,16);
			last_selected_cursor = 0;
			load_cursor(66,20,SLIDER_ONE);
			return;
		}
		if(menu_state == CONSUME_ITEM_MENU)
		{
			commander_select_cursor = last_selected_cursor;
			display_inventory(0,8,CONSUMABLE);
			display_info_panel(12,0,32,16);
			last_selected_cursor = 0;
			load_cursor(66,20,SLIDER_ONE);
			return;
		}
		if(menu_state == BUY_SELECT_MENU)
		{
			commander_select_cursor = selected_unit;
			spr_set(1);
			spr_x(4);
			spr_y(28+(commander_select_cursor*8));
			print_unit_data(buyable_units[commander_select_cursor],commander_select_cursor,0,1,1);
			menu_state = SHOP_MENU;
			menu_options = num_of_buyable_units;
			hide_npcs(5);
			reset_npcs();
			satb_update();
			display_info_panel(12,0,16,16);
			display_info_panel(12,16,16,16);
			return;
		}
		if(menu_state == BUY_CMDR_UNIT_MENU)
		{
			commander_select_cursor = last_selected_cursor;
			menu_state = BUY_SELECT_MENU;
			menu_options = party_size;
			put_string(party_commanders[commander_select_cursor].name,25,(s_y/8)+2+commander_select_cursor);

			load_cursor(186,12+(8*commander_select_cursor),CURSOR);
			spr_hide(SLIDER_TWO);
		}
	}

	// if(j_2 & JOY_RUN)
	// {
	// }
	// if(j_2 & JOY_SEL)
	// {
	// }
}

clear_commander_select()
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
