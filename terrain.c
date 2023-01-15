#incchr(water_anim_one,"assets/water_anim_1.pcx")
#incchr(water_anim_two,"assets/water_anim_2.pcx")
#incchr(terrain_icons,"map/sprites/terrain_icons.pcx",14,2)
#incpal(t_icon_pal,"map/sprites/terrain_icons.pcx")
//#incchr(buffs,"map/sprites/buff1.pcx")
#incchr(buffs,"map/sprites/buff1.pcx",12,3)
 #incchr(chest,"map/sprites/chest.pcx")
#incpal(buff_pals,"map/sprites/buff1.pcx")

#incchr(cursor, "map/cursor.pcx");
#incpal(cursorpal, "map/cursor.pcx");

//Map imagesR
#incbin(overworldmap,"tiles/strifesisters_overworld.strifersisters.layer-Layer 1.map001.stm")
#inctilepal(overworldtilespal,"map/backgrounds/strifesisters.tiles.pcx")

#incpal(overworldpal,"map/backgrounds/default.pcx")
#inctile(overworldtiles,"map/backgrounds/default.pcx")

#incbin(battlefieldbat, "map/backgrounds/strifesisters.hcastle.layer-Layer 1.map001.stm")
#incbin(m1, "map/backgrounds/strifesisters.hcastle - Map 1.layer-Layer 1.map001.stm")
#incbin(m0, "map/backgrounds/strifesisters.hcastle - Map 2.layer-Layer 1.map001.stm")
#incbin(m2, "map/backgrounds/town1.castlbattle.layer-Layer 1.map001.stm")
#incbin(m3, "map/backgrounds/town1.castlbattle.layer-Layer 1.map001.stm")
#incbin(m4, "map/backgrounds/town1.castlbattle.layer-Layer 1.map001.stm")
#incbin(m5, "map/backgrounds/town1.castlbattle.layer-Layer 1.map001.stm")
#incbin(m6, "map/backgrounds/town1.castlbattle.layer-Layer 1.map001.stm")

#define TERRAIN_EFFECT_COUNT 5
#define NORMAL_TERRAIN 0
#define DENSE_TERRAIN 1
#define NEGATIVE_TERRAIN 2
#define BUILDING_TERRAIN 3
#define CAPTURABLE_TERRAIN 4

#define TERRAIN_TYPE_COUNT 7
#define GRASS 0
#define SAND 1
#define FOREST 2
#define ROUTE 3
#define MOUNTAIN 4
#define WATER 5
#define STRUCTURE 6

#define TERRAIN_CAP_PLAYER 1
#define TERRAIN_CAP_CPU 2
#define TERRAIN_HEAL 3
#define TERRAIN_TREASURE 4
#define TERRAIN_TRAP 5
#define TERRAIN_EVENT 6
#define MAX_EVENT_TERRAIN 10
#define MAX_TERRAIN_ITEMS 5

//#define TERRAIN_ICON_VRAM 0x4E00
#define TERRAIN_ICON_VRAM 0x5600
#define TERRAIN_ITEM_VRAM (TERRAIN_ICON_VRAM + 0x1C0)
#define TERRAIN_ICON_PAL 14

#define WATER_VRAM 0x2640
#define WATER_ANIM_ZERO (overworldtiles + 0x1640)

#define RED_CRYSTAL 0x7
#define BLUE_CRYSTAL 0xA
#define GREEN_CRYSTAL 0xD

typedef struct{
  int x, y;
  char item_no, frame;
}Terrain_Item;

const char def_bonus[TERRAIN_EFFECT_COUNT] = { 0, 10, -10, 20, 20 };
const char atk_bonus[TERRAIN_EFFECT_COUNT] = { 0, -3, -3, 0, 0 };
const char type_effect_map[TERRAIN_TYPE_COUNT] = {
  NORMAL_TERRAIN, NEGATIVE_TERRAIN, DENSE_TERRAIN,
  NORMAL_TERRAIN, DENSE_TERRAIN, NORMAL_TERRAIN, BUILDING_TERRAIN;
};
const char terrain_animation[] = { 0, 1, 0, 2 };

Terrain_Item terrain_items[MAX_TERRAIN_ITEMS];
int event_terrains[MAX_EVENT_TERRAIN];
int map_offset = 0;
char water_frame = 0;
char water_trigger = 0;
char item_trigger = 0;
char terrain_item_count = 0;

void load_terrains()
{
  load_terrain_icons();
  load_terrain_items();
  load_vram(0x4DB0,cursor,0x10);
}

void load_terrain_icons()
{
  load_vram(TERRAIN_ICON_VRAM,terrain_icons,0x1C0);
  load_palette(TERRAIN_ICON_PAL,t_icon_pal,1);
}

void load_terrain_items()
{
  load_vram(TERRAIN_ITEM_VRAM,buffs,0x240);
//  load_vram(TERRAIN_ITEM_VRAM+0x240,chest,0x40);
  load_palette(7,buff_pals,1);
}

void put_terrain_effect(char num, int x, int y, int pal)
{
  int ptr[4], address, tile_address;
  tile_address = (TERRAIN_ICON_VRAM>>4) + (num<<2) + pal;
  ptr[0] = tile_address;
  ptr[1] = tile_address+1;
  ptr[2] = tile_address+2;
  ptr[3] = tile_address+3;
  address = vram_addr(x,y);
  load_vram(address,ptr,2);
  load_vram(address+0x20,ptr+2,2);
}

void put_terrain_icon(char terrain_no, int x, int y)
{
  put_terrain_effect(terrain_no,x,y,0xE000);
}

void put_terrain_item(char item_no, int x, int y)
{
  put_terrain_effect(item_no,x<<1,y<<1,0x7000);
}

void create_terrain_item(char item_no, int x, int y)
{
  terrain_items[terrain_item_count].frame = terrain_item_count%4;//range(1,3);
  terrain_items[terrain_item_count].item_no = item_no;
  terrain_items[terrain_item_count].x = x;
  terrain_items[terrain_item_count++].y = y;
}

void put_terrain_bonus(char terrain_no, int x, int y)
{
  char bonus;
  bonus = terrain_def_bonus(terrain_effect_by_type(terrain_no));
  put_number(bonus,2,x,y);
  put_char('%',x+2,y);
}

char terrain_def_bonus(char te_type)
{
  return def_bonus[te_type];
}

char terrain_atk_bonus(char te_type)
{
  return atk_bonus[te_type];
}

char terrain_effect_by_type(char t_type)
{
  return type_effect_map[t_type];
}

char terrain_type(int t_type)
{
  if((t_type > 125 && t_type < 129) || t_type == 44) {return GRASS;}
  if(t_type > 73 && t_type < 89) {return SAND;}
  if(t_type > 44 && t_type < 60) {return FOREST;}
  if(t_type > 88 && t_type < 119) {return WATER;}
  if(t_type > 118 && t_type < 124) {return STRUCTURE;}
  if(t_type > 0 && t_type < 40) {return MOUNTAIN;}
  if(t_type > 131) {return ROUTE;}
  return ROUTE;
}

char is_traversable(int pos)
{
	int terrain_no, t_type;
	terrain_no = battlefieldbat[map_offset+pos];
  t_type = terrain_type(terrain_no);
	// if(terrain_no > 88 && terrain_no < 119)
  if(t_type == MOUNTAIN || t_type == WATER)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

void swap_water_tiles()
{
  if(water_trigger++ == 16)
  {
    switch(water_frame++)
    {
      case 0: load_vram(WATER_VRAM,WATER_ANIM_ZERO,0x3B0); break;
      case 1: load_vram(WATER_VRAM,water_anim_one,0x3B0); break;
      case 2: load_vram(WATER_VRAM,water_anim_two,0x3B0); break;
      case 3: load_vram(WATER_VRAM,water_anim_one,0x3B0); break;
    }
    water_frame %= 4;
    water_trigger = 0;
//    cycle_terrain_items();
  }
}

void cycle_terrain_items()
{
  char i;
  if(item_trigger++ == 16)
  {
    for(i=0; i<terrain_item_count; i++)
    {
      put_terrain_item(terrain_items[i].item_no+(terrain_animation[terrain_items[i].frame]),terrain_items[i].x,terrain_items[i].y);
      terrain_items[i].frame++;
      terrain_items[i].frame %= 4;
    }
    item_trigger = 0;
  }
}

void remove_terrain_item(char item_index)
{
  char i;

  put_tile(map_get_tile(terrain_items[item_index].x,terrain_items[item_index].y-2),
           terrain_items[item_index].x,terrain_items[item_index].y);

  for(i=item_index; i<terrain_item_count; i++)
  {
    memcpy(&terrain_items[i],&terrain_items[i+1],sizeof(Terrain_Item));
  }
  terrain_item_count--;
}

char item_at_position(int position)
{
  char i;
  for(i=0; i<terrain_item_count; i++)
  {
    if((terrain_items[i].y<<4)+terrain_items[i].x == position)
    {
      return i;
    }
  }
  return -1;
}