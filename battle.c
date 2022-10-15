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
char can_attack;
int one_dmg, two_dmg, one_health, two_health;
char ticker = 0;

char atker, trgt;
#define COMMANDER_ONE_VRAM 0x2200
#define COMMANDER_TWO_VRAM 0x3200
#define PLAYER_SPRITE_VRAM 0x6000
#define CPU_SPRITE_VRAM 0x5000

const char batty[1024];

// void calc_stats(Army *army_one, Army *army_two)
// {
// 	int one_dmg, two_dmg, one_hp, two_hp, one_avg, two_avg, r, damaged, num, diff;
// 	if((army_one->hp_current == 0 || army_two->hp_current == 0))
// 	{
// 		return;
// 	}
//
// 	one_dmg = rand() % army_one->atk_total;
// 	two_dmg = rand() % army_two->atk_total;
//
// 	if(!can_attack)
// 	{
// 		two_dmg = 0;
// 	}
//
// 	one_dmg = one_dmg - ((one_dmg * army_two->def_total)/100);
// 	two_dmg = two_dmg - ((two_dmg * army_one->def_total)/100);
//
// 	army_one->hp_current -= (two_dmg * 5);
// 	army_two->hp_current -= (one_dmg * 5);
//
// 	if(army_one->hp_current <= 0)
// 	{
// 		army_one->to_kill = army_one->current_army_size;
// 		army_one->current_army_size = 0;
// 		army_one->hp_current = 0;
// 		army_one->healthbars = 0;
// 		reduce_healthbar(4,2,0,7,0);
// 		return;
// 	}
//
// 	if(army_two->hp_current <= 0)
// 	{
// 		army_two->to_kill = army_two->current_army_size;
// 		army_two->current_army_size = 0;
// 		army_two->hp_current = 0;
// 		army_two->healthbars = 0;
// 		reduce_healthbar(20,2,0,7,0);
// 		return;
// 	}
// 	one_hp = (army_one->hp_total / army_one->army_start_size);
// 	two_hp = (army_two->hp_total / army_two->army_start_size);
// 	one_avg = roundUp(army_one->hp_current,one_hp);
// 	two_avg = roundUp(army_two->hp_current,two_hp);
//
// 	// if(army_one->hp_current % one_hp)
// 		army_one->to_kill = army_one->current_army_size - one_avg;
// 		army_one->current_army_size = one_avg;
// 	// if(army_two->hp_current % two_hp)
// 		army_two->to_kill = army_two->current_army_size - two_avg;
// 		army_two->current_army_size = two_avg;
//
// 	num = roundUp(7*army_one->current_army_size,army_one->army_start_size);
// 	if(num < army_one->healthbars)
// 	{
// 		diff = army_one->healthbars - num;
// 		army_one->healthbars = num;
// 		// reduce_healthbar(army_one->x,army_one->y,army_one->healthbars,diff,0);
// 		reduce_healthbar(4,2,army_one->healthbars,diff,0);
// 	}
//
// 	num = roundUp(7*army_two->current_army_size,army_two->army_start_size);
// 	if(num < army_two->healthbars)
// 	{
// 		diff = army_two->healthbars - num;
// 		army_two->healthbars = num;
// 		// reduce_healthbar(army_two->x,army_two->y,army_two->healthbars,diff,0);
// 		reduce_healthbar(20,2,army_two->healthbars,diff,0);
// 	}
// }

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
	int hit_frames, attack_turns;

	attacking_team = TEAM_ONE;
	target_team = TEAM_TWO;
	attacker = TEAM_ONE;
	attack_turns = (team_one_count + team_two_count) * 2;

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

	while(attack_turns)
	{
		if(t++ == 4)
		{
			if(attacking_team == TEAM_ONE && attacker == team_one_count)
			{
				// put_number(attacker,2,5,1);
				target_size = team_one_count;
				attacker_size = team_two_count;
				swap_turns(TEAM_TWO,TEAM_ONE);
				// target = range(target_team,target_team+(target_size-1));
				find_new_target();
			}
			else if(attacking_team == TEAM_TWO && attacker == ((team_two_count)+9))
			{
				// put_number(attacker,2,5,1);
				target_size = team_two_count;
				attacker_size = team_one_count;
				swap_turns(TEAM_ONE,TEAM_TWO);
				find_new_target();
			}

			put_number(atker,3,20,0);
			put_number(trgt,3,20,1);
			put_number(attacker,3,0,0);
			put_number(target,3,10,0);
			t = 0;
			animate_attack(attacker,hit_frames);
			animate_stun(target,hit_frames++);
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
			// target = range(target_team,target_team+(target_size-1));
			find_new_target();
			attack_turns--;
		}

		satb_update();
		vsync(1);
	}
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

void calc_hit_damage()
{
	int t_hp, t_atk, t_def,
			a_atk, fifth, dmg;

	t_hp = unit_list[npcs[target].type].hp;
	t_atk = unit_list[npcs[target].type].atk;
	t_def = unit_list[npcs[target].type].def;

	a_atk = unit_list[npcs[attacker].type].atk;

	fifth = t_hp / 5;
	dmg = max((a_atk - t_def)*30,0);
	if(dmg)
	{
		if(dmg > t_hp){ npcs[target].active = 0; return 5;}
		if(dmg < fifth){return 0;}
		if((t_hp - dmg)<fifth){return 4;}
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
	while(npcs[(t = get_random_target())].active)
	{
		target = t;
		// print_unit_type(npcs[target].type,0,2);
		// put_number(unit_list[npcs[attacker].type].hp,3,0,2);
		// put_number(unit_list[npcs[target].type].hp,3,0,3);
		return;
	}
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

// void update_army_frame(struct soldier *army, char army_size, int frame)
// {
// 	struct soldier *soldier;
// 	char i;
// 	for(i = 0, soldier = army; i<army_size; i++, soldier++)
// 	{
// 		if(soldier->active)
// 		{
// 			soldier->frame = frame;
// 		}
// 	}
// }

// void change_army_frame(struct soldier *army, int army_size, int frame, int direction)
// {
// 	struct soldier *soldier;
// 	char i;
//
// 	for(i = 0, soldier = army; i<army_size; i++, soldier++)
// 	{
// 		if(soldier->active)
// 		{
// 			spr_set(soldier->id);
// 			if(animation_ended(soldier))
// 			{
// 				if(soldier->state == DYING)
// 				{
// 					soldier->active = 0;
// 					spr_y(-32);
// 					spr_hide();
// 				}
// 				else
// 				{
// 					soldier->frame = frame;
// 					soldier->tic = 0;
// 					soldier->state = IDLE;
// 					spr_pattern(soldier->frame);
// 				}
// 			}
// 			else
// 			{
// 				// soldier->tic = (soldier->tic + 1 ) % 4;
// 				if(soldier->state == DYING)
// 				{
// 					spr_y(spr_get_y()-1);
// 					spr_x(spr_get_x()+direction);
// 					spr_pattern((soldier->frame) + ((soldier->tic/2) * 0x100));
// 					soldier->tic++;
// 				}
// 				else
// 				{
// 					spr_pattern((soldier->frame) + (soldier->tic * 0x100));
// 					soldier->tic++;
// 				}
// 			}
// 		}
// 	}
// }

// void animation_ended(struct soldier *soldier)
// {
// 	switch(soldier->state)
// 	{
// 		case IDLE:
// 		if(soldier->tic == 3)
// 		{
// 			return 1;
// 		}
// 			break;
//
// 		case ATTACKING:
// 		if(soldier->tic == 5)
// 		{
// 			return 1;
// 		}
// 			break;
//
// 		case DYING:
// 		if(soldier->tic == 4*2) //return 1;
// 		{
// 			// soldier->active = 0;
// 			return 1;
// 		}
// 			break;
// 	}
// 	return 0;
// }

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
	// cleanup_battle(i1,i2);

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

// void load_battle_scene(int i1, int i2)
// {
// 	disp_off();
// 	reset_satb();
// 	load_battle_bg();
//
// 	if(entities[i1].team == PLAYER)
// 	{
// 		load_unit_type_to_vram(PLAYER_SPRITE_VRAM,entities[i1]->unit.unit_type,16,0);
// 		load_unit_type_to_vram(CPU_SPRITE_VRAM,entities[i2]->unit.unit_type,17,16);
// 	}
// 	else
// 	{
// 		load_unit_type_to_vram(PLAYER_SPRITE_VRAM,entities[i1]->unit.unit_type,16,16);
// 		load_unit_type_to_vram(CPU_SPRITE_VRAM,entities[i2]->unit.unit_type,17,0);
// 	}
//
// 	set_font_color(0, 12);
// 	set_font_pal(0);
// 	load_default_font(0,0x4900);
//
// 	init_armies(i1,i2);
// 	disp_on();
// }

// void load_unit_type_to_vram(int vram_location, enum Unit_Type type, char pal,char offset)
// {
// 	switch(type)
// 	{
// 		case BLOBS:
// 		case INFANTRY:
// 		load_vram(vram_location, attack, 0xC00);
// 		load_palette(pal,soldierpal+offset,1);
// 		break;
// 		case SPEARS:
// 		load_vram(vram_location, attack2, 0xC00);
// 		load_palette(pal, spearpal+offset, 1);
// 		break;
// 		case FLYERS:
// 		load_vram(vram_location, demonbtl, 0xC00);
// 		load_palette(pal, demonbtlpal+offset, 1);
// 		break;
// 		case MUSKETS:
// 		load_vram(vram_location, musketbtl, 0xC00);
// 		load_palette(pal, musketbtlpal+offset, 1);
// 		break;
// 		case HOUNDS:
// 		load_vram(vram_location, houndbtl, 0xC00);
// 		load_palette(pal, houndbtlpal+offset, 1);
// 		break;
// 		case AXES:
// 		load_vram(vram_location, bandit, 0xC00);
// 		load_palette(pal, banditpal+offset, 1);
// 		break;
// 		case MAGES:
// 		deafult:
// 		load_vram(vram_location, attack, 0xC00);
// 		load_palette(pal, spearpal, 1);
// 		break;
// 	}
// }

// void calc_army_stats(char id, Army *army, char x, char y)
// {
// 	Unit unit;
// 	int army_size;
// 	army_size = entities[id].army_size;
// 	// army_size = min(entities[id].army_size, 15);
//
// 	army->atk_total   = unit_list[entities[id].id].atk;
// 	army->def_total   = unit_list[entities[id].id].def;
// 	army->hp_total    = entities[id].hp;//unit_list[entities[id].id].hp  * army_size;
// 	army->hp_current  = army->hp_total;
//
// 	army->current_army_size = army_size;
// 	army->unit_type = entities[id]->unit.unit_type;
// 	army->atk_buff = 0;
// 	army->def_buff = 0;
// 	// army->commander = &commanders[id];
// 	army->army_start_size = army_size;
// 	army->x = x;
// 	army->y = y;
// 	army->healthbars = 7;
// }

void init_armies(int player, int cpu)
{

	int j;
	int i;
	char total_count;
	char *row;

	reset_npcs();

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
	// set_screen_size(SCR_SIZE_32x32);
	// set_map_data(testmap,MAP_WIDTH,BATTLE_MAP_HEIGHT);
	// set_map_data(testmap,16,10);
	// set_tile_data(testtiles,12,testtable,TILE_WIDTH);
	// load_tile(TILE_VRAM);
	// load_map(0,0,0,0,MAP_WIDTH,BATTLE_MAP_HEIGHT);
	// load_map(0,4,0,0,16,10);
	// load_palette(0,testpal,1);
	// load_palette(0,grasspal,1);
	// load_palette(0,battlepal,1);
  // load_vram(TILE_VRAM,battlefield,0x1000);
  // load_background(battle,battlepal,battlebat,32,28);

	// load_information_panel();
	// load_palette(1,grasspal,1);
	set_map_data(batty,16,16);
	set_tile_data(grass,0,grasspal,16);
	// load_vram(0x1000,grass,0x10);
	// cls(0x100);

	// load_healthbars();
	// display_healthbar(4,2);
	// display_healthbar(20,2);
}

int get_y_from_pos(int pos){
	return ((pos) / TILES_PER_LINE);
}

int get_x_from_pos(int pos){
	return (pos % TILES_PER_LINE);
	// return ((pos) & (TILES_PER_LINE - 1));
}

// int populate_army(struct soldier *soldiers, int sprite_size, int sprite_start, int pal, int flip, int army_size, int spriteno)
// {
// 	int i;
// 	for(i = spriteno; i<(army_size + spriteno); i++, soldiers++){
// 		// spr_make(i, ((soldiers->pos & (TILES_PER_LINE-1))*SOLDIER_SIZE), ((soldiers->pos / TILES_PER_LINE)*SOLDIER_SIZE), sprite_start, FLIP_MAS|SIZE_MAS, NO_FLIP|SZ_32x32, pal, 1);
// 		spr_make(i, soldiers->x, soldiers->y, sprite_start, FLIP_MAS|SIZE_MAS, NO_FLIP|SZ_32x32, pal, 1);
//
// 		if(flip){
// 			spr_ctrl(FLIP_X_MASK,FLIP_X);
// 		}
// 	}
// }

void load_pals(int off, char count)
{
	char i;
	for(i=0; i<count; i++)
	{
		switch(npcs[i+off].type)
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
	int i;
	// entities[player_selected_index].army_size = p_c_army.current_army_size;
	// entities[cpu_selected_index].army_size = c_c_army.current_army_size;
	// entities[player_selected_index].hp = p_c_army.hp_current;
	// entities[cpu_selected_index].hp = c_c_army.hp_current;
	for(i=5; i<35; i++)
	{
		spr_hide(i);
	}

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
