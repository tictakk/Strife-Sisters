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
	load_palette(0,healthbarpal,1);

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
			if(!npcs[target].active)
			{
				hide_healthbar(target);
			}
			spr_set(attacker);
			spr_pattern(idle_vrams[attacker]);
			spr_set(target);
			spr_pattern(idle_vrams[target]);
			hit_frames = 0;
			attacker++;
			// wait_for_I_input();
			if(team_one_total == 0 || team_two_total == 0)
			{
				// return;
				break;
			}
			find_new_target();
			attacks++;
		}
		satb_update();
		vsync(1);
	}
}

void display_unit_info(char id, char x, char y)
{
	print_unit_type(unit_entities[npcs[id].type].unit_type,x,y);
	put_number(npcs[id].type,3,x,y+3);
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
	}
	if(hit_frames == 8)
	{
		dmg = calc_hit_damage();
		reduce_healthbar(x/8,(y/8)+4,hlth-dmg,dmg);
	}
}

void show_healthbar(char npc_id, int x, int y)
{
	char hlth, h_diff, dmg, p;
	hlth = unit_list[unit_entities[npcs[npc_id].type].unit_type].hp;
	p = hlth / 5;
	h_diff = roundUp(unit_entities[npcs[npc_id].type].hp,p);

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

void kill_unit()
{
	if(attacking_team == TEAM_ONE)
	{
		team_two_total--;
	}
	else
	{
		team_one_total--;
	}
	npcs[target].active = 0;
	spr_hide(target);
	// hide_healthbar(target);
	delete_unit_entity(npcs[target].type);
	reduce_unit_ids(npcs[target].type);
	reduce_npc_ids(npcs[target].type);
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
		if(dmg >= t_hp)
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
	t = 0;
	// if(target_size)
	// {
	// 	while((npcs[(t = get_random_target())].active != 1)){}
	// }
	while((npcs[(t = get_random_target())].active != 1)){}
	target = t;
}

void get_next_attacker()
{
	char j;
	j=0;

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
	// wait_for_I_input();

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
	set_screen_size(SCR_SIZE_32x32);
	// load_palette(0,grasspal,1);
	load_font(font,125,0xf00);
	set_font_addr(0xf00);
	cls();

	load_battle_bg();

	init_armies(i1,i2);
	// cls(1);
	scroll(0, 0, 0, 0, 223, 0xC0);
	// load_font(font,125,0xf00);
	// set_screen_size(SCR_SIZE_32x32);
	// load_battle_scene(i1,i2);

	load_healthbars();
	intro();

	reset_satb();
	cleanup_battle(i1,i2);

	if(team_one_total == 0)
	{
		return 1;
	}

	if(team_two_total == 0)
	{
		return 0;
	}
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
			show_healthbar(total_count,spr_get_x(),spr_get_y());
		}
	}

	row = commanders[cpu].row_one;
	total_count = 9;
	for(j=0; j<3; j++, row+=3)
	{
		for(i=0; i<commanders[cpu].row_counts[j]; i++, total_count++)
		{
			add_battle_npc(j+4+(i/3),2+(i%3),row[i],17,total_count);
			show_healthbar(total_count,spr_get_x(),spr_get_y());
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
	unsigned char last_id;
	char i;

	// for(i=0; i<team_one_count; i++)
	// {
	// 	if(npcs[i].active == 0)
	// 	{
	// 		delete_unit_entity(npcs[i].type);
	// 		reduce_unit_ids(npcs[i].type);
	// 		reduce_npc_ids(npcs[i].type);
	// 		wait_for_I_input();
	// 	}
	// }
	//
	// for(i=9; i<team_two_count+9; i++)
	// {
	// 	if(npcs[i].active == 0)
	// 	{
	// 		delete_unit_entity(npcs[i].type);
	// 		reduce_unit_ids(npcs[i].type);
	// 		reduce_npc_ids(npcs[i].type);
	// 		wait_for_I_input();
	// 	}
	// }

	total_units = 0;
	team_one_total = 0;
	team_two_total = 0;

	spr_set(62);
	spr_hide();
	spr_set(63);
	spr_hide();
	satb_update();

	total_sprites = 0;
	reset_satb();
	// cls();
}
