#incbin(map_metadata,"maps/mapout")

// #define MAP_METADATA_SIZE 117 //I think
#define MAP_METADATA_SIZE 235 //I think

#define MAP_RESULT_NONE 1
#define MAP_RESULT_WIN 2
#define MAP_RESULT_LOSE 3

//''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
//'                        Map meta data structure                           '
//''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
// [ @area, map id | map x | map y | map type | p_cmdr_pos | c_cmdr_pos | c_cmdr_ids | event_pos | map_items | item_positions]
//     1       1       1       1        1           16           40           80          16           6             12
//                                 181 byte total

const char S = 100;
const char A = 90;
const char B = 80;
const char C = 70;
const char D = 60;

char ai_objective = 0;
unsigned char map_x = 0;
unsigned char map_y = 0;
unsigned char map_tile_count = 0;
unsigned char map_type = 0;
char cpu_cmdr_count = 0;

struct battle_map_data{
  int player_start_pos[8],cpu_start_pos[15];
  char cpu_commander_ids[150];
  int event_positions[8];
  char map_items[6];
  int item_positions[6];
};
// unsigned char raw_map_data[MAP_METADATA_SIZE];
struct battle_map_data battle_map_metadata;
//F, D, C, B, A, S
const char grades[] = {83,65,66,67,68,70};
const char map_grades[] = {
  7, 9, 11,
  22, 18, 15,
  15, 15, 19,
  19, 17, 15,
  15, 15, 15
};

const char map_turn_bonus[] = {
  2, 5, 3, 
  5, 7, 11,
  11, 8, 6,
  12, 14, 9,
  4, 15, 15
};
// int *map_grades;

void init_map_data(int map_id)
{
  int offset;
  int i;

  cpu_cmdr_count = 0;
  offset = ((int)map_id) * MAP_METADATA_SIZE;

  ai_objective = map_metadata[offset+1];//raw_map_data[1];
  map_x = map_metadata[offset+2];//raw_map_data[2];
  map_y = map_metadata[offset+3];//raw_map_data[3];
  map_type = map_metadata[offset+4];//raw_map_data[4];

  // offset+=5;
  // offset+=1;
  offset+=5;

  load_meta_data_int(battle_map_metadata.player_start_pos,offset,8);
  load_meta_data_int(battle_map_metadata.cpu_start_pos,offset+16,15);
  load_meta_data_char(battle_map_metadata.cpu_commander_ids,offset+46,150);
  load_meta_data_int(battle_map_metadata.event_positions,offset+196,8);
  load_meta_data_char(battle_map_metadata.map_items,offset+212,6);
  load_meta_data_int(battle_map_metadata.item_positions,offset+218,6);
  // memcpy(battle_map_metadata.player_start_pos,)
  // memcpy(&battle_map_metadata,raw_map_data+5,MAP_METADATA_SIZE-5);

  for(i=0; i<15; i++)
  {
    if(battle_map_metadata.cpu_commander_ids[i*10] != 0)
    {
      cpu_cmdr_count++;
    }
  }
  // map_grades = map_1_grades + (map_id*6);
  // debug_number = cpu_cmdr_count;
}

void load_meta_data_int(int *field, int offset, int len)
{
  int i;
  for(i=0; i<len; i++)
  {
    field[i] = (map_metadata[offset+(i*2)] & 0xFF);
    field[i] += (map_metadata[offset+(i*2+1)] << 8);
  }
}

void load_meta_data_char(char *field, int offset, int len)
{
  int i;
  for(i=0; i<len; i++)
  {
    field[i] = (char)map_metadata[offset+i];
  }
}

char get_map_grade_result(char map_no, int score)
{
  unsigned int i, grade;
  grade = get_percentage(score/100,map_grades[map_no]);
  // grade = ;
  if(grade>=S)
  {
    return grades[0];
  }else if(grade>=A)
  {
    return grades[1];
  }else if(grade>=B)
  {
    return grades[2];
  }else if(grade>=C)
  {
    return grades[3];
  }
  else if(grade>=D)
  {
    return grades[4];
  }else
  {
    return grades[5];
  }
}