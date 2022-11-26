#include <huc.h>
#include "paths.h"
#include "strifesisters.h"
#include "item.c"
#include "units.c"
#include "terrain.c"
#include "commander.c"
#include "map.c"

#define SELECTOR 0
#define CURSOR 1
#define SLIDER_ONE 1
#define SLIDER_TWO 2

#define PORTRAIT_VRAM 0x4400//0x3C00
#define RECRUIT_VRAM 0x4600
#define DIALOGUE_VRAM 0x4000
#define P_BORDER_VRAM 0x4000
#define MAX_GLOBAL_UNITS 30
#define MAX_PARTY_SIZE 10
#define MAX_INVENTORY 15
#define UNIT_LIST_SIZE 16
#define MAX_COMMANDERS_PER_BATTLE 6

#define PREBATTLE 0xFE
#define INBATTLE 0xFD
#define POSTBATTLE 0xFC
#define EXPLORE 0xFB
#define REQ 4

#incchr(healthbar,"map/backgrounds/healthbars.pcx");
#incpal(healthbarpal,"map/backgrounds/healthbars.pcx");

#incchr(panel_gfx, "map/sprites/deploy_menu.pcx")
#incpal(panel_pal, "map/sprites/deploy_menu.pcx")

#incchr(info_gfx, "map/sprites/infobox.pcx")
#incpal(info_pal, "map/sprites/infobox.pcx")
#incbat(info_bat, "map/sprites/infobox.pcx", 0x1500, 0, 0, 32, 4)

#incchr(dialog_gfx, "map/sprites/dialoguebox.pcx")
#incpal(dialog_pal, "map/sprites/dialoguebox.pcx")
// #incbat(dialog_bat, "map/sprites/dialoguebox.pcx", 0x1500, 0, 0, 32, 4)

#incchr(icons_gfx, "map/sprites/icons.pcx")
#incpal(icons_pal, "map/sprites/icons.pcx")

#incchr(violet_gfx, "map/sprites/violet.pcx")
#incpal(violet_pal, "map/sprites/violet.pcx")

#incchr(rei_gfx, "map/sprites/rei.pcx")
#incpal(rei_pal, "map/sprites/rei.pcx")

#incchr(unknown_gfx, "map/sprites/unknown.pcx")
#incpal(unknown_pal, "map/sprites/unknown.pcx")

#incspr(rei_walk, "map/sprites/reiwalk.pcx")
#incpal(rei_walk_pal, "map/sprites/reiwalk.pcx")

#incspr(cat_walk, "characters/cat.pcx")
#incpal(cat_walk_pal, "characters/cat.pcx")

#incspr(violet_walk, "characters/violetwalk.pcx")
#incpal(violet_walk_pal, "characters/violetwalk.pcx")

#incchr(king_gfx, "characters/king.pcx")
#incpal(king_pal, "characters/king.pcx")

#define MAP_WIDTH 32
#define TILE_WIDTH 16
#define TILE_VRAM 0x1000
#define OVERWORLD_MAP_HEIGHT 32
#define NO_OF_CASTLES 16

#define MAX_ARMY_SIZE 9
#define MAX_COMMANDERS 3
#define MAX_MOVE_RANGE 5

#incspr(cursor, "map/cursor.pcx");
#incpal(cursorpal, "map/cursor.pcx");
#incspr(vert_pointer, "map/sprites/pointer_ud.pcx");
#incspr(horz_pointer, "map/sprites/pointer_lr.pcx");
#incpal(pointerpal, "map/sprites/pointer_ud.pcx");

struct Commander{
	char id;
	char no_of_items;
	char equipable;
	char *name;
	char unit;
	char sprite_type;
};

struct Castle{
	int pos, map_id;
};

#incspr(selector,"map/sprites/selector.pcx");
#incpal(selectorpal,"map/sprites/selector.pcx");

//font
#incchr(font, "map/sprites/font.pcx")
#incpal(fontpal,"map/sprites/font.pcx")
//menus
// #incchr(borders, "tiles/borders.tiles.pcx")
#incchr(borders, "tiles/borders1.pcx")
#incpal(borderspal, "tiles/borders.tiles.pcx")

char j_1, j_2;
int selector_x, selector_y, s_x, s_y, y_offset, x_offset;
struct Castle castles[NO_OF_CASTLES];
struct Commander commanders[TOTAL_COMMANDERS];
int total_sprites = 0;

char no_of_party_items;
char num_of_bad_terrains;

char party[MAX_PARTY_SIZE];
char party_items[MAX_INVENTORY];
char party_size;
int screen_dimensions = 0;

//spawns
int script_pointer = 0;
int spawn_positions[6];
char enemies[7];
int ptr[288];

/*
	MENU STUFF
*/

#define CONSUMABLE 0
#define EQUIPABLE 1
#define ANY_ITEM 2

int current_global_units = 0;
unsigned char game_state = PREBATTLE;

/*
	BATTLE STUFF
*/
#define SOLDIER_SIZE 32
#define BATTLE_MAP_HEIGHT 16

struct Node{
	// int ownX, ownY, fromX, fromY;
	int ownPos, fromPos;
	char checked;
};

enum SoldierState{
	IDLE, ATTACKING, DYING
};

enum Direction{
	NORTH, NORTHEAST, EAST, SOUTHEAST, SOUTH, SOUTHWEST, WEST, NORTHWEST
};

// struct soldier{
// 	int x, y, frame;
// 	char tic, id;
// 	enum SoldierState state;
// 	enum Direction direction;
// 	char attack, active;
// };

/* sprites */
#incspr(attack, "map/sprites/swordy.pcx")
#incpal(soldierpal, "map/sprites/swordy.pcx",0,2)

#incspr(blobbattle, "map/sprites/blobbattle.pcx")
#incpal(blobbattlepal, "map/sprites/blobbattle.pcx",0,2)

#incspr(attack2, "map/sprites/spear_anim.pcx")
#incpal(spearpal, "map/sprites/spear_anim.pcx",0,2)

// #incspr(bandit, "characters/banditbattle.pcx")
// #incpal(banditpal, "characters/bandit.pcx",0,2)

#incspr(bandit, "characters/axeunit.pcx")
#incpal(banditpal, "characters/axeunit.pcx",0,2)

#incspr(musketbtl, "map/sprites/archer.pcx")
#incpal(musketbtlpal, "map/sprites/archer.pcx",0,2)

#incspr(demonbtl, "map/sprites/demon.pcx")
#incpal(demonbtlpal, "map/sprites/demon.pcx",0,2)

#incspr(houndbtl, "map/sprites/hound_big.pcx")
#incpal(houndbtlpal, "map/sprites/hound_big.pcx",0,2)

#incchr(portraitgfx, "map/sprites/portrait.pcx", 0, 0, 4, 4)
#incchr(portraitm, "map/sprites/portraitm.pcx", 0, 0, 4, 4)

#incpal(portraitpal, "map/sprites/portrait.pcx")
#incpal(portraitmpal, "map/sprites/portraitm.pcx")

//items
#incchr(potion_gfx, "map/sprites/healthpotion.pcx",0,0,4,4)
#incpal(potionpal, "map/sprites/healthpotion.pcx")

#incchr(empty_gfx, "map/sprites/empty.pcx",0,0,4,4)

#incchr(elixir_gfx, "map/sprites/elixir.pcx",0,0,4,4)
#incpal(elixirpal, "map/sprites/elixir.pcx")

#incchr(dagger_gfx, "map/sprites/dagger.pcx",0,0,4,4)
#incpal(daggerpal, "map/sprites/dagger.pcx")

#incchr(bow_gfx, "map/sprites/bow.pcx",0,0,4,4)
#incpal(bowpal, "map/sprites/bow.pcx")

#incchr(shield_gfx, "map/sprites/shield.pcx",0,0,4,4)
#incpal(shieldpal, "map/sprites/shield.pcx")

#incspr(sld,"map/sprites/sldpiece.pcx")
#incspr(spr,"map/sprites/sprpiece.pcx")
#incspr(dmn,"map/sprites/demonpiece.pcx")
#incspr(hnd,"map/sprites/houndpiece.pcx")
#incspr(arch0,"map/sprites/archpiece0.pcx")
#incspr(blob,"map/sprites/blob.pcx")
#incspr(bnd,"characters/bandit.pcx")
#incspr(dark,"map/sprites/dark.pcx")

#incpal(sldpal, "map/sprites/sldpiece.pcx",0,3)
#incpal(cmdrpal, "map/sprites/sprpiece.pcx",2,3)
#incpal(sprpal, "map/sprites/sprpiece.pcx",0,3)
#incpal(mskpal,"map/sprites/msktpiece.pcx",0,2)
#incpal(dmnpal,"map/sprites/demonpiece.pcx",0,2)
#incpal(blobpal,"map/sprites/blob.pcx",0,2)

const char area_one_buyable_items[] = {
	0, 1, 2, 3
};

const char row_names[17] = {
	70, 114, 111, 110, 116, 0,
	73, 110, 110, 101, 114, 0,
	82, 101, 97, 114, 0
};

int menu_size = 0;
unsigned int player_gold = 0;
char buyable_units[3];
char buyable_items[4];

int map_counter = 0;
int map_size = 0;
int objective_pos;
int frame = 0x5000;
int tic = 0;
int sx = 0;
int sy = 0;
int yOffset = 0;
char j1 = 0;
int j2 = 0;
int xOffset = 0;
int total_units = 0;
int n = 0;
char num_of_buyable_units = 0;
char num_of_buyable_items = 0;
char g = 0;
char game_over = 0;
char game_loop = 0;

void main()
{
	game_loop = 1;

	buyable_units[0] = SWORD_UNIT;
	buyable_units[1] = SPEAR_UNIT;
	buyable_units[2] = ARCHER_UNIT;
	num_of_buyable_units = 3;

	buyable_items[0] = 0;
	buyable_items[1] = 1;
	buyable_items[2] = 2;
	buyable_items[3] = 3;
	num_of_buyable_items = 4;

	initialize_units();
	initialize_commanders();
	initialize_items();

	commanders[0].name = "Rei";
	commanders[0].no_of_items = 1;
	commanders[0].equipable = 1;
	commanders[1].name = "Violet";
	commanders[1].no_of_items = 1;
	commanders[1].equipable = 3;

	commanders[2].name = "King";
	commanders[3].name = "Alucard";
	commanders[4].name = "Kain";
	commanders[5].name = "Sacha";
	commanders[6].name = "Almer"; //Olimar :)
	commanders[7].name = "Ven";
	commanders[8].name = "Phalc";
	commanders[9].name = "Kurt";
	commanders[23].name = "Alpha.H";
	commanders[24].name = "Cmdr 24";
	commanders[25].name = "Cmdr 25";
	commanders[26].name = "Cmdr 26";
	commanders[27].name = "Cmdr 27";

	commanders[0].unit = SWORD_UNIT;
	commanders[1].unit = ARCHER_UNIT;
	commanders[2].unit = SPEAR_UNIT;

	commanders[23].sprite_type = 5;
	commanders[24].sprite_type = 6;
	commanders[25].sprite_type = 7;
	commanders[26].sprite_type = 7;
	commanders[27].sprite_type = 7;
	commanders[10].sprite_type = 0;

	items[0].name = "Dagger";
	items[0].type = EQUIPABLE;
	items[1].name = "Shield";
	items[1].type = EQUIPABLE;
	items[1].description = "+5 Def";
	items[2].name = "Bow";
	items[2].type = EQUIPABLE;
	items[3].name = "Potion";
	items[3].type = CONSUMABLE;
	items[3].description = "+5 HP";
	items[4].name = "Elixir";
	items[4].type = CONSUMABLE;
	items[4].description = "+5 AP";
	items[5].name = "Topaz";
	items[5].type = CONSUMABLE;
	items[6].name = "Ruby";
	items[6].type = CONSUMABLE;
	items[7].name = "Opal";
	items[7].type = CONSUMABLE;
	items[8].name = "Pearl";
	items[8].type = CONSUMABLE;
	items[9].name = "Herb";
	items[9].type = CONSUMABLE;
	items[10].name = "Boots";
	items[10].type = CONSUMABLE;
	items[11].name = "Helmet";
	items[11].type = CONSUMABLE;
	items[14].name = "Empty";

	player_gold = 1000;

	party[0] = REI;
	party[1] = VIOLET;
	party[2] = GEN_ONE;
	// party[2] = KING;
	party_size = 3;

	party_items[0] = 2; //bow -> equip
	party_items[1] = 4; //elixir -> consume
	party_items[2] = 5; //topaz -> consume
	party_items[3] = 0; //dagger -> consume
	party_items[4] = 3; //potion -> consume
	party_items[5] = 3; //potion -> consume
	party_items[6] = 1; //shield -> consume
	party_items[7] = 3; //potion -> consume
	party_items[8] = 3; //potion -> consume
	party_items[9] = 3; //potion -> consume
	party_items[10] = 3; //potion -> consume
	no_of_party_items = 11;

	disp_off();
	init_satb();

	load_vram(0x6900,cursor,0x40);
	// load_vram(0x68C0+0x40,vert_pointer,0x100);

	while(game_loop)
	{
		display_intro();
		overworld_loop();
		// battle_loop(0,23,1);
		// battle_loop(24,0,1);
	}
	for(;;)
	{
		display_outro();
	}
}

void display_intro()
{
	// load_background(build_screen,build_pal,build_bat,32,28);
	cls();
	disp_on();
	put_string("Strife Sisters v0.5",7,14);
	wait_for_I_input();
	// sync(60*10);

	// load_background(explain_menu,explain_pal,explain_bat,32,28);
	// while(!(joytrg(0) & JOY_RUN)){}
	disp_off();
}

void display_outro()
{
	cls();
	scroll(0,0,0,224,0xC0);
	disp_on();
	put_string("Thanks for playing!",7,14);
	wait_for_I_input();
}

spr_make(spriteno,spritex,spritey,spritepattern,ctrl1,ctrl2,sprpal,sprpri)
int spriteno,spritex,spritey,spritepattern,ctrl1,ctrl2,sprpal,sprpri;
{
	spr_set(spriteno);
	spr_x(spritex);
	spr_y(spritey);
	spr_pattern(spritepattern);
	spr_ctrl(ctrl1,ctrl2);
	spr_pal(sprpal);
	spr_pri(sprpri);
}

void display_sprite(int x_pos, int y_pos, int location, char flip, char pal)
{
	spr_make(total_sprites++,x_pos,y_pos,location,flip,NO_FLIP|SZ_16x16,pal,0);
	spr_pri(1);
}

int range(char min, char max)
{
	return (rand()%(max-min+1)) + min;
}

void print_unit_type(char id, int x, int y)
{
	switch(id)
	{
		case SWORD_UNIT:
		put_string("SWD    ",x,y);
		break;

		case DEMON_UNIT:
		put_string("DMN    ",x,y);
		break;

		case SPEAR_UNIT:
		put_string("SPR    ",x,y);
		break;

		case ARCHER_UNIT:
		put_string("ARC    ",x,y);
		break;

		case MAGE_UNIT:
		put_string("MAG    ",x,y);
		break;

		case HOUND_UNIT:
		put_string("HND    ",x,y);
		break;

		case BLOB_UNIT:
		put_string("BLB    ",x,y);
		break;

		case AXE_UNIT:
		put_string("AXE    ",x,y);
		break;

		default:
		put_string("err ",x,y);
		put_number(id,3,x+4,y);
		// put_string(commanders[id-16].name,x,y);
		break;
	}
}

void print_unit_attack_icon(char unit_no, int x, int y)
{
	switch(unit_list[unit_no].a_type)
	{
			case NORMAL:
			put_string(":",x,y);//sword
			break;

			case PIERCE:
			put_string(";",x,y);//spear
			break;

			case UNARMED:
			put_string(">",x,y);//unarmed
			break;

			case MAGIC:
			put_string("=",x,y);//magic
			break;

			case MISSILE:
			put_string("<",x,y);//bow
			break;

			case BLUNT:
			put_string("?",x,y);
			break;

			default:
			put_string("=",x,y);
			break;
	}
}

void print_unit_defense_icon(char unit_no, int x, int y)
{
	switch(unit_list[unit_no].d_type)
	{
			case HEAVY:
			put_string("[",x,y);
			break;

			case MEDIUM:
			put_string("\\",x,y);
			break;

			case LIGHT:
			put_string("]",x,y);
			break;

			default:
			put_string("^",x,y);
			break;
	}
}

void sync(int cycles)
{
  int i;
  for(i=0; i<cycles; i++)
  {
    vsync();
  }
}

void load_cursor(int x, int y, int cursor_no)
{
	if(cursor_no == SLIDER_TWO)
	{
		spr_make(cursor_no,x,y,0x6900,FLIP_MAS|SIZE_MAS,FLIP_X|SZ_16x16,19,1);
	}
	else
	{
		spr_make(cursor_no,x,y,0x6900,FLIP_MAS|SIZE_MAS,NO_FLIP|SZ_16x16,19,1);
	}
}

void tick(){
	if(++tic > 6)
	{
		tic = 0;
		++frame;
		// soldierFrame += 0x100;
		// altFrame += 0x100;
	}
	if(frame > 3){
		frame = 0;
		// soldierFrame = 0;
		// altFrame = 0;
		// soldierFrame = 0x4000;
		// altFrame = 0x5000;
	}
}

int write_text(char x, char y, char *text)
{
	int i;
	char text_y, text_x;
	text_x = x;
	text_y = y;
	i=0;
	for(;;)
	{
		if(text[i] == 0)
		{
			break;
		}
		if(text[i] == 10)
		{
			text_y++;
			text_x = x;
			i++;
			continue;
		}
		put_char(text[i],text_x,text_y);
		i++;
		text_x++;
		vsync(2);
	}
	return i;
}

void wait_for_I_input()
{
	vsync(10);
	// joytrg(0);
	// joytrg(0);
	// joytrg(0);
	// joytrg(0);
	while(!(joytrg(0) & JOY_I)){}
	// vsync(30);
}

initialize_commanders()
{
	char i;
	struct Commander *cmdr;
	// for(i=0, cmdr = commanders, unit_ptr = &unit_list[REI]; i<TOTAL_COMMANDERS; i++, cmdr++, unit_ptr++)
	for(i=0, cmdr = commanders; i<TOTAL_COMMANDERS; i++, cmdr++)
	{
		cmdr->id = i;
		cmdr->no_of_items = 0;
		cmdr->equipable = 14;
		cmdr->unit = 0;
		cmdr->sprite_type = i+16;
		if(i>24)
		{
			cmdr->name = "generic";
		}
	}
}

void darken_palette(int pal_num)
{
	modify_palette(pal_num,-1);
}

void lighten_palette(int pal_num)
{
	modify_palette(pal_num,2);
}

void modify_palette(int pal_num, char modifier)
{
	int rgb;
	char i, r, g, b;

	for(i=0; i<16; i++)
	{
		rgb = get_color((pal_num*16)+i);

		g = (((rgb >> 6) & 0x7) + modifier);
		r = (((rgb >> 3) & 0x7) + modifier);
		b = ((rgb & 0x7) + modifier);

		if(g > 7 && g < 255)
		{
			g = 7;
		}
		else if(g < 0)
		{
			g = 0;
		}
		else if(g == 255)
		{
			g = 0;
		}

		if(r > 7 && r < 255)
		{
			r = 7;
		}
		else if(r < 0)
		{
			r = 0;
		}
		else if(r == 255)
		{
			r = 0;
		}

		if(b > 7 && b < 255)
		{
			b = 7;
		}
		else if(b < 0)
		{
			b = 0;
		}
		else if(b == 255)
		{
			b = 0;
		}
		set_color_rgb((pal_num*16)+i,r,g,b);
	}
}

void load_item(char item_id, int index)
{
	switch(item_id)
	{
		case 0:
		load_vram(PORTRAIT_VRAM + (index * 0x100),dagger_gfx,0x100);
		load_palette(12+index,daggerpal,1);
		break;

		case 1:
		load_vram(PORTRAIT_VRAM + (index * 0x100),shield_gfx,0x100);
		load_palette(12+index,shieldpal,1);
		break;

		case 2:
		load_vram(PORTRAIT_VRAM + (index * 0x100),bow_gfx,0x100);
		load_palette(12+index,bowpal,1);
		break;

		case 3:
		load_vram(PORTRAIT_VRAM + (index * 0x100),potion_gfx,0x100);
		load_palette(12+index,potionpal,1);
		break;

		case 14:
		default:
		load_vram(PORTRAIT_VRAM + (index * 0x100),empty_gfx,0x100);
		load_palette(12+index,elixirpal,1);
		break;
	}
}

void display_spawn(int index, int spawn_x, int spawn_y, int sprite_offset, int spawn_offset)
{
	int sprite_location, sprite_no;
	sprite_location = 0;//UNIT_VRAM_START;
	sprite_no = 18;
	switch(index)
	{
		case SWORD_UNIT:
		sprite_location = 0;//UNIT_VRAM_START;
		sprite_no = 18;
		break;
		case SPEAR_UNIT:
		sprite_location = 0;//UNIT_VRAM_START + 0x100;
		sprite_no = 18;
		break;
		case DEMON_UNIT:
		sprite_location = 0;//UNIT_VRAM_START + 0x300;
		sprite_no = 22;
		break;
		case ARCHER_UNIT:
		sprite_location = 0;//UNIT_VRAM_START + 0x200;
		sprite_no = 18;
		break;
		case HOUND_UNIT:
		sprite_location = 0;//UNIT_VRAM_START + 0x400;
		sprite_no = 22;
		break;
	}
	spr_make(10+spawn_offset,spawn_x,spawn_y,sprite_location,FLIP_MAS|SIZE_MAS,SZ_16x32,sprite_no,1);
}

void display_item(char cmdr_id, int index, int x, int y)
{
	int bat[16];
	int offset, i;
	// offset = (((s_x/8)+x)*16) + (((s_y/8)+y)*16);
	offset = ((s_x+x) + ((s_y/8)*screen_dimensions)) + (y*screen_dimensions);
	// put_number(vram_addr(0,36),4,0,36);

	for(i=0; i<16; i++)
	{
		bat[i] = (((PORTRAIT_VRAM + (index*0x100))>>4)+(0xC000+(0x1000 * index))) + i;
	}
	load_bat(offset,bat,4,4);
	// load_bat(2304,bat,4,4);
}

void load_portrait(char cmdr_id)
{
	switch(cmdr_id)
	{
		case REI:
		load_vram(PORTRAIT_VRAM,rei_gfx,0x100);
		load_palette(12,rei_pal,1);
		break;

		case VIOLET:
		load_vram(PORTRAIT_VRAM,violet_gfx,0x100);
		load_palette(12,violet_pal,1);
		break;

		case KING:
		load_vram(PORTRAIT_VRAM,king_gfx,0x100);
		load_palette(12,king_pal,1);
		break;

		default:
		load_vram(PORTRAIT_VRAM,unknown_gfx,0x100);
		load_palette(12,unknown_pal,1);
		break;
	}
}

void display_cmdr_info(char cmdr_id, int x, int y)
{
	int _sx, _sy;
	_sx = s_x / 8 + x;
	_sy = s_y / 8 + y + 2;

	put_string("      ",_sx,_sy+1);
	put_string("   ",_sx,_sy+2);
	put_string("   ",_sx+3,_sy+2);
	put_string("   ",_sx,_sy+3);
	put_string("   ",_sx+3,_sy+3);

	put_string("AT",_sx,_sy+1);
	// put_number(commanders[cmdr_id].unit->atk+1,2,_sx+2,_sy+1);
	put_string("DF",_sx,_sy+2);
	// put_number(commanders[cmdr_id].unit->def,2,_sx+2,_sy+2);
	put_string("SP",_sx,_sy+3);
	// put_number(commanders[cmdr_id].unit->spd,2,_sx+2,_sy+3);

	put_string("Lv",_sx+5,_sy-4);
	// put_number(commanders[cmdr_id].lvl+1,2,_sx+8,_sy-4);
	put_string("HP",_sx+5,_sy-3);
	// put_number(commanders[cmdr_id].unit->hp,4,_sx+8,_sy-3);
	put_string("AP",_sx+5,_sy-2);
	// put_number(commanders[cmdr_id].ap,2,_sx+8,_sy-2);
}

void display_cmdr_army_info(char cmdr_id, int x, int y)
{
	char i, j, off;
	int _sx, _sy;
	_sx = s_x / 8 + x;
	_sy = s_y / 8 + y;

	for(i=0, j=0, off=0; i<3; i++, j+=4, off+=6)
	{
		put_string("         ",_sx,_sy+j);
		put_string(row_names+off,_sx,_sy+j);
		// put_number(commanders[cmdr_id].row_counts[i],1,_sx,_sy+j+1);
		put_string("/",_sx+1,_sy+j+1);
		put_number(MAX_UNITS_PER_ROW,1,_sx+2,_sy+j+1);
	}
}

void story(unsigned char area, unsigned char state, char id)
{
	int offset = 0, value = 0;
	offset = find_area_offset(area);//find_offset_by_area(area);

	if(state == EXPLORE)
	{
		offset = find_kv_offset(REQ,id,offset);
	}
	else
	{
		offset = find_kv_offset(state,0,offset);
		offset--;
	}

	if(offset)
	{
		hide_npcs(5);
		satb_update();
	}

	while(offset)
	{
		value = do_story(0,0,script+offset);
		// put_number(value,3,5,45);
		offset += value;
		if(value == 0)
		{
			return;
		}
		wait_for_I_input();
	}

	show_npcs(5);
}

int do_story(int x, int y, char *str)
{
	if(str[0] == 0)//dialog
	{
		display_info_panel(x,y,32,6);
		if(str[1] != 0)
		{
			load_portrait(str[1]);
			display_item(str[1],0,x+1,y+1);
			return write_text((s_x/8)+6,(s_y/8)+1,str+2) + 3;
		}
		else
		{
			return write_text((s_x/8)+1,(s_y/8)+1,str+2) + 3;
		}
	}
	return 0;
}

//this doesn't break and I think it should...
//this may cause some bugs down there line
int find_area_offset(char area)
{
	int offset=0;

	// offset = current_offset;
	while(offset < 2046)
	{
		if(script[offset++] == 0xFF)
		{
			if(script[offset++] == area)
			{
				return offset;
			}
		}
	}
	return 0;
}

int find_kv_offset(unsigned char key, unsigned char value, int current_offset)
{
	int offset=0;
	offset = current_offset;
	while(offset < 2046)//just a large number, need to change this to how long out actually is
	{
		if(script[offset] == 0xFF)//we've crossed into a new area
		{
			return 1;
		}
		if(script[offset++] == key)
		{
			//have to check this twice because on one occasion the FF marker comes directly after the key we're looking for
			if(script[offset] == 0xFF)
			{
				return 1;
			}
			if(script[offset++] == value)
			{
				return offset;
			}
		}
	}
	return 1;
}

void display_dialog(int x, int y, char *str)
{
	display_info_panel(x,y,32,8);
	// put_number(str[1],4,10,45);
	write_text((s_x/8)+1,(s_y/8)+1,str);
}

void display_info_panel(int x, int y, int width, int length)
{
	int j, i, z, vaddr, offset, size;
	// int ptr[288];
	size = length * width;
	offset = s_x/8 + ((s_y/8)*screen_dimensions) + (x*screen_dimensions) + y;
	// offset = 0;

	for(j=0; j<length; j++)
	{
		for(i=0; i<width; i++)
		{
			z = (j*width) + i;
			ptr[i] = (0x6400 >> 4) + ((i+(width-1)) / (width - ((i+(width-2))/(width-1)))) + ((((j+(length-1)) / (length - ((j+(length-2))/(length-1))))) * 3) + 0x8000;
		}
		vaddr = vram_addr(0,j);
		load_vram(vaddr+offset,ptr,width);
	}
}

void display_abs_info_panel(int x, int y, int width, int length)
{
	int j, i, z, vaddr, offset, size;
	size = length * width;
	offset = y*8+x;

	for(j=0; j<length; j++)
	{
		for(i=0; i<width; i++)
		{
			z = (j*width) + i;
			ptr[i] = (0x6400 >> 4) + ((i+(width-1)) / (width - ((i+(width-2))/(width-1)))) + ((((j+(length-1)) / (length - ((j+(length-2))/(length-1))))) * 3) + 0x8000;
		}
		vaddr = vram_addr(0,j);
		load_vram(vaddr+offset,ptr,width);
	}
}

void change_background_palette(int tile, int pal, int map_offset)
{
	int vaddr, x, y;
	int ptr[2];
	x = (tile%16)*2;
	y = ((tile/16)*2);// + ((yOffset/8));
	if(y < 6){ return; }
	vaddr = vram_addr(x,y);

	ptr[0] = (pal + 0x100) + ((battlefieldbat[map_offset+tile-32] * 4));
	ptr[1] = (pal + 0x100) + ((battlefieldbat[map_offset+tile-32] * 4) + 1);
	load_vram(vaddr,ptr,2);

	ptr[0] += 2;
	ptr[1] += 2;
	load_vram(vaddr+0x20,ptr,2);
}

load_healthbars()
{
	load_palette(2,healthbarpal,1);
	// load_vram(0x1300,healthbar,0xC0);
	load_vram(0xe00,healthbar,0xC0);
}

void display_healthbar(char x, char y)
{
	int j, vaddr;
	int ptr[1];

	for(j=0; j<5; j++)
	{
		vaddr = vram_addr(x+j,y);
		// ptr[0] = (0x1360 >> 4) + ((j+6) / (7 - ((j+5)/6))) + 0x2000;
		ptr[0] = (0xe60 >> 4) + ((j+4) / (5 - ((j+3)/4))) + 0x2000;
		load_vram(vaddr,ptr,1);
	}
}

void reduce_healthbar(char x, char y, char dmg, char i)
// void reduce_healthbar(char army_no, char dmg)
{
	int j, vaddr,z;
	int ptr[1];
	z=0;
	for(j=i+dmg-1; j >= dmg; j--)
	{
		vaddr = vram_addr(x+j,y);
		// ptr[0] = (0x1390 >> 4) + ((j+6) / (7 - ((j+5)/6))) + 0x2000;
		ptr[0] = (0xe90 >> 4) + ((j+4) / (5 - ((j+3)/4))) + 0x2000;
		load_vram(vaddr,ptr,1);
	}
}

int min(int a, int b)
{
	(a<b)? a : b;
}

int max(int a, int b)
{
	(a>b)? a : b;
}

int get_unit_cost(char id)
{
	switch(unit_list[id].unit_type)
	{
		case SPEARS:
		return 500;
		break;
		case INFANTRY:
		return 350;
		break;
		case FLYERS:
		return 600;
		break;
		case MUSKETS:
		return 400;
		break;
		case HOUNDS:
		return 200;
		break;
		case MAGES:
		return 400;
		break;
		default:
		return 100;
		break;
	}
}

void equip_item(int cmdr_index, int item_index, char item_type)
{
	char index = 0, item_id = 40;

	if(commanders[cmdr_index].no_of_items > 0)
	{
		item_id = commanders[cmdr_index].equipable;
	}
	index = get_item_real_index(item_index,item_type);
	commanders[cmdr_index].equipable = party_items[index];
	commanders[cmdr_index].no_of_items = 1;
	if(item_id != 40)
	{
		party_items[no_of_party_items++] = item_id;
	}
}

char get_item_real_index(char item_index, char item_type)
{
	char i=0, j=0, real_index = 0;
	for(i=0; i<no_of_party_items; i++)
	{
		if(items[party_items[i]].type == item_type)
		{
			if(j++ == item_index)
			{
				return i;
			}
		}
	}
	// return real_index;
	return 0;
}

void load_commanders_gfx(int cmdr_id, int address, int pal)
{
	// put_number(cmdr_id,3,10,5+(g++));
	switch(cmdr_id)
	{
		case REI:
		load_vram(address,rei_walk,0x100);
		load_palette(pal,rei_walk_pal,1);
		break;

		case VIOLET:
		load_vram(address,violet_walk,0x100);
		load_palette(pal,violet_walk_pal,1);
		break;

		case KING:
		load_vram(address,cat_walk,0x100);
		load_palette(pal,cat_walk_pal,1);
		break;

		// case BLOB_CMDR:
		// load_vram(address,blob,0x100);
		// load_palette(pal,blobpal,1);
		// break;
		//
		// case HOUND_CMDR:
		// load_vram(address,hnd,0x100);
		// load_palette(pal,dmnpal,1);
		// break;

		default:
		load_vram(address,sld,0x100);
		load_palette(pal,sldpal,1);
		break;
	}
}

void remove_unit_from_row(char *row, char position, unsigned char id)
{
	// char i;
	// if(position == 2)
	// {
	// 	return;
	// }
	// for(i=position;i<3;i++)
	// {
	// 	row[i] = row[i+1];
	// }
}

char next_level(char level, int exp)
{
	int current_exp = 0;
	current_exp = exp;
	if(level==9)
	{
		return 9;
	}
	while(EXP_CHART[level] < current_exp)
	{
		current_exp -= EXP_CHART[level++];
	}
	return level;
}

void load_units_by_cmdr_id(char cmdr_type, char cmdr_id)
{
	switch(cmdr_type)
	{
		case 10:
		commanders[cmdr_id].sprite_type = SWORD_UNIT;
		commanders[cmdr_id].unit = SWORD_UNIT;
		break;
		
		case 20:
		commanders[cmdr_id].sprite_type = SWORD_UNIT;
		commanders[cmdr_id].unit = SWORD_UNIT;
		break;

		case 21:
		commanders[cmdr_id].sprite_type = SPEAR_UNIT;
		commanders[cmdr_id].unit = SWORD_UNIT;
		break;

		case 22:
		commanders[cmdr_id].sprite_type = ARCHER_UNIT;
		commanders[cmdr_id].unit = ARCHER_UNIT;
		break;

		case 23:
		commanders[cmdr_id].sprite_type = HOUND_UNIT;
		commanders[cmdr_id].unit = HOUND_UNIT;
		break;

		case 24:
		commanders[cmdr_id].sprite_type = BLOB_UNIT;
		commanders[cmdr_id].unit = BLOB_UNIT;
		break;

		case 25:
		commanders[cmdr_id].sprite_type = AXE_UNIT;
		commanders[cmdr_id].unit = AXE_UNIT;
		break;
	}
}

void add_commander_to_party(char cmdr_id)
{
	if(party_size < MAX_PARTY_SIZE)
	{
		party[party_size++] = cmdr_id;
	}
}

#include "overworld.c"
#include "battle.c"
