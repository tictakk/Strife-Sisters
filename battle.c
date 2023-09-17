// #include "battle_menu.h"
#include "battle_bonus.c"
#include "battle.h"

#define WORDING_NONE 0
#define WORDING_MISS 1
#define WORDING_ADV 2
#define WORDING_CRIT 3

#define PHASE_START 0
#define PHASE_HEAL 1
#define PHASE_SUPPORT 2
#define PHASE_MELEE 3
#define PHASE_RANGED 4
#define PHASE_MAGIC 5
#define PHASE_END 6

#define COUNTER_ATTACK_BONUS 15
#define LIFE_STEAL_BONUS 25
#define EVADE_BONUS 25
#define CRIT_BONUS 68
#define REGEN_BONUS 5

// #incbin(battlemap,"tiles/battletiles.battle_backgrounds.layer-Layer 1.map001.stm")
// #inctilepal(battletilespal,"tiles/battletiles.tiles.pcx")
// #inctile(battletiles,"tiles/battletiles.tiles.pcx")
// #incpal(battlepal,"tiles/battletiles.tiles.pcx")

int battle_exp = 0, damage_dealt = 0, roll = 0;
char battle_killed = 0, battle_lost = 0, player_id;
char crit, evade, battle_phase, attacker_effect, target_effect;

void battle_end_screen()
{
  if(
    !team_one_count && entities[atker].team == PLAYER ||
    !team_two_count && entities[trgt].team == PLAYER
    )
  {
    put_string("Defeated",9,1);
    return;
  }
  display_window_abs(9,0,12,10);
  write_text(10,1,"damage");
  sync(30);

  display_number_incrementing(17,1,damage_dealt,3);
  write_text(10,2,"killed");
  sync(30);
  display_number_incrementing(19,2,battle_killed,1);
  write_text(10,3,"lost");
  sync(30);
  display_number_incrementing(19,3,battle_lost,1);
  write_text(10,4,"exp");
  sync(30);
  display_number_incrementing(17,4,battle_exp,3);
  sync(60);
}

void battle_seq()
{
  load_pals(atker,0);
  load_pals(trgt,9);
  satb_update();
  sync(60);
  // put_hex(&battle_exp,6,0,0);
  // countdown(2,8,"Prepare",12);
  // countdown(2,8," Press Run for arts",22);
  battle_phase = PHASE_START;
  attacking_bonuses = entities[atker].bg->bonuses;
  targeted_bonuses = entities[trgt].bg->bonuses;
  d_battle(atker,LEFT_SIDE);
  // if(clear_eyes_called == 0)
  // {
  animating = 0;
  battle_phase = PHASE_START;
  attacking_bonuses = entities[trgt].bg->bonuses;
  targeted_bonuses = entities[atker].bg->bonuses;
  d_battle(trgt,RIGHT_SIDE);
  // }
  // countdown(3,2,8,"end "); countdown timer at the end for what?
  end_sequence();
  vsync();
}

void determine_crit(char b_id)
{
  int odds;
  
  crit = 0;
  odds = ((unit_header[0].spd + battleunits[b_id].s_bonus) * 100)/255;
  roll = range(1,100);

  if(check_battle_bonus(BONUS_AXE,attacking_bonuses))
  {
    odds = max(odds,CRIT_BONUS); //68 == 25% of 255
  }

  if(roll < odds)
  {
    crit = 1;
    attacker_effect = WORDING_CRIT;
  }
  else
  {
    crit = 0;
  }
}

void determine_miss(char b_id, char target_id)
{
  //algo for miss here...
  //max modifer = +51, -204
  int difference, accuracy;
  difference = (unit_header[0].spd + battleunits[b_id].s_bonus) - (unit_header[1].spd + battleunits[target_id].s_bonus);
  evade = 0;

  if(check_battle_bonus(BONUS_BOW,targeted_bonuses))//if +3 bow units
  {
    difference += EVADE_BONUS;
  }
  if(difference >= 25)
  {
    return;
  }
  if(difference < -204)
  {
    misses++;
    set_unit_evade(target_id);
    return;
  }

  accuracy = BASE_HIT_CHANCE + difference;
  roll = rand() & 0xFF;

  if(accuracy < roll)
  {
    // put_string("miss",10,0);
    // put_number(roll,5,10,0);
    // put_number(accuracy,5,16,0);
    // wait_for_I_input();
    set_unit_evade(target_id);
    misses++;
  }
  // return evade;
}

void apply_modifier(char target, char modifier, char amount)
{
  switch(modifier)
  {
    case MOD_SPD:
    battleunits[target].s_bonus += amount;
    break;

    case MOD_POW:
    battleunits[target].p_bonus += amount;
    break;

    case MOD_ATK:
    battleunits[target].a_bonus += amount;
    break;

    case MOD_DEF:
    battleunits[target].d_bonus += amount;
    break;

    case MOD_HP:
    // apply_damage(target,calc_art_damage(battleunits[art_unit_id].ent_id,(int)amount));
    load_unit_header(battleunits[art_unit_id].unit->id,0);
    apply_level_to_header(battleunits[art_unit_id].unit->level,0);
    bid_to_unit_header(target,1);
    apply_level_to_header(battleunits[target].unit->level,1);

    apply_damage(target,calc_damage(art_unit_id,
                                    target,
                                    battleunits[art_unit_id].unit->level,
                                    unit_header[0].intel,
                                    battleunits[art_unit_id].a_bonus,
                                    battleunits[target].d_bonus,
                                    unit_header[1].res,
                                    amount));
    break;
  }
  update_healthbar(target);
}

void perform_art()
{
  switch(arts[art_queue_id].relationship)
  {
    case ONE_TO_ONE:
    animate_one_to_one_art();
    break;

    case ONE_TO_MANY:
    animate_one_to_many_art();
    break;

    case MANY_TO_MANY:
    animate_many_to_many_art();
    break;
  }
  // if(do_art(art_unit_id) == 1)
  // {
  //   hide_art_name();
  //   clear_targets();
  //   remove_effects();
  // }
  clear_art_queue();
}

void animate_one_to_one_art()
{
  char i;

  for(i=0; i<18; i++)
  {
    if(battleunits[i].active && battleunits[i].target)
    {
      spr_set(i+MAX_EFFECT_COUNT);
      load_art(art_queue_id,spr_get_x()+8,spr_get_y()+16,!unit_direction(i));
      animate_art(art_queue_id);
      apply_art(i);
      remove_effects();
      // return;
    }
  }
}

void animate_one_to_many_art()
{
  char i;

  spr_set(get_first_target()+MAX_EFFECT_COUNT);
  load_art(art_queue_id,spr_get_x(),spr_get_y(),!unit_direction(get_first_target()));
  animate_art(art_queue_id);
  for(i=0; i<18; i++)
  {
    if(battleunits[i].active && battleunits[i].target)
    {
      apply_art(i);
    }
  }
  remove_effects();
}

void animate_many_to_many_art()
{
  char i;

  for(i=0; i<18; i++)
  {
    if(battleunits[i].active && battleunits[i].target)
    {
      spr_set(i+MAX_EFFECT_COUNT);
      load_art(art_queue_id,spr_get_x(),spr_get_y(),!unit_direction(i));
      animate_art(art_queue_id);
      apply_art(i);
      remove_effects();
    }
  }
}

void animate_art(char art_id)
{
  char i;

  for(i=0; i<arts[art_id].frame_count; i++)
  {
    // put_number(effect_count,3,0,0);
    animate_effects();
    satb_update();
    vsync(3);
  }
}

void clear_art_queue()
{
  art_queue_id = 0;
  art_queued = 0;
  art_unit_id = 0;
}

void apply_art(char target)
{
  char modifier;
  modifier = 0;
  // wait_for_I_input();
  // spr_set(target+5);
  // load_art(art_queue_id,spr_get_x(),spr_get_y(),!unit_direction(target));
  // animate_art(art_queue_id);
  if(arts[art_queue_id].stunning)
  {
    set_unit_stunned(target);
  }
  // remove_effects();
  apply_modifier(target,get_modifier_type(art_queue_id),get_modifier_amount(art_queue_id)-modifier);
}

void end_sequence()
{
  update_info_bar();
  battle_end_screen();
  distribute_exp();
  fade_screen();
  sync(10);
}

void distribute_exp()
{
  unsigned char i, unit_count, exp_per;
  if(atker == player_id)
  {
    unit_count = team_one_count;
  }
  else
  {
    unit_count = team_two_count;
  }
  if(unit_count == 0)
  {
    return;
  }
  exp_per = (battle_exp / unit_count);
  // for(i=0; i<9; i++)
  // {
  //   if(entities[player_id].bg->units[i].id)
  //   {
  //     char old_level;
  //     entities[player_id].bg->units[i].exp += exp_per;
  //     old_level = entities[player_id].bg->units[i].level;
  //     if(old_level < level_up_unit(&entities[player_id].bg->units[i]))
  //     {
  //       write_text(10,6,"new level!");
  //       print_unit_fullname(entities[player_id].bg->units[i].id,10,7);
  //       write_text(10,8,"level");
  //       put_number(entities[player_id].bg->units[i].level,2,16,8);
  //       sync(60);
  //     }
  //   }
  // }
  sync(30);
}

void d_battle(char team, char side)
{
  char i = 0, loop_counter;
  int b_ticker = 0;

  attack_side = side;
  // animating = 0;
  loop_counter = 0;
  battle_clock = 0;
  adv_plus_flag = 0;
  adv_plus_plus_flag = 0;
  next_phase();
  while(battle_phase != PHASE_END)
  {
    // put_number(animating,3,0,0);
    // if(joytrg(0) == JOY_RUN)
    // {
    //   battle_ctrls();
    // }
    if(b_ticker++ == 2)
    {
      // put_number(animating,3,0,0);
      // cycle_button_press(10,1);
      b_ticker = 0;

      if(animating == 0)
      {
        if(meter_queued && entities[team].team == PLAYER)
        {
          battle_clock = unit_meter_queued;
          unit_meter_queued = 0;
          meter_queued = 0;
        }
        else
        {
          battle_clock = get_highest_speed(team);
          if(battle_clock == -1)
          {
            next_phase();
          }
        }
      }
      stun_count = 0;
      for(i=0; i<18; i++)
      {
        battle_unit_state(i);
      }
      // animate_status_effects();
      if(art_queued)
      {
        perform_art();
      }
      // animate_effects();
    }
    satb_update();
    vsync();
  }
}

void clear_targets()
{
  char i;
  for(i=0; i<18; i++)
  {
    battleunits[i].target = 0;
  }
}

char get_highest_speed(char team)
{
  char i, hi_spd, highest;

  if(team_one_count == 0 || team_two_count == 0)
  {
    return -1;
  }
  hi_spd = 0;
  highest = -1;

  for(i=0; i<18; i++)
  {
    if(battleunits[i].active && battleunits[i].ent_id == team && battleunits[i].attacks)//battleunits[i].attacks > hi_attacks)
    {
      bid_to_unit_header(i,0);
      apply_level_to_header(battleunits[i].unit->level,0);

      if(get_attack_type_by_phase(battle_phase) == arts[unit_header[0].attacks[battleunits[i].column]].move_type)
      {
        if(unit_header[0].spd > hi_spd)
        {
          hi_spd = unit_header[0].spd;
          highest = i;
        }
      }
    }
  }
  return highest;
}

void next_phase()
{
  // switch(battle_phase)
  // {
    // case PHASE_START: battle_phase = PHASE_SUPPORT; break;
    // case PHASE_
    // case PHASE_SUPPORT: battle_phase = PHASE_ATTACK; break;
    // case PHASE_ATTACK: battle_phase = PHASE_MAGIC; break;
    // case PHASE_MAGIC: battle_phase = PHASE_HEAL; break;
    // case PHASE_HEAL: battle_phase = PHASE_END; break;
  // }
  battle_phase++;
}

char get_attack_type_by_phase(char phase)
{
  switch(phase)
  {
    case PHASE_START: return MOVE_NONE;
    case PHASE_SUPPORT: return MOVE_ART_SUPPORT;
    case PHASE_MELEE: return MOVE_PHYSICAL_ATTACK;
    case PHASE_RANGED: return MOVE_RANGED_ATTACK;
    case PHASE_MAGIC: return MOVE_ART_ATTACK;
    case PHASE_HEAL: return MOVE_HEAL;
  }
  return 0;
}

void battle_unit_state(char b_id)
{
  if(battleunits[b_id].state == ATTACK){ b_u_attack(b_id); return; }
  if(battleunits[b_id].state == STUNNED){ b_u_stun(b_id); return; }
  if(battleunits[b_id].state == STATE_EVADE){ b_u_evade(b_id); return; }
  if(battleunits[b_id].state == IDLE){ b_u_idle(b_id); return; }
  if(battleunits[b_id].state == WAITING) { b_u_wait(b_id); return; }
  if(battleunits[b_id].state == HEALING) { b_u_heal(b_id); return; }
  if(battleunits[b_id].state == METER_ATTACK) { b_u_meter(b_id); return; }
}

void b_u_meter(char b_id)
{
  if(battleunits[b_id].frame == 0)
  {
    display_window_rel(2,10,12,3);
    bid_to_unit_header(b_id,0);
    put_string(arts[unit_header[0].attacks[battleunits[b_id].column]].name,3,11);
  } 
  if(battleunits[b_id].frame < 6)//animating attack
  {
    spr_set(b_id+MAX_EFFECT_COUNT);
    animate_attack(b_id,battleunits[b_id].frame++);
  }
  else if(battleunits[b_id].frame == 7)
  {
    battleunits[b_id].frame++;
  }
  else if(battleunits[b_id].frame < 11)
  {
    spr_set(b_id+MAX_EFFECT_COUNT);
    animate_attack(b_id,battleunits[b_id].frame++);
  }
  else
  {
    battleunits[b_id].attacks--;
    set_unit_waiting(b_id);
    art_queued = 1;
    art_unit_id = b_id;
    // art_queue_id = battleunits[b_id].unit->unit->attacks[battleunits[b_id].column];//battleunits[b_id].unit->unit->art;
    art_queue_id = unit_header[0].attacks[battleunits[b_id].column];
  }
}

void b_u_idle(char b_id)
{
  if(battle_clock == b_id && battleunits[b_id].active
    // && entities[battleunits[b_id].ent_id].bg->units[battleunits[b_id].pos]->unit.rng >= attack_range
    && animating == 0)
  {
    bid_to_unit_header(b_id,0);
    if((arts[unit_header[0].attacks[battleunits[b_id].column]].move_type > 19 && arts[unit_header[0].attacks[battleunits[b_id].column]].move_type < 23) 
        || arts[unit_header[0].attacks[battleunits[b_id].column]].move_type == MOVE_ART_ATTACK)
    {
      determine_targets(b_id,1); //ranged
    }
    else
    {
      determine_targets(b_id,0);
    }
    determine_action_state(b_id);
  }
}

void b_u_attack(char b_id)
{
  if(battleunits[b_id].frame == 0){} //check if unit has advantage
  if(battleunits[b_id].frame == 6)
  {
    set_targets_stunned(b_id);
  }
  if(battleunits[b_id].frame < 11)//animating attack
  {
    animate_attack(b_id,battleunits[b_id].frame++);
  }
  else //finish animation, calculate damage
  {
    unsigned char dmg;
    char i;
    for(i=0; i<18; i++)
    {
      if(battleunits[i].target)
      {
        bid_to_unit_header(b_id,0);
        apply_level_to_header(battleunits[b_id].unit->level,0);

        bid_to_unit_header(i,1);
        apply_level_to_header(battleunits[i].unit->level,1);

        get_advantages(unit_header[0].a_type);
        check_advantage_plus(unit_header[1].a_type);
        check_advantage_plus_plus(unit_header[1].a_type);
        // determine_crit();
        // determine_miss();
        // if(!evade)
        // {
          dmg = calc_damage(b_id,
                            i,
                            battleunits[b_id].unit->level,
                            unit_header[0].atk,
                            battleunits[b_id].a_bonus,
                            battleunits[i].d_bonus,
                            unit_header[1].def,
                            arts[unit_header[0].attacks[battleunits[b_id].column]].base_amt);
          //TODO: if this is a counter hit, 15 instead of 0 in third param here
          if(check_battle_bonus(BONUS_MAGIC,target_bonuses) && attack_side == RIGHT_SIDE)
          {
            calc_hit_damage(i,dmg,COUNTER_ATTACK_BONUS);
          }
          else
          {
            calc_hit_damage(i,dmg,0);
          }
          
          if(check_battle_bonus(BONUS_POLE,attacking_bonuses))//life steal
          {
            // calc_heal(0,b_id,0,0,0,15);
            heal_dmg_percent(b_id,dmg,LIFE_STEAL_BONUS);
          }
          // evade = 0;
        // }
        battleunits[i].target = 0;
      }
      crit = 0;
    }
    set_unit_waiting(b_id);
    battleunits[b_id].attacks--;
    remove_effects();
    load_map(0,5,0,5,16,9); 
  }
}

void b_u_evade(char b_id)
{
  if(battleunits[b_id].frame++ == 0)
  {
    spr_set(MAX_EFFECT_COUNT+b_id);
    create_miss_effect(spr_get_x(),spr_get_y());
  }
  else if(battleunits[b_id].frame < 11)
  {
    animate_effects();
  }
  else
  {
    set_unit_waiting(b_id);
    remove_effects();
  }
}

void b_u_stun(char b_id)
{
  if(battleunits[b_id].frame == 0)
  {
    spr_set(b_id+MAX_EFFECT_COUNT);
    spr_pattern(stun_vrams[stun_count++]);
    battleunits[b_id].frame++;
    create_hit_spark(spr_get_x(),spr_get_y(),0);
    spr_show(effect_count-1);
    // satb_update();
  }
  else if(battleunits[b_id].frame < 11)
  {
    animate_effects();
    if(position_status == STATUS_BLOWBACK)
    {
      blowback(b_id);
    }
    else
    {
      shake_unit(b_id);
    }
  }
  else
  { 
    set_unit_waiting(b_id);
    update_healthbar(b_id);
    position_status = 0;
    if(entities[battleunits[b_id].ent_id].bg->units[battleunits[b_id].pos].hp <= 0)
    {
      kill_unit(b_id);
    }
    remove_effects();
  }
}

void b_u_wait(char b_id)
{
  if(animating == 0)
  {
    battleunits[b_id].state = IDLE;
    // battleunits[b_id].target = 0;
    battleunits[b_id].frame = 0;
  }
}

void b_u_heal(char b_id)
{
  if(battleunits[b_id].frame == 0)
  {
    char j;
    for(j=0; j<18; j++)
    {
      if(battleunits[j].target)
      {
        spr_set(j+MAX_EFFECT_COUNT);
        // create_effect(EFFECT_HEAL,spr_get_x()+8,spr_get_y()+16,0);
        create_healing(spr_get_x()+8,spr_get_y()+16,0);
      }
    }
  }
  if(battleunits[b_id].frame < 11)
  {
    animate_attack(b_id,battleunits[b_id].frame++);
    animate_effects();
  }
  else
  {
    char i;
    for(i=0; i<18; i++)
    {
      if(battleunits[i].target)
      {
        // calc_heal(b_id,i,0,0,0,35);
        heal_health_percent(i,35);
      }
    }
    set_unit_waiting(b_id);
    battleunits[b_id].attacks--;
    //This may cause an issue when there may be a time when heal is done at the same time as another
    //effect. This erases all effects.
    remove_effects();
    load_map(0,5,0,5,16,9); 
  }
}

void animate_attack(char unit_id, char hit_frames)
{
	spr_set(unit_id+MAX_EFFECT_COUNT);
	spr_pattern(attack_vrams[0]+ATTACK_ANIMATIONS[hit_frames]);
}

void animate_health(char b_id, char start, char end)
{
  if(!(b_id/9))//side one
  {
    // display_healthbar(10-(4*(b_id/3)),2+(b_id%3),hp_p);
    animate_healthbar(start,end,10-(4*(b_id/3)),2+(b_id%3));
  }
  else //side two
  { 
    // display_healthbar(7+(4*(b_id/3)),2+(b_id%3),hp_p);
    animate_healthbar(start,end,7+(4*(b_id/3)),2+(b_id%3));
  }
}

void update_healthbar(char b_id)
{
  int hp_p;
  bid_to_unit_header(b_id,0);
  apply_level_to_header(battleunits[b_id].unit->level,0);
  hp_p = get_percentage(battleunits[b_id].unit->hp,unit_header[0].hp);
  // add_battle_unit(3-(i/3)-j,3+(i%3),player,total_count,1,9,(j*3)+i,&entities[player].bg->units[(j*3)+i]);

  if(!(b_id/9))//side one
  {
    // put_number(battleunits[b_id].unit->hp,3,10-(4*(b_id/3)),2+(b_id%3));
    display_healthbar(10-(4*(b_id/3)),2+(b_id%3),hp_p);
  }
  else //side two
  { 
    // put_number(battleunits[b_id].unit->hp,3,7+(4*(b_id/3)),2+(b_id%3));
    display_healthbar(7+(4*(b_id/3)),2+(b_id%3),hp_p);
  }
}

void hide_healthbar(char unit_id)
{
	int x,y;
	spr_set(unit_id+MAX_EFFECT_COUNT);
	y = spr_get_y();
	x = spr_get_x();
	put_string("     ",x/8,y/8+4);
}

void heal_dmg_percent(char target_id, int percentage, int dmg)
{
  int heal_amt;

  bid_to_unit_header(target_id,1);
  apply_level_to_header(battleunits[target_id].unit->level,1);

  heal_amt = calc_percentage(dmg,percentage);
  set_heal(target_id,heal_amt,1);
}

void heal_health_percent(char target_id, int percentage)
{
  int heal_amt;
  bid_to_unit_header(target_id,0);
  apply_level_to_header(battleunits[target_id].unit->level,0);

  heal_amt = calc_percentage(unit_header[0].hp,percentage);

  set_heal(target_id,heal_amt,0);
  battleunits[target_id].target = 0;
}

void set_heal(char b_id, unsigned char amt, char header_id)
{
  apply_heal(b_id,min(battleunits[b_id].unit->hp + amt,unit_header[header_id].hp));
}

void apply_heal(char unit_id, int total_hp)
{
  battleunits[unit_id].unit->hp = total_hp;
  update_healthbar(unit_id);
}

void calc_hit_damage(char t_id, unsigned char damage, int bonus)
{
  //apply bonuses here?
  hits++;
  if(bonus)
  {
    apply_damage(t_id,damage+get_percentage(damage,bonus));
  }
  else
  {
    apply_damage(t_id,damage);
  }
}

unsigned char calc_damage(char a_id, char t_id, int a_level, int a_base, int a_bonus, int d_bonus, int d_base, int m_pow)
{
  int dmg = 0, adv_modifier = 0;
  //1 = crit in this case
  // dmg = ((((2 * a_level * (2*crit)) / 3) + 2) * m_pow) * a_base;
  // dmg /= d_base;
  // dmg += 2;
  // dmg /= 4;
  // put_number(dmg,4,0,0);
  // crit = 0;

  dmg = (m_pow + a_level) + ( max( (a_base + a_bonus) - (d_base+d_bonus),1) * (1 + crit));
  dmg /= 2;

  if(adv_plus_plus_flag)
  {
    adv_modifier = calc_percentage(50,dmg);
    dmg += adv_modifier;
  }
  else if(adv_plus_flag)
  {
    adv_modifier = calc_percentage(25,dmg);
    dmg += adv_modifier;
  }

  // put_number(adv_modifier,4,0,0);
  return min(dmg,255);
}

void apply_damage(char t_id, unsigned char damage)
{
  if(battleunits[t_id].ent_id != player_id)
  {
    damage_dealt += damage;
  }
  if(damage)//WHEN A UNIT LEVELS UP, IT'S HEALTH BAR SHRINKS BECAUSE IT GAINS MORE HEALTH THAN IT HAS
	{
    int hp_p_before, hp_p_after;

    bid_to_unit_header(t_id,1);
    apply_level_to_header(battleunits[t_id].unit->level,1);
    hp_p_before = get_percentage(battleunits[t_id].unit->hp,unit_header[1].hp);
    if(damage >= battleunits[t_id].unit->hp)
    {
      battleunits[t_id].unit->hp = 0;
      animate_health(t_id,hp_p_before,0);
    }
    else
    {
      battleunits[t_id].unit->hp = max(battleunits[t_id].unit->hp-damage,0);

      bid_to_unit_header(t_id,1);
      apply_level_to_header(battleunits[t_id].unit->level,1);
      hp_p_after = get_percentage(battleunits[t_id].unit->hp,unit_header[1].hp);

      animate_health(t_id,hp_p_before,hp_p_after);
    }
	}
}

int calc_art_damage(char attacker_id, int target_id)
{
  // return (unsigned char) max(((base_damage + party_commanders[entities[attacker_id].id].wis) * 4) / 6,0);
  // put_number(base_damage,5,0,0);
  // return base_damage;
  return 20;
}

void load_animations_to_vram(char attacker)
{
  char i, count;
  count = 0;
  transfer_units_to_attack_vram(attacker);

  for(i=0; i<18; i++)
  {
    if(battleunits[i].target)
    {
      // bid_to_unit_header(i,0);
      // transfer_units_to_stun_vram(battleunits[i].unit->unit->id,count++);
      // transfer_units_to_stun_vram(unit_header[0].id,count++);
      transfer_units_to_stun_vram(battleunits[i].unit->id,count++);
    }
  }
}

/*
	BATTLE ROUTINE
*/
char battle_loop(int i1, int i2, char range, char a_t, char t_t)
{
  char first_team, second_team;

  if(entities[i1].team == PLAYER)
  {
    first_team = ALLY_PALETTE;
    second_team = ENEMY_PALETTE;
  }
  else
  {
    first_team = ENEMY_PALETTE;
    second_team = ALLY_PALETTE;
  }
	attack_range = range;
	xOffset = -26;
	atker = i1, trgt = i2;
  cmdr_count = 28;
  crit = 0;
  attacker_effect = 0;
  target_effect = 0;

  attacking_bonuses = entities[i1].bg.bonuses;
  target_bonuses = entities[i2].bg.bonuses;

  // entities[i1].bg->meter = min(entities[i1].bg->meter+1,MAX_METER);
  // entities[i2].bg->meter = min(entities[i2].bg->meter+1,MAX_METER);
  if(entities[atker].team == PLAYER)
  {
    player_id = atker;
  }
  else
  {
    player_id = trgt;
  }
  // a_terrain = terrain_effect_by_type(terrain_type(a_t));
	t_terrain = terrain_def_bonus(t_t);//terrain_effect_by_type(terrain_type(t_t));

	//a_atk_bonus = 0;//terrain_atk_bonus(a_terrain);
	//a_def_bonus = 0;//terrain_def_bonus(a_terrain);
	//t_atk_bonus = 0;//terrain_atk_bonus(t_terrain);
	//t_def_bonus = 0;//terrain_def_bonus(t_terrain);
  screen_dimensions = 32;

	set_screen_size(SCR_SIZE_32x32);
  cls();

  load_palette(0,battlepal,1);
  set_map_data(battlemap,16,14);
  set_tile_data(battletiles,8,battletilespal,TILE_WIDTH);

  load_tile(TILE_VRAM);
  load_map(0,0,0,0,16,14);
  load_vram(0x4BB0,icons_gfx,0x60);
	load_vram(0x49A0,icons_gfx+0x60,0x50);
  load_vram(0x4DF0,icons_gfx+0xC0,0x10);
  // load_palette(30,effect_pal,1);
  // load_palette(31,lightening_effect_pal,1);

	load_healthbars();
  set_infobar();

	init_armies(i1,i2,first_team,second_team);
	scroll(0, 0, 0, 0, 223, 0xC0);

  // set_infobar();
  // display_battle_menu();
  // set_art_lists();
  load_palette(EFFECTS_WORD_PAL,effect_pal,1);
  // gfx_line(14,0,14,10,3);

  load_palette(0,battlepal,1);
  load_palette(14,redpal,1);
  load_palette(9,borderspal,1);
  
  disp_on();

  load_palette(10,fontpal,2);

  battle_seq();
  disp_off();
	reset_satb();
	cleanup_battle(i1,i2);
  
	if(team_one_count <= 0)
	{
    return ATTACKER_DEFEAT;
		// return 1;
	}

	if(team_two_count <= 0)
	{
		// return 0;
    return TARGET_DEFEAT;
	}
	// return 2;
  return NO_DEFEAT;
}

void set_portrait(char index, char entity_id)
{
  if(entities[entity_id].team == PLAYER)
  {
    load_portrait(party_commanders[entities[entity_id].id].sprite_type,index);
    if(index == 0)
    {
      display_item(0,0,0,6);
      // put_string("arts",6,1);
      // put_string("*+",10,1);
    }
    else
    {
      display_item(1,1,28,6);
      // put_string("arts",20,1);
      // put_string("*+",24,1);
    }
  }
  // else
  // {
  //   load_portrait(enemy_commanders[entities[entity_id].id].sprite_type,index);
  // }
}

void set_infobar()
{
  display_window_abs(0,0,16,6);
  display_window_abs(16,0,16,6);

  set_font_pal(9);
  put_char('F',11,1);
  put_char('M',7,1);
  put_char('R',3,1);

  // set_font_pal(12);

  put_char('F',20,1);
  put_char('M',24,1);
  put_char('R',28,1);

  set_font_pal(10);
  
  set_portrait(0,atker);
  set_portrait(1,trgt);
  // display_item(0,0,1,1);
  // display_item(1,1,27,1);

  // put_string("Pow",5,3);
  // put_string("Meter",9,3);

  // put_string("Pow",24,3);
  // put_string("Meter",18,3);

  // update_info_bar();
}

void display_battle_menu()
{
  char player_ent, unit_offset;
  player_ent = (entities[atker].team == PLAYER)? atker : trgt;
  unit_offset = (entities[atker].team == PLAYER)? 0 : 9;
  cursor_x = 11;
  cursor_y = 1;
  display_window_abs(10,0,12,12);
  display_cursor();
}

void update_info_bar()
{
  // put_number(calculate_power(entities[atker].id),3,5,4);
  // display_meter_bars(entities[atker].bg->meter,10,4);
  // put_number(calculate_power(entities[trgt].id),3,24,4);
  // display_meter_bars(entities[trgt].bg->meter,19,4);
}

void init_armies(int player, int cpu, char t_one, char t_two)
{
	int j,i;
	char total_count;

//	reset_npcs();
	team_one_count = 0;
	team_two_count = 0;
	total_count = 0;
  battle_clock = 0;

	for(j=0; j<3; j++)
	{
		for(i=0; i<3; i++, total_count++)
		{
      if(entities[player].bg->units[(j*3)+i].id)
      {
        add_battle_unit(3-(i/3)-j,3+(i%3),player,total_count,1,9,(j*3)+i,&entities[player].bg->units[(j*3)+i],t_one);
        update_healthbar(total_count);
        team_one_count++;
      }
    }
	}

	total_count = 9;
	for(j=0; j<3; j++)
	{
		for(i=0; i<3; i++, total_count++)
		{
      if(entities[cpu].bg->units[(j*3)+i].id)
      {
        add_battle_unit(j+4+(i/3),3+(i%3),cpu,total_count,1,0,(j*3)+i,&entities[cpu].bg->units[(j*3)+i],t_two);
        update_healthbar(total_count);
        team_two_count++;
      }
		}
	}

	for(j=0; j<9; j++)
	{
		spr_set(j+9+MAX_EFFECT_COUNT);
		spr_ctrl(FLIP_MAS,FLIP_X);
	}
	satb_update();
}

void hide_art_name()
{
  // load_map(0,3,0,3,16,4);
  load_map(0,5,0,5,16,9); 
}

void load_pals(char entity_id, int off)
{
	char i;

  load_palette(17,soldierpal,1);
  load_palette(18,enemypal,1);
  // load_palette(17,soldierpal,1); //sword,knight,lancer,monk,fighger,brawler,archer,spear
  // load_palette(18,magebtlpal,1);
  // load_palette(19,magebtlpal+16,1);
  // load_palette(21,blobbattlepal,1);
  // load_palette(20,golempal,1);
  // load_palette(22,monkbtlpal,1);
  // load_palette(25,houndbtlpal,1);
  // load_palette(27,magebtlpal,1);
  // load_palette(24,magebtlpal+32,1);
  // load_palette(23,raiderbtlpal,1);

	for(i=0; i<9; i++)
	{
		if(entities[entity_id].bg->units[i].hp)
		{
			switch(entities[entity_id].bg->units[i].id)
			{
        case REI:
          load_palette(cmdr_count++,rei_battle_pal,1);
          spr_set(MAX_EFFECT_COUNT+(i+off));
          spr_pal(cmdr_count-1);
          break;

        case VIOLET:
          load_palette(cmdr_count++,violet_battle_pal,1);
          spr_set(MAX_EFFECT_COUNT+(i+off));
          spr_pal(cmdr_count-1);
          break;
        
        case KING:
          load_palette(cmdr_count++,king_battle_pal,1);
          spr_set(MAX_EFFECT_COUNT+(i+off));
          spr_pal(cmdr_count-1);
          break;
			}
		}
	}
}

void kill_unit(char b_id)
{
  entities[battleunits[b_id].ent_id].bg->units[battleunits[b_id].pos].hp = 0;
  entities[battleunits[b_id].ent_id].bg->units[battleunits[b_id].pos].id = 0;// = &unit_list[NO_UNIT];
  battleunits[b_id].active = 0;

  if(entities[battleunits[b_id].ent_id].team == PLAYER)
  {
    units_lost++;
    battle_lost++;
  }
  else
  {
    units_killed++;
    battle_killed++;
    battle_exp += (20 * (int)battleunits[b_id].unit->level);
    // battle_exp += battleunits[b_id].unit->unit->exp;
  }
  if(battleunits[b_id].ent_id == atker)
  {
    team_one_count--;
  }
  else
  {
    team_two_count--;
  }
  set_bonuses(&party_commanders[entities[battleunits[b_id].ent_id].id]);
  spr_hide(MAX_EFFECT_COUNT+b_id);
  set_battle_bonuses();
}

void set_battle_bonuses()
{
  attacker_bonuses = entities[atker].bg->bonuses;
  target_bonuses = entities[trgt].bg->bonuses;
  if(attack_side == LEFT_SIDE)
  {
    attacking_bonuses = entities[atker].bg->bonuses;
    targeted_bonuses = entities[trgt].bg->bonuses;
  }
  else
  {
    attacking_bonuses = entities[trgt].bg->bonuses;
    targeted_bonuses = entities[atker].bg->bonuses;
  }
}

void blowback(char b_id)
{
  char abs_position;
  abs_position = b_id%9;
  
  if(battleunits[b_id].frame < 6)
  {
    slide_back(b_id,4);
    battleunits[b_id].frame++;
  }
  else
  {
    if((abs_position < 6) && !battleunits[b_id+3].active) //unit not behind
    {
      slide_back(b_id,4);
      battleunits[b_id].frame++;
    }
    else
    {
      slide_forward(b_id,4);
      battleunits[b_id].frame++;
    }
  }
}

void slide_back(char b_id, char amt)
{
  if(unit_direction(b_id)){ slide_right(b_id,amt); } else { slide_left(b_id,amt);}
}

void slide_forward(char b_id, char amt)
{
  if(unit_direction(b_id)){ slide_left(b_id,amt); } else { slide_right(b_id,amt); }
}

void slide_right(char b_id, char amt)
{
  spr_set(MAX_EFFECT_COUNT+b_id);
  spr_x(spr_get_x()+amt);
}

void slide_left(char b_id, char amt)
{
  spr_set(MAX_EFFECT_COUNT+b_id);
  spr_x(spr_get_x()-amt);
}

void shake_unit(char b_id)
{
  spr_set(b_id+MAX_EFFECT_COUNT);
    
  if(battleunits[b_id].frame % 2)
  {
    spr_x(spr_get_x(b_id+MAX_EFFECT_COUNT)+1);
  }
  else
  {
    spr_x(spr_get_x(b_id+MAX_EFFECT_COUNT)-1);
  }
  battleunits[b_id].frame++;
}

void cleanup_battle(int player_selected_index, int cpu_selected_index)
{
  char i;

  for(i=0; i<18; i++)
  {
    battleunits[i].active = 0;
    battleunits[i].frame = 0;
    battleunits[i].pal = 0;
    battleunits[i].state = 0;
    battleunits[i].target_team = 0;
    battleunits[i].pos = 0;
    battleunits[i].attacks = 0;
    battleunits[i].target = 0;
    battleunits[i].a_bonus = 0;
    battleunits[i].d_bonus = 0;
    battleunits[i].column = 0;
    battleunits[i].s_bonus = 0;
    battleunits[i].ent_id = 0;
    battleunits[i].p_bonus = 0;
    battleunits[i].unit = 0;
  }

  // picker = 0;
	total_units = 0;
  animating = 0;
  a_atk_bonus = 0;
  t_atk_bonus = 0;
  a_def_bonus = 0;
  t_def_bonus = 0;
  t_terrain = 0;
  // team_one_judgement = 0;
  // team_two_judgement = 0;
  position_status = 0;
  meter_queued = 0;
  unit_meter_queued = 0;
  battle_killed = 0;
  battle_lost = 0;
  battle_exp = 0;
  damage_dealt = 0;
  battle_phase = PHASE_START;

	spr_set(62);
	spr_hide();
	spr_set(63);
	spr_hide();
	satb_update();
	reset_satb();
}

void load_arts_by_targets(char art_type, char flip)
{
  char i;
  for(i=0; i<18; i++)
  {
    if(battleunits[i].target)
    {
      spr_set(i+MAX_EFFECT_COUNT);
      load_art(art_type,spr_get_x(),spr_get_y(),flip);
    }
  }
}

void set_targets_stunned(char b_id)
{
  char i;
  for(i=0; i<18; i++)
  {
    if(battleunits[i].target)
    {
      bid_to_unit_header(b_id,0);
      apply_level_to_header(battleunits[b_id].unit->level,0);
      bid_to_unit_header(i,1);
      apply_level_to_header(battleunits[i].unit->level,1);

      if(no_miss_flag == 0)
      {
        determine_miss(b_id,i);
      }
    
      if(battleunits[i].state != STATE_EVADE)
      {
        determine_crit(b_id);
        set_unit_stunned(i);
        if(crit)
        {
          spr_set(MAX_EFFECT_COUNT+b_id);
          create_crit_effect(spr_get_x(),spr_get_y());
          // wait_for_I_input();
        }
      }
      battleunits[i].frame = 0;
    }
  }
}

char unit_direction(char b_id)
{
  return (b_id>8)? 1 : 0;
}