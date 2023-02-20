#include "battle.h"
#incbin(battlemap,"tiles/battletiles.battle_backgrounds.layer-Layer 1.map001.stm")
#inctilepal(battletilespal,"tiles/battletiles.tiles.pcx")
#inctile(battletiles,"tiles/battletiles.tiles.pcx")
#incpal(battlepal,"tiles/battletiles.tiles.pcx")

char atker, trgt;

void battle_seq()
{
  load_pals(atker,0);
  load_pals(trgt,9);

  battle_clock = get_highest_speed(atker);
  d_battle(atker);
  battle_clock = get_highest_speed(trgt);
  d_battle(trgt);
  end_sequence();
}

void end_sequence()
{
  update_info_bar();
  fade_screen();
}

void d_battle(char team)
{
  char i = 0;
  int b_ticker = 0;
  animating = 0;
  
  while(battle_clock != -1)
  {
    if(b_ticker++ == 2)
    {
      b_ticker = 0;

      if(animating == 0)
      {
        battle_clock = get_highest_speed(team);
      }
      stun_count = 0;
      for(i=0; i<18; i++)
      {
        battle_unit_state(i);
      }
      animate_effects();
    }
    satb_update();
    vsync();
  }
}

void complete_art_damage(char art_type)
{
  char i, amt;
  amt = arts[art_type].modifier_amt;
  for(i=0; i<18; i++)
  {
    if(battleunits[i].target)
    {
      battleunits[i].unit->hp = min(battleunits[i].unit->hp+amt,battleunits[i].unit->unit->hp);
      if(battleunits->unit->hp <= 0)
      {
        kill_unit(target);
      }
      battleunits[i].target = 0;
      update_healthbar(i);
    }
  }
}

char get_highest_speed(char team)
{
  char i, hi_spd, highest;

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
//  put_string("id",0,0);
//  put_string("state:",3,0);
//  put_string("trgt:",11,0);
//  if(battleunits[b_id].active)
//  {
//    put_number(b_id,2,1,b_id+2);
//    put_number(battleunits[b_id].state,2,7,b_id+2);
//    put_number(battleunits[b_id].target,3,13,b_id+2);
//  }

  if(battleunits[b_id].state == ATTACK){ b_u_attack(b_id); return; }
  if(battleunits[b_id].state == STUNNED){ b_u_stun(b_id); return; }
  if(battleunits[b_id].state == IDLE){ b_u_idle(b_id); return; }
  if(battleunits[b_id].state == WAITING) { b_u_wait(b_id); return; }
  if(battleunits[b_id].state == HEALING) { b_u_heal(b_id); return; }
  if(battleunits[b_id].state == METER_ATTACK) { b_u_meter(b_id); return; }
}

void b_u_meter(char b_id)
{
  if(battleunits[b_id].frame < 6)//animating attack
  {
    spr_set(b_id+5);
    animate_attack(b_id,battleunits[b_id].frame++);
  }
  else if(battleunits[b_id].frame == 7)
  {
    spr_set(0);
    spr_show();
    battleunits[b_id].frame++;
    load_art_by_relationship(b_id);
  }
  else if(battleunits[b_id].frame < 11)
  {
    spr_set(b_id+5);
    animate_attack(b_id,battleunits[b_id].frame++);
  }
  else if(battleunits[b_id].frame < (12+arts[battleunits[b_id].unit->unit->art].frame_count))
  {
    battleunits[b_id].frame++;
  }
  else
  {
    set_targets_stunned();
    set_unit_waiting(b_id);
    battleunits[b_id].attacks--;

    complete_art_damage(battleunits[b_id].unit->unit->art);
    remove_effects();
    hide_art_name();
  }
}

void b_u_idle(char b_id)
{
    if(battle_clock == b_id && battleunits[b_id].active
      && entities[battleunits[b_id].ent_id].bg->units[battleunits[b_id].pos]->unit.rng >= attack_range
      && animating == 0)
  {
    if(battleunits[b_id].unit->meter == MAX_UNIT_METER)
    {
      determine_action(b_id,arts[battleunits[b_id].unit->unit->art].target);
      set_unit_meter(b_id);
      // spr_set(get_first_target()+5);
      // load_arts_by_targets(battleunits[b_id].unit->unit->art,(b_id>8)? 1 : 0);
      battleunits[b_id].unit->meter = 0;
    }
    else
    {
      // determine_action(b_id,battleunits[b_id].unit->unit->attacks[battleunits[b_id].pos/3]);
      determine_action(b_id,SINGLE_HIT);
    }
  }
}

void b_u_attack(char b_id)
{    
  if(battleunits[b_id].frame == 0) //check if unit has advantage
  {
    char i;
    unsigned char adv;
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
  if(battleunits[b_id].frame == 0)
  {
    spr_set(b_id+5);
    spr_pattern(stun_vrams[stun_count++]);
    battleunits[b_id].frame++;
    create_hit_spark(spr_get_x(),spr_get_y(),0);
    spr_show();
  }
  else if(battleunits[b_id].frame < 7)
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
  else
  {
    set_unit_waiting(b_id);
    update_healthbar(b_id);

    if(entities[battleunits[b_id].ent_id].bg->units[battleunits[b_id].pos].hp <= 0)
    {
      kill_unit(b_id);
      spr_hide();
    }
  }
}

void b_u_wait(char b_id)
{
  if(animating == 0)
  {
    battleunits[b_id].state = IDLE;
    battleunits[b_id].target = 0;
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
    remove_effects();
  }
}

void animate_attack(char unit_id, char hit_frames)
{
	spr_set(unit_id+5);
	spr_pattern(attack_vrams[0]+ATTACK_ANIMATIONS[hit_frames]);
}

void update_healthbar(char b_id)
{
  // int hp, hp_p;

  // hp = battleunits[b_id].unit->hp * 100;
  // hp_p = (hp<)hp / battleunits[b_id].unit->unit->hp;
  int hp_p;
  hp_p = get_percentage(battleunits[b_id].unit->hp,battleunits[b_id].unit->unit->hp);

  // wait_for_I_input();
  // put_number(b_id,4,0,5);
  // put_number(hp,4,0,6);
  // put_number(hp_p,4,0,7);
  // put_number(spr_get_x(),4,0,8);

  // wait_for_I_input();
  spr_set(b_id+5);
  display_healthbar(spr_get_x()/8,(spr_get_y()/8)+4,(char)hp_p);
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
  battleunits[healer_id].unit->meter = min(battleunits[healer_id].unit->meter+1,MAX_UNIT_METER);
  spr_set(target_id+5);
  update_healthbar(target_id);
}

void calc_hit_damage(char attacker_id, char target_id, char a_a_bonus, char t_a_bonus, char t_d_bonus)
{
	int t_hp, t_def,
			a_atk, fifth, dmg;

  unsigned char adv;

  adv = check_advantage(battleunits[attacker_id].unit->unit->a_type,battleunits[target_id].unit->unit->a_type);

	t_hp  = entities[battleunits[target_id].ent_id].bg->units[battleunits[target_id].pos].hp;
	t_def = entities[battleunits[target_id].ent_id].bg->units[battleunits[target_id].pos].unit->def + t_d_bonus;

	a_atk = entities[battleunits[attacker_id].ent_id].bg->units[battleunits[attacker_id].pos].unit->atk + a_a_bonus;

	dmg = max((a_atk - t_def),0);

  if(adv)
  {
    dmg *= 2;
    battleunits[attacker_id].unit->meter = min(battleunits[attacker_id].unit->meter+5,MAX_UNIT_METER);
  }
  else
  {
    battleunits[attacker_id].unit->meter = min(battleunits[attacker_id].unit->meter+1,MAX_UNIT_METER);
  }

	if(dmg)
	{
    entities[battleunits[target_id].ent_id].bg->units[battleunits[target_id].pos].hp -= dmg;
    battleunits[target_id].unit->meter = min(battleunits[target_id].unit->meter+1,MAX_UNIT_METER);
	}
  battleunits[target_id].target = 0;
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
	xOffset = -36;
	atker = i1, trgt = i2;
  speed_divider = 10;

  // a_terrain = terrain_effect_by_type(terrain_type(a_t));
	t_terrain = terrain_def_bonus(t_t);//terrain_effect_by_type(terrain_type(t_t));
  if(active_player_calling == CALLING_ATK_BUFF)
  {
    if(entities[i1].team == PLAYER)
    {
      a_atk_bonus = 5;
    }
    else
    {
      t_atk_bonus = 5;
    }
  }

  if(active_cpu_calling == CALLING_ATK_BUFF)
  {
    if(entities[i1].team == CPU)
    {
      a_atk_bonus = 5;
    }
    else
    {
      t_atk_bonus = 5;
    }
  }

  if(active_player_calling == CALLING_DEF_BUFF)
  {
    if(entities[i1].team == PLAYER)
    {
      a_def_bonus = 5;
    }
    else
    {
      t_def_bonus = 5;
    }
  }

  if(active_cpu_calling == CALLING_DEF_BUFF)
  {
    if(entities[i1].team == CPU)
    {
      a_def_bonus = 5;
    }
    else
    {
      t_def_bonus = 5;
    }
  }

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

	init_armies(i1,i2);
	scroll(0, 0, 0, 0, 223, 0xC0);

	load_healthbars();
  set_infobar();

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
  }
  else
  {
    load_portrait(enemy_commanders[entities[entity_id].id].sprite_type,index);
  }
}

void set_infobar()
{
  display_window(0,0,16,6);
  display_window(16,0,16,6);
  set_portrait(0,atker);
  set_portrait(1,trgt);
  display_item(0,0,1,1);
  display_item(1,1,27,1);

  // put_string("Lv",5,1);
  // put_number(1,2,7,1);
  put_number(a_atk_bonus,3,5,1);
  put_number(a_def_bonus,3,8,1);
  put_string("Pow",5,3);
  put_number(136,3,5,4);
  put_number(calculate_power(entities[atker].id),3,5,4);
  put_string("Meter",9,3);
  put_number(party_commanders[entities[atker].id].meter,3,10,4);

  // put_string("Lv",23,1);
  // put_number(3,2,25,1);
  put_number(t_atk_bonus,3,20,1);
  put_number(t_def_bonus,3,23,1);
  put_string("Pow",24,3);
  put_number(calculate_power(entities[trgt].id),3,24,4);
  put_string("Meter",18,3);
  put_number(party_commanders[entities[trgt].id].meter,3,19,4);
}

void update_info_bar()
{
  put_number(calculate_power(entities[atker].id),3,5,4);
  put_number(party_commanders[entities[atker].id].meter,3,10,4);
  put_number(calculate_power(entities[trgt].id),3,24,4);
  put_number(party_commanders[entities[trgt].id].meter,3,19,4);
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
  load_map(0,4,0,32,16,2);
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
          load_palette(battleunits[i+off].pal,blobbattlepal,1);
          break;

				case SWORD_UNIT:
          load_palette(battleunits[i+off].pal,soldierpal,1);
          break;

        case LANCER_UNIT:
        case KNIGHT_UNIT:
				case SPEAR_UNIT:
          load_palette(battleunits[i+off].pal,spearpal,1);
          break;

				case ARCHER_UNIT:
          load_palette(battleunits[i+off].pal,musketbtlpal,1);
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

        case MAGE_UNIT:
          load_palette(battleunits[i+off].pal,magebtlpal,1);
          break;

        case CLERIC_UNIT:
          load_palette(battleunits[i+off].pal,magebtlpal+16,1);
          break;

        case MONK_UNIT:
          load_palette(battleunits[i+off].pal,monkbtlpal,1);
          break;
			}
		}
	}
}

void kill_unit(char b_id)
{
  // char cmdr_id;

  entities[battleunits[b_id].ent_id].bg->units[battleunits[b_id].pos].hp = 0;
  entities[battleunits[b_id].ent_id].bg->units[battleunits[b_id].pos].unit = &unit_list[NO_UNIT];
  battleunits[b_id].active = 0;
  if(battleunits[b_id].ent_id == atker)
  {
    team_one_count--;
  }
  else
  {
    team_two_count--;
  }
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
  }

	total_units = 0;
  animating = 0;
  a_atk_bonus = 0;
  t_atk_bonus = 0;
  a_def_bonus = 0;
  t_def_bonus = 0;
  t_terrain = 0;

	spr_set(62);
	spr_hide();
	spr_set(63);
	spr_hide();
	satb_update();
	reset_satb();
}

void load_art_by_relationship(char b_id)
{
  switch(arts[battleunits[b_id].unit->unit->art].relationship)
  {
    case ONE_TO_MANY:
    case ONE_TO_ONE:
    spr_set(get_first_target()+5);
    load_art(battleunits[b_id].unit->unit->art,spr_get_x(),spr_get_y(),unit_direction(b_id));
    break;

    case MANY_TO_MANY:
    load_arts_by_targets(battleunits[b_id].unit->unit->art,unit_direction(b_id));
    break;
  }
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
