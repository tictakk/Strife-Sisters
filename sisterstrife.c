#include <huc.h>
#include "paths.h"
#include "strifesisters.h"

#define CMDR 0
#define SWORD_UNIT 1
#define SPEAR_UNIT 2
#define ARCHER_UNIT 3
#define DEMON_UNIT 4
#define MAGE_UNIT 5
#define HOUND_UNIT 6
#define PORTRAIT_VRAM 0x3C00
#define CMDR_PANEL_VRAM 0x3600
#define RECRUIT_VRAM 0x4600
#define FONT_VRAM 0x5200
#define DIALOGUE_VRAM 0x4000
#define P_BORDER_VRAM 0x4000
#define CMDR_VRAM_SIZE 0x800
#define MAX_GLOBAL_UNITS 50
#define MAX_PARTY_SIZE 8
#define NUM_OF_ITEMS 25
#define MAX_INVENTORY 24

#define AGI_STONE 0
#define STR_STONE 1
#define	DEF_STONE 2
#define HP_STONE 3

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

#define MAP_WIDTH 32
#define TILE_WIDTH 16
#define TILE_VRAM 0x1000
#define OVERWORLD_MAP_HEIGHT 32
#define NO_OF_CASTLES 16

#define MAX_ARMY_SIZE 15
#define MAX_COMMANDERS 3
#define TOTAL_COMMANDERS 20

#define TAROS 0 //purple
#define KODEN 1 //dark green
#define BAKURIA 2 //yeller
#define SANGAI 3 //blue
#define KAYTOKEN 4 //red

// #define MAP_32x32 0
// #define MAP_16x16 1

#incspr(cursor, "map/cursor.pcx");
#incpal(cursorpal, "map/cursor.pcx");

// #incpal(menupal, "map/sprites/inbattle.pcx");

enum GAME_STATE{
	OVERWORLD, BATTLE_SELECT, BATTLE
};

enum Unit_Type{
	INFANTRY, FLYERS, SPEARS, MUSKETS, MAGES, HOUNDS, COMMANDER
};

typedef struct{
	unsigned char atk, def, spd;
	int hp;
} Stats;

typedef struct{
	unsigned char atk, def, mov, rng, ign, spd, p_group, id;
	int hp, address;
	enum Unit_Type unit_type;
} Unit;

// typedef struct{
  // Unit *unit;
  // char army_size, stars;
	// unsigned int exp;
	// Stats stat;
// } GEntity;

typedef struct{
	int cost;
	char type, attribute_num, modified;
	const char *name;
	const char *description;
} Item;

struct Commander{
	int exp;
	char lvl;
	char id;
	char no_of_items;
	char ap;
	char hp;
	char equipable;
	char *name;
	char units[6];
	Unit unit;
	// Stats stat;
};

struct Castle{
	char buyable_units[MAX_COMMANDERS];
	int x, y;
	char no_of_commanders, no_of_soldiers, id, owner, no_of_items, no_of_units;
	char commanders[MAX_COMMANDERS];
	char buyable_items[MAX_COMMANDERS];
};

//Map images
#incbin(overworldmap,"tiles/strifesisters_overworld.strifersisters.layer-Layer 1.map001.stm")
#inctilepal(overworldtilespal,"tiles/strifesisters_overworld.tiles.pcx")
#incpal(overworldpal,"tiles/strifesisters_overworld.tiles.pcx")
#inctile(overworldtiles,"tiles/strifesisters_overworld.tiles.pcx")

// #inctile(battlefield,"map/backgrounds/battlefieldmap.tiles.pcx")
// #incpal(battlefieldpal, "map/backgrounds/battlefieldmap.tiles.pcx")
#incbin(battlefieldbat, "tiles/battlefield1.Untitled 3.layer-Layer 1.map001.stm")

// #inctile(battlefield,"map/backgrounds/battlefieldmap.tiles.pcx")
// #incpal(battlefieldpal, "map/backgrounds/battlefieldmap.tiles.pcx")
// #incbin(battlefieldbat, "map/backgrounds/battlefieldmap.Untitled 4.layer-Layer 1.map001.stm")

// #incchr(castle,"map/sprites/castle.pcx");
// #incchr(castle1,"map/sprites/castle1.pcx");
// #incchr(castle2,"map/sprites/castle2.pcx");
// #incchr(castle3,"map/sprites/castle3.pcx");
// #incchr(castle4,"map/sprites/castle4.pcx");
// #incpal(castlepal, "map/sprites/castle.pcx");

#incspr(selector,"map/sprites/selector.pcx");
#incpal(selectorpal,"map/sprites/selector.pcx");

#incchr(build_screen, "map/sprites/build_screen.pcx")
#incpal(build_pal, "map/sprites/build_screen.pcx")
#incbat(build_bat, "map/sprites/build_screen.pcx",0x1000,32,28)
//
// #incchr(explain_menu, "map/sprites/explain_menu.pcx")
// #incpal(explain_pal, "map/sprites/explain_menu.pcx")
// #incbat(explain_bat, "map/sprites/explain_menu.pcx",0x1000,32,28)

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
struct Commander commanders[TOTAL_COMMANDERS+5];
int total_sprites = 0;
// int counter = 0;
// char current_menu_size = 0;
char no_of_party_items;
char num_of_bad_terrains;
char untraversable_terrain[15];

Item items[NUM_OF_ITEMS];
char global_units[MAX_GLOBAL_UNITS];
char troops[12];
char party[MAX_PARTY_SIZE];
char party_items[MAX_INVENTORY];
char party_size;
char no_of_troops;
char enemy[5];

//spawns
char num_of_spawns = 0;
int spawn_positions[6];
char enemy_spawns[30];
char spawn_commanders[6];
char spawn_group_size[6];

//merge_units
char merge_units[2];
char merge_pointer;

/*
	MENU STUFF
*/

#define CASTLE_INFO_MENU 2
#define PRE_BATTLE_MENU 4
#define BATTLE_MENU 8
#define ARTS_MENU 16
#define FORMATION_MENU 32

#define EXIT_OPTION 1
#define SELECT_OPTION 2

#define CURSOR_SIZE 1
#define MAX_SPRITES 63

#define CONSUMABLE 0
#define EQUIPABLE 1
#define ANY_ITEM 2

extern unsigned char current_menu = 0;
extern unsigned char menu_state = 0;
extern unsigned char menu_select = 0;

// #incspr(castleinfo, "map/sprites/castleinfo.pcx")
// #incspr(castlemenu, "map/sprites/castlemenu.pcx")
// #incpal(castlemenupal, "map/sprites/castlemenu.pcx")

// #incspr(taros, "map/sprites/Taros.pcx");
// #incspr(koden, "map/sprites/Koden.pcx");
// #incspr(kaytoken, "map/sprites/Kaytoken.pcx");
// #incspr(sangai, "map/sprites/Sangai.pcx");
// #incspr(bakuria, "map/sprites/Bakuria.pcx");

// #incspr(dialog,"map/sprites/dialog.pcx");
// #incpal(walkerpal, "map/sprites/walker.pcx");

char current_menu = 0;
char current_menu_length = 0;
char current_menu_width = 0;
char current_menu_choices = 0;

char kingdom = 0;

int menu_min = 0;
int menu_max = 0;
int cursor_vram = 0;
int current_global_units = 0;
// int z = 0;

/*
	BATTLE STUFF
*/
#define SOLDIER_SIZE 32
#define BATTLE_MAP_HEIGHT 16

struct Node{
	int ownX, ownY, fromX, fromY;
	char checked;
};

enum SoldierState{
	IDLE, ATTACKING, DYING
};

enum Direction{
	NORTH, NORTHEAST, EAST, SOUTHEAST, SOUTH, SOUTHWEST, WEST, NORTHWEST
};

struct soldier{
	int x, y, frame;
	char tic, id;
	enum SoldierState state;
	enum Direction direction;
	char attack, active;
};

/* sprites */
#incspr(attack, "map/sprites/swordy.pcx")
#incpal(soldierpal, "map/sprites/swordy.pcx",0,2)

#incspr(attack2, "map/sprites/spear_anim.pcx")
#incpal(spearpal, "map/sprites/spear_anim.pcx",0,2)

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

// #incchr(cmdr_info_panel, "map/sprites/cmdr_info_panel.pcx",0,0,8,12)

//items
#incchr(potion_gfx, "map/sprites/healthpotion.pcx",0,0,4,4)
#incpal(potionpal, "map/sprites/healthpotion.pcx")

#incchr(elixir_gfx, "map/sprites/elixir.pcx",0,0,4,4)
#incpal(elixirpal, "map/sprites/elixir.pcx")

#incchr(shield_gfx, "map/sprites/shield.pcx",0,0,4,4)
#incpal(shieldpal, "map/sprites/shield.pcx")

#incspr(msk,"map/sprites/msktpiece.pcx")
#incspr(msk1,"map/sprites/msktpiece1.pcx")
#incspr(sld,"map/sprites/sldpiece.pcx")
#incspr(sld1,"map/sprites/sldpiece1.pcx")
#incspr(sld0,"map/sprites/sldpiece0.pcx")
#incspr(sld2,"map/sprites/sldpiece2.pcx")
#incspr(spr,"map/sprites/sprpiece.pcx")
#incspr(spr1,"map/sprites/sprpiece1.pcx")
#incspr(spr0,"map/sprites/sprpiece0.pcx")
#incspr(spr2,"map/sprites/sprpiece2.pcx")
#incspr(dmn,"map/sprites/demonpiece.pcx")
#incspr(dmn1,"map/sprites/demonpiece1.pcx")
#incspr(dmn0,"map/sprites/demonpiece0.pcx")
#incspr(dmn2,"map/sprites/demonpiece2.pcx")
#incspr(hnd,"map/sprites/houndpiece.pcx")
#incspr(hnd1,"map/sprites/houndpiece2.pcx")
#incspr(arch0,"map/sprites/archpiece0.pcx")
#incspr(arch2,"map/sprites/archpiece2.pcx")
// #incspr(actionbtn, "map/sprites/action.pcx")

#incpal(sldpal, "map/sprites/sldpiece.pcx",0,2)
#incpal(sprpal, "map/sprites/sprpiece.pcx",0,2)
// #incpal(actionpal, "map/sprites/action.pcx")
#incpal(mskpal,"map/sprites/msktpiece.pcx",0,2)
#incpal(dmnpal,"map/sprites/demonpiece.pcx",0,2)
#incpal(hndpal,"map/sprites/houndpiece.pcx",0,2)

const int sprite_memory_start = 0x5000;
const int sprite_size = 0x800;
const int valid_spawn_points[] = { 421, 466, 355, 359, 329, 425 };

int army_one_x_start = 0;
int army_one_y_start = 0;
int army_two_x_start = 0;
int army_two_y_start = 0;
int avg = 0;
// char army_one_current_size = ARMY_ONE_SIZE;
// char army_two_current_size = ARMY_TWO_SIZE;
int commander_one_pos = 0; //1,3
int commander_two_pos = 0; //15,3

char team_one_arts_flag = 0;
char team_two_arts_flag = 0;

int menu_size = 0;
unsigned int player_gold = 0;

// Unit spears;
// Unit flyers;
// Unit infantry;
// Unit muskets;
// Unit mages;
// Unit hounds;
Unit unit_list[8];

char d_one[4];
char d_two[4];
struct soldier armyOne[15]; //9 is max?
struct soldier armyTwo[15];
struct soldier *sp;

int map_counter = 0;
int map_size = 0;
int soldierFrame = 0;
int altFrame = 0;
int frame = 0x5000;
int tic = 0;
int sx = 0;
int sy = 0;
int yOffset = 0;
char j1 = 0;
int j2 = 0;
int xOffset = 0;
int total_units = 0;

// int team_id = 0;
// char commander_one_hp = 10;
// char commander_two_hp = 10;
// char sprite_offset = 0;
// char commander_select = 0;

// int TEAM_ONE_ID = 1;
// int TEAM_TWO_ID = 2;
// int ATTACK_AREA_ONE = 4;
// int ATTACK_AREA_TWO = 8;

main()
{
	party_size = 7;
	d_one[0] = 3;
	d_two[0] = 2;
	d_one[1] = 1;
	d_two[1] = 4;
	d_one[2] = 5;
	d_two[2] = 6;
	d_one[3] = 7;
	d_two[3] = 8;

	selector_x = 0;
	selector_y = 64;
	cursor_vram = 0x68C0;

	unit_list[SPEAR_UNIT].hp  = 100;
	unit_list[SPEAR_UNIT].atk = 16;
	unit_list[SPEAR_UNIT].def = 13;
	unit_list[SPEAR_UNIT].rng = 2;
	unit_list[SPEAR_UNIT].mov = 3;
	unit_list[SPEAR_UNIT].ign = 0;
	unit_list[SPEAR_UNIT].spd = 10;
	unit_list[SPEAR_UNIT].p_group = 0;
	unit_list[SPEAR_UNIT].address = 0x3A00;
	unit_list[SPEAR_UNIT].unit_type = SPEARS;

	unit_list[DEMON_UNIT].hp  = 70;
	unit_list[DEMON_UNIT].atk = 16;
	unit_list[DEMON_UNIT].def = 12;
	unit_list[DEMON_UNIT].rng = 1;
	unit_list[DEMON_UNIT].mov = 4;
	unit_list[DEMON_UNIT].ign = 1;
	unit_list[DEMON_UNIT].spd = 15;
	unit_list[DEMON_UNIT].p_group = 4;
	unit_list[DEMON_UNIT].address = 0x3E00;
	unit_list[DEMON_UNIT].unit_type = FLYERS;

	unit_list[SWORD_UNIT].hp  = 90;
	unit_list[SWORD_UNIT].atk = 14;
	unit_list[SWORD_UNIT].def = 14;
	unit_list[SWORD_UNIT].mov = 3;
	unit_list[SWORD_UNIT].rng = 1;
	unit_list[SWORD_UNIT].ign = 0;
	unit_list[SWORD_UNIT].spd = 12;
	unit_list[SWORD_UNIT].p_group = 0;
	unit_list[SWORD_UNIT].address = 0x3800;
	unit_list[SWORD_UNIT].unit_type = INFANTRY;

	unit_list[ARCHER_UNIT].hp  = 70;
	unit_list[ARCHER_UNIT].atk = 17;
	unit_list[ARCHER_UNIT].def = 10;
	unit_list[ARCHER_UNIT].rng = 2;
	unit_list[ARCHER_UNIT].mov = 3;
	unit_list[ARCHER_UNIT].ign = 0;
	unit_list[ARCHER_UNIT].spd = 8;
	unit_list[ARCHER_UNIT].p_group = 0;
	unit_list[ARCHER_UNIT].address = 0x3C00;
	unit_list[ARCHER_UNIT].unit_type = MUSKETS;

	unit_list[MAGE_UNIT].atk = 20;
	unit_list[MAGE_UNIT].def = 10;
	unit_list[MAGE_UNIT].hp  = 60;
	unit_list[MAGE_UNIT].ign = 0;
	unit_list[MAGE_UNIT].spd = 5;
	unit_list[MAGE_UNIT].mov = 3;
	unit_list[MAGE_UNIT].p_group = 0;
	unit_list[MAGE_UNIT].unit_type = MAGES;

	unit_list[HOUND_UNIT].atk = 15;
	unit_list[HOUND_UNIT].def = 27;
	unit_list[HOUND_UNIT].hp  = 60;
	unit_list[HOUND_UNIT].ign = 0;
	unit_list[HOUND_UNIT].mov = 4;
	unit_list[HOUND_UNIT].spd = 11;
	unit_list[HOUND_UNIT].p_group = 4;
	unit_list[HOUND_UNIT].address = 0x4000;
	unit_list[HOUND_UNIT].unit_type = HOUNDS;
	// make_menus();
	initialize_commanders();

	commanders[0].name = "Rei";
	commanders[0].no_of_items = 1;
	commanders[0].equipable = 1;
	commanders[1].name = "Violet";
	// commanders[1].no_of_items = 0;
	commanders[2].name = "Scen";
	commanders[3].name = "Alucard";
	commanders[4].name = "Stimpy";
	commanders[5].name = "Sacha";
	commanders[6].name = "Almer"; //Olimar :)
	commanders[7].name = "Ven";
	commanders[8].name = "Phalc";
	commanders[9].name = "Keely";

	add_unit_to_pool(1,15,1,SPEAR_UNIT,0,0,0,0);
	add_unit_to_pool(1,15,1,SWORD_UNIT,0,0,0,0);
	add_unit_to_pool(1,15,1,ARCHER_UNIT,0,0,0,0);
	add_unit_to_pool(1,15,1,SWORD_UNIT,0,0,0,0);

	add_unit_to_pool(1,15,1,SPEAR_UNIT,0,0,0,0);
	add_unit_to_pool(1,15,1,SWORD_UNIT,0,0,0,0);
	add_unit_to_pool(1,15,1,SWORD_UNIT,0,0,0,0);
	add_unit_to_pool(1,15,1,SWORD_UNIT,0,0,0,0);
	add_unit_to_pool(1,15,1,SPEAR_UNIT,0,0,0,0);
	add_unit_to_pool(1,15,1,DEMON_UNIT,0,0,0,0);

	troops[0] = SPEAR_UNIT;
	troops[1] = SWORD_UNIT;
	troops[2] = ARCHER_UNIT;
	troops[3] = SWORD_UNIT;
	troops[4] = ARCHER_UNIT;
	troops[5] = SWORD_UNIT;
	troops[6] = SWORD_UNIT;

	enemy[0] = SWORD_UNIT;
	enemy[1] = SWORD_UNIT;
	enemy[2] = SWORD_UNIT;
	enemy[3] = SWORD_UNIT;

	no_of_troops = 7;

	items[0].name = "A Gem";
	items[0].type = CONSUMABLE;
	items[1].name = "Shield";
	items[1].type = EQUIPABLE;
	items[1].description = "+5 Def";
	items[2].name = "Thing3";
	items[2].type = EQUIPABLE;
	items[3].name = "Potion";
	items[3].type = CONSUMABLE;
	items[3].description = "+5 HP";
	items[4].name = "Elixir";
	items[4].type = CONSUMABLE;
	items[4].description = "+5 AP";
	items[5].name = "S Gem";
	items[5].type = CONSUMABLE;
	items[6].name = "D Gem";
	items[6].type = CONSUMABLE;
	items[7].name = "L S Gem";
	items[7].type = CONSUMABLE;
	items[8].name = "Thing9";
	items[8].type = CONSUMABLE;
	items[9].name = "Thing10";
	items[9].type = CONSUMABLE;
	items[10].name = "Thing11";
	items[10].type = CONSUMABLE;
	items[11].name = "Thing12";
	items[11].type = CONSUMABLE;

	castles[8].buyable_items[0] = 3;
	castles[8].buyable_items[1] = 1;
	castles[8].buyable_items[2] = 4;

	castles[8].no_of_units = 2;
	// castles[8].buyable_units[0].stars = 2;
	// castles[8].buyable_units[0].level = 1;
	castles[8].buyable_units[0] = SPEAR_UNIT;

	// castles[8].buyable_units[1].stars = 4;
	// castles[8].buyable_units[1].level = 1;
	castles[8].buyable_units[1] = SWORD_UNIT;

	items[3].cost = 125;
	items[1].cost = 350;
	items[4].cost = 500;

	player_gold = 10000;
	party[0] = 0;
	party[1] = 1;
	party[2] = 3;
	party[3] = 5;
	party[4] = 2;
	party[5] = 4;
	party[6] = 6;
	party_items[0] = 2; //thing3 -> equip
	party_items[1] = 4; //elixir -> consume
	party_items[2] = 5; //thing6 -> consume
	party_items[3] = 6; //thing7 -> consume
	party_items[4] = 5; //thing6 -> consume
	party_items[5] = 3; //potion -> consume
	party_items[6] = 1; //potion -> consume
	party_items[7] = 3; //potion -> consume
	party_items[8] = 3; //potion -> consume
	party_items[9] = 3; //potion -> consume
	party_items[10] = 3; //potion -> consume
	no_of_party_items = 11;

	disp_off();
	cls();
	init_satb();

	load_palette(19, cursorpal, 1);
	// load_palette(17, walkerpal,1);
	load_vram(0x68C0,cursor,0x40);
	satb_update();

	// display_intro();
	for(;;)
	{
		// battlefield_loop(commanders[0].army_units,4,commanders[1].army_units,4);
		overworld_loop();
		// battle_loop(0,0);
		// battlefield_loop(d_one,4,d_two,4);
	}
}

void display_intro()
{
	load_background(build_screen,build_pal,build_bat,32,28);
	disp_on();
	sync(60*10);

	// load_background(explain_menu,explain_pal,explain_bat,32,28);
	// while(!(joytrg(0) & JOY_RUN)){}
	disp_off();
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

void print_unit_type(enum Unit_Type type, int x, int y)
{
	switch(type)
	{
		case INFANTRY:
		put_string("SWD",x,y);
		// put_number(1,2,x,y);
		break;

		case FLYERS:
		put_string("DMN",x,y);
		// put_number(4,2,x,y);
		break;

		case SPEARS:
		put_string("SPR ",x,y);
		// put_number(2,2,x,y);
		break;

		case MUSKETS:
		put_string("ARC",x,y);
		// put_number(3,2,x,y);
		break;

		case MAGES:
		put_string("MAG",x,y);
		// put_number(5,2,x,y);
		break;

		case HOUNDS:
		put_string("HND",x,y);
		// put_number(6,2,x,y);
		break;

		default:
		put_string("error",x,y);
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

void display_sprite_by_no(int x_pos, int y_pos, int location, char flip, char pal, char sprite_no)
{
	spr_make(sprite_no,x_pos,y_pos,location,flip,NO_FLIP|SZ_16x16,pal,0);
	spr_pri(1);
}

void load_cursor(int x, int y)
{
	display_sprite_by_no(x,y,cursor_vram,FLIP_MAS|SIZE_MAS,3,1);
}

void tick(){
	// int i = 0;
	if(++tic > 6)
	{
		tic = 0;
		++frame;
		soldierFrame += 0x100;
		altFrame += 0x100;
	}
	if(frame > 3){
		frame = 0;
		soldierFrame = 0;
		altFrame = 0;
		// soldierFrame = 0x4000;
		// altFrame = 0x5000;
	}
}

void write_text(char x, char y, char *text)
{
	char i, text_y, text_x, sprite_no;
	sprite_no = 2;
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
		// put_number(text[i],3,0,5+(g++));
		put_char(text[i],text_x,text_y);
		i++;
		text_x++;
		vsync(2);
	}
}

void add_unit_to_pool(char stars, char size, char level, char unit_number, char atk, char def, int hp, char spd)
{
	// memcpy(&global_units[i],&global_units[i+1],sizeof(GEntity));
  // global_units[current_global_units].army_size = size;
	// global_units[current_global_units].atk = unit->atk+atk;
	// global_units[current_global_units].def = unit->def+def;
	// global_units[current_global_units].hp = unit->hp+hp;
	// global_units[current_global_units].spd = unit->spd+spd;
	// set_unit_stats(&global_units[current_global_units]);

	// memcpy(&global_units[current_global_units],unit,sizeof(Unit));

	global_units[current_global_units] = unit_number;
  current_global_units += 1;
}

initialize_commanders()
{
	char i;
	struct Commander *cmdr;
	// infantry.hp  = 90;
	// infantry.atk = 14;
	// infantry.def = 14;
	// infantry.mov = 3;
	// infantry.rng = 1;
	// infantry.ign = 0;
	// infantry.spd = 12;
	// infantry.p_group = 0;
	// infantry.address = 0x3800;
	// infantry.unit_type = INFANTRY;
	for(i=0, cmdr = commanders; i<TOTAL_COMMANDERS; i++, cmdr++)
	{
		// cmdr->hp = 5+i;
		cmdr->ap = 5;
		// cmdr->unit.hp = 90 * 15;
		// cmdr->unit.atk = 14;
		// cmdr->unit.def = 14;
		// cmdr->unit.mov = 3;
		// cmdr->unit.ign = 0;
		// cmdr->unit.spd = 12;
		// cmdr->unit.p_group = 0;
		// cmdr->unit.address = 0x00;
		// cmdr->exp = 0;

		cmdr->lvl = 1;
		cmdr->id = i;
		cmdr->no_of_items = 0;
	}
	commanders[TOTAL_COMMANDERS+4].ap = 5;
	commanders[TOTAL_COMMANDERS+4].id = 25;
	commanders[TOTAL_COMMANDERS+4].name = "Bandit";
}

void darken_palette(int pal_num)
{
	modify_palette(pal_num,-1);
}

void lighten_palette(int pal_num)
{
	modify_palette(pal_num,2);
}

char is_traversable(int pos)
{
  char i;
  for(i=0; i<num_of_bad_terrains; i++)
  {
    if(battlefieldbat[pos] == untraversable_terrain[i])
    {
      return 0;
    }
  }
  return 1;
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
		case 1:
		load_vram(PORTRAIT_VRAM + (index * 0x100),shield_gfx,0x100);
		load_palette(12+index,shieldpal,1);
		break;

		case 3:
		load_vram(PORTRAIT_VRAM + (index * 0x100),potion_gfx,0x100);
		load_palette(12+index,potionpal,1);
		break;

		default:
		load_vram(PORTRAIT_VRAM + (index * 0x100),elixir_gfx,0x100);
		load_palette(12+index,elixirpal,1);
		break;
	}
}

void display_spawn(int index, int spawn_x, int spawn_y, int sprite_offset, int spawn_offset)
{
	switch((unit_list[index]).unit_type)
	{
		case INFANTRY:
		load_vram(0x3800+(spawn_offset*0x200),sld0,0x100);
		load_vram(0x3900+(spawn_offset*0x200),sld2,0x100);
		load_palette(18+spawn_offset,sldpal,1);
		break;
		case SPEARS:
		load_vram(0x3800+(spawn_offset*0x200),spr0,0x100);
		load_vram(0x3900+(spawn_offset*0x200),spr2,0x100);
		load_palette(18+spawn_offset,sprpal,1);
		break;
		case FLYERS:
		load_vram(0x3800+(spawn_offset*0x200),dmn0,0x100);
		load_vram(0x3900+(spawn_offset*0x200),dmn2,0x100);
		load_palette(18+spawn_offset,dmnpal,1);
		break;
		case MUSKETS:
		load_vram(0x3800+(spawn_offset*0x200),arch0,0x100);
		load_vram(0x3900+(spawn_offset*0x200),arch2,0x100);
		load_palette(18+spawn_offset,mskpal,1);
		break;
		case HOUNDS:
		load_vram(0x3800+(spawn_offset*0x200),hnd,0x100);
		load_vram(0x3900+(spawn_offset*0x200),hnd1,0x100);
		load_palette(18+spawn_offset,hndpal,1);
		break;
	}
	spr_make(10+spawn_offset,spawn_x,spawn_y,0x3800+(spawn_offset*0x200),FLIP_MAS|SIZE_MAS,SZ_16x32,18+spawn_offset,1);
}

void display_item(char cmdr_id, int index, int x, int y)
{
	int bat[16];
	int offset, i;
	offset = s_x/8 + ((s_y/8)*32) + (x*32) + y;

	for(i=0; i<16; i++)
	{
		bat[i] = (((PORTRAIT_VRAM + (index*0x100))>>4)+(0xC000+(0x1000 * index))) + i;
	}
	load_bat(offset,bat,4,4);
}

void load_portrait(char cmdr_id)
{
	switch(cmdr_id)
	{
		case 0:
		load_vram(PORTRAIT_VRAM,rei_gfx,0x100);
		load_palette(12,rei_pal,1);
		break;

		case 1:
		load_vram(PORTRAIT_VRAM,violet_gfx,0x100);
		load_palette(12,violet_pal,1);
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

	put_string("       ",_sx,_sy+1);
	put_string("    ",_sx,_sy+2);
	put_string("    ",_sx+3,_sy+2);
	put_string("    ",_sx,_sy+3);
	put_string("    ",_sx+3,_sy+3);
	put_string("    ",_sx,_sy+4);
	put_string("    ",_sx+3,_sy+4);
	put_string("       ",_sx,_sy+6);

	put_string(commanders[cmdr_id].name,_sx,_sy+1);
	put_string("Lv:",_sx,_sy+2);
	put_number(commanders[cmdr_id].lvl,2,_sx+3,_sy+2);
	put_string("AP:",_sx,_sy+3);
	put_number(commanders[cmdr_id].ap,2,_sx+3,_sy+3);
	put_string("EX:",_sx,_sy+4);
	put_number(commanders[cmdr_id].exp,2,_sx+3,_sy+4);
	put_string("Item:",_sx,_sy+6);

	if(commanders[cmdr_id].no_of_items)
	{
		put_string(items[commanders[cmdr_id].equipable].name,_sx,_sy+7);
	}
	else
	{
		put_string("none    ",_sx,_sy+7);
	}
}

void display_dialog(int x, int y, char *str)
{
	display_info_panel(x,y,32,8);
	// put_string("dialog, in this economy?",1,1);
	write_text((s_x/8)+1,(s_y/8)+1,str);
}

void display_info_panel(int x, int y, int width, int length)
{
	int j, i, z, vaddr, offset, size;
	int ptr[288];
	size = length * width;
	offset = s_x/8 + ((s_y/8)*64) + (x*64) + y;
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

void load_information_panel(char box)
{
	int i, addr;
	int *p;

	if(box)
	{
		load_vram(0x1500,dialog_gfx,0x800);
		load_palette(5,dialog_pal,1);
	}
	else
	{
		load_vram(0x1500,info_gfx,0x800);
		load_palette(5,info_pal,1);
		load_palette(4,icons_pal,1);
		load_vram(0x9A0,icons_gfx,0x20);
		change_palette(0,0);
	}
	for(i=0; i<128; i++)
	{
		addr = vram_addr((i%32),(i/32));
		p = (0x150 + 0x5000) + i;
		load_vram(addr,p,1);
	}
}

void change_background_palette(int tile, int pal)
{
	int vaddr, x, y;
	int ptr[2];
	x = (tile%16)*2;
	y = ((tile/16)*2);
	if(y < 6){ return; }
	vaddr = vram_addr(x,y);

	// ptr[0] = (pal + 0x180) + (((tile)/16) * 0x40) + ((tile%16) * 2);
	// ptr[1] = (pal + 0x180) + (((tile)/16) * 0x40) + ((tile%16) * 2) + 1;
	ptr[0] = (pal + 0x100) + ((battlefieldbat[tile-32] * 4));
	ptr[1] = (pal + 0x100) + ((battlefieldbat[tile-32] * 4) + 1);
	load_vram(vaddr,ptr,2);

	ptr[0] += 2;
	ptr[1] += 2;
	load_vram(vaddr+0x20,ptr,2);
}

void change_palette(int v, int pal)
{
	int vaddr;
	int ptr[1];
	vaddr = vram_addr(6,2);
	ptr[0] = (0x4000) + (0x9A0 >> 4);
	load_vram(vaddr,ptr,1);

	vaddr = vram_addr(10,2);
	ptr[0] = (0x4000) + (0x9A0 >> 4) + 1;
	load_vram(vaddr,ptr,1);
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

	for(j=0; j<7; j++)
	{
		vaddr = vram_addr(x+j,y);
		// ptr[0] = (0x1360 >> 4) + ((j+6) / (7 - ((j+5)/6))) + 0x2000;
		ptr[0] = (0xe60 >> 4) + ((j+6) / (7 - ((j+5)/6))) + 0x2000;
		load_vram(vaddr,ptr,1);
	}
}

void reduce_healthbar(char x, char y, char dmg, char i, char id)
// void reduce_healthbar(char army_no, char dmg)
{
	int j, vaddr,z;
	int ptr[1];
	z=0;
	for(j=i+dmg-1; j >= dmg; j--)
	{
		if(id==2){
			put_number(j,3,0,z++);
		}
		vaddr = vram_addr(x+j,y);
		// ptr[0] = (0x1390 >> 4) + ((j+6) / (7 - ((j+5)/6))) + 0x2000;
		ptr[0] = (0xe90 >> 4) + ((j+6) / (7 - ((j+5)/6))) + 0x2000;
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

void set_sprite_to_location(char id, int location, char pal_num, char offset)
{
	switch((unit_list[id]).unit_type)
	{
		case INFANTRY:
		load_vram(location,sld,0x200);
		load_vram(location-0x200,sld1,0x200);
		// load_palette(pal_num,sldpal+offset,1);
		break;

		case SPEARS:
		load_vram(location,spr,0x200);
		load_vram(location-0x200,spr1,0x200);
		// load_palette(pal_num,sprpal+offset,1);
		break;

		case MUSKETS:
		load_vram(location,msk,0x200);
		load_vram(location-0x200,msk1,0x200);
		// load_palette(pal_num,mskpal+offset,1);
		break;

		case FLYERS:
		load_vram(location,dmn,0x200);
		load_vram(location-0x200,dmn1,0x200);
		// load_palette(pal_num,dmnpal+offset,1);
		break;

		case HOUNDS:
		load_vram(location,hnd,0x200);
		load_vram(location-0x200,hnd1,0x200);
		// load_palette(pal_num,hndpal+offset,1);
		break;

		default:
		load_vram(location,sld,0x200);
		load_vram(location-0x200,sld1,0x200);
		// load_palette(pal_num,sldpal+offset,1);
		break;
	}
}

int get_unit_cost(char id)
{
	switch(unit_list[id].unit_type)
	{
		case SPEARS:
		return 500;
		case INFANTRY:
		return 350;
		case FLYERS:
		return 600;
		case MUSKETS:
		return 400;
		case HOUNDS:
		return 200;
		case MAGES:
		return 400;
		default:
		return 100;
	}
}

void equip_item(int cmdr_index, int item_index, char item_type)
{
	char index = 0, item_id = 40;

	if(commanders[party[cmdr_index]].no_of_items > 0)
	{
		item_id = commanders[party[cmdr_index]].equipable;
	}
	index = get_item_real_index(item_index,item_type);
	commanders[party[cmdr_index]].equipable = party_items[index];
	commanders[party[cmdr_index]].no_of_items = 1;
	remove_item_from_inventory(index);
	if(item_id != 40)
	{
		party_items[no_of_party_items++] = item_id;
	}
}

void consume_item(int cmdr_index, int item_index, char item_type)
{
	remove_item_from_inventory(get_item_real_index(item_index,item_type));
	// remove_item_from_inventory(item_index,item_type);
	//do thing item is supposed to do
}

void remove_spawn_by_index(int index)
{
	int i, j;
	for(i=index; i<num_of_spawns; i++)
	{
		for(j=0; j<5; j++)
		{
			enemy_spawns[i*5+j] = enemy_spawns[(i+1)*5+j];
		}
		spawn_positions[i] = spawn_positions[i+1];
		spawn_group_size[i] = spawn_group_size[i+1];
	}
	num_of_spawns--;
}

void remove_unit_from_game(int unit_index)
{
	int i;
	for(i=0; i<no_of_troops; i++)
	{
		if(troops[i] > unit_index)
		{
			--troops[i];
		}
	}
	for(i=0; i<num_of_spawns; i++)
	{
		if(enemy_spawns[i] > unit_index)
		{
			--enemy_spawns[i];
		}
	}
	// if(merge_units[0] > unit_index){ --merge_units[0]; }
	if(merge_units[1] > unit_index){ --merge_units[1]; }

	for(i=unit_index; i<current_global_units-1; i++)
	{
		// memcpy(&global_units[i],&global_units[i+1],sizeof(Unit));
		global_units[i] = global_units[i+1];
	}
	current_global_units--;
}

void remove_unit_from_party(int unit_id)
{
	int i, location;
	location = 0;
	for(i=0; i<no_of_troops; i++)
	{
		if(troops[i] == unit_id)
		{
			location = i;
		}
	}
	for(i=location; i<no_of_troops; i++)
	{
		troops[i] = troops[i+1];
	}
	no_of_troops--;
}

void remove_item_from_inventory(char item_index)//, char item_type)
{
	char i = 0;//, j = 0, real_index = 0;

	// for(i=0; i<no_of_party_items; i++)
	// {
	// 	if(items[party_items[i]].type == item_type)
	// 	{
	// 		if(j++ == item_index)
	// 		{
	// 			real_index = i;
	// 		}
	// 	}
	// }
	// real_index = get_item_real_index(item_index,item_type);

	for(i=item_index; i<no_of_party_items; i++)
	{
		party_items[i] = party_items[i+1];
	}
	no_of_party_items--;
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
				real_index = i;
			}
		}
	}
	return real_index;
}

// void combine_units(GEntity *unit_one, GEntity *unit_two)
// {
	// char atk, def, spd, hp;
	// atk = (unit_one->stat.atk + unit_two->stat.atk)/2;
	// def = (unit_one->stat.def + unit_two->stat.def)/2;
	// spd = (unit_one->stat.spd + unit_two->stat.spd)/2;
	// hp  = (unit_one->stat.hp + unit_two->stat.hp)/2;
	// add_gentity(0,15,0,unit_one->unit,atk,def,hp,spd);
// }

void load_palette_groups()
{
	load_palette(17,sprpal,4);
	load_palette(21,dmnpal,4);
	load_palette(25,hndpal,4);
}

void load_units(int l)
{
	int location;
	location = l;
	load_vram((location),sld0,0x100);
	load_vram((location+=0x100),sld2,0x100);
	load_vram((location+=0x100),spr0,0x100);
	load_vram((location+=0x100),spr2,0x100);
	load_vram((location+=0x100),arch0,0x100);
	load_vram((location+=0x100),arch2,0x100);
	load_vram((location+=0x100),dmn0,0x100);
	load_vram((location+=0x100),dmn2,0x100);
	load_vram((location+=0x100),hnd,0x100);
	load_vram((location+=0x100),hnd1,0x100);
}

int next_level(int level)
{
	return (4*(level*level*level))/5;
}

int get_exp(int level) //just a placeholder really
{
	return (level*level)*10;
}

char get_random_stat_category()
{
	int r_stat;
	r_stat = rand() & 255;
	if(r_stat >= 192)
	{
		return 0;
	}
	else if(r_stat >= 128)
	{
		return 1;
	}
	else if(r_stat >= 64)
	{
		return 2;
	}
	else
	{
		return 3;
	}
}

int increase_stat(char stat)
{
	return stat + (stat/get_random_stat_percent());
}

unsigned char get_random_stat_percent()
{
	int r_percent;
	r_percent = rand() & 255;
	if(r_percent > 220)
	{
		return 3;
	}
	else if(r_percent > 170)
	{
		return 4;
	}
	else if(r_percent > 100)
	{
		return 5;
	}
	else
	{
		return 255;
	}
}

#include "overworld.c"
#include "battle.c"
