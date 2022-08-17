#incbin(script, "script/out")
#define NO_OF_MAPS 30
#define SWORD_UNIT 0
#define SPEAR_UNIT 1
#define ARCHER_UNIT 2
#define MAGE_UNIT 3
#define DEMON_UNIT 4
#define HOUND_UNIT 5
#define BLOB_UNIT 6
#define BANDIT_UNIT 7
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
#define CMDR_FOUR 19
#define CMDR_FIVE 20
#define CMDR_SIX 21

const int EXP_CHART[10] = //ten levels max
{
  50, 150, 350, 700, 1500, 2500, 5000, 10000, 25000, 50000
};

enum battle_map_type{
  CASTLE, VILLAGE, WILD, QUEST
};

struct battle_map_data{
  unsigned char player_start_pos[3], cpu_start_pos[3];
  int map_no;
  int pos;
  enum battle_map_type map_type;
};

struct battle_map_data battle_map_metadata[NO_OF_MAPS];

char *location_lookup(int position)
{
  switch(position)
  {

    case 994:
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

void init_map_data()
{
  char i;

  battle_map_metadata[0].player_start_pos[0] = 71;
  battle_map_metadata[0].player_start_pos[1] = 51;
  battle_map_metadata[0].player_start_pos[2] = 60;

  battle_map_metadata[0].cpu_start_pos[0] = 199;
  battle_map_metadata[0].cpu_start_pos[1] = 210;
  battle_map_metadata[0].cpu_start_pos[2] = 218;
  battle_map_metadata[0].map_type = WILD;
  battle_map_metadata[0].map_no = 0;

  battle_map_metadata[1].player_start_pos[0] = 71;
  battle_map_metadata[1].player_start_pos[1] = 51;
  battle_map_metadata[1].player_start_pos[2] = 60;

  battle_map_metadata[1].cpu_start_pos[0] = 199;
  battle_map_metadata[1].cpu_start_pos[1] = 210;
  battle_map_metadata[1].cpu_start_pos[2] = 219;
  battle_map_metadata[1].map_type = WILD;
  battle_map_metadata[1].map_no = 1;

  battle_map_metadata[2].player_start_pos[0] = 71;
  battle_map_metadata[2].player_start_pos[1] = 51;
  battle_map_metadata[2].player_start_pos[2] = 60;

  battle_map_metadata[2].cpu_start_pos[0] = 199;
  battle_map_metadata[2].cpu_start_pos[1] = 210;
  battle_map_metadata[2].cpu_start_pos[2] = 219;
  battle_map_metadata[2].map_type = VILLAGE;
  battle_map_metadata[2].map_no = 2;

  battle_map_metadata[0].pos = 903;
  battle_map_metadata[1].pos = 833;
  battle_map_metadata[2].pos = 807;
  battle_map_metadata[3].pos = 705;
  battle_map_metadata[4].pos = 713;

  battle_map_metadata[3].map_no = 3;
  battle_map_metadata[4].map_no = 4;
  battle_map_metadata[5].map_no = 5;
  battle_map_metadata[6].map_no = 6;
  battle_map_metadata[7].map_no = 7;
}
