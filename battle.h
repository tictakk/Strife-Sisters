#define MAX_GROUP_SIZE 9
#define TEAM_ONE 0
#define TEAM_TWO 9

//#define FFA 0
//#define TARGET_UNIT 1
//#define LOWEST_DEF_UNIT 2
//#define HIGHEST_ATK_UNIT 3
//#define LOWEST_HP_UNIT 4
//#define HEARLER_UNIT 5

#define IDLE 0
#define ATTACKED 1
#define TARGETED 2
#define DYING 3

typedef struct {
    unsigned char ent_id, active, frame, pal, state, target_id, target_ent_id;
} BattleUnit;

const char bottom_row_attack_chart[9] = { 3, 1, 1, 4, 2, 2, 4, 2, 2};
const char middle_row_attack_chart[9] = { 1, 1, 1, 2, 2, 2, 2, 2, 2};
const char top_row_attack_chart[9]    = { 1, 1, 3, 2, 2, 4, 2, 2, 4};

int target, attacking_team, target_team, attacker_size, target_size,
	team_one_count, team_two_count;

int a_def_bonus, a_atk_bonus, t_def_bonus, t_atk_bonus;

char a_terrain, t_terrain, team_one_strategy, team_two_strategy, clock_divider, battle_clock, attacking, attacker;

BattleUnit battleunits[18];

const int ATTACK_ANIMATIONS[11] = {
								  0x000, 0x00, 0x00,
								  0x100, //0x100,
								  0x200, //0x200,
								  0x300, 0x300,
								  0x400, 0x400, 0x400,
								  0x400
								};

const int idle_vrams[MAX_GROUP_SIZE*2] = {
									0x1500,0x1600,0x1700,
									0x1800,0x1900,0x1A00,
									0x1B00,0x1C00,0x1D00,

									0x1E00,0x1F00,0x2000,
									0x2100,0x2200,0x2300,
									0x2400,0x2500,0x2600
									};

const int attack_vrams[MAX_GROUP_SIZE] = {
									0x2A00,0x2F00,0x3400,
									0x3900,0x3E00,0x4300,
									0x4800,0x4D00,0x5200
									};

const int stun_vrams[MAX_GROUP_SIZE] = {
								  0x5700,0x5800,0x5900,
								  0x5A00,0x5B00,0x5C00,
								  0x5D00,0x5E00,0x5F00
								  };

void add_battle_npc(char x, char y, char entity_id, char pal, char index, char active)
{
  int p_x = 0, p_y = 0;

  npcs[index].pos_x = x;
  npcs[index].pos_y = y;
  npcs[index].type = entity_id;
  npcs[index].active = active;
  npcs[index].frame = 0;

  if(active==0)
  {
    return;
  }

  switch(entity_id)
  {
    case BLOB_UNIT:
      load_vram(idle_vrams[index],blobbattle,0x100);
      npcs[index].pal = 16;
      break;

    case SWORD_UNIT:
      load_vram(idle_vrams[index],attack,0x100);
      npcs[index].pal = 17;
      break;

    case SPEAR_UNIT:
      load_vram(idle_vrams[index],attack2,0x100);
      npcs[index].pal = 18;
      break;

    case ARCHER_UNIT:
      load_vram(idle_vrams[index],musketbtl,0x100);
      npcs[index].pal = 19;
      break;

    case DEMON_UNIT:
      load_vram(idle_vrams[index],demonbtl,0x100);
      npcs[index].pal = 20;
      break;

    case HOUND_UNIT:
      load_vram(idle_vrams[index],houndbtl,0x100);
      npcs[index].pal = 21;
      break;

    case AXE_UNIT:
      load_vram(idle_vrams[index],bandit,0x100);
      npcs[index].pal = 22;
      break;

    case MAGE_UNIT:
      load_vram(idle_vrams[index],magebtl,0x100);
      npcs[index].pal = 23;
      break;

    case KNIGHT_UNIT:
      load_vram(idle_vrams[index],knightbtl,0x100);
      npcs[index].pal = 18;
      break;

    case LANCER_UNIT:
      load_vram(idle_vrams[index],lancerbtl,0x100);
      npcs[index].pal = 18;
      break;

    case MONK_UNIT:
      load_vram(idle_vrams[index],monkbtl,0x100);
      npcs[index].pal = 24;
      break;

    default:
      put_string("error",5,5);
      put_number(entity_id,3,5,6);
      break;
  }
  p_x = (int)(x << 5);
  p_y = (int)(y << 5);

  if(active)
  {
	  spr_make(index,((p_x/4)*5)+xOffset,((p_y/4)*5)-16,idle_vrams[index],FLIP_MAS|SIZE_MAS,SZ_32x32,npcs[index].pal,1);
  }
}

void transfer_units_to_attack_vram(char npc_id)
{
  switch(npcs[npc_id].type)
  {
    case BLOB_UNIT:
      load_vram(attack_vrams[npc_id%9],blobbattle+0x300,0x500);
      break;

    case SWORD_UNIT:
      load_vram(attack_vrams[npc_id%9],attack+0x300,0x500);
      break;

    case SPEAR_UNIT:
      load_vram(attack_vrams[npc_id%9],attack2+0x300,0x500);
      break;

    case ARCHER_UNIT:
      load_vram(attack_vrams[npc_id%9],musketbtl+0x300,0x500);
      break;

    case DEMON_UNIT:
      load_vram(attack_vrams[npc_id%9],demonbtl+0x300,0x500);
      break;

    case HOUND_UNIT:
      load_vram(attack_vrams[npc_id%9],houndbtl+0x300,0x500);
      break;

    case AXE_UNIT:
      load_vram(attack_vrams[npc_id%9],bandit+0x300,0x500);
      break;

    case MAGE_UNIT:
      load_vram(attack_vrams[npc_id%9],magebtl+0x300,0x500);
      break;

    case KNIGHT_UNIT:
      load_vram(attack_vrams[npc_id%9],knightbtl+0x300,0x500);
      break;

    case MONK_UNIT:
      load_vram(attack_vrams[npc_id%9],monkbtl+0x300,0x500);
      break;

    case LANCER_UNIT:
      load_vram(attack_vrams[npc_id%9],lancerbtl+0x300,0x500);
      break;

    default:
    // put_string("error default atk",5,5);
    // put_number(npcs[npc_id].type,2,5,6);
    break;
  }
}

void transfer_units_to_stun_vram(char npc_id)
{
  switch(npcs[npc_id].type)
  {
    case BLOB_UNIT:
      load_vram(stun_vrams[npc_id%9],blobbattle+0x900,0x100);
      break;

    case SWORD_UNIT:
      load_vram(stun_vrams[npc_id%9],attack+0x900,0x100);
      break;

    case SPEAR_UNIT:
      load_vram(stun_vrams[npc_id%9],attack2+0x900,0x100);
      break;

    case ARCHER_UNIT:
      load_vram(stun_vrams[npc_id%9],musketbtl+0x900,0x100);
      break;

    case DEMON_UNIT:
      load_vram(stun_vrams[npc_id%9],demonbtl+0x900,0x100);
      break;

    case HOUND_UNIT:
      load_vram(stun_vrams[npc_id%9],houndbtl+0x900,0x100);
      break;

    case AXE_UNIT:
      load_vram(stun_vrams[npc_id%9],bandit+0x900,0x100);
      break;

    case MAGE_UNIT:
      load_vram(stun_vrams[npc_id%9],magebtl+0x900,0x100);
      //	put_string("error mage",5,5);
      break;

    case BLOB_UNIT:
      load_vram(stun_vrams[npc_id%9],blobbattle+0x900,0x100);
      break;

    case SWORD_UNIT:
      load_vram(stun_vrams[npc_id%9],attack+0x900,0x100);
      break;

    case SPEAR_UNIT:
      load_vram(stun_vrams[npc_id%9],attack2+0x900,0x100);
      break;

    case ARCHER_UNIT:
      load_vram(stun_vrams[npc_id%9],musketbtl+0x900,0x100);
      break;

    case DEMON_UNIT:
      load_vram(stun_vrams[npc_id%9],demonbtl+0x900,0x100);
      break;

    case HOUND_UNIT:
      load_vram(stun_vrams[npc_id%9],houndbtl+0x900,0x100);
      break;

    case AXE_UNIT:
      load_vram(stun_vrams[npc_id%9],bandit+0x900,0x100);
      break;

    case MAGE_UNIT:
      load_vram(stun_vrams[npc_id%9],magebtl+0x900,0x100);
      //	put_string("error mage",5,5);
      break;

    case KNIGHT_UNIT:
      load_vram(stun_vrams[npc_id%9],knightbtl+0x900,0x100);
      break;

    case LANCER_UNIT:
      load_vram(stun_vrams[npc_id%9],knightbtl+0x900,0x100);
      break;

    case MONK_UNIT:
      load_vram(stun_vrams[npc_id%9],monkbtl+0x900,0x100);
      break;

    default:
//       put_string("error default stn",5,5);
//       put_number(npc_id,4,5,6);
      break;
  }
}

char find_target_unit(char attacking_unit_position)
{
  char attackable_count;
  char attackble_units[9];
  attackable_count = 0;

  attackable_count = find_attackable_units(attacking_unit_position,attackble_units);

  if(attackable_count == 1)
  {
    return attackble_units[0];
  }
  return attackble_units[range(0,attackable_count-1)];
}

char find_attackable_units(char unit_position, char attackble_units[9])
{
  char i;
  char *a_chart;
  char attackble_chart[9];
  char attackble_count, attack_tier;

  attack_tier = 5;
  attackble_count = 0;

  a_chart = get_attack_row_chart(unit_position);
  memcpy(attackble_chart,a_chart,9);
  modify_attack_chart(attackble_chart);

  for(i=0; i<9; i++)
  {
    if(npcs[target_team+i].active && attackble_chart[i] < attack_tier)
    {
      attack_tier = attackble_chart[i];
      attackble_count = 0;
    }
    if(npcs[target_team+i].active && attackble_chart[i] == attack_tier)
    {
      attackble_units[attackble_count++] = i;
    }
  }
  return attackble_count;
}

char* get_attack_row_chart(char unit_position)
{
  switch(unit_position)
  {
    case 2:
    case 5:
    case 8:
//      put_string("bot",0,15);
//      put_number(unit_position,3,0,14);
//      wait_for_I_input();
      return bottom_row_attack_chart;

    case 1:
    case 4:
    case 7:
//      put_string("mid",0,15);
//      put_number(unit_position,3,0,14);
//      wait_for_I_input();
      return middle_row_attack_chart;

    case 0:
    case 3:
    case 6:
//      put_string("top",0,15);
//      put_number(unit_position,3,0,14);
//      wait_for_I_input();
      return top_row_attack_chart;
  }
  return middle_row_attack_chart;
}

void modify_attack_chart(char *attack_chart)
{
  char i;
  for(i=0; i<3; i++)
  {
    if(!npcs[target_team+i].active)
    {
      attack_chart[i+3]--;
      attack_chart[i+6]--;
    }
  }

}