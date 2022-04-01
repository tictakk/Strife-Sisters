#incchr(battle,"map/sprites/bg.pcx")
#incpal(battlepal, "map/sprites/bg.pcx")
#incbat(battlebat, "map/sprites/bg.pcx",0x1000,32,28)

#define TILES_PER_LINE 14
#define ARMY_ONE_START 5
#define ARMY_TWO_START 21
#include "battle.h"
// #include "battle_menu.c"
char grid[NUMBER_OF_TILES];
int army_one_size = 0;
int army_two_size = 0;
char cmdr_one_attack = 0;
char cmdr_two_attack = 0;
char surrender_army_one = 0;
char surrender_army_two = 0;
char kill_count = 0;
int one_dmg, two_dmg, one_health, two_health;
char ticker = 0;
char attack_start;
char frame_counter = 0;

char entity_id_one = 0;
char entity_id_two = 0;

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
	// put_number(one_dmg,3,1,1);
	// put_number(two_dmg,3,1,2);
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

void controls(){
	j1 = joy(0);
	j2 = joytrg(0);
	// if (j1 & JOY_LEFT)
	// {
	// 	if(sx > 0){
	// 		xOffset += 2;
	// 		sx = (sx -= 2) & 511;
	// 		scroll(0, (sx >> 1), (sy >> 1), 0, 223, 0xC0);
	// 	}
	// }
	// if (j1 & JOY_RIGHT)
	// {
	// 	if(sx < 510){
	// 		xOffset -= 2;
	// 		sx = (sx +=2) & 511;
	// 		scroll(0, (sx >> 1), (sy >> 1), 0, 223, 0xC0);
	// 	}
	// }
	// if (j1 & JOY_UP)
	// {
	// 	if(sy > 0)
	// 	{
	// 		yOffset += 2;
	// 		sy = (sy -= 2) & 255;
	// 		scroll(0, (sx >> 1), (sy >> 1), 0, 223, 0xC0);
	// 	}
	// }
	// if (j1 & JOY_DOWN)
	// {
	// 	if(sy < 64)
	// 	{
	// 		yOffset -= 2;
	// 		sy = (sy += 2) & 255;
	// 		scroll(0, (sx >> 1), (sy >> 1), 0, 223, 0xC0);
	// 	}
	// }

	if(j2 & JOY_RUN)
	{
		load_in_battle_menu();
		menu_loop(5);
		if(comm_1 == 5)
		{
			surrender_army_one = 0;
		}
	}

	if(j2 & JOY_I)
	{
		// reinforce(army_one_current_size,&p_right_reserve_size,1);
		// reinforce(army_two_current_size,&c_right_reserve_size,2);

		// select();
		rand();
		// display_menu(0);
		// if(health < 255)
		// {
			// reduce_healthbar(24,6,health--);
		// 	put_number(health,3,7,8);
		// }
	}

	if(j2 & JOY_II)
	{
		// back();
		rand();
		// c_menu();
	}

	if(j2 & JOY_DOWN)
	{
		// cur_pos++;
		// update_cursor();
		rand();
		// spr_set(0);
		// spr_y(spr_get_y()+8);
	}

	if(j2 & JOY_UP)
	{
		// spr_set(0);
		// spr_y(spr_get_y()-8);
		// cur_pos--;
		// update_cursor();
		rand();
	}
}

int is_opp_adjacent(int gridpos)
{
	if((grid[gridpos-1] & opp) == opp && (gridpos%TILES_PER_LINE)) return gridpos-1;
	if((grid[gridpos+1] & opp) == opp) return gridpos+1;
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
	int opp_pos;
	opp_pos = is_opp_adjacent(soldier->pos);

	if(opp_pos)
	{//then attack
		soldier->state = 2;
		soldier->frame = 0x4800;
		attack_start = 1;
	}
	else
	{
		// do_command(sp);
		advance(sp);
	}
}

void arts()
{
	// deathball();
}

void advance(struct soldier *soldier)
{
	int x, y;
	y = get_y_from_pos(soldier->pos);
	x = get_x_from_pos(soldier->pos);
	if(team_id == 1 && x == (TILES_PER_LINE/2)-1)
	{
		return;
	}
	else if(team_id == 2 && x == TILES_PER_LINE/2)
	{
		return;
	}
	//if army one + 1 if army two - 1
	if(team_id == 1)
	{
		update_position(soldier,x+1,y);
			// soldier->state = 4;
	}else{
		update_position(soldier,x-1,y);
			// soldier->state = 4;
	}
}

void offense(struct soldier *soldier)
{
	int opp_x, opp_y, weight_x, weight_y, x, y, r, north, east, south, west, total_weight, remain_weight;

	total_weight = 16;
	remain_weight = 16;
	r = 0;
	weight_x = 0;
	weight_y = 0;
	opp_x = 0;
	opp_y = 0;
	// north = 63;
	// east = 127;
	// south = 191;
	// west = 255;
	north = 4;
	east = 4;
	south = 4;
	west = 4;
	r = rand() & 255;

	opp_x = get_x_from_pos(avg);
	opp_y = get_y_from_pos(avg);
	x = get_x_from_pos(soldier->pos);
	y = get_y_from_pos(soldier->pos);

	weight_x = opp_x - x; //distance
	weight_y = opp_y - y;

	if(weight_x > 0)
	{
		// weight = 31 + (31 / weight_x);
		// east += weight;
		// south += weight;
		weight_x = abs(weight_x);

		if(weight_x > 4) weight_x = 4;
		east += weight_x;
		west -= weight_x;

		// east += total_weight - (total_weight - weight_x);
		// west += 4 - east;
		// if(west < 0) west = 0;
		// remain_weight = total_weight - (east + west);
	}
	else if(weight_x < 0) //x = -1
	{
		// weight = 31 + (31 / (weight_x *  -1));
		// east -= weight;
		// south -= weight;
		weight_x = abs(weight_x);

		if(weight_x > 4) weight_x = 4;
		east -= weight_x;
		west += weight_x;
	}

	if(weight_y > 0)
	{
		weight_y = abs(weight_y);

		if(weight_y > 4) weight_y = 4;

		south += weight_y;
		north -= weight_y;
		// south += (weight - (east + west));
		// north -= weight_y;
		// south += weight_y;

	}
	else if(weight_y < 0) // y = -1 opp below
	{
		weight_y = abs(weight_y);

		if(weight_y > 4) weight_y = 4;

		north += weight_y;
		south -= weight_y;
	}

	north = north * 16;
	east = (east * 16) + north;
	south = (south * 16) + east;
	west = (west * 16) + east;

	if(r < north)
	{
		y -= 1;
	}
	else if(r < east)
	{
		x += 1;
	}
	else if(r < south)
	{
		y += 1;
	}
	else if(r < west)
	{
		x -= 1;
	}

	update_position(soldier,x,y);
}

void update_position(struct soldier *soldier, int nx, int ny)
{
	int x,y,desired;
	desired = 0;
	// x = get_x_from_pos(soldier->pos);
	// y = get_y_from_pos(soldier->pos);

	desired = (ny * TILES_PER_LINE) + nx;

	if((nx > -1 && nx < TILES_PER_LINE) && (ny > -1 && ny < 5) && ((grid[desired] & 1)!=1) && ((grid[desired]) & 2 ) != 2)
	{
		x = get_x_from_pos(soldier->pos);
		y = get_y_from_pos(soldier->pos);

		grid[soldier->pos-1] ^= team_area;
		grid[soldier->pos] ^= team_id;
		grid[soldier->pos+1] ^= team_area;

		if(x > nx)
		{
			soldier->direction = 3;
			soldier->walk = 32;
			soldier->x = 32;
			// spr_ctrl(FLIP_X_MASK,NO_FLIP_X);
		}
		else if (y > ny)
		{
			soldier->direction = 0;
			soldier->walk = 32;
			soldier->y = 32;
		}
		else if (y < ny)
		{
			soldier->direction = 2;
			soldier->walk = 32;
			soldier->y = -32;
		}
		else if (x < nx)
		{
			soldier->direction = 1;
			soldier->walk = 32;
			soldier->x = -32;
			// spr_ctrl(FLIP_X_MASK,FLIP_X);
		}

		soldier->pos = (ny * TILES_PER_LINE) + nx;
		soldier->state = 1;
		soldier->frame = 0x4400;

		grid[soldier->pos-1] ^= team_area;
		grid[soldier->pos] ^= team_id;
		grid[soldier->pos+1] ^= team_area;

		return 1;
	}
	else
	{
		return 0;
	}
}

void soldier_move_state(struct soldier *soldier){
	int opp;
	if(soldier->walk == 1){
		opp = is_opp_adjacent(soldier);
		if(opp)
		{
			soldier->state = 3;
			// sodlier->frame = 0x4400;
		}
		// advance()
		soldier->state = 0;
		soldier->frame = 0x4000;
		// do_command(soldier);

		// do_command(soldier);
		// soldier_idle_state(soldier);
	}

	switch(soldier->direction)
	{
		case 0:
			soldier->walk--;
			soldier->y--;
			break;
		case 1:
			soldier->walk--;
			soldier->x++;
			break;
		case 2:
			soldier->walk--;
			soldier->y++;
			break;
		case 3:
			soldier->walk--;
			soldier->x--;
			break;
	}
	// update_positin(soldier);
	update_sprite_pos(soldier);
}

void soldier_attack_state(struct soldier *soldier){
	int r, nearest_opp, ran;
	struct soldier* opponent;
	attack_start = 1;

	if(soldier->tic++ == 24){
		r = rand() & 255;
		ran = rand() & 255;
		nearest_opp = is_opp_adjacent(soldier->pos);

		if((nearest_opp & 127) < (soldier->pos & 127))
		{
			// spr_ctrl(FLIP_X_MASK,NO_FLIP_X);
		}else{
			// spr_ctrl(FLIP_X_MASK,FLIP_X);
		}
		if(nearest_opp == 0)
		{
			soldier->tic = 0;
			soldier->state = 0;
			return;
		}
		if(nearest_opp == commander_two_pos)//opp is commander
		{
			cmdr_two_attack = 1;
			if(r > 255)
			{
				commander_two_hp--;
			}
			if(ran > 255)
			{
				cmdr_two_attack = 0;
				kill_unit(soldier,team_id,team_area);
			}else{
				soldier->state = 0;
			}
			soldier->tic = 0;
		}
		else if(nearest_opp == commander_one_pos)
		{
			cmdr_one_attack = 1;
			if(r > 255)
			{
				commander_one_hp--;
			}
			if(ran > 255)
			{
				cmdr_one_attack = 0;
				kill_unit(soldier,team_id,team_area);
			}else{
				soldier->state = 0;
			}
			soldier->tic = 0;
		}
		else
		{ //opponent is a soldier
			opponent = get_unit_from_position(nearest_opp);

			if(r > 255)
			{
				kill_unit(opponent,opp,opp_area);
				soldier->state = 0;
			}
			soldier->tic = 0;
		}
	}
}

void soldier_die_state(struct soldier *soldier){
	soldier->active = 0;
	total_units--;
	spr_hide(soldier->id);
	if(soldier->id >= ARMY_TWO_START)
	{
		army_two_current_size--;
	}
	else
	{
		army_one_current_size--;
	}
}

int get_opp_avg_distance()
{
	// struct soldier *opp;
	// int i, x, y, counter;
	// counter = 0;
	// i=0;
	// x=0;
	// y=0;
	// avg=0;
	// for(i=0, opp = opp_army; i<opp_army_size; ++i, ++opp)
	// {
	// 	if(opp->active == 1 && opp->state !=3)
	// 	{
	// 		avg =	opp->pos;
	// 		x += get_x_from_pos(avg);
	// 		y += get_y_from_pos(avg);
	// 		counter++;
	// 	}
	// }
	// if(counter == 0)
	// {
	// 	return 0;
	// }
	//
	// x /= counter;
	// y /= counter;
	// return (y * TILES_PER_LINE) + x;
}

struct soldier* get_unit_from_position(int unit_position){
	// int c;
	// struct soldier *opponent;
	// for(c=0, opponent = opp_army; c<team_army_size; ++c, ++opponent) //army one stuff
	// {
	// 	if(opponent->pos == unit_position){
	// 		return opponent;
	// 	}
	// }
}

void kill_unit(struct soldier* s,int id,int area_id){
	// grid[s->pos] ^= opp;
	// grid[s->pos-1] ^= opp_area;
	// grid[s->pos+1] ^= opp_area;
	// s->pos = -128;
	// s->state = 3;

	grid[s->pos] ^= id;
	grid[s->pos-1] ^= area_id;
	grid[s->pos+1] ^= area_id;
	s->pos = -TILES_PER_LINE;
	s->state = 3;
	// current_opp_army_size -= 1;
	// spr_hide();
}

void intro()
{
	struct soldier *soldier;
	int moves, i, f, tick, frame, tmp_frame;

	frame = 0;
	tick = 0;
	f=0;
	// xOffset = 0;

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
	tick = 0;

	while(moves > 0)
	{
		if(tick++ == 6)
		{
			tick = 0;
			frame += 0x100;
			frame %= 0x400;

			change_army_frame(armyOne,army_one_size,PLAYER_SPRITE_VRAM);
			change_army_frame(armyTwo,army_two_size,CPU_SPRITE_VRAM);

			randomize_army_attacks(armyOne,army_one_size,PLAYER_SPRITE_VRAM + 0x800);
			randomize_army_attacks(armyTwo,army_two_size,CPU_SPRITE_VRAM + 0x800);
		}
		if(i++ % 24 == 0)
		{
			calc_stats(&p_c_army, &c_c_army);
			put_number(p_c_army.current_army_size,3,0,0);
			put_number(c_c_army.current_army_size,3,24,0);
			// put_number(p_c_army.to_kill,3,0,1);
			// put_number(c_c_army.to_kill,3,24,1);
			update_army_size(&p_c_army,armyOne);
			update_army_size(&c_c_army,armyTwo);
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
	moves = 4;
	while(moves > 0)
	{
		if(tick++ == 6)
		{
			tick = 0;
			change_army_frame(armyOne,army_one_size,PLAYER_SPRITE_VRAM);
			change_army_frame(armyTwo,army_two_size,CPU_SPRITE_VRAM);
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
		if(soldier->active && soldier->frame < frame)
		{
			if((rand() & 255) > 240 && soldier->frame < frame)
			{
				soldier->frame = frame;
				soldier->tic = 0;
				spr_set(soldier->id);
				spr_pattern(soldier->frame);
			}
		}
	}
}

void update_army_size(Army *army, struct soldier *soldiers)
{
	struct soldier *soldier;
	char i;
	int num;

	for(i=0; i<army->to_kill; i++)
	{
		num = get_random_unit_id(soldiers,army->army_start_size);
		if(army->commander->id == 2)
		{
			put_number(num,3,0,6);
		}
		soldier = (soldiers + num);
		soldier->active = 0;

		spr_set(soldier->id);
		spr_x(-32);
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

void change_army_frame(struct soldier *army, char army_size, int frame)
{
	struct soldier *soldier;
	char i;
	for(i = 0, soldier = army; i<army_size; i++, soldier++)
	{
		if(soldier->active)
		{
			spr_set(soldier->id);
			if(soldier->tic == 4)
			{
				// soldier->frame = 0x5000;
				soldier->frame = frame;
				soldier->tic = 1;
				spr_pattern(soldier->frame);
			}
			else
			{
				// soldier->tic = (soldier->tic + 1 ) % 4;
				spr_pattern((soldier->frame) + (soldier->tic++ * 0x100));
			}
			// spr_pattern(soldier->frame + frame);
		}
	}
}

void move_army_left(struct soldier *army, char army_size)
{
	struct soldier *soldier;
	char i;
	for(i=0, soldier = army; i<army_size; i++, soldier++)
	{
		if(soldier->active)
		{
			spr_set(soldier->id);
			spr_x(soldier->x-- + xOffset);
		}
	}
}

void move_army_right(struct soldier *army, char army_size)
{
	struct soldier *soldier;
	char i, j;
	j=0;
	for(i=0, soldier = army; i<army_size; i++, soldier++)
	{
		if(soldier->active)
		{
			spr_set(soldier->id);
			spr_x(soldier->x++ + xOffset);
			// spr_y(soldier->y);
		}
	}
}

void reset_camera()
{
	char z;
	sx = 0;
	sy = 0;
	yOffset = 0;
	xOffset = 0;

	scroll(0, 0, 0, 0, 223, 0xC0);
}

void camera_right(int scroll_amt, int x)
{
	camera_update(scroll_amt,x,0);
}

void camera_left(int scroll_amt, int x)
{
	camera_update(scroll_amt*-1,x*-1,0);
}

void camera_down(int scroll_amt, int y)
{
	camera_update(scroll_amt,0,y);
}

void camera_up(int scroll_amt, int y)
{
	camera_update(scroll_amt*-1,0,y*-1);
}

void camera_update(int scroll_amt, int x, int y)
{
	int start, incr;
	incr = x+y;
	for(start=0; start!=scroll_amt; sx+=x, sy+=y, xOffset-=x, yOffset-=y, start+=incr)
	{
		scroll(0, (sx >> 1), (sy >> 1), 0, 223, 0xC0);
		satb_update();
		vsync();
		// loop_armies(1);
	}
}

void camera_shake(char shakes)
{
	int i;
	i=0;
	while(shakes-- > 0)
	{
		// camera_up(4,2);
		// camera_down(4,2);
		camera_left(4,2);
		camera_right(4,2);
		// if((i++)%4 == 0)
		// {
			change_army_frame(armyOne,army_one_size,((i%4)*0x100));
			change_army_frame(armyTwo,army_two_size,((i%4)*0x100));
			satb_update();
			vsync();
		// }
		i++;
	}
}



void load_spell()
{
	spr_make(total_sprites++,64,104,0x6880,FLIP_MAS|SIZE_MAS,NO_FLIP|SZ_16x16,6,0);
	spr_pri(1);
}

void loop_armies(char i)
{
	char z, loops;
	loops = i;
	while(loops-- > 0)
	{
		vsync();
		tick();
		for(z=0, sp = armyOne; z<army_one_size; ++z, ++sp)
		{
			if(sp->active){
				// spr_set(z+sprite_offset);
				spr_set(sp->id);
				// spr_pattern(soldierFrame+(sp->state * 0x400));
				spr_pattern(soldierFrame);

				spr_y(get_y_from_pos(sp->pos) * 32 + sp->y + (yOffset>>1));
				spr_x(get_x_from_pos(sp->pos) * 32 + sp->x + (xOffset>>1));
			}
		}

		for(z=army_one_size, sp = armyTwo; z<(army_one_size + army_two_size); ++z, ++sp)
		{ //have to offset army one's size
			if(sp->active){
				// spr_set(z+sprite_offset);
				spr_set(sp->id);
				// spr_pattern(soldierFrame+(sp->state * 0x400));
				spr_pattern(soldierFrame);


				spr_y(get_y_from_pos(sp->pos) * 32 + sp->y + (yOffset>>1));
				spr_x(get_x_from_pos(sp->pos) * 32 + sp->x + (xOffset>>1));
			}
		}
		spr_set(62);
		spr_y(get_y_from_pos(commander_one_pos) * 32 + (yOffset>>1));
		spr_x(get_x_from_pos(commander_one_pos) * 32 + (xOffset>>1));

		spr_set(63);
		spr_y(get_y_from_pos(commander_two_pos) * 32 + (yOffset>>1));
		spr_x(get_x_from_pos(commander_two_pos) * 32 + (xOffset>>1));

		satb_update();
		// vsync();
	}
}

/*
	BATTLE ROUTINE
*/

char battle_loop(char i1, char i2)
{
	entity_id_one = i1;
	entity_id_two = i2;
	// p_left_reserve_size = 3;
	// p_right_reserve_size = 3;
	//
	// c_left_reserve_size = 3;
	// c_right_reserve_size = 3;

	army_one_current_size = 0;
	army_two_current_size = 0;
	total_sprites = 0;
	// xOffset = -96;
	xOffset = -64;
	// disp_off();
	scroll(0, 0, 0, 0, 223, 0xC0);
	load_battle_scene(i1,i2);

	// cursor_vram = 0x7000;
	// load_vram(cursor_vram,cursor,0x40);
	// load_cursor(91,12);

	// display_menu(&menus[0]);

	comm_1 = 0;
	comm_2 = 0;
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

	if(p_c_army.current_army_size == 0)
	{
		return 0;
	}
	else if(c_c_army.current_army_size == 0)
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
	load_unit_type_to_vram(PLAYER_SPRITE_VRAM,entities[i1]->unit.unit_type);
	if(entities[i1]->unit.unit_type != MUSKETS)
	{
		load_palette(16, soldierpal, 1);
	}
	else
	{
		load_palette(16, musketbtlpal, 1);
	}

	/* init sprites */
	// load_palette(16, soldierpal, 1);
	// load_vram(CPU_SPRITE_VRAM, attack, 0xC00);
	load_unit_type_to_vram(CPU_SPRITE_VRAM,entities[i2]->unit.unit_type);

	if(entities[i2]->unit.unit_type != MUSKETS)
	{
		load_palette(17, soldierpal, 1);
	}
	else
	{
		load_palette(17, musketbtlpal, 1);
	}

	set_font_color(0, 12);
	set_font_pal(0);
	load_default_font(0,0x800);

	init_armies(i1,i2);
	disp_on();

	surrender_army_one = 1;
	surrender_army_two = 1;
}

void load_unit_type_to_vram(int vram_location, enum Unit_Type type)
{
	switch(type)
	{
		case INFANTRY:
		load_vram(vram_location, attack, 0xC00);
		break;
		case SPEARS:
		load_vram(vram_location, attack2, 0xC00);
		break;
		case CALVARY:
		case MUSKETS:
		case ENGINEERS:
		load_vram(vram_location, musketbtl, 0xC00);
		break;
		deafult:
		load_vram(vram_location, attack, 0xC00);
		break;
	}
}

// void calc_army_stats(struct Commander *commander, Army *army, char x, char y)
void calc_army_stats(char id, Army *army, char x, char y)
{
	Unit unit;
	int army_size;
	army_size = min(entities[id].army_size, 15);
	memcpy(&unit,entities[id].unit,sizeof(Unit));

	army->atk_total   = unit.atk;// * army_size;
	army->def_total   = unit.def;// * army_size;
	army->hp_total    = unit.hp  * army_size;
	army->hp_current  = army->hp_total;
	army->current_army_size = army_size;
	army->unit_type = entities[id]->unit.unit_type;
	army->atk_buff = 0;
	army->def_buff = 0;
	army->commander = &commanders[id];
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

	// p_c_army = 0;
	// c_c_army = 0;

	calc_army_stats(player,&p_c_army,4,2);
	calc_army_stats(cpu,&c_c_army,24,2);

	// calc_army_stats(&commanders[2],&p_l_army,4,4);
	// calc_army_stats(&commanders[3],&c_l_army,24,4);
	//
	// calc_army_stats(&commanders[4],&p_r_army,4,6);
	// calc_army_stats(&commanders[5],&c_r_army,24,6);

	army_one_size = p_c_army.army_start_size; //MAX_ARMY_SIZE;
	army_two_size = c_c_army.army_start_size; //MAX_ARMY_SIZE;
	// army_one_size = commanders[group_one.commanders[player_selected_index]].army_size;
	// army_two_size = commanders[group_two.commanders[cpu_selected_index]].army_size;
	army_one_current_size = army_one_size;
	army_two_current_size = army_two_size;

	// total_units = army_one_current_size + army_two_current_size;
	total_units = army_one_size + army_two_size;
	//sprite_offset = MAX_SPRITES - (total_units + 2); //2 commanders
	// sprite_offset = ARMY_ONE_START;
	// total_sprites = total_units;
	// commander_one_pos = (2 * TILES_PER_LINE) + 3;
	// commander_two_pos = (2 * TILES_PER_LINE) + 10;

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
		sp->walk = 0;
		sp->x = (get_x_from_pos(p)*20)-64;
		sp->y = (get_y_from_pos(p)*32)+64;
		sp->frame = PLAYER_SPRITE_VRAM;
	}
	sp--;
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
		sp->walk = 0;
		sp->x = (get_x_from_pos(p)*20)-64;
		sp->y = (get_y_from_pos(p)*32)+64;
		sp->frame = CPU_SPRITE_VRAM;
	}

	// grid[commander_one_pos] ^= 1;
	// grid[commander_one_pos-1] ^= 4;
	// grid[commander_one_pos] ^= 4;
	//
	// grid[commander_two_pos] ^= 2;
	// grid[commander_two_pos-1] ^= 8;
	// grid[commander_two_pos] ^= 8;

	sp = armyOne;
	populate_army(sp,0x400,PLAYER_SPRITE_VRAM,0,0,army_one_size,5);

	// load_palette(17, soldierpal, 2);
	// load_palette(21, samuraipal, 5);
	// load_palette(22, ballpal, 6);
	// load_palette(23, cmdrpal, 1);

	sp = armyTwo;
	// p = 0;
	// row = 0;
	// column = 0;
	//(0x5000 + ((ARMY_ONE_SIZE) * 0x400))sprite start for army two
	populate_army(sp,0x400,CPU_SPRITE_VRAM,17,1,army_two_size,21);

	satb_update();
	// load_commanders();
	vsync();
}

void toggle_position(int pos, int id)
{
	if((pos & 31) > 0)
	{
		grid[pos-1] ^= id;
	}
	if((pos & 31) < 31)
	{
		grid[pos+1] ^= id;
	}
	grid[pos] ^= id;
}

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
		load_formation_menu();
		return 1;

		default:
		comm_1 = selection;
		comm_2 = selection;
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

char in_battle_select(char selection)
{
	switch(selection)
	{
		case ARTS:
		load_arts_menu();
		return 1;
		// break;

		default:
		comm_1 = selection;
		comm_2 = selection;
		return 0;
	}
}

char arts_select(char selection)
{
	switch(selection)
	{
		default:
		team_one_arts_flag = 1;
		return 0;
	}
}

int populate_army(struct soldier *soldiers, int sprite_size, int sprite_start, int pal, int flip, int army_size, int spriteno)
{
	int i;
	for(i = spriteno; i<(army_size + spriteno); ++i, ++soldiers){
		// spr_make(i, ((soldiers->pos & (TILES_PER_LINE-1))*SOLDIER_SIZE), ((soldiers->pos / TILES_PER_LINE)*SOLDIER_SIZE), sprite_start, FLIP_MAS|SIZE_MAS, NO_FLIP|SZ_32x32, pal, 1);
		spr_make(i, soldiers->x, soldiers->y, sprite_start, FLIP_MAS|SIZE_MAS, NO_FLIP|SZ_32x32, pal, 1);
		spr_pri(1);
		// spr_show();
		if(flip){
			spr_ctrl(FLIP_X_MASK,FLIP_X);
		}
	}
}

void loop()
{

}

void update_sprite_pos(struct soldier *soldier)
{
	spr_y(get_y_from_pos(soldier->pos) * 32 + soldier->y + 64);
	spr_x(get_x_from_pos(soldier->pos) * 32 + soldier->x + (xOffset));
}

void cleanup_battle(int player_selected_index, int cpu_selected_index)
{
	int i;
	entities[player_selected_index].army_size -= (char)(p_c_army.army_start_size - p_c_army.current_army_size);
	entities[cpu_selected_index].army_size -= (char)(c_c_army.army_start_size - c_c_army.current_army_size);
	for(i=5; i<26; i++)
	{
		spr_hide(i);
	}
	for(i=0; i<NUMBER_OF_TILES; i++)
	{
		grid[i] = 0;
	}
	for(i=0, sp=armyOne; i<MAX_ARMY_SIZE; i++)
	{
		sp->pos = 0;
		sp->active = 0;
		sp->id = 0;
		sp->state = 0;
		sp->tic = 0;
		sp->walk = 0;
		sp->x = 0;
		sp->y = 0;
		sp->state = 0;
		sp->direction = 0;
	}

	for(i=0, sp=armyTwo; i<MAX_ARMY_SIZE; i++)
	{
		sp->pos = 0;
		sp->active = 0;
		sp->id = 0;
		sp->state = 0;
		sp->tic = 0;
		sp->walk = 0;
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
	disp_off();
	vsync();

	// grid[commander_one_pos] ^= 1;
	// grid[commander_one_pos-1] ^= 4;
	// grid[commander_one_pos] ^= 4;
	//
	// grid[commander_two_pos] ^= 2;
	// grid[commander_two_pos-1] ^= 8;
	// grid[commander_two_pos] ^= 8;
	// commanders[group_one.commanders[player_selected_index]].army_size = army_one_current_size;
	// commanders[group_two.commanders[cpu_selected_index]].army_size = army_two_current_size;

	total_sprites = 0;
	reset_satb();
	cls();
}
