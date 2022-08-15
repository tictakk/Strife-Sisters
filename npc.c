#incpal(npc_pal, "characters/mannpc.pcx")
#incspr(man_walk, "characters/mannpc.pcx")
#incspr(woman_walk, "characters/womannpc.pcx")

#define MAX_NPCS 42
#define NO_OF_NPC_TYPES 4
#define NO_OF_UNIT_TYPES 12
#define NO_OF_BASIC_TYPE (NO_OF_NPC_TYPES + NO_OF_UNIT_TYPES)
#define MAX_UNIT_TYPES (NO_OF_NPC_TYPES + NO_OF_UNIT_TYPES + TOTAL_COMMANDERS)
#define UNIT_VRAM_START 0x3800

struct npc{
  unsigned char pos_x, pos_y, type, active, frame, pal;
};
const char npc_level_data[17] = {
  0xFF, 0xFF, 10, 7, MAN_NPC, 25, 8, 11, WOMAN_NPC, 25, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};

const int NPC_FRAMES[6] =
{
  0x00, 0x00, 0x00, 0x80, 0x80, 0x80
};

const char UNIT_PALS[MAX_UNIT_TYPES] = {17,17,17,17,17,21,21,17,25,25,25,25,25,25,25,25,25};

int npc_vram[MAX_UNIT_TYPES];
struct npc npcs[MAX_NPCS];
char npc_count = 0;
char npc_type_count = 0;
char cmdr_pal_count = 0;
char npc_cmdr_count = 0;

void init_npcs()
{
  npc_count = 0;
  npc_type_count = 0;
  npc_cmdr_count = 0;
  cmdr_pal_count = 26;

  load_palette(17,sldpal,2);
  load_palette(19,dmnpal,2);
  load_palette(21,blobpal,2);
  load_palette(23,blobpal,2);
  load_palette(25,npc_pal,2);
}

void clear_npcs()
{
  char i;
  init_npcs();
  for(i=0; i<MAX_UNIT_TYPES; i++)
  {
    npc_vram[i] = 0;
  }
}

// void load_npcs(int level)
void load_npcs(char *data)
{
  int i;
  char x, y, type, level_counter, pal;
  level_counter = 0;
  i = 0;
  for(;;)
  {
    x = data[i++];
    if(x == -1)
    {
      break;
    }
    else
    {
      y = data[i++];
      type = data[i++];
      pal  = data[i++];
      add_npc(x,y,type,pal);
    }
  }
}

void add_npc(char x, char y, char type, char pal)
{
  if(npc_count < MAX_NPCS)
  {
    npcs[npc_count].pos_x = x;
    npcs[npc_count].pos_y = y;
    npcs[npc_count].type = type;
    npcs[npc_count].pal = pal;
    npcs[npc_count++].frame = 0;
    if(npc_vram[type] == 0)
    {
      npc_vram[type] = UNIT_VRAM_START + (npc_type_count * 0x100);
      npc_type_count++;

      if(type < NO_OF_BASIC_TYPE)
      {
        switch(type)
        {
          case SWORD_UNIT:
          load_vram(npc_vram[type],sld,0x100);
          break;

          case SPEAR_UNIT:
          load_vram(npc_vram[type],spr,0x100);
          break;

          case ARCHER_UNIT:
          load_vram(npc_vram[type],arch0,0x100);
          break;

          case DEMON_UNIT:
          load_vram(npc_vram[type],dmn,0x100);
          break;

          case HOUND_UNIT:
          load_vram(npc_vram[type],hnd,0x100);
          break;

          case BLOB_UNIT:
          load_vram(npc_vram[type],blob,0x100);
          break;

          case MAN_NPC:
          load_vram(npc_vram[type],man_walk,0x100);
          break;

          case WOMAN_NPC:
          load_vram(npc_vram[type],woman_walk,0x100);
          break;

          case BANDIT_UNIT:
          case MAGE_UNIT:
          case BOY_NPC:
          case GIRL_NPC:
          default:
          put_string("error",5,5);
          break;
        }
      }
      else
      {
        put_string("not loading blob",10,5);
        put_number(type,3,10,6);
        load_commanders_gfx(type,npc_vram[type],pal);
      }
    }
  }
}

void draw_npcs(char sprite_offset)
{
  char i;
  int x, y;
  for(i=0; i<npc_count; i++)
  {
    x = npcs[i].pos_x * 16;
    y = npcs[i].pos_y * 16;
    draw_npc(sprite_offset+i,x,y,i);
  }
}

void draw_npc(char sprite_no, int x, int y, char index)
{
  int pattern;
  char frame;

  pattern = npc_vram[npcs[index].type];//NPC_VRAM_START + (npcs[index].type * 0x100);
  npcs[index].frame = (npcs[index].frame + 1) % 6;
  frame = npcs[index].frame;
  pattern += NPC_FRAMES[frame];

  spr_make(sprite_no,x,y,pattern,FLIP_MAS|SIZE_MAS,SZ_16x32,npcs[index].pal,1);
}

char check_collision(int x, int y)
{
  char i;
  for(i=0; i<npc_count; i++)
  {
    if(npcs[i].pos_x * 16 == x && npcs[i].pos_y * 16 == y)
    {
      return npcs[i].type;
    }
  }
  return -1;
}
