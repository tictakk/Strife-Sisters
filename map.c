#incbin(map_metadata,"maps/mapout")

#define MAP_METADATA_SIZE 141 //I think

//''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
//'                        Map meta data structure                           '
//''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
// [ @area, map id | map x | map y | p_cmdr_pos | c_cmdr_pos | c_cmdr_ids | event_pos ]
//     1       1       1       1         24           48           48          16
//                                 141 byte total

char ai_objective = 0;
unsigned char map_x = 0;
unsigned char map_y = 0;
unsigned char map_tile_count = 0;
unsigned char map_type = 5;
char cpu_cmdr_count = 0;

struct battle_map_data{
  int player_start_pos[12], cpu_start_pos[24];
  int cpu_commander_ids[24];
  int event_positions[8];
  int map_obj;
  int pos;
};

unsigned char raw_map_data[MAP_METADATA_SIZE];
struct battle_map_data battle_map_metadata;
int capture_position;

void init_map_data(int map_id)
{
  int offset;
  int i;

  cpu_cmdr_count = 0;
  offset = ((int)map_id) * MAP_METADATA_SIZE;
  for(i=0; i<MAP_METADATA_SIZE; i++, offset++)
  {
    raw_map_data[i] = (char)map_metadata[offset];
  }

  ai_objective = raw_map_data[1];
  map_x = raw_map_data[2];
  map_y = raw_map_data[3];
  map_type = raw_map_data[4];
  // join_byte_list_positions(raw_map_data+5,battle_map_metadata.player_start_pos,24);
  // join_byte_list_positions(raw_map_data+(5+24),battle_map_metadata.cpu_start_pos,24);
  // join_byte_list_positions(raw_map_data+(5+24+48),battle_map_metadata.cpu_commander_ids,48);
  // join_byte_list_positions(raw_map_data+(5+24+48+48),battle_map_metadata.event_positions,16);

  memcpy(&battle_map_metadata,raw_map_data+5,MAP_METADATA_SIZE-5);

  for(i=0; i<24; i++)
  {
    if(battle_map_metadata.cpu_commander_ids[i] != 0)
    {
      cpu_cmdr_count++;
    }
  }
  battle_map_metadata.map_obj = raw_map_data[1];
}

void join_byte_list_positions(unsigned char *bytes, int *ints, int ints_size)
{
  int i;
  for(i=0; i<ints_size; i++)
  {
    ints[i] = (bytes[i*2] + ((bytes[(i*2)+1]) << 8));
  }
}
