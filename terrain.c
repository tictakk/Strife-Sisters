#incchr(water_anim_one,"assets/water_anim_1.pcx")
#incchr(water_anim_two,"assets/water_anim_2.pcx")
#incchr(terrain_icons,"map/sprites/terrain_icons.pcx",14,2)
#incpal(t_icon_pal,"map/sprites/terrain_icons.pcx")

#incspr(buffs,"map/sprites/gems.pcx")
#incspr(chest,"map/sprites/chest.pcx")
#incpal(chest_pal,"map/sprites/chest.pcx")

#incchr(cursor, "map/cursor.pcx");
#incpal(cursorpal, "map/cursor.pcx");
#incchr(run_button,"map/sprites/run_button.pcx")
// #inctilepal(overworldtilespal,"map/backgrounds/strifesisters.tiles.pcx")

// #incpal(overworldpal,"map/backgrounds/default.pcx")
// #inctile(overworldtiles,"map/backgrounds/default.pcx")
#incpal(overworldpal,"map/backgrounds/new_assets/strifemaps.tiles.pcx")
#inctile(overworldtiles,"map/backgrounds/new_assets/strifemaps.tiles.pcx")

#incbin(battlefieldbat,"map/backgrounds/new_assets/strifemaps.Strifemaps.layer-Layer 1.map001.stm")
#incbin(m0, "map/backgrounds/new_assets/strifemaps.Strifemaps.layer-Layer 1.map002.stm")
#incbin(m1, "map/backgrounds/new_assets/strifemaps.Strifemaps.layer-Layer 1.map004.stm")
#incbin(m3, "map/backgrounds/new_assets/strifemaps.Strifemaps.layer-Layer 1.map005.stm")
#incbin(m2, "map/backgrounds/new_assets/strifemaps.Strifemaps.layer-Layer 1.map003.stm")
#incbin(m4, "map/backgrounds/new_assets/strifemaps.Strifemaps.layer-Layer 1.map006.stm")
#incbin(m5, "map/backgrounds/new_assets/strifemaps.Strifemaps.layer-Layer 1.map007.stm")
#incbin(m6, "map/backgrounds/new_assets/strifemaps.Strifemaps.layer-Layer 1.map008.stm")
#incbin(m7, "map/backgrounds/new_assets/strifemaps.Strifemaps.layer-Layer 1.map009.stm")
#incbin(m8, "map/backgrounds/new_assets/strifemaps.Strifemaps.layer-Layer 1.map010.stm")
#incbin(m9, "map/backgrounds/new_assets/strifemaps.Strifemaps.layer-Layer 1.map011.stm")
#incbin(m10, "map/backgrounds/new_assets/strifemaps.Strifemaps.layer-Layer 1.map012.stm")
#incbin(m11, "map/backgrounds/new_assets/strifemaps.Strifemaps.layer-Layer 1.map013.stm")
#incbin(m12, "map/backgrounds/new_assets/strifemaps.Strifemaps.layer-Layer 1.map014.stm")
#incbin(m13, "map/backgrounds/new_assets/strifemaps.Strifemaps.layer-Layer 1.map015.stm")
#incbin(m14, "map/backgrounds/new_assets/strifemaps.Strifemaps.layer-Layer 1.map016.stm")
#incbin(m15, "map/backgrounds/new_assets/strifemaps.Strifemaps.layer-Layer 1.map017.stm")
#incbin(m16, "map/backgrounds/new_assets/strifemaps.Strifemaps.layer-Layer 1.map018.stm")
// #incbin(m17, "map/backgrounds/new_assets/strifemaps.Strifemaps.layer-Layer 1.map019.stm")
// #incbin(m18, "map/backgrounds/new_assets/strifemaps.Strifemaps.layer-Layer 1.map020.stm")
// #incbin(m19, "map/backgrounds/new_assets/strifemaps.Strifemaps.layer-Layer 1.map021.stm")
// #incbin(m20, "map/backgrounds/new_assets/strifemaps.Strifemaps.layer-Layer 1.map022.stm")
// #incbin(m21, "map/backgrounds/new_assets/strifemaps.Strifemaps.layer-Layer 1.map023.stm")
// #incbin(m22, "map/backgrounds/new_assets/strifemaps.Strifemaps.layer-Layer 1.map024.stm")
// #incbin(m23, "map/backgrounds/new_assets/strifemaps.Strifemaps.layer-Layer 1.map025.stm")

//Map images
#incbin(overworldmap,"tiles/strifesisters_overworld.strifersisters.layer-Layer 1.map001.stm")
#inctilepal(overworldtilespal,"map/backgrounds/new_assets/strifemaps.tiles.pcx")

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
#define WALL 7

#define TERRAIN_CAP_PLAYER 1
#define TERRAIN_CAP_CPU 2
#define TERRAIN_HEAL 3
#define TERRAIN_TREASURE 4
#define TERRAIN_TRAP 5
#define TERRAIN_EVENT 6
#define MAX_EVENT_TERRAIN 10
#define MAX_TERRAIN_ITEMS 6

//#define TERRAIN_ICON_VRAM 0x4E00
#define TERRAIN_ICON_VRAM 0x5600
#define TERRAIN_ITEM_VRAM 0x5800//(TERRAIN_ICON_VRAM + 0x1C0)
#define TERRAIN_ICON_PAL 8

#define WATER_VRAM 0x2640
#define WATER_ANIM_ZERO (overworldtiles + 0x1640)

#define RED_CRYSTAL 0x1
#define BLUE_CRYSTAL 0x2
#define GREEN_CRYSTAL 0x3
#define CHEST 0x4
#define COIN 0x5

typedef struct{
  int x, y;
  char item_no, pal;
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

//CHEST -> Gold, used for units and upgrades?
//BLUE GEM -> Meter boost
//RED GEM -> Currency to upgrade units
//GREEN GEM -> Curreny to upgrade stones
//GOLD GEM -> Calling stone

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
  load_vram(TERRAIN_ITEM_VRAM,buffs,0xC0);
  load_vram(TERRAIN_ITEM_VRAM+0xC0,chest,0x80);
  load_palette(30,chest_pal,1);
}

void put_terrain_effect(int vram, char num, int x, int y, char pal)
{
  int ptr[4], address, tile_address;
  tile_address = (vram>>4) + (num<<2) + (pal<<12);
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
  put_terrain_effect(TERRAIN_ICON_VRAM,terrain_no,x,y,TERRAIN_ICON_PAL);
}

void put_terrain_item(char item_no, int x, int y, char pal)
{
  // put_terrain_effect(TERRAIN_ITEM_VRAM,item_no-1,x<<1,y<<1,pal);
  spr_make(item_no+1,x<<4,(y<<4)-s_y,TERRAIN_ITEM_VRAM+((terrain_items[item_no].item_no-1)<<6),0,0,30,1);
  spr_show();
}

void create_terrain_item(char item_no, int x, int y, char pal)
{
  // terrain_items[terrain_item_count].frame = terrain_item_count%4;//range(1,3);
  terrain_items[terrain_item_count].pal = (item_no>GREEN_CRYSTAL)? 15 : 14;
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
  if(t_type > 123 && t_type < 126){ return WALL;}
  if(t_type > 0 && t_type < 40) {return MOUNTAIN;}
  if(t_type == 154) {return RED_CRYSTAL;}
  if(t_type == 155) {return BLUE_CRYSTAL;}
  if(t_type == 156) {return GREEN_CRYSTAL;}
  if(t_type == 157) {return CHEST;}
  // if(t_type > 131) {return ROUTE;}
  return ROUTE;
}

char is_traversable(int pos)
{
	int terrain_no, t_type;
	terrain_no = battlefieldbat[map_offset+pos];
  t_type = terrain_type(terrain_no);
	// if(terrain_no > 88 && terrain_no < 119)
  if(t_type == MOUNTAIN || t_type == WATER || t_type == WALL)
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
  for(i=0; i<terrain_item_count; i++)
  {
    put_terrain_item(i,terrain_items[i].x,terrain_items[i].y,terrain_items[i].pal);
  }
}

void remove_terrain_item(char item_index)
{
  char i;
  //leaving this here to understand how put_tile and map_get_tile is used properly
  // put_tile(map_get_tile(terrain_items[item_index].x,terrain_items[item_index].y-2),
          //  terrain_items[item_index].x,terrain_items[item_index].y);

  for(i=item_index; i<terrain_item_count; i++)
  {
    memcpy(&terrain_items[i],&terrain_items[i+1],sizeof(Terrain_Item));
  }
  spr_hide(item_index+1);
  terrain_item_count--;
}

void remove_terrain_items()
{
  char i;
  for(i=0; i<terrain_item_count; i++)
  {
    remove_terrain_item(i);
  }
  terrain_item_count = 0;
}

char item_at_position(int position)
{
  char i;
  for(i=0; i<terrain_item_count; i++)
  {
    if(((terrain_items[i].y<<4)+terrain_items[i].x) == position)
    {
      return i;
    }
  }
  return -1;
}