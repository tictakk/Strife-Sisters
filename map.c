#incbin(map_metadata,"maps/mapout")

#define MAP_METADATA_SIZE 92 //I think

//''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
//'                        Map meta data structure                           '
//''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
// [ @area, map id | map x | map y | p_cmdr_pos | c_cmdr_pos | c_cmdr_ids | event_pos ]
//     1       1       1       1         12           30           30          16
//                                 92 byte total

unsigned char map_x = 0;
unsigned char map_y = 0;
unsigned char map_tile_count = 0;
char cpu_cmdr_count = 0;

struct battle_map_data{
  int player_start_pos[6], cpu_start_pos[15];
  int cpu_commander_ids[15];
  int event_positions[10];
  int map_no;
  int pos;
};

unsigned char raw_map_data[MAP_METADATA_SIZE];
struct battle_map_data battle_map_metadata;

void init_map_data(int map_id)
{
  int offset;
  int i;
  put_number(map_id,4,15,15);
  cpu_cmdr_count = 0;
  offset = ((int)map_id) * MAP_METADATA_SIZE;
  for(i=0; i<MAP_METADATA_SIZE; i++, offset++)
  {
    raw_map_data[i] = (char)map_metadata[offset];
  }

  map_x = raw_map_data[2];
  map_y = raw_map_data[3];
  join_byte_list_positions(raw_map_data+4,battle_map_metadata.player_start_pos,12);
  join_byte_list_positions(raw_map_data+4+12,battle_map_metadata.cpu_start_pos,30);
  join_byte_list_positions(raw_map_data+4+12+30,battle_map_metadata.cpu_commander_ids,30);
  join_byte_list_positions(raw_map_data+4+12+30+30,battle_map_metadata.event_positions,16);
  // join_byte_list(raw_map_data+4+12+30,battle_map_metadata.cpu_commander_ids);
  // join_byte_list(raw_map_data+4+12+30+30,battle_map_metadata.event_positions);

  for(i=0; i<15; i++)
  {
    if(battle_map_metadata.cpu_commander_ids[i] != 0)
    {
      cpu_cmdr_count++;
    }
  }
}
// void join_byte_list_positions(unsigned char bytes[60], int ints[30])
void join_byte_list_positions(unsigned char *bytes, int *ints, char ints_size)
{
  char i;
  for(i=0; i<ints_size; i++)
  {
    ints[i] = (bytes[i*2] + ((bytes[(i*2)+1]) << 8));
  }
}

void join_byte_list(unsigned char bytes[12], int ints[6])
{
  char i;
  for(i=0; i<6; i++)
  {
    ints[i] = (bytes[i*2] + ((bytes[(i*2)+1]) << 8));
  }
}
