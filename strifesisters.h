#incbin(script, "script/out")
#define NO_OF_MAPS 30
#define SWORD_UNIT 0
#define SPEAR_UNIT 1
#define ARCHER_UNIT 2
#define MAGE_UNIT 3
#define DEMON_UNIT 4
#define HOUND_UNIT 5
#define BLOB_UNIT 6
#define AXE_UNIT 7
#define PALADIN 8
#define MONK 9
#define SNPIER 10
#define LANCER 11
#define MAN_NPC 12
#define WOMAN_NPC 13
#define BOY_NPC 14
#define GIRL_NPC 15
#define REI 16
#define VIOLET 17
#define KING 18
#define ALUCARD 19
#define KAIN 20
#define SACHA 21
#define ALMER 22
#define VEN 23
#define PHALC 24
#define KURT 25
#define SWORD_CMDR 35
#define SPEAR_CMDR 36
#define ARCH_CMDR 37
#define GENERIC_CMDR 38
#define HOUND_CMDR 39
#define BLOB_CMDR 40

#define ROW_ONE 0
#define ROW_TWO 1
#define ROW_THREE 2
#define MAX_UNITS_PER_ROW 3

const int EXP_CHART[10] = //ten levels max
{
  50, 150, 350, 700, 1500, 2500, 5000, 10000, 25000, 50000
};

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
