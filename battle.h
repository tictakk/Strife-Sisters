#incbin(battlemap,"tiles/battletiles.battle_backgrounds.layer-Layer 1.map001.stm")
#inctilepal(battletilespal,"tiles/battletiles.tiles.pcx")
#inctile(battletiles,"tiles/battletiles.tiles.pcx")
#incpal(battlepal,"tiles/battletiles.tiles.pcx")

#define MAX_GROUP_SIZE 9
#define TEAM_ONE 0
#define TEAM_TWO 9

#define IDLE 0
#define ATTACK 1
#define STUNNED 2
#define PARALYZED 3
#define DYING 4
#define WAITING 5
#define HEALING 6
#define METER_ATTACK 8

#define STATUS_NORMAL 0
#define STATUS_DAZED 1
#define STATUS_CONFUSED 2
#define STATUS_RAGE 3
#define STATUS_BLOWBACK 4
#define STATE_ACID 5

typedef struct {
  char ent_id, active, frame, pal, state, target_team, pos, attacks, 
       target, meter, status, a_bonus, d_bonus;
  Unit_Entity *unit;
}BattleUnit;

const char top_row_attack_chart[9]    = { 1, 1, 3, 2, 4, 5, 4, 5, 5};
const char middle_row_attack_chart[9] = { 1, 1, 1, 2, 3, 2, 4, 5, 4};
const char bottom_row_attack_chart[9] = { 3, 1, 1, 5, 4, 2, 5, 4, 4};

// char attacker_art_list[MAX_ARMY_SIZE];
// char target_art_list[MAX_ARMY_SIZE];

char atker, trgt, meter_queued = 0, unit_meter_queued = 0, position_status = 0;//FINISH THIS STUFF

int team_one_count, team_two_count;

int a_def_bonus, a_atk_bonus, t_def_bonus, t_atk_bonus;

char a_terrain, t_terrain, attack_range, 
     battle_clock, animating, attacker, stun_count;

char team_one_judgement=0, team_two_judgement=0, clear_eyes_called=0, sea_legs_art=0;
char team_one_tracked=0, team_two_tracked=0, team_one_tracking=0, team_two_tracking=0;
char art_queued = 0, art_unit_id = 0, art_queue_id = 0;

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

void add_battle_unit(char x, char y, char entity_id, char index, char active,
                     char enemy_ent_id, char position, Unit_Entity *ue)
{
  int p_x = 0, p_y = 0;

  battleunits[index].active = active;
  battleunits[index].ent_id = entity_id;
  battleunits[index].target_team = enemy_ent_id;
  battleunits[index].frame = 0;
  battleunits[index].state = IDLE;
  battleunits[index].pos = position;
  battleunits[index].attacks = 1;
  battleunits[index].target = 0;
  battleunits[index].unit = ue;
  battleunits[index].meter = 0;
  battleunits[index].a_bonus = 0;
  battleunits[index].d_bonus = 0;
  
  if(entities[entity_id].bg->units[position]->unit.rng < attack_range)
  {
    battleunits[index].attacks = 0;
  }

  switch(ue->unit->id)
  {
    case BLOB_UNIT:
      load_vram(idle_vrams[index],blobbattle,0x100);
      battleunits[index].pal = 28;
      break;

    case SWORD_UNIT:
      load_vram(idle_vrams[index],attack,0x100);
      battleunits[index].pal = 17;
      break;

    case SPEAR_UNIT:
      load_vram(idle_vrams[index],attack2,0x100);
      battleunits[index].pal = 18;
      break;

    case STALKER_UNIT:
      load_vram(idle_vrams[index],stalkerbtl,0x100);
      battleunits[index].pal = 27;
      break;

    case ARCHER_UNIT:
      load_vram(idle_vrams[index],musketbtl,0x100);
      battleunits[index].pal = 19;
      break;
    
    case SNIPER_UNIT:
      load_vram(idle_vrams[index],sniperbtl,0x100);
      battleunits[index].pal = 28;
      break;

    case BERSERKER_UNIT:
      load_vram(idle_vrams[index],berserkerbtl,0x100);
      battleunits[index].pal = 28;
      break;

    case DEMON_UNIT:
      load_vram(idle_vrams[index],demonbtl,0x100);
      battleunits[index].pal = 20;
      break;

    case HOUND_UNIT:
      load_vram(idle_vrams[index],houndbtl,0x100);
      battleunits[index].pal = 21;
      break;

    case AXE_UNIT:
      load_vram(idle_vrams[index],axebtl,0x100);
      battleunits[index].pal = 22;
      break;

    case RAIDER_UNIT:
      load_vram(idle_vrams[index],raiderbtl,0x100);
      battleunits[index].pal = 25;
      break;

    case CLERIC_UNIT:
      load_vram(idle_vrams[index],magebtl,0x100);
      battleunits[index].pal = 24;
      break;

    case MAGE_UNIT:
      load_vram(idle_vrams[index],magebtl,0x100);
      battleunits[index].pal = 23;
      break;

    case WITCH_UNIT:
      load_vram(idle_vrams[index],witchbtl,0x100);
      battleunits[index].pal = 29;
      break;

    case PRIEST_UNIT:
      load_vram(idle_vrams[index],priestbtl,0x100);
      battleunits[index].pal = 29;
      break;

    case BLACK_MAGE_UNIT:
      load_vram(idle_vrams[index],magebtl,0x100);
      battleunits[index].pal = 26;
      break;

    case PALADIN_UNIT:
      load_vram(idle_vrams[index],paladinbtl,0x100);
      battleunits[index].pal = 28;
      break;

    case KNIGHT_UNIT:
      load_vram(idle_vrams[index],knightbtl,0x100);
      battleunits[index].pal = 18;
      break;

    case LANCER_UNIT:
      load_vram(idle_vrams[index],lancerbtl,0x100);
      battleunits[index].pal = 18;
      break;

    case MONK_UNIT:
      load_vram(idle_vrams[index],monkbtl,0x100);
      battleunits[index].pal = 24;
      break;

    case FIGHTER_UNIT:
      load_vram(idle_vrams[index],fighterbtl,0x100);
      battleunits[index].pal = 24;
      break;

    case BRAWLER_UNIT:
      load_vram(idle_vrams[index],brawlerbtl,0x100);
      battleunits[index].pal = 24;
      break;

    case GOLEM_UNIT:
      load_vram(idle_vrams[index],golembtl,0x100);
      battleunits[index].pal = 28;
      break;

    default:
      put_string("errorz",5,5);
      // put_number(entity_id,3,5,6);
      break;
  }
  p_x = (int)(x << 5);
  p_y = (int)(y << 5);

  if(active)
  {
	  spr_make(index+5,((p_x/4)*5)+xOffset,((p_y/4)*5)-16,idle_vrams[index],FLIP_MAS|SIZE_MAS,SZ_32x32,battleunits[index].pal,1);
  }
}

void transfer_units_to_attack_vram(char type)
{
  switch(type)
  {
    case BLOB_UNIT:
//      load_vram(attack_vrams[npc_id%9],blobbattle+0x300,0x500);
      load_vram(attack_vrams[0],blobbattle+0x300,0x500);
      break;

    case SWORD_UNIT:
      load_vram(attack_vrams[0],attack+0x300,0x500);
      break;

    case SPEAR_UNIT:
      load_vram(attack_vrams[0],attack2+0x300,0x500);
      break;

    case STALKER_UNIT:
      load_vram(attack_vrams[0],stalkerbtl+0x300,0x500);
      break;

    case ARCHER_UNIT:
      load_vram(attack_vrams[0],musketbtl+0x300,0x500);
      break;

    case SNIPER_UNIT:
      load_vram(attack_vrams[0],sniperbtl+0x300,0x500);
      break;

    case BERSERKER_UNIT:
      load_vram(attack_vrams[0],berserkerbtl+0x300,0x500);
      break;

    case GOLEM_UNIT:
      load_vram(attack_vrams[0],golembtl+0x300,0x500);
      break;

    case DEMON_UNIT:
      load_vram(attack_vrams[0],demonbtl+0x300,0x500);
      break;

    case HOUND_UNIT:
      load_vram(attack_vrams[0],houndbtl+0x300,0x500);
      break;

    case AXE_UNIT:
      load_vram(attack_vrams[0],axebtl+0x300,0x500);
      break;

    case RAIDER_UNIT:
      load_vram(attack_vrams[0],raiderbtl+0x300,0x500);
      break;

    case WITCH_UNIT:
      load_vram(attack_vrams[0],witchbtl+0x300,0x500);
      break;

    case PRIEST_UNIT:
      load_vram(attack_vrams[0],priestbtl+0x300,0x500);
      break;

    case BLACK_MAGE_UNIT:
    case CLERIC_UNIT:
    case MAGE_UNIT:
      load_vram(attack_vrams[0],magebtl+0x300,0x500);
      break;

    case KNIGHT_UNIT:
      load_vram(attack_vrams[0],knightbtl+0x300,0x500);
      break;

    case PALADIN_UNIT:
      load_vram(attack_vrams[0],paladinbtl+0x300,0x500);
      break;

    case FIGHTER_UNIT:
      load_vram(attack_vrams[0],fighterbtl+0x300,0x500);
      break;

    case BRAWLER_UNIT:
      load_vram(attack_vrams[0],brawlerbtl+0x300,0x500);
      break;

    case MONK_UNIT:
      load_vram(attack_vrams[0],monkbtl+0x300,0x500);
      break;

    case LANCER_UNIT:
      load_vram(attack_vrams[0],lancerbtl+0x300,0x500);
      break;

    default:
    // put_string("error default atk",5,5);
    // put_number(npcs[npc_id].type,2,5,6);
    break;
  }
}

void transfer_units_to_stun_vram(char type, char index)
{
  switch(type)
  {
    case BLOB_UNIT:
//      load_vram(stun_vrams[npc_id%9],blobbattle+0x900,0x100);
      load_vram(stun_vrams[index],blobbattle+0x900,0x100);
      break;

    case SWORD_UNIT:
      load_vram(stun_vrams[index],attack+0x900,0x100);
      break;

    case SPEAR_UNIT:
      load_vram(stun_vrams[index],attack2+0x900,0x100);
      break;

    case STALKER_UNIT:
      load_vram(stun_vrams[index],stalkerbtl+0x900,0x100);
      break;

    case ARCHER_UNIT:
      load_vram(stun_vrams[index],musketbtl+0x900,0x100);
      break;

    case SNIPER_UNIT:
      load_vram(stun_vrams[index],sniperbtl+0x900,0x100);
      break;

    case BERSERKER_UNIT:
      load_vram(stun_vrams[index],berserkerbtl+0x900,0x100);
      break;

    case GOLEM_UNIT:
      load_vram(stun_vrams[index],golembtl+0x900,0x100);
      break;

    case DEMON_UNIT:
      load_vram(stun_vrams[index],demonbtl+0x900,0x100);
      break;

    case HOUND_UNIT:
      load_vram(stun_vrams[index],houndbtl+0x900,0x100);
      break;

    case AXE_UNIT:
      load_vram(stun_vrams[index],axebtl+0x900,0x100);
      break;

    case WITCH_UNIT:
      load_vram(stun_vrams[index],witchbtl+0x900,0x100);
      break;

    case PRIEST_UNIT:
      load_vram(stun_vrams[index],priestbtl+0x900,0x100);
      break;

    case BLACK_MAGE_UNIT:
    case CLERIC_UNIT:
    case MAGE_UNIT:
      load_vram(stun_vrams[index],magebtl+0x900,0x100);
      //	put_string("error mage",5,5);
      break;

    case BLOB_UNIT:
      load_vram(stun_vrams[index],blobbattle+0x900,0x100);
      break;

    case SWORD_UNIT:
      load_vram(stun_vrams[index],attack+0x900,0x100);
      break;

    case SPEAR_UNIT:
      load_vram(stun_vrams[index],attack2+0x900,0x100);
      break;

    case STALKER_UNIT:
      load_vram(stun_vrams[index],stalkerbtl+0x900,0x100);
      break;

    case ARCHER_UNIT:
      load_vram(stun_vrams[index],musketbtl+0x900,0x100);
      break;

    case SNIPER_UNIT:
      load_vram(stun_vrams[index],sniperbtl+0x900,0x100);
      break;

    case GOLEM_UNIT:
      load_vram(stun_vrams[index],golembtl+0x900,0x100);
      break;

    case DEMON_UNIT:
      load_vram(stun_vrams[index],demonbtl+0x900,0x100);
      break;

    case HOUND_UNIT:
      load_vram(stun_vrams[index],houndbtl+0x900,0x100);
      break;

    case AXE_UNIT:
      load_vram(stun_vrams[index],axebtl+0x900,0x100);
      break;

    case BLACK_MAGE_UNIT:
    case CLERIC_UNIT:
    case MAGE_UNIT:
      load_vram(stun_vrams[index],magebtl+0x900,0x100);
      //	put_string("error mage",5,5);
      break;

    case KNIGHT_UNIT:
      load_vram(stun_vrams[index],knightbtl+0x900,0x100);
      break;

    case PALADIN_UNIT:
      load_vram(stun_vrams[index],paladinbtl+0x900,0x100);
      break;

    case LANCER_UNIT:
      load_vram(stun_vrams[index],knightbtl+0x900,0x100);
      break;

    case FIGHTER_UNIT:
      load_vram(stun_vrams[index],fighterbtl+0x900,0x100);
      break;

    case BRAWLER_UNIT:
      load_vram(stun_vrams[index],brawlerbtl+0x900,0x100);
      break;

    case MONK_UNIT:
      load_vram(stun_vrams[index],monkbtl+0x900,0x100);
      break;

    case RAIDER_UNIT:
      load_vram(stun_vrams[index],raiderbtl+0x900,0x100);
      break;

    default:
       put_string("error default",5,5);
       put_number(type,3,19,5);
      //  print_unit_fullname(type,19,5);
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
  if(attackable_count == 0)
  {
    battle_clock = -1;
    return 0;
  }
  return attackble_units[range(0,attackable_count-1)];
}

char find_attackable_units(char unit_position, char attackble_units[9])
{
  char i;
  char *a_chart;
  char attackble_chart[9];
  char attackble_count,attack_tier;
  char b_unit_team;
  b_unit_team = battleunits[unit_position].ent_id;

  attack_tier = 5;
  attackble_count = 0;

  a_chart = get_attack_row_chart(battleunits[unit_position].pos);
  memcpy(attackble_chart,a_chart,9);
  modify_attack_chart(attackble_chart,battleunits[unit_position].target_team);

  for(i=0; i<18; i++)
  {
    if(battleunits[i].ent_id != b_unit_team)
    {
      if(battleunits[i].active && attackble_chart[battleunits[i].pos] < attack_tier)
      {
        attack_tier = attackble_chart[battleunits[i].pos];
        attackble_count = 0;
      }
      if(battleunits[i].active && attackble_chart[battleunits[i].pos] == attack_tier)
      {
        attackble_units[attackble_count++] = i;
      }
    }
  }
  return attackble_count;
}

char get_opposing_team_id(char team)
{
  char i;
  for(i=0; i<18; i++)
  {
    if(battleunits[i].ent_id != team)
    {
      return battleunits[i].ent_id;
    }
  }
  return -1;
}

char* get_attack_row_chart(char unit_position)
{
  switch(unit_position)
  {
    case 2:
    case 5:
    case 8:
      return bottom_row_attack_chart;

    case 1:
    case 4:
    case 7:
      return middle_row_attack_chart;

    case 0:
    case 3:
    case 6:
      return top_row_attack_chart;
  }
  return middle_row_attack_chart;
}

void modify_attack_chart(char *attack_chart, char target_team)
{
  char i;
  for(i=0; i<3; i++)
  {
    if(!battleunits[target_team+i].active)
    {
      attack_chart[i+3]--;
      attack_chart[i+6]--;
    }
  }
}

char get_first_target()
{
  char i;
  for(i=0; i<18; i++)
  {
    if(battleunits[i].target)
    {
      return i;
    }
  }
  return -1;
}

void determine_action(char b_id, char target_type)
{
  switch(target_type)
  {
    case SINGLE_HIT:
      set_unit_attack(b_id);
      target_single_unit(b_id);
      break;
    case MULTI_ROW:
      set_unit_attack(b_id);
      target_multi_row(b_id);
      break;
    case MULTI_COL_2:
      set_unit_attack(b_id);
      target_multi_col(b_id,2);
      break;
    case MULTI_COL_3:
      set_unit_attack(b_id);
      target_multi_col(b_id,3);
      break;
    case MULTI_ATTACK_AOE:
      set_unit_attack(b_id);
      attack_multi_aoe(b_id);
      break;
    case HEAL:
      set_unit_heal(b_id);
      heal_single_unit(b_id);
      break;
    case MULTI_HEAL_AOE:
      set_unit_heal(b_id);
      heal_multi_aoe(b_id);
      break;
    case ALL_OPPONENTS:
      set_unit_attack(b_id);
      target_opponents(b_id);
      // transfer_units_to_attack_vram(b_id);
      break;
    case ALL_ALLIES:
      set_unit_attack(b_id);
      target_allies(b_id);
      break;
    default:
      set_unit_attack(b_id);
      target_single_unit(b_id);
      break;
  }
}

void heal_multi_aoe(char b_id)
{
  char target;
  target = find_lowest_hp(b_id,battleunits[b_id].ent_id);

  target_aoe(target,battleunits[b_id].ent_id,IDLE,1,0);
  load_animations_to_vram(battleunits[b_id].unit->unit->id);
}

void heal_single_unit(char b_id)
{
  char target;
  target = find_lowest_hp(b_id,battleunits[b_id].ent_id);

  // set_unit_heal(b_id);
  battleunits[target].target = 1;
  load_animations_to_vram(battleunits[b_id].unit->unit->id);
}

void attack_multi_aoe(char b_id)
{
  // set_unit_attack(b_id);
  target_aoe(find_target_unit(b_id),get_opposing_team_id(battleunits[b_id].ent_id),STUNNED,1,1);
  load_animations_to_vram(battleunits[b_id].unit->unit->id);
}

void target_multi_row(char b_id)
{
  // set_unit_attack(b_id);
  target_row(find_target_unit(b_id)/3);
  load_animations_to_vram(battleunits[b_id].unit->unit->id);
}

void target_multi_col(char b_id, char cnt)
{
  // set_unit_attack(b_id);
  target_col(find_target_unit(b_id),cnt);
  load_animations_to_vram(battleunits[b_id].unit->unit->id);
}

void target_single_unit(char b_id)
{
  char target;
  if(team_one_tracked && !unit_direction(b_id))
  {
    target = team_one_tracked;
  }
  else if(team_two_tracked && unit_direction(b_id))
  {
    target = team_two_tracked;
  }
  else
  {
    target = find_target_unit(b_id);
  }

  set_unit_target(target);
  load_animations_to_vram(battleunits[b_id].unit->unit->id);
  // if(debug_flag)
  // {
  //   put_number(b_id,3,0,0);
  //   wait_for_I_input();
  // }
}

void target_aoe(char position, char team, char state, char targeted, char animated)
{
  //target
  set_unit_state(position,state,targeted,animated);

  //front
  if(battleunits[position-3].ent_id == team
     && battleunits[position-3].active
     && position-3 >= 0)
  {
    set_unit_state(position-3,state,targeted,animated);
  }

  //to left
  if(battleunits[position-1].ent_id == team
     && battleunits[position-1].active
     && position-1 >= 0)
  {
    set_unit_state(position-1,state,targeted,animated);
  }

  //to right
  if(battleunits[position+1].ent_id == team
     && battleunits[position+1].active
     && position+1 < 18)
  {
    set_unit_state(position+1,state,targeted,animated);
  }
  //behind
  if(battleunits[position+3].ent_id == team
     && battleunits[position+3].active
     && position+3 < 18)
  {
    set_unit_state(position+3,state,targeted,animated);
  }
}

void target_opponents(char position)
{
  char i, trgt_ent;
  trgt_ent = (battleunits[position].ent_id == atker)? trgt : atker;

  for(i=0; i<18; i++)
  {
    if(battleunits[i].active && battleunits[i].ent_id == trgt_ent)
    {
      set_unit_target(i);
    }
  }
  load_animations_to_vram(battleunits[position].unit->unit->id);
}

void target_allies(char position)
{
 char i, trgt_ent;
  trgt_ent = (battleunits[position].ent_id == atker)? atker : trgt;

  for(i=0; i<18; i++)
  {
    if(battleunits[i].active && battleunits[i].ent_id == trgt_ent)
    {
      set_unit_target(i);
    }
  }
  load_animations_to_vram(battleunits[position].unit->unit->id);
}

void target_row(char position)
{
  char i;

  for(i=0; i<3; i++)
  {
    if(battleunits[(position*3)+i].active)
    {
      // set_unit_stunned((position*3)+i);
      set_unit_target((position*3)+i);
    }
  }
}

void target_col(char position, char hits)
{
  char i;
  for(i=0; i<hits; i++)
  {
    if((position+(i*3)) < 18 && battleunits[(position+(i*3))].active)
    {
      // set_unit_stunned((position+(i*3)));
      set_unit_target(position+(3*i));
    }
  }
}

void set_unit_state(char b_id, char state, char targeted, char animated)
{
  battleunits[b_id].state = state;
  battleunits[b_id].target = targeted;
  battleunits[b_id].frame = 0;
  animating += animated;
}

void set_unit_target(char b_id)
{
  battleunits[b_id].target = 1;
}

void set_unit_stunned(char b_id)
{
  battleunits[b_id].state = STUNNED;
  battleunits[b_id].frame = 0;
  battleunits[b_id].target = 1;
  animating++;
}

void set_unit_attack(char b_id)
{
  battleunits[b_id].state = ATTACK;
  battleunits[b_id].frame = 0;
  animating++;
}

void set_unit_meter(char b_id)
{
  battleunits[b_id].state = METER_ATTACK;
  battleunits[b_id].frame = 0;
}

void set_unit_heal(char b_id)
{
  battleunits[b_id].state = HEALING;
  battleunits[b_id].frame = 0;
  animating++;
}

void set_unit_waiting(char b_id)
{
  spr_set(b_id+5);
  spr_pattern(idle_vrams[b_id]);
  battleunits[b_id].state = WAITING;
  battleunits[b_id].frame = 0;
  animating--;
}

char find_lowest_hp(char b_id, char team)
{
  int lowest, hp_p;
  char i, id;
  hp_p = 0;
  lowest = get_percentage(battleunits[b_id].unit->hp,battleunits[b_id].unit->unit->hp);
  id = b_id;

  for(i=0; i<18; i++)
  {
    if(battleunits[i].active && battleunits[i].ent_id == team)
    {
      hp_p = get_percentage(battleunits[i].unit->hp,battleunits[i].unit->unit->hp);
      if(hp_p < lowest)
      {
        lowest = hp_p;
        id = i;
      }
    }
  }
  return id;
}

char do_art(char b_id)
{
  switch(battleunits[b_id].unit->unit->art)
  {
    case JUDGEMENT_ART:
    judgement(b_id);
    return 0;

    case FRENZY_ART:
    return frenzy(b_id);

    case TRACK_ART:
    tracked(b_id);
    return 1;

    case INNVIGORATE_ART:
    innvigorate(b_id);
    return 0;

    case RESTORE_ART:
    restore(b_id);
    return 0;

    case RED_EYE_ART:
    red_eye(b_id);
    return 1;

    case BLACK_EYE_ART:
    black_eye(b_id);
    return 1;

    case CLEAR_EYES_ART:
    clear_eyes(b_id);
    return 0;

    case RAPID_THRUST_ART:
    rapid_thrust(b_id);
    return 1;

    case CAPTURE_ART:
    capture(b_id);
    return 1;

    case SEA_LEGS_ART:
    sea_legs(b_id);
    return 1;
  }
  return 1;
}

void restore(char b_id)
{
  char i;
  for(i=0; i<18; i++)
  {
    if(battleunits[i].target)
    {
      battleunits[i].unit->hp =  battleunits[i].unit->unit->hp;
      update_healthbar(i);
    }
  }
  // set_unit_waiting(b_id);
  clear_targets();
  battleunits[b_id].attacks--;
  hide_art_name();
}

void innvigorate(char b_id)
{
  char i, hp;
  for(i=0; i<18; i++)
  {
    if(battleunits[i].target)
    {
      hp = (char) calc_percentage(30,battleunits[i].unit->unit->hp);
      battleunits[i].unit->hp = min(battleunits[i].unit->hp+hp,battleunits[i].unit->unit->hp);
      update_healthbar(i);
    }
  }
  clear_targets();
  hide_art_name();
}

void tracked(char b_id)
{
  char i;

  for(i=0; i<18; i++)
  {
    if(battleunits[b_id].target)
    {
      if(battleunits[b_id].ent_id == atker)
      {
        team_one_tracking = 1;
        team_one_tracked = i;
      }
      else
      {
        team_one_tracking = 1;
        team_two_tracked = i;
      }
    }
  }
  // battleunits[b_id].attacks--;
  hide_art_name();
}

void clear_eyes(char b_id)
{
  flash_screen();
  reset_battle_screen();
  battleunits[b_id].attacks--;
  hide_art_name();
  clear_eyes_called = 1;
}

void sea_legs(char b_id)
{
  sea_legs_art = 1;
}

void judgement(char b_id)
{
  flash_screen();
  reset_battle_screen();
  // set_unit_waiting(b_id);
  battleunits[b_id].attacks--;
  hide_art_name();
  if(battleunits[b_id].ent_id == atker){ team_one_judgement = 1; } else { team_two_judgement = 1; }
}

char frenzy(char b_id)
{
  int r;

  r = range(1,100);
  if(r < 75 && battleunits[get_first_target()].unit->hp > 0)
  {
    battleunits[b_id].frame = 0;
    set_unit_meter(b_id);
    animating++;
    return 0;//trigger again
  }
  return 1;//done
}

void capture(char b_id)
{
  char t_id;
  t_id = get_first_target();
  if(battleunits[t_id].unit->unit->a_type == NONE)
  {
    if(range(1,100)<51)
    {
      add_unit_to_convoy(battleunits[t_id].unit->unit->id);
      kill_unit(t_id);
    }
  }
}

void rapid_thrust(char b_id)
{
  spr_set(b_id+5);
  load_art(art_queue_id,spr_get_x()+8,spr_get_y()+16,!unit_direction(b_id));
  animate_art(art_queue_id);
  // apply_art(i);
  remove_effects();
  battleunits[b_id].attacks = 3;
}

void red_eye(char b_id)
{
  char dmg, percent_dmg, missing_amt, percent_missing, full_hp, current_hp;

  full_hp = battleunits[b_id].unit->unit->hp;
  current_hp = battleunits[b_id].unit->hp;
  missing_amt = full_hp - current_hp;

  percent_missing = (char) get_percentage(missing_amt,full_hp);

  dmg = 3 * max((percent_missing / 10),1);
  apply_damage(get_first_target(),dmg);
  update_healthbar(get_first_target());
}

void black_eye(char b_id)
{
  char target_id, dmg;
  target_id = get_first_target();
  dmg = calc_percentage(35,battleunits[target_id].unit->hp);

  apply_damage(target_id,dmg);
  update_healthbar(target_id);
}

void reset_battle_screen()
{
  load_palette(0,battlepal,1);
  load_healthbars();
  set_infobar();
  load_palette(9,borderspal,1);
  load_palette(10,fontpal,2);
}

char unit_direction(char b_id);
