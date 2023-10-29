#incbin(map_metadata,"maps/mapout")

// #define MAP_METADATA_SIZE 117 //I think
#define MAP_METADATA_SIZE 175 //I think

#define MAP_RESULT_NONE 1
#define MAP_RESULT_WIN 2
#define MAP_RESULT_LOSE 3

//''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
//'                        Map meta data structure                           '
//''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
// [ @area, map id | map x | map y | map type | p_cmdr_pos | c_cmdr_pos | c_cmdr_ids | event_pos | map_items | item_positions]
//     1       1       1       1        1           16           40           80          16           6             12
//                                 181 byte total
// #define MAP_
char ai_objective = 0;
unsigned char map_x = 0;
unsigned char map_y = 0;
unsigned char map_tile_count = 0;
unsigned char map_type = 5;
char cpu_cmdr_count = 0;

struct battle_map_data{
  int player_start_pos[8], cpu_start_pos[20];
  char cpu_commander_ids[80];
  int event_positions[8];
  char map_items[6];
  int item_positions[6];
};

// unsigned char raw_map_data[MAP_METADATA_SIZE];
struct battle_map_data battle_map_metadata;
int capture_position;
//F, D, C, B, A, S
// const int map_1_grades[] = {600, 800, 1000, 1200, 1400, 1600};
// const int map_2_grades[] = {1000, 1100, 1200, 1300, 1400, 1500};
// const int map_3_grades[] = {1000, 1100, 1200, 1300, 1400, 1500};
const char grades[] = {70,68,67,66,65,83};
int *map_grades;

void init_map_data(int map_id)
{
  int offset;
  int i, j;

  cpu_cmdr_count = 0;
  offset = ((int)map_id) * MAP_METADATA_SIZE;

  // memcpy(&battle_map_metadata,&map_metadata[offset+5],MAP_METADATA_SIZE-5);
  // for(i=0; i<MAP_METADATA_SIZE; i++, offset++)
  // {
  //   raw_map_data[i] = (char)map_metadata[offset];
  // }
  ai_objective = map_metadata[offset+1];//raw_map_data[1];
  map_x = map_metadata[offset+2];//raw_map_data[2];
  map_y = map_metadata[offset+3];//raw_map_data[3];
  map_type = map_metadata[offset+4];//raw_map_data[4];

  // offset+=5;
  // offset+=1;
  offset+=5 ;

  load_meta_data_int(battle_map_metadata.player_start_pos,offset,8);
  load_meta_data_int(battle_map_metadata.cpu_start_pos,offset+16,20);
  load_meta_data_char(battle_map_metadata.cpu_commander_ids,offset+56,80);
  load_meta_data_int(battle_map_metadata.event_positions,offset+136,8);
  load_meta_data_char(battle_map_metadata.map_items,offset+152,6);
  load_meta_data_int(battle_map_metadata.item_positions,offset+158,6);
  // memcpy(battle_map_metadata.player_start_pos,)
  // memcpy(&battle_map_metadata,raw_map_data+5,MAP_METADATA_SIZE-5);

  for(i=0; i<20; i++)
  {
    if(battle_map_metadata.cpu_commander_ids[i*4+1] != 0)
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
  char i, grade;
  grade = 70;
  for(i=0; i<6; i++)
  {
    if(score>=map_grades[i])
    {
      // return grades[i];
      grade = grades[i];
    }
  }
  return grade;
}