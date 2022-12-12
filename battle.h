#define MAX_GROUP_SIZE 9
#define TEAM_ONE 0
#define TEAM_TWO 9

int attacker, target, attacking_team, target_team, attacker_size, target_size,
    team_one_count, team_two_count;


int a_def_bonus, a_atk_bonus, t_def_bonus, t_atk_bonus;

char a_terrain, t_terrain;

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
  // switch(unit_entities[npcs[npc_id].type].unit_type)
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
    put_string("error mage",5,5);
    put_number(npc_count,2,13,6);
    break;

    default:
    // put_string("error default atk",5,5);
    // put_number(npcs[npc_id].type,2,5,6);
    break;
  }
}

void transfer_units_to_stun_vram(char npc_id)
{
  // switch(unit_entities[npcs[npc_id].type].unit_type)
  // put_hex(stun_vrams,5,1,18);
  // wait_for_I_input();
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
    put_string("error mage",5,5);
    break;

    default:
    // put_string("error default stn",5,5);
    // put_number(npc_id,4,5,6);
    break;
  }
}
