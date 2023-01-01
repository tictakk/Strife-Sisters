#incchr(battle,"map/sprites/bg.pcx")
#incpal(battlepal, "map/sprites/bg.pcx")
#incbat(battlebat, "map/sprites/bg.pcx",0x1000,32,28)
#incchr(grass, "map/backgrounds/grass.pcx")
#incpal(grasspal, "map/backgrounds/grass.pcx")

#define TILES_PER_LINE 14
#define ARMY_ONE_START 5
#define ARMY_TWO_START 21
#define HEALTHBAR_SIZE 5
#include "battle.h"

int attack_turns;
int attacks;
char ticker = 0;
char team_one_kills = 0;
char team_two_kills = 0;

char atker, trgt;

#define COMMANDER_ONE_VRAM 0x2200
#define COMMANDER_TWO_VRAM 0x3200
#define PLAYER_SPRITE_VRAM 0x6000
#define CPU_SPRITE_VRAM 0x5000

const char batty[1024];

int roundUp(int num, int div)
{
	int whole, remainder;
	whole = num/div;
  if (div == 0)
      return 0;

  remainder = num % div; //1
  if (remainder == 0)
      return whole;
	else
			return whole+1;
}

void battle_seq()
{
  char i = 0;
  int b_ticker = 0;
  animating = 0;

  load_pals(atker,0);
  load_pals(trgt,9);

  battle_clock = get_highest_speed();
//  put_string("id",0,0);
//  put_string("state:",3,0);
//  put_string("trgt:",11,0);
  while(battle_clock != -1)
  {
    if(b_ticker++ == 4)
    {
      b_ticker = 0;
      put_number(animating,3,10,0);
      if(animating == 0)
      {
        battle_clock = get_highest_speed();
      }
      stun_count = 0;
      for(i=0; i<18; i++)
      {
        battle_unit_state(i);
      }
    }
    satb_update();
    vsync();
  }
}

char get_highest_speed()
{
  char i, hi_spd, hi_attacks, highest;

  hi_spd = 0;
  hi_attacks = 0;
  highest = -1;

  for(i=0; i<18; i++)
  {
    if(battleunits[i].active && battleunits[i].attacks > hi_attacks)
    {
      hi_attacks = battleunits[i].attacks;
      hi_spd = battleunits[i].unit->unit->spd;
      highest = i;
    }
    else if(battleunits[i].active && battleunits[i].attacks == hi_attacks && hi_attacks != 0)
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
}

void b_u_idle(char b_id)
{
    if(battle_clock == b_id && battleunits[b_id].active
      && entities[battleunits[b_id].ent_id].bg->units[battleunits[b_id].pos]->unit.rng >= attack_range
      && animating == 0)
  {
      determine_action(b_id);
  }
}

void b_u_attack(char b_id)
{
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
        calc_hit_damage(b_id,i,0,0,0);
      }
    }
    spr_set(b_id);
    spr_pattern(idle_vrams[b_id]);
    battleunits[b_id].state = WAITING;
    battleunits[b_id].frame = 0;
    battleunits[b_id].attacks--;
    animating--;
  }
}

void b_u_stun(char b_id){
  if(battleunits[b_id].frame == 0)
  {
    spr_set(b_id);
    spr_pattern(idle_vrams[b_id]);
  }
  if(battleunits[b_id].frame == 3)
  {
    spr_set(b_id);
    spr_pattern(stun_vrams[stun_count++]);
    battleunits[b_id].frame++;
  }
  else if(battleunits[b_id].frame == 12)
  {
    spr_set(b_id);
    spr_pattern(idle_vrams[b_id]);
    battleunits[b_id].state = WAITING;
    battleunits[b_id].frame = 0;
    show_healthbar(battleunits[b_id].ent_id,battleunits[b_id].pos,spr_get_x(b_id),(spr_get_y(b_id)));

    if(entities[battleunits[b_id].ent_id].bg->units[battleunits[b_id].pos].hp <= 0)
    {
      kill_unit(b_id);

      spr_hide();
      hide_healthbar(b_id);
    }
    animating--;
  }
  else
  {
    battleunits[b_id].frame++;
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
    spr_set(b_id);
    spr_pattern(idle_vrams[b_id]);
    battleunits[b_id].state = WAITING;
    battleunits[b_id].frame = 0;
    battleunits[b_id].attacks--;
    animating--;
  }
}

void animate_attack(char unit_id, char hit_frames)
{
	spr_set(unit_id);
	spr_pattern(attack_vrams[0]+ATTACK_ANIMATIONS[hit_frames]);
}

void show_healthbar(char entity_id, char unit_id, int x, int y)
{
  int hp, hp_p;

  hp = entities[entity_id].bg->units[unit_id].hp * 100;
  hp_p = hp / entities[entity_id].bg->units[unit_id].unit->hp;

	display_healthbar(x/8,(y/8)+4,(char)hp_p);
//  put_number(entities[entity_id].bg->units[unit_id].hp,2,x/8,y/8+4);
//  put_char('/',x/8+2,y/8+4);
//  put_number(entities[entity_id].bg->units[unit_id].unit->hp,2,x/8+3,y/8+4);
}

void hide_healthbar(char unit_id)
{
	int x,y;
	spr_set(unit_id);
	y = spr_get_y();
	x = spr_get_x();
	put_string("     ",x/8,y/8+4);
}

void calc_heal(char attacker_id, char target_id, char a_a_bonus, char t_a_bonus, char t_d_bonus)
{
  if(get_percentage(battleunits[target_id].unit->hp,battleunits[target_id].unit->unit->hp)<100)
  {
    int heal_amt;
    heal_amt = calc_percentage(20,battleunits[target_id].unit->unit->hp);

    battleunits[target_id].unit->hp = min(battleunits[target_id].unit->hp + heal_amt,battleunits[target_id].unit->unit->hp);
  }
  battleunits[target_id].target = 0;
  spr_set(target_id);
  show_healthbar(battleunits[target_id].ent_id,battleunits[target_id].pos,spr_get_x(),spr_get_y());
}

void calc_hit_damage(char attacker_id, char target_id, char a_a_bonus, char t_a_bonus, char t_d_bonus)
{
	//TODO: cleanup pls
	int t_hp, t_atk, t_def,
			a_atk, fifth, dmg;

	t_hp  = entities[battleunits[target_id].ent_id].bg->units[battleunits[target_id].pos].hp;
	t_atk = entities[battleunits[target_id].ent_id].bg->units[battleunits[target_id].pos].unit->atk + t_a_bonus;
	t_def = entities[battleunits[target_id].ent_id].bg->units[battleunits[target_id].pos].unit->def + t_d_bonus;

	a_atk = entities[battleunits[attacker_id].ent_id].bg->units[battleunits[attacker_id].pos].unit->atk + a_a_bonus;

	dmg = max((a_atk - t_def),0);
//  put_string("atk:",0,0);
//  put_number(a_atk,3,4,0);
//  put_string("def:",0,1);
//  put_number(t_def,3,4,1);
//  put_string("dmg:",0,2);
//  put_number(dmg,3,4,2);
//  put_string("trgt:",0,3);
//  put_number(target_id,3,5,3);
//  put_string("attacker:",0,4);
//  put_number(attacker_id,3,9,4);
//  wait_for_I_input();
	if(dmg)
	{
    entities[battleunits[target_id].ent_id].bg->units[battleunits[target_id].pos].hp -= dmg;
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
char battle_loop(int i1, int i2, char range, char a_t, char t_t)
{
	attack_range = range;
	xOffset = -36;
	atker = i1, trgt = i2;
  speed_divider = 10;

  a_terrain = terrain_effect_by_type(terrain_type(a_t));
	t_terrain = terrain_effect_by_type(terrain_type(t_t));
	a_atk_bonus = terrain_atk_bonus(a_terrain);
	a_def_bonus = terrain_def_bonus(a_terrain);
	t_atk_bonus = terrain_atk_bonus(t_terrain);
	t_def_bonus = terrain_def_bonus(t_terrain);

	set_screen_size(SCR_SIZE_32x32);

//	load_font(font,125,0xf00);
//	set_font_addr(0xf00);
	cls();
	load_battle_bg();

	init_armies(i1,i2);
	scroll(0, 0, 0, 0, 223, 0xC0);

	load_healthbars();

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
        add_battle_unit(3-(i/3)-j,2+(i%3),player,total_count,1,9,(j*3)+i,&entities[player].bg->units[(j*3)+i]);

        show_healthbar(player,total_count,spr_get_x(),spr_get_y());
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
        add_battle_unit(j+4+(i/3),2+(i%3),cpu,total_count,1,0,(j*3)+i,&entities[cpu].bg->units[(j*3)+i]);
        show_healthbar(cpu,(j*3)+i,spr_get_x(),spr_get_y());
        team_two_count++;
      }
		}
	}

	for(j=0; j<9; j++)
	{
		spr_set(j+9);
		spr_ctrl(FLIP_MAS,FLIP_X);
	}
	satb_update();
}

void load_battle_bg()
{
	set_map_data(batty,16,16);
	set_tile_data(grass,0,grasspal,16);
	// cls();
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

        case MAGE_UNIT:
          load_palette(battleunits[i+off].pal,magebtlpal,1);
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
  char cmdr_id;

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

  cmdr_id = entities[battleunits[b_id].ent_id].id;
  if(entities[battleunits[b_id].ent_id].team == PLAYER)
  {
    give_commander_exp(cmdr_id,battleunits[b_id].unit->unit->id);
  }
}

void cleanup_battle(int player_selected_index, int cpu_selected_index)
{
  char i;

//  for(i=0; i<9; i++)
//  {
//    if(entities[player_selected_index].bg->units[i].hp)
//    {
//      entities[player_selected_index].bg->units[i].exp += team_one_kills;
//    }
//    if(entities[cpu_selected_index].bg->units[i].hp)
//    {
//      entities[cpu_selected_index].bg->units[i].exp += team_two_kills;
//    }
//  }
  level(player_selected_index);
  level(cpu_selected_index);
  for(i=0; i<18; i++)
  {
    battleunits[i].active = 0;
    battleunits[i].frame = 0;
    battleunits[i].pal = 0;
    battleunits[i].state = 0;
    battleunits[i].target_team = 0;
    battleunits[i].pos = 0;
    battleunits[i].type = 0;
    battleunits[i].attacks = 0;
    battleunits[i].target = 0;

  }

	total_units = 0;
  team_one_kills = 0;
  team_two_kills = 0;
  animating = 0;

	spr_set(62);
	spr_hide();
	spr_set(63);
	spr_hide();
	satb_update();

	reset_satb();
}

void level(char entity_id)
{
  char cmdr_id, level;

  if(entities[entity_id].team == PLAYER)
  {
    cmdr_id = entities[entity_id].id;
    level = party_commanders[cmdr_id].level;
    if(level < level_up_commander(cmdr_id))
    {
      put_string("level up",1,1);
      wait_for_I_input();
      put_string("new level:",1,1);
      put_number(party_commanders[cmdr_id].level,2,10,1);
      wait_for_I_input();
    }
  }
}
