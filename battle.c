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
// #include "battle_menu.c"
// char grid[NUMBER_OF_TILES];
int attack_turns;
int attacks;
int team_one_total;
int team_two_total;
int team_one_hp;
int team_two_hp;
int team_two_max_hp;
int team_one_max_hp;
char attack_range;
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

void intro()
{
	int hit_frames, i;

	attacking_team = TEAM_ONE;
	target_team = TEAM_TWO;
	attacker = TEAM_ONE;
	attack_turns = (team_one_count + team_two_count);
	attacks = 0;

	attacker_size = team_one_count;
	target_size = team_two_count;

	load_pals(atker,0,team_one_count);
	load_pals(trgt,9,team_two_count);

	find_new_target();
	disp_on();

	load_palette(0,healthbarpal,1);
	satb_update();
	load_animations_to_vram(TEAM_ONE,TEAM_TWO);

	hit_frames = 0;
	t = 0;
	while((attack_turns>attacks) && team_one_count && team_two_count)
	{
		if(t++ == 4)
		{
			if(attacking_team == TEAM_ONE && attacker == 9)
			{
        target_size = team_one_count;
				attacker_size = team_two_count;
				swap_turns(TEAM_TWO,TEAM_ONE);
			}
			else if(attacking_team == TEAM_TWO && attacker == ((team_two_count)+9))
			{
				target_size = team_two_count;
				attacker_size = team_one_count;
				swap_turns(TEAM_ONE,TEAM_TWO);
			}
			t = 0;
			if((attacking_team == TEAM_ONE && entities[atker].bg->units[attacker].unit->rng < attack_range) ||
				(attacking_team == TEAM_TWO && entities[trgt].bg->units[attacker-9].unit->rng < attack_range) ||
				(!npcs[attacker].active))
			{
        attacker++;
				attacks++;

        find_new_target();
			}
			else
			{
				animate_attack(attacker,hit_frames);
				animate_stun(target,hit_frames++);
			}
		}
		if(hit_frames == 12)
		{
			spr_set(attacker);
			spr_pattern(idle_vrams[attacker]);
			spr_set(target);
			spr_pattern(idle_vrams[target]);
			hit_frames = 0;
			attacker++;

			if(team_one_hp <= 0 || team_two_hp <= 0)
			{
				break;
			}
			attacks++;
		}
		satb_update();
		vsync(1);
	}
}

void battle_seq()
{
  int b_ticker = 0;
  clock_divider = 10;
  attacking = 0;

  //two options:
  //attacking -> continue animation, battle stuff
  //not attacking -> two options
  //unit can attack: find next unit to attack
  //no one can attack: decrement battle clock

  put_number(battle_clock,3,15,0);
  while(battle_clock)
  {
    if(b_ticker++ == 4)
    {

      if(attacking)
      {

      }
      else
      {
        if((attacker = find_next_attacker()))
        {

        }
      }

      put_number(--battle_clock,3,15,0);
    }
  }
}

char check_next_attacker()
{

}

void display_unit_info(char entity_id, char id, char x, char y)
{
//	put_number(team_one_hp,3,10,2);
//	put_number(team_two_hp,3,16,2);
	// print_unit_type(entities[entity_id].row_one[id].unit->id,x,y);
	// put_number(npcs[id].type,3,x,y+3);
	// put_string("HP",x+4,y);
	// put_number(entities[entity_id].row_one[id].unit->hp,3,x+2,y+1);
	// put_string("AT",x+7,y);
	// put_number(entities[entity_id].row_one[id].unit->atk,2,x+6,y+1);
	// put_string("DF",x+10,y);
	// put_number(entities[entity_id].row_one[id].unit->def,2,x+9,y+1);
}

void animate_attack(char unit_id, char hit_frames)
{
	spr_set(unit_id);
	spr_pattern(attack_vrams[unit_id%9]+ATTACK_ANIMATIONS[hit_frames]);
}

void animate_stun(char unit_id, char hit_frames)
{
	int x,y;
	char hlth, h_diff, dmg, attacker_id, 
		 target_id, t_d_bonus, t_a_bonus, a_a_bonus, a_d_bonus;
	hlth = 5;

	if(attacking_team == TEAM_ONE)
	{
		attacker_id = atker;
		target_id = trgt;
		a_a_bonus = a_atk_bonus;
		a_d_bonus = a_def_bonus;
		t_a_bonus = t_atk_bonus;
		t_d_bonus = t_def_bonus;
	}
	else
	{
		attacker_id = trgt;
		target_id = atker;
		a_a_bonus = t_atk_bonus;
		a_d_bonus = t_def_bonus;
		t_a_bonus = a_atk_bonus;
		t_d_bonus = a_def_bonus;
	}

	spr_set(unit_id);
	y = spr_get_y();
	x = spr_get_x();

	if(hit_frames == 2)
	{
		spr_set(unit_id);
		spr_pattern(stun_vrams[unit_id%9]);
	}
	if(hit_frames == 8)
	{
		dmg = calc_hit_damage(attacker_id,target_id,a_a_bonus,t_a_bonus,t_d_bonus);
		 reduce_healthbar(x/8,(y/8)+4,hlth-dmg,dmg);
	}
}

void show_healthbar(char entity_id, char unit_id, int x, int y)
{
	unsigned char max_hlth, h_diff, h_p, dmg;

	max_hlth = entities[entity_id].bg->units[unit_id].unit->hp;
	h_p = max_hlth / 5;
	h_diff = roundUp(entities[entity_id].bg->units[unit_id].hp,h_p);
//  put_string("unit id",0,5); put_number(unit_id,3,10,5);
//  put_string("hp:",0,6); put_number(h_p,3,4,6);
//  wait_for_I_input();
	display_healthbar(x/8,(y/8)+4);
	reduce_healthbar(x/8,(y/8)+4,h_diff,5-h_diff);
}

void hide_healthbar(char unit_id)
{
	int x,y;
	spr_set(unit_id);
	y = spr_get_y();
	x = spr_get_x();
	put_string("     ",x/8,y/8+4);
}

void kill_unit(char target_id)
{
	 if(attacking_team == TEAM_ONE)
	 {
     team_one_kills++;
	// 	team_two_total--;
	 }
	 else
	 {
     team_two_kills++;
	// 	team_one_total--;
	 }

	entities[target_id].bg->units[(target-target_team)].hp = 0;
	npcs[target].active = 0;
	spr_hide(target);
}

void calc_hit_damage(char attacker_id, char target_id, char a_a_bonus, char t_a_bonus, char t_d_bonus)
{
	//TODO: cleanup pls
	int t_hp, t_atk, t_def,
			a_atk, fifth, dmg;

	t_hp  = entities[target_id].bg->units[target-target_team].hp;
	t_atk = entities[target_id].bg->units[target-target_team].unit->atk + t_a_bonus;
	t_def = entities[target_id].bg->units[target-target_team].unit->def + t_d_bonus;

	a_atk = entities[attacker_id].bg->units[attacker-attacking_team].unit->atk + a_a_bonus; 

	fifth = t_hp / 5;
	dmg = max((a_atk - t_def),0);

	if(attacking_team == TEAM_ONE)
	{
		team_two_hp -= dmg;
	}
	else
	{
		team_one_hp -= dmg;
	}
	if(dmg)
	{
		if(dmg >= entities[target_id].bg->units[target-target_team].hp)
		{	
			kill_unit(target_id);
			return 5;
		}
		if(dmg < fifth)
		{
			entities[target_id].bg->units[target-target_team].hp -= dmg;
			return 0;
		}

		if((t_hp - dmg) < fifth)
		{
			entities[target_id].bg->units[target-target_team].hp -= dmg;
			return 4;
		}

		entities[target_id].bg->units[target-target_team].hp -= dmg;
		return min(dmg/fifth,5);
	}
	return 0;
}

void swap_turns(char atkr, char trgt)
{
	load_animations_to_vram(atkr,trgt);
	attacking_team = atkr;
	target_team = trgt;
	attacker = atkr;
//  find_new_target();
}

void find_new_target()
{
  target = find_target_unit(attacker-attacking_team) + target_team;
}

char find_next_attacker()
{
  char i;
  for(i=0; i<18; i++)
  {
//    if(npcs[])
//    {
//
//    }
  }

  return 0;
}

char get_random_target()
{
	char i;
	for(i=0; i<9; i++)
	{
		if(npcs[i+target_team].active)
		{
			return i+target_team;
		}
	}
	return 0;
}

void load_animations_to_vram(char attackers, char targets)
{
	char i;
	// put_hex(stun_vrams,5,1,18);
	for(i=0; i<attacker_size; i++)
	{
		transfer_units_to_attack_vram(i+attackers);
		// transfer_units_to_stun_vram(i+targets);
	}

	for(i=0; i<target_size; i++)
	{
		transfer_units_to_stun_vram(i+targets);
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

	a_terrain = terrain_effect_by_type(terrain_type(a_t));
	t_terrain = terrain_effect_by_type(terrain_type(t_t));
	a_atk_bonus = terrain_atk_bonus(a_terrain);
	a_def_bonus = terrain_def_bonus(a_terrain);
	t_atk_bonus = terrain_atk_bonus(t_terrain);
	t_def_bonus = terrain_def_bonus(t_terrain);

	set_screen_size(SCR_SIZE_32x32);

	load_font(font,125,0xf00);
	set_font_addr(0xf00);
	cls();
	load_battle_bg();

	init_armies(i1,i2);
	scroll(0, 0, 0, 0, 223, 0xC0);

	load_healthbars();
//	intro();
  battle_seq();
	reset_satb();
	cleanup_battle(i1,i2);

	if(team_one_hp <= 0)
	{
		return 1;
	}

	if(team_two_hp <= 0)
	{
		return 0;
	}
	return 2;
}

void init_armies(int player, int cpu)
{
	int j,i;
	char total_count;

	reset_npcs();
	team_one_total = 0;
	team_two_total = 0;
	team_one_count = 0;
	team_two_count = 0;
	total_units = 0;
	total_count = 0;
	team_one_hp = 0;
	team_one_max_hp = 0;
	team_two_hp = 0;
	team_two_max_hp = 0;
  battle_clock = 0;

	team_one_count = 9;
	team_two_count = 9;

	total_units	   = team_two_count + team_one_count;
	team_one_total = team_one_count;
	team_two_total = team_two_count;

	for(j=0; j<3; j++)
	{
		for(i=0; i<3; i++, total_count++)
		{
			add_battle_npc(3-(i/3)-j,2+(i%3),entities[player].bg->units[(j*3)+i]->unit.id,17,total_count,entities[player].bg->units[(j*3)+i].hp>0);
			team_one_hp += entities[player].bg->units[(j*3)+i].hp;
			team_one_max_hp += entities[player].bg->units[(j*3)+i]->unit.hp;

      if(entities[player].bg->units[(j*3)+i]->unit.id)
      {
        if(entities[player].bg->units[(j*3)+i]->unit.spd > battle_clock)
        {
          battle_clock = entities[player].bg->units[(j*3)+i]->unit.spd;
        }
        show_healthbar(player,total_count,spr_get_x(),spr_get_y());
      }
    }
	}

	total_count = 9;
	for(j=0; j<3; j++)
	{
		for(i=0; i<3; i++, total_count++)
		{
			add_battle_npc(j+4+(i/3),2+(i%3),entities[cpu].bg->units[(j*3)+i]->unit.id,17,total_count,entities[cpu].bg->units[(j*3)+i].hp>0);
			team_two_hp += entities[cpu].bg->units[(j*3)+i].hp;
			team_two_max_hp += entities[cpu].bg->units[(j*3)+i]->unit.hp;
      if(entities[cpu].bg->units[(j*3)+i]->unit.id)
      {
        if(entities[cpu].bg->units[(j*3)+i]->unit.spd > battle_clock)
        {
          battle_clock = entities[cpu].bg->units[(j*3)+i]->unit.spd;
        }
        show_healthbar(cpu,total_count-9,spr_get_x(),spr_get_y());
      }
		}
	}

	for(j=0; j<team_two_count; j++)
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

void load_pals(char entity_id, int off, char count)
{
	char i;
	for(i=0; i<count; i++)
	{
		if(entities[entity_id].bg->units[i].hp)
		{
			switch(entities[entity_id].bg->units[i].unit->id)
			{
				case BLOB_UNIT:
          load_palette(npcs[i+off].pal,blobbattlepal,1);
          break;

				case SWORD_UNIT:
          load_palette(npcs[i+off].pal,soldierpal,1);
          break;

        case LANCER_UNIT:
        case KNIGHT_UNIT:
				case SPEAR_UNIT:
          load_palette(npcs[i+off].pal,spearpal,1);
          break;

				case ARCHER_UNIT:
          load_palette(npcs[i+off].pal,musketbtlpal,1);
          break;

				case DEMON_UNIT:
          load_palette(npcs[i+off].pal,demonbtlpal,1);
          break;

				case HOUND_UNIT:
          load_palette(npcs[i+off].pal,houndbtlpal,1);
          break;

				case AXE_UNIT:
          load_palette(npcs[i+off].pal,banditpal,1);
          break;

        case MAGE_UNIT:
          load_palette(npcs[i+off].pal,magebtlpal,1);
          break;

        case MONK_UNIT:
          load_palette(npcs[i+off].pal,monkbtlpal,1);
          break;
			}
		}
	}
}

void cleanup_battle(int player_selected_index, int cpu_selected_index)
{
  char i;

  for(i=0; i<9; i++)
  {
    if(entities[player_selected_index].bg->units[i].hp)
    {
      entities[player_selected_index].bg->units[i].exp += team_one_kills;
    }
    if(entities[cpu_selected_index].bg->units[i].hp)
    {
      entities[cpu_selected_index].bg->units[i].exp += team_two_kills;
    }
  }
	total_units = 0;
  team_one_kills = 0;
  team_two_kills = 0;

	spr_set(62);
	spr_hide();
	spr_set(63);
	spr_hide();
	satb_update();

	reset_satb();
	// cls();
}
