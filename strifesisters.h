#incbin(script, "script/out")
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
#define CREATURE_1 26
#define CREATURE_2 27
#define CREATURE_3 28

#define REI 29
#define VIOLET 30
#define KING 31
#define TINKER 32
#define KAIN 33
#define SACHA 34
#define ALMER 35
#define VEN 36
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

const int EXP_CHART[10] = //ten levels max
{
  50, 150, 350, 700, 1500, 2500, 5000, 10000, 25000, 50000
};

char s_y_relative = 0;
char s_x_relative = 0;

char *location_lookup(int position)
{
  switch(position)
  {
    case 997:
    return "Fort Begin";

    case 971:
    return "Port Here";

    case 876:
    return "Circle Stop";

    case 780:
    return "Castle Grr";

    case 713:
    return "Wonderton";

    case 611:
    return "Fort Scary";

    case 705:
    return "Smallton";

    case 807:
    return "Townsville";

    case 833:
    return "Wet Fork";

    case 903:
    return "Sea Views";

    default:
    return "No Location";
  }
}
