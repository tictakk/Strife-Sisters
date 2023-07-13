#include "battle_menu.h"
#include "battle.h"

// #incbin(battlemap,"tiles/battletiles.battle_backgrounds.layer-Layer 1.map001.stm")
// #inctilepal(battletilespal,"tiles/battletiles.tiles.pcx")
// #inctile(battletiles,"tiles/battletiles.tiles.pcx")
// #incpal(battlepal,"tiles/battletiles.tiles.pcx")

int battle_exp = 0, damage_dealt = 0;
char battle_killed = 0, battle_lost = 0, player_id;

void countdown(char x, char y, char *str,char len)
{
  int time, i, j;
  if(simulation_mode)
  {
    return;
  }
  display_window_abs(x,y,len,3);
  put_string(str,x+1,y+1);
  j = 150;
  while(j-- > 0)
  {
    cycle_button_press(10,9);
    if(joytrg(0) == JOY_RUN)
    {
      battle_ctrls();
      display_window_abs(x,y,12,3);
      put_string(str,x+1,y+1);
      // put_number(time,3,x+7,y+1);
    }
    // if(j==0 && time > 0)
    // {
    //   j = 240;
    //   time--;
    // }
    // put_string(" ",x+8,y+1);
    vsync();
  }
  hide_art_name();
}

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
  display_window_abs(8,0,12,10);
  write_text(9,1,"damage");
  sync(30);
  display_number_incrementing(16,1,damage_dealt,3);
  write_text(9,2,"killed");
  sync(30);
  display_number_incrementing(18,2,battle_killed,1);
  write_text(9,3,"lost");
  sync(30);
  display_number_incrementing(18,3,battle_lost,1);
  write_text(9,4,"exp");
  sync(30);
  display_number_incrementing(16,4,battle_exp,3);

  party_commanders[entities[player_id].id].exp += battle_exp;

  while(party_commanders[entities[player_id].id].exp > next_level((int)party_commanders[entities[player_id].id].level))
  {
    write_text(9,6,"new level!");
    write_text(9,7,"level");
    put_number(++party_commanders[entities[player_id].id].level,2,16,7);
    level_commander(entities[player_id].id);
  }
  sync(60);
}

void battle_seq()
{
  load_pals(atker,0);
  load_pals(trgt,9);
  countdown(2,8,"Prepare",12);
  // countdown(2,8," Press Run for arts",22);
  d_battle(atker);
  if(clear_eyes_called == 0)
  {
    d_battle(trgt);
  }
  // countdown(3,2,8,"end "); countdown timer at the end for what?
  end_sequence();
}

void apply_modifier(char target, char modifier, char amount)
{
  switch(modifier)
  {
    case MOD_ATK:
    battleunits[target].a_bonus += amount;
    break;

    case MOD_DEF:
    battleunits[target].d_bonus += amount;
    break;

    case MOD_HP:
    // put_number(calc_art_damage(battleunits[art_unit_id].ent_id,amount),4,0,0);
    apply_damage(target,calc_art_damage(battleunits[art_unit_id].ent_id,amount));
    break;
  }
  update_healthbar(target);
}

void perform_art()
{
  if(art_queue_id == BLOW_BACK_ART)
  {
    position_status = STATUS_BLOWBACK;
  }
  
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
  if(do_art(art_unit_id) == 1)
  {
    hide_art_name();
    clear_targets();
    remove_effects();
  }
  clear_art_queue();
}

void animate_one_to_one_art()
{
  char i;

  for(i=0; i<18; i++)
  {
    if(battleunits[i].active && battleunits[i].target)
    {
      spr_set(i+5);
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

  spr_set(get_first_target()+5);
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
      spr_set(i+5);
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
  if(art_queue_id == BLOW_BACK_ART)
  {
    if((target%9) + 3 < 9 && battleunits[target+3].active)
    {
      modifier = 5;
    }
  }
  apply_modifier(target,get_modifier_type(art_queue_id),get_modifier_amount(art_queue_id)-modifier);
}

void end_sequence()
{
  update_info_bar();
  battle_end_screen();
  fade_screen();
}

void d_battle(char team)
{
  char i = 0;
  int b_ticker = 0;
  animating = 0;
  battle_clock = 0;
  
  while(battle_clock != -1)
  {
    if(joytrg(0) == JOY_RUN)
    {
      battle_ctrls();
    }
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
      if(battleunits[i].unit->unit->spd > hi_spd)
      {
        hi_spd = battleunits[i].unit->unit->spd;
        highest = i;
      }
    }
  }
  return highest;
}

void battle_unit_state(char b_id)
{
  if(battleunits[b_id].state == ATTACK){ b_u_attack(b_id); return; }
  if(battleunits[b_id].state == STUNNED){ b_u_stun(b_id); return; }
  if(battleunits[b_id].state == IDLE){ b_u_idle(b_id); return; }
  if(battleunits[b_id].state == WAITING) { b_u_wait(b_id); return; }
  if(battleunits[b_id].state == HEALING) { b_u_heal(b_id); return; }
  if(battleunits[b_id].state == METER_ATTACK) { b_u_meter(b_id); return; }
}

void b_u_meter(char b_id)
{
  if(battleunits[b_id].frame == 0)
  {
    display_window_rel(2,8,12,3);
    put_string(arts[battleunits[b_id].unit->unit->art].name,3,9);
  }
  if(battleunits[b_id].frame < 6)//animating attack
  {
    spr_set(b_id+5);
    animate_attack(b_id,battleunits[b_id].frame++);
  }
  else if(battleunits[b_id].frame == 7)
  {
    // spr_set(0);
    // spr_show();
    battleunits[b_id].frame++;
    // load_art_by_relationship(b_id);
  }
  else if(battleunits[b_id].frame < 11)
  {
    spr_set(b_id+5);
    animate_attack(b_id,battleunits[b_id].frame++);
  }
  // else if(battleunits[b_id].frame < (12+arts[battleunits[b_id].unit->unit->art].frame_count))
  // {
  //   battleunits[b_id].frame++;
  // }
  else
  {
    battleunits[b_id].attacks--;
    set_unit_waiting(b_id);
    art_queued = 1;
    art_unit_id = b_id;
    art_queue_id = battleunits[b_id].unit->unit->art;
  }
}

void b_u_idle(char b_id)
{
  if(battle_clock == b_id && battleunits[b_id].active
    // && entities[battleunits[b_id].ent_id].bg->units[battleunits[b_id].pos]->unit.rng >= attack_range
    && animating == 0)
  {
    if(battleunits[b_id].meter)
    {
      // determine_action(b_id,arts[battleunits[b_id].unit->unit->art].target);
      set_meter_targets();
      // set_unit_attack(b_id);
      load_animations_to_vram(battleunits[b_id].unit->unit->id);
      set_unit_meter(b_id);
      battleunits[b_id].meter = 0;
      animating++;

      update_info_bar();
    }
    else
    {
      if(battleunits[b_id].unit->unit->id == PRIEST_UNIT || battleunits[b_id].unit->unit->id == CLERIC_UNIT)
      {
        determine_action(b_id,HEAL);
      }
      else
      {
        determine_action(b_id,SINGLE_HIT);
      }
    }
  }
}

void b_u_attack(char b_id)
{
  if(battleunits[b_id].frame == 0) //check if unit has advantage
  {
    char i;
    unsigned char adv;
    if(!sea_legs_art)
    {
      adv = 0;
      for(i=0; i<18; i++)
      {
        if(battleunits[i].active && battleunits[i].target)
        {
          adv = check_advantage(unit_list[battleunits[b_id].unit->unit->id].a_type,battleunits[i].unit->unit->a_type);
          if(adv)
          {
            break;
          }
        }
      }

      if(adv)
      {
        spr_set(b_id+5);
        create_effect(EFFECT_ADV,spr_get_x(),spr_get_y(),0);
        spr_show();
      }
    }
  }
  if(battleunits[b_id].frame == 6)
  {
    set_targets_stunned();
  }
  if(battleunits[b_id].frame < 11)//animating attack
  {
    animate_attack(b_id,battleunits[b_id].frame++);
  }
  else //finish animation, calculate damage
  {
    char i;
    for(i=0; i<18; i++)
    {
      if(battleunits[i].target)
      {
        if(battleunits[b_id].ent_id == atker)
        {
          calc_hit_damage(b_id,i,a_atk_bonus,0,t_def_bonus);
        }
        else
        {
          calc_hit_damage(b_id,i,t_atk_bonus,0,a_def_bonus);
        }
      }
    }
    set_unit_waiting(b_id);
    battleunits[b_id].attacks--;
    remove_effects();
  }
}

void b_u_stun(char b_id){
  // put_number(b_id,3,10,0);
  // wait_for_I_input();
  if(battleunits[b_id].frame == 0)
  {
    spr_set(b_id+5);
    spr_pattern(stun_vrams[stun_count++]);
    battleunits[b_id].frame++;
    // create_hit_spark(spr_get_x(),spr_get_y(),0);
    // spr_show();
  }
  else if(battleunits[b_id].frame < 11)
  {
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
        spr_set(j+5);
        create_healing(spr_get_x()+8,spr_get_y()+16,0);
      }
    }
  }
  if(battleunits[b_id].frame < 11)
  {
    animate_attack(b_id,battleunits[b_id].frame++);
  }
  else
  {
    char i;
    for(i=0; i<18; i++)
    {
      if(battleunits[i].target)
      {
        calc_heal(b_id,i,0,0,0);
      }
    }
    set_unit_waiting(b_id);
    battleunits[b_id].attacks--;
    //This may cause an issue when there may be a time when heal is done at the same time as another
    //effect. This erases all effects.
    // remove_effects();
  }
}

void animate_attack(char unit_id, char hit_frames)
{
	spr_set(unit_id+5);
	spr_pattern(attack_vrams[0]+ATTACK_ANIMATIONS[hit_frames]);
}

void update_healthbar(char b_id)
{
  int hp_p;
  hp_p = get_percentage(battleunits[b_id].unit->hp,battleunits[b_id].unit->unit->hp);

  spr_set(b_id+5);
  display_healthbar((spr_get_x()/8),(spr_get_y()/8)+4,(char)hp_p);
}

void hide_healthbar(char unit_id)
{
	int x,y;
	spr_set(unit_id+5);
	y = spr_get_y();
	x = spr_get_x();
	put_string("     ",x/8,y/8+4);
}

void calc_heal(char healer_id, char target_id, char a_a_bonus, char t_a_bonus, char t_d_bonus)
{
  if(get_percentage(battleunits[target_id].unit->hp,battleunits[target_id].unit->unit->hp)<100)
  {
    int heal_amt;
    heal_amt = calc_percentage(20,battleunits[target_id].unit->unit->hp);

    battleunits[target_id].unit->hp = min(battleunits[target_id].unit->hp + heal_amt,battleunits[target_id].unit->unit->hp);
  }
  battleunits[target_id].target = 0;
  spr_set(target_id+5);
  update_healthbar(target_id);
}

void calc_hit_damage(char attacker_id, char target_id, char a_a_bonus, char t_a_bonus, char t_d_bonus)
{
  unsigned char dmg;
  dmg = calc_damage(attacker_id,target_id,a_a_bonus,t_d_bonus);
  apply_damage(target_id,dmg);
  battleunits[target_id].target = 0;
}

unsigned char calc_damage(char a_id, char t_id, char a_a_bonus, char t_d_bonus)
{
  int a_base_atk, t_base_def;
	int t_def, a_atk, dmg, cmdr_attack_bonus, cmdr_defense_bonus;
  unsigned char adv;

  adv = check_advantage(battleunits[a_id].unit->unit->a_type,battleunits[t_id].unit->unit->a_type);
  a_base_atk = entities[battleunits[a_id].ent_id].bg->units[battleunits[a_id].pos].unit->atk;
  t_base_def = entities[battleunits[t_id].ent_id].bg->units[battleunits[t_id].pos].unit->def;

  cmdr_attack_bonus = party_commanders[entities[battleunits[a_id].ent_id].id].tac;
  cmdr_defense_bonus = party_commanders[entities[battleunits[t_id].ent_id].id].fort;

  a_atk = cmdr_attack_bonus + a_base_atk;
  t_def = cmdr_defense_bonus + t_base_def;

  dmg = max(((a_atk * 4) - (t_def * 2))/6,0);
  // put_number(dmg,4,0,0);
  // if(adv && !sea_legs_art)
  // {
  //   dmg *= 2;
  // }
  return (char) dmg;
}

void apply_damage(char t_id, unsigned char damage)
{
  if(battleunits[t_id].ent_id != player_id)
  {
    damage_dealt += damage;
    battle_exp += calc_percentage(25,damage);
  }
  if(damage)
	{
    battleunits[t_id].unit->hp = max(battleunits[t_id].unit->hp-damage,0);
	}
}

unsigned char calc_art_damage(char attacker_id, int base_damage)
{
  return (unsigned char) max(((base_damage + party_commanders[entities[attacker_id].id].wis) * 4) / 6,0);
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
      transfer_units_to_stun_vram(battleunits[i].unit->unit->id,count++);
    }
  }
}

/*
	BATTLE ROUTINE
*/
char battle_loop(int i1, int i2, char range, char a_t, char t_t, char art, char a_target)
{
	attack_range = range;
	xOffset = -26;
	atker = i1, trgt = i2;

  entities[i1].bg->meter = min(entities[i1].bg->meter+1,MAX_METER);
  entities[i2].bg->meter = min(entities[i2].bg->meter+1,MAX_METER);
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

	load_healthbars();

	init_armies(i1,i2);
	scroll(0, 0, 0, 0, 223, 0xC0);

  set_infobar();
  // display_battle_menu();
  // set_art_lists();
  load_palette(EFFECTS_WORD_PAL,effect_pal,1);
  
  battle_seq();
	reset_satb();
	cleanup_battle(i1,i2);

	if(team_one_count <= 0)
	{
		return 1;
	}

	if(team_two_count <= 0)
	{
		return 0;
	}
	return 2;
}

void set_portrait(char index, char entity_id)
{
  if(entities[entity_id].team == PLAYER)
  {
    load_portrait(party_commanders[entities[entity_id].id].sprite_type,index);
    if(index == 0)
    {
      display_item(0,0,1,1);
      put_string("arts",6,1);
      put_string("*+",10,1);
    }
    else
    {
      display_item(1,1,27,1);
      put_string("arts",20,1);
      put_string("*+",24,1);
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
  
  set_portrait(0,atker);
  set_portrait(1,trgt);
  // display_item(0,0,1,1);
  // display_item(1,1,27,1);

  put_string("Pow",5,3);
  put_string("Meter",9,3);

  put_string("Pow",24,3);
  put_string("Meter",18,3);

  update_info_bar();
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
  print_army_combos(player_ent,unit_offset);
}

void battle_ctrls()
{
  char loop_ctrls, curs_pos;
  if(meter_queued)
  {
    return;
  }
  loop_ctrls = 1;
  display_battle_menu();
  while(loop_ctrls)
  {
    switch(joytrg(0))
    {
      case JOY_DOWN:
      if(cursor_y < 10)
      {
        curs_down();
      }
      break;

      case JOY_UP:
      if(cursor_y > 1)
      {
        curs_up();
      }
      break;

      case JOY_I:
      if(entities[trgt].team == PLAYER)
      {
        curs_pos = (cursor_y - 1) + 9;
        if(battleunits[curs_pos].active){ unit_meter_queued = curs_pos; }
      }
      else
      {
        curs_pos = cursor_y - 1;
        if(battleunits[curs_pos].active){ unit_meter_queued = curs_pos; }
      }
      if(battleunits[curs_pos].active && arts[battleunits[curs_pos].unit->unit->art].cost <= entities[battleunits[curs_pos].ent_id].bg->meter)
      {
        // display_battle_selector(battleunits[curs_pos].ent_id == atker,arts[battleunits[curs_pos].unit->unit->art].target);
        // while(display_battle_selector(battleunits[curs_pos].ent_id == atker,battleunits))
        // {
          // put_number(curs_pos,3,6,0);
          if(display_battle_selector(battleunits[curs_pos].ent_id == atker,arts[battleunits[curs_pos].unit->unit->art].target))
          {
            if(is_valid_meter_targets())
            {
              entities[battleunits[curs_pos].ent_id].bg->meter -= arts[battleunits[curs_pos].unit->unit->art].cost;
              loop_ctrls = 0;
              meter_queued = 1;
              battleunits[curs_pos].meter = 1;
              battleunits[curs_pos].attacks++;
              highlight_target_type(0);
              clear_bs_settings();
            }
            else
            {
              display_window_abs(22,7,10,4);
              // put_string(str,x+1,y+1);
              put_string("invalid",23,8);
              put_string("target",23,9);
              highlight_target_type(0);
              sync(80);
              clear_bs_settings();
              picker = 0;
              load_map(27,3,0,3,5,4);
            }
          } 
        // }
      }
      break;

      case JOY_II:
      loop_ctrls = 0;
      break;
    }
    vsync();
  }
  // set_infobar();
  hide_art_name();
  set_infobar();
}

void print_army_combos(char entity_id, char unit_offset)
{
  char i,j;
  j=0;
  for(i=0; i<MAX_ARMY_SIZE; i++)
  {
    if(entities[entity_id].bg->units[i].hp && battleunits[i+unit_offset].active)
    {
      if(arts[entities[entity_id].bg->units[i].unit->art].cost > entities[entity_id].bg->meter)
      {
        set_font_pal(11);
      }
      put_string(arts[entities[entity_id].bg->units[i].unit->art].name,12,1+j);
      print_target_type_icon(arts[entities[entity_id].bg->units[i].unit->art].target,20,1+j++);
      set_font_pal(10);
    }
    else
    {
      put_string("         ",12,1+j++);
    }
    // if(arts[attacker_art_list[i+unit_offset]].cost > entities[entity_id].bg->meter)
    // {
    //   set_font_pal(11);
    // }
    // put_string(arts[attacker_art_list[i+unit_offset]].name,12,1+j++);
    // set_font_pal(10);
  }
}

void update_info_bar()
{
  put_number(calculate_power(entities[atker].id),3,5,4);
  display_meter_bars(entities[atker].bg->meter,10,4);
  put_number(calculate_power(entities[trgt].id),3,24,4);
  display_meter_bars(entities[trgt].bg->meter,19,4);
}

void init_armies(int player, int cpu)
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
      if(entities[player].bg->units[(j*3)+i]->unit.id)
      {
        add_battle_unit(3-(i/3)-j,3+(i%3),player,total_count,1,9,(j*3)+i,&entities[player].bg->units[(j*3)+i]);
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
      if(entities[cpu].bg->units[(j*3)+i]->unit.id)
      {
        add_battle_unit(j+4+(i/3),3+(i%3),cpu,total_count,1,0,(j*3)+i,&entities[cpu].bg->units[(j*3)+i]);
        update_healthbar(total_count);
        team_two_count++;
      }
		}
	}

	for(j=0; j<9; j++)
	{
		spr_set(j+9+5);
		spr_ctrl(FLIP_MAS,FLIP_X);
	}
	satb_update();
}

void hide_art_name()
{
  load_map(0,3,0,3,16,4);
}

void load_pals(char entity_id, int off)
{
	char i;
	for(i=0; i<9; i++)
	{
		if(entities[entity_id].bg->units[i].hp)
		{
			switch(entities[entity_id].bg->units[i].unit->id)
			{
				case BLOB_UNIT:
          load_palette(battleunits[i+off].pal,sniperbtlpal,1);
          break;

				case SWORD_UNIT:
          load_palette(battleunits[i+off].pal,soldierpal,1);
          break;

        case PALADIN_UNIT:
          load_palette(battleunits[i+off].pal,paladinpal,1);
          break;

        case KNIGHT_UNIT:
        case LANCER_UNIT:
				case SPEAR_UNIT:
          load_palette(battleunits[i+off].pal,spearpal,1);
          break;

        case STALKER_UNIT:
          load_palette(battleunits[i+off].pal,stalkerpal,1);
          break;

				case ARCHER_UNIT:
          load_palette(battleunits[i+off].pal,musketbtlpal,1);
          break;

        case GOLEM_UNIT:
          load_palette(battleunits[i+off].pal,golempal,1);
          break;
        
        case BERSERKER_UNIT:
        case SNIPER_UNIT:
          load_palette(battleunits[i+off].pal,sniperbtlpal,1);
          break;

				case DEMON_UNIT:
          load_palette(battleunits[i+off].pal,demonbtlpal,1);
          break;

				case HOUND_UNIT:
          load_palette(battleunits[i+off].pal,houndbtlpal,1);
          break;

				case AXE_UNIT:
          load_palette(battleunits[i+off].pal,axebtlpal,1);
          break;

        case RAIDER_UNIT:
          load_palette(battleunits[i+off].pal,raiderbtlpal,1);
          break;

        case PRIEST_UNIT:
        case WITCH_UNIT:
          load_palette(battleunits[i+off].pal,witchbtlpal,1);
          break;

        case MAGE_UNIT:
          load_palette(battleunits[i+off].pal,magebtlpal,1);
          break;

        case CLERIC_UNIT:
          load_palette(battleunits[i+off].pal,magebtlpal+16,1);
          break;

        case BLACK_MAGE_UNIT:
          load_palette(battleunits[i+off].pal,magebtlpal+32,1);
          break;

        case BRAWLER_UNIT:
        case FIGHTER_UNIT:
        case MONK_UNIT:
          load_palette(battleunits[i+off].pal,monkbtlpal,1);
          break;
			}
		}
	}
}

void kill_unit(char b_id)
{
  if(battleunits[b_id].ent_id == atker && team_one_judgement || 
     battleunits[b_id].ent_id == trgt && team_two_judgement
    )
  {
    entities[battleunits[b_id].ent_id].bg->units[battleunits[b_id].pos].hp = 1;
    return;
  }
  entities[battleunits[b_id].ent_id].bg->units[battleunits[b_id].pos].hp = 0;
  entities[battleunits[b_id].ent_id].bg->units[battleunits[b_id].pos].unit = &unit_list[NO_UNIT];
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
    battle_exp += battleunits[b_id].unit->unit->exp;
  }
  if(battleunits[b_id].ent_id == atker)
  {
    team_one_count--;
  }
  else
  {
    team_two_count--;
  }
  if(team_one_tracked == b_id)
  {
    team_one_tracked = 0;
    team_one_tracking = 0;
  }
  if(team_two_tracked = b_id)
  {
    team_two_tracked = 0;
    team_two_tracking = 0;
  }
  spr_hide(5+b_id);
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
  spr_set(5+b_id);
  spr_x(spr_get_x()+amt);
}

void slide_left(char b_id, char amt)
{
  spr_set(5+b_id);
  spr_x(spr_get_x()-amt);
}

void shake_unit(char b_id)
{
  spr_set(b_id+5);
    
  if(battleunits[b_id].frame % 2)
  {
    spr_x(spr_get_x(b_id+5)+1);
  }
  else
  {
    spr_x(spr_get_x(b_id+5)-1);
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
  }

  picker = 0;
  clear_eyes_called = 0;
	total_units = 0;
  animating = 0;
  a_atk_bonus = 0;
  t_atk_bonus = 0;
  a_def_bonus = 0;
  t_def_bonus = 0;
  t_terrain = 0;
  team_one_judgement = 0;
  team_two_judgement = 0;
  position_status = 0;
  meter_queued = 0;
  unit_meter_queued = 0;
  battle_killed = 0;
  battle_lost = 0;
  battle_exp = 0;
  damage_dealt = 0;

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
      spr_set(i+5);
      load_art(art_type,spr_get_x(),spr_get_y(),flip);
    }
  }
}

void set_targets_stunned()
{
  char i;
  for(i=0; i<18; i++)
  {
    if(battleunits[i].target)
    {
      set_unit_stunned(i);
      battleunits[i].frame = 0;
    }
  }
}

char unit_direction(char b_id)
{
  return (b_id>8)? 1 : 0;
}

void set_meter_targets()
{
  char i, offset;
  offset = (picker == PICKER_LEFT)? 0 : 9;
  for(i=0; i<MAX_ARMY_SIZE; i++)
  {
    if(battleunits[offset+i].active && marked_targets[i])
    {
      battleunits[offset+i].target = 1;
    }
  }
  unmark_all();
  picker = 0;
}

char is_valid_meter_targets()
{
  char i, offset;
  offset = (picker == PICKER_LEFT)? 0 : 9;
  for(i=0; i<MAX_ARMY_SIZE; i++)
  {
    if(battleunits[offset+i].active && marked_targets[i])
    {
      return 1;
    }
  }
  return 0;
}
