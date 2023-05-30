#include <huc.h>

//squirrel
#include "sound/psglib.c"
#include "snginit.c"
int VSyncCnt;
int TimerCnt;
int MainCnt;
int SubCnt;

char screen_dimensions = 0;
// char debug_number = 0;
// char debug_array[24];

#define PLAYER 1
#define CPU 2

#define NO_TARGET 0
#define SINGLE_HIT 1
#define HEAL 2
#define MULTI_ROW 3
#define MULTI_COL_2 4
#define MULTI_COL_3 5
#define MULTI_ATTACK_AOE 6
#define MULTI_HEAL_AOE 7
#define ALL_OPPONENTS 8
#define ALL_ALLIES 9
#define ALL_UNITS 10

int selector_x, selector_y, s_x, s_y, y_offset, x_offset;

#include "map_dimension.h"
#include "paths.c"
#include "strifesisters.h"
// #include "item.c"
#include "effects.c"
#include "arts.c"
#include "units.c"
#include "terrain.c"
#include "commander.c"
#include "map.c"
#include "calling.c"

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

#define COMMAND_DIALOG 0
#define COMMAND_SOUND 1
#define COMMAND_WALK 2
#define COMMAND_PAN 3
#define COMMAND_FOCUS 4
#define COMMAND_STOP 9

#incchr(healthbar,"map/backgrounds/healthbars.pcx");
#incpal(healthbarpal,"map/backgrounds/healthbars.pcx");

// #incchr(panel_gfx, "map/sprites/deploy_menu.pcx")
// #incpal(panel_pal, "map/sprites/deploy_menu.pcx")

// #incchr(info_gfx, "map/sprites/infobox.pcx")
// #incpal(info_pal, "map/sprites/infobox.pcx")
// #incbat(info_bat, "map/sprites/infobox.pcx", 0x1500, 0, 0, 32, 4)

// #incchr(dialog_gfx, "map/sprites/dialoguebox.pcx")
// #incpal(dialog_pal, "map/sprites/dialoguebox.pcx")
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
#incpal(rei_grey, "map/sprites/reiwalk.pcx")

#incspr(cat_walk, "characters/cat.pcx")
#incpal(cat_walk_pal, "characters/cat.pcx")

#incspr(violet_walk, "characters/violetwalk.pcx")
#incpal(violet_walk_pal, "characters/violetwalk.pcx")

#incchr(king_gfx, "characters/king.pcx")
#incpal(king_pal, "characters/king.pcx")

#incspr(tinker, "characters/tinker.pcx")
#incpal(tinker_pal, "characters/tinker.pcx")

#define MAP_WIDTH 32
#define TILE_WIDTH 16
#define TILE_VRAM 0x1000
#define OVERWORLD_MAP_HEIGHT 32
#define NO_OF_CASTLES 16

#define MAX_COMMANDERS 3
#define MAX_MOVE_RANGE 6

#incspr(selector,"map/sprites/selector.pcx");
#incpal(selectorpal,"map/sprites/selector.pcx");

//font
#incchr(font, "map/sprites/font.pcx")
#incpal(fontpal,"map/sprites/font.pcx")
//menus
// #incchr(borders, "tiles/borders.tiles.pcx")
#incchr(borders, "tiles/borders1.pcx")
#incpal(borderspal, "tiles/borders.tiles.pcx")

#define BR_3_3 0x20EE
#define BR_2_3 0x20E5
#define BR_1_3 0x20EB
#define BR_0_3 0x20F1
#define BM_3_3 0x20ED
#define BM_2_3 0x20E4
#define BM_1_3 0x20EA
#define BM_0_3 0x20F0
#define BL_3_3 0x20EC
#define BL_2_3 0x20E3
#define BL_1_3 0x20E9
#define BL_0_3 0x20EF

#define MAX_PARTY_UNIT_SIZE 16

const int health_full[]  = { BL_3_3, BM_3_3, BM_3_3, BM_3_3, BR_3_3 };
const int health_14_15[] = { BL_3_3, BM_3_3, BM_3_3, BM_3_3, BR_2_3 };
const int health_13_15[] = { BL_3_3, BM_3_3, BM_3_3, BM_3_3, BR_1_3 };
const int health_12_15[] = { BL_3_3, BM_3_3, BM_3_3, BM_3_3, BR_0_3 };
const int health_11_15[] = {BL_3_3, BM_3_3, BM_3_3, BM_2_3, BR_0_3 };
const int health_10_15[] = {BL_3_3, BM_3_3, BM_3_3, BM_1_3, BR_0_3 };
const int health_9_15[] = {BL_3_3, BM_3_3, BM_3_3, BM_0_3, BR_0_3 };
const int health_8_15[] = {BL_3_3, BM_3_3, BM_2_3, BM_0_3, BR_0_3 };
const int health_7_15[] = {BL_3_3, BM_3_3, BM_1_3, BM_0_3, BR_0_3 };
const int health_6_15[] = {BL_3_3, BM_3_3, BM_0_3, BM_0_3, BR_0_3 };
const int health_5_15[] = {BL_3_3, BM_2_3, BM_0_3, BM_0_3, BR_0_3 };
const int health_4_15[] = {BL_3_3, BM_1_3, BM_0_3, BM_0_3, BR_0_3 };
const int health_3_15[] = {BL_3_3, BM_0_3, BM_0_3, BM_0_3, BR_0_3 };
const int health_2_15[] = {BL_2_3, BM_0_3, BM_0_3, BM_0_3, BR_0_3 };
const int health_1_15[] = {BL_1_3, BM_0_3, BM_0_3, BM_0_3, BR_0_3 };
const int health_empty[] = {BL_0_3, BM_0_3, BM_0_3, BM_0_3, BR_0_3 };

const unsigned char selector_frames[5] = {0x00, 0x40, 0x80, 0x40, 0x00};


struct Castle{
    int pos, map_id;
};

char j_1, j_2;
struct Castle castles[NO_OF_CASTLES];
// struct Commander commanders[TOTAL_COMMANDERS];

int total_sprites = 0;
char menu_rows = 0;
char no_of_party_items;
char num_of_bad_terrains;

char party_items[MAX_INVENTORY];
char menu_height;
// int screen_dimensions = 0;

//spawns
char selector_frame = 0;
int ptr[32];
char script_ram[256];

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
	int ownPos, fromPos;
	char checked;
};

enum Direction{
	NORTH, NORTHEAST, EAST, SOUTHEAST, SOUTH, SOUTHWEST, WEST, NORTHWEST
};

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
#incspr(mag,"map/sprites/mage.pcx",2,2)
#incspr(blob,"map/sprites/blob.pcx",2,2)
#incspr(bnd,"characters/bandit.pcx")
#incspr(dark,"map/sprites/dark.pcx")
// #incspr(blob,"map/sprites/froggy.pcx")

#incpal(sldpal, "map/sprites/sldpiece.pcx",0,1)
#incpal(cmdrpal, "map/sprites/sprpiece.pcx",2,3)
// #incpal(sprpal, "map/sprites/sprpiece.pcx",0,3)
#incpal(mskpal,"map/sprites/msktpiece.pcx",0,2)
#incpal(dmnpal,"map/sprites/demonpiece.pcx",0,2)
#incpal(blobpal,"map/sprites/blob.pcx",0,2)
#incpal(magepal,"map/sprites/mage.pcx",0,2)
#incpal(bndpal,"characters/bandit.pcx",0,2)
// #incpal(blobpal,"map/sprites/froggy.pcx",0,2)

int menu_size = 0;
unsigned int player_gold = 0;
// char buyable_items[4];
char party_units[MAX_PARTY_UNIT_SIZE];

int map_counter = 0;
int map_size = 0;
int objective_pos;
int tic = 0;
int sx = 0;
int sy = 0;
int yOffset = 0;
char j1 = 0;
int j2 = 0;
int xOffset = 0;
int total_units = 0;
int n = 0;
char num_of_buyable_items = 0;
char g = 0;
char game_over = 0;
char game_loop = 0;
char cursor_x = 0;
char cursor_y = 0;
int red_crystal_count = 0; //bp -> beast points
int blue_crystal_count = 0; //ap -> art points
int green_crystal_count = 0; //up -> upgrade points
int materials_count = 1000;
char party_units_size = 0;

//SIMULATION / DEBUGGIN
int one_pow_9x9_attacker;
int one_pow_6x6_attacker;
int one_pow_3x3_attacker;

int one_pow_9x9_target;
int one_pow_6x6_target;
int one_pow_3x3_target;

char round_one = 0;
char round_two = 0;
char round_three = 0;

int two_pow_9x9_target;
int two_pow_6x6_target;
int two_pow_3x3_target;

int two_pow_9x9_attacker;
int two_pow_6x6_attacker;
int two_pow_3x3_attacker;

char round_four = 0;
char round_five = 0;
char round_six = 0;

char debug_flag = 0;
char simulation_mode = 0;

void main()
{
  char in;
	//setup (one time)
  psgInit(5);
  psgOn(0);

	sngInit();
	psgDelay(0);
	
	//play music (probably move this to whatever place you want this music to play)
	//right now you have 1 song, as you get more you can psgPlay( X ) where X is song number :)
	// psgPlay(0);

  one_pow_9x9_attacker = 0;
  one_pow_6x6_attacker = 0;
  one_pow_3x3_attacker = 0;

  one_pow_9x9_target = 0;
  one_pow_6x6_target = 0;
  one_pow_3x3_target = 0;

  two_pow_9x9_target = 0;
  two_pow_6x6_target = 0;
  two_pow_3x3_target = 0;

  two_pow_9x9_attacker = 0;
  two_pow_6x6_attacker = 0;
  two_pow_3x3_attacker = 0;
	
	game_loop = 1;

	for(in=0; in<MAX_PARTY_UNIT_SIZE; in++)
	{
		party_units[in] = 0;
	}
  party_units[0] = SPEAR_UNIT;

	initialize_units();
	initialize_commanders();
  init_arts();
  init_stones();
//	initialize_items();

  add_commander_to_party(name0,REI);
  add_commander_to_party(name1,VIOLET);

  // party_commanders[0].bg.calling_stone = CALLING_MOVE_BUFF;
  party_commanders[0].bg.calling_stone = CALLING_DOUBLE_ATTACK;
	party_commanders[0].bg.units[4].unit = &unit_list[CLERIC_UNIT];
	party_commanders[0].bg.units[4].hp = unit_list[CLERIC_UNIT].hp;
  party_commanders[0].bg.units[0].unit = &unit_list[SWORD_UNIT];
	party_commanders[0].bg.units[0].hp = unit_list[SWORD_UNIT].hp;
  party_commanders[0].bg.units[1].unit = &unit_list[SWORD_UNIT];
	party_commanders[0].bg.units[1].hp = unit_list[SWORD_UNIT].hp;
  party_commanders[0].bg.units[2].unit = &unit_list[SWORD_UNIT];
  party_commanders[0].bg.units[2].hp = unit_list[SWORD_UNIT].hp;
  // party_commanders[0].bg.art = POWER_WAVE_ART;

  party_commanders[1].bg.units[0].unit = &unit_list[SWORD_UNIT];
	party_commanders[1].bg.units[0].hp = unit_list[SWORD_UNIT].hp;
  party_commanders[1].bg.units[2].unit = &unit_list[SWORD_UNIT];
	party_commanders[1].bg.units[2].hp = unit_list[SWORD_UNIT].hp;
  party_commanders[1].bg.units[3].unit = &unit_list[ARCHER_UNIT];
	party_commanders[1].bg.units[3].hp = unit_list[ARCHER_UNIT].hp;
  party_commanders[1].bg.units[5].unit = &unit_list[ARCHER_UNIT];
	party_commanders[1].bg.units[5].hp = unit_list[ARCHER_UNIT].hp;

	player_gold = 2000;
	no_of_party_items = 0;

	disp_off();
	init_satb();

  load_vram(0x4DB0,cursor,0x10);
	// load_vram(0x68C0+0x40,vert_pointer,0x100);

	while(game_loop)
	{
    // simulate_battle(SWORD_UNIT,FIGHTER_UNIT);
		display_intro();
		overworld_loop();
    // game_result();
		// battle_loop(0,23,1);
		// battle_loop(24,0,1);
    disp_off();
    display_outro();
	}
  // disp_off();
	// for(;;)
	// {
	// display_outro();
	// }
}

void heal_commander_bg(char id)
{
  char i;
  for(i=0; i<9; i++)
  {
    if(party_commanders[id].bg.units[i].unit->id)
    {
      party_commanders[id].bg.units[i].hp = unit_list[party_commanders[id].bg.units[i].unit->id].hp;
    }
  }
}

void display_intro()
{
	// load_background(build_screen,build_pal,build_bat,32,28);
	cls();
	disp_on();
	put_string("Strife Sisters v0.8.0",6,13);
	// wait_for_I_input();
	// sync(60*10);

	// load_background(explain_menu,explain_pal,explain_bat,32,28);
	// while(!(joytrg(0) & JOY_RUN)){}
	disp_off();
}

void display_outro()
{
  reset_satb();
  satb_update();
	cls();
	scroll(0,0,0,224,0xC0);
	disp_on();
	put_string("Thanks for playing!",7,14);
  for(;;)
  {
    sync(1);
    put_string("Thanks for playing!",7,14);
  }
	// wait_for_I_input();
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

int range(int min, int max)
{
	return (rand()%(max-min+1)) + min;
}

void print_unit_type(char id, int x, int y)
{
	switch(id)
	{
		case SWORD_UNIT:
      put_string("SWD",x,y);
      break;

		case DEMON_UNIT:
      put_string("DMN",x,y);
      break;

		case SPEAR_UNIT:
      put_string("SPR",x,y);
      break;

    case STALKER_UNIT:
      put_string("STK",x,y);
      break;

		case ARCHER_UNIT:
      put_string("ARC",x,y);
      break;

    case SNIPER_UNIT:
      put_string("SNP",x,y);
      break;

    case CLERIC_UNIT:
      put_string("CLR",x,y);
      break;

		case MAGE_UNIT:
      put_string("MAG",x,y);
      break;

    case WITCH_UNIT:
      put_string("WIT",x,y);
      break;

    case PRIEST_UNIT:
      put_string("PRE",x,y);
      break;

    case BLACK_MAGE_UNIT:
      put_string("BLK",x,y);
      break;

		case HOUND_UNIT:
      put_string("HND",x,y);
      break;

		case BLOB_UNIT:
      put_string("BLB",x,y);
      break;

		case AXE_UNIT:
      put_string("AXE",x,y);
      break;

    case RAIDER_UNIT:
      put_string("RDR",x,y);
      break;

    case KNIGHT_UNIT:
      put_string("KNT",x,y);
      break;

    case PALADIN_UNIT:
      put_string("PAL",x,y);
      break;

    case MONK_UNIT:
      put_string("MNK",x,y);
      break;

    case FIGHTER_UNIT:
      put_string("FTR",x,y);
      break;

    case BRAWLER_UNIT:
      put_string("BRW",x,y);
      break;

    case LANCER_UNIT:
      put_string("LAN",x,y);
      break;

    case BERSERKER_UNIT:
      put_string("BRK",x,y);
      break;

		default:
      put_string("---",x,y);
//		  put_string("err ",x,y);
//      put_number(id,3,x+4,y);
      // put_string(commanders[id-16].name,x,y);
      break;
	}
}

void print_unit_fullname(char unit_id, int x, int y)
{
  switch(unit_id)
  {
    case SWORD_UNIT:
      put_string("Soldier ",x,y);
      break;

    case DEMON_UNIT:
      put_string("Demon   ",x,y);
      break;

    case SPEAR_UNIT:
      put_string("Spearman",x,y);
      break;

    case STALKER_UNIT:
      put_string("Stalker ",x,y);
      break;

    case ARCHER_UNIT:
      put_string("Archer  ",x,y);
      break;

    case SNIPER_UNIT:
      put_string("Sniper  ",x,y);
      break;

    case CLERIC_UNIT:
      put_string("Cleric  ",x,y);
      break;

    case MAGE_UNIT:
      put_string("Mage    ",x,y);
      break;

    case WITCH_UNIT:
      put_string("Witch   ",x,y);
      break;

    case PRIEST_UNIT:
      put_string("Priest  ",x,y);
      break;

    case BLACK_MAGE_UNIT:
      put_string("B.Mage  ",x,y);
      break;

    case HOUND_UNIT:
      put_string("Helhound",x,y);
      break;

    case BLOB_UNIT:
      put_string("Blob    ",x,y);
      break;

    case AXE_UNIT:
      put_string("Axeman  ",x,y);
      break;

    case KNIGHT_UNIT:
      put_string("Knight  ",x,y);
      break;

    case PALADIN_UNIT:
      put_string("Paladin ",x,y);
      break;

    case MONK_UNIT:
      put_string("Monk    ",x,y);
      break;

    case FIGHTER_UNIT:
      put_string("Fighter ",x,y);
      break;

    case BRAWLER_UNIT:
      put_string("Brawler ",x,y);
      break;

    case LANCER_UNIT:
      put_string("Lancer  ",x,y);
      break;

    case BERSERKER_UNIT:
      put_string("Berserkr",x,y);
      break;

    case RAIDER_UNIT:
      put_string("Raider  ",x,y);
      break;

    default:
      put_string("        ",x,y);
      break;
  }
}

void draw_32x32_sprite(char type, int x, int y)
{
  switch(type)
  {
    case SWORD_UNIT:
      load_palette(30,soldierpal,1);
      load_vram(0x6E00,attack,0x100);
      break;

    case SNIPER_UNIT:
      load_palette(30,sniperbtlpal,1);
      load_vram(0x6E00,sniperbtl,0x100);
    break;

    case BERSERKER_UNIT:
      load_palette(30,sniperbtlpal,1);
      load_vram(0x6E00,berserkerbtl,0x100);
    break;

    case KNIGHT_UNIT:
      load_palette(30,spearpal,1);
      load_vram(0x6E00,knightbtl,0x100);
      break;
    
    case PALADIN_UNIT:
      load_palette(30,paladinpal,1);
      load_vram(0x6E00,paladinbtl,0x100);
      break;

    case SPEAR_UNIT:
      load_palette(30,spearpal,1);
      load_vram(0x6E00,attack2,0x100);
      // spr_make(1,x,y,0x6E00,FLIP_MAS|SIZE_MAS,SZ_32x32,30,1);
      break;

    case STALKER_UNIT:
      load_palette(30,stalkerpal,1);
      load_vram(0x6E00,stalkerbtl,0x100);
      // spr_make(1,x,y,0x6E00,FLIP_MAS|SIZE_MAS,SZ_32x32,30,1);
      break;

    case ARCHER_UNIT:
      load_palette(30,musketbtlpal,1);
      load_vram(0x6E00,musketbtl,0x100);
      // spr_make(1,x,y,0x6E00,FLIP_MAS|SIZE_MAS,SZ_32x32,30,1);
      break;

    case AXE_UNIT:
      load_palette(30,axebtlpal,1);
      load_vram(0x6E00,axebtl,0x100);
      // spr_make(1,x,y,0x6E00,FLIP_MAS|SIZE_MAS,SZ_32x32,30,1);
      break;

    case HOUND_UNIT:
      load_palette(30,houndbtlpal,1);
      load_vram(0x6E00,houndbtl,0x100);
      // spr_make(1,x,y,0x6E00,FLIP_MAS|SIZE_MAS,SZ_32x32,30,1);
      break;

    case CLERIC_UNIT:
      load_palette(30,magebtlpal+16,1);
      load_vram(0x6E00,magebtl,0x100);
      // spr_make(1,x,y,0x6E00,FLIP_MAS|SIZE_MAS,SZ_32x32,30,1);
      break;

    case WITCH_UNIT:
      load_palette(30,witchbtlpal,1);
      load_vram(0x6E00,witchbtl,0x100);
      break;

    case PRIEST_UNIT:
      load_palette(30,witchbtlpal,1);
      load_vram(0x6E00,priestbtl,0x100);
      break;

    case MAGE_UNIT:
      load_palette(30,magebtlpal,1);
      load_vram(0x6E00,magebtl,0x100);
      // spr_make(1,x,y,0x6E00,FLIP_MAS|SIZE_MAS,SZ_32x32,30,1);
      break;
    
    case BLACK_MAGE_UNIT:
      load_palette(30,magebtlpal+32,1);
      load_vram(0x6E00,magebtl,0x100);
      // spr_make(1,x,y,0x6E00,FLIP_MAS|SIZE_MAS,SZ_32x32,30,1);
      break;

    case FIGHTER_UNIT:
      load_palette(30,monkbtlpal,1);
      load_vram(0x6E00,fighterbtl,0x100);
      // spr_make(1,x,y,0x6E00,FLIP_MAS|SIZE_MAS,SZ_32x32,30,1);
      break;

    case BRAWLER_UNIT:
      load_palette(30,monkbtlpal,1);
      load_vram(0x6E00,brawlerbtl,0x100);
      // spr_make(1,x,y,0x6E00,FLIP_MAS|SIZE_MAS,SZ_32x32,30,1);
      break;

    case MONK_UNIT:
      load_palette(30,monkbtlpal,1);
      load_vram(0x6E00,monkbtl,0x100);
      // spr_make(1,x,y,0x6E00,FLIP_MAS|SIZE_MAS,SZ_32x32,30,1);
      break;

    case LANCER_UNIT:
      load_palette(30,spearpal,1);
      load_vram(0x6E00,lancerbtl,0x100);
      // spr_make(1,x,y,0x6E00,FLIP_MAS|SIZE_MAS,SZ_32x32,30,1);
      break;

    case RAIDER_UNIT:
      load_palette(30,raiderbtlpal,1);
      load_vram(0x6E00,raiderbtl,0x100);
      // spr_make(1,x,y,0x6E00,FLIP_MAS|SIZE_MAS,SZ_32x32,30,1);
      break;

    // default:
      // spr_hide(1);
  }
  spr_make(1,x,y,0x6E00,FLIP_MAS|SIZE_MAS,SZ_32x32,30,1);
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

		case AXE:
       put_string("?",x,y);
       break;

		default:
       put_string("=",x,y);
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

void curs_down()
{
  put_char(' ',cursor_x,s_y_relative+cursor_y++);
  put_char('{',cursor_x,s_y_relative+cursor_y);
}

void curs_up()
{
  put_char(' ',cursor_x,s_y_relative+cursor_y--);
  put_char('{',cursor_x,s_y_relative+cursor_y);
}

void curs_right(char amt)
{
  put_char(' ',cursor_x,s_y_relative+cursor_y);
  cursor_x += amt;
  put_char('{',cursor_x,s_y_relative+cursor_y);
}

void curs_left(char amt)
{
  put_char(' ',cursor_x,s_y_relative+cursor_y);
  cursor_x -= amt;
  put_char('{',cursor_x,s_y_relative+cursor_y);
}

void remove_cursor()
{
  put_char(' ',cursor_x,cursor_y+s_y_relative);
}

void display_cursor()
{
  put_char('{',cursor_x,cursor_y+s_y_relative);
}

void load_cursor(int x, int y, int cursor_no)
{
  cursor_x = x;
  cursor_y = y;
  display_cursor();
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
	while(!(joytrg(0) & JOY_I)){}
}

void affirmative_question(char *request, char x, char y)
{
  char question, response;

  question = 1;
  response = 0;
  // remove_cursor();

  display_window_rel(22,14,10,14);
  put_string(request,s_x_relative+x+1,s_y_relative+y+1);
  put_string("YES",s_x_relative+x+2,s_y_relative+y+3);
  put_string("NO",s_x_relative+x+6,s_y_relative+y+3);

  load_cursor(x+5,y+3,SLIDER_ONE);
  while(question > 0)
  {
    switch(joytrg(0))
    {
      case JOY_I:
        question = 0;
        break;

      case JOY_II:
        question = 0;
        response = 0;
        break;

      case JOY_RIGHT:
        if(response)
        {
          curs_right(4);
          response = 0;
        }
        break;

      case JOY_LEFT:
        if(!response)
        {
          curs_left(4);
          response = 1;
        }
        break;  
      vsync();
    }
  }
  remove_cursor();
  return response;
}

initialize_commanders()
{
	char i, j;
	struct Commander *cmdr;

	for(i=0, cmdr = party_commanders; i<TOTAL_COMMANDERS; i++, cmdr++)
	{
		// cmdr->meter = 0;
		cmdr->max_bp = 5;
		cmdr->sprite_type = SWORD_UNIT;
		cmdr->name = name20;
		for(j=0; j<9; j++)
		{
			cmdr->bg.units[j].unit = &unit_list[NO_UNIT];
      cmdr->bg.units[j].hp = 0;
    }
    cmdr->bg.calling_stone = 0;
	}
}

void darken_palette(int pal_num)
{
	modify_palette(pal_num,-2);
}

void lighten_palette(int pal_num)
{
	modify_palette(pal_num,2);
}

void fade_screen()
{
  char i, j; 

  for(j=0; j<3; j++)
  {
    for(i=0; i<32; i++)
    {
      darken_palette(i);
    }
  }
}

void flash_screen()
{
  char i, j;

  for(j=0; j<3; j++)
  {
    for(i=0; i<16; i++)
    {
      lighten_palette(i);
    }
  } 
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

void display_item(char cmdr_id, int index, int x, int y)
{
	int bat[16];
	int offset, i;
  
  // offset = ((s_x_relative+x) + (s_y_relative*screen_dimensions)) + (y*screen_dimensions);
  offset = y*screen_dimensions + x;

	for(i=0; i<16; i++)
	{
		bat[i] = (((PORTRAIT_VRAM + (index*0x100))>>4)+(0xC000+(0x1000 * index))) + i;
	}
	load_bat(offset,bat,4,4);
	// load_bat(2304,bat,4,4);
}

void load_portrait(char cmdr_id, char index)
{
	switch(cmdr_id)
	{
		case REI:
		load_vram(PORTRAIT_VRAM+(index*0x100),rei_gfx,0x100);
		load_palette(12+index,rei_pal,1);
		break;

		case VIOLET:
		load_vram(PORTRAIT_VRAM+(index*0x100),violet_gfx,0x100);
		load_palette(12+index,violet_pal,1);
		break;

		case KING:
		load_vram(PORTRAIT_VRAM+(index*0x100),king_gfx,0x100);
		load_palette(12+index,king_pal,1);
		break;

		default:
		load_vram(PORTRAIT_VRAM+(index*0x100),unknown_gfx,0x100);
		load_palette(12+index,unknown_pal,1);
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
	put_string("SP",_sx,_sy+3);

	put_string("Lv",_sx+5,_sy-4);
	put_string("HP",_sx+5,_sy-3);
	put_string("AP",_sx+5,_sy-2);
}

int script_offset_lookup(unsigned char area, unsigned char state, char id)
{
	int offset = 0;
	offset = find_area_offset(area);//find_offset_by_area(area);
	offset = find_kv_offset(state,0,offset);
	return offset-1;
}

void load_script_into_ram(int off)
{
  int i;
  for(i=0; i<256; i++)
  {
    script_ram[i] = (char)script[off+i];
  }
}

void story(unsigned char area, unsigned char state, char id)
{
	int offset = 0, value = 0, command_loop;
  offset = script_offset_lookup(area,state,id);

  command_loop = 1;

  // put_number(offset,6,0,0);
  // wait_for_I_input();

  load_script_into_ram(offset);

  while(command_loop)
  {
    command_loop = do_command();
    offset += command_loop;
    if(command_loop != 0)
    {
      load_script_into_ram(offset);
    }
  }

	show_npcs(5);
}

int do_command()
{
  switch(script_ram[0])
  {
    case COMMAND_DIALOG:
    menu_height = 48;
    scroll(0,s_x,s_y+menu_height,menu_height,224,0xC0);
    scroll(1,0,0,0,menu_height,0x80);
    return do_story(0,0);

    case COMMAND_PAN:
    do_pan();
    return 3;

    case COMMAND_FOCUS:
    do_focus();
    return 3;

    case COMMAND_STOP: return 0;
  }
}

void do_pan()
{
  scroll(1,0,0,0,menu_height,0x80);
  pan_camera_y(((int)script_ram[2]<<8)+((int)script_ram[1]));
}

void do_focus()
{
  int location;
  location = ((int)script_ram[2]<<8)+((int)script_ram[1]);
  do_pan();
  flash_selector((location&15) << 4,((location>>4) << 4)-s_y,5);
}

int do_story(int x, int y)//, char *str)
{
  int text_size;
  display_window_abs(y,x,32,6);
	if(script_ram[1] != 0)
	{
    load_portrait(script_ram[1] ,0);
		display_item(script_ram[1],0,x+1,y+1);
	}
  text_size = write_text(6,1,script_ram+2)+3;
  wait_for_I_input();
  return text_size;
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
	int offset = 0;
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
			// if(script[offset] == 0xFF)
			// {
			// 	return 1;
			// }
			// if(script[offset++] == value)
			// {
				return ++offset;
        // return offset;
			// }
      // return offset++;
		}
	}
	return 1;
}

void display_window_rel(int x, int y, char row_x, char row_y)
{
  int vaddr;
  vaddr = vram_addr(x,y+(s_y_relative));
  display_window(x,y,row_x,row_y,vaddr);
}

void display_window_abs(int x, int y, char row_x, char row_y)
{
  int vaddr;
  vaddr = vram_addr(x,y);
  display_window(x,y,row_x,row_y,vaddr);
}

void display_window(int x, int y, char row_x, char row_y, int vaddr)
{
  switch(row_x)
  {
    case 8:
      window_8(vaddr,row_x,row_y);
      break;

    case 10:
      window_10(vaddr,row_x,row_y);
      break;

    case 12:
      window_12(vaddr,row_x,row_y);
      break;

    case 14:
      window_14(vaddr,row_x,row_y);
      break;

    case 16:
      window_16(vaddr,row_x,row_y);
      break;

    case 18:
      window_18(vaddr,row_x,row_y);
      break;

    case 22:
      window_22(vaddr,row_x,row_y);
      break;

    case 24:
      window_24(vaddr,row_x,row_y);
      break;

    case 32:
      window_32(vaddr,row_x,row_y);
      break;
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
			// z = (j*width) + i;
			ptr[i] = (0x6400 >> 4) + ((i+(width-1)) / (width - ((i+(width-2))/(width-1)))) + ((((j+(length-1)) / (length - ((j+(length-2))/(length-1))))) * 3) + 0x9000;
		}
		vaddr = vram_addr(0,j);
		load_vram(vaddr+offset,ptr,width);
	}
}

void display_abs_black_panel(int x, int y, int width, int length)
{
	int j, i, z, vaddr, offset, size;
	size = length * width;
	offset = y*8+x;
  vaddr = vram_addr(x,y);
  
  vreg(0x00,vaddr);

	for(j=0; j<length; j++)
	{
		for(i=0; i<width; i++)
		{
      vreg(0x02,0xA4BB);
		}
    sync(2);
	}
}

void change_background_palette(int tile, int pal)
{
	int vaddr, x, y;
  int tiledata;

  x = (tile&15)<<1;
	y = ((tile>>4)<<1);
	if(y < 3){ return; }

	vaddr = vram_addr(x,y);

  vreg(0x01,vaddr);
  tiledata = peekw(0x02) & 0xFFF;

  vreg(0x00,vaddr);
  vreg(0x02,tiledata+pal);
  vreg(0x02,tiledata+pal+1);

  vreg(0x00,vaddr+0x20);

  vreg(0x01,vaddr+0x20);
  tiledata = peekw(0x02) & 0xFFF;

  vreg(0x02,tiledata+pal);
  vreg(0x02,tiledata+pal+1);
}

load_healthbars()
{
	load_palette(2,healthbarpal,1);
//	load_vram(0xe00,healthbar,0xC0);
  load_vram(0xe00,healthbar,0x150);
}

void display_healthbar(char x, char y, char percent)
{
  int addr;
  addr = vram_addr(x,y);

  if(percent > 98) { load_vram(addr,health_full,5); return;}
  if(percent > 93) { load_vram(addr,health_14_15,5); return;}
  if(percent > 86) { load_vram(addr,health_13_15,5); return;}
  if(percent > 80) { load_vram(addr,health_12_15,5); return;}
  if(percent > 73) { load_vram(addr,health_11_15,5); return;}
  if(percent > 66) { load_vram(addr,health_10_15,5); return;}
  if(percent > 60) { load_vram(addr,health_9_15,5); return;}
  if(percent > 53) { load_vram(addr,health_8_15,5); return;}
  if(percent > 46) { load_vram(addr,health_7_15,5); return;}
  if(percent > 40) { load_vram(addr,health_6_15,5); return;}
  if(percent > 33) { load_vram(addr,health_5_15,5); return;}
  if(percent > 26) { load_vram(addr,health_4_15,5); return;}
  if(percent > 20) { load_vram(addr,health_3_15,5); return;}
  if(percent > 13) { load_vram(addr,health_2_15,5); return;}
  if(percent > 0) { load_vram(addr,health_1_15,5); return;}
  load_vram(addr,health_empty,5);
}

void display_meter_bars(char bar_count, char x, char y)
{
  switch(bar_count)
  {
    case 0: put_string("   ",x,y);break;
    case 1: 
    put_char('&',x,y);
    put_string("  ",x+1,y);
    break;

    case 2:
    put_char('$',x,y);
    put_string("  ",x+1,y);
    break;

    case 3:
    put_char('$',x,y);
    put_char('&',x+1,y);
    put_string(" ",x+2,y);
    break;

    case 4:
    put_char('$',x,y);
    put_char('$',x+1,y);
    put_string(" ",x+2,y);
    break;

    case 5:
    put_char('$',x,y);
    put_char('$',x+1,y);
    put_char('&',x+2,y);
    break;
  }
}

void simulate_battle(char type_one, char type_two)
{
  simulation_mode = 1;
  s_x_relative = 0;
  s_y_relative = 0;
  load_palette(0,overworldpal,8);
	load_palette(10,fontpal,2);
	load_palette(9,borderspal,1);

  set_font_pal(10);
	load_font(font,125,0x4800);
  disp_on();

  add_entity(PLAYER,0,0,0,&party_commanders[6]);
  add_entity(CPU,0,0,0,&enemy_commanders[0]);

  add_units_to_cmdr(6,type_one,3);
  add_units_to_cmdr(MAX_PARTY_COMMANDERS,type_two,3);
  round_one = run_sim(6,MAX_PARTY_COMMANDERS,&one_pow_3x3_attacker,&two_pow_3x3_target,0,1);
  clear_commander_battle_group(&party_commanders[6]);
  clear_commander_battle_group(&enemy_commanders[0]);

  add_units_to_cmdr(6,type_one,6);
  add_units_to_cmdr(MAX_PARTY_COMMANDERS,type_two,6);
  round_two = run_sim(6,MAX_PARTY_COMMANDERS,&one_pow_6x6_attacker,&two_pow_6x6_target,0,1);
  clear_commander_battle_group(&party_commanders[6]);
  clear_commander_battle_group(&enemy_commanders[0]);

  add_units_to_cmdr(6,type_one,9);
  add_units_to_cmdr(MAX_PARTY_COMMANDERS,type_two,9);
  round_three = run_sim(6,MAX_PARTY_COMMANDERS,&one_pow_9x9_attacker,&two_pow_9x9_target,0,1);
  clear_commander_battle_group(&party_commanders[6]);
  clear_commander_battle_group(&enemy_commanders[0]);

  add_units_to_cmdr(6,type_one,9);
  add_units_to_cmdr(MAX_PARTY_COMMANDERS,type_two,9);
  round_four = run_sim(6,MAX_PARTY_COMMANDERS,&two_pow_3x3_attacker,&one_pow_3x3_target,1,0);
  clear_commander_battle_group(&party_commanders[6]);
  clear_commander_battle_group(&enemy_commanders[0]);

  add_units_to_cmdr(6,type_one,6);
  add_units_to_cmdr(MAX_PARTY_COMMANDERS,type_two,6);
  round_five = run_sim(6,MAX_PARTY_COMMANDERS,&two_pow_6x6_attacker,&one_pow_6x6_target,1,0);
  clear_commander_battle_group(&party_commanders[6]);
  clear_commander_battle_group(&enemy_commanders[0]);

  add_units_to_cmdr(6,type_one,9);
  add_units_to_cmdr(MAX_PARTY_COMMANDERS,type_two,9);
  round_six = run_sim(6,MAX_PARTY_COMMANDERS,&two_pow_9x9_attacker,&one_pow_9x9_target,1,0);
  clear_commander_battle_group(&party_commanders[6]);
  clear_commander_battle_group(&enemy_commanders[0]);

  // while(calculate_power(6) > 0 && calculate_power(MAX_PARTY_COMMANDERS) > 0)
  // {
    // battle_loop(0,1,1,0,0,0,0);
  // }
  display_simulation_results(type_one,type_two);
  for(;;)
  {
    // battle_loop(1,0,1,0,0,0,0);
  }
}

char run_sim(char cmdr_one, char cmdr_two, int *attacker, int *target, char attacking_entity, char targeted_entity)
{
  int starting_pow_one, starting_pow_two;
  char rounds;

  starting_pow_one = calculate_power(cmdr_one);
  starting_pow_two = calculate_power(cmdr_two);
  rounds = 0;
  while((calculate_power(cmdr_one) > 0 || get_commander_battle_points(cmdr_one) ) && (calculate_power(cmdr_two) > 0 || get_commander_battle_points(cmdr_two)))
  {
    rounds++;
    battle_loop(attacking_entity,targeted_entity,1,0,0,0,0);
  }
  *attacker = get_percentage(calculate_power(cmdr_one),starting_pow_one);
  *target = get_percentage(calculate_power(cmdr_two),starting_pow_two);
  return rounds;
}

void display_simulation_results(char type_one, char type_two)
{
  display_window_rel(0,6,32,22);
  put_string("SIMULATION RESULTS",6,7);
  print_unit_fullname(type_one,5,9);
  put_string("vs",14,9);
  print_unit_fullname(type_two,18,9);
  set_font_pal(9);
  put_string("POW",6,10);
  put_string("POW",19,10);
  put_number(unit_list[type_one].pow,3,9,10);
  put_number(unit_list[type_two].pow,3,22,10);
  put_string("rds",28,10);

  set_font_pal(10);

  put_number(one_pow_9x9_attacker,3,7,11);
  put_string("9x9",13,11);
  put_number(two_pow_9x9_target,3,18,11);
  put_number(round_one,3,28,11);

  put_number(one_pow_6x6_attacker,3,7,12);
  put_string("6x6",13,12);
  put_number(two_pow_6x6_target,3,18,12);
  put_number(round_two,3,28,12);

  put_number(one_pow_3x3_attacker,3,7,13);
  put_string("3x3",13,13);
  put_number(two_pow_3x3_target,3,18,13);
  put_number(round_three,3,28,13);

  print_unit_fullname(type_two,5,15);
  put_string("vs",14,15);
  print_unit_fullname(type_one,18,15);

  put_number(one_pow_3x3_target,3,7,17);
  put_string("3x3",13,17);
  put_number(two_pow_3x3_attacker,3,18,17);
  put_number(round_four,3,28,17);

  put_number(one_pow_6x6_target,3,7,18);
  put_string("6x6",13,18);
  put_number(two_pow_6x6_attacker,3,18,18);
  put_number(round_five,3,28,18);

  put_number(one_pow_9x9_target,3,7,19);
  put_string("9x9",13,19);
  put_number(two_pow_9x9_attacker,3,18,19);
  put_number(round_six,3,28,19);
}

void add_units_to_cmdr(char cmdr_id, char unit_type, char count)
{
  char i;
  for(i=0; i<count; i++)
  {
    load_unit_to_cmdr(cmdr_id,i,unit_type);
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

void load_commanders_gfx(int cmdr_id, int address, int pal)
{
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

		case TINKER:
		load_vram(address,tinker,0x100);
		load_palette(pal,tinker_pal,1);
		break;

		default:
		load_vram(address,sld,0x100);
		load_palette(pal,sldpal,1);
		break;
	}
}

void list_commander_army(Battlegroup *bg, char x, char y)
{
  char i, j;
  for(i=0; i<3; i++)
  {
    for(j=0; j<3; j++)
    {
      print_unit_info(&bg->units[(i*3)+j].unit,x+(i*5)+(s_x/8),y+(j*4)+(s_y/8));
    }
  }
}

void list_party_units(char x, char y)
{
  char i;
  for(i=0; i<MAX_PARTY_UNIT_SIZE; i++)
  {
    print_unit_type(party_units[i],x+(4*(i/8)),y+(i%8));
  }
}

void remove_unit_from_convoy(char position)
{
  party_units[position] = NO_UNIT;
}

int get_percentage(int num, int denom)
{
  int hp, hp_p;
  if(denom == 0)
  {
    return 0;
  }
  if(num < 0)
  {
    return 0;
  }

  hp = num * 100;
  hp_p = hp / denom;

  return hp_p;
}

int calc_percentage(int num, int denom)
{
  int p;
  p = num * denom;
  return p / 100;
}

void load_unit_to_cmdr(char cmdr_id, char unit_pos, char unit_type)
{
  party_commanders[cmdr_id].bg.units[unit_pos] = &unit_list[unit_type];
  party_commanders[cmdr_id].bg.units[unit_pos].hp = unit_list[unit_type].hp;
}

void load_predefined_group_layout(char layout_type, char cmdr_id)
{
  switch(layout_type)
  {
    case 23:
      party_commanders[cmdr_id].sprite_type = HOUND_UNIT;
      load_unit_to_cmdr(cmdr_id,1,HOUND_UNIT);
      // load_unit_to_cmdr(cmdr_id,2,HOUND_UNIT);
      break;

    case 24:
      party_commanders[cmdr_id].sprite_type = BLOB_UNIT;
      load_unit_to_cmdr(cmdr_id,0,BLOB_UNIT);
      // load_unit_to_cmdr(cmdr_id,1,BLOB_UNIT);
      load_unit_to_cmdr(cmdr_id,2,BLOB_UNIT);

      // load_unit_to_cmdr(cmdr_id,3,BLOB_UNIT);
      // load_unit_to_cmdr(cmdr_id,4,BLOB_UNIT);
      // load_unit_to_cmdr(cmdr_id,5,BLOB_UNIT);

      // load_unit_to_cmdr(cmdr_id,6,BLOB_UNIT);
      // load_unit_to_cmdr(cmdr_id,7,BLOB_UNIT);
      // load_unit_to_cmdr(cmdr_id,8,BLOB_UNIT);
      break;

    case 25:
      party_commanders[cmdr_id].sprite_type = RAIDER_UNIT;
      load_unit_to_cmdr(cmdr_id,0,RAIDER_UNIT);
      load_unit_to_cmdr(cmdr_id,2,RAIDER_UNIT);
      break;

    case 26:
      party_commanders[cmdr_id].sprite_type = SWORD_UNIT;
      load_unit_to_cmdr(cmdr_id,0,SWORD_UNIT);
      load_unit_to_cmdr(cmdr_id,2,SWORD_UNIT);
      break;

    case 27:
      party_commanders[cmdr_id].sprite_type = ARCHER_UNIT;
      load_unit_to_cmdr(cmdr_id,0,ARCHER_UNIT);
      load_unit_to_cmdr(cmdr_id,2,ARCHER_UNIT);
      break;

    case 28:
      party_commanders[cmdr_id].sprite_type = SPEAR_UNIT;
      load_unit_to_cmdr(cmdr_id,0,SPEAR_UNIT);
      load_unit_to_cmdr(cmdr_id,2,SPEAR_UNIT);
      break;
  }
}

void swap_commander_units(char cmdr_id, char first, char second)
{
  int tmp_hp;
  char tmp_meter;
  Unit *tmp_unit;

  tmp_hp = party_commanders[cmdr_id].bg.units[first].hp;
  tmp_unit = party_commanders[cmdr_id].bg.units[first].unit;

  party_commanders[cmdr_id].bg.units[first].hp = party_commanders[cmdr_id].bg.units[second].hp;
  party_commanders[cmdr_id].bg.units[first].unit = party_commanders[cmdr_id].bg.units[second].unit;

  party_commanders[cmdr_id].bg.units[second].hp = tmp_hp;
  party_commanders[cmdr_id].bg.units[second].unit = tmp_unit;
}

void swap_convoy_units(char cmdr_id, char unit_id, char convoy_id)
{
  char tmp_id;

  tmp_id = party_commanders[cmdr_id].bg.units[unit_id].unit->id;

  party_commanders[cmdr_id].bg.units[unit_id].hp = unit_list[party_units[convoy_id]].hp;
  party_commanders[cmdr_id].bg.units[unit_id].unit = &unit_list[party_units[convoy_id]];

  party_units[convoy_id] = tmp_id;
}

void put_green_square(char x, char y)
{
  int p[3];
  int vaddr;

  vaddr = vram_addr(x+s_x_relative,y+s_y_relative);

  p[0] = 0x4F4;
  p[1] = 0x4F4;
  p[2] = 0x4F4;

  load_vram(vaddr,p,3);
  load_vram(vaddr+64,p,3);
  load_vram(vaddr+128,p,3);
}

void put_highlight_square(char position, char x, char y)
{
  int p[3];
  int vaddr, highlight_addr;

  highlight_addr = (position%2 == 0)? 0x54F4 : 0x44F4;
  vaddr = vram_addr(x+s_x_relative,y+s_y_relative);

  p[0] = highlight_addr;
  p[1] = highlight_addr;
  p[2] = highlight_addr;

  load_vram(vaddr,p,3);
  load_vram(vaddr+64,p,3);
  load_vram(vaddr+128,p,3);
}

void put_blue_square(char x, char y)
{
  int p[3];
  int vaddr;

  vaddr = vram_addr(x+s_x_relative,y+s_y_relative);

  p[0] = 0x94F4;
  p[1] = 0x94F4;
  p[2] = 0x94F4;

  load_vram(vaddr,p,3);
  load_vram(vaddr+64,p,3);
  load_vram(vaddr+128,p,3);
}

int calculate_power(char cmdr_id)
{
  int i, hp_total, hp_max, power, percent;
  power = 0;
  hp_max = 0;
  hp_total = 0;
  percent = 0;
  
	for(i=0; i<9; i++)
  {
    if(party_commanders[cmdr_id].bg.units[i].unit->id)
    {
      power += party_commanders[cmdr_id].bg.units[i].unit->pow;
      hp_max += party_commanders[cmdr_id].bg.units[i].unit->hp;
      hp_total += party_commanders[cmdr_id].bg.units[i].hp;
    }
  }
  //having an issue with multiplying a signed num with the get_percentage fn, so divide by ten I guess?
  percent = get_percentage(hp_total/10,hp_max/10);
  return (power * percent)/100;
}

int roundUp(int num, int div)
{
	int whole, remainder;
	whole = num/div;
  if (div == 0)
      return 0;

  remainder = num % div; //1
  if (remainder == 0)
      return whole;
	else
			return whole+1;
}

void add_cmdr_from_story(char cmdr_id)
{
  switch(cmdr_id)
  {
    case KING:
    add_commander_to_party(name2,KING);
    load_unit_to_cmdr(party_size-1,3,SPEAR_UNIT);
    load_unit_to_cmdr(party_size-1,1,SWORD_UNIT);
    load_unit_to_cmdr(party_size-1,5,SPEAR_UNIT);
    load_unit_to_cmdr(party_size-1,7,CLERIC_UNIT);
    break;
  }
}

void add_unit_to_convoy(char unit_id)
{
  char i;
  for(i=0; i<MAX_PARTY_UNIT_SIZE; i++)
  {
    if(party_units[i] == 0)
    {
      party_units[i] = unit_id;
      party_units_size++;
      return;
    }
  }
}

void display_selector(char spr_num, int x, int y, char pal) //info sprites
{
  spr_make(spr_num,x,y,0x68C0,0,NO_FLIP|SZ_16x16,pal,1);
}

void cycle_selector()
{
  selector_frame = (selector_frame + 1) % 5;
  spr_set(SELECTOR);
  spr_pattern(0x68C0+selector_frames[selector_frame]);
}

void flash_selector(int location_x, int location_y, unsigned char flashes)
{
  char i;
  display_selector(0,location_x,location_y,16);
  for(i=0; i<(flashes*5); i++)
  {
    cycle_selector();    
    satb_update();
    sync(6);
  }
}

void display_number_incrementing(char x, char y, int final_number, char num_len)
{
  int i;
  i=0;
  while((i+=6) < final_number)
  {
    put_number(i,num_len,x,y);
    sync(1);
  }
  put_number(final_number,num_len,x,y);
}

#include "overworld.c"
#include "battle.c"
