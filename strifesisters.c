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

typedef struct {
    char idx;
    char bank[3];
    int  addr[3];
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
;_dancer:
    .include "game_data/units/dancer.asm"
; _captain:
    .include "game_data/units/captain.asm"
; _brawler:
    .include "game_data/units/brawler.asm"
;_golem:
    .include "game_data/units/golem.asm"
; _fighter:
    .include "game_data/units/fighter.asm"
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
;_calien:
    .include "game_data/units/calien.asm"
;_rannon:
    .include "game_data/units/hein.asm"
;_adonis:
    .include "game_data/units/adonis.asm"
;_zaladin:
    .include "game_data/units/zaladin.asm"
    .code
#endasm


extern const char nounit[];
extern const char soldier[];

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
struct Node map[80]; //a unit with a max move of 5 will fill 60 nodes max.

#include "map_dimension.h"
#include "paths.c"

#define MAX_TACTIC_METER 50
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

#define SONG_INTRO 0
#define SONG_DIALOG 1
#define SONG_SAD 2
#define SONG_BATTLE 3

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

#define PORTRAIT_VRAM 0x6200//0x4400//move to 0x6200???
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
#define COMMAND_REMOVE 5
#define COMMAND_FOCUS 4
#define COMMAND_STOP 9

#define TILE_HIGHLIGHT 453
#define TILE_FORMATION 353
#define TILE_GRAY 291

#incchr(laconic,"assets/laconic.pcx")
#incpal(laconic_pal,"assets/laconic.pcx")
#incbat(laconic_bat,"assets/laconic.pcx",0x1000,0,0,32,24)

// #incchr(healthbar,"map/backgrounds/healthbars.pcx");
// #incpal(healthbarpal,"map/backgrounds/healthbars.pcx");
#incchr(healthbar,"map/sprites/health.pcx")

#incchr(sisters_logo,"map/backgrounds/sister_logo_small.pcx")
#incpal(sisters_logo_pal,"map/backgrounds/sister_logo_small.pcx");
#incbat(sisters_bat,"map/backgrounds/sister_logo_small.pcx",0x1000,0,0,32,16)

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
#incpal(violet_battle_pal,"characters/witch_big.pcx",1,1)

#incchr(king_gfx, "characters/king.pcx")
#incpal(king_pal, "characters/king.pcx")
#incpal(king_battle_pal,"map/sprites/swordy.pcx",3,1)

#incspr(tinker, "characters/tinker.pcx")
#incpal(tinker_pal, "characters/tinker.pcx")
#incchr(tinker_gfx, "characters/tinker_portrait.pcx")
#incpal(tinker_gfx_pal, "characters/tinker_portrait.pcx")
#incpal(tinker_battle_pal,"map/sprites/archer.pcx",1,1)

#incchr(calien_gfx,"characters/calien_portrait.pcx")
#incpal(calien_gfx_pal,"characters/calien_portrait.pcx")
#incpal(calien_battle_pal,"map/sprites/swordy.pcx",4,1)

#incspr(tearle,"characters/tearle.pcx")
#incchr(tearle_gfx,"characters/tearle_portrait.pcx")
#incpal(tearle_gfx_pal,"characters/tearle_portrait.pcx")
#incpal(tearle_pal,"characters/tearle.pcx")
#incpal(tearle_battle_pal,"characters/berserker.pcx",1,1)

#incchr(hein_gfx,"characters/hein_portrait.pcx")
#incpal(hein_portrait_pal,"characters/hein_portrait.pcx")
#incspr(hein,"characters/hein_walk.pcx")
#incpal(hein_pal,"characters/hein_walk.pcx")
#incpal(hein_battle_pal,"characters/banditbattle.pcx",1,1);

#incchr(adonis_gfx,"characters/adonis_portrait.pcx")
#incpal(adonis_portrait_pal,"characters/adonis_portrait.pcx")
#incspr(adonis,"characters/adonis_walk.pcx")
#incpal(adonis_pal,"characters/adonis_walk.pcx")

#incspr(zaladin,"characters/zaladin_sprite.pcx")
#incchr(zaladin_gfx,"characters/zaladin_portrait.pcx")
#incpal(zaladin_gfx_pal,"characters/zaladin_portrait.pcx")
#incpal(zaladin_pal,"characters/zaladin_sprite.pcx")
#incpal(zaladin_battle_pal,"map/sprites/swordy.pcx",3,1)

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

// const char *objective0 = "Defeat all enemies";
// const char *objective1 = "Defend the objective";
// const char *objective2 = "Secure the objective";
// const char *objective3 = "Kill the target";


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

const char map_level_table[15] = {
  1, 2, 3, 4, 7,
  9, 11, 12, 14,
  15, 16, 17, 18,
  19, 20
};

const int payouts[5] = {700, 650, 600, 550, 500};

const unsigned char selector_frames[5] = {0x00, 0x40, 0x80, 0x40, 0x00};
const char demons[4] = { BLOB_UNIT, HOUND_UNIT, GOLEM_UNIT, DEMON_UNIT };

enum Direction{
	NO, NORTH, NORTHEAST, EAST, SOUTHEAST, SOUTH, SOUTHWEST, WEST, NORTHWEST
};

struct Castle{
  char map_id;
  int pos;
  enum Direction direction;
  char unlocked;
  char range_lo, range_hi;
};

char j_1, j_2;
struct Castle castles[NO_OF_CASTLES];
// struct Commander commanders[TOTAL_COMMANDERS];
char selected_formation = 0;
char menu_rows = 0;

// char party_items[MAX_INVENTORY];
char menu_height;
// int screen_dimensions = 0;

char selector_frame = 0;
char script_ram[256];
int map_no = 0;

char training_enemies[10];
char training_enemies_count = 0;
char training_enemies_formation = 0;

char owned_formations[45];
char owned_formation_count = 0;
char formation_cursor = 0;
// extern char *script;

/*
	BATTLE STUFF
*/
#define BATTLE_MAP_HEIGHT 16

#incspr(sld,"map/sprites/sldpiece.pcx")
#incspr(spr,"map/sprites/sprpiece.pcx")
#incspr(dmn,"map/sprites/demonpiece.pcx")
#incspr(hnd,"map/sprites/houndpiece.pcx")
#incspr(arch0,"map/sprites/archpiece0.pcx")
#incspr(mag,"map/sprites/mage.pcx",2,2)
#incspr(blobsmall,"map/sprites/blob.pcx")
#incspr(bnd,"characters/bandit.pcx")
#incspr(dark,"map/sprites/dark.pcx")
#incspr(gol,"characters/golem_small.pcx")
#incspr(mnk,"characters/monk.pcx")
#incspr(brk,"characters/berserker_small.pcx")
#incspr(brl,"characters/brawler_small.pcx")
#incspr(lan,"characters/lance.pcx")
#incspr(snipersmall,"characters/sniper_small.pcx")
#incspr(knightsmall,"characters/knight_small.pcx")
#incspr(stlk,"characters/stalker_small.pcx")
#incspr(wtc,"characters/witch_small.pcx")
#incspr(dan,"characters/dancer_small.pcx")

#incpal(danpal,"characters/dancer_small.pcx")
#incpal(sldpal, "map/sprites/sldpiece.pcx",0,1)
// #incpal(cmdrpal, "map/sprites/sprpiece.pcx",2,3)
#incpal(mskpal,"map/sprites/msktpiece.pcx",0,2)
#incpal(dmnpal,"map/sprites/demonpiece.pcx")
// #incpal(blobpal,"map/sprites/blob.pcx")
#incpal(magepal,"map/sprites/mage.pcx")
#incpal(bndpal,"characters/bandit.pcx",0,2)
#incpal(monkpal,"characters/monk.pcx")
#incpal(lancepal,"characters/lance.pcx")
// #incpal(sniperpal,"characters/sniper_small.pcx")
#incpal(golpal,"characters/golem_small.pcx")

unsigned int final_score = 0;
unsigned int player_gold = 0;
int party_units[MAX_PARTY_UNIT_SIZE];

int map_counter = 0;
int map_size = 0;
int objective_pos = 0;
int sx = 0;
int sy = 0;
int yOffset = 0;
int xOffset = 0;

char game_over = 0;
char cursor_x = 0;
char cursor_y = 0;

char party_units_size = 0;

char no_miss_flag = 0;
char debug_flag = 0;
int debug_value = 0;
char prebattle_flag = 99;

char demo_select_x = 5;
char demo_select_y = 31;

char demo_flag = 1;

unsigned char current_formation[9];

void main()
{
  owned_formations[0] = 1;

  owned_formation_count = 1;
	//setup (one time)
  psgInit(5);
  psgOn(0);

	sngInit();
	psgDelay(0);
	
	//play music (probably move this to whatever place you want this music to play)
	//right now you have 1 song, as you get more you can psgPlay( X ) where X is song number :)

  // add_unit_to_convoy(SPEAR_UNIT);
	initialize_commanders();
  init_arts();
  // init_overworld_data();
  // add_commander_to_party(name0,REI);
  // add_commander_to_party(name1,VIOLET);

	player_gold = 250;//1000;
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
  party_commanders[3].tactic_id = TACTIC_SWITCH;
	for(;;)
	{
    // unlock_all_units();
    unlock_unit(SWORD_UNIT);
    // simulate_battle(SWORD_UNIT,FIGHTER_UNIT);
		// display_intro();
    display_demo();
    if(demo_flag)
    {
      display_intro();
    }
		overworld_loop(demo_select_x,demo_select_y);
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
  load_palette(0,laconic_pal,1);
  // load_default_font();
  cls();
  load_background(laconic,laconic_pal,laconic_bat,32,24);
  disp_on();
  sync(120);
  fade_screen();
  sync(60);
  disp_off();

  load_palette(0,sisters_logo_pal,1);
  cls();
  load_background(sisters_logo,sisters_logo_pal,sisters_bat,32,16);
  // load_background(laconic,laconic_pal,laconic_bat,32,24);
  load_font(font,125,0x4800);
  load_vram(0x4DB0,cursor,0x10);
  load_palette(10,fontpal,3);
  set_font_pal(12);
  disp_on();
  // put_string("Demo",14,17);
  put_string("Press Run",12,19);
  put_string("| Laconic Software 2023",4,25);
  if(demo_flag)
  {
    psgPlay(0);
  }
  while(loop)
  {
    if(joytrg(0) & JOY_RUN)
    {
      if(demo_flag)
      {
        select_level(0);
      }
      else
      {
        select_level_menu();
      }
      loop = 0;
    }
    else if(joytrg(0) & JOY_SEL)
    {
      psgPlay(++g);
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

  clear_joy_events(0x1F);
  while(loop)
  {
    switch(joytrg(0))
    {
      // case JOY_DOWN: if(curs_pos < 2) {curs_pos++; curs_down();} break;
      // case JOY_UP: if(curs_pos > 0) {curs_pos--; curs_up();} break;
      case JOY_LEFT: if(curs_pos > 1) { put_number(--curs_pos,2,13,19);} break;
      case JOY_RIGHT: if(curs_pos < 17) { put_number(++curs_pos,2,13,19);} break;
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
  preload_commanders_map_1();
  switch(level)
  {
    case 0: demo_select_x = 5; demo_select_y = 31; return;
    case 1: demo_select_x = 3; demo_select_y = 27; return;
    case 2: demo_select_x = 1; demo_select_y = 23; return;
    case 3: demo_select_x = 5; demo_select_y = 19; return;
    case 4: demo_select_x = 9; demo_select_y = 19; return;
    case 5: demo_select_x = 9; demo_select_y = 22; return;
    case 6: demo_select_x = 11; demo_select_y = 25; return;
    case 7: demo_select_x = 10; demo_select_y = 30; return;
    case 8: demo_select_x = 18; demo_select_y = 29; return;
    case 9: demo_select_x = 20; demo_select_y = 26; return; 
    case 10: demo_select_x = 24; demo_select_y = 27; return;
    case 11: demo_select_x = 27; demo_select_y = 27; return;
    case 12: demo_select_x = 28; demo_select_y = 24; return;
    case 13: demo_select_x = 26; demo_select_y = 22; return;
    case 14: demo_select_x = 23; demo_select_y = 22; return;
    // case 15: demo_select_x = 20; demo_select_y = 21; return;
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
  l = min(level - 1,MAX_LEVEL);
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
  // add_commander_to_party(name1,VIOLET);
  // add_commander_to_party(name2,KING);
  // add_commander_to_party(name3,TINKER);

  // party_commanders[0].bg.formation = 0;
  // party_commanders[1].bg.formation = 0;
  // party_commanders[2].bg.formation = 0;
  // party_commanders[3].bg.formation = 0;

  load_unit_to_cmdr(0,4,REI,1,1);
  // load_unit_to_cmdr(1,4,VIOLET,1,14);
  // load_unit_to_cmdr(2,4,KING,1,14);
  // load_unit_to_cmdr(3,7,TINKER,1,14);

  // for(owned_formation_count=0; owned_formation_count<44; owned_formation_count++)
  // {
  //   owned_formations[owned_formation_count] = 1;
  // }

  // load_unit_to_cmdr(0,0,KNIGHT_UNIT,0,16);
  // load_unit_to_cmdr(0,1,KNIGHT_UNIT,0,16);
  // load_unit_to_cmdr(0,2,KNIGHT_UNIT,0,16);
  // load_unit_to_cmdr(0,3,DANCER_UNIT,0,16);
  // load_unit_to_cmdr(0,5,DANCER_UNIT,0,16);
  // load_unit_to_cmdr(0,7,CLERIC_UNIT,0,16);

  // load_unit_to_cmdr(1,0,BRAWLER_UNIT,0,16);
  // load_unit_to_cmdr(1,1,BRAWLER_UNIT,0,16);
  // load_unit_to_cmdr(1,2,BRAWLER_UNIT,0,16);
  // load_unit_to_cmdr(1,3,DANCER_UNIT,0,16);
  // load_unit_to_cmdr(1,5,DANCER_UNIT,0,16);
  // load_unit_to_cmdr(1,7,CLERIC_UNIT,0,16);

  // load_unit_to_cmdr(2,0,LANCER_UNIT,0,16);
  // load_unit_to_cmdr(2,1,LANCER_UNIT,0,16);
  // load_unit_to_cmdr(2,2,LANCER_UNIT,0,16);
  // load_unit_to_cmdr(2,3,SPEAR_UNIT,0,16);
  // load_unit_to_cmdr(2,5,SPEAR_UNIT,0,16);
  // load_unit_to_cmdr(2,7,CLERIC_UNIT,0,16);

  // load_unit_to_cmdr(3,0,KNIGHT_UNIT,0,16); //39
  // load_unit_to_cmdr(3,1,KNIGHT_UNIT,0,16);
  // load_unit_to_cmdr(3,2,KNIGHT_UNIT,0,16);
  // load_unit_to_cmdr(3,4,STALKER_UNIT,0,16);
  // load_unit_to_cmdr(3,6,ARCHER_UNIT,0,16);
  // load_unit_to_cmdr(3,8,ARCHER_UNIT,0,16);

  // party_commanders[0].bg.formation = 36;
  // party_commanders[1].bg.formation = 36;
  // party_commanders[2].bg.formation = 36;
  // party_commanders[3].bg.formation = 39;

  // unlock_all_units();
  // for(i=1; i<9; i++)
  // {
  //   unlock_units_by_castle(i);
  // }
  // load_unit_to_cmdr(0,0,REI,1,4);
  // load_unit_to_cmdr(0,2,SWORD_UNIT,0,3);
  // load_unit_to_cmdr(1,4,VIOLET,1,3);
  // load_unit_to_cmdr(2,4,KING,1,3);
  player_gold = 250;//9000;
}

void play_credits()
{
  int len;
  char i;
  char *title, *name;
  title = title1;
  name = creditname1;
  
  for(i=0; i<5; i++)
  {
    cls();
    put_string(title,6,16);
    put_string(name,7,17);

    sync(240);
    title += strlen(title)+1;
    name += strlen(name) + 1;
  }

  name = creditname6;
  for(i=0; i<10; i++)
  {
    cls();
    put_string(title6,6,16);
    put_string(name,7,17);

    sync(240);
    title += strlen(title)+1;
    name += strlen(name) + 1;
  }
  cls();
  put_string("Thanks for playing!",7,16);
  put_string("Final Score",7,17);
  put_number(final_score,7,8,18);
  for(;;){}
}

void preload_default(char level)
{
  party_size = 0;
  add_commander_to_party(name0,REI);
  add_commander_to_party(name1,VIOLET);
  add_commander_to_party(name2,KING);

  load_unit_to_cmdr(0,4,REI,1,level);
  load_unit_to_cmdr(1,4,VIOLET,1,level);
  load_unit_to_cmdr(2,1,KING,1,level);

  if(level > 13)
  {
    add_commander_to_party(name3,TINKER);
    load_unit_to_cmdr(3,4,TINKER,1,level);
  }

  player_gold = 5000;
}

void display_intro()
{
	// load_background(build_screen,build_pal,build_bat,32,28);
  // disp_off();
  psgFadeOut(10);
  sync(10);
  psgPlay(SONG_DIALOG);
	cls();
	disp_on();

  clear_and_write(3,10,25,intro1);
  clear_and_write(3,10,25,intro2);
  clear_and_write(3,10,25,intro3);
  clear_and_write(3,10,25,intro4);
  clear_and_write(3,10,25,intro5);
  clear_and_write(3,10,25,intro6);
  clear_and_write(3,10,25,intro7);
  clear_and_write(3,10,25,intro8);
  clear_and_write(3,10,25,intro9);
  clear_and_write(3,10,25,intro10);
  clear_and_write(3,10,25,intro11);

  fade_screen();
  sync(10);
  psgFadeOut(10);
  sync(10);
  psgPlay(SONG_INTRO);
	disp_off();
}

void clear_and_write(char x, char y, char line_len, char *str)
{
  cls();
  write_text_with_line_size(x,y,line_len,str);
  wait_for_I_input();
  clear_joy_events(0x1F);
}

void display_outro()
{
  // char loop = 1;
  fade_screen();
  psgFadeOut(10);
  set_screen_size(SCR_SIZE_32x32);
  clear_joy_events(0x1F);
  reset_satb();
  satb_update();
  gfx_clear(0);

  load_font(font,125,0x4800);
	scroll(0,0,0,224,0xC0);
  // load_palette(0,overworldpal,8);
  load_palette(10,fontpal,3);
  set_font_pal(12);
  load_palette(0,sisters_logo_pal,1);
  sync(60);
  // cls();
  disp_on();
  if(demo_flag)
  {
    psgPlay(2);
  }
	put_string("To be continued...",7,16);
  sync(120);
  play_credits();
  // vsync(30);
  // sync(2);
  // while(loop)
  // {
    // if(joytrg(0) & JOY_RUN)
    // {
    //   loop = 0;
    //   game_over = 1;
    // }
  // }
  // disp_off();
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

    case CLERIC_UNIT:
      put_string("CLR",x,y);
      break;

    case WITCH_UNIT:
      put_string("WIT",x,y);
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

    case RAIDER_UNIT:
      put_string("RDR",x,y);
      break;

    case KNIGHT_UNIT:
      put_string("KNT",x,y);
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

    case DANCER_UNIT:
      put_string("DAN",x,y);
      break;

    case RANNON:
      put_string("RAN",x,y);
      break;

    case TINKER:
      put_string("TIN",x,y);
      break;

    case TEARLE:
      put_string("TRL",x,y);
      break;

    case CALIEN:
      put_string("CAL",x,y);
      break;

    case ZALADIN:
      put_string("ZAL",x,y);
      break;

    case ADONIS:
      put_string("ADN",x,y);
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

    case KNIGHT_UNIT:
      return "Knight ";

    case FIGHTER_UNIT:
      return "Fighter";

    case BRAWLER_UNIT:
      return "Brawler";

    case LANCER_UNIT:
      return "Lancer ";

    case BERSERKER_UNIT:
      return "Bersrkr";

    case RAIDER_UNIT:
      return "Raider ";

    case GOLEM_UNIT:
      return "Golem  ";

    case DANCER_UNIT:
      return "Dancer ";

    case REI:
      return "Rei    ";

    case VIOLET:
      return "Violet ";

    case KING:
      return "Kingsly";

    case TINKER:
      return "Tinker ";

    case RANNON:
      return "Rannon ";

    case CALIEN:
      return "Calien ";

    case ZALADIN:
      return "Zaladin";

    case TEARLE:
      return "Tearle ";

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
      load_palette(20,soldierpal,1);
      // load_vram(0x6E00,attack,0x100);
      break;

    case BERSERKER_UNIT:
      load_palette(20,soldierpal,1);
      // load_vram(0x6E00,berserkerbtl,0x100);
    break;

    case KNIGHT_UNIT:
      load_palette(20,soldierpal,1);
      // load_vram(0x6E00,knightbtl,0x100);
      break;

    case PALADIN_UNIT:
      load_palette(20,paladinpal,1);
      // load_vram(0x6E00,paladinbtl,0x100);
      break;

    case SPEAR_UNIT:
      load_palette(20,soldierpal,1);
      // load_vram(0x6E00,attack2,0x100);
      break;

    case STALKER_UNIT:
      load_palette(20,stalkerpal,1);
      // load_vram(0x6E00,stalkerbtl,0x100);
      break;

    case ARCHER_UNIT:
      load_palette(20,musketbtlpal,1);
      // load_vram(0x6E00,musketbtl,0x100);
      break;

    case AXE_UNIT:
      load_palette(30,axebtlpal,1);
      // load_vram(0x6E00,axebtl,0x100);
      break;

    case HOUND_UNIT:
      load_palette(20,houndbtlpal,1);
      // load_vram(0x6E00,houndbtl,0x100);
      break;

    case CLERIC_UNIT:
      load_palette(20,magebtlpal,1);
      // load_vram(0x6E00,magebtl,0x100);
      break;

    case WITCH_UNIT:
      load_palette(20,witchbtlpal,1);
      // load_vram(0x6E00,witchbtl,0x100);
      break;

    case MAGE_UNIT:
      load_palette(20,magebtlpal,1);
      // load_vram(0x6E00,magebtl,0x100);
      break;
    
    case BLACK_MAGE_UNIT:
      load_palette(20,magebtlpal+32,1);
      // load_vram(0x6E00,magebtl,0x100);
      break;

    case BRAWLER_UNIT:
      load_palette(20,monkbtlpal,1);
      // load_vram(0x6E00,brawlerbtl,0x100);
      break;

    case LANCER_UNIT:
      load_palette(20,soldierpal,1);
      // load_vram(0x6E00,lancerbtl,0x100);
      break;

    case RAIDER_UNIT:
      load_palette(20,raiderbtlpal,1);
      // load_vram(0x6E00,raiderbtl,0x100);
      break;

    case BLOB_UNIT:
      load_palette(20,blobbattlepal,1);
      break;

    case REI:
      load_palette(20,rei_battle_pal,1);
      break;
    
    case VIOLET:
      load_palette(20,violet_battle_pal,1);
      break;

    case TINKER:
      load_palette(20,tinker_battle_pal,1);
      break;

    case KING:
      load_palette(20,king_battle_pal,1);
      break;
    
    case DANCER_UNIT:
      load_palette(20,dancerbigpal,1);
      break;

    // default:
      // spr_hide(1);
  }
  spr_make(1,x,y,0x6E00,FLIP_MAS|SIZE_MAS,SZ_32x32,20,1);
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

int write_text_with_line_size(char x, char y, char line_size, char *text)
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
		if(text[i] == 32 && text_x >= line_size)
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
  char cur_x_holder, cur_y_holder;
  cur_x_holder = cursor_x;
  cur_y_holder = cursor_y;
  display_window_rel(x,y,10,14);
  put_string(line1,s_x_relative+x+1,s_y_relative+y+5);
  put_string(line2,s_x_relative+x+1,s_y_relative+y+6);
  load_cursor(x+3,y+8,SLIDER_ONE);
  put_string("ok",s_x_relative+x+4,s_y_relative+y+8);
  wait_for_I_input();
  cursor_x = cur_x_holder;
  cursor_y = cur_y_holder;
}

void check_add_new_commander(char map_no)
{
  if(map_no == 1 && party_size < 2)//add violet
  {
    add_commander_to_party(name1, VIOLET);
    load_unit_to_cmdr(1,4,VIOLET,1,2);
  }
  if(map_no == 3 && party_size < 3)//add king
  {
    add_commander_to_party(name2,KING);
    party_commanders[2].bg.formation = 15;
    if(owned_formations[15] == 0)
    {
      owned_formations[15] = 1;
      owned_formation_count++;
    }
    load_unit_to_cmdr(2,3,KING,1,3);
    load_unit_to_cmdr(2,1,SWORD_UNIT,0,3);
    load_unit_to_cmdr(2,5,SPEAR_UNIT,0,3);
    load_unit_to_cmdr(2,7,CLERIC_UNIT,0,3);
    return;
  }
  if(map_no == 7)//add tinker
  {
    // remove_party_commander_from_game(2);
    add_commander_to_party(name3,TINKER);
    party_commanders[3].bg.formation = 34;
    if(owned_formations[34] == 0)
    {
      owned_formations[34] = 1;
      owned_formation_count++;
    }
    party_commanders[3].tactic_id = TACTIC_SWITCH;
    load_unit_to_cmdr(3,7,TINKER,1,11);
    load_unit_to_cmdr(3,6,ARCHER_UNIT,0,11);
    load_unit_to_cmdr(3,8,ARCHER_UNIT,0,11);

    load_unit_to_cmdr(3,4,KNIGHT_UNIT,0,11);
    load_unit_to_cmdr(3,3,KNIGHT_UNIT,0,11);
    load_unit_to_cmdr(3,5,KNIGHT_UNIT,0,11);
  }

  if(map_no == 13)//add tearle
  {
    // remove_party_commander_from_game(2);
    add_commander_to_party(name4,TEARLE);
    party_commanders[3].bg.formation = 44;
    party_commanders[3].tactic_id = TACTIC_LEAP;

    load_unit_to_cmdr(3,0,KNIGHT_UNIT,0,18);
    load_unit_to_cmdr(3,1,KNIGHT_UNIT,0,18);
    load_unit_to_cmdr(3,2,KNIGHT_UNIT,0,18);

    load_unit_to_cmdr(3,3,BRAWLER_UNIT,0,18);
    load_unit_to_cmdr(3,4,TEARLE,1,18);
    load_unit_to_cmdr(3,5,BRAWLER_UNIT,0,18);

    load_unit_to_cmdr(3,6,WITCH_UNIT,0,18);
    load_unit_to_cmdr(3,7,CLERIC_UNIT,0,18);
    load_unit_to_cmdr(3,8,WITCH_UNIT,0,18);
    return;
  }
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
      cmdr->bg.formation = 0;
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
    vsync();
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

void tile_highlight(int pal_num, char pal_index, int rgb)
{
  set_color((pal_num<<4)+(pal_index+5),rgb);
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

    case TEARLE:
    load_vram(PORTRAIT_VRAM+(index*0x100),tearle_gfx,0x100);
		load_palette(12+index,tearle_gfx_pal,1);
		break;

    case TINKER:
    load_vram(PORTRAIT_VRAM+(index*0x100),tinker_gfx,0x100);
		load_palette(12+index,tinker_gfx_pal,1);
    break;

		case RANNON:
		load_vram(PORTRAIT_VRAM+(index*0x100),hein_gfx,0x100);
		load_palette(12+index,hein_portrait_pal,1);
		break;

    case ADONIS:
		load_vram(PORTRAIT_VRAM+(index*0x100),adonis_gfx,0x100);
		load_palette(12+index,adonis_portrait_pal,1);
		break;

    case CALIEN:
		load_vram(PORTRAIT_VRAM+(index*0x100),calien_gfx,0x100);
		load_palette(12+index,calien_gfx_pal,1);
    break;

    case ZALADIN:
		load_vram(PORTRAIT_VRAM+(index*0x100),zaladin_gfx,0x100);
		load_palette(12+index,zaladin_gfx_pal,1);
    break;

		default:
		load_vram(PORTRAIT_VRAM+(index*0x100),unknown_gfx,0x100);
		load_palette(12+index,unknown_pal,1);
		break;
	}
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

    case COMMAND_REMOVE:
    do_remove();
    return 2;

    case COMMAND_STOP: return 0;
  }
}

void do_remove()
{
  remove_party_commander_from_game(script_ram[1]);
}

void do_walk()
{
  walk_entity(script_ram[1],((script_ram[3]&0xFF)<<8)+((script_ram[2]&0xFF)));
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
    clear_joy_events(0x1F);
    text_size = write_text(6,1,script_ram+2)+3;
    wait_for_I_input();
	}
  else
  {
    clear_joy_events(0x1F);
    text_size = write_text(1,1,script_ram+2)+3;
    wait_for_I_input();
  }
  // clear_joy_events(0x1F);
  // text_size = write_text(6,1,script_ram+2)+3;
  // wait_for_I_input();
  return text_size;
}

//this doesn't break and I think it should...
//this may cause some bugs down there line
int find_area_offset(char area)
{
	unsigned int offset=0;

	// offset = current_offset;
	while(offset < 33000)
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
	unsigned int offset = 0;
	offset = current_offset;
	while(offset < 33000)//just a large number, need to change this to how long out actually is
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

    case RANNON:
    load_vram(address,hein,0x100);
    load_palette(get_commander_palette(cmdr_id),hein_pal,1);
    break;

    case ADONIS:
    load_vram(address,adonis,0x100);
    load_palette(get_commander_palette(cmdr_id),adonis_pal,1);
    break;

    case CALIEN:
    load_vram(address,sld,0x100);
    load_palette(get_commander_palette(cmdr_id),calien_battle_pal,1);
    break;

    case ZALADIN:
    load_vram(address,zaladin,0x100);
    load_palette(get_commander_palette(cmdr_id),zaladin_pal,1);
    break;
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

    case RANNON:
    load_palette(25,hein_pal,1);
    break;

    case ZALADIN:
    load_palette(25,zaladin_pal,1);
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

    case ADONIS:
    case RANNON:
    case CALIEN:
    case ZALADIN:
    return 25;
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
  // put_number(x_start+x,5,s_x_relative,s_y_relative);
  return x_start+x;
}

int get_iso_y(int iso_x, int iso_y, int sprite_x, int sprite_y)
{
  int y_start, y;
  
  y_start = (iso_y * 8);

  y = (sprite_y * 8) - (sprite_x * 8);
  // put_number(y_start+y,5,s_x_relative+6,s_y_relative);
  return y_start+y;
}

void list_party_units(char x, char y)
{
  char i;
  for(i=0; i<MAX_PARTY_UNIT_SIZE; i++)
  {
    print_unit_type(party_units[i]&0xFF,x+(4*(i/8)),y+(i%8));
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
  // put_number(unit_type,5,s_x_relative,s_y_relative);
  // wait_for_I_input();
  load_unit_header(unit_type,0);
  apply_level_to_header(level,0);
  party_commanders[cmdr_id].bg.units[unit_pos].id = unit_type;//&unit_list[unit_type];
  party_commanders[cmdr_id].bg.units[unit_pos].hp = unit_header[0].hp;//unit_list[unit_type].hp;
  party_commanders[cmdr_id].bg.units[unit_pos].exp = (level > 1)? next_level(level-1) : 0;
  party_commanders[cmdr_id].bg.units[unit_pos].level = level;
}

void load_predefined_group_layout(char *units, char cmdr_id, char level)
{
  char i, highest_id;

  highest_id = units[0];
  party_commanders[cmdr_id].sprite_type = highest_id;

  for(i=0; i<MAX_ARMY_SIZE; i++)
  {
    if(units[i+1])
    {
      if(units[i+1] >= COMMANDER_ID_START)
      {
        load_unit_to_cmdr(cmdr_id,i,units[i+1],1,map_level_table[level]);
      }
      else
      {
        load_unit_to_cmdr(cmdr_id,i,units[i+1],0,map_level_table[level]);
      }
    }
  }
}

void display_popup(char *str, char screen)
{
  scroll(screen,0,s_y+32,32,224,0x80);
  display_window_rel(11,11,10,4);
  write_text(12,12+s_y_relative,str);

  sync(100);
  scroll(screen,0,s_y+32,32,224,0xC0);
}

void swap_commander_units(char cmdr_id, char first, char second)
{
  unsigned char tmp_hp, tmp_lvl;
  char tmp_id;
  int tmp_exp;

  // put_hex(party_commanders,6,s_x_relative,s_y_relative);

  tmp_hp = party_commanders[cmdr_id].bg.units[first].hp;
  tmp_id = party_commanders[cmdr_id].bg.units[first].id;
  tmp_exp = party_commanders[cmdr_id].bg.units[first].exp;
  tmp_lvl = party_commanders[cmdr_id].bg.units[first].level;

  party_commanders[cmdr_id].bg.units[first].hp = party_commanders[cmdr_id].bg.units[second].hp;
  party_commanders[cmdr_id].bg.units[first].id = party_commanders[cmdr_id].bg.units[second].id;
  party_commanders[cmdr_id].bg.units[first].exp = party_commanders[cmdr_id].bg.units[second].exp;
  party_commanders[cmdr_id].bg.units[first].level = party_commanders[cmdr_id].bg.units[second].level;

  party_commanders[cmdr_id].bg.units[second].hp = tmp_hp;
  party_commanders[cmdr_id].bg.units[second].id = tmp_id;
  party_commanders[cmdr_id].bg.units[second].exp = tmp_exp;
  party_commanders[cmdr_id].bg.units[second].level = tmp_lvl;
}

void swap_convoy_units(char cmdr_id, char unit_id, char convoy_id)
{
  int tmp_id;//0x3765 == party_units ram address (convoy)

  load_unit_header(party_commanders[cmdr_id].bg.units[unit_id].id,0);
  load_unit_header(party_units[convoy_id]&0xFF,1);

  if(party_commanders[cmdr_id].bg.units[unit_id].id == 0)//unit going to the convoy
  {
    tmp_id = 0;
  }
  else
  {
    tmp_id = party_commanders[cmdr_id].bg.units[unit_id].id;
    tmp_id += party_commanders[cmdr_id].bg.units[unit_id].level << 8;
  }

  if((party_units[convoy_id]&0xFF) == 0)//unit coming from the convoy
  {
    party_commanders[cmdr_id].bg.units[unit_id].hp = 0;//unit_header[1].hp;//unit_list[party_units[convoy_id]].hp;
    party_commanders[cmdr_id].bg.units[unit_id].id = 0;///unit_header[1].id;//&unit_list[party_units[convoy_id]];
    party_commanders[cmdr_id].bg.units[unit_id].exp = 0;//next_level((party_units[convoy_id]>>8)-1);
    party_commanders[cmdr_id].bg.units[unit_id].level = 0;//party_units[convoy_id]>>8;
  }
  else
  {
    party_commanders[cmdr_id].bg.units[unit_id].hp = unit_header[1].hp;//unit_list[party_units[convoy_id]].hp;
    party_commanders[cmdr_id].bg.units[unit_id].id = unit_header[1].id;//&unit_list[party_units[convoy_id]];
    party_commanders[cmdr_id].bg.units[unit_id].exp = next_level((party_units[convoy_id]>>8)-1);
    party_commanders[cmdr_id].bg.units[unit_id].level = party_units[convoy_id]>>8;
  }

  party_units[convoy_id] = tmp_id;
}

void put_green_square(int address, char x, char y)
{
  int addr;
  int p[12];
  int vaddr;
  char i, j;
  load_palette(13,square_pal,1);
  addr = address; //0xE5B0;
  vaddr = vram_addr(x+s_x_relative,y+s_y_relative);

  for(i=0; i<8; i++)
  {
    for(j=0; j<12; j++, addr++)
    {
      p[j] = addr;
    }
    load_vram(vaddr+(i*64),p,12);
  }
}

unsigned int next_level(int level)
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

unsigned int cube(int number)
{
  number*number*number;
}

void add_unit_to_convoy(char unit_id, char level)
{
  char i;

  for(i=0; i<MAX_PARTY_UNIT_SIZE; i++)
  {
    if(party_units[i] == NO_UNIT)
    {
      party_units[i] = unit_id;
      party_units[i] += level << 8;
      party_units_size++;
      return;
    }
  }
}

void load_selector(int address,char pal)
{
  load_vram(address,selector,0x40);
  load_palette(pal,selectorpal,1);
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
  for(i=0; i<(flashes*3); i++)
  {
    cycle_selector();
    satb_update();
    sync(6);
  }
}

void start_battle(char cmdr_id, char battle_type)
{
  fade_screen();
  disp_off();
  reset_satb();
  heal_commander_bg(cmdr_id);

  // load_predefined_group_layout(training_enemies,MAX_PARTY_COMMANDERS,0);

  add_entity(PLAYER,cmdr_id,0,party_commanders,1);
  add_entity(CPU,0,1,enemy_commanders,0);

  battle_loop(0,1,1,0,0,battle_type);

  cleanup_entities();
}

void randomize_enemy_team()
{
  char i;
  char random_demon;
  char *formation;

  for(i=0; i<10; i++)
  {
    training_enemies[i] = 0;
  }

  srand(clock_tt());
  random_demon = get_random_demon();
  srand(clock_tt());
  formation = get_random_formation();

  training_enemies[0] = random_demon;
  training_enemies_count = 0;

  for(i=0; i<9; i++)
  {
    if(formation[i])
    {
      training_enemies[i+1] = random_demon;
      training_enemies_count++;
    }
  }
  // put_number(training_enemies_count,4,s_x_relative,s_y_relative);
}

char get_next_formation(char cursor)
{
  char i, form_counter;
  form_counter = 0;
  for(i=0; i<45; i++)
  {
    if(owned_formations[i])
    {
      if(form_counter++ == cursor)
      {
        return i;
      }
    }
  }
  return 0;
}

void draw_formation(char f_id)
{
  char i;
  char *formation;

  formation = get_formation(f_id);
  for(i=0; i<9; i++)
  {
    if(formation[i])
    {
      tile_highlight(0xE,i,TILE_FORMATION);
      current_formation[i] = 1;
    }
    else
    {
      tile_highlight(0xE,i,TILE_GRAY);
      current_formation[i] = 0;
    }
  }
}

char valid_formation_placement(char index)
{
  return current_formation[index];
}

char get_random_demon()
{
  return unlocked_units[range(0,buyable_unit_count-1)];
}

char *get_random_formation()
{
  training_enemies_formation = range(castles[map_no].range_lo,castles[map_no].range_hi);
  return get_formation(training_enemies_formation);//f0+(range(0,2) * 9);
}

char *get_formation(char f_num)
{
  return f0 + (f_num*9);
}

void display_number_incrementing(char x, char y, int final_number, char num_len)
{
  int i, inc_amt;
  inc_amt = 6;
  if(final_number > 1000)
  {
    inc_amt += 20;
  }
  if(final_number > 5000)
  {
    inc_amt += 200;
  }
  i=0;  
  while((i+=inc_amt) < final_number)
  {
    put_number(i,num_len,x,y);
    sync(1);
  }
  put_number(final_number,num_len,x,y);
}

char get_convoy_size()
{
  char i, c_size;
  for(i=0, c_size=0; i<MAX_PARTY_UNIT_SIZE; i++)
  {
    if((party_units[i]&0xFF))
    {
      c_size++;
    }
  }
  return c_size;
}

// char get_next_convoy_spot()
// {
//   char i;
//   for(i=0; i<MAX_PARTY_UNIT_SIZE; i++)
//   {
//     if((party_units[i]&0xFF) == 0)
//     {
//       return i;
//     }
//   }
//   return -1;
// }

#include "overworld.c"
#include "battle.c"
