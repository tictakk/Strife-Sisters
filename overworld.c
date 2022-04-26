#include "pathing.c"
#include "battlefield.c"

// #define FONT_VRAM 0x5200
// #define DIALOGUE_VRAM 0x4000
#define COMMANDER_SELECT_VRAM 0x4400
#define PANEL_VRAM 0x2000
#define MAX_DEPLOYMENTS 15
#define MAX_DEPLOYABLE 5
#define DEPLOY_OFFSET 50
// #define CASTLE_VRAM_START 0x2290
#define CASTLE_VRAM_START 0x2310
// #define CASTLE_VRAM_START 0x3310

#define OVERWORLD 0
#define MENU 1
#define SELECT 2
#define SECONDS 60

char number_of_castles;
char number_of_commanders;

char TOP_SCREEN_SIZE = 0;
char initialized = 0;
char chk = 0;
char map_y_offset = 0;
char menu_state = 0;
char deployment_builder[5];

int commander_select_cursor = 0;

// #incchr(dialoguebox,"map/sprites/dialoguebox.pcx");
// #incpal(dialogueboxpal,"map/sprites/dialoguebox.pcx");
// #incbat(dialoguebox_bat, "map/sprites/dialoguebox.pcx", 0x2100, 0, 0, 32, 8)

struct Deployment{
	char active, path_size, invul, no_of_commanders, kingdom, last_castle, dance;
	int posX, posY, dstX, dstY, walk, castX, castY, absX, absY, sprite_no;
	char commanders[5];
	int path[10];
};

char current_deployments = 0;
char current = 0;
char current_selected_castle = 0;
char current_command_group = 0;
char selector_sprite_no = -1;
char overworld_state = OVERWORLD;
char hz = 60;
int secs = 180;

// char battle_army_one;
// char battle_army_two;

// int darkpal[16];

const char tile_pal_ref[44];

struct Deployment deployments[MAX_DEPLOYMENTS];

void overworld_loop()
{
	int v[1];
	char i;
	TOP_SCREEN_SIZE = 0;//= 96;
	i = 0;
	chk = 1;
	// battle_army_one = 255;
	// battle_army_two = 255;
	s_y = 0;
	selector_y = 0;
	// s_y = 96;
	// selector_y = 96;
	map_y_offset = 0;//12;
	cursor_vram = 0x5080;
	v[0] = 0x110;

	// load_palette(18,alphabetpal,1);
	disp_off();
  load_overworld_bg();
	// (*commanders[0].unit) = &spears;
	// put_number((*commanders[0].unit).hp,8,5,6);
	add_commander_to_castle(0,1);
	commanders[0].kingdom = castles[0].owner;
	add_commander_to_castle(1,1);
	commanders[1].kingdom = castles[0].owner;
	add_commander_to_castle(2,0);
	commanders[2].kingdom = castles[0].owner;
	add_commander_to_castle(3,1);
	commanders[3].kingdom = castles[0].owner;
	add_commander_to_castle(4,1);
	commanders[4].kingdom = castles[0].owner;
	add_commander_to_castle(5,1);
	commanders[5].kingdom = castles[0].owner;
	add_commander_to_castle(6,3);
	commanders[6].kingdom = castles[3].owner;
	add_commander_to_castle(7,3);
	commanders[7].kingdom = castles[3].owner;
	add_commander_to_castle(9,3);
	commanders[9].kingdom = castles[3].owner;
	add_commander_to_castle(8,3);
	commanders[8].kingdom = castles[3].owner;
	add_commander_to_castle(11,6);
	commanders[11].kingdom = castles[6].owner;
	add_commander_to_castle(10,6);
	commanders[10].kingdom = castles[6].owner;
	add_commander_to_castle(12,7);
	commanders[12].kingdom = castles[7].owner;

	// load_castle_bat();
	// display_castles();
	// for(i=0; i<20; i++)
	// {
	// 	put_hex(battlefieldbat[i],6,5,5+i);
	// }
	for(;;)
	{
		vsync();
		if(menu_state == 0)
		{
			tick();
			load_vram(0,v,1);
			// timer();

			if(tic==0)
			{
				check_interactions();
				update_deployments();
				// draw_deployments(0);
			}
			update_kingdom_stats();
		}

		overworld_controls();
		draw_deployments(0);
		satb_update();
	}
}

void change_course(char deploy_no)
{
	char castle_no, i, path_size, posx, posy, dstx, dsty;
	int x_distance, y_distance;
	castle_no = deployments[deploy_no].last_castle;
	dstx = deployments[deploy_no].dstX;
	dsty = deployments[deploy_no].dstY;
	posx = deployments[deploy_no].posX;
	posy = deployments[deploy_no].posY;

	for(i=0;i<20;i++)
	{
		deployments[deploy_no].path[i] = 0;
	}
	deployments[deploy_no].castX = castles[castle_no].x;
	deployments[deploy_no].castY = castles[castle_no].y;
	deployments[deploy_no].posX = dstx;
	deployments[deploy_no].posY = dsty;
	path_size = get_path(((deployments[deploy_no].posY*32)+deployments[deploy_no].posX),
												(castles[castle_no].y*32)+castles[castle_no].x,
												 deployments[deploy_no].path,overworld,0);

	deployments[deploy_no].path_size = path_size;
	deployments[deploy_no].walk = 16 - deployments[deploy_no].walk;
	deployments[deploy_no].dstX = posx;
	deployments[deploy_no].dstY = posy;

	deployments[deploy_no].dstX = deployments[deploy_no].path[path_size] & 31;
	deployments[deploy_no].dstY = deployments[deploy_no].path[path_size] / 32;
	x_distance = deployments[deploy_no].absX - ((deployments[deploy_no].dstX)*16);
	y_distance = deployments[deploy_no].absY - ((deployments[deploy_no].dstY)*16);

	deployments[deploy_no].path_size--;
}

void load_overworld_bg()
{
	char i;

	total_sprites = 0;
	cursor_vram = 0x5080;
	set_screen_size(SCR_SIZE_64x64);

	set_map_data(overworldmap,MAP_WIDTH,OVERWORLD_MAP_HEIGHT);
	set_tile_data(overworldtiles,44,tile_pal_ref,TILE_WIDTH);
	load_tile(TILE_VRAM);
	load_palette(0,overworldpal,1);
	load_palette(1,panel_pal,1);
	load_palette(3,castlepal,1);
	load_map(0,0,0,0,MAP_WIDTH,OVERWORLD_MAP_HEIGHT);

	set_font_color(2, 1);
	set_font_pal(1);
	load_default_font(0,0x1B00);
	// load_vram(FONT_VRAM,alphabet,0x2900);

	load_vram(0x5080,cursor,0x40);
	load_vram(0x5000,selector,0x40);
	load_palette(16,selectorpal,1);
	display_sprite(selector_x,selector_y,0x5000,FLIP_MAS|SIZE_MAS,0);

	load_castle_bat();
	display_castles();

	if(initialized == 0)
	{
		init_overworld_data();
		number_of_castles = 3;
		number_of_commanders = 6;
	}else{
		load_vram(0x5040,walker,0x40);
		for(i=0; i<MAX_DEPLOYMENTS; ++i)
		{
			if(deployments[i].active)
			{
				display_sprite_by_no(deployments[i].absX,deployments[i].absY,0x5040,0,5,deployments[i].sprite_no);
				// total_sprites++;
			}
		}
	}
	disp_on();
	vsync();
	satb_update();
}

// dialog_panel()
// {
// 	int j, vaddr;
// 	int ptr[1];
// 	TOP_SCREEN_SIZE = 64;
// 	map_y_offset = 6;
// 	// s_y = TOP_SCREEN_SIZE * 2;
//
// 	load_palette(2,dialogueboxpal,1);
// 	load_vram(0x2100,dialoguebox,4096);
// 	for(j=0; j<256; j++)
// 	{
// 		vaddr = vram_addr((j&31),(j/32));
// 		ptr[0] = (0x2100 >> 4) + j + 0x2000;
// 		load_vram(vaddr,ptr,1);
// 	}
// 	// load_map(0,2,0,0,MAP_WIDTH,OVERWORLD_MAP_HEIGHT-2);
//
// 	scroll(0, (s_x>>1), (s_y>>1)+32, TOP_SCREEN_SIZE, 223, 0xC0);
// 	scroll(1, 0, 0, 0, TOP_SCREEN_SIZE, 0xC0);
// 	vsync();
// }

display_info_panel()
{
	// TOP_SCREEN_SIZE = 96;
	// map_y_offset = 12;
	int j, vaddr, offset;
	int ptr[1];
	offset = (s_x>>1)/8 + (((s_y>>1)/8)*64);
	load_vram(0x2100, panel_gfx, 4608);
	// put_number((((s_y>>1)/8)*32),4,((s_x>>1)/8)+3,15);
	// load_bat(0x0, panel_bat, 24, 12);
	load_palette(1,panel_pal,2);

	for(j=0; j<288; j++)
	{
		vaddr = vram_addr((j%24),(j/24));
		ptr[0] = (0x2100 >> 4) + j + 0x1000;
		load_vram(vaddr+offset,ptr,1);
	}
}

void init_overworld_data()
{
	// castle = castles;
	//TAROS
	load_castle_data(0,100,0,6,1,TAROS);
	load_castle_data(1,100,4,2,1,TAROS);
	load_castle_data(2,100,0,12,1,TAROS);
	//KODEN
	load_castle_data(3,100,13,10,1,KODEN);
	load_castle_data(4,100,10,17,1,KODEN);
	load_castle_data(5,100,18,15,1,KODEN);
	//BAKURIA
	load_castle_data(6,100,0,25,1,BAKURIA);
	load_castle_data(7,100,5,25,1,BAKURIA);
	load_castle_data(8,100,2,31,1,BAKURIA);
	//SANGA
	load_castle_data(9,100,16,24,1,SANGAI);
	load_castle_data(10,100,13,28,1,SANGAI);
	load_castle_data(11,100,21,24,1,SANGAI);
	//KAYTOKEN
	load_castle_data(12,100,21,0,1,KAYTOKEN);
	load_castle_data(13,100,26,2,1,KAYTOKEN);
	load_castle_data(14,100,24,8,1,KAYTOKEN);

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

void add_deployment_to_castle(char deploy_no, char castle_no)
{
	char i;
	for(i=0; i<deployments[deploy_no].no_of_commanders; i++)
	{
		put_number(deployments[deploy_no].commanders[i],3,5,10+i);
		add_commander_to_castle(deployments[deploy_no].commanders[i],castle_no);
	}
}

void load_castle_bat()
{
	load_vram(0x3300,castle,60);
	load_vram(0x3340,castle1,60);
	load_vram(0x3380,castle2,60);
	load_vram(0x33C0,castle3,60);
	load_vram(0x3400,castle4,60);
}

void display_castles()
{
	char i;
	for(i=0; i<NO_OF_CASTLES; i++)
	{
		if(castles[i].y < 32)
		{
			put_castle(castles[i].owner,castles[i].x,castles[i].y);
		}
	}
}

void put_castle(int kingdom, int x, int y)
{
	int bat[4];
	int castle, offset;
	offset = 0;
	castle = (0x3330 + (kingdom * 4));
	bat[0] = (castle + 0);
	bat[1] = (castle + 1);
	bat[2] = (castle + 2);
	bat[3] = (castle + 3);
	load_bat((y*128)+(x*2)+offset,bat,2,2);
}

void load_castle_data(int index, char population, char x, char y,
                      char level, char kingdom)
{
	castles[index].no_of_soldiers = population;
	castles[index].x = x;
	castles[index].y = y;
	castles[index].level = level;
	castles[index].owner = kingdom;
	// castles[index].no_of_soldiers = 10;

	if(y < 30)
	{
		put_castle(kingdom,x,y);
	}
}

void display_army_sizes(char castle_no)
{
	char i;
	for(i=0; i<castles[castle_no].no_of_commanders; i++)
	{
		// put_number(commanders[castles[castle_no].commanders[i]]->unit.def,3,(i*5)+2,6);
		put_number(commanders[castles[castle_no].commanders[i]].army_size,2,(i*5)+2,6);
		// put_char('/',(i*5)+3,6);
		// put_number(MAX_ARMY_SIZE,1,(i*5)+4,6);
	}
	put_string("castle units:",2,1);
	put_number(castles[castle_no].no_of_soldiers,2,18,1);
}

void display_commanders_in_castle(char castle_no)
{
	// int x, y;
	char i;
	menu_state = 1;
	commander_select_cursor = 0;
	put_number(castle_no,3,15,15);
	display_info_panel();
	display_cmdr_info_panel(s_x,s_y);
	load_portrait(castles[castle_no].commanders[0]);
	display_portrait(castles[castle_no].commanders[0],s_x,s_y);
	print_commander_data(castles[castle_no].commanders[0]);

	load_cursor(3,27);
	vsync(3);
	// put_number(castles[castle_no].no_of_commanders,3,5,5);
	for(i=0; i<castles[castle_no].no_of_commanders; i++)
	{
		print_commander_stats(castles[castle_no].commanders[i],i);
		// load_commander(commanders[castles[castle_no].commanders[i]].sprite,COMMANDER_SELECT_VRAM + (i*0x100),20+i,0x100);
		// spr_make(14+i,14+(i*40),2+14,COMMANDER_SELECT_VRAM + (i*0x100),FLIP_MAS|SIZE_MAS,NO_FLIP|SZ_32x32,20+i,1);
		// put_string(commanders[castles[castle_no].commanders[i]].name,1,1);
	}
	draw_deployments(0);
	satb_update();
}

void display_recruit_menu(char castle_no)
{
	char i;
	menu_state = 1;
	commander_select_cursor = 0;

	display_info_panel();
	display_cmdr_info_panel(s_x,s_y);
	print_castle_data(current_selected_castle);

	load_cursor(3,27);
	vsync(3);
	for(i=0; i<castles[castle_no].no_of_commanders; i++)
	{
		print_commander_stats(castles[castle_no].commanders[i],i);
		// load_commander(commanders[castles[castle_no].commanders[i]].sprite,COMMANDER_SELECT_VRAM + (i*0x100),20+i,0x100);
		// spr_make(14+i,14+(i*40),2+14,COMMANDER_SELECT_VRAM + (i*0x100),FLIP_MAS|SIZE_MAS,NO_FLIP|SZ_32x32,20+i,1);
		// put_string(commanders[castles[castle_no].commanders[i]].name,1,1);
	}
	draw_deployments(0);
	satb_update();
}

void print_commander_stats(char cmdr_id, char i)
{
	int x, y;
	x = 2+((s_x>>1)/8);
	y = 4+((s_y>>1)/8);

	put_string(commanders[cmdr_id].name,x,y+i);
	put_number(commanders[cmdr_id].army_size,2,x+8,y+i);
	put_char('/',x+8+2,y+i);
	put_number(45,2,x+8+3,y+i);
	print_unit_type(commanders[cmdr_id].unit->unit_type,x+14,y+i);
}

void print_commander_data(char cmdr_id)
{
	int x, y;
	x = 23+((s_x>>1)/8);
	y = 5+((s_y>>1)/8);
	put_string("       ",x+2,y);
	put_string(commanders[cmdr_id].name,x+2,y);
	put_string("HP",x+4,y+1);
	put_number(commanders[cmdr_id].hp,3,x+1,y+2);
	put_char('/',x+4,y+2);
	put_number(commanders[cmdr_id].hp,3,x+5,y+2);

	put_string("AP",x+4,y+3);
	put_number(commanders[cmdr_id].ap,3,x+1,y+4);
	put_char('/',x+4,y+4);
	put_number(commanders[cmdr_id].ap,3,x+5,y+4);
}

void print_castle_data(char castle_no)
{
	char i;
	int x, y;
	x = 2+((s_x>>1)/8);
	y = 4+((s_y>>1)/8);

	put_string("Recruit",25,1);
	put_string("units",26,3);
	put_number(castles[castle_no].no_of_soldiers,3,25,4);
	put_char('/',28,4);
	put_number(100,3,29,4);

	put_string("level",26,6);
	put_number(castles[castle_no].level,2,27,7);
	//room for more castle info if necessary
}

void recruit_soldiers_to_commander(char castle_no, char commander_no)
{
	char max_recruitable, recruited, no_of_soldiers;
	no_of_soldiers = castles[castle_no].no_of_soldiers; // 10
	max_recruitable = MAX_ARMY_SIZE - commanders[commander_no].army_size; //7
	if(!max_recruitable){ return; }
	if(no_of_soldiers < max_recruitable)//there aren't enough soldiers to the castle to fill the current army
	{
		recruited = no_of_soldiers; //7 - 10
	}
	else if(max_recruitable < no_of_soldiers)//there are still some soldiers left over in the castle
	{
		recruited = max_recruitable; // 10 - 7 = 3
	}
	else //exactly enough
	{
		recruited = no_of_soldiers;
	}

	commanders[commander_no].army_size += recruited;
	castles[castle_no].no_of_soldiers -= recruited;
}

// void print_commanders_in_castle(char castle_no)
// {
// 	char i;
// 	print_castle_number(castle_no,10,8);
// 	for(i=0; i<castles[castle_no].no_of_commanders; i++)
// 	{
// 		put_number(commanders[castles[castle_no].commanders[i]].sprite,3,10,10+i);
// 	}
// }

void print_castle_number(char castle_no,int x, int y)
{
	put_number(castle_no,2,x,y);
}

void build_deployment()
{
	char i, cmdr_index, x, y, id;
	cmdr_index = 0;
	x = deployments[current].posX;
	y = deployments[current].posY;

	for(i=0; i<6; i++)
	{
		if(deployment_builder[i] == 1)
		{
			id = castles[current_selected_castle].commanders[i];
			deployments[current].kingdom = castles[current_selected_castle].owner;
			deployments[current].commanders[cmdr_index++] = id;
			deployments[current].no_of_commanders++;
		}
	}
	clear_deployment_builder();
	for(i=0; i<deployments[current].no_of_commanders; i++)
	{
		remove_commander_from_castle(deployments[current].commanders[i],current_selected_castle);
	}

	deployments[current].dance = 0;
	deployments[current].active = 1;
	// deployments[current].invul = 1;
	deployments[current].sprite_no = DEPLOY_OFFSET + current;
	deployments[current].absX = (x * 16);
	deployments[current].absY = (y * 16);

	load_vram(0x5040,walker,0x40);
	display_sprite_by_no(deployments[current].absX,deployments[current].absY,0x5040,0,17,(DEPLOY_OFFSET) + current);
}

char remove_commander_from_castle(char cmdr_id, char castle_id)
{
	char i, rmv;
	for(i=0; i<MAX_COMMANDERS; i++)
	{
		if(castles[castle_id].commanders[i] == cmdr_id)
		{
			rmv = i;
			castles[castle_id].no_of_commanders--;
			break;
		}
	}
	for(i=rmv; i<MAX_COMMANDERS; i++)
	{
		castles[castle_id].commanders[i] = castles[castle_id].commanders[i+1];
	}
	return cmdr_id;
}

char get_first_available_deployment()
{
	char j;
	for(j=0; j<MAX_DEPLOYMENTS; ++j)
	{
		if(deployments[j].active == 0)
		{
			return j;
		}
	}
}

void check_interactions()
{
	char i;
	for(i=0; i<MAX_DEPLOYMENTS; ++i)
	{
		if(deployments[i].active && deployments[i].invul == 0)
		{
			// return check_battlefield_interaction(i, deployments[i].posX, deployments[i].posY);
			check_battlefield_interaction(i, deployments[i].posX, deployments[i].posY);
			check_castle_interaction(i);
		}
	}
}

check_battlefield_interaction(char deploy_no, int x, int y)
{
	int i, x_diff, y_diff, x_1, x_2, y_1, y_2;
	for(i=0; i<MAX_DEPLOYMENTS; ++i)
	{
		if(deployments[i].active && i != deploy_no && deployments[i].kingdom != deployments[deploy_no].kingdom)
		{
			spr_set(deployments[i].sprite_no);
			x_1 = spr_get_x();
			spr_set(deployments[deploy_no].sprite_no);
			x_2 = spr_get_x();
			x_diff = abs(x_1 - x_2);

			spr_set(deployments[i].sprite_no);
			y_1 = spr_get_y();
			spr_set(deployments[deploy_no].sprite_no);
			y_2 = spr_get_y();
			y_diff = abs(y_1 - y_2);

			if(x_diff < 5 && y_diff < 5)
			{
				char battle_loser;
				battle_loser = do_battle(deployments[deploy_no].commanders,deployments[deploy_no].no_of_commanders,
																 deployments[i].commanders,deployments[i].no_of_commanders);

				if(battle_loser == 0)
				{
					change_course(deploy_no);
					deployments[i].dance = 16;
				}
				else
				{
					change_course(i);
					deployments[deploy_no].dance = 16;
				}
				deployments[i].invul = 1;
				deployments[deploy_no].invul = 1;
				// reinit_overworld();
			}
		}
	}
}

check_castle_interaction(char deploy_no)
{
	char i;
	int posx, posy;
	posx = deployments[deploy_no].posX;
	posy = deployments[deploy_no].posY;

	for(i=0; i<NO_OF_CASTLES; i++)
	{
		if(
				(castles[i].x == posx && castles[i].y == posy) &&
				(castles[i].owner != deployments[deploy_no].kingdom) &&
				(castles[i].no_of_commanders > 0)
			)
		{
			char battle_loser;
			battle_loser = do_battle(deployments[deploy_no].commanders,deployments[deploy_no].no_of_commanders,
															 castles[i].commanders,castles[i].no_of_commanders);

			deployments[deploy_no].invul = 1;
			if(battle_loser == 0)
			{
				change_course(deploy_no);
			}
			load_overworld_bg();
			//HEREHERE
			reinit_overworld();
		}
	}
}

char do_battle(char *commanders_one, char size_one, char *commanders_two, char size_two)
// char do_battle(char deploy_one, char deploy_two)
{
	// char battle_loser;
	hide_deployments();
	// return battle_select_loop(commanders_one,size_one,commanders_two,size_two);
	// battle_loop(0,0);
	battlefield_loop(commanders_one,size_one,commanders_two,size_two);
	return 0;
}

void reinit_overworld()
{
	vsync();
	load_overworld_bg();
	scroll(0, (s_x>>1), (s_y>>1), 0, 223, 0xC0);
	// scroll(1, 0, 0, 0, TOP_SCREEN_SIZE, 0xC0);
}

update_deployments()
{
	int x_move, y_move;
	int path_size, x_distance, y_distance;
	char i, j, dstx, dsty, posy, posx, walk, b, size;
	for(i=0; i<MAX_DEPLOYMENTS; i++)
	{
		posy = deployments[i].posY;
		posx = deployments[i].posX;
		dstx = deployments[i].dstX;
		dsty = deployments[i].dstY;

		if(deployments[i].active && deployments[i].dance != 0)
		{
			deployments[i].dance--;
		}
		else if(deployments[i].active && deployments[i].walk != 0)
		{
			x_move = 0;
			y_move = 0;
			x_distance = (dstx*16)-deployments[i].absX;
			y_distance = (dsty*16)-deployments[i].absY;

			spr_set(deployments[i].sprite_no);
			walk = deployments[i].walk;

			if(x_distance < 0 && y_distance < 0)
			{
				x_move = -1;
				y_move = -1;
				walk--;
			}
			else if(x_distance < 0 && y_distance > 0)
			{
				x_move = -1;
				y_move = 1;
				walk--;
			}
			else if(x_distance > 0 && y_distance < 0)
			{
				x_move = 1;
				y_move = -1;
				walk--;
			}
			else if(x_distance > 0 && y_distance > 0)
			{
				x_move = 1;
				y_move = 1;
				walk--;
			}
			else if(x_distance < 0)
			{
				x_move = -1;
				walk--;
			}
			else if(x_distance > 0)
			{
				x_move = 1;
				walk--;
			}
			else if(y_distance < 0)
			{
				y_move = -1;
				walk--;
			}
			else if(y_distance > 0)
			{
				y_move = 1;
				walk--;
			}
			deployments[i].walk = walk;
			/*
					this is a really slow hack to get the deployment sprites to their correct location
					after a battle has finished, need to figure out why the below doesn't work (and also)
					probably use deployments actual position instead of calculating it every time
			*/
			deployments[i].absX = deployments[i].absX + x_move;
			deployments[i].absY = deployments[i].absY + y_move;

			if(walk==0)
			{
				deployments[i].posX = dstx;
				deployments[i].posY = dsty;
			}
		}
		else if(deployments[i].active)
		{
			if(deployments[i].castX == posx && deployments[i].castY == posy)
			{
				char castle_no, z, c_i;
				castle_no = get_castle_no_by_pos(posx,posy);
				if(castles[castle_no].owner != deployments[i].kingdom)
				{
					change_castle_owner(castle_no,deployments[i].kingdom);
					while(castles[castle_no].no_of_commanders)
					{
						capture_commander(castles[castle_no].commanders[0],0);
						remove_commander_from_castle(castles[castle_no].commanders[0],castle_no);
					}
				}
				add_deployment_to_castle(i,castle_no);
				clear_deployment(i);
				spr_hide(deployments[i].sprite_no);
				load_castle_bat();
				display_castles();
				return;
			}

			for(j=0; j<NO_OF_CASTLES; j++)
			{
				if(posx == castles[j].x && posy == castles[j].y)
				{
					deployments[i].last_castle = j;
				}
			}

			path_size = deployments[i].path_size;

			deployments[i].dstX = deployments[i].path[path_size] & 31;
			deployments[i].dstY = deployments[i].path[path_size] / 32;

			deployments[i].path_size = path_size - 1;
			deployments[i].walk = 16;
		}
	}
}

void change_castle_owner(char castle_no, char kingdom)
{
	castles[castle_no].owner = kingdom;
}

void change_commander_kingdom(char cmdr, char kingdom)
{
	commanders[cmdr].kingdom = kingdom;
}

void draw_deployments(char offset)
{
	char i;
	for(i=0;i<8;i++)
	{
		if(deployments[i].active)
		{
			spr_set(deployments[i].sprite_no);
			spr_y(deployments[i].absY + (y_offset >> 1)); //+ (offset * 16));
			spr_x(deployments[i].absX + (x_offset >> 1));
		}
	}
}

void clear_deployment(char deploy_no)
{
	char i;
	deployments[deploy_no].active = 0;
	deployments[deploy_no].invul = 0;
	deployments[deploy_no].no_of_commanders = 0;
	deployments[deploy_no].castX = 0;
	deployments[deploy_no].castY = 0;

	for(i=0; i<MAX_COMMANDERS; i++)
	{
		deployments[deploy_no].commanders[0] = 0;
	}
}

char castle_menu_select(char selection)
{
  switch(selection){

    case 0:
    load_castle_info_menu();
    return 1;

		case 1:
		// recruit_soldiers_to_commander(current_selected_castle,0);
		// display_commanders_in_castle(current_selected_castle);
		display_recruit_menu(current_selected_castle);
		// display_army_sizes(current_selected_castle);
		menu_state = 2;
		return 0;

		case 2: //this is where we'll start our deploy builder
		deployments[current].posX = castles[current_selected_castle].x;
		deployments[current].posY = castles[current_selected_castle].y;
		display_commanders_in_castle(current_selected_castle);
		return 0;

		default:
		// castles[current_selected_castle].owner = 1;
		// display_castles();
		return 0;
  }
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
	char j;
	for(j = 0; j < NO_OF_CASTLES; j++)
	{
		if(castles[j].x == x && castles[j].y == y)
		{
			current_selected_castle = j;
			return j;
		}
	}
	return -1;
}

void overworld_controls(){
	char i, move_screen;
	move_screen = 16;
	j_1 = joy(0);
	j_2 = joytrg(0);
	// put_number(commander_select_cursor,3,15,15);
	if (j_1 & JOY_LEFT)
	{
		if(menu_state == 1 || menu_state == 2)
		{
			return;
		}
		if(selector_x > 0){
			spr_set(0);
			selector_x -= 4;
			spr_x(selector_x);
		}
		else if(s_x > 0){
			x_offset += move_screen;
			s_x = (s_x -= move_screen) & 511;

			scroll(0, (s_x >> 1), (s_y >> 1), 0, 223, 0xC0);
		}
	}
	if (j_1 & JOY_RIGHT)
	{
		if(menu_state == 1 || menu_state == 2)
		{
				return;
		}
		if(selector_x < 240)
		{
			spr_set(0);
			selector_x += 4;
			spr_x(selector_x);
		}
		else if(s_x < 495){
			x_offset -= move_screen;
			s_x = (s_x += move_screen) & 511;

			scroll(0, (s_x >> 1), (s_y >> 1), 0, 223, 0xC0);
		}
	}
	if (j_1 & JOY_UP)
	{
		if(menu_state == 1 || menu_state == 2)
		{
			if(j_2 & JOY_UP)
			{
				spr_set(1);
				if(commander_select_cursor > 0)
				{
					commander_select_cursor--;
					put_number(commander_select_cursor,3,15,15);
					spr_y(spr_get_y()-8);
					if(menu_state == 1)
					{
						load_portrait(castles[current_selected_castle].commanders[commander_select_cursor]);
						display_portrait(castles[current_selected_castle].commanders[commander_select_cursor],s_x,s_y);
						print_commander_data(castles[current_selected_castle].commanders[commander_select_cursor]);
					}
				}
			}
			return;
		}
		if(selector_y > 0)
		{
			spr_set(0);
			selector_y -= 4;
			spr_y(selector_y);
		}
		else if(s_y > (0 * 2))
		{
			y_offset += move_screen;
			s_y = (s_y -= move_screen); //& 511;
			scroll(0, (s_x >> 1), (s_y >> 1), 0, 223, 0xC0);
		}
	}
	if (j_1 & JOY_DOWN)
	{
		if(menu_state == 1 || menu_state == 2)
		{
			if(j_2 & JOY_DOWN)
			{
				spr_set(1);
				if(commander_select_cursor < (castles[current_selected_castle].no_of_commanders - 1))
				{
					commander_select_cursor++;
					put_number(commander_select_cursor,3,15,15);
					spr_y(spr_get_y()+8);
					if(menu_state == 1)
					{
						load_portrait(castles[current_selected_castle].commanders[commander_select_cursor]);
						display_portrait(castles[current_selected_castle].commanders[commander_select_cursor],s_x,s_y);
						print_commander_data(castles[current_selected_castle].commanders[commander_select_cursor]);
					}
				}
			}
			return;
		}
		if(selector_y < 208)
		{
			spr_set(0);
			selector_y += 4;
			spr_y(selector_y);
		}
		else if(s_y < 512 + 72)
		{
			y_offset -= move_screen;
			s_y = (s_y += move_screen); //& 511;
			scroll(0, (s_x >> 1), (s_y >> 1), 0, 223, 0xC0);
		}
	}

	if(j_2 & JOY_I)
	{
		char castle_no;

		if(menu_state == 2) //recruit units
		{
			int cmdr, cmdr_index;

			spr_set(1);

			cmdr = commander_select_cursor;
			// deployment_builder[cmdr] ^= 1;
			// cmdr_index = castles[current_selected_castle].commanders[commander_select_cursor];
			// put_number(cmdr_index,3,15,15);
			recruit_soldiers_to_commander(current_selected_castle,castles[current_selected_castle].commanders[cmdr]);
			print_castle_data(current_selected_castle);
			print_commander_stats(castles[current_selected_castle].commanders[cmdr],cmdr);
			// display_army_sizes(current_selected_castle);
			// clear_deployment_builder();
		}
		else if(menu_state == 1)
		{
			int cmdr, cmdr_index;
			char pal;

			spr_set(1);
			cmdr = commander_select_cursor;

			deployment_builder[cmdr] ^= 1;
			// cmdr_index = commanders[castles[current_selected_castle].commanders[cmdr]].sprite;

			if(deployment_builder[cmdr])
			{
				set_font_pal(2);
			}
			else
			{
				set_font_pal(1);
			}
			print_commander_stats(castles[current_selected_castle].commanders[cmdr],cmdr);
			set_font_pal(1);
		}
		else
		{
			castle_no = -1;
			castle_no = select_castle_by_cursor();

			if(castle_no != 255)
			{
				// put_number(castle_no,3,15,15);
				spr_hide(0);
				current = get_first_available_deployment();
				load_castle_menu(castles[castle_no].owner);
			}
		}
	}

	if(j_2 & JOY_II)
	{
		if(menu_state == 1 || menu_state == 2)
		{
			char j;
			clear_commander_select();
			load_castle_menu(castles[current_selected_castle].owner);
			clear_deployment_builder();
			commander_select_cursor = 0;
		}
	}

	if(j_2 & JOY_RUN)
	{
		if(menu_state == 1)
		{
			clear_commander_select();
			selector_menu();
			commander_select_cursor = 0;
		}

		if(menu_state == 2)
		{
			clear_commander_select();
			commander_select_cursor = 0;
		}
	}
}

clear_commander_select()
{
	char i;
	load_map(0,0,0,0,MAP_WIDTH,OVERWORLD_MAP_HEIGHT);
	display_castles();
	menu_state = 0;

	commander_select_cursor = 0;

	spr_hide(--total_sprites);
	for(i=0;i<castles[current_selected_castle].no_of_commanders;i++)
	{
		spr_hide(14+i);
	}
	// load_palette(20,menupal,1);
	satb_update();
}

void selector_menu()
{
	char exit, i, z, move_screen;
	move_screen = 16;
	exit = 1;
	put_string("select a castle to deploy to",2,1);
	while(exit)
	{
		vsync();
		j_1 = joy(0);
		j_2 = joytrg(0);
		if (j_1 & JOY_LEFT)
		{
			if(selector_x > 0){
				spr_set(0);
				selector_x -= 4;
				spr_x(selector_x);
			}
			else if(s_x > 0){

				x_offset += move_screen;
				s_x = (s_x -= move_screen) & 511;
				scroll(0, (s_x >> 1), (s_y >> 1), 0, 223, 0xC0);
			}
		}
		if (j_1 & JOY_RIGHT)
		{
			if(selector_x < 240)
			{
				spr_set(0);
				selector_x += 4;
				spr_x(selector_x);
			}
			else if(s_x < 508){
				x_offset -= move_screen;
				s_x = (s_x += move_screen) & 511;
				scroll(0, (s_x >> 1), (s_y >> 1), 0, 223, 0xC0);
			}
		}
		if (j_1 & JOY_UP)
		{
			if(selector_y > 0)
			{
				spr_set(0);
				selector_y -= 4;
				spr_y(selector_y);
			}
			else if(s_y > 0)
			{
				y_offset += move_screen;
				s_y = (s_y -= move_screen); //& 511;
				scroll(0, (s_x >> 1), (s_y >> 1), 0, 223, 0xC0);
			}
		}
		if (j_1 & JOY_DOWN)
		{
			if(selector_y < 208)
			{
				spr_set(0);
				selector_y += 4;
				spr_y(selector_y);
			}
			else if(s_y < 512 + 72)
			{
				y_offset -= move_screen;
				s_y = (s_y += move_screen); //& 511;
				scroll(0, (s_x >> 1), (s_y >> 1), 0, 223, 0xC0);
			}
		}
		if(j_2 & JOY_I)
		{
			char x, y, j, path_size;

	    x = (selector_x + (s_x >> 1)) / 16;
			y = ((selector_y + (s_y >> 1)) / 16) - map_y_offset;

			for(j = 0; j < NO_OF_CASTLES; j++)
			{
				if(castles[j].x == x && castles[j].y == y)
				{
					exit = 0;

					deployments[current].castX = x;
					deployments[current].castY = y;

					build_deployment();

					path_size = get_path(((deployments[current].posY*32)+deployments[current].posX),
																(deployments[current].castY*32)+deployments[current].castX,
																 deployments[current].path,overworld,0);

					deployments[current].path_size = path_size;
					deployments[current].walk = 0;

					overworld_state = OVERWORLD;
					menu_state = 0;
					// j_2 = 0;
				}
			}
		}

		if(j_2 & JOY_II)
		{
			exit = 0;
			overworld_state = 1;
			commander_select_cursor = 0;
			display_commanders_in_castle(current_selected_castle);
		}
		draw_deployments(0);
		satb_update();
	}
	// display_info_panel();
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

	// put_string("Castles:",1,1);
	// put_number(number_of_castles,2,9,1);
	// put_string("Commanders:",1,2);
	// put_number(number_of_commanders,2,12,2);
}

update_kingdom_stats()
{
	char i, cmdrs, cast;
	cmdrs = 0;
	cast = 0;
	for(i=0; i<TOTAL_COMMANDERS; i++)
	{
		if(commanders[i].kingdom == TAROS)
		{
			cmdrs++;
		}
	}
	for(i=0; i<NO_OF_CASTLES; i++)
	{
		if(castles[i].owner == TAROS)
		{
			cast++;
		}
	}
	number_of_castles = cast;
	number_of_commanders = current_captured;
}

print_deployment_stats(char deploy_no)
{
	put_string("abs",10,9);
	put_number(deployments[deploy_no].absX,3,10,10);
	put_number(deployments[deploy_no].absY,3,10,11);

	put_string("dest",15,9);
	put_number(deployments[deploy_no].dstX,3,15,10);
	put_number(deployments[deploy_no].dstY,3,15,11);

	put_string("pos",10,15);
	put_number(deployments[deploy_no].posX,3,10,16);
	put_number(deployments[deploy_no].posY,3,10,17);

	put_string("walk",15,15);
	put_number(deployments[deploy_no].walk,3,15,16);
}

clear_deployment_builder()
{
	char i;
	for(i=0;i<5;i++)
	{
		deployment_builder[i] = 0;
	}
}

hide_deployments()
{
	spr_hide(deployments[0].sprite_no);
	spr_hide(deployments[1].sprite_no);
	spr_hide(deployments[2].sprite_no);
	spr_hide(deployments[3].sprite_no);
	spr_hide(deployments[4].sprite_no);
	spr_hide(deployments[5].sprite_no);
	spr_hide(deployments[6].sprite_no);
	spr_hide(deployments[7].sprite_no);
}
