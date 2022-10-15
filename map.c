#incbin(map_metadata,"maps/mapout")

#define MAP_METADATA_SIZE 152 //I think

unsigned char map_x = 0;
unsigned char map_y = 0;
unsigned char map_tile_count = 0;
char cpu_cmdr_count = 0;

struct battle_map_data{
  int player_start_pos[30], cpu_start_pos[30];
  int cpu_commander_ids[6];
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

  cpu_cmdr_count = 0;
  offset = ((int)map_id) * MAP_METADATA_SIZE;
  for(i=0; i<MAP_METADATA_SIZE; i++, offset++)
  {
    raw_map_data[i] = (char)map_metadata[offset];
  }

  map_x = raw_map_data[2];
  map_y = raw_map_data[3];
  join_byte_list_positions(raw_map_data+4,battle_map_metadata.player_start_pos);
  join_byte_list_positions(raw_map_data+64,battle_map_metadata.cpu_start_pos);
  join_byte_list(raw_map_data+124,battle_map_metadata.cpu_commander_ids);
  join_byte_list(raw_map_data+136,battle_map_metadata.event_positions);

  for(i=0; i<6; i++)
  {
    if(battle_map_metadata.cpu_commander_ids[i] != 0)
    {
      cpu_cmdr_count++;
    }
  }

  // battle_map_metadata[0].player_start_pos[0] = 71;
  // battle_map_metadata[0].player_start_pos[1] = 51;
  // battle_map_metadata[0].player_start_pos[2] = 60;
  //
  // battle_map_metadata[0].cpu_start_pos[0] = 199;
  // battle_map_metadata[0].cpu_start_pos[1] = 210;
  // battle_map_metadata[0].cpu_start_pos[2] = 218;
  // battle_map_metadata[0].map_no = 0;

  // battle_map_metadata[1].player_start_pos[0] = 71;
  // battle_map_metadata[1].player_start_pos[1] = 51;
  // battle_map_metadata[1].player_start_pos[2] = 60;
  //
  // battle_map_metadata[1].cpu_start_pos[0] = 199;
  // battle_map_metadata[1].cpu_start_pos[1] = 210;
  // battle_map_metadata[1].cpu_start_pos[2] = 219;
  // battle_map_metadata[1].map_no = 0;
  //
  // battle_map_metadata[2].player_start_pos[0] = 71;
  // battle_map_metadata[2].player_start_pos[1] = 51;
  // battle_map_metadata[2].player_start_pos[2] = 60;
  //
  // battle_map_metadata[2].cpu_start_pos[0] = 199;
  // battle_map_metadata[2].cpu_start_pos[1] = 210;
  // battle_map_metadata[2].cpu_start_pos[2] = 219;
  // battle_map_metadata[2].map_no = 2;

  // battle_map_metadata[0].pos = 994;
  // battle_map_metadata[1].pos = 903;
  // battle_map_metadata[2].pos = 833;
  // battle_map_metadata[3].pos = 807;
  // battle_map_metadata[4].pos = 705;
  // battle_map_metadata[5].pos = 713;
}
void join_byte_list_positions(unsigned char bytes[60], int ints[30])
{
  char i;
  for(i=0; i<30; i++)
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
