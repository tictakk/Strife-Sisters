#incbin(script, "script/out")
// #incbin(area_0,"script/area_0")
// #incbin(area_1,"script/area_1")
// #incbin(area_2,"script/area_2")
// #incbin(area_3,"script/area_3")
// #incbin(area_4,"script/area_4")
// #incbin(area_5,"script/area_5")

// #asm
// .bank 35
// .org $8000
// _script: 
// .incbin "script/area_0", 
// .incbin "script/area_1", 
// .incbin "script/area_2", 
// .incbin "script/area_3", 
// .incbin "script/area_4", 
// .incbin "script/area_5"

// .data
// .bank DATA_BANK
// .code
// #endasm  

#define NO_OF_MAPS 25

#define NO_UNIT 0
#define SWORD_UNIT 1 //soldier?
#define SPEAR_UNIT 2
#define ARCHER_UNIT 3
#define MAGE_UNIT 4
#define DEMON_UNIT 5
#define HOUND_UNIT 6
#define BLOB_UNIT 7
#define AXE_UNIT 8
#define PALADIN_UNIT 9
#define MONK_UNIT 10
#define SNIPER_UNIT 11
#define LANCER_UNIT 12
#define KNIGHT_UNIT 13
#define BERSERKER_UNIT 14
#define STALKER_UNIT 15
#define ARTILLERY 16
#define CLERIC_UNIT 17
#define PRIEST_UNIT 18
#define BLACK_MAGE_UNIT 19
#define WITCH_UNIT 20
#define RAIDER_UNIT 21
#define PIRATE_UNIT 22
#define CAPTAIN 23
#define FIGHTER_UNIT 24
#define BRAWLER_UNIT 25
#define GOLEM_UNIT 26
#define CREATURE_2 27
#define CREATURE_3 28

#define REI 29
#define VIOLET 30
#define KING 31
#define TINKER 32
#define CALIEN 33 //Calien, Sunette, Zonja, Taffa, Xelase
#define TEARLE 34 //Cyran, Danag, Dorlus, Tearle
#define HEIN 35 //Zaladin, Adonis, Zandre, Wikus
#define ADONIS 36
#define PHALC 37
#define KURT 38
#define GEN_ONE 39
#define SWORD_CMDR 40
#define SPEAR_CMDR 41
#define ARCH_CMDR 42
#define GENERIC_CMDR 43
#define HOUND_CMDR 44
#define BLOB_CMDR 45

#define ROW_ONE 0
#define ROW_TWO 1
#define ROW_THREE 2
#define MAX_UNITS_PER_ROW 3

char s_y_relative = 0;
char s_x_relative = 0;
