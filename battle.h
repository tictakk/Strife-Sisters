// #incbin(battlemap,"tiles/battletiles.battle_backgrounds.layer-Layer 1.map001.stm")
// #inctilepal(battletilespal,"tiles/battletiles.tiles.pcx")
// #inctile(battletiles,"tiles/battletiles.tiles.pcx")
// #incpal(battlepal,"tiles/battletiles.tiles.pcx")

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
#define STATE_EVADE 9

#define STATUS_NORMAL 0
#define STATUS_DAZED 1
#define STATUS_CONFUSED 2
#define STATUS_RAGE 3
#define STATUS_BLOWBACK 4
#define STATE_ACID 5

#define COMMANDER_PALETTE_1 28
#define COMMANDER_PALETTE_2 29

#define LEFT_SIDE 1
#define RIGHT_SIDE 2

typedef struct {
  char ent_id, active, frame, pal, state, target_team, pos, attacks, 
       target, meter, column, a_bonus, d_bonus, s_bonus, p_bonus;
  Unit_Entity *unit;
}BattleUnit;

// const char big_palettes[] = {17,18,};

const char top_row_attack_chart[9]    = { 
                                          1, 1, 2, 
                                          2, 2, 3, 
                                          3, 3, 4
                                        };

const char middle_row_attack_chart[9] = { 
                                          1, 1, 1, 
                                          2, 2, 2, 
                                          3, 3, 3
                                        };
                                        
const char bottom_row_attack_chart[9] = { 
                                          2, 1, 1, 
                                          3, 2, 2, 
                                          4, 3, 3
                                        };

const char ranged_attack_chart[9] = { 
                                      1, 1, 1, 
                                      1, 1, 1, 
                                      1, 1, 1
                                    };
// char vram_temp[256];
char cmdr_count = 28;
const int target_square_tiles[] = {
                                    492,652,812,
                                    487,647,807,
                                    482,642,802,

                                    497,657,817,
                                    502,662,922,
                                    507,667,927
                                  };

// char attacker_art_list[MAX_ARMY_SIZE];
// char target_art_list[MAX_ARMY_SIZE];

char atker, trgt, meter_queued = 0, unit_meter_queued = 0, position_status = 0;//FINISH THIS STUFF

int team_one_count, team_two_count;
int a_def_bonus, a_atk_bonus, t_def_bonus, t_atk_bonus;
char a_terrain, t_terrain, attack_range, 
     battle_clock, animating, attacker, stun_count;

char art_queued = 0, art_unit_id = 0, art_queue_id = 0;

char attack_side, attacker_bonuses, target_bonuses;
char attacking_bonuses, targeted_bonuses;

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
                     char enemy_ent_id, char position, Unit_Entity *ue, char pal)
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
  battleunits[index].s_bonus = 0;
  battleunits[index].p_bonus = 0;
  battleunits[index].column = position/3;

  load_unit_header(ue->id,0);
  
  if(unit_header[0].rng < attack_range)
  {
    battleunits[index].attacks = 0;
  }

  myPointers.bank[2] = unit_header[0].bank;
  myPointers.addr[2] = unit_header[0].address;

  load_vram_fptr( setFarLoadvram(idle_vrams[index], myPointers.bank[2], myPointers.addr[2], 0x100) );
  // battleunits[index].pal = 20;//figure out how I wanna do palettes
  // farMemcpy( setFarMemcpy(vram_temp, myPointers.bank[2], myPointers.addr[2], 256) );
  // load_vram(idle_vrams[index],vram_temp,0x100);

  p_x = (int)(x << 5);
  p_y = (int)(y << 5);

  if(active)
  {
	  spr_make(index+MAX_EFFECT_COUNT,((p_x/4)*5)+xOffset,((p_y/4)*5)-16,idle_vrams[index],FLIP_MAS|SIZE_MAS,SZ_32x32,pal,1);
  }
  satb_update();
}

void bid_to_unit_header(char b_id, char header_no)
{
  load_unit_header(battleunits[b_id].unit->id,header_no);
}

void transfer_units_to_attack_vram(char type)
{
  load_unit_header(type,0);
  myPointers.bank[2] = unit_header[0].bank;
  myPointers.addr[2] = unit_header[0].address;

  load_vram_fptr( setFarOffsetLoadvram(attack_vrams[0], myPointers.bank[2], myPointers.addr[2], 0x500 , 0x400) );
}

void transfer_units_to_stun_vram(char type, char index)
{
  load_unit_header(type,1);
  myPointers.bank[2] = unit_header[1].bank;
  myPointers.addr[2] = unit_header[1].address;

  load_vram_fptr( setFarOffsetLoadvram(stun_vrams[index], myPointers.bank[2], myPointers.addr[2], 0x100 , 0x1000) );
}

char find_target_unit_type(char unit_position, char attackble_units[9], char type)
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
        bid_to_unit_header(i,1);
        if(unit_header[1].a_type == type)
        {
          attackble_units[attackble_count++] = i;
        }
      }
    }
  }
  return attackble_count;
}

char find_target_unit(char attacking_unit_position, char ranged, char type)
{
  char attackable_count;
  char attackble_units[9];
  attackable_count = 0;

  if(type)
  {
    attackable_count = find_target_unit_type(attacking_unit_position,attackble_units,type);
  }
  else
  {
    attackable_count = find_attackable_units(attacking_unit_position,attackble_units,ranged);
  }

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

char find_attackable_units(char unit_position, char attackble_units[9], char ranged)
{
  char i;
  char *a_chart;
  char attackble_chart[9];
  char attackble_count,attack_tier;
  char b_unit_team;
  b_unit_team = battleunits[unit_position].ent_id;

  attack_tier = 5;
  attackble_count = 0;

  if(ranged)
  {
    memcpy(attackble_chart,ranged_attack_chart,9);
  }
  else
  {
    a_chart = get_attack_row_chart(battleunits[unit_position].pos);
    memcpy(attackble_chart,a_chart,9);
    modify_attack_chart(attackble_chart,battleunits[unit_position].target_team);
  }

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

void determine_targets(char b_id, char ranged)
{
  // switch(arts[battleunits[b_id].unit->unit->attacks[battleunits[b_id].column]].target)
  switch(arts[unit_header[0].attacks[battleunits[b_id].column]].target)
  {
    case SINGLE_HIT:
      target_single_unit(b_id,ranged);
      break;
    case MULTI_ROW:
      target_multi_row(b_id,ranged);
      break;
    case MULTI_COL_3:
      target_multi_col(b_id,ranged);
      break;
    // case MULTI_ATTACK_AOE:
      // break;
    case HEAL:
      heal_single_unit(b_id);
      break;
    case HEAL_ALL:
      target_allies(b_id);
      break;
    case ALL_OPPONENTS:
      target_opponents(b_id);
      break;
    case ALL_ALLIES:
      target_allies(b_id);
      break;
    default:
      target_single_unit(b_id,ranged);
      break;
  }
}

void determine_action_state(char b_id)
{
  switch(arts[unit_header[0].attacks[battleunits[b_id].column]].move_type)
  {
    case MOVE_RANGED_ATTACK:
    case MOVE_PHYSICAL_ATTACK:
    set_unit_attack(b_id);
    break;

    case MOVE_ART_SUPPORT:
    case MOVE_ART_ATTACK:
    load_animations_to_vram(battleunits[b_id].unit->id);
    set_unit_meter(b_id);
    battleunits[b_id].meter = 0;
    animating++;
    break;

    case MOVE_HEAL:
    set_unit_heal(b_id);
    break;

    case MOVE_NONE:
    battleunits[b_id].attacks = 0;
    break;
  }
}

// void heal_all(char b_id)
// {
//   char target;
//   target = find_lowest_hp(b_id,battleunits[b_id].ent_id);

//   target_aoe(target,battleunits[b_id].ent_id,IDLE,1,0);
//   load_animations_to_vram(battleunits[b_id].unit->unit->id);
// }

void heal_single_unit(char b_id)
{
  char target;
  target = find_lowest_hp(b_id,battleunits[b_id].ent_id);

  // set_unit_heal(b_id);
  battleunits[target].target = 1;
  load_animations_to_vram(battleunits[b_id].unit->id);
}

void target_multi_row(char b_id, char ranged)
{
  // set_unit_attack(b_id);
  target_row(find_target_unit(b_id,ranged,NONE));
  load_animations_to_vram(battleunits[b_id].unit->id);
}

void target_multi_col(char b_id, char ranged)
{
  target_col(find_target_unit(b_id,ranged,NONE)/3);
  load_animations_to_vram(battleunits[b_id].unit->id);
}

void target_single_unit(char b_id, char ranged)
{
  char target;
  if(check_battle_bonus(BONUS_SWORD,targeted_bonuses))
  {
    target = find_target_unit(b_id,ranged,NORMAL);
  }
  else
  {
    target = find_target_unit(b_id,ranged,NONE);
  }
  set_unit_target(target);
  highlight_battle_square(target,0xE000);
  load_animations_to_vram(battleunits[b_id].unit->id);
  // if(debug_flag)
  // {
  //   put_number(b_id,3,0,0);
  //   wait_for_I_input();
  // }
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
      highlight_battle_square(i,0xE000);
    }
  }
  load_animations_to_vram(battleunits[position].unit->id);
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
      highlight_battle_square(i,0xE000);
    }
  }
  load_animations_to_vram(battleunits[position].unit->id);
}

void target_col(char position)
{
  char i;

  for(i=0; i<3; i++)
  {
    if(battleunits[(position*3)+i].active)
    {
      // set_unit_stunned((position*3)+i);
      set_unit_target((position*3)+i);
      highlight_battle_square((position*3)+i,0xE000);
    }
  }
}

void target_row(char position)
{
  char i;
  for(i=0; i<3 - ((position%9)/3); i++)
  {
    if((position+(i*3)) < 18 && battleunits[(position+(i*3))].active)
    {
      set_unit_target(position+(3*i));
      highlight_battle_square(position+(3*i),0xE000);
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

void set_unit_evade(char b_id)
{
  battleunits[b_id].state = STATE_EVADE;
  battleunits[b_id].frame = 0;
  battleunits[b_id].target = 0;
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
  spr_set(b_id+MAX_EFFECT_COUNT);
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
  bid_to_unit_header(b_id,1);
  apply_level_to_header(battleunits[b_id].unit->level,1);
  lowest = get_percentage(battleunits[b_id].unit->hp,unit_header[1].hp);
  id = b_id;

  for(i=0; i<18; i++)
  {
    if(battleunits[i].active && battleunits[i].ent_id == team)
    {
      bid_to_unit_header(i,1);
      apply_level_to_header(battleunits[i].unit->level,1);
      hp_p = get_percentage(battleunits[i].unit->hp,unit_header[1].hp);
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
  // switch(art_queue_id)
  // {

  //   case INNVIGORATE_ART:
  //   innvigorate(b_id);
  //   return 0;

  //   case RESTORE_ART:
  //   restore(b_id);
  //   return 0;

  //   case RED_EYE_ART:
  //   red_eye(b_id);
  //   return 1;

  //   case BLACK_EYE_ART:
  //   black_eye(b_id);
  //   return 1;

  //   case RAPID_THRUST_ART:
  //   rapid_thrust(b_id);
  //   return 1;
  // }
  // return 1;
}

void restore(char b_id)
{
  char i;
  for(i=0; i<18; i++)
  {
    if(battleunits[i].target)
    {
      bid_to_unit_header(i,0);
      apply_level_to_header(battleunits[i].unit->level,0);
      battleunits[i].unit->hp =  unit_header[0].hp;//battleunits[i].unit->unit->hp;
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
      bid_to_unit_header(i,1);
      apply_level_to_header(battleunits[i].unit->level,1);
      hp = (char) calc_percentage(30,unit_header[1].hp);
      battleunits[i].unit->hp = min(battleunits[i].unit->hp+hp,unit_header[1].hp);
      update_healthbar(i);
    }
  }
  clear_targets();
  hide_art_name();
}

// void clear_eyes(char b_id)
// {
//   flash_screen();
//   reset_battle_screen();
//   battleunits[b_id].attacks--;
//   hide_art_name();
//   clear_eyes_called = 1;
// }

// void sea_legs(char b_id)
// {
//   sea_legs_art = 1;
// }

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

void capture(char b_id){}

void rapid_thrust(char b_id)
{
  spr_set(b_id+MAX_EFFECT_COUNT);
  load_art(art_queue_id,spr_get_x()+8,spr_get_y()+16,!unit_direction(b_id));
  animate_art(art_queue_id);
  // apply_art(i);
  remove_effects();
  battleunits[b_id].attacks = 3;
}

void red_eye(char b_id)
{
  char dmg, percent_dmg, missing_amt, percent_missing, full_hp, current_hp;
  bid_to_unit_header(b_id,0);
  apply_level_to_header(battleunits[b_id].unit->level,0);
  full_hp = unit_header[0].hp;
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

void highlight_battle_square(int square, int pal)
{
  //492
  int i, j;
  for(i=0; i<3; i++)
  {
    for(j=0; j<3; j++)
    {
      change_background_pal_single(target_square_tiles[square]+(j*32)+i,pal);
    }
  }
} 

char get_bonus_mask_by_target()
{

}

char unit_direction(char b_id);
