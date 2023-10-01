#include <huc.h>

//squirrel
#include "sound/psglib.c"
#include "snginit.c"
#include "farMemcpy.h"
#include "farMemcpy.c"
#include "load_vram_fptr.h"
#include "formations.c"

int VSyncCnt;
int TimerCnt;
int MainCnt;
int SubCnt;

int hits=0;
int misses=0;

typedef struct {
    char idx;
    char bank[4];
    int  addr[4];
} Pointers;
Pointers myPointers;

struct Node{
	int ownPos, fromPos;
	char checked;
};

#asm
GROWTH_DPS = 0
GROWTH_RANGED_DPS = 1
GROWTH_MAGIC_DPS = 2
GROWTH_MAGIC_SUPPORT = 3
GROWTH_TANK_MELEE = 4
GROWTH_RESIST_MELEE = 5
GROWTH_REI = 6
GROWTH_VIOLET = 7
GROWTH_KING = 8
GROWTH_TINKER = 9
GROWTH_SPEED_DPS = 10

    .data
  _nounit:
    .include "game_data/units/no_unit.asm"
;  _soldier:
    .include "game_data/units/soldier.asm"
;  _spearman: 
    .include "game_data/units/spearman.asm"
;   _archer:
    .include "game_data/units/archer.asm"
;   _mage:
    .include "game_data/units/mage.asm"
;   _demon:
    .include "game_data/units/demon.asm"
;   _hound:
    .include "game_data/units/hound.asm"
;   _blob:
    .include "game_data/units/blob.asm"
;  _axeman:
    .include "game_data/units/axeman.asm"
;  _paladin:
    .include "game_data/units/paladin.asm"
;  _monk:
    .include "game_data/units/monk.asm"
;  _sniper:
    .include "game_data/units/sniper.asm"
;  _lancer:
    .include "game_data/units/lancer.asm"
;  _knight:
    .include "game_data/units/knight.asm"
;  _berserker:
    .include "game_data/units/berserker.asm"
;  _stalker:
    .include "game_data/units/stalker.asm"
;  _artillery:
    .include "game_data/units/artillery.asm"
;  _cleric:
    .include "game_data/units/cleric.asm"
;  _priest:
    .include "game_data/units/priest.asm"
;  _black_mage:
    .include "game_data/units/blackmage.asm"
;  _witch:
    .include "game_data/units/witch.asm"
; _raider:
    .include "game_data/units/raider.asm"
; _thief:
    .include "game_data/units/thief.asm"
; _captain:
    .include "game_data/units/captain.asm"
; _fighter:
    .include "game_data/units/fighter.asm"
; _brawler:
    .include "game_data/units/brawler.asm"
;_golem:
    .include "game_data/units/golem.asm"
;_holder1:
    .include "game_data/units/holder1.asm"
;_holder2:
    .include "game_data/units/holder2.asm"
;_rei:
    .include "game_data/units/rei.asm"
;_violet:
    .include "game_data/units/violet.asm"
;_king:
    .include "game_data/units/king.asm"
;_tinker:
    .include "game_data/units/tinker.asm"
;_tearle:
    .include "game_data/units/tearle.asm"
    .code
#endasm

extern const char nounit[];
extern const char soldier[];
// extern const char spearman[];
// extern const char archer[];

char screen_dimensions = 0;
// char debug_number = 0;
// char debug_array[24];

#define FRONT_COLUMN 0
#define MIDDLE_COLUMN 1
#define REAR_COLUMN 2

#define WHITE_FONT 10
#define GOLD_FONT 9
#define HIGHLIGHT_FONT 11
#define OTHER_FONT 12

int selector_x, selector_y, s_x, s_y, y_offset, x_offset;
int g = 0;

struct Node neighbors[4];
struct Node map[65]; //a unit with a max move of 5 will fill 60 nodes max.

#include "map_dimension.h"
#include "paths.c"

#define NO_TARGET 0
#define SINGLE_HIT 1
#define HEAL 2
#define MULTI_ROW 3
// #define MULTI_COL_2 4
#define MULTI_COL_3 5
// #define MULTI_ATTACK_AOE 6
#define HEAL_ALL 7
#define ALL_OPPONENTS 8
#define ALL_ALLIES 9
#define ALL_UNITS 10

#include "strifesisters.h"
#include "effects.c"
#include "arts.c"
#include "units.c"

#incbin(battlemap,"tiles/battletiles.battle_backgrounds.layer-Layer 1.map001.stm")
#inctilepal(battletilespal,"tiles/battletiles.tiles.pcx")
#inctile(battletiles,"tiles/battletiles.tiles.pcx")
#incpal(battlepal,"tiles/battletiles.tiles.pcx")

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

#define COMMAND_DIALOG 0
#define COMMAND_SOUND 1
#define COMMAND_WALK 2
#define COMMAND_PAN 3
#define COMMAND_FOCUS 4
#define COMMAND_STOP 9

// #incchr(healthbar,"map/backgrounds/healthbars.pcx");
// #incpal(healthbarpal,"map/backgrounds/healthbars.pcx");
#incchr(healthbar,"map/sprites/health.pcx")

#incchr(sisters_logo,"map/backgrounds/sister_logo_small.pcx")
#incpal(sisters_logo_pal,"map/backgrounds/sister_logo_small.pcx");
#incbat(sisters_bat,"map/backgrounds/sister_logo_small.pcx",0x1000,0,0,32,16)

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
#incpal(rei_battle_pal,"map/sprites/swordy.pcx",2,1)

#incspr(cat_walk, "characters/cat.pcx")
#incpal(cat_walk_pal, "characters/cat.pcx")

#incspr(violet_walk, "characters/violetwalk.pcx")
#incpal(violet_walk_pal, "characters/violetwalk.pcx")
#incpal(violet_battle_pal,"map/sprites/magebattle.pcx",3,3)

#incchr(king_gfx, "characters/king.pcx")
#incpal(king_pal, "characters/king.pcx")
#incpal(king_battle_pal,"map/sprites/swordy.pcx",3,1)

#incspr(tinker, "characters/tinker.pcx")
#incpal(tinker_pal, "characters/tinker.pcx")

#incspr(calien,"characters/messanger.pcx")
#incpal(calien_pal,"characters/messanger.pcx")

#incspr(tearle,"characters/tearle.pcx")
#incpal(tearle_pal,"characters/tearle.pcx")

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

#define BAR_8_8 0xA0E0
#define BAR_7_8 0xA0E1
#define BAR_6_8 0xA0E2
#define BAR_5_8 0xA0E3
#define BAR_4_8 0xA0E4
#define BAR_3_8 0xA0E5
#define BAR_2_8 0xA0E6
#define BAR_1_8 0xA0E7
#define BAR_0_8 0xA0E8

#define MAX_PARTY_UNIT_SIZE 16

// const char soldier[] = {
  // 50, 20, 10, 10, 10, 15, 3, 100, 1, 1, 1, 0, 1, 5, 0, 24, 7, 0
// };

// #incasmlabel(soldier,"game_data/units/soldier.asm")
// extern const char soldier[];

// extern int unit_first[];

// const char unit_first[] = {
//   #asm .db 50, 20, 10, 10, 10, 15, 3, 100, 1, 1, 1, 0, 1, 5, 0, 24, 7, 0 #endasm
// };

// const int health_full[]  = { BL_3_3, BM_3_3, BM_3_3, BM_3_3, BR_3_3 };
// const int health_14_15[] = { BL_3_3, BM_3_3, BM_3_3, BM_3_3, BR_2_3 };
// const int health_13_15[] = { BL_3_3, BM_3_3, BM_3_3, BM_3_3, BR_1_3 };
// const int health_12_15[] = { BL_3_3, BM_3_3, BM_3_3, BM_3_3, BR_0_3 };
// const int health_11_15[] = {BL_3_3, BM_3_3, BM_3_3, BM_2_3, BR_0_3 };
// const int health_10_15[] = {BL_3_3, BM_3_3, BM_3_3, BM_1_3, BR_0_3 };
// const int health_9_15[] = {BL_3_3, BM_3_3, BM_3_3, BM_0_3, BR_0_3 };
// const int health_8_15[] = {BL_3_3, BM_3_3, BM_2_3, BM_0_3, BR_0_3 };
// const int health_7_15[] = {BL_3_3, BM_3_3, BM_1_3, BM_0_3, BR_0_3 };
// const int health_6_15[] = {BL_3_3, BM_3_3, BM_0_3, BM_0_3, BR_0_3 };
// const int health_5_15[] = {BL_3_3, BM_2_3, BM_0_3, BM_0_3, BR_0_3 };
// const int health_4_15[] = {BL_3_3, BM_1_3, BM_0_3, BM_0_3, BR_0_3 };
// const int health_3_15[] = {BL_3_3, BM_0_3, BM_0_3, BM_0_3, BR_0_3 };
// const int health_2_15[] = {BL_2_3, BM_0_3, BM_0_3, BM_0_3, BR_0_3 };
// const int health_1_15[] = {BL_1_3, BM_0_3, BM_0_3, BM_0_3, BR_0_3 };
// const int health_empty[] = {BL_0_3, BM_0_3, BM_0_3, BM_0_3, BR_0_3 };

const health_full[] = {BAR_8_8, BAR_8_8, BAR_8_8};

const health_23_24[] = {BAR_8_8, BAR_8_8, BAR_7_8};
const health_22_24[] = {BAR_8_8, BAR_8_8, BAR_6_8};
const health_21_24[] = {BAR_8_8, BAR_8_8, BAR_5_8};
const health_20_24[] = {BAR_8_8, BAR_8_8, BAR_4_8};
const health_19_24[] = {BAR_8_8, BAR_8_8, BAR_3_8};
const health_18_24[] = {BAR_8_8, BAR_8_8, BAR_2_8};
const health_17_24[] = {BAR_8_8, BAR_8_8, BAR_1_8};
const health_16_24[] = {BAR_8_8, BAR_8_8, BAR_0_8};

const health_15_24[] = {BAR_8_8, BAR_7_8, BAR_0_8};
const health_14_24[] = {BAR_8_8, BAR_6_8, BAR_0_8};
const health_13_24[] = {BAR_8_8, BAR_5_8, BAR_0_8};
const health_half[] = {BAR_8_8, BAR_4_8, BAR_0_8};
const health_11_24[] = {BAR_8_8, BAR_3_8, BAR_0_8};
const health_10_24[] = {BAR_8_8, BAR_2_8, BAR_0_8};
const health_9_24[] = {BAR_8_8, BAR_1_8, BAR_0_8};

const health_8_24[] = {BAR_8_8, BAR_0_8, BAR_0_8};
const health_7_24[] = {BAR_7_8, BAR_0_8, BAR_0_8};
const health_6_24[] = {BAR_6_8, BAR_0_8, BAR_0_8};
const health_5_24[] = {BAR_5_8, BAR_0_8, BAR_0_8};
const health_4_24[] = {BAR_4_8, BAR_0_8, BAR_0_8};
const health_3_24[] = {BAR_3_8, BAR_0_8, BAR_0_8};
const health_2_24[] = {BAR_2_8, BAR_0_8, BAR_0_8};
const health_1_24[] = {BAR_1_8, BAR_0_8, BAR_0_8};

const health_none[] = {BAR_0_8, BAR_0_8, BAR_0_8}; 

const char map_level_table[18] = {
  1, 3, 5, 7, 8, 
  10, 10, 10, 10,
  12, 18, 18, 18,
  18, 18, 18, 18,
  20
};

const unsigned char selector_frames[5] = {0x00, 0x40, 0x80, 0x40, 0x00};

struct Castle{
  char map_id;
  int pos;
  char unlocked[3];
};

char j_1, j_2;
struct Castle castles[NO_OF_CASTLES];
// struct Commander commanders[TOTAL_COMMANDERS];

int total_sprites = 0;
char menu_rows = 0;
// char no_of_party_items;
char num_of_bad_terrains;

// char party_items[MAX_INVENTORY];
char menu_height;
// int screen_dimensions = 0;

char selector_frame = 0;
char button_frame = 0;
char script_ram[256];
int map_no = 0;

// extern char *script;
/*
	MENU STUFF
*/

#define CONSUMABLE 0
#define EQUIPABLE 1
#define ANY_ITEM 2
/*
	BATTLE STUFF
*/
#define BATTLE_MAP_HEIGHT 16

enum Direction{
	NORTH, NORTHEAST, EAST, SOUTHEAST, SOUTH, SOUTHWEST, WEST, NORTHWEST
};

#incspr(sld,"map/sprites/sldpiece.pcx")
#incspr(spr,"map/sprites/sprpiece.pcx")
#incspr(dmn,"map/sprites/demonpiece.pcx")
#incspr(hnd,"map/sprites/houndpiece.pcx")
#incspr(arch0,"map/sprites/archpiece0.pcx")
#incspr(mag,"map/sprites/mage.pcx",2,2)
#incspr(blobsmall,"map/sprites/blob.pcx")
#incspr(bnd,"characters/bandit.pcx")
#incspr(thf,"characters/thief_small.pcx")
#incspr(dark,"map/sprites/dark.pcx")
#incspr(gol,"characters/golem_small.pcx")
#incspr(mnk,"characters/monk.pcx")
#incspr(brl,"characters/brawler_small.pcx")
#incspr(lan,"characters/lance.pcx")
#incspr(snipersmall,"characters/sniper_small.pcx")
#incspr(knightsmall,"characters/knight.pcx")
#incspr(stlk,"characters/stalker_small.pcx")

#incpal(sldpal, "map/sprites/sldpiece.pcx",0,1)
#incpal(thfpal, "characters/thief_small.pcx")
// #incpal(cmdrpal, "map/sprites/sprpiece.pcx",2,3)
#incpal(mskpal,"map/sprites/msktpiece.pcx",0,2)
// #incpal(dmnpal,"map/sprites/demonpiece.pcx",0,2)
// #incpal(blobpal,"map/sprites/blob.pcx")
#incpal(magepal,"map/sprites/mage.pcx")
#incpal(bndpal,"characters/bandit.pcx",0,2)
#incpal(monkpal,"characters/monk.pcx")
#incpal(lancepal,"characters/lance.pcx")
#incpal(sniperpal,"characters/sniper_small.pcx")
#incpal(golpal,"characters/golem_small.pcx")

int menu_size = 0;
unsigned int player_gold = 0;
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
// char g = 0;
char game_over = 0;
// char game_loop = 0;
char cursor_x = 0;
char cursor_y = 0;
//int red_crystal_count = 0; //bp -> beast points
//int blue_crystal_count = 0; //ap -> art points
//int green_crystal_count = 0; //up -> upgrade points
int materials_count = 0;
char party_units_size = 0;


char no_miss_flag = 0;
char debug_flag = 0;
int debug_value = 0;
char prebattle_flag = 99;

char demo_select_x = 5;
char demo_select_y = 31;

// char *script;

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

  add_unit_to_convoy(SPEAR_UNIT);
  
	initialize_commanders();
  init_arts();
  // init_overworld_data();
  // add_commander_to_party(name0,REI);
  // add_commander_to_party(name1,VIOLET);

	player_gold = 9999;//1000;
	// no_of_party_items = 0;

  // set_commander_stats(0,1,10,8,8);
  // set_commander_stats(1,1,7,10,8);
	disp_off();
	init_satb();
  load_vram(0x4DB0,cursor,0x10);
  // TACTIC_LEAP;
  party_commanders[0].tactic_id = TACTIC_RAGE;
  party_commanders[1].tactic_id = TACTIC_SCORCH;
  party_commanders[2].tactic_id = TACTIC_DASH;
	for(;;)
	{
    // unlock_all_units();
    unlock_unit(SWORD_UNIT);
    unlock_unit(ARCHER_UNIT);
    unlock_unit(CLERIC_UNIT);
    // simulate_battle(SWORD_UNIT,FIGHTER_UNIT);
		// display_intro();
    display_demo();
		overworld_loop(demo_select_x,demo_select_y);
    // preload_commanders_map_1();
    // overworld_loop(5,31);

    // game_result();
		// battle_loop(0,23,1);
		// battle_loop(24,0,1);
    disp_off();
    display_outro();
	}
}

void heal_commander_bg(char id)
{
  char i;
  for(i=0; i<9; i++)
  {
    if(party_commanders[id].bg.units[i].id)
    {
      load_unit_header(party_commanders[id].bg.units[i].id,0);
      apply_level_to_header(party_commanders[id].bg.units[i].level,0);
      party_commanders[id].bg.units[i].hp = unit_header[0].hp;
      // party_commanders[id].bg.units[i].hp = unit_list[party_commanders[id].bg.units[i].unit->id].hp;
    }
  }
}

void display_demo()
{
  char ticky = 0, loop = 0;
  loop = 1;
  load_vram(0x4DB0,cursor,0x10);
  set_screen_size(SCR_SIZE_32x32);
  load_palette(0,sisters_logo_pal,1);
  // load_default_font();
  cls();
  load_background(sisters_logo,sisters_logo_pal,sisters_bat,32,16);
  load_font(font,125,0x4800);
  load_vram(0x4DB0,cursor,0x10);
  load_palette(10,fontpal,3);
  set_font_pal(12);
  disp_on();
  put_string("Demo",14,17);
  put_string("Press Run",12,19);
  put_string("| Laconic Software 2023",4,24);
  // psgPlay(0);
  while(loop)
  {
    if(joytrg(0) & JOY_RUN)
    {
      select_level_menu();
      loop = 0;
      
      // return;
    }
    else if(ticky++ < 50)
    {
      put_string("Press Run",12,19);
    }
    else if(ticky < 100)
    {
      put_string("         ",12,19);
    }
    else
    {
      ticky = 0;
    }
    vsync();
  }
  disp_off();
}

void select_level_menu()
{
  char curs_pos = 1, loop = 0;
  loop = 1;
  put_string("                 ",8,19);
  put_string("           ",8,20);
  put_string("           ",8,21);
  // load_cursor(7,19,SLIDER_ONE);
  load_cursor(15,19,SLIDER_ONE);
  put_number(curs_pos,2,13,19);

  // put_string("1)Form a plan.",8,19);
  // put_string("2)Defend the town!",8,20);
  // put_string("3)Catching a golem.",8,21);
  clear_joy_events(0x1F);
  while(loop)
  {
    switch(joytrg(0))
    {
      // case JOY_DOWN: if(curs_pos < 2) {curs_pos++; curs_down();} break;
      // case JOY_UP: if(curs_pos > 0) {curs_pos--; curs_up();} break;
      case JOY_LEFT: if(curs_pos > 1) { put_number(--curs_pos,2,13,19); } break;
      case JOY_RIGHT: if(curs_pos < 18) { put_number(++curs_pos,2,13,19); }break;
      case JOY_RUN:
      case JOY_I:
      select_level(curs_pos-1);
      loop = 0;
      break;
      // return;
    }
    vsync();
  }
}

void select_level(char level)
{
  switch(level)
  {
    case 0: demo_select_x = 5; demo_select_y = 31; preload_commanders_map_1(); return;
    case 1: demo_select_x = 3; demo_select_y = 27; preload_commanders_map_2(); return;
    case 2: demo_select_x = 1; demo_select_y = 23; preload_commanders_map_3(); return;
    case 3: demo_select_x = 5; demo_select_y = 19; preload_default(6); return;
    case 4: demo_select_x = 9; demo_select_y = 19; preload_default(8); return;
    case 5: demo_select_x = 9; demo_select_y = 22; preload_default(10); return;
    case 6: demo_select_x = 11; demo_select_y = 25; preload_default(12); return;
    case 7: demo_select_x = 10; demo_select_y = 30; preload_default(13); return;
    case 8: demo_select_x = 18; demo_select_y = 29; preload_default(14); return;
    case 9: demo_select_x = 20; demo_select_y = 26; preload_default(15); return;
    case 10: demo_select_x = 24; demo_select_y = 27; preload_default(16); return;
    case 11: demo_select_x = 27; demo_select_y = 27; preload_default(17); return;
    case 12: demo_select_x = 28; demo_select_y = 24; preload_default(18); return;
    case 13: demo_select_x = 26; demo_select_y = 22; preload_default(19); return;
    case 14: demo_select_x = 23; demo_select_y = 22; preload_default(20); return;
    case 15: demo_select_x = 20; demo_select_y = 21; preload_default(20); return;
    // default: demo_select_x = 10; demo_select_y = 30; preload_default(); return;
    // default: 
    // demo_select_x = 5; demo_select_y = 31; preload_commanders_map_1(); return;
    // return;
  }
}

void load_unit_header(int id, char header_no)
{
  myPointers.idx = 0;
  getFarPointer(nounit, &(myPointers.bank[0]), &(myPointers.addr[0]));
  addOffsetFarPointer(&(myPointers.bank[0]), &(myPointers.addr[0]), id*sizeof(Unit));
  farMemcpy( setFarMemcpy(&unit_header[header_no], myPointers.bank[0], myPointers.addr[0], sizeof(Unit)) );
}

void apply_level_to_header(char level, char header_no)
{
  unsigned char l, offset;
  l = level - 1;
  offset = 6 * unit_header[header_no].growth_chart;
  unit_header[header_no].hp += (STAT_GROWTH_CHART[offset+0] * l);
  unit_header[header_no].atk += (STAT_GROWTH_CHART[offset+1] * l);
  unit_header[header_no].def += (STAT_GROWTH_CHART[offset+2] * l);
  unit_header[header_no].intel += (STAT_GROWTH_CHART[offset+3] * l);
  unit_header[header_no].res += (STAT_GROWTH_CHART[offset+4] * l);
  unit_header[header_no].spd += (STAT_GROWTH_CHART[offset+5] * l);
}

void preload_commanders_map_1()
{
  party_size = 0;
  add_commander_to_party(name0,REI);
  add_commander_to_party(name1,VIOLET);

  load_unit_to_cmdr(0,7,CLERIC_UNIT,0,1);
  load_unit_to_cmdr(0,2,SWORD_UNIT,0,1);
  load_unit_to_cmdr(0,0,SWORD_UNIT,0,1);
  load_unit_to_cmdr(0,1,REI,1,1);

  load_unit_to_cmdr(1,1,SWORD_UNIT,0,1);
  load_unit_to_cmdr(1,3,ARCHER_UNIT,0,1);
  load_unit_to_cmdr(1,4,VIOLET,1,1);
  load_unit_to_cmdr(1,5,ARCHER_UNIT,0,1);
}

void preload_commanders_map_2()
{
  party_size = 0;
  add_commander_to_party(name0,REI);
  add_commander_to_party(name1,VIOLET);
  add_commander_to_party(name2,KING);

  load_unit_to_cmdr(0,7,CLERIC_UNIT,0,3);
  load_unit_to_cmdr(0,2,SWORD_UNIT,0,3);
  load_unit_to_cmdr(0,0,SWORD_UNIT,0,3);
  load_unit_to_cmdr(0,1,REI,1,3);

  load_unit_to_cmdr(1,6,ARCHER_UNIT,0,3);
  load_unit_to_cmdr(1,8,ARCHER_UNIT,0,3);
  load_unit_to_cmdr(1,4,VIOLET,1,3);
  load_unit_to_cmdr(1,7,CLERIC_UNIT,0,3);

  load_unit_to_cmdr(2,3,SPEAR_UNIT,0,3);
  load_unit_to_cmdr(2,5,SPEAR_UNIT,0,3);
  load_unit_to_cmdr(2,7,CLERIC_UNIT,0,3);
  load_unit_to_cmdr(2,1,KING,1,3);
}

void preload_commanders_map_3()
{
  party_size = 0;
  add_commander_to_party(name0,REI);
  add_commander_to_party(name1,VIOLET);
  add_commander_to_party(name2,KING);

  load_unit_to_cmdr(0,6,CLERIC_UNIT,0,5);
  load_unit_to_cmdr(0,8,ARCHER_UNIT,0,5);
  load_unit_to_cmdr(0,2,SWORD_UNIT,0,5);
  load_unit_to_cmdr(0,0,HOUND_UNIT,0,5);
  load_unit_to_cmdr(0,1,REI,1,5);

  load_unit_to_cmdr(1,1,BRAWLER_UNIT,0,5);
  load_unit_to_cmdr(1,6,ARCHER_UNIT,0,5);
  load_unit_to_cmdr(1,4,VIOLET,1,5);
  load_unit_to_cmdr(1,8,ARCHER_UNIT,0,5);
  load_unit_to_cmdr(1,7,CLERIC_UNIT,0,5);

  load_unit_to_cmdr(2,3,SPEAR_UNIT,0,5);
  load_unit_to_cmdr(2,5,SPEAR_UNIT,0,5);
  load_unit_to_cmdr(2,6,HOUND_UNIT,0,5);
  load_unit_to_cmdr(2,8,CLERIC_UNIT,0,5);
  load_unit_to_cmdr(2,1,KING,1,5);
}

void preload_default(char level)
{
  party_size = 0;
  add_commander_to_party(name0,REI);
  add_commander_to_party(name1,VIOLET);
  add_commander_to_party(name2,KING);
  if(level > 8)
  {
    add_commander_to_party(name3,TINKER);
    load_unit_to_cmdr(3,4,TINKER,1,level);
    load_unit_to_cmdr(3,3,RAIDER_UNIT,0,level);
    load_unit_to_cmdr(3,5,RAIDER_UNIT,0,level);
    load_unit_to_cmdr(3,1,RAIDER_UNIT,0,level);
    load_unit_to_cmdr(3,6,THIEF_UNIT,0,level);
    load_unit_to_cmdr(3,8,CLERIC_UNIT,0,level);
  }
  // set_commander_stats(0,5,22,12,16);
  // set_commander_stats(1,5,17,22,12);
  // set_commander_stats(2,5,15,16,22); 
  // set_commander_stats(3,5,15,15,15);

  load_unit_to_cmdr(0,4,REI,1,level);
  load_unit_to_cmdr(0,0,KNIGHT_UNIT,0,level);
  load_unit_to_cmdr(0,2,KNIGHT_UNIT,0,level);
  load_unit_to_cmdr(0,6,MAGE_UNIT,0,level);
  load_unit_to_cmdr(0,7,CLERIC_UNIT,0,level);
  load_unit_to_cmdr(0,8,HOUND_UNIT,0,level);

  load_unit_to_cmdr(1,4,VIOLET,1,level);
  load_unit_to_cmdr(1,0,GOLEM_UNIT,0,level);
  load_unit_to_cmdr(1,2,GOLEM_UNIT,0,level);
  load_unit_to_cmdr(1,7,CLERIC_UNIT,0,level);
  load_unit_to_cmdr(1,6,MAGE_UNIT,0,level);
  load_unit_to_cmdr(1,8,MAGE_UNIT,0,level);

  load_unit_to_cmdr(2,1,KING,1,level);
  load_unit_to_cmdr(2,3,SPEAR_UNIT,0,level);
  load_unit_to_cmdr(2,5,SPEAR_UNIT,0,level);
  load_unit_to_cmdr(2,6,ARCHER_UNIT,0,level);
  load_unit_to_cmdr(2,7,ARCHER_UNIT,0,level);
  load_unit_to_cmdr(2,8,CLERIC_UNIT,0,level);

  // load_unit_to_cmdr(3,0,SWORD_UNIT,1,8);
  // load_unit_to_cmdr(3,2,SWORD_UNIT,0,8);
  // load_unit_to_cmdr(3,3,ARCHER_UNIT,0,8);
  // load_unit_to_cmdr(3,5,ARCHER_UNIT,0,8 );
}

void display_intro()
{
	// load_background(build_screen,build_pal,build_bat,32,28);
	cls();
	disp_on();
	put_string("Strife Sisters v0.9.4",6,13);
	// wait_for_I_input();
	// sync(60*10);

	// load_background(explain_menu,explain_pal,explain_bat,32,28);
	// while(!(joytrg(0) & JOY_RUN)){}
	disp_off();
}

void display_outro()
{
  char loop = 1;
  psgFadeOut(10);
  set_screen_size(SCR_SIZE_32x32);
  clear_joy_events(0x1F);
  reset_satb();
  satb_update();
  gfx_clear(0);
  load_font(font,125,0x4800);

	scroll(0,0,0,224,0xC0);
  load_palette(0,sisters_logo_pal,1);
  cls();
  disp_on();
  // psgPlay(2);
	put_string("Thanks for playing!",7,14);
  // vsync(30);
  // sync(2);
  while(loop)
  {
    // if(joytrg(0) & JOY_RUN)
    // {
    //   loop = 0;
    //   game_over = 1;
    // }
  }
  disp_off();
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
      put_string("PKM",x,y);
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
      put_string("CLT",x,y);
      break;

		case HOUND_UNIT:
      put_string("HND",x,y);
      break;

    case GOLEM_UNIT:
      put_string("GLM",x,y);
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

    case THIEF_UNIT:
      put_string("THF",x,y);
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

    case REI:
      put_string("REI",x,y);
      break;

    case VIOLET:
      put_string("VIO",x,y);
      break;

    case KING:
      put_string("KNG",x,y);
      break;

		default:
      put_string("$$$",x,y);
      break;
	}
}

void print_unit_fullname(char unit_id, int x, int y)
{
  put_string(get_unit_fullname(unit_id),x,y);
}

char* get_unit_fullname(char unit_id)
{
  switch(unit_id)
  {
    case NO_UNIT:
      return " $$$$  ";

    case SWORD_UNIT:
      return "Soldier";

    case DEMON_UNIT:
      return "Demon  ";

    case SPEAR_UNIT:
      return "Pikeman";

    case STALKER_UNIT:
      return "Stalker";

    case ARCHER_UNIT:
      return "Archer ";

    case SNIPER_UNIT:
      return "Sniper ";

    case CLERIC_UNIT:
      return "Cleric ";

    case MAGE_UNIT:
      return "Mage   ";

    case WITCH_UNIT:
      return "Witch  ";

    case PRIEST_UNIT:
      return "Priest ";

    case BLACK_MAGE_UNIT:
      return "Cultist";

    case HOUND_UNIT:
      return "Hellhnd";

    case BLOB_UNIT:
      return "Blob   ";

    case AXE_UNIT:
      return "Axeman ";

    case KNIGHT_UNIT:
      return "Knight ";

    case PALADIN_UNIT:
      return "Paladin";

    case MONK_UNIT:
      return "Monk   ";

    case FIGHTER_UNIT:
      return "Fighter";

    case BRAWLER_UNIT:
      return "Brawler";

    case LANCER_UNIT:
      return "Lancer ";

    case BERSERKER_UNIT:
      return "Berserkr";

    case RAIDER_UNIT:
      return "Raider ";

    case GOLEM_UNIT:
      return "Golem  ";

    case THIEF_UNIT:
      return "Thief  ";

    case REI:
      return "Rei    ";

    case VIOLET:
      return "Violet ";

    case KING:
      return "King   ";

    case TINKER:
      return "Tinker ";

    default:
      return "        ";
  }
}

void draw_32x32_sprite(char type, int x, int y)
{
  load_unit_header(type,1);
  myPointers.bank[2] = unit_header[1].bank;
  myPointers.addr[2] = unit_header[1].address;

  load_vram_fptr( setFarOffsetLoadvram(0x6E00, myPointers.bank[2], myPointers.addr[2], 0x100, 0) );
  switch(type)
  {
    case SWORD_UNIT:
      load_palette(30,soldierpal,1);
      // load_vram(0x6E00,attack,0x100);
      break;

    case SNIPER_UNIT:
      load_palette(30,sniperbtlpal,1);
      // load_vram(0x6E00,sniperbtl,0x100);
    break;

    case BERSERKER_UNIT:
      load_palette(30,sniperbtlpal,1);
      // load_vram(0x6E00,berserkerbtl,0x100);
    break;

    case KNIGHT_UNIT:
      load_palette(30,spearpal,1);
      // load_vram(0x6E00,knightbtl,0x100);
      break;

    case PALADIN_UNIT:
      load_palette(30,paladinpal,1);
      // load_vram(0x6E00,paladinbtl,0x100);
      break;

    case SPEAR_UNIT:
      load_palette(30,spearpal,1);
      // load_vram(0x6E00,attack2,0x100);
      break;

    case STALKER_UNIT:
      load_palette(30,stalkerpal,1);
      // load_vram(0x6E00,stalkerbtl,0x100);
      break;

    case ARCHER_UNIT:
      load_palette(30,musketbtlpal,1);
      // load_vram(0x6E00,musketbtl,0x100);
      break;

    case AXE_UNIT:
      load_palette(30,axebtlpal,1);
      // load_vram(0x6E00,axebtl,0x100);
      break;

    case HOUND_UNIT:
      load_palette(30,houndbtlpal,1);
      // load_vram(0x6E00,houndbtl,0x100);
      break;

    case CLERIC_UNIT:
      load_palette(30,magebtlpal+16,1);
      // load_vram(0x6E00,magebtl,0x100);
      break;

    case WITCH_UNIT:
      load_palette(30,witchbtlpal,1);
      // load_vram(0x6E00,witchbtl,0x100);
      break;

    case PRIEST_UNIT:
      load_palette(30,witchbtlpal,1);
      // load_vram(0x6E00,priestbtl,0x100);
      break;

    case MAGE_UNIT:
      load_palette(30,magebtlpal,1);
      // load_vram(0x6E00,magebtl,0x100);
      break;
    
    case BLACK_MAGE_UNIT:
      load_palette(30,magebtlpal+32,1);
      // load_vram(0x6E00,magebtl,0x100);
      break;

    case FIGHTER_UNIT:
      load_palette(30,monkbtlpal,1);
      // load_vram(0x6E00,fighterbtl,0x100);
      break;

    case BRAWLER_UNIT:
      load_palette(30,monkbtlpal,1);
      // load_vram(0x6E00,brawlerbtl,0x100);
      break;

    case MONK_UNIT:
      load_palette(30,monkbtlpal,1);
      // load_vram(0x6E00,monkbtl,0x100);
      break;

    case LANCER_UNIT:
      load_palette(30,spearpal,1);
      // load_vram(0x6E00,lancerbtl,0x100);
      break;

    case RAIDER_UNIT:
      load_palette(30,raiderbtlpal,1);
      // load_vram(0x6E00,raiderbtl,0x100);
      break;

    case THIEF_UNIT:
      load_palette(30,thfpal,1);
      // load_vram(0x6E00,thiefbtl,0x100);
      break;

    case REI:
      load_palette(30,rei_battle_pal,1);
      break;
    
    case VIOLET:
      load_palette(30,violet_battle_pal,1);
      break;

    // default:
      // spr_hide(1);
  }
  spr_make(1,x,y,0x6E00,FLIP_MAS|SIZE_MAS,SZ_32x32,30,1);
}

void print_unit_attack_icon(char a_type, int x, int y)
{
  // load_unit_header(unit_no,0);
	switch(a_type)
	{
		case NORMAL:
      put_char('[',x,y);//sword
      break;

		case PIERCE:
       put_char('\\',x,y);//spear
       break;

		case UNARMED:
       put_char('_',x,y);//unarmed
       break;

		case MAGIC:
       put_char('^',x,y);//magic
       break;

		case MISSILE:
       put_char(']',x,y);//bow
       break;

		case AXE:
       put_char('`',x,y);
       break;

    case BEAST:
      put_char(127,x,y);
      break;

		default:
       put_char('[',x,y);
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
  put_char(' ',cursor_x+s_x_relative,s_y_relative+cursor_y++);
  put_char('{',cursor_x+s_x_relative,s_y_relative+cursor_y);
}

void curs_up()
{
  put_char(' ',cursor_x+s_x_relative,s_y_relative+cursor_y--);
  put_char('{',cursor_x+s_x_relative,s_y_relative+cursor_y);
}

void curs_right(char amt)
{
  put_char(' ',cursor_x+s_x_relative,s_y_relative+cursor_y);
  cursor_x += amt;
  put_char('{',cursor_x+s_x_relative,s_y_relative+cursor_y);
}

void curs_left(char amt)
{
  put_char(' ',cursor_x+s_x_relative,s_y_relative+cursor_y);
  cursor_x -= amt;
  put_char('{',cursor_x+s_x_relative,s_y_relative+cursor_y);
}

void remove_cursor()
{
  put_char(' ',cursor_x+s_x_relative,cursor_y+s_y_relative);
}

void display_cursor()
{
  put_char('{',cursor_x+s_x_relative,cursor_y+s_y_relative);
}

void load_cursor(char x, char y, int cursor_no)
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

    if(!(get_joy_events(0,0) & JOY_I))
    {
      vsync(2);
    }
	}
	return i;
}

void wait_for_I_input()
{
  // char j;
  // j = joy(0);
  // joy(0);
	vsync(10);
	while(!(joytrg(0) & JOY_I)){}
}

void affirmative_question(char *request, char x, char y)
{
  char question, response;

  question = 1;
  response = 0;
  // remove_cursor();

  display_window_rel(x,y,10,14);
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

void affirm_statement(char *line1, char *line2, char x, char y)
{
  display_window_rel(x,y,10,14);
  put_string(line1,s_x_relative+x+1,s_y_relative+y+5);
  put_string(line2,s_x_relative+x+1,s_y_relative+y+6);
  load_cursor(x+3,y+8,SLIDER_ONE);
  put_string("ok",s_x_relative+x+4,s_y_relative+y+8);
  wait_for_I_input();
}

initialize_commanders()
{
	char i, j;
	struct Commander *cmdr;

	for(i=0, cmdr = party_commanders; i<TOTAL_COMMANDERS; i++, cmdr++)
	{
		// cmdr->meter = 0;
		cmdr->max_bp = 50;
		cmdr->sprite_type = SWORD_UNIT;
		cmdr->name = name20;
    cmdr->tactic_id = 0;
		for(j=0; j<9; j++)
		{
			cmdr->bg.units[j].id = 0;//&unit_list[NO_UNIT];
      cmdr->bg.units[j].hp = 0;
      cmdr->bg.units[j].exp = 0;
      cmdr->bg.units[j].level = 0;
      cmdr->bg.units[j].sta = 0;
    }
    // cmdr->bg.calling_stone = 0;
	}
}

void darken_palette(int pal_num)
{
	modify_palette(pal_num,-3);
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
		rgb = get_color((pal_num<<4)+i);

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
		set_color_rgb((pal_num<<4)+i,r,g,b);
	}
}

// void load_item(char item_id, int index)
// {
// 	switch(item_id)
// 	{
// 		case 0:
// 		load_vram(PORTRAIT_VRAM + (index * 0x100),dagger_gfx,0x100);
// 		load_palette(12+index,daggerpal,1);
// 		break;

// 		case 1:
// 		load_vram(PORTRAIT_VRAM + (index * 0x100),shield_gfx,0x100);
// 		load_palette(12+index,shieldpal,1);
// 		break;

// 		case 2:
// 		load_vram(PORTRAIT_VRAM + (index * 0x100),bow_gfx,0x100);
// 		load_palette(12+index,bowpal,1);
// 		break;

// 		case 3:
// 		load_vram(PORTRAIT_VRAM + (index * 0x100),potion_gfx,0x100);
// 		load_palette(12+index,potionpal,1);
// 		break;

// 		case 14:
// 		default:
// 		load_vram(PORTRAIT_VRAM + (index * 0x100),empty_gfx,0x100);
// 		load_palette(12+index,elixirpal,1);
// 		break;
// 	}
// }

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

    case COMMAND_WALK:
    do_walk();
    return 4;

    case COMMAND_STOP: return 0;
  }
}

void do_walk()
{
  walk_entity(1,((script_ram[3]&0xFF)<<8)+((script_ram[2]&0xFF)));
  update_map();
  vsync();
}

void do_pan()
{
  scroll(1,0,0,0,menu_height,0x80);
  pan_camera_y(((int)(script_ram[2]&0xFF)<<8)+((int)(script_ram[1])&0xFF));
  update_map();
  vsync();
}

void do_focus()
{
  int location;
  location = ((script_ram[2]&0xFF)<<8)+((script_ram[1]&0xFF));
  // put_hex((script_ram[2]&0xFF)<<8,4,0,0);
  // put_hex(script_ram[1]&0xFF,4,6,0);
  // wait_for_I_input();
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
  clear_joy_events(0x1F);
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
	while(offset < 12000)
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
	while(offset < 12000)//just a large number, need to change this to how long out actually is
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
  vaddr = vram_addr(x+s_x_relative,y+s_y_relative);
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
    // case 8:
    //   window_8(vaddr,row_x,row_y);
    //   break;

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

    case 32:
      window_32(vaddr,row_x,row_y);
      break;
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
      vreg(0x02,0xA4A0);
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

void change_background_pal_single(int tile, int pal)
{
  int vaddr, x, y;
  int tiledata;

  x = (tile%32);
	y = (tile/32);

  // vaddr = vram_addr(30,13);
  vaddr = vram_addr(x,y);
  
  vreg(0x01,vaddr);
  tiledata = peekw(0x02) & 0xFFF;

  vreg(0x00,vaddr);
  vreg(0x02,tiledata+pal);
}

load_healthbars()
{
	// load_palette(2,healthbarpal,1);
//	load_vram(0xe00,healthbar,0xC0);
  load_vram(0xe00,healthbar,0x150);
}

void display_healthbar(char x, char y, char percent)
{
  int addr;
  addr = vram_addr(x,y);

  if(percent > 95) { load_vram(addr,health_full,3); return;}
  if(percent > 91) { load_vram(addr,health_23_24,3); return;}
  if(percent > 87) { load_vram(addr,health_22_24,3); return;}
  if(percent > 83) { load_vram(addr,health_21_24,3); return;}
  if(percent > 79) { load_vram(addr,health_20_24,3); return;}
  if(percent > 75) { load_vram(addr,health_19_24,3); return;}
  if(percent > 70) { load_vram(addr,health_18_24,3); return;}
  if(percent > 66) { load_vram(addr,health_17_24,3); return;}
  if(percent > 62) { load_vram(addr,health_16_24,3); return;}
  if(percent > 58) { load_vram(addr,health_15_24,3); return;}
  if(percent > 54) { load_vram(addr,health_14_24,3); return;}
  if(percent > 50) { load_vram(addr,health_13_24,3); return;}
  if(percent > 45) { load_vram(addr,health_half,3); return;}

  if(percent > 41) { load_vram(addr,health_11_24,3); return;}
  if(percent > 37) { load_vram(addr,health_10_24,3); return;}
  if(percent > 33) { load_vram(addr,health_9_24,3); return;}
  if(percent > 29) { load_vram(addr,health_8_24,3); return;}
  if(percent > 25) { load_vram(addr,health_7_24,3); return;}
  if(percent > 21) { load_vram(addr,health_6_24,3); return;}
  if(percent > 17) { load_vram(addr,health_5_24,3); return;}
  if(percent > 13) { load_vram(addr,health_4_24,3); return;}
  if(percent > 9) { load_vram(addr,health_3_24,3); return;}
  if(percent > 5) { load_vram(addr,health_2_24,3); return;}
  if(percent > 1) { load_vram(addr,health_1_24,3); return;}
  load_vram(addr,health_none,3);
}

void animate_healthbar(char starting_percentage, char ending_percentage, char x, char y)
{
  char i;
  char health_counter;
  health_counter = starting_percentage;
  display_healthbar(x,y,starting_percentage);
  while(health_counter >= ending_percentage)
  {
    display_healthbar(x,y,health_counter);
    health_counter -= 4;
    sync(1);
  }
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

void add_units_to_cmdr(char cmdr_id, char unit_type, char count)
{
  char i;
  for(i=0; i<count; i++)
  {
    load_unit_to_cmdr(cmdr_id,i,unit_type,0);
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

void load_commanders_gfx(int cmdr_id, int address)
{
	switch(cmdr_id)
	{
		case REI:
		load_vram(address,rei_walk,0x100);
    // load_commander_palette(REI);
		load_palette(get_commander_palette(cmdr_id),rei_walk_pal,1);
		break;

		case VIOLET:
		load_vram(address,violet_walk,0x100);
    // load_commander_palette(VIOLET);
		load_palette(get_commander_palette(cmdr_id),violet_walk_pal,1);
		break;

		case KING:
		load_vram(address,cat_walk,0x100);
    // load_commander_palette(KING);
		load_palette(get_commander_palette(cmdr_id),cat_walk_pal,1);
		break;

		case TINKER:
		load_vram(address,tinker,0x100);
    // load_commander_palette(TINKER);
		load_palette(get_commander_palette(cmdr_id),tinker_pal,1);
		break;

    case TEARLE:
    load_vram(address,tearle,0x100);
    load_palette(get_commander_palette(cmdr_id),tearle_pal,1);
    break;
		// default:
		// load_vram(address,sld,0x100);
    // load_commander_palette()
		// load_palette(get_commander_palette(cmdr_id),sldpal,1);
		// break;
	}
}

void load_commanders_palettes()
{
  load_palette(26,rei_walk_pal,1);
  load_palette(27,violet_walk_pal,1);
  load_palette(28,cat_walk_pal,1);
  load_palette(29,tinker_pal,1);
  load_palette(30,tearle_pal,1);
}

void load_commander_palette(char cmdr_id)
{
  switch(cmdr_id)
  {
    case REI:
    load_palette(26,rei_walk_pal,1);
    break;

    case VIOLET:
    load_palette(27,violet_walk_pal,1);
    break;

    case KING:
    load_palette(28,cat_walk_pal,1);
    break;

    case TINKER:
    load_palette(29,tinker_pal,1);
    break;

    case TEARLE:
    load_palette(30,tearle_pal,1);
    break;
  }
}

char get_commander_palette(char cmdr_id)
{
  switch(cmdr_id)
	{
		case REI:
    return 26;
		// load_palette(26,rei_walk_pal,1);
		// break;

		case VIOLET:
    return 27;
		// load_palette(27,violet_walk_pal,1);
		// break;

		case KING:
    return 28;
		// load_palette(28,cat_walk_pal,1);
		// break;

		case TINKER:
    return 29;

    case TEARLE:
    return 30;

		// load_palette(29,tinker_pal,1);
		// break;

		// default:
		// load_vram(address,sld,0x100);
		// load_palette(pal,sldpal,1);
		// break;
	}
  return 26;
}

void list_commander_army(Battlegroup *bg, char x, char y)
{
  char i, j;
  for(i=0; i<3; i++)
  {
    for(j=0; j<3; j++)
    {
      // print_unit_info(&bg->units[(i*3)+j].unit,x+(i*5)+(s_x/8),y+(j*4)+(s_y/8));
      // put_number(bg->units[(j*3)+i].id,4,0,0);
      // wait_for_I_input();
      print_unit_info(&bg->units[(j*3)+i],x+(i*5)+(s_x/8),y+(j*4)+(s_y/8));
    }
  }
}

int get_iso_x(int iso_x, int iso_y, int sprite_x, int sprite_y)
{
  int x_start, x; 
  
  x_start = (iso_x * 16);

  x = (sprite_x * 16) + (sprite_y * 16);
  return x_start+x;
}

int get_iso_y(int iso_x, int iso_y, int sprite_x, int sprite_y)
{
  int y_start, y; 
  
  y_start = (iso_y * 8);

  y = (sprite_y * 8) - (sprite_x * 8);
  return y_start+y;
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

void load_unit_to_cmdr(char cmdr_id, char unit_pos, char unit_type, char is_cmdr, char level)
{
  load_unit_header(unit_type,0);
  apply_level_to_header(level,0);
  party_commanders[cmdr_id].bg.units[unit_pos].id = unit_type;//&unit_list[unit_type];
  party_commanders[cmdr_id].bg.units[unit_pos].hp = unit_header[0].hp;//unit_list[unit_type].hp;
  party_commanders[cmdr_id].bg.units[unit_pos].sta = unit_header[0].sta;//unit_list[unit_type].sta;
  party_commanders[cmdr_id].bg.units[unit_pos].exp = (level > 1)? next_level(level-1) : 0;
  party_commanders[cmdr_id].bg.units[unit_pos].level = level;
}

void load_predefined_group_layout(char formation, char r_one, char r_two, char r_three, char cmdr_id, char level)
{
  int f_pos;
  char i,j,unit,highest_id;

  highest_id = determine_sprite_type(r_one,r_two,r_three);
  // highest_id = r_two;
  // highest_id = (r_one > r_two)? r_one : r_two;
  // highest_id = (highest_id > r_three)? highest_id : r_three;
  party_commanders[cmdr_id].sprite_type = highest_id;
  unit = 0;

  f_pos = formation * MAX_ARMY_SIZE;
  for(i=0; i<MAX_ARMY_SIZE; i++)
  {
    if(f0[f_pos+i] == 1)
    {
      switch(i/3)
      {
        case 0: unit = r_one; break;
        case 1: unit = r_two; break;
        case 2: unit = r_three; break;
      }
      load_unit_to_cmdr(cmdr_id,i,unit,0,map_level_table[level]);
    }
  }
}

char determine_sprite_type(char r_one, char r_two, char r_three)
{
  if(r_one > 28)
  {
    return r_one;
  }
  if(r_two > 28)
  {
    return r_two;
  }
  if(r_three > 28)
  {
    return r_three;
  }

  return r_two;
}

void display_popup(char *str, char screen)
{
  // s_y_relative = (s_y/8);
  scroll(screen,0,s_y+32,32,224,0x80);
  display_window_rel(11,11,10,4);
  write_text(12,12+s_y_relative,str);

  sync(100);
  // load_map(0,2,0,0,16,29);
  scroll(screen,0,s_y+32,32,224,0xC0);
  // s_y_relative = 0;
}

void swap_commander_units(char cmdr_id, char first, char second)
{
  unsigned char tmp_hp, tmp_sta, tmp_lvl;
  char tmp_id;
  int tmp_exp;

  // put_hex(party_commanders,6,s_x_relative,s_y_relative);

  tmp_hp = party_commanders[cmdr_id].bg.units[first].hp;
  tmp_id = party_commanders[cmdr_id].bg.units[first].id;
  tmp_sta = party_commanders[cmdr_id].bg.units[first].sta;
  tmp_exp = party_commanders[cmdr_id].bg.units[first].exp;
  tmp_lvl = party_commanders[cmdr_id].bg.units[first].level;

  party_commanders[cmdr_id].bg.units[first].hp = party_commanders[cmdr_id].bg.units[second].hp;
  party_commanders[cmdr_id].bg.units[first].id = party_commanders[cmdr_id].bg.units[second].id;
  party_commanders[cmdr_id].bg.units[first].sta = party_commanders[cmdr_id].bg.units[second].sta;
  party_commanders[cmdr_id].bg.units[first].exp = party_commanders[cmdr_id].bg.units[second].exp;
  party_commanders[cmdr_id].bg.units[first].level = party_commanders[cmdr_id].bg.units[second].level;

  party_commanders[cmdr_id].bg.units[second].hp = tmp_hp;
  party_commanders[cmdr_id].bg.units[second].id = tmp_id;
  party_commanders[cmdr_id].bg.units[second].sta = tmp_sta;
  party_commanders[cmdr_id].bg.units[second].exp = tmp_exp;
  party_commanders[cmdr_id].bg.units[second].level = tmp_lvl;
}

void swap_convoy_units(char cmdr_id, char unit_id, char convoy_id)
{
  char tmp_id;

  load_unit_header(party_commanders[cmdr_id].bg.units[unit_id].id,0);
  load_unit_header(party_units[convoy_id],1);
  tmp_id = party_commanders[cmdr_id].bg.units[unit_id].id;

  party_commanders[cmdr_id].bg.units[unit_id].hp = unit_header[1].hp;//unit_list[party_units[convoy_id]].hp;
  party_commanders[cmdr_id].bg.units[unit_id].id = unit_header[1].id;//&unit_list[party_units[convoy_id]];
  party_commanders[cmdr_id].bg.units[unit_id].exp = 0;
  party_commanders[cmdr_id].bg.units[unit_id].level = 1;

  party_units[convoy_id] = tmp_id;
}

void put_green_square(char x, char y)
{

  // vaddr = vram_addr(x,y);

  // vreg(0x01,vaddr);

  // tiledata = peekw(0x02) & 0xFFF;

  // vreg(0x00,vaddr);
  // vreg(0x02,tiledata+pal);
  // vreg(0x02,tiledata+pal+1);
  int addr;
  int p[12];
  int vaddr;
  char i, j;

  addr = 0xF5B0;
  vaddr = vram_addr(x+s_x_relative,y+s_y_relative);

  for(i=0; i<8; i++)
  {
    // p[i] = addr;
    for(j=0; j<12; j++, addr++)
    {
      p[j] = addr;
    }
    load_vram(vaddr+(i*64),p,12);
  }
}

void read_tile_data(int x, int y)
{
  // int vaddr, tiledata;
  // int tiledata1, tiledata2;
  // vaddr = vram_addr(x,y+36);
  // vreg(0x01,vaddr);
  // tiledata1 = peek(0x02);
  // tiledata2 = peek(0x03); 
  // tiledata = (tiledata2<<8) + tiledata1;
  // put_hex(vaddr,6,s_x_relative,s_y_relative);

  // vreg(0x00,vaddr);
  // vreg(0x02,tiledata&0xFFF);
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
    if(party_commanders[cmdr_id].bg.units[i].id)
    {
      load_unit_header(party_commanders[cmdr_id].bg.units[i].id,0);
      power += unit_header[0].pow;//party_commanders[cmdr_id].bg.units[i].unit->pow;
      hp_max += unit_header[0].hp;//party_commanders[cmdr_id].bg.units[i].unit->hp;
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

int next_level(int level)
{
  return (cube(level)*6/4) + ((level*level) / 2) + (2*level);
}

// 1) 1 + 0 + 2 = 3 -> 1 blob
// 2) 12 + 2 + 4 = 18 -> 4 blobs
// 3) 40 + 4 + 6 = 50 -> 12 blobs
// 4) 96 + 8 + 8 = 112 -> 28 blobs
// 5) 187 + 12 + 10 = 209 -> 52 blobs
// 6) 324 + 18 + 12 = 354 -> 88 blobs
// 7) 514 + 24 + 14 = 552 -> 128 blobs

int cube(int number)
{
  number*number*number;
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

    case TINKER:
    add_commander_to_party(name3,TINKER);
    load_unit_to_cmdr(party_size-1,3,AXE_UNIT);
    load_unit_to_cmdr(party_size-1,1,HOUND_UNIT);
    load_unit_to_cmdr(party_size-1,5,AXE_UNIT);
    load_unit_to_cmdr(party_size-1,7,MAGE_UNIT);
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

void cycle_button_press(char x, char y)
{
  button_frame = (button_frame+1) % 16;
  if(button_frame == 0)
  {
    put_string("}~",x,y);
  }
  else if(button_frame == 8)
  {
    put_string("*+",x,y);
  }
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

// char 

void display_number_incrementing(char x, char y, int final_number, char num_len)
{
  int i;
  i=0;
  // if(final_number <= 6)
  // {
    // put_number(final_number,num_len,x,y);
  // }
  while((i+=6) < final_number)
  {
    put_number(i,num_len,x,y);
    sync(1);
  }
  put_number(final_number,num_len,x,y);
}

#include "overworld.c"
#include "battle.c"
