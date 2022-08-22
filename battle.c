#incchr(battle,"map/sprites/bg.pcx")
#incpal(battlepal, "map/sprites/bg.pcx")
#incbat(battlebat, "map/sprites/bg.pcx",0x1000,32,28)

#define TILES_PER_LINE 14
#define ARMY_ONE_START 5
#define ARMY_TWO_START 21
#include "battle.h"
// #include "battle_menu.c"
// char grid[NUMBER_OF_TILES];
int army_one_size;
int army_two_size;
char can_attack;
// char cmdr_one_attack = 0;
// char cmdr_two_attack = 0;
// char surrender_army_one = 0;
// char surrender_army_two = 0;
// char kill_count = 0;
int one_dmg, two_dmg, one_health, two_health;
char ticker = 0;
// char attack_start;
// char frame_counter = 0;

// char entity_id_one = 0;
// char entity_id_two = 0;

#define COMMANDER_ONE_VRAM 0x2200
#define COMMANDER_TWO_VRAM 0x3200
#define PLAYER_SPRITE_VRAM 0x6000
#define CPU_SPRITE_VRAM 0x5000

void calc_stats(Army *army_one, Army *army_two)
{
	int one_dmg, two_dmg, one_hp, two_hp, one_avg, two_avg, r, damaged, num, diff;
	if((army_one->hp_current == 0 || army_two->hp_current == 0))
	{
		return;
	}

	one_dmg = rand() % army_one->atk_total;
	two_dmg = rand() % army_two->atk_total;

	if(!can_attack)
	{
		two_dmg = 0;
	}

	one_dmg = one_dmg - ((one_dmg * army_two->def_total)/100);
	two_dmg = two_dmg - ((two_dmg * army_one->def_total)/100);

	army_one->hp_current -= (two_dmg * 5);
	army_two->hp_current -= (one_dmg * 5);

	if(army_one->hp_current <= 0)
	{
		army_one->to_kill = army_one->current_army_size;
		army_one->current_army_size = 0;
		army_one->hp_current = 0;
		army_one->healthbars = 0;
		reduce_healthbar(4,2,0,7,0);
		return;
	}

	if(army_two->hp_current <= 0)
	{
		army_two->to_kill = army_two->current_army_size;
		army_two->current_army_size = 0;
		army_two->hp_current = 0;
		army_two->healthbars = 0;
		reduce_healthbar(20,2,0,7,0);
		return;
	}
	one_hp = (army_one->hp_total / army_one->army_start_size);
	two_hp = (army_two->hp_total / army_two->army_start_size);
	one_avg = roundUp(army_one->hp_current,one_hp);
	two_avg = roundUp(army_two->hp_current,two_hp);

	// if(army_one->hp_current % one_hp)
		army_one->to_kill = army_one->current_army_size - one_avg;
		army_one->current_army_size = one_avg;
	// if(army_two->hp_current % two_hp)
		army_two->to_kill = army_two->current_army_size - two_avg;
		army_two->current_army_size = two_avg;

	num = roundUp(7*army_one->current_army_size,army_one->army_start_size);
	if(num < army_one->healthbars)
	{
		diff = army_one->healthbars - num;
		army_one->healthbars = num;
		// reduce_healthbar(army_one->x,army_one->y,army_one->healthbars,diff,0);
		reduce_healthbar(4,2,army_one->healthbars,diff,0);
	}

	num = roundUp(7*army_two->current_army_size,army_two->army_start_size);
	if(num < army_two->healthbars)
	{
		diff = army_two->healthbars - num;
		army_two->healthbars = num;
		// reduce_healthbar(army_two->x,army_two->y,army_two->healthbars,diff,0);
		reduce_healthbar(20,2,army_two->healthbars,diff,0);
	}
}

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

/************************************************************
 * 															 														*
 *  				Soldier State Functions					 								*
 * 															 														*
 ************************************************************/

void soldier_state_machine(struct soldier *soldier){
	if(soldier->state == 0) soldier_idle_state(soldier);
	if(soldier->state == 1) soldier_move_state(soldier);
	if(soldier->state == 2) return;//soldier_attack_state(soldier);
	if(soldier->state == 3) soldier_die_state(soldier);
	// if(soldier->state == 4) ;
}

void soldier_idle_state(struct soldier *soldier){
}

void arts()
{
}

void advance(struct soldier *soldier)
{
}

void offense(struct soldier *soldier)
{
}

void update_position(struct soldier *soldier, int nx, int ny)
{
}

void soldier_move_state(struct soldier *soldier)
{
}

void soldier_attack_state(struct soldier *soldier)
{
}

void soldier_die_state(struct soldier *soldier)
{
}

int get_opp_avg_distance()
{
}

struct soldier* get_unit_from_position(int unit_position){
}

void kill_unit(struct soldier* s,int id,int area_id){
}

void intro()
{
	struct soldier *soldier;
	int moves, i, f, t, frame, tmp_frame;

	frame = 0;
	t = 0;
	f=0;
	// xOffset = -64;

	// for(i=0, soldier = armyOne; i<army_one_size; i++, soldier++)
	// {
	// 	spr_set(soldier->id);
	// 	spr_show(soldier-id);
	// 	spr_x(soldier->x + xOffset);
	// 	spr_y(soldier->y);
	// }
	//
	// for(i=0, soldier = armyTwo; i<army_two_size; i++, soldier++)
	// {
	// 	spr_set(soldier->id);
	// 	spr_show(soldier->id);
	// 	spr_x(soldier->x + xOffset);
	// 	spr_y(soldier->y);
	// }
	satb_update();
	vsync(10);

	moves = 15;
	t = 0;

	while(moves > 0)
	{
		if(t++ == 6)
		{
			t = 0;
			// frame += 0x100;
			// frame %= 0x400;

			change_army_frame(armyOne,army_one_size,PLAYER_SPRITE_VRAM,-1);
			change_army_frame(armyTwo,army_two_size,CPU_SPRITE_VRAM,1);

			randomize_army_attacks(armyOne,army_one_size,PLAYER_SPRITE_VRAM + 0x300);

			if(can_attack)
			{
				randomize_army_attacks(armyTwo,army_two_size,CPU_SPRITE_VRAM + 0x300);
			}
		}
		if(i++ % 24 == 0)
		{
			calc_stats(&p_c_army, &c_c_army);
			put_number(p_c_army.atk_total,3,0,0);
			put_number(c_c_army.atk_total,3,24,0);
			put_number(p_c_army.def_total,3,0,1);
			put_number(c_c_army.def_total,3,24,1);
			put_number(p_c_army.hp_current,6,0,2);
			put_number(c_c_army.hp_current,6,24,2);
			update_army_size(&p_c_army,armyOne,PLAYER_SPRITE_VRAM+0x800);
			update_army_size(&c_c_army,armyTwo,CPU_SPRITE_VRAM+0x800);
			p_c_army.to_kill = 0;
			c_c_army.to_kill = 0;
			moves--;

			if(p_c_army.current_army_size == 0 || c_c_army.current_army_size == 0)
			{
				moves = 0;
			}
			// update_stats();
		}
		satb_update();
		vsync();
	}
	moves = 6;
	while(moves > 0)
	{
		if(t++ == 6)
		{
			t = 0;
			change_army_frame(armyOne,army_one_size,PLAYER_SPRITE_VRAM,-1);
			change_army_frame(armyTwo,army_two_size,CPU_SPRITE_VRAM,1);
		}
		if(i++ % 24 == 0)
		{
			moves--;
		}
		satb_update();
		vsync();
	}
}

void randomize_army_attacks(struct soldier *army, char army_size, int frame)
{
	struct soldier *soldier;
	char i;

	for(i = 0, soldier = army; i<army_size; i++, soldier++)
	{
		if(soldier->active && soldier->frame < frame && soldier->state == IDLE)
		{
			if((rand() & 255) > 240 && soldier->frame < frame)
			{
				soldier->frame = frame;
				soldier->tic = 0;
				soldier->state = ATTACKING;
				spr_set(soldier->id);
				spr_pattern(soldier->frame);
			}
		}
	}
}

void update_army_size(Army *army, struct soldier *soldiers,int frame)
{
	struct soldier *soldier;
	char i;
	int num;

	for(i=0; i<army->to_kill; i++)
	{
		num = get_random_unit_id(soldiers,army->army_start_size);
		soldier = (soldiers + num);
		// soldier->active = 0;
		soldier->state = DYING;
		soldier->frame = frame;
	}
}

int get_random_unit_id(struct soldier *soldier, int size)
{
	int army_size, r, i;
	i=0;
	army_size = size;
	while(1)
	{
		r = rand() & 15;
		if(r < size && (soldier + r)->active)
		{
			// put_number(r,3,0,6+i++);
			return r;
		}
	}
}

void update_army_frame(struct soldier *army, char army_size, int frame)
{
	struct soldier *soldier;
	char i;
	for(i = 0, soldier = army; i<army_size; i++, soldier++)
	{
		if(soldier->active)
		{
			soldier->frame = frame;
		}
	}
}

void change_army_frame(struct soldier *army, int army_size, int frame, int direction)
{
	struct soldier *soldier;
	char i;

	for(i = 0, soldier = army; i<army_size; i++, soldier++)
	{
		if(soldier->active)
		{
			spr_set(soldier->id);
			if(animation_ended(soldier))
			{
				if(soldier->state == DYING)
				{
					soldier->active = 0;
					spr_y(-32);
					spr_hide();
				}
				else
				{
					soldier->frame = frame;
					soldier->tic = 0;
					soldier->state = IDLE;
					spr_pattern(soldier->frame);
				}
			}
			else
			{
				// soldier->tic = (soldier->tic + 1 ) % 4;
				if(soldier->state == DYING)
				{
					spr_y(spr_get_y()-1);
					spr_x(spr_get_x()+direction);
					spr_pattern((soldier->frame) + ((soldier->tic/2) * 0x100));
					soldier->tic++;
				}
				else
				{
					spr_pattern((soldier->frame) + (soldier->tic * 0x100));
					soldier->tic++;
				}
			}
		}
	}
}

void animation_ended(struct soldier *soldier)
{
	switch(soldier->state)
	{
		case IDLE:
		if(soldier->tic == 3)
		{
			return 1;
		}
			break;

		case ATTACKING:
		if(soldier->tic == 5)
		{
			return 1;
		}
			break;

		case DYING:
		if(soldier->tic == 4*2) //return 1;
		{
			// soldier->active = 0;
			return 1;
		}
			break;
	}
	return 0;
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
	// entity_id_one = i1;s
	// entity_id_two = i2;
	// p_left_reserve_size = 3;
	// p_right_reserve_size = 3;
	//
	// c_left_reserve_size = 3;
	// c_right_reserve_size = 3;

	// army_one_current_size = 0;
	// army_two_current_size = 0;

	army_one_size = 0;
	army_one_size = 0;
	total_sprites = 0;
	can_attack = target_attack;
	xOffset = -96;

	// xOffset = 0;
	// disp_off();
	scroll(0, 0, 0, 0, 223, 0xC0);
	load_battle_scene(i1,i2);

	// cursor_vram = 0x7000;
	// load_vram(cursor_vram,cursor,0x40);
	// load_cursor(91,12);

	// display_menu(&menus[0]);

	// comm_1 = 0;
	// comm_2 = 0;
	one_dmg = 0;
	two_dmg = 0;
	one_health = 60;
	two_health = 120;

	intro();
	// while(army_one_current_size > 0 && army_two_current_size > 0 && total_units > 2 && surrender_army_one && surrender_army_two)
	// {
	// 	loop();
	// }

	cleanup_battle(i1,i2);

	if(p_c_army.current_army_size == 0)//attacker
	{
		return 0;
	}
	else if(c_c_army.current_army_size == 0)//target
	{
		return 1;
	}
	else
	{
		return 2;
	}
}

void load_battle_scene(int i1, int i2)
{
	disp_off();
	reset_satb();
	// init_satb();
	load_battle_bg();
	// reset_camera();
	/* load menu */
	// load_vram(PLAYER_SPRITE_VRAM, attack2, 0xC00);
	if(entities[i1].team == PLAYER)
	{
		load_unit_type_to_vram(PLAYER_SPRITE_VRAM,entities[i1]->unit.unit_type,16,0);
		load_unit_type_to_vram(CPU_SPRITE_VRAM,entities[i2]->unit.unit_type,17,16);
	}
	else
	{
		load_unit_type_to_vram(PLAYER_SPRITE_VRAM,entities[i1]->unit.unit_type,16,16);
		load_unit_type_to_vram(CPU_SPRITE_VRAM,entities[i2]->unit.unit_type,17,0);
	}

	set_font_color(0, 12);
	set_font_pal(0);
	load_default_font(0,0x490);

	init_armies(i1,i2);
	disp_on();

	// surrender_army_one = 1;
	// surrender_army_two = 1;
}

void load_unit_type_to_vram(int vram_location, enum Unit_Type type, char pal,char offset)
{
	switch(type)
	{
		case BLOBS:
		case INFANTRY:
		load_vram(vram_location, attack, 0xC00);
		load_palette(pal, soldierpal+offset, 1);
		break;
		case SPEARS:
		load_vram(vram_location, attack2, 0xC00);
		load_palette(pal, spearpal+offset, 1);
		break;
		case FLYERS:
		load_vram(vram_location, demonbtl, 0xC00);
		load_palette(pal, demonbtlpal+offset, 1);
		break;
		case MUSKETS:
		load_vram(vram_location, musketbtl, 0xC00);
		load_palette(pal, musketbtlpal+offset, 1);
		break;
		case HOUNDS:
		load_vram(vram_location, houndbtl, 0xC00);
		load_palette(pal, houndbtlpal+offset, 1);
		break;
		case BANDITS:
		load_vram(vram_location, bandit, 0xC00);
		load_palette(pal, banditpal+offset, 1);
		break;
		case MAGES:
		deafult:
		load_vram(vram_location, attack, 0xC00);
		load_palette(pal, spearpal, 1);
		break;
	}
}

void calc_army_stats(char id, Army *army, char x, char y)
{
	Unit unit;
	int army_size;
	army_size = entities[id].army_size;
	// army_size = min(entities[id].army_size, 15);

	// memcpy(&unit,entities[id].unit,sizeof(Unit));

	army->atk_total   = unit_list[entities[id].id].atk;
	army->def_total   = unit_list[entities[id].id].def;
	army->hp_total    = entities[id].hp;//unit_list[entities[id].id].hp  * army_size;
	army->hp_current  = army->hp_total;

	army->current_army_size = army_size;
	army->unit_type = entities[id]->unit.unit_type;
	army->atk_buff = 0;
	army->def_buff = 0;
	// army->commander = &commanders[id];
	army->army_start_size = army_size;
	army->x = x;
	army->y = y;
	army->healthbars = 7;
}

void init_armies(int player, int cpu)
{
	int p;
	int row;
	int column;
	char j;

	calc_army_stats(player,&p_c_army,4,2);
	calc_army_stats(cpu,&c_c_army,24,2);

	put_number(entities[player].id,2,8,1);
	put_number(entities[cpu].id,2,11,1);

	army_one_size = p_c_army.army_start_size; //MAX_ARMY_SIZE;
	army_two_size = c_c_army.army_start_size; //MAX_ARMY_SIZE;

	total_units = army_one_size + army_two_size;

	p = 0;
	row = 0;
	column = 7;
	for(j=0, sp = armyOne; j<army_one_size; ++j, ++sp)
	{
		p = (((j%5) + row) * TILES_PER_LINE) + (column - (j/5)); //32 + (2 - 0)
		// sp->pos = p;
		sp->active = 1;
		sp->id = j + ARMY_ONE_START;
		sp->state = 0;
		sp->tic = 0;
		sp->x = (get_x_from_pos(p)*20)-64;
		sp->y = (get_y_from_pos(p)*32)+64;
		sp->frame = PLAYER_SPRITE_VRAM;
	}
	// sp--;
	j--;
	p = 0;
	row = 0;
	column = 10;
	for(j=0, sp = armyTwo; j<army_two_size; ++j, ++sp)
	{
		p = (((j%5) + row) * TILES_PER_LINE) + (column + (j/5));
		// sp->pos = p;
		sp->active = 1;
		sp->id = j + ARMY_TWO_START;
		sp->state = 0;
		sp->tic = 0;
		sp->x = (get_x_from_pos(p)*20)-64;
		sp->y = (get_y_from_pos(p)*32)+64;
		sp->frame = CPU_SPRITE_VRAM;
	}

	sp = armyOne;
	//still zero here
	populate_army(sp,0x400,PLAYER_SPRITE_VRAM,16,0,army_one_size,5);
	//not zero here

	sp = armyTwo;
	populate_army(sp,0x400,CPU_SPRITE_VRAM,17,1,army_two_size,21);

	satb_update();
	// load_commanders();
	vsync();
}

void toggle_position(int pos, int id){}

void load_battle_bg()
{
	// // set_screen_size(SCR_SIZE_32x32);
	// // set_map_data(testmap,MAP_WIDTH,BATTLE_MAP_HEIGHT);
	// set_map_data(testmap,16,10);
	// set_tile_data(testtiles,12,testtable,TILE_WIDTH);
	// load_tile(TILE_VRAM);
	// // load_map(0,0,0,0,MAP_WIDTH,BATTLE_MAP_HEIGHT);
	// load_map(0,4,0,0,16,10);
	// load_palette(0,testpal,1);

	load_palette(0,battlepal,1);
  // load_vram(TILE_VRAM,battlefield,0x1000);
  load_background(battle,battlepal,battlebat,32,28);

	// load_information_panel();
	load_healthbars();
	display_healthbar(4,2);
	display_healthbar(20,2);
}

int get_y_from_pos(int pos){
	return ((pos) / TILES_PER_LINE);
}

int get_x_from_pos(int pos){
	return (pos % TILES_PER_LINE);
	// return ((pos) & (TILES_PER_LINE - 1));
}

char prebattle_select(char selection)
{
	switch(selection)
	{
		case 2:
		return 1;

		default:
		return 0;
	}
}

char formation_select(char selection)
{
	switch(selection)
	{
		default:
		// comm_1 = selection;
		// comm_2 = selection;
		return 0;
		// break;
	}
}

int populate_army(struct soldier *soldiers, int sprite_size, int sprite_start, int pal, int flip, int army_size, int spriteno)
{
	int i;
	for(i = spriteno; i<(army_size + spriteno); i++, soldiers++){
		// spr_make(i, ((soldiers->pos & (TILES_PER_LINE-1))*SOLDIER_SIZE), ((soldiers->pos / TILES_PER_LINE)*SOLDIER_SIZE), sprite_start, FLIP_MAS|SIZE_MAS, NO_FLIP|SZ_32x32, pal, 1);
		spr_make(i, soldiers->x, soldiers->y, sprite_start, FLIP_MAS|SIZE_MAS, NO_FLIP|SZ_32x32, pal, 1);
		if(the_sy_val == 0 && sy > 192)
		{
			the_sy_val = path;
		}
		if(flip){
			spr_ctrl(FLIP_X_MASK,FLIP_X);
		}
	}
}

void cleanup_battle(int player_selected_index, int cpu_selected_index)
{
	int i;
	entities[player_selected_index].army_size = p_c_army.current_army_size;
	entities[cpu_selected_index].army_size = c_c_army.current_army_size;
	entities[player_selected_index].hp = p_c_army.hp_current;
	entities[cpu_selected_index].hp = c_c_army.hp_current;
	for(i=5; i<35; i++)
	{
		spr_hide(i);
	}

	for(i=0, sp=armyOne; i<15; i++)
	{
		sp->active = 0;
		sp->id = 0;
		sp->state = 0;
		sp->tic = 0;
		sp->x = 0;
		sp->y = 0;
		sp->state = 0;
		sp->direction = 0;
	}

	for(i=0, sp=armyTwo; i<15; i++)
	{
		sp->active = 0;
		sp->id = 0;
		sp->state = 0;
		sp->tic = 0;
		sp->x = 0;
		sp->y = 0;
		sp->state = 0;
		sp->direction = 0;
	}

	sp = 0;
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
