#include "pathing.c"
#include "npc.c"
#include "battlefield.c"

#define COMMANDER_SELECT_VRAM 0x4400
#define PANEL_VRAM 0x2000
#define MAX_DEPLOYMENTS 3
#define DEPLOY_OFFSET 50
#define CASTLE_VRAM_START 0x2310
#define DEFAULT_SPRITE_NUMBER 50

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
#define BUY_AMOUNT_MENU 13
#define SECONDS 60

char number_of_castles;
char number_of_commanders;

char initialized = 0;
char chk = 0;
char map_y_offset = 0;
char menu_state = 0;
char menu_options = 0;
char submenu_state = 0;

//party
enum Direction party_direction;
char party_moving = 0;
int party_pos_x = 2;
int party_pos_y = 31;
int party_pos_last = 0;
char party_walk = 0;
int party_last_pos = 0;

char selected_unit = 0;
char cmdr_selected = 0;
char cycler = 0;
char current = 0;
char current_command_group = 0;
char current_selected_castle = 0;
int commander_select_cursor = 0;
int last_selected_cursor = 0;
char overworld_state = OVERWORLD;
char hz = 60;
int secs = 180;

void overworld_loop()
{
	char i, s;
	merge_units[0] = 0;
	merge_units[1] = 0;
	merge_pointer = 0;
	num_of_spawns = 0;
	s = script[0];
	party_direction = NORTH;
	i = 0;
	chk = 1;
	s_y = 288;
	selector_y = 208-16;
	selector_x = 32;
	map_y_offset = 0;//12;
	cursor_vram = 0x5080;
	// spawn_pos = 173;

	// load_palette(18,alphabetpal,1);
	disp_off();
  load_overworld_bg();
	// load_units(0x3800);
	// load_palette_groups();

	spawn_starting_enemies();

	// write_text(5,40,script);
	for(;;)
	{
		vsync();
		if(menu_state == 0)
		{
			tick();
			// timer();

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
							display_info_panel(1,18,13,3);
							// put_number(pos,4,(s_x/8)+19,(s_y/8)+2);
							put_string(location_lookup(pos),(s_x/8)+19,(s_y/8)+2);
							party_moving = 0;
							party_walk = 0;
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
		if(party_walk == 0)
		{
			int id;
			id = get_castle_no_by_pos(party_pos_x,party_pos_y);

			if(id > 0 && castles[id].no_of_commanders > 0)
			{
				party_moving = 0;
				party_walk = 0;

				// put_number(get_castle_no_by_pos(party_pos_x,party_pos_y),3,3,45);
				begin_battlefield(id,castles[id].commanders,3);
				castles[id].no_of_commanders = 0;
				// load_units(0x3800);
				display_spawns();
				load_overworld_bg();
			}
		}
		if(!party_moving)
		{
			overworld_controls();
		}
		// overworld_controls();
		satb_update();
	}
}

void hide_spawns(){}

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
	load_npcs(area_one);
}

void display_spawns()
{
}

void load_unit(char id, int address, int index)
{
}

void display_unit(int x, int y, int spr_no, int index)
{
	spr_make(spr_no+index,x,y,RECRUIT_VRAM+(index*0x100),FLIP_MAS|SIZE_MAS,SZ_16x32,25+index,1);
}

void spawn_enemy(char spawn_id, char unit_id, char atk, char def, char hp, char spd, char level)
{
	// int ran_n=0;
	// enemy_spawns[spawn_id] = current_global_units;
	// add_unit_to_pool(1,15,0,unit,atk,def,hp,spd);
	// enemy_spawns[spawn_id] = unit_id;
}

void randomize_spawn(int level)
{
	// int r_swds, r_sprs, i, j, hp;
	//
	// r_swds = rand() & 3;
	// r_sprs = 3 - r_swds;
	// for(i=0; i<r_swds; i++)
	// {
	// 	enemy[i] = HOUND_UNIT;
	// }
	// for(i=0; i<r_sprs; i++)
	// {
	// 	enemy[i+r_swds] = SPEAR_UNIT;
	// }
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
	char i;

	total_sprites = 0;
	cursor_vram = 0x5080;
	set_screen_size(SCR_SIZE_64x64);
	screen_dimensions = 64;

	set_map_data(overworldmap,MAP_WIDTH,32);
	set_tile_data(overworldtiles,124,overworldtilespal,TILE_WIDTH);

	load_tile(TILE_VRAM);

	load_palette(0,overworldpal,7);
	// load_palette(8,panel_pal,1);
	// load_palette(9,castlepal,1);
	load_palette(10,fontpal,2);
	load_palette(8,borderspal,1);

	load_map(0,0,0,0,MAP_WIDTH,OVERWORLD_MAP_HEIGHT);
	load_vram(0x6400,borders,0x100);

	set_font_pal(10);
	load_font(font,125,0x2F00);

	load_vram(0x5300,cursor,0x40);
	cursor_vram = 0x5300;
	load_vram(0x6A00,rei_walk,0x300);
	load_palette(16,rei_walk_pal,1);
	// display_sprite(selector_x,selector_y,0x5000,FLIP_MAS|SIZE_MAS,0);
	spr_make(0,selector_x,selector_y,0x6A00,FLIP_MAS|SIZE_MAS,SZ_16x32,0,1);

	// load_castle_bat();
	// display_castles();

	if(initialized == 0)
	{
		init_overworld_data();
		number_of_castles = 3;
		number_of_commanders = 6;
	}
	else
	{
		load_vram(0x5000,rei_walk,0x300);
	}
	disp_on();
	scroll(0,s_x,288,0, 223, 0xC0);
	vsync();
	satb_update();
}

void display_inventory(int x, int y, char item_type)
{
	int j=0, i=0;
	display_info_panel(x,y,24,12);
	put_string("Inventory",y+6+(s_x/8),x+1+(s_y/8));

	load_item(party_items[0],1);
	// load_item(party_items[get_item_real_index(0,item_type)],1);
	// display_item(0,1,3,26);
	// put_string(items[party_items[get_item_real_index(0,item_type)]].description,25,45);

	for(j=0; j<no_of_party_items; j++)
	{
		if(item_type == ANY_ITEM || items[party_items[j]].type == item_type)
		{
			print_item_in_inventory(j,y+2,x+3,i++);
		}
	}

	menu_options = i;
	commander_select_cursor = 0;
	menu_state = CONSUME_MENU + (item_type & 1);
}

void display_units(int x, int y)
{
	int j;
	display_info_panel(0,8,24,12);
	// put_string("Army",18+(s_x/8),1+(s_y/8));

	// load_unit(&global_units[party[0]],RECRUIT_VRAM,0);
	// display_unit(16,104,DEFAULT_SPRITE_NUMBER,0);
	// display_unit_stats(&global_units[party[0]],5,14,0);
	for(j=0; j<no_of_troops; j++)
	{
		print_unit_data(troops[j],j,8,0,0);
	}
	menu_options = no_of_troops;
	commander_select_cursor = 0;
	menu_state = UNIT_MENU;
}

void init_overworld_data()
{
	// castle = castles;
	//TAROS
	load_castle_data(0,100,2,31,1,TAROS);
	load_castle_data(1,100,3,19,1,TAROS);
	load_castle_data(2,100,12,24,1,TAROS);
	//KODEN
	load_castle_data(3,100,13,10,1,KODEN);
	load_castle_data(4,100,10,17,1,KODEN);
	load_castle_data(5,100,18,15,1,KODEN);
	//BAKURIA
	load_castle_data(6,100,0,25,1,BAKURIA);
	load_castle_data(7,100,1,1,1,BAKURIA);
	// load_castle_data(8,100,2,31,1,BAKURIA);
	//SANGA
	load_castle_data(9,100,12,24,1,SANGAI);
	load_castle_data(10,100,13,28,1,SANGAI);
	load_castle_data(11,100,21,24,1,SANGAI);
	//KAYTOKEN
	load_castle_data(12,100,21,0,1,KAYTOKEN);
	load_castle_data(13,100,26,2,1,KAYTOKEN);
	load_castle_data(14,100,24,8,1,KAYTOKEN);
	// load_castle_data(15,100,7,28,1,KAYTOKEN);

	// castles[8].no_of_items = 3;
	initialized = 1;
}

char add_commander_to_castle(char cmdr_id, char castle_no)
{
	char current_cmdrs;
	current_cmdrs = castles[castle_no].no_of_commanders;
	if(current_cmdrs == MAX_COMMANDERS)
	{
		return 0;
	}
	castles[castle_no].commanders[current_cmdrs] = cmdr_id;
	castles[castle_no].no_of_commanders = current_cmdrs + 1;
	// commanders[cmdr_id].kingdom = castles[castle_no].owner;
	return 1;
}

void load_castle_data(int index, char population, char x, char y,
                      char level, char kingdom)
{
	// castles[index].no_of_soldiers = population;
	castles[index].x = x;
	castles[index].y = y;
	// castles[index].
	// castles[index].owner = kingdom;
}

void display_overworld_menu()
{
	int x,y, i;
	x = (s_x/8);
	y = (s_y/8);
	// spr_hide(10);
	// spr_hide(11);
	spr_hide(0);
	hide_spawns();
	// satb_update();

	display_info_panel(0,0,8,12);
	display_info_panel(0,8,24,12);
	display_info_panel(12,0,32,16);
	put_string("Items",x+2,y+1);
	put_string("Equip",x+2,y+2);
	put_string("Merge",x+2,y+3);
	put_string("Order",x+2,y+4);
	put_string("Stats",x+2,y+5);

	menu_state = OVERWORLD_MENU;
	menu_options = 5;
	commander_select_cursor = 0;
}

void display_commanders_in_castle(char castle_no)
{
	char j, id;
	display_info_panel(0,0,23,12);

	put_string("Recruit",10+(s_x/8),1+(s_y/8));
	put_string("Unit",(s_x/8)+2,(s_y/8)+3);
	put_string("Cost",(s_x/8)+11,(s_y/8)+3);
	put_string("Lvl",(s_x/8)+17,(s_y/8)+3);

	display_info_panel(0,23,9,12);
	display_info_panel(12,4,24,8);
	// put_string("Cost",18+(s_x/8),9+(s_y/8));
	// put_number(buy_total,6,16+(s_x/8),10+(s_y/8));
	put_string("Gold",1+(s_x/8),9+(s_y/8));
	put_number(player_gold,5,(s_x/8)+1,10+(s_y/8));

	// vsync(3); //not sure why this is here
	if(castle_empty(current_selected_castle))
	{
		spr_set(1);
		spr_y(-32);
		spr_set(DEFAULT_SPRITE_NUMBER);
		spr_y(-32);
	}
	else
	{
		load_unit(buyable_units[0],RECRUIT_VRAM,0);
		display_unit(216,8,DEFAULT_SPRITE_NUMBER,0);
		spr_set(1);
		spr_x(4);
		spr_y(28);
	}

	for(j=0; j<num_of_buyable_units; j++)
	{
		print_unit_data(buyable_units[j],j,0,1,1);
	}

	// for(j=0; j<no_of_troops; j++)
	// {
	// 	print_unit_data(troops[j],j,3,14,0);
	// }

	for(j=0; j<party_size; j++)
	{
		put_string(commanders[party[j]-16].name,25,2+(s_y/8)+j);
	}

	menu_state = SHOP_MENU;
	commander_select_cursor = 0;
	menu_options = num_of_buyable_units;
	satb_update();
}

void display_shop_menu(char castle_no)
{
	char i;
	display_info_panel(0,0,24,12);
	put_string("Shop",10+(s_x/8),1+(s_y/8));
	// display_info_panel(13,4,24,12);
	// put_string("Inventory",10+(s_x/8),14+(s_y/8));
	// display_cmdr_info_panel(s_x,s_y,24);
	display_info_panel(0,24,8,12);
	display_inventory(13,4,ANY_ITEM);

	if(castles[castle_no].no_of_items == 0)
	{
		spr_set(1);
		spr_y(-32);
	}
	else
	{
		load_item(castles[castle_no].buyable_items[0],0);
		display_item(0,0,1,26);

		put_string("Gold",18+(s_x/8),9+(s_y/8));
		put_number(player_gold,6,17+(s_x/8),10+(s_y/8));
		spr_set(1);
		spr_y(20);
	}

	for(i=0; i<castles[castle_no].no_of_items; i++)
	{
		print_commander_stats(castles[castle_no].buyable_items[i],i);
	}

	for(i=0; i<party_size; i++)
	{
		put_string(commanders[party[i]-16].name,26,(s_y/8)+i);
	}

	menu_options = castles[castle_no].no_of_items;
	menu_state = RECRUIT_MENU;
	commander_select_cursor = 0;
	satb_update();
}

void display_castle_menu(char castle_no)
{
	hide_npcs(5);

	// put_number(party_pos_x,3,10,40);
	// put_number(party_pos_y,3,10,41);

	display_info_panel(0,0,8,12);
	put_string("Info",2+s_x/8,1+s_y/8);
	put_string("Shop",2+s_x/8,2+s_y/8);
	put_string("Units",2+s_x/8,3+s_y/8);
	put_string("Merge",2+s_x/8,4+s_y/8);
	put_string("Rest",2+s_x/8,5+s_y/8);

	load_cursor(2,4);
	menu_state = CASTLE_MENU;
	menu_options = 5;
	current_selected_castle = castle_no;
	// hide_npcs(5);
}

void print_item_in_inventory(int i, int x_off, int y_off, char index)
{
	int x, y;
	x = x_off+(s_x/8);
	y = y_off+(s_y/8);

	put_string(items[party_items[i]].name,x+((index/8) * 8),y+(index%8));
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
	print_unit_type(unit_list[unit_id].unit_type,x+((i/7) * 7),y+(i%7));
	// put_number(unit_list[unit_id].id,3,x+((i/7) * 7),y+(i%7));

	if(disp_cost)
	{
		put_number(get_unit_cost(buyable_units[unit_id]),3,x+9,y+i);
		// put_number(get_unit_cost(unit_list[unit_id]),3,x+9,y+i);
		// put_number(gentity->level,2,x+14,y+i);
	}
	else
	{
		// put_number(gentity->level,2,x+3+((i/7) * 7),y+(i%7));
	}
}

void print_commander_data(char cmdr_id)
{
	int x, y;
	x = 23+((s_x>>1)/8);
	y = 5+((s_y>>1)/8);
	put_string("       ",x+2,y);
	put_string(commanders[cmdr_id].name,x+2,y);
	put_string("HP",x+4,y+1);
	put_char('/',x+4,y+2);

	put_string("AP",x+4,y+3);
	put_number(commanders[cmdr_id].ap,3,x+1,y+4);
	put_char('/',x+4,y+4);
	put_number(commanders[cmdr_id].ap,3,x+5,y+4);
}

void print_castle_data(char castle_no)
{
	char i;
	int x, y;
	x = 2+(s_x/8);
	y = 4+(s_y/8);

	// put_number(castles[castle_no].no_of_soldiers,3,25,4);
	put_char('/',28,4);
	put_number(100,3,29,4);

	// put_string("level",26,6);
	// put_number(castles[castle_no].level,2,27,7);
	//room for more castle info if necessary
}

char buy_item(char item_index, char castle_id)
{
	int cost;
	char item_id;
	item_id = castles[castle_id].buyable_items[item_index];
	// cost = items[item_id].cost;
	cost = 100;
	if(cost > player_gold){ return 0; }
	if(no_of_party_items == MAX_INVENTORY ){ return 0; }
	player_gold -= cost;
	party_items[no_of_party_items++] = item_id;
	return 1;
}

char buy_unit(char unit_index, char castle_id)
{
	char unit;
	int cost;
	unit = buyable_units[unit_index];
	cost = get_unit_cost(unit);
	if(cost > player_gold){ return 0; }
	if(no_of_troops == 14 ){ return 0; }
	player_gold -= cost;
	troops[no_of_troops++] = unit;
	return 1;
}

void select_buyable_unit(char unit_index)
{
	selected_unit = unit_index;
	menu_state = BUY_SELECT_MENU;
	menu_options = party_size;
	commander_select_cursor = 0;
	reset_npcs();
	display_army_info(party[0]-16,12,13);

	spr_set(1);
	spr_y(12);
	spr_x(186);
	satb_update();
}

void remove_unit_from_cmdr()
{
	int cost;
	char cmdr_id, unit_count, min;
	cost = get_unit_cost(selected_unit);
	cmdr_id = cmdr_selected;

	min = 0;
	unit_count = commanders[cmdr_id].no_of_units;
	if(unit_count > min)
	{
		// buy_total -= cost;
		player_gold += cost;

		commanders[cmdr_id].no_of_units--;
		display_cmdr_army_info(cmdr_id,12,13);
		hide_npcs(5);
		destroy_npc(npc_count-1);
		show_npcs(5);
		// put_number(buy_total,6,16+(s_x/8),10+(s_y/8));
		put_number(player_gold,5,(s_x/8)+1,10+(s_y/8));
	}
}

void add_unit_to_cmdr()
{
	int cost;
	char cmdr_id, unit_count, max;
	cost = get_unit_cost(selected_unit);
	cmdr_id = cmdr_selected;

	max = commanders[cmdr_id].max_units;
	unit_count = commanders[cmdr_id].no_of_units;

	if(unit_count < max && player_gold > cost)
	{
		player_gold -= cost;
		commanders[cmdr_id].units[unit_count] = selected_unit;
		commanders[cmdr_id].no_of_units++;
		display_cmdr_army_info(cmdr_id,12,13);
		// put_number(buy_total,6,16+(s_x/8),10+(s_y/8));
		put_number(player_gold,5,(s_x/8)+1,10+(s_y/8));
		load_cmdr_army_to_npcs(cmdr_id);
		draw_npcs(5);
	}
}

void display_army_info(char cmdr_id, int x, int y)
{
	cmdr_selected = cmdr_id;
	display_cmdr_army_info(cmdr_id,12,13);
	load_cmdr_army_to_npcs(cmdr_id);
	load_portrait(cmdr_id+16);
	draw_npcs(5);
}

void load_cmdr_army_to_npcs(char cmdr_id)
{
	char i;
	int x,y;
	hide_npcs(5);
	// clear_npcs();
	reset_npcs();
	x = 6;
	y = 7;
	for(i=0; i<commanders[cmdr_id].no_of_units; i++)
	{
		add_npc(x+i,y,commanders[cmdr_id].units[i],UNIT_PALS[commanders[cmdr_id].units[i]]);
	}
}

// void reset_cost()
// {
// 	buy_total = 0;
// }

void remove_unit_stats(int x, int y, int index)
{
	int _sx, _sy, y_off;
	_sx = s_x / 8 + x;
	_sy = s_y / 8 + y;
	y_off = index * 4;
	_sy += y_off;

	spr_hide(DEFAULT_SPRITE_NUMBER+index);

	put_string("   ",_sx,_sy);
	put_string("  ",_sx+4,_sy);

	put_string("   ",_sx,_sy+1);
	put_string("  ",_sx+4,_sy+1);

	put_string("   ",_sx,_sy+2);
	put_string("  ",_sx+4,_sy+2);
}

void display_unit_stats(char id, int x, int y, int index)
{
	int _sx, _sy, y_off;
	_sx = s_x / 8 + x;
	_sy = s_y / 8 + y;
	y_off = index * 4;
	_sy += y_off;

	load_unit(id,RECRUIT_VRAM,index);
	// display_unit(16,104+(index*32),DEFAULT_SPRITE_NUMBER,index);
	display_unit(x*8-16-4,y*8+(index*32)-8,DEFAULT_SPRITE_NUMBER,index);

	put_string("Atk",_sx,_sy);
	put_number(unit_list[id].atk,2,_sx+4,_sy);

	put_string("Def",_sx,_sy+1);
	put_number(unit_list[id].def,2,_sx+4,_sy+1);

	put_string("Spd",_sx,_sy+2);
	put_number(unit_list[id].spd,2,_sx+4,_sy+2);

	// put_string("EX:",_sx,_sy+3);
	// put_number(gentity->exp,4,_sx+4,_sy+3);
}

void remove_unit_from_castle(char unit_index, char castle_id)
{
	// char i;
	// for(i=unit_index; i<castles[castle_id].no_of_units; i++)
	// {
	// 	castles[castle_id].buyable_units[i] = castles[castle_id].buyable_units[i+1];
	// }
	// castles[castle_id].no_of_units--;
}

void swap_units(int index_one, int index_two)
{
	char tmp;
	tmp = troops[index_one];
	troops[index_one] = troops[index_two];
	troops[index_two] = tmp;
}

void castle_empty(char castle_no){ return 0;}

char get_map_id_by_pos(int pos)
{
	char i;
	for(i=0; i<NO_OF_MAPS; i++)
	{
		if(battle_map_metadata[i].pos == pos)
		{
			return i;
		}
	}
	return -1;
}

char select_castle_by_cursor()
{
	char x, y;

	x = (selector_x + (s_x >> 1)) / 16;
	y = ((selector_y + (s_y >> 1)) / 16) - map_y_offset;

	return get_castle_no_by_pos(x,y);
}

char get_castle_no_by_pos(char x, char y)
{
	int j;
	for(j = 0; j < NO_OF_CASTLES; j++)
	{
		// if(castles[j].x == x && castles[j].y == y)
		if(castles[j].x == party_pos_x && castles[j].y == party_pos_y)
		{
			current_selected_castle = j;
			return j;
		}
	}
	return -1;
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
		// if(menu_state == 1 || menu_state == 2)
		if(menu_state == BUY_SELECT_MENU)
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
		// if(menu_state == 1 || menu_state == 2)
		if(menu_state == BUY_SELECT_MENU)
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
		if(menu_state == BUY_AMOUNT_MENU)
		{
			return;
		}
		if(menu_state != 0)
		{
				spr_set(1);
				// put_number(commander_select_cursor,4,10,40);
				if(commander_select_cursor > 0  && submenu_state < 2)
				{
					commander_select_cursor--;
					spr_y(spr_get_y()-8);

					if(menu_state == RECRUIT_MENU)
					{
						load_item(castles[current_selected_castle].buyable_items[commander_select_cursor],0);
						display_item(0,0,1,26);
					}
					else if(menu_state == SHOP_MENU)
					{
						// load_unit(castles[current_selected_castle].buyable_units[commander_select_cursor],RECRUIT_VRAM,0);
						load_unit(buyable_units[commander_select_cursor],RECRUIT_VRAM,0);
						display_unit(216,8,DEFAULT_SPRITE_NUMBER,0);
					}
					else if(menu_state == CONSUME_ITEM_MENU || menu_state == EQUIP_ITEM_MENU)
					{
						load_portrait(party[commander_select_cursor]);
						display_cmdr_info(party[commander_select_cursor]-16,21,13);
					}
					else if(menu_state == UNIT_MENU)
					{
						display_unit_stats(troops[commander_select_cursor],5,14,submenu_state);
					}
					else if(menu_state == ORDER_MENU)
					{
						display_unit_stats(troops[commander_select_cursor],25,3,0);
					}
					else if(menu_state == BUY_SELECT_MENU)
					{
						display_army_info(party[commander_select_cursor]-16,12,13);
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
				// put_number(commander_select_cursor,4,10,40);
				if(commander_select_cursor < (menu_options-1) && submenu_state < 2)
				{
					commander_select_cursor++;
					spr_y(spr_get_y()+8);

					if(menu_state == RECRUIT_MENU)
					{
						load_item(castles[current_selected_castle].buyable_items[commander_select_cursor],0);
						display_item(0,0,1,26);
					}
					else if(menu_state == SHOP_MENU)
					{
						// load_unit(castles[current_selected_castle].buyable_units[commander_select_cursor],RECRUIT_VRAM,0);
						load_unit(buyable_units[commander_select_cursor],RECRUIT_VRAM,0);
						display_unit(216,8,DEFAULT_SPRITE_NUMBER,0);
					}
					else if(menu_state == CONSUME_ITEM_MENU || menu_state == EQUIP_ITEM_MENU)
					{
						load_portrait(party[commander_select_cursor]);
						display_cmdr_info(party[commander_select_cursor]-16,21,13);
					}
					else if(menu_state == UNIT_MENU)
					{
						display_unit_stats(troops[commander_select_cursor],5,14,submenu_state);
					}
					else if(menu_state == ORDER_MENU)
					{
						display_unit_stats(troops[commander_select_cursor],25,3,0);
					}
					else if(menu_state == BUY_SELECT_MENU)
					{
						display_army_info(party[commander_select_cursor]-16,12,13);
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
		if(menu_state == RECRUIT_MENU) //recruit units
		{
			int item;
			if(castles[current_selected_castle].no_of_items < 1){ return ;}
			spr_set(1);

			item = commander_select_cursor;
			if(buy_item(item,current_selected_castle))
			{
				display_shop_menu(current_selected_castle);
			}
		}
		else if(menu_state == SHOP_MENU)
		{
			// if(num_of_buyable_units < 1){ return ;}
			last_selected_cursor = commander_select_cursor;
			spr_set(1);
			set_font_pal(11);
			print_unit_data(buyable_units[commander_select_cursor],commander_select_cursor,0,1,1);
			set_font_pal(10);
			select_buyable_unit(commander_select_cursor);
			load_portrait(party[0]);
			display_item(0,0,5,14);
			// if(buy_unit(unit,current_selected_castle))
			// {
			// 	display_commanders_in_castle(current_selected_castle);
			// }
		}
		else if(menu_state == OVERWORLD_MENU)
		{
			// hide_npcs(5);
			switch(commander_select_cursor)
			{
				case 0:
				display_inventory(0,8,CONSUMABLE);
				load_cursor(66,20);
				break;
				case 1:
				display_inventory(0,8,EQUIPABLE);
				load_cursor(66,20);
				break;
				case 2:
				display_units(0,0);
				display_unit_stats(troops[0],5,14,0);
				load_cursor(66,20);
				break;
				case 3:
				display_units(0,0);
				display_unit_stats(troops[0],25,3,0);
				menu_state = ORDER_MENU;
				load_cursor(66,20);
				break;
				case 4:
				put_number(commanders[0].unit.atk,3,10,42);
				put_number(commanders[0].unit.def,3,10,43);
				put_number(commanders[0].unit.spd,3,10,44);
				put_number(commanders[0].unit.hp,3,10,45);
				break;
			}
		}
		else if(menu_state == CASTLE_MENU)
		{
			switch(commander_select_cursor)
			{
				case 1:
				spr_set(1);
				spr_y(-32);
				satb_update();
				display_shop_menu(current_selected_castle);
				break;

				case 2:
				display_commanders_in_castle(current_selected_castle);
				break;

				case 4:
				// heal_party();
				break;
			}
		}
		else if(menu_state == OVERWORLD)
		{
			int pos,id;
			pos = get_absolute_pos();
			if(overworld[pos] < CAST)
			{
				// put_number(pos,4,5,40);
				id = get_map_id_by_pos(pos);
				if(id != -1)
				{
					reset_satb();
					reset_npcs();
					begin_explore(id);

					display_spawns();
					load_overworld_bg();
				}
				else
				{
					put_number(pos,4,5,40);
				}
			}
			else
			{
				char castle_no;
				castle_no = -1;
				castle_no = select_castle_by_cursor();
				current_selected_castle = castle_no;

				if(castle_no != 255)
				{
					spr_hide(0);
					hide_spawns();
					display_castle_menu(castle_no);
					current_selected_castle = castle_no;
					return;
				}
			}
		}
		else if(menu_state == CONSUME_MENU)
		{
			int z;
			load_portrait(0);
			for(z=0; z<party_size; z++)
			{
				put_string(commanders[party[z]-16].name,3,52+z);
			}
			display_item(0,0,16,16);
			display_cmdr_info(party[0]-16,21,13);
			load_cursor(12,124);
			reset_menu_state(CONSUME_ITEM_MENU,party_size,0,commander_select_cursor);
		}
		else if(menu_state == CONSUME_ITEM_MENU)
		{
			consume_item(commander_select_cursor,last_selected_cursor,CONSUMABLE);
			display_inventory(0,8,CONSUMABLE);
			load_cursor(66,20);
			last_selected_cursor = 0;
		}
		else if(menu_state == EQUIP_MENU)
		{
			int z;
			load_portrait(0);
			for(z=0; z<party_size; z++)
			{
				put_string(commanders[party[z]-16].name,3,52+z);
			}
			display_item(0,0,16,16);
			display_cmdr_info(party[0]-16,21,13);
			load_cursor(12,124);
			reset_menu_state(EQUIP_ITEM_MENU,party_size,0,commander_select_cursor);
		}
		else if(menu_state == EQUIP_ITEM_MENU)
		{
			equip_item(commander_select_cursor,last_selected_cursor,EQUIPABLE);
			display_inventory(0,8,EQUIPABLE);
			load_cursor(66,20);
			last_selected_cursor = 0;
		}
	}

	if(j_2 & JOY_II)
	{
		if(menu_state == RECRUIT_MENU || menu_state == SHOP_MENU)
		{
			// clear_party_units();
			// reset_cost();
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
			display_spawns();
			satb_update();
			reset_menu_state(OVERWORLD,0,0,0);
			return;
		}
		if(menu_state == OVERWORLD_MENU)
		{
			clear_commander_select();
			spr_show(0);
			display_spawns();
			reset_menu_state(OVERWORLD,0,0,0);
			return;
		}
		if(menu_state == EQUIP_MENU || menu_state == CONSUME_MENU)
		{
			display_info_panel(0,8,24,12);
			load_cursor(0,4);
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
			load_cursor(66,20);
			return;
		}
		if(menu_state == CONSUME_ITEM_MENU)
		{
			commander_select_cursor = last_selected_cursor;
			display_inventory(0,8,CONSUMABLE);
			display_info_panel(12,0,32,16);
			last_selected_cursor = 0;
			load_cursor(66,20);
			return;
		}
		if(menu_state == BUY_SELECT_MENU)
		{
			commander_select_cursor = last_selected_cursor;
			spr_set(1);
			spr_x(4);
			spr_y(28+(commander_select_cursor*8));
			print_unit_data(buyable_units[commander_select_cursor],commander_select_cursor,0,1,1);
			menu_state = SHOP_MENU;
			menu_options = num_of_buyable_units;
			hide_npcs(5);
			reset_npcs();
			satb_update();
			display_info_panel(12,4,24,8);
			return;
		}
		if(menu_state == UNIT_MENU)
		{
			if(submenu_state > 0)
			{
				if(submenu_state == 2)
				{
					current_global_units--;
					remove_unit_stats(5,14,submenu_state--);
					set_font_pal(10);
					print_unit_data(troops[commander_select_cursor],commander_select_cursor,8,0,0);
					load_cursor(66,20+(8*commander_select_cursor));
				}
				else
				{
					remove_unit_stats(5,14,submenu_state--);
					commander_select_cursor = last_selected_cursor;
					set_font_pal(10);
					print_unit_data(troops[commander_select_cursor],commander_select_cursor,8,0,0);
					load_cursor(66,20+(8*commander_select_cursor));
				}
			}
			else
			{
				set_font_pal(10);
				print_unit_data(troops[commander_select_cursor],commander_select_cursor,8,0,0);
				display_info_panel(0,8,24,12);
				remove_unit_stats(5,14,submenu_state);
				load_cursor(0,4);
				reset_menu_state(OVERWORLD_MENU,5,0,0);
			}
			return;
		}
		if(menu_state == ORDER_MENU)
		{
			if(submenu_state == 0)
			{
				display_info_panel(0,8,24,12);
				remove_unit_stats(25,3,0);
				load_cursor(0,4);
				reset_menu_state(OVERWORLD_MENU,5,0,0);
			}
			if(submenu_state == 1)
			{
				set_font_pal(10);
				print_unit_data(troops[last_selected_cursor],last_selected_cursor,8,0,0);
				display_unit_stats(troops[last_selected_cursor],25,3,0);
				commander_select_cursor = last_selected_cursor;
				last_selected_cursor = 0;
				load_cursor(66,20+(8*commander_select_cursor));
				submenu_state--;
			}
		}
	}

	if(j_2 & JOY_RUN)
	{
		if(menu_state == SHOP_MENU || menu_state == BUY_SELECT_MENU)
		{
			clear_commander_select();
			display_castle_menu(0);
		}
		else
		{
			hide_npcs(5);
			display_overworld_menu();
			load_cursor(0,4);
			// load_cursor(16,80);
		}
	}

	if(j_2 & JOY_SEL)
	{
		// display_dialog(0,0,"dialog, in this economy \nthat's madness!");
		// put_number(find_offset_by_area(0),4,10,39);
		// story(1);
		// put_number(get_map_id_by_pos(get_absolute_pos()),3,5,39);
		story(get_map_id_by_pos(get_absolute_pos()),PREBATTLE,0);
		clear_commander_select();
	}
}

clear_commander_select()
{
	load_map(0,0,0,0,MAP_WIDTH,OVERWORLD_MAP_HEIGHT);
	// display_castles();
	spr_hide(1);
	spr_hide(DEFAULT_SPRITE_NUMBER);
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
