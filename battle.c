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
int army_one_size;
int army_two_size;
int attack_turns;
int attacks;
int team_one_total;
int team_two_total;
char can_attack;
int one_dmg, two_dmg, one_health, two_health;
char ticker = 0;

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

void controls()
{
}

int is_opp_adjacent(int gridpos)
{
	return 0;
}

void intro()
{
	int hit_frames;

	attacking_team = TEAM_ONE;
	target_team = TEAM_TWO;
	attacker = TEAM_ONE;
	attack_turns = (team_one_count + team_two_count);
	attacks = 0;

	attacker_size = team_one_count;
	target_size = team_two_count;
	// target = range(target_team,target_team+(target_size-1));
	find_new_target();
	disp_on();
	load_pals(0,team_one_count);
	load_pals(9,team_two_count);

	satb_update();
	load_animations_to_vram(TEAM_ONE,TEAM_TWO);
	hit_frames = 0;
	t = 0;

	while((attack_turns>attacks) && team_one_count && team_two_count)
	{
		if(t++ == 4)
		{
			display_unit_info(attacker,0,0);
			display_unit_info(target,18,0);
			if(attacking_team == TEAM_ONE && attacker == team_one_count)
			{
				target_size = team_one_count;
				attacker_size = team_two_count;
				swap_turns(TEAM_TWO,TEAM_ONE);
				find_new_target();
			}
			else if(attacking_team == TEAM_TWO && attacker == ((team_two_count)+9))
			{
				target_size = team_two_count;
				attacker_size = team_one_count;
				swap_turns(TEAM_ONE,TEAM_TWO);
				find_new_target();
			}
			t = 0;
			if(npcs[attacker].active != 1)
			{
				attacker++;
				attacks++;
			}
			else
			{
				animate_attack(attacker,hit_frames);
				animate_stun(target,hit_frames++);
			}
		}
		if(hit_frames == 12)
		{
			hide_healthbar(target);
			spr_set(attacker);
			spr_pattern(idle_vrams[attacker]);
			spr_set(target);
			spr_pattern(idle_vrams[target]);
			hit_frames = 0;
			attacker++;
			find_new_target();
			attacks++;
			// wait_for_I_input();
		}
		satb_update();
		vsync(1);
	}
}

void display_unit_info(char id, char x, char y)
{
	print_unit_type(unit_entities[npcs[id].type].unit_type,x,y);
	put_string("HP",x+4,y);
	put_number(unit_entities[npcs[id].type].hp,4,x+2,y+1);
	put_string("AT",x+7,y);
	put_number(unit_list[unit_entities[npcs[id].type].unit_type].atk,3,x+6,y+1);
	put_string("DF",x+10,y);
	put_number(unit_list[unit_entities[npcs[id].type].unit_type].def,3,x+9,y+1);
}

void animate_attack(char unit_id, char hit_frames)
{
	spr_set(unit_id);
	spr_pattern(attack_vrams[unit_id%9]+ATTACK_ANIMATIONS[hit_frames]);
}

void animate_stun(char unit_id, char hit_frames)
{
	int x,y;
	char hlth, h_diff, dmg;
	hlth = 5;

	spr_set(unit_id);
	y = spr_get_y();
	x = spr_get_x();

	if(hit_frames == 2)
	{
		spr_set(unit_id);
		spr_pattern(stun_vrams[unit_id%9]);
		display_healthbar(x/8,(y/8)+4);
	}
	if(hit_frames == 8)
	{
		dmg = calc_hit_damage();
		reduce_healthbar(x/8,(y/8)+4,hlth-dmg,dmg);
	}
}

void hide_healthbar(char unit_id)
{
	int x,y;
	spr_set(unit_id);
	y = spr_get_y();
	x = spr_get_x();
	put_string("     ",x/8,y/8+4);
}

void kill_unit()
{
	npcs[target].active = 0;
	spr_hide(target);
}

void calc_hit_damage()
{
	//TODO: cleanup pls
	int t_hp, t_atk, t_def,
			a_atk, fifth, dmg;

	t_hp = unit_entities[npcs[target].type].hp;
	t_atk = unit_list[unit_entities[npcs[target].type].unit_type].atk;
	t_def = unit_list[unit_entities[npcs[target].type].unit_type].def;

	a_atk = unit_list[unit_entities[npcs[attacker].type].unit_type].atk;

	fifth = t_hp / 5;
	dmg = max((a_atk - t_def)*10,0);
	if(dmg)
	{
		if(dmg > t_hp)
		{
			unit_entities[npcs[target].type].hp = 0;
			kill_unit();
			return 5;
		}
		if(dmg < fifth){ unit_entities[npcs[target].type].hp -= dmg;return 0;}
		if((t_hp - dmg)<fifth){ unit_entities[npcs[target].type].hp -= dmg;return 4;}
		unit_entities[npcs[target].type].hp -= dmg;
		return 5 - ((t_hp - ((a_atk - t_def)*10))/fifth);
	}
	return 0;
}

void swap_turns(char atkr, char trgt)
{
	load_animations_to_vram(atkr,trgt);
	attacking_team = atkr;
	target_team = trgt;
	attacker = atkr;
	// target = range(target_team,target_team+(target_size-1));
}

void find_new_target()
{
	char t;
	while(npcs[(t = get_random_target())].active != 1){}
	target = t;
}

void get_next_attacker()
{
	char j;
	j=0;
	// put_number(attacking_team,3,0,20);
	// put_number(attacker_size,3,0,21);
	// put_number((attacking_team+attacker_size)+1,3,0,22);

	for(j=attacker; j<(attacking_team+attacker_size)+1; j++)
	{
		if(npcs[j].active)
		{
			attacker = j;
			return;
			// return;
		}
	}
	// attacker = j;
	// return attacker_size;
}

char get_random_target()
{
	return range(target_team,target_team+(target_size-1));
}

void load_animations_to_vram(char attackers, char targets)
{
	char i;
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

void reset_camera()
{
	char z;
	sx = 0;
	sy = 0;
	// yOffset = 0;
	xOffset = 0;

	scroll(0, 0, 0, 0, 223, 0xC0);
}
/*
	BATTLE ROUTINE
*/

char battle_loop(int i1, int i2, char target_attack)
{
	army_one_size = 0;
	army_one_size = 0;
	total_sprites = 0;
	can_attack = target_attack;
	xOffset = -36;
	atker = i1, trgt = i2;

	init_armies(i1,i2);

	scroll(0, 0, 0, 0, 223, 0xC0);
	set_screen_size(SCR_SIZE_32x32);
	load_battle_bg();
	// load_battle_scene(i1,i2);

	one_dmg = 0;
	two_dmg = 0;
	one_health = 60;
	two_health = 120;
	load_healthbars();
	// display_healthbar(16,12);
	// display_healthbar(16,17);
	// display_healthbar(16,22);
	// display_healthbar(21,12);
	// reduce_healthbar(13,2,7,0);
	// reduce_healthbar(13,4,0,7);

	intro();

	reset_satb();
	cleanup_battle(i1,i2);

	// if(p_c_army.current_army_size == 0)//attacker
	// {
	// 	return 0;
	// }
	// else if(c_c_army.current_army_size == 0)//target
	// {
	// 	return 1;
	// }
	// else
	// {
	// 	return 2;
	// }

	return 2;
}

void init_armies(int player, int cpu)
{

	int j;
	int i;
	char total_count;
	char *row;

	reset_npcs();

	team_one_total = 0;
	team_two_total = 0;
	team_one_count = 0;
	team_two_count = 0;
	total_units = 0;
	total_count = 0;

	team_one_count += commanders[player].row_counts[0];
	team_one_count += commanders[player].row_counts[1];
	team_one_count += commanders[player].row_counts[2];

	team_two_count += commanders[cpu].row_counts[0];
	team_two_count += commanders[cpu].row_counts[1];
	team_two_count += commanders[cpu].row_counts[2];

	total_units = team_two_count + team_one_count;
	team_one_total = team_one_count;
	team_two_total = team_two_count;

	row = commanders[player].row_one;
	for(j=0; j<3; j++, row+=3)
	{
		for(i=0; i<commanders[player].row_counts[j]; i++, total_count++)
		{
			add_battle_npc(3-(i/3)-j,2+(i%3),row[i],17,total_count);
		}
	}

	row = commanders[cpu].row_one;
	total_count = 9;
	for(j=0; j<3; j++, row+=3)
	{
		for(i=0; i<commanders[cpu].row_counts[j]; i++, total_count++)
		{
			add_battle_npc(j+4+(i/3),2+(i%3),row[i],17,total_count);
		}
	}

	for(j=0; j<team_two_count; j++)
	{
		spr_set(j+9);
		spr_ctrl(FLIP_MAS,FLIP_X);
	}
}

void load_battle_bg()
{
	set_map_data(batty,16,16);
	set_tile_data(grass,0,grasspal,16);
}

int get_y_from_pos(int pos){
	return ((pos) / TILES_PER_LINE);
}

int get_x_from_pos(int pos){
	return (pos % TILES_PER_LINE);
}

void load_pals(int off, char count)
{
	char i;
	for(i=0; i<count; i++)
	{
		switch(unit_entities[npcs[i+off].type].unit_type)
		{
			case BLOB_UNIT:
			case SWORD_UNIT:
			load_palette(npcs[i+off].pal,soldierpal,1);
			break;

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
		}
	}
}

void cleanup_battle(int player_selected_index, int cpu_selected_index)
{
	int i, j;

	for(i=0;i<2;i++)
	{
		for(j=0;j<9;j++)
		{

		}
	}
	// entities[player_selected_index].army_size = p_c_army.current_army_size;
	// entities[cpu_selected_index].army_size = c_c_army.current_army_size;
	// entities[player_selected_index].hp = p_c_army.hp_current;
	// entities[cpu_selected_index].hp = c_c_army.hp_current;
	// for(i=5; i<35; i++)
	// {
	// 	spr_hide(i);
	// }

	// for(i=0, sp=armyOne; i<15; i++)
	// {
	// 	sp->active = 0;
	// 	sp->id = 0;
	// 	sp->state = 0;
	// 	sp->tic = 0;
	// 	sp->x = 0;
	// 	sp->y = 0;
	// 	sp->state = 0;
	// 	sp->direction = 0;
	// }
	//
	// for(i=0, sp=armyTwo; i<15; i++)
	// {
	// 	sp->active = 0;
	// 	sp->id = 0;
	// 	sp->state = 0;
	// 	sp->tic = 0;
	// 	sp->x = 0;
	// 	sp->y = 0;
	// 	sp->state = 0;
	// 	sp->direction = 0;
	// }

	// sp = 0;
	total_units = 0;

	spr_set(62);
	spr_hide();
	spr_set(63);
	spr_hide();
	satb_update();

	total_sprites = 0;
	reset_satb();
	// cls();
}
