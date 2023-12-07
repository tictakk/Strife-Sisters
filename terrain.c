#incchr(water_anim_one,"assets/water_anim_1.pcx")
#incchr(water_anim_two,"assets/water_anim_2.pcx")
#incchr(terrain_icons,"map/sprites/terrain_icons.pcx",14,2)
#incpal(t_icon_pal,"map/sprites/terrain_icons.pcx")
#incchr(square,"assets/bigger_square.pcx")
#incchr(square_extra,"assets/square_extra.pcx")
#incpal(square_pal,"assets/bigger_square.pcx")

#incchr(cursor, "map/cursor.pcx");
#incpal(cursorpal, "map/cursor.pcx");
#incchr(run_button,"map/sprites/run_button.pcx")

// #incspr(chest,"map/sprites/chest.pcx")
// #incpal(chest_pal,"map/sprites/chest.pcx")

// #incpal(overworldpal,"map/backgrounds/new_assets/strifemaps.tiles.pcx")
// #inctile(overworldtiles,"map/backgrounds/new_assets/strifemaps.tiles.pcx")
#incpal(overworldpal,"/Users/matthewkersey/Documents/overworld.pcx")
// #inctile(overworldtiles,"/Users/matthewkersey/Desktop/tutorial_maps.pcx")
#inctile(overworldtiles,"/Users/matthewkersey/Documents/overworld.pcx")

#incbin(tutorial_1,"/Users/matthewkersey/Desktop/tutorial_maps.strifesisterstiles.layer-Layer 1.map001.stm")//1
#incbin(tutorial_2,"/Users/matthewkersey/Desktop/tutorial_maps.strifesisterstiles.layer-Layer 1.map002.stm")//2
#incbin(battlefieldbat,"/Users/matthewkersey/Desktop/tutorial_maps.strifesisterstiles.layer-Layer 1.map011.stm")//3
#incbin(m0, "/Users/matthewkersey/Desktop/tutorial_maps.strifesisterstiles.layer-Layer 1.map012.stm")//4
#incbin(m1, "/Users/matthewkersey/Desktop/tutorial_maps.strifesisterstiles.layer-Layer 1.map013.stm")//5
#incbin(m3, "/Users/matthewkersey/Desktop/tutorial_maps.strifesisterstiles.layer-Layer 1.map014.stm")//6
#incbin(m2, "/Users/matthewkersey/Desktop/tutorial_maps.strifesisterstiles.layer-Layer 1.map015.stm")//7
#incbin(m4, "/Users/matthewkersey/Desktop/tutorial_maps.strifesisterstiles.layer-Layer 1.map003.stm")//8
#incbin(m5, "/Users/matthewkersey/Desktop/tutorial_maps.strifesisterstiles.layer-Layer 1.map004.stm")//9
#incbin(m6, "/Users/matthewkersey/Desktop/tutorial_maps.strifesisterstiles.layer-Layer 1.map005.stm")//10
#incbin(m7, "/Users/matthewkersey/Desktop/tutorial_maps.strifesisterstiles.layer-Layer 1.map006.stm")//11
#incbin(m8, "/Users/matthewkersey/Desktop/tutorial_maps.strifesisterstiles.layer-Layer 1.map007.stm")//12
#incbin(m9, "/Users/matthewkersey/Desktop/tutorial_maps.strifesisterstiles.layer-Layer 1.map008.stm")//13
#incbin(m10, "/Users/matthewkersey/Desktop/tutorial_maps.strifesisterstiles.layer-Layer 1.map009.stm")//14
#incbin(m11, "/Users/matthewkersey/Desktop/tutorial_maps.strifesisterstiles.layer-Layer 1.map010.stm")//15
// #incbin(m12, "map/backgrounds/new_assets/strifemaps.Strifemaps.layer-Layer 1.map014.stm")//16
// #incbin(m13, "map/backgrounds/new_assets/strifemaps.Strifemaps.layer-Layer 1.map015.stm")//17
// #incbin(m14, "map/backgrounds/new_assets/strifemaps.Strifemaps.layer-Layer 1.map016.stm")//18
// #incbin(m15, "map/backgrounds/new_assets/strifemaps.Strifemaps.layer-Layer 1.map017.stm")//19

//Map images
// #incbin(overworldmap,"tiles/strifesisters_overworld.strifersisters.layer-Layer 1.map001.stm")
// #incbin(overworldmap,"map/backgrounds/strifesisters_overworld.strifersisters.layer-Layer 1.map001.stm")
#incbin(overworldmap,"/Users/matthewkersey/Documents/overworld.overworld.layer-Layer 1.map001.stm")

// #inctilepal(overworldtilespal,"map/backgrounds/new_assets/strifemaps.tiles.pcx")
#inctilepal(overworldtilespal,"/Users/matthewkersey/Documents/overworld.pcx")

#define TERRAIN_EFFECT_COUNT 5
#define NORMAL_TERRAIN 0
#define DENSE_TERRAIN 1
#define NEGATIVE_TERRAIN 2
#define BUILDING_TERRAIN 3
#define CAPTURABLE_TERRAIN 4

#define TERRAIN_TYPE_COUNT 8
#define GRASS 0
#define SAND 1
#define FOREST 2
#define ROUTE 3
#define MOUNTAIN 4
#define WATER 5
#define STRUCTURE 6
#define WALL 7

//#define TERRAIN_ICON_VRAM 0x4E00
#define TERRAIN_ICON_VRAM 0x5600
#define TERRAIN_ITEM_VRAM 0x5800//(TERRAIN_ICON_VRAM + 0x1C0)
#define TERRAIN_ICON_PAL 8

#define SQUARE_1 0xE5B0
#define SQUARE_2 0xD500

#define WATER_VRAM 0x2680
#define WATER_ANIM_ZERO (overworldtiles + 0x1680)

const char def_bonus[TERRAIN_TYPE_COUNT] = { 0, -10, 10, 0, 20, 0, 25, 0 };
const char terrain_animation[] = { 0, 1, 0, 2 };

int map_offset = 0;
char water_frame = 0;
char water_trigger = 0;

void load_terrains()
{
  load_terrain_icons();
  load_vram(0x4DB0,cursor,0x10);
}

void load_terrain_icons()
{
  load_vram(TERRAIN_ICON_VRAM,terrain_icons,0x1C0);
  load_vram(0x5B00,square,0x600);
  load_vram(0x5000,square,0x600);
  load_palette(TERRAIN_ICON_PAL,t_icon_pal,1);
  // load_palette(14,square_pal,1);
  load_palette(14,square_pal,2);
  // load_palette(13,square_pal,1);
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
  if(terrain_no == WALL)
  {
    put_terrain_effect(TERRAIN_ICON_VRAM,STRUCTURE,x,y,TERRAIN_ICON_PAL);
  }
  else
  {
    put_terrain_effect(TERRAIN_ICON_VRAM,terrain_no,x,y,TERRAIN_ICON_PAL);
  }
}

char terrain_def_bonus(char te_type)
{
  return def_bonus[te_type];
}

char terrain_type(int t_type)
{
  if(t_type < 30 || (t_type > 70 && t_type < 74)) {return GRASS;} 
  if(t_type > 29 && t_type < 46) {return MOUNTAIN;}
  if(t_type > 74 && t_type < 90) {return SAND;}
  if(t_type > 45 && t_type < 61) {return FOREST;}
  if( (t_type > 60 && t_type < 71) || (t_type > 151 && t_type < 163) ) {return WALL;}
  if(t_type > 89 && t_type < 120) {return WATER;}
  if(t_type > 119 && t_type < 129) {return STRUCTURE; }
  // if(t_type > 157) { return WALL; }
  // if(t_type > 123 && t_type < 126){ return WALL;}
  // if(t_type > 131) {return ROUTE;}
  return ROUTE;
}

char is_traversable(int pos)
{
	int terrain_no, t_type;
	terrain_no = tutorial_1[map_offset+pos];
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
  }
}