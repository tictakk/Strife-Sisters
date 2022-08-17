#include <huc.h>
#include "paths.h"
#include "strifesisters.h"
#include "item.c"
#include "commander.c"

// #define UNIT_VRAM_START 0x3800
#define PORTRAIT_VRAM 0x4400//0x3C00
#define RECRUIT_VRAM 0x4600
#define DIALOGUE_VRAM 0x4000
#define P_BORDER_VRAM 0x4000
#define MAX_GLOBAL_UNITS 30
#define MAX_PARTY_SIZE 8
// #define NUM_OF_ITEMS 10
#define MAX_INVENTORY 12
// #define UNIT_LIST_SIZE 8
#define UNIT_LIST_SIZE 16
#define MAX_COMMANDERS_PER_BATTLE 6

// #define AGI_STONE 0
// #define STR_STONE 1
// #define	DEF_STONE 2
// #define HP_STONE 3

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

// #incpal(npc_pal, "characters/mannpc.pcx")
// #incspr(man_walk, "characters/mannpc.pcx")
// #incspr(woman_walk, "characters/womannpc.pcx")

#define MAP_WIDTH 32
#define TILE_WIDTH 16
#define TILE_VRAM 0x1000
#define OVERWORLD_MAP_HEIGHT 32
#define NO_OF_CASTLES 16

#define MAX_ARMY_SIZE 15
#define MAX_COMMANDERS 3

#define TAROS 0 //purple
#define KODEN 1 //dark green
#define BAKURIA 2 //yeller
#define SANGAI 3 //blue
#define KAYTOKEN 4 //red

#incspr(cursor, "map/cursor.pcx");
#incpal(cursorpal, "map/cursor.pcx");

enum Unit_Type{
	INFANTRY, FLYERS, SPEARS, MUSKETS, MAGES, HOUNDS, COMMANDER, BLOBS, BANDITS
};

typedef struct{
	unsigned char atk, def, mov, rng, ign, spd, exp, id;
	int hp;
	enum Unit_Type unit_type;
} Unit;

// typedef struct{
//   Unit *unit;
//   char army_size, stars;
// 	unsigned int exp;
// 	Stats stat;
// } GEntity;

struct Commander{
	int exp;
	char lvl;
	char id;
	char no_of_items;
	char no_of_units;
	char ap;
	// char hp;
	char equipable;
	char *name;
	char units[6];
	Unit unit;
	// Stats stat;
};

struct Castle{
	char buyable_units[MAX_COMMANDERS];
	int x, y, map_no;
	char no_of_commanders, id, no_of_items, no_of_units;
	char commanders[MAX_COMMANDERS];
	char buyable_items[MAX_COMMANDERS];
};

//Map images
#incbin(overworldmap,"tiles/strifesisters_overworld.strifersisters.layer-Layer 1.map001.stm")
#inctilepal(overworldtilespal,"tiles/strifesisters_overworld.tiles.pcx")
#incpal(overworldpal,"tiles/strifesisters_overworld.tiles.pcx")
#inctile(overworldtiles,"tiles/strifesisters_overworld.tiles.pcx")

#incbin(battlefieldbat, "tiles/battlefield1.Untitled 3.layer-Layer 1.map001.stm")
#incbin(m1, "map/backgrounds/map1.map1.layer-Layer 1.map001.stm")
#incbin(m2, "map/backgrounds/town1.castlbattle.layer-Layer 1.map001.stm")
#incbin(m3, "map/backgrounds/map3.map1.layer-Layer 1.map001.stm")
#incbin(m4, "map/backgrounds/map4.map1.layer-Layer 1.map001.stm")
#incbin(m5, "map/backgrounds/map5.map1.layer-Layer 1.map001.stm")

#incspr(selector,"map/sprites/selector.pcx");
#incpal(selectorpal,"map/sprites/selector.pcx");

// #incbin(script, "script/script.txt")
// #incchr(build_screen, "map/sprites/build_screen.pcx")
// #incpal(build_pal, "map/sprites/build_screen.pcx")
// #incbat(build_bat, "map/sprites/build_screen.pcx",0x1000,32,28)

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
char untraversable_terrain[15];

// Item items[NUM_OF_ITEMS];
// char global_units[MAX_GLOBAL_UNITS];
char troops[12];
char party[MAX_PARTY_SIZE];
char party_items[MAX_INVENTORY];
char party_size;
char no_of_troops;
int screen_dimensions = 0;

//spawns
char num_of_spawns = 0;
int script_pointer = 0;
int spawn_positions[6];
// char enemy_spawns[6];
// char spawn_commanders[6];
// char spawn_group_size[6];

//merge_units
char merge_units[2];
char merge_pointer;

/*
	MENU STUFF
*/

#define CONSUMABLE 0
#define EQUIPABLE 1
#define ANY_ITEM 2

int cursor_vram = 0;
int current_global_units = 0;

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

#incspr(bandit, "characters/banditbattle.pcx")
#incpal(banditpal, "characters/banditbattle.pcx",0,2)

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

#incspr(sld,"map/sprites/sldpiece.pcx")
#incspr(spr,"map/sprites/sprpiece.pcx")
#incspr(dmn,"map/sprites/demonpiece.pcx")
#incspr(hnd,"map/sprites/houndpiece.pcx")
#incspr(arch0,"map/sprites/archpiece0.pcx")
#incspr(blob,"map/sprites/blob.pcx")
#incspr(bnd,"characters/bandit.pcx")

#incpal(sldpal, "map/sprites/sldpiece.pcx",0,3)
#incpal(cmdrpal, "map/sprites/sprpiece.pcx",2,3)
#incpal(sprpal, "map/sprites/sprpiece.pcx",0,3)
#incpal(mskpal,"map/sprites/msktpiece.pcx",0,2)
#incpal(dmnpal,"map/sprites/demonpiece.pcx",0,2)
#incpal(blobpal,"map/sprites/blob.pcx",0,2)

const char area_one[] = {
	7, 6, SWORD_UNIT, 17,
	1, 3, SWORD_UNIT, 17,
	3, 0, SPEAR_UNIT, 17,
	9, 3, ARCHER_UNIT, 17,
	12, 5, SPEAR_UNIT, 17,
	11, 11, ARCHER_UNIT, 17,
	255
};

const int valid_spawn_points[] = { 421, 466, 355, 359, 329, 425 };

int army_one_x_start = 0;
int army_one_y_start = 0;
int army_two_x_start = 0;
int army_two_y_start = 0;
int avg = 0;

int menu_size = 0;
unsigned int player_gold = 0;
Unit unit_list[UNIT_LIST_SIZE+MAX_COMMANDERS_PER_BATTLE];
// Unit unit_list[16+25];//12 units, 4 ncpcs, 25 commanders

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

void main()
{
	selector_x = 0;
	selector_y = 64;
	cursor_vram = 0x68C0;
	init_map_data();
	untraversable_terrain[0] = 89;
	untraversable_terrain[1] = 90;
	untraversable_terrain[3] = 91;
	untraversable_terrain[4] = 92;
	untraversable_terrain[5] = 93;
	untraversable_terrain[6] = 98;
	num_of_bad_terrains = 7;

	commanders[8].unit.id = CMDR_FOUR;
	commanders[3].unit.id = CMDR_FIVE;
	commanders[4].unit.id = CMDR_SIX;

	castles[1].commanders[0] = 8;
	castles[1].commanders[1] = 3;
	castles[1].commanders[2] = 4;
	castles[1].no_of_commanders = 3;
	castles[1].map_no = 5;

	castles[2].commanders[0] = 5;
	castles[2].commanders[1] = 6;
	castles[2].commanders[2] = 7;
	castles[2].no_of_commanders = 3;
	castles[2].map_no = 2;
	castles[0].no_of_commanders = 0;

	unit_list[SPEAR_UNIT].hp  = 100;
	unit_list[SPEAR_UNIT].atk = 16;
	unit_list[SPEAR_UNIT].def = 13;
	unit_list[SPEAR_UNIT].rng = 2;
	unit_list[SPEAR_UNIT].mov = 3;
	unit_list[SPEAR_UNIT].ign = 0;
	unit_list[SPEAR_UNIT].spd = 10;
	unit_list[SPEAR_UNIT].exp = 25;
	//unit_list[SPEAR_UNIT].address = 0x3900;
	unit_list[SPEAR_UNIT].unit_type = SPEARS;
	unit_list[SPEAR_UNIT].id = SPEAR_UNIT;


	unit_list[DEMON_UNIT].hp  = 70;
	unit_list[DEMON_UNIT].atk = 16;
	unit_list[DEMON_UNIT].def = 12;
	unit_list[DEMON_UNIT].rng = 1;
	unit_list[DEMON_UNIT].mov = 4;
	unit_list[DEMON_UNIT].ign = 1;
	unit_list[DEMON_UNIT].spd = 15;
	unit_list[DEMON_UNIT].exp = 25;
	//unit_list[DEMON_UNIT].address = 0x3B00;
	unit_list[DEMON_UNIT].unit_type = FLYERS;
	unit_list[DEMON_UNIT].id = DEMON_UNIT;

	unit_list[SWORD_UNIT].hp  = 90;
	unit_list[SWORD_UNIT].atk = 14;
	unit_list[SWORD_UNIT].def = 14;
	unit_list[SWORD_UNIT].mov = 3;
	unit_list[SWORD_UNIT].rng = 1;
	unit_list[SWORD_UNIT].ign = 0;
	unit_list[SWORD_UNIT].spd = 12;
	unit_list[SWORD_UNIT].exp = 25;
	//unit_list[SWORD_UNIT].address = 0x3800;
	unit_list[SWORD_UNIT].unit_type = INFANTRY;
	unit_list[SWORD_UNIT].id = SWORD_UNIT;

	unit_list[ARCHER_UNIT].hp  = 70;
	unit_list[ARCHER_UNIT].atk = 17;
	unit_list[ARCHER_UNIT].def = 10;
	unit_list[ARCHER_UNIT].rng = 2;
	unit_list[ARCHER_UNIT].mov = 3;
	unit_list[ARCHER_UNIT].ign = 0;
	unit_list[ARCHER_UNIT].spd = 8;
	unit_list[ARCHER_UNIT].exp = 25;
	//unit_list[ARCHER_UNIT].address = 0x3A00;
	unit_list[ARCHER_UNIT].unit_type = MUSKETS;
	unit_list[ARCHER_UNIT].id = ARCHER_UNIT;

	unit_list[MAGE_UNIT].atk = 20;
	unit_list[MAGE_UNIT].def = 10;
	unit_list[MAGE_UNIT].hp  = 60;
	unit_list[MAGE_UNIT].ign = 0;
	unit_list[MAGE_UNIT].spd = 5;
	unit_list[MAGE_UNIT].mov = 3;
	unit_list[MAGE_UNIT].exp = 25;
	unit_list[MAGE_UNIT].unit_type = MAGES;
	unit_list[MAGE_UNIT].id = MAGE_UNIT;

	unit_list[HOUND_UNIT].atk = 15;
	unit_list[HOUND_UNIT].def = 27;
	unit_list[HOUND_UNIT].hp  = 60;
	unit_list[HOUND_UNIT].ign = 0;
	unit_list[HOUND_UNIT].mov = 4;
	unit_list[HOUND_UNIT].spd = 11;
	unit_list[HOUND_UNIT].exp = 25;
	//unit_list[HOUND_UNIT].address = 0x3C00;
	unit_list[HOUND_UNIT].unit_type = HOUNDS;
	unit_list[HOUND_UNIT].id = HOUND_UNIT;

	unit_list[BLOB_UNIT].atk = 16;
	unit_list[BLOB_UNIT].def = 14;
	unit_list[BLOB_UNIT].hp  = 65;
	unit_list[BLOB_UNIT].ign = 0;
	unit_list[BLOB_UNIT].mov = 3;
	unit_list[BLOB_UNIT].spd = 16;
	unit_list[BLOB_UNIT].exp = 25;
	//unit_list[BLOB_UNIT].address = 0x3D00;
	unit_list[BLOB_UNIT].unit_type = BLOBS;
	unit_list[BLOB_UNIT].id = BLOB_UNIT;

	unit_list[BANDIT_UNIT].atk = 17;
	unit_list[BANDIT_UNIT].def = 17;
	unit_list[BANDIT_UNIT].hp  = 80;
	unit_list[BANDIT_UNIT].ign = 0;
	unit_list[BANDIT_UNIT].mov = 3;
	unit_list[BANDIT_UNIT].spd = 13;
	unit_list[BANDIT_UNIT].exp = 25;
	//unit_list[BANDIT_UNIT].address = 0x3D00;
	unit_list[BANDIT_UNIT].unit_type = BANDITS;
	unit_list[BANDIT_UNIT].id = BANDIT_UNIT;

	initialize_commanders();
	initialize_items();

	commanders[0].name = "Rei";
	commanders[0].no_of_items = 1;
	commanders[0].equipable = 1;
	commanders[1].name = "Violet";
	// commanders[1].no_of_items = 0;
	commanders[2].name = "King";
	commanders[3].name = "Alucard";
	commanders[4].name = "Stimpy";
	commanders[5].name = "Sacha";
	commanders[6].name = "Almer"; //Olimar :)
	commanders[7].name = "Ven";
	commanders[8].name = "Phalc";
	commanders[9].name = "Keely";
	commanders[23].name = "A.Hound";
	commanders[24].name = "Bandit";

	commanders[1].units[0] = SWORD_UNIT;
	commanders[1].units[1] = SWORD_UNIT;
	commanders[1].no_of_units = 2;

	commanders[0].unit.hp = 90*10;
	commanders[0].unit.atk = 7;
	commanders[0].unit.def = 10;
	commanders[0].unit.spd = 15;
	commanders[0].unit.mov = 3;
	commanders[0].unit.rng = 3;
	commanders[0].unit.ign = 0;
	//commanders[0].unit.address = 0x3E00;//0x6A00;
	commanders[0].unit.unit_type = COMMANDER;
	commanders[0].unit.id = REI;

	commanders[1].unit.hp = 90*10;
	commanders[1].unit.atk = 11;
	commanders[1].unit.def = 11;
	commanders[1].unit.spd = 13;
	commanders[1].unit.mov = 3;
	commanders[1].unit.rng = 3;
	commanders[1].unit.ign = 0;
	commanders[1].unit.unit_type = COMMANDER;
	commanders[1].unit.id = VIOLET;

	commanders[0].units[0] = SWORD_UNIT;
	commanders[0].units[1] = SWORD_UNIT;
	commanders[0].units[2] = ARCHER_UNIT;
	commanders[0].units[3] = ARCHER_UNIT;
	commanders[0].units[4] = SPEAR_UNIT;
	commanders[0].units[5] = DEMON_UNIT;
	commanders[0].no_of_units = 4;

	commanders[1].units[0] = SWORD_UNIT;
	commanders[1].units[1] = SWORD_UNIT;
	commanders[1].no_of_units = 2;

	commanders[2].unit.hp = 50*10;
	commanders[2].unit.atk = 8;
	commanders[2].unit.def = 8;
	commanders[2].unit.spd = 13;
	commanders[2].unit.mov = 3;
	commanders[2].unit.rng = 3;
	commanders[2].unit.ign = 0;
	commanders[2].unit.exp = 16;
	commanders[2].unit.unit_type = COMMANDER;
	commanders[2].unit.id = KING;

	commanders[2].units[0] = ARCHER_UNIT;
	commanders[2].units[1] = ARCHER_UNIT;
	commanders[2].units[2] = ARCHER_UNIT;
	commanders[2].no_of_units = 3;

	commanders[3].unit.hp = 60*10;
	commanders[3].unit.atk = 9;
	commanders[3].unit.def = 7;
	commanders[3].unit.spd = 12;
	commanders[3].unit.mov = 3;
	commanders[3].unit.rng = 3;
	commanders[3].unit.ign = 0;
	commanders[3].unit.exp = 0;
	commanders[3].unit.unit_type = COMMANDER;
	commanders[3].unit.id = CMDR_SIX;

	commanders[3].units[0] = SPEAR_UNIT;
	commanders[3].units[1] = SPEAR_UNIT;
	commanders[3].no_of_units = 2;

	commanders[4].unit.hp = 60*10;
	commanders[4].unit.atk = 19;
	commanders[4].unit.def = 20;
	commanders[4].unit.spd = 15;
	commanders[4].unit.mov = 3;
	commanders[4].unit.rng = 3;
	commanders[4].unit.ign = 0;
	commanders[4].unit.exp = 0;
	commanders[4].unit.unit_type = COMMANDER;
	commanders[4].unit.id = CMDR_FOUR;

	commanders[4].units[0] = SPEAR_UNIT;
	commanders[4].units[1] = SPEAR_UNIT;
	commanders[4].units[2] = ARCHER_UNIT;
	commanders[4].units[3] = ARCHER_UNIT;
	commanders[4].no_of_units = 4;

	commanders[5].unit.hp = 850;
	commanders[5].unit.atk = 16;
	commanders[5].unit.def = 21;
	commanders[5].unit.spd = 25;
	commanders[5].unit.mov = 3;
	commanders[5].unit.rng = 3;
	commanders[5].unit.ign = 0;
	commanders[5].unit.exp = 0;
	commanders[5].unit.unit_type = COMMANDER;
	commanders[5].unit.id = CMDR_FIVE;

	commanders[5].units[0] = SWORD_UNIT;
	commanders[5].units[1] = SWORD_UNIT;
	commanders[5].units[2] = ARCHER_UNIT;
	commanders[5].units[3] = ARCHER_UNIT;
	commanders[5].no_of_units = 4;

	commanders[6].unit.hp = 950;
	commanders[6].unit.atk = 12;
	commanders[6].unit.def = 12;
	commanders[6].unit.spd = 15;
	commanders[6].unit.mov = 3;
	commanders[6].unit.rng = 3;
	commanders[6].unit.ign = 0;
	commanders[6].unit.exp = 0;
	commanders[6].unit.unit_type = COMMANDER;
	commanders[6].unit.id = CMDR_SIX;

	commanders[6].units[0] = SPEAR_UNIT;
	commanders[6].units[1] = SPEAR_UNIT;
	commanders[6].units[2] = SPEAR_UNIT;
	commanders[6].units[3] = SPEAR_UNIT;
	commanders[6].no_of_units = 4;

	commanders[7].unit.hp = 750;
	commanders[7].unit.atk = 14;
	commanders[7].unit.def = 11;
	commanders[7].unit.spd = 15;
	commanders[7].unit.mov = 3;
	commanders[7].unit.rng = 3;
	commanders[7].unit.ign = 0;
	commanders[7].unit.exp = 0;
	commanders[7].unit.unit_type = COMMANDER;
	commanders[7].unit.id = 7;

	commanders[7].units[0] = ARCHER_UNIT;
	commanders[7].units[1] = ARCHER_UNIT;
	commanders[7].units[2] = ARCHER_UNIT;
	commanders[7].units[3] = ARCHER_UNIT;
	commanders[7].no_of_units = 4;

	commanders[8].unit.hp = 750;
	commanders[8].unit.atk = 14;
	commanders[8].unit.def = 11;
	commanders[8].unit.spd = 15;
	commanders[8].unit.mov = 3;
	commanders[8].unit.rng = 3;
	commanders[8].unit.ign = 0;
	commanders[8].unit.exp = 0;
	commanders[8].unit.unit_type = COMMANDER;
	commanders[8].unit.id = CMDR_FIVE;

	commanders[8].units[0] = SPEAR_UNIT;
	commanders[8].units[1] = SPEAR_UNIT;
	commanders[8].units[2] = SPEAR_UNIT;
	commanders[8].units[3] = SPEAR_UNIT;
	commanders[8].no_of_units = 3;

	commanders[24].ap = 5;
	commanders[24].unit.id = 24;
	commanders[24].unit.hp = 90*10;
	commanders[24].unit.atk = 8;
	commanders[24].unit.def = 11;
	commanders[24].unit.spd = 13;
	commanders[24].unit.mov = 3;
	commanders[24].unit.rng = 1;
	commanders[24].unit.ign = 0;
	commanders[24].unit.exp = 0;
	commanders[24].unit.unit_type = COMMANDER;
	commanders[24].units[0] = SWORD_UNIT;
	commanders[24].units[1] = SWORD_UNIT;
	commanders[24].units[2] = SWORD_UNIT;
	commanders[24].units[3] = SWORD_UNIT;
	commanders[24].units[4] = SWORD_UNIT;
	commanders[24].units[5] = SWORD_UNIT;
	commanders[24].no_of_units = 0;

	commanders[23].ap = 5;
	commanders[23].unit.id = 23;
	commanders[23].unit.hp = 80*10;
	commanders[23].unit.atk = 9;
	commanders[23].unit.def = 12;
	commanders[23].unit.spd = 13;
	commanders[23].unit.mov = 3;
	commanders[23].unit.rng = 1;
	commanders[23].unit.ign = 0;
	commanders[23].unit.exp = 0;
	commanders[23].unit.unit_type = COMMANDER;
	commanders[23].units[0] = HOUND_UNIT;
	commanders[23].units[1] = HOUND_UNIT;
	commanders[23].units[2] = HOUND_UNIT;
	commanders[23].units[3] = HOUND_UNIT;
	commanders[23].units[4] = HOUND_UNIT;
	commanders[23].units[5] = HOUND_UNIT;
	commanders[23].no_of_units = 0;
	// add_unit_to_pool(1,15,1,SPEAR_UNIT,0,0,0,0);

	troops[0] = SPEAR_UNIT;
	troops[1] = SWORD_UNIT;
	troops[2] = ARCHER_UNIT;
	troops[3] = SWORD_UNIT;
	troops[4] = ARCHER_UNIT;
	troops[5] = SWORD_UNIT;
	troops[6] = SWORD_UNIT;

	no_of_troops = 7;

	items[0].name = "Dagger";
	items[0].type = CONSUMABLE;
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

	castles[8].buyable_items[0] = 3;
	castles[8].buyable_items[1] = 1;
	castles[8].buyable_items[2] = 4;

	castles[8].no_of_units = 2;
	castles[8].buyable_units[0] = SPEAR_UNIT;
	castles[8].buyable_units[1] = SWORD_UNIT;

	player_gold = 10000;

	party[0] = 2;
	party[1] = 0;
	party[2] = 1;
	party_size = 3;

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
	// load_background(build_screen,build_pal,build_bat,32,28);
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

int range(char min, char max)
{
	return (rand()%(max-min+1)) + min;
}

char random_enemy_id(char na)//will eventually be hound types, blobs and bandits
{
	// return HOUND_UNIT;
	return 23;//hound cmdr
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
		put_string("SPR",x,y);
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

		case COMMANDER:
		put_string("CMD",x,y);
		break;

		case BLOBS:
		put_string("BLB",x,y);
		break;

		case BANDITS:
		put_string("BND",x,y);
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
	while(!(joytrg(0) & JOY_I)){}
	vsync();
}

initialize_commanders()
{
	char i;
	struct Commander *cmdr;
	for(i=0, cmdr = commanders; i<TOTAL_COMMANDERS; i++, cmdr++)
	{
		cmdr->ap = 5;
		cmdr->lvl = 0;
		cmdr->id = i;
		cmdr->no_of_items = 0;
		cmdr->unit.exp = 50;
		cmdr->exp = 0;
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
	// spr_make(10+spawn_offset,spawn_x,spawn_y,0x3800+(spawn_offset*0x200),FLIP_MAS|SIZE_MAS,SZ_16x32,18+spawn_offset,1);
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
		case 0:
		load_vram(PORTRAIT_VRAM,rei_gfx,0x100);
		load_palette(12,rei_pal,1);
		break;

		case 1:
		load_vram(PORTRAIT_VRAM,violet_gfx,0x100);
		load_palette(12,violet_pal,1);
		break;

		case 2:
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

void do_story(int x, int y, char *str)
{
	if(str[0] == 0)
	{
		display_info_panel(x,y,32,6);
		load_portrait(str[1]);
		display_item(str[1],0,x+1,y+1);
		return write_text((s_x/8)+6,(s_y/8)+1,str+2) + 3;
	}
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

void change_background_palette(int tile, int pal, int map_offset)
{
	int vaddr, x, y;
	int ptr[2];
	x = (tile%16)*2;
	y = ((tile/16)*2);
	if(y < 6){ return; }
	vaddr = vram_addr(x,y);

	ptr[0] = (pal + 0x100) + ((battlefieldbat[map_offset+tile-32] * 4));
	ptr[1] = (pal + 0x100) + ((battlefieldbat[map_offset+tile-32] * 4) + 1);
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
	// int i;
	// for(i=index; i<num_of_spawns; i++)
	// {
	// 	enemy_spawns[i] = enemy_spawns[i+1];
	// 	spawn_positions[i] = spawn_positions[i+1];
	// }
	// num_of_spawns--;
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
	// for(i=0; i<num_of_spawns; i++)
	// {
	// 	if(enemy_spawns[i] > unit_index)
	// 	{
	// 		--enemy_spawns[i];
	// 	}
	// }
	// if(merge_units[0] > unit_index){ --merge_units[0]; }
	if(merge_units[1] > unit_index){ --merge_units[1]; }

	for(i=unit_index; i<current_global_units-1; i++)
	{
		// memcpy(&global_units[i],&global_units[i+1],sizeof(Unit));
		// global_units[i] = global_units[i+1];
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

void load_palette_groups()
{
	// load_palette(18,sprpal,2);
	// load_palette(20,dmnpal,2);
	// load_palette(22,blobpal,2);
	// load_palette(24,rei_walk_pal,1);
	// load_palette(25,cat_walk_pal,1);
}

void load_commanders_gfx(int cmdr_id, int address, int pal)
{
	// put_number(cmdr_id,3,10,5);
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

		default:
		load_vram(address,dmn,0x100);
		load_palette(pal,sprpal,1);
		// load_vram(address,sld0,0x40);
		// load_vram(address+0x80,sld0+0x40,0x40);
		// load_vram(address+0x40,sld2,0x40);
		// load_vram(address+0xC0,sld2+0x40,0x40);
		// load_palette(pal,cmdrpal,1);
		break;
	}
}

void load_units(int l)
{
}

// void load_npc_units(int l)
// {
// 	int location;
// 	location = l;
// 	load_vram(0x3800,man_walk,0x200);
// 	location+=0x200;
// 	load_vram(0x3A00,woman_walk,0x200);
// }

char next_level(char level, int exp)
{
	// return (4*(level*level*level))/5;
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

void scale_stat(char cmdr_id, char attribute)
{
	int offset, scale;
	char stat = 0;

	offset = (cmdr_id*4)+attribute;
	scale = STAT_SCALER_TABLE[offset];

	if(scale == 0 || scale > 5)
	{
		return;
	}

	switch(attribute)
	{
		case ATK_ATTRIBUTE:
		stat = commanders[cmdr_id].unit.atk;
		stat += (stat / scale);
		commanders[cmdr_id].unit.atk = stat;
		case DEF_ATTRIBUTE:
		stat = commanders[cmdr_id].unit.def;
		stat += (stat / scale);
		commanders[cmdr_id].unit.def = stat;
		case SPD_ATTRIBUTE:
		stat = commanders[cmdr_id].unit.spd;
		stat += (stat / scale);
		commanders[cmdr_id].unit.spd = stat;
		case HP_ATTRIBUTE:
		stat = commanders[cmdr_id].unit.hp;
		stat += (stat / scale);
		commanders[cmdr_id].unit.hp = stat;
	}
	// return stat + (stat/get_random_stat_percent());
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

void move_sprite(int sprite_no, int x, int y)
{
  spr_set(sprite_no);
  spr_y(x);
  spr_x(y);
}

#include "overworld.c"
#include "battle.c"
