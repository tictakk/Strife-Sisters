#define MAX_GROUP_SIZE 9
#define TEAM_ONE 0
#define TEAM_TWO 9

// typedef struct{
//   char x,y,healthbars, to_kill;
//   int  atk_total, def_total, hp_total,
//        hp_current, current_army_size,
//        atk_buff, def_buff, army_start_size;
//   enum Unit_Type unit_type;
// } Army;
//
// Army p_c_army;
// Army c_c_army;

int attacker, target, attacking_team, target_team, attacker_size, target_size,
    team_one_count, team_two_count;

const int ATTACK_ANIMATIONS[11] = {
                                  0x000, 0x00, 0x00,
                                  0x100, //0x100,
                                  0x200, //0x200,
                                  0x300, 0x300,
                                  0x400, 0x400, 0x400,
                                  0x400
                                };

int idle_vrams[MAX_GROUP_SIZE*2] = {
                                    0x1500,0x1600,0x1700,
                                    0x1800,0x1900,0x1A00,
                                    0x1B00,0x1C00,0x1D00,

                                    0x1E00,0x1F00,0x2000,
                                    0x2100,0x2200,0x2300,
                                    0x2400,0x2500,0x2600
                                    };

int attack_vrams[MAX_GROUP_SIZE] = {
                                    0x2A00,0x2F00,0x3400,
                                    0x3900,0x3E00,0x4300,
                                    0x4800,0x4D00,0x5200
                                    };

int stun_vrams[MAX_GROUP_SIZE] = {
                                  0x5700,0x5800,0x5900,
                                  0x5A00,0x5B00,0x5C00,
                                  0x5D00,0x5E00,0x5F00
                                  };

void add_battle_npc(char x, char y, char entity_id, char pal, char index)
{
    int p_x = 0, p_y = 0;
    npcs[index].pos_x = x;
    npcs[index].pos_y = y;
    npcs[index].type = entity_id;
    npcs[index].active = 1;
    npcs[index].frame = 0;

    // put_number(unit_entities[entity_id].unit_type,3,0,index);
    switch(unit_entities[entity_id].unit_type)
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
    }
    p_x = (int)(x << 5);
    p_y = (int)(y << 5);
    spr_make(index,((p_x/4)*5)+xOffset,((p_y/4)*5)-16,idle_vrams[index],FLIP_MAS|SIZE_MAS,SZ_32x32,npcs[index].pal,1);
}

void transfer_units_to_attack_vram(char npc_id)
{
  switch(unit_entities[npcs[npc_id].type].unit_type)
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
    put_string("error default",5,5);
    put_number(npcs[npc_id].type,2,13,9);
    break;
  }
}

void transfer_units_to_stun_vram(char npc_id)
{
  switch(unit_entities[npcs[npc_id].type].unit_type)
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
    put_string("error default",5,5);
    break;
  }
}

void reduce_npc_ids(unsigned char entity_id)
{
  unsigned char i;
  // if(entity_id == 17)
  // {
  //   put_number(commanders[2].row_one[0],3,29,3);
  //   put_number(commanders[2].row_one[1],3,29,4);
  //   put_number(commanders[2].row_one[2],3,29,5);
  //   wait_for_I_input();
  // }
  for(i=0; i<18; i++)
  {
    if(npcs[i].type > entity_id)
    {
      npcs[i].type--;
    }
  }
  // if(entity_id == 17)
  // {
  //   put_number(commanders[2].row_one[0],3,29,3);
  //   put_number(commanders[2].row_one[1],3,29,4);
  //   put_number(commanders[2].row_one[2],3,29,5);
  //   wait_for_I_input();
  // }
}
