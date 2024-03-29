// #incpal(npc_pal, "characters/mannpc.pcx")
// #incspr(man_walk, "characters/mannpc.pcx")
// #incspr(woman_walk, "characters/womannpc.pcx")

// #define MAX_NPCS 42
#define MAX_NPCS 20
#define NO_OF_BASIC_TYPE 29
#define MAX_UNIT_TYPES (NO_OF_BASIC_TYPE + TOTAL_COMMANDERS)
#define UNIT_VRAM_START 0x3A00//0x5200

#define ALLY_PALETTE 17
#define ENEMY_PALETTE 18
#define CMDR_PALETTE 0

struct npc{
  unsigned char pos_x, pos_y, type, frame, pal; //active
};

const int NPC_FRAMES[6] = { 0x00, 0x00, 0x00, 0x40, 0x40, 0x40 };

int npc_vram[MAX_UNIT_TYPES];
struct npc npcs[MAX_NPCS];
char npc_count = 0;
char npc_type_count = 0;
char cmdr_pal_count = 0;
char current_frame = 0;
char highlight_pal_no = 0;

void init_npcs()
{
  npc_count = 0;
  npc_type_count = 0;
  cmdr_pal_count = 26;
  current_frame = 0;

  load_palette(17,soldierpal,1);
  load_palette(18,enemypal,1);
  load_palette(19,dmnpal,1);
}

void clear_npcs()
{
  int i;
  for(i=0; i<MAX_UNIT_TYPES; i++)
  {
    npc_vram[i] = 0;
  }
}

void hide_npcs(char offset)
{
  char i;
  // for(i=0; i<npc_count; i++)
  for(i=offset; i<64; i++)
  {
    // put_number(i,4,0,0);
    // wait_for_I_input();
    spr_hide(i);
  }
}

void show_npcs(char offset)
{
  char i;
  for(i=0; i<npc_count; i++)
  {
    spr_show(offset+i);
  }
}

void reset_npcs()
{
  init_npcs();
  clear_npcs();
}

void add_npc(unsigned char x, unsigned char y, char type, char pal)
{
  if(npc_count < MAX_NPCS)
  {
    // put_string("    ",s_x_relative,s_y_relative);
    // put_number(npc_count,4,s_x_relative,s_y_relative);
    // wait_for_I_input();

    npcs[npc_count].pos_x = x;
    npcs[npc_count].pos_y = y;
    npcs[npc_count].type = type;
    npcs[npc_count].pal = pal;
    // npcs[npc_count].active = 1;
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
          load_vram(npc_vram[type],blobsmall,0x100);
          break;

          case STALKER_UNIT:
          load_vram(npc_vram[type],stlk,0x100);
          break;

          case CLERIC_UNIT:
          load_vram(npc_vram[type],mag,0x100);
          break;

          case WITCH_UNIT:
          load_vram(npc_vram[type],wtc,0x100);
          break;

          case RAIDER_UNIT:
          load_vram(npc_vram[type],bnd,0x100);
          break;

          case GOLEM_UNIT:
          load_vram(npc_vram[type],gol,0x100);
          break;

          case BERSERKER_UNIT:
          load_vram(npc_vram[type],brk,0x100);
          break;

          case BRAWLER_UNIT:
          load_vram(npc_vram[type],mnk,0x100);
          break;

          case LANCER_UNIT:
          load_vram(npc_vram[type],lan,0x100);
          break;

          case SNIPER_UNIT:
          load_vram(npc_vram[type],snipersmall,0x100);
          break;

          case DANCER_UNIT:
          load_vram(npc_vram[type],dan,0x100);
          break;  

          case KNIGHT_UNIT:
          load_vram(npc_vram[type],knightsmall,0x100);
          break;

          default:
          load_vram(npc_vram[type],sld,0x100);
          put_string("error default",5,5);
          put_number(type,3,5,6);
          wait_for_I_input();
          break;
        }
      }
      else
      {
        // put_hex(npc_vram[type],5,0,0);
        // put_number(type,4,7,0);
        // wait_for_I_input();
        npcs[npc_count-1].pal = get_commander_palette(type);//cmdr_pal_count;
        load_commanders_gfx(type,npc_vram[type]);//,cmdr_pal_count++);
      }
    }
  }
}

void increment_frame()
{
  current_frame = (current_frame + 1) % 6;
}

void draw_npcs(char sprite_offset)
{
  char i;
  int x, y;
  // increment_frame();
  // for(i=63; i>63-npc_count; i--)
  for(i=0;i<npc_count;i++)
  {
    x = npcs[i].pos_x << 4;
    y = npcs[i].pos_y << 4;
    draw_npc(63-i,x,y-16,i);
  }
}

void draw_npc(char sprite_no, int x, int y, char index)
{
  int pattern;
  pattern = npc_vram[npcs[index].type];
  spr_make(sprite_no,x,y+yOffset,pattern+NPC_FRAMES[current_frame],FLIP_MAS|SIZE_MAS,SZ_16x32,npcs[index].pal,1);
}

void destroy_npc(char index)
{
  char i;
  for(i=index; i<npc_count; i++)
  {
    memcpy(&npcs[i],&npcs[i+1],sizeof(struct npc));
  }
  npc_count--;
}
