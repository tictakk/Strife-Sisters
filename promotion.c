#include <huc.h>
#include "paths.h"

// #define STRENGTH 1
// #define RECOVERY 2
// #define LUCK 3
// #define INTELLIGENCE 4
// #define WILL 5
// #define HP 6
// #define AP 7
// #define ATTACK 8
// #define DEFENSE 9

#define CMDR 0
#define BEAST 1
#define SAMURAI 2

#define PORTRAIT_VRAM 0x3500
#define CMDR_PANEL_VRAM 0x3600
#define FONT_VRAM 0x5200
#define DIALOGUE_VRAM 0x4000
#define P_BORDER_VRAM 0x4000
#define CMDR_VRAM_SIZE 0x800
#define MAX_CAPTURED 20
#define NO_OF_MENUS 20
#define NO_OF_MENU_ITEMS 30

#incspr(cmdr, "map/sprites/commander.pcx");
#incpal(cmdrpal, "map/sprites/commander.pcx");

#incspr(beast, "map/sprites/beasts.pcx");
#incpal(beastpal, "map/sprites/beasts.pcx");

#incspr(samurai, "map/sprites/samurai.pcx");
#incpal(samuraipal, "map/sprites/samurai.pcx");

// #incchr(bdisplay,"map/backgrounds/battle_display.pcx");
// #incpal(bdisplaypal,"map/backgrounds/battle_display.pcx");

#incchr(healthbar,"map/backgrounds/healthbars.pcx");
#incpal(healthbarpal,"map/backgrounds/healthbars.pcx");

#incchr(panel_gfx, "map/sprites/deploy_menu.pcx")
#incpal(panel_pal, "map/sprites/deploy_menu.pcx")
#incbat(panel_bat, "map/sprites/deploy_menu.pcx", 0x2100, 0, 0, 24, 12)
// #incchr(panel_gfx, "map/sprites/infobox.pcx")
// #incpal(panel_pal, "map/sprites/infobox.pcx")
// #incbat(panel_bat, "map/sprites/infobox.pcx", 0x2100, 0, 0, 32, 4)

#define MAP_WIDTH 32
#define TILE_WIDTH 16
#define TILE_VRAM 0x1000
#define OVERWORLD_MAP_HEIGHT 32
#define NO_OF_CASTLES 15

#define MAX_ARMY_SIZE 45
#define MAX_COMMANDERS 6
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

#incpal(menupal, "map/sprites/inbattle.pcx");

enum GAME_STATE{
	OVERWORLD, BATTLE_SELECT, BATTLE
};

enum Unit_Type{
	INFANTRY, CALVARY, SPEARS, MUSKETS, ENGINEERS
};

typedef struct{
	char atk, def, hp;
	enum Unit_Type unit_type;
} Unit;

struct Commander{
	char id;
	char kingdom;
	char lvl;
	// char sprite;
	char army_size, army_type;
	Unit *unit;
	char hp;
	char ap;
	char str;
	char rec;
	char lck;
	char int;
	char will;
	char *name;
};

struct Castle{
	int x, y;
	char level, no_of_commanders, no_of_soldiers, population, id;
	char owner;
	char commanders[MAX_COMMANDERS];
};

//Map images
#incpal(overworldpal,"map/overworldtilemap1.tiles.pcx");
#inctile(overworldtiles,"map/overworldtilemap.tiles.pcx");
// #inctilepal(overworldtable,"map/overworldtilemap.tiles.pcx");
#incbin(overworldmap,"map/overworldtilemap.Untitled 6.layer-Layer 1.map001.stm")

// #incchr(battlefield,"map/sprites/battlefield2.pcx")
// #incpal(battlefieldpal, "map/sprites/battlefield2.pcx")
// #incbat(battlefieldbat, "map/sprites/battlefield2.pcx",0x1000,32,28)
#inctile(battlefield,"map/backgrounds/battlefieldmap.tiles.pcx")
#incpal(battlefieldpal, "map/backgrounds/battlefieldmap.tiles.pcx")
#incbin(battlefieldbat, "map/backgrounds/battlefieldmap.Untitled 4.layer-Layer 1.map001.stm")

#incchr(castle,"map/sprites/castle.pcx");
#incchr(castle1,"map/sprites/castle1.pcx");
#incchr(castle2,"map/sprites/castle2.pcx");
#incchr(castle3,"map/sprites/castle3.pcx");
#incchr(castle4,"map/sprites/castle4.pcx");
#incpal(castlepal, "map/sprites/castle.pcx");

#incspr(selector,"map/sprites/selector.pcx");
#incpal(selectorpal,"map/sprites/selector.pcx");

char j_1, j_2;
int selector_x, selector_y, s_x, s_y, y_offset, x_offset;
struct Castle castles[NO_OF_CASTLES];
struct Commander commanders[TOTAL_COMMANDERS];
char captured[20];
char current_captured = 0;

int total_sprites = 0;
int counter = 0;
char current_menu_size = 0;

/*
	MENU STUFF
*/

#define CASTLE_MENU 1
#define CASTLE_INFO_MENU 2
#define PRE_BATTLE_MENU 4
#define BATTLE_MENU 8
#define ARTS_MENU 16
#define FORMATION_MENU 32

#define EXIT_OPTION 1
#define SELECT_OPTION 2

#define CURSOR_SIZE 1
#define MAX_SPRITES 63

extern unsigned char current_menu = 0;
extern unsigned char menu_state = 0;
extern unsigned char menu_select = 0;

#incspr(borders, "map/sprites/boarders.pcx");
#incspr(menu, "map/sprites/inbattle.pcx");
#incspr(pre, "map/sprites/prebattlemenu.pcx");
#incspr(formation, "map/sprites/formationmenu.pcx");
#incspr(artsmenu, "map/sprites/artsmenu.pcx");
#incspr(castleinfo, "map/sprites/castleinfo.pcx");
#incspr(castlemenu, "map/sprites/castlemenu.pcx");
#incspr(walker, "map/sprites/walker.pcx");

#incspr(taros, "map/sprites/Taros.pcx");
#incspr(koden, "map/sprites/Koden.pcx");
#incspr(kaytoken, "map/sprites/Kaytoken.pcx");
#incspr(sangai, "map/sprites/Sangai.pcx");
#incspr(bakuria, "map/sprites/Bakuria.pcx");

#incspr(dialog,"map/sprites/dialog.pcx");
#incpal(walkerpal, "map/sprites/walker.pcx");

// struct Castle *current_castle;
// struct Stack stk;
char current_menu = 0;
char current_menu_length = 0;
char current_menu_width = 0;
char current_menu_choices = 0;
char current_menu_x = 0;
char current_menu_y = 0;
char cursor_pointer = 0;
char kingdom = 0;

// struct Commander brendt;
// struct Commander lucious;
// struct Commander sophia;
//
// struct Commander metaron;
// struct Commander falc;
// struct Commander camile;

int menu_min = 0;
int menu_max = 0;
int cursor_vram = 0;
// int z = 0;

/*
	BATTLE STUFF
*/
#define NUMBER_OF_TILES 70//5120
#define SOLDIER_SIZE 32
#define BATTLE_MAP_HEIGHT 16

struct Node{
	int ownX, ownY, fromX, fromY;
	char checked;
};

enum SoldierState{
	WAITING, MOVING, ATTACKING, DYING
};

enum Direction{
	UP, RIGHT, DOWN, LEFT
};

enum Commands{
	ADVANCE, IDLE, SKIRMISH, MELEE, ARTS, RECALL
};

struct soldier{
	int pos, x, y, frame;
	char tic, id;
	enum SoldierState state;
	enum Direction direction;
	char walk, attack, die, active;
};

// struct battle_group{
//   char group_size;
//   char commanders[5];
//   char defeated[5];
// };

// #incpal(testpal,"map/battlemap5.tiles.pcx")
// #inctile(testtiles,"map/battlemap5.tiles.pcx")
// #inctilepal(testtable,"map/battlemap5.tiles.pcx")

// #incchr(battleselectchr,"map/sprites/prebattleconcept2.pcx")
// #incpal(battleselectpal,"map/sprites/prebattleconcept2.pcx")
// #incbat(battleselectbat,"map/sprites/prebattleconcept2.pcx",0x1000)

// #incbin(testmap,"map/battlemap5.Untitled 2.layer-Layer 1.map001.stm")

/* sprites */
#incspr(attack, "map/sprites/unit_test.pcx")
#incpal(soldierpal, "map/sprites/unit_test.pcx")

#incspr(attack2, "map/sprites/unit_test2.pcx")

#incspr(musketbtl, "map/sprites/msktbtl.pcx")
#incpal(musketbtlpal, "map/sprites/msktbtl.pcx")

// #incpal(soldierpal2, "map/sprites/unit_test.pcx");
#incspr(commander, "map/sprites/commander1.pcx")
#incpal(commanderpal, "map/sprites/commander1.pcx")

#incchr(portraitgfx, "map/sprites/portrait.pcx", 0, 0, 4, 4)
#incchr(portraitm, "map/sprites/portraitm.pcx", 0, 0, 4, 4)
// #incspr(portrait2, "map/sprites/portrait2.pcx")
// #incspr(portrait1, "map/sprites/portrait1.pcx")
#incpal(portraitpal, "map/sprites/portrait.pcx")
#incpal(portraitmpal, "map/sprites/portraitm.pcx")

#incchr(cmdr_info_panel, "map/sprites/cmdr_info_panel.pcx",0,0,8,12)
#incbat(cmdr_info_panel_bat, "map/sprites/cmdr_info_panel.pcx",0x3600,0,0,8,12)

#incspr(brskr, "map/sprites/berserker.pcx")
#incpal(brskrpal, "map/sprites/berserker.pcx")

// #incspr(ball, "map/sprites/ball.pcx");
// #incpal(ballpal, "map/sprites/ball.pcx");

const int sprite_memory_start = 0x5000;
const int sprite_size = 0x800;

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

// char grid[NUMBER_OF_TILES];

enum Commands comm_1 = 1;
enum Commands comm_2 = 1;
enum Commands commands = 1;

Unit spears;
Unit calvary;
Unit infantry;
Unit muskets;
Unit engineers;

// struct Node map[100];
// struct Node neighbors[4];

struct soldier armyOne[15]; //9 is max?
struct soldier armyTwo[15];
struct soldier *sp;

// 	INFANTRY, CALVARY, SPEARS, MISSLES, ENGINEERS

int map_counter = 0;
int map_size = 0;
// int soldierFrame = 0x4000;
int soldierFrame = 0;
// int altFrame = 0x5000;
int altFrame = 0;
int frame = 0x5000;
int tic = 0;
int sx = 0;
int sy = 0;
int yOffset = 0;
char j1 = 0;
int j2 = 0;
int xOffset = 0;
int z = 0;
int j = 0;

int team_id = 0;
int opp = 0;
int opp_area = 0;
int opp_army_size = 0;
int opp_attack_area = 0;
int team_area = 0;
int team_army_size = 0;
int total_units = 0;

char commander_one_hp = 10;
char commander_two_hp = 10;
char sprite_offset = 0;
char commander_select = 0;

int TEAM_ONE_ID = 1;
int TEAM_TWO_ID = 2;
int ATTACK_AREA_ONE = 4;
int ATTACK_AREA_TWO = 8;

// struct Menu menus[NO_OF_MENUS];
// struct Menu_Item menu_items[NO_OF_MENU_ITEMS];

main()
{
	// struct Commander *cmdr;
	char deploy_one[2];
	char deploy_two[2];
	selector_x = 0;
	selector_y = 64;
	cursor_vram = 0x68C0;

	spears.hp  = 135;
	spears.atk = 22;
	spears.def = 27;
	spears.unit_type = SPEARS;

	calvary.hp  = 95;
	calvary.atk = 26;
	calvary.def = 22;
	calvary.unit_type = CALVARY;

	infantry.hp  = 105;
	infantry.atk = 20;
	infantry.def = 29;
	infantry.unit_type = INFANTRY;

	muskets.hp  = 70;
	muskets.atk = 30;
	muskets.def = 13;
	muskets.unit_type = MUSKETS;

	engineers.atk = 15;
	engineers.def = 27;
	engineers.hp  = 60;
	engineers.unit_type = ENGINEERS;

	deploy_one[0] = 1;
	deploy_two[0] = 2;
	deploy_one[1] = 3;
	deploy_two[1] = 4;
	// make_menus();
	initialize_commanders();
	// commanders[1].unit = &spears;
	commanders[1].army_size = 16;
	commanders[2].army_size = 15;
	commanders[3].army_size = 17;
	// commanders[1].army_type = 1;
	commanders[6].army_type = 1;
	commanders[9].army_type = 2;
	commanders[0].name = "Sammie";
	// commanders[0].army_type = 1;
	commanders[1].name = "Alucard";
	// commanders[1].army_type = 2;
	commanders[2].name = "Ren";
	// commanders[2].army_type = &calvary;
	commanders[3].name = "Violet";
	// commanders[3]->unit = &infantry;
	commanders[4].name = "Stimpy";
	commanders[5].name = "Sacha";
	commanders[6].name = "Oli";
	commanders[7].name = "Venric";
	commanders[8].name = "Phalc";
	commanders[9].name = "Keely";
	commanders[0].unit = &spears;
	// cmdr = commanders;
	// cmdr->unit = &spears;
	// cmdr++;
	// cmdr->unit = &spears;
	// (*commanders[0].unit) = &spears;
	// (*commanders[1].unit) = &spears;
	disp_off();
	cls();
	init_satb();

	commanders[6].army_size = 18;

	load_palette(19, cursorpal, 1);
	load_palette(20, menupal, 1);
	load_palette(17, walkerpal,1);
	load_vram(0x68C0,cursor,0x40);
	satb_update();

	// set_font_color(2, 1);
	// set_font_pal(1);
	// load_default_font(1,0x1B00);

	for(;;)
	{
		overworld_loop();
		// battle_loop(0,0);
		// battlefield_loop(deploy_one,2,deploy_two,2);
	}
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

char get_pal_by_sprite_no(int sprite_no)
{
	switch(sprite_no)
	{
		case CMDR: return 20;
		case BEAST: return 21;
		case SAMURAI: return 22;
		default: return 20;
	}
}

void display_sprite(int x_pos, int y_pos, int location, char flip, char pal)
{
	spr_make(total_sprites++,x_pos,y_pos,location,flip,NO_FLIP|SZ_16x16,pal,0);
	spr_pri(1);
}

print_unit_type(enum Unit_Type type, int x, int y)
{
	switch(type)
	{
		case INFANTRY:
		put_string("inftry",x,y);
		break;

		case CALVARY:
		put_string("calvary",x,y);
		break;

		case SPEARS:
		put_string("spears",x,y);
		break;

		case MUSKETS:
		put_string("archer",x,y);
		break;

		case ENGINEERS:
		put_string("enginr",x,y);
		break;
	}
}

void display_sprite_by_no(int x_pos, int y_pos, int location, char flip, char pal, char sprite_no)
{
	spr_make(sprite_no,x_pos,y_pos,location,flip,NO_FLIP|SZ_16x16,pal,0);
	spr_pri(1);
}

void clear_sprite(int sprite_size)
{
	while(sprite_size-- > 0)
	{
		spr_set(--total_sprites);
		spr_hide();
	}
}

void load_cursor(int x, int y)
{
	display_sprite(x,y,cursor_vram,FLIP_MAS|SIZE_MAS,3);
}

void clear_menu()
{
	clear_sprite(current_menu_size);
	current_menu_size = 0 ;
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
			// put_number(0,1,1,1);
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
}

initialize_commanders()
{
	char i;
	struct Commander *cmdr;
	for(i=0, cmdr = commanders; i<TOTAL_COMMANDERS; i++, cmdr++)
	{
		cmdr->hp = 5+i;
		cmdr->ap = 5;
		cmdr->str = 5;
		cmdr->rec = 5;
		cmdr->lck = 5;
		cmdr->int = 5;
		cmdr->will = 5;
		// cmdr->sprite = i%3;
		cmdr->army_size = 10 + (i%5);
		// cmdr->army_size = 15;
		cmdr->army_type = 0;
		cmdr->kingdom = i/6;
		// cmdr->name = "steve";
		cmdr->unit = &calvary;
		cmdr->id = i;
	}
}

// void load_commander(char cmdr_no, int cmdr_vram, char pal_no, int amt_to_load)
// {
// 	switch(cmdr_no)
// 	{
// 		case CMDR:
// 		load_palette(pal_no,cmdrpal,1);
// 		load_vram(cmdr_vram,cmdr,amt_to_load);
// 		break;
//
// 		case BEAST:
// 		load_palette(pal_no,beastpal,1);
// 		load_vram(cmdr_vram,beast,amt_to_load);
// 		break;
//
// 		case SAMURAI:
// 		load_palette(pal_no,samuraipal,1);
// 		load_vram(cmdr_vram,samurai,amt_to_load);
// 		break;
// 	}
// }

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

void capture_commander(char cmdr_id, char kingdom)
{
	captured[current_captured++] = cmdr_id;
}

void load_portrait(char cmdr_id)
{
	switch(cmdr_id)
	{

		case 2:
		load_vram(PORTRAIT_VRAM,portraitm,0x100);
		load_palette(4,portraitmpal,1);
		break;

		default:
		load_vram(PORTRAIT_VRAM,portraitgfx,0x100);
		load_palette(4,portraitpal,1);
		break;
	}
}

void display_portrait(char cmdr_id, int x, int y)
{
	int bat[16];
	int offset, i;
	offset = (x>>1)/8 + (((y>>1)/8)*64);
	offset += 26;

	for(i=0; i<16; i++)
	{
		bat[i] = ((PORTRAIT_VRAM>>4)+0x4000) + i;
	}
	load_bat(offset,bat,4,4);
}

void display_cmdr_info_panel(int x, int y)
{
	int offset, i;
	int bat[96];

	offset = (x>>1)/8 + (((y>>1)/8)*64);
	offset += 24;

	for(i=0; i<96; i++)
	{
		bat[i] = ((CMDR_PANEL_VRAM >> 4) + 0x1000) + i;
	}

	load_vram(CMDR_PANEL_VRAM,cmdr_info_panel,0x600);
	load_bat(offset,bat,8,12);
}

// load_information_panel()
// {
// 	int j, vaddr;
// 	int ptr[1];
//
// 	load_palette(1,bdisplaypal,1);
// 	load_vram(0x6000,bdisplay,4096);
// 	for(j=0; j<256; j++)
// 	{
// 		vaddr = vram_addr((j&31),(j/32));
// 		ptr[0] = (0x6000 >> 4) + j + 0x1000;
// 		load_vram(vaddr,ptr,1);
// 	}
// }

void change_background_palette(int tile, int pal)
{
	int vaddr;
	int ptr[2];
	vaddr = vram_addr((tile%16)*2,((tile/16)*2));

	// ptr[0] = (pal + 0x180) + (((tile)/16) * 0x40) + ((tile%16) * 2);
	// ptr[1] = (pal + 0x180) + (((tile)/16) * 0x40) + ((tile%16) * 2) + 1;
	ptr[0] = (pal + 0xE0) + ((battlefieldbat[tile] * 4));
	ptr[1] = (pal + 0xE0) + ((battlefieldbat[tile] * 4) + 1);
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

void set_sprite_to_location(char id, int location, char pal_num)
{
	switch(id)
	{
		// case 1:
		default:
		load_vram(location,brskr,0x200);
		load_palette(pal_num,brskrpal,1);
		break;
	}
}

#include "menu.c"
#include "overworld.c"
// #include "battle_select.c"
#include "battle.c"
