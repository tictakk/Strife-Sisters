#include <stdlib.h>
#include "huc.h"
#include "malloc.c"

/* vram allocation */
#define TILE_VRAM    0x1000
#define TILES_PER_LINE 128
#define NUMBER_OF_TILES 2000//5120
#define SOLDIER_SIZE 32
#define ARMY_ONE_SIZE 4
#define ARMY_TWO_SIZE 2

/* promotion's exported tileset */
#define TILE_WIDTH 8
#incpal(testpal,"map/bgmap.tiles.pcx");
#inctile(testtiles,"map/bgmap.tiles.pcx");
#inctilepal(testtable,"map/bgmap.tiles.pcx");

/* promotion's exported map */
#define MAP_WIDTH  128
#define MAP_HEIGHT 64
#incbin(testmap,"map/bgmap.Untitled 1.layer-Layer 1.map001.stm");

/* sprites */
#incspr(sold, "map/sprites/soldierorg.pcx", 0, 0, 2, 8);
#incspr(attack, "map/sprites/soldierorg.pcx", 0, 128, 2, 8);
#incpal(soldierpal, "map/sprites/soldier.pcx");

int sprite_memory_start = 0x5000;
int sprite_size = 0x800;
int army_one_x_start = 0;
int army_one_y_start = 0;
int army_two_x_start = 0;
int army_two_y_start = 0;
int opp = 0;
int current_army_id = 0;
char army_one_current_size = ARMY_ONE_SIZE;
char army_two_current_size = ARMY_TWO_SIZE;
char current_opp_army_size = 0;

char grid[NUMBER_OF_TILES];

struct Node{
	int ownX, ownY, fromX, fromY;
	int checked;
};

enum SoldierState{
	WAITING, MOVING, ATTACKING, DYING
};

enum Direction{
	UP, RIGHT, DOWN, LEFT
};	

struct Location{
    int x, y;
};

// struct Node{
//     struct Location key, value;
//     struct Node *next;
// };

struct soldier{
	int x, y, pos, tic, id;
	char vx, vy;
	enum SoldierState state;
	enum Direction direction;
	char walk, attack, die, active;
};

struct Node map[200];
// struct Node visited[50];
struct Node neighbors[4];
// struct Node path[15];

struct soldier *armyOne[ARMY_ONE_SIZE]; 
struct soldier *armyTwo[ARMY_TWO_SIZE];
struct soldier *opp_army;
// struct Location *locations[1];

int map_counter = 0;
int map_size = 0;
int soldierFrame = 0x5000;
int altFrame = 0x5800;
int frame = 0x5000;
int tic = 0;
int sx = 0;
int sy = 0;
int yOffset = 0;
int j1 = 0;
int xOffset = 0;
// int path_pointer = 0;

main()
{
	int soldierx = 160; //5
	int soldiery = 128; //4
	
	int pattern = 0;

	int altx = 0;
	int alty = 32;

	int i = 0;
	int z;
	// int path_pointer = 0;
	int movex = 0;
	int movey = 0;
	int attack_pos = 0;

	//init army
	struct soldier *sp;
	// struct Node *node;

	int j;

	int p;
	int row;
	int column;

	row = 2;
	column = 1;
	for(j=0, sp = armyOne; j<ARMY_ONE_SIZE; ++j, ++sp)
	{
		p = ((j + row) * 128) + column;
		sp->pos = p;
		sp->active = 1;
		sp->id = j;
		grid[p] = 1;
		grid[p+1] = 3;
		grid[p-1] = 3;
	}
	row = 5;
	column = 5;
	for(j=0, sp = armyTwo; j<ARMY_TWO_SIZE; ++j, ++sp)
	{
		p = ((j + row) * 128) + column;
		sp->pos = p;
		sp->active = 1;
		sp->id = j;
		grid[p] = 2;
	}
	p = 0;

	/* disable display */
	disp_off();

	/* disable display */
	set_screen_size(SCR_SIZE_64x64);

	/* clear display */
	cls();

	/* sprite atribute table */
	init_satb();

	/* init map */
	// set_map_data(testmap,MAP_WIDTH,MAP_HEIGHT);
	// set_tile_data(testtiles,238,testtable,TILE_WIDTH);
	// load_tile(TILE_VRAM);
	// load_map(0,0,0,0,MAP_WIDTH,MAP_HEIGHT);
	// load_palette(0,testpal,1);

	/* init sprites */
	load_palette(16, soldierpal, 1);
	// populate_armies(3,soldierx,soldiery,0x400,0x5000,2,1);
	sp = armyOne;
	populate_army(sp,0x400,0x5000,2,1,ARMY_ONE_SIZE,0);

	load_palette(17, soldierpal, 2);

	sp = armyTwo;
	//(0x5000 + ((ARMY_ONE_SIZE) * 0x400))sprite start for army two
	populate_army(sp,0x400,0x5000,1,0,ARMY_TWO_SIZE,ARMY_ONE_SIZE);
	satb_update();

	set_color_rgb(1, 7, 7, 7);
	set_font_color(1, 0);
	set_font_pal(0);
	load_default_font();

	/* enable display */
	disp_on();
	vsync();
	// get_neighbors(8,5);

	// get_path(648);

	// put_number(node->ownX,2,6,0);
	// put_number(node->ownY,2,8,0);

	for(;;)
	{
		// put_number(1,1,0,0);
		tick();
		vsync();
		controls();

		// opp_army = armyTwo;
		// opp = 2;
		// current_opp_army_size = ARMY_TWO_SIZE;
		// current_army_id = 1;
		// if(frame==3){
		// get_path(645);
		// }
		for(z=0, sp = armyOne; z<ARMY_ONE_SIZE; ++z, ++sp)
		{
			if(sp->active){
				spr_set(z);
				spr_pattern(soldierFrame);
				// soldier_state_machine(sp);
				// get_path(648);
			}
		}

		// opp_army = armyOne;
		// opp = 1;
		// current_opp_army_size = ARMY_ONE_SIZE;
		// current_army_id = 2;
		for(z=ARMY_ONE_SIZE, sp = armyTwo; z<(ARMY_ONE_SIZE + ARMY_TWO_SIZE); ++z, ++sp)
		{ //have to offset army one's size
			if(sp->active){
				spr_set(z);
				spr_pattern(soldierFrame);
				// put_number(node->ownX,2,0,0);
				// put_number(node->ownY,2,2,0);
				// put_number(sp->pos,3,0,0);
				soldier_state_machine(sp);
			}
		}

		satb_update();
	}
}

spr_make(spriteno,spritex,spritey,spritepattern,ctrl1,ctrl2,sprpal,sprpri)
int spriteno,spritex,spritey,spritepattern,ctrl1,ctrl2,sprpal,sprpri;
{
	spr_set(spriteno);
	spr_x(spritex);
	spr_y(spritey);
	spr_pattern(spritepattern);
	spr_ctrl(ctrl1,ctrl2);
	spr_pal(sprpal);
	spr_pri(sprpri);
}

int populate_army(struct soldier *soldiers, int sprite_size, int sprite_start, int pal, int flip, int army_size, int spriteno)
{
	int i;
	for(i = spriteno; i<(army_size + spriteno); ++i, ++soldiers){
		spr_make(i, ((soldiers->pos & 127)*SOLDIER_SIZE), ((soldiers->pos / 128)*SOLDIER_SIZE), (sprite_start + (i * sprite_size)), FLIP_MAS|SIZE_MAS, NO_FLIP|SZ_32x32, pal, 1);
		if(flip){
			spr_ctrl(FLIP_X_MASK,FLIP_X);
		}
		load_vram((sprite_start + (i * sprite_size)), attack, sprite_size);
	}
}

int find_nearest_opponent(int unitPos, struct soldier *soldiers)
{
	// int armySize = current_opp_army_size;//ARMY_ONE_SIZE;
	int i = 0;
	int oppPos = 0;
	int distance = 0;
	int nearestPos = 5120; //258
	int nearestOpp = 0;
	// nearestPos = soldiers->pos;

	for(i = 0; i<current_opp_army_size; ++i, ++soldiers)
	{
		if(soldiers->active && !is_opp_adjacent(soldiers->pos)){
			oppPos = soldiers->pos;
			if(oppPos > unitPos){
				distance = oppPos - unitPos;
			}else{
				distance = unitPos - oppPos;
			}
			if(distance < nearestPos){
				nearestPos = distance;
				nearestOpp = oppPos;
			}
		}
	}
	return nearestOpp;
}

int move_to(int nearestOpp, struct soldier *s)
{
	int dstY, dstX, currentY, currentX, pos, moved; 
	pos = s->pos;
	dstY = get_y_from_pos(nearestOpp);
	dstX = get_x_from_pos(nearestOpp);
	currentY = get_y_from_pos(pos);
	currentX = get_x_from_pos(pos);

	// if(current_army_id == 1 && s->id == 1)
	// {
	// 	put_number(currentX,2,0,0);
	// 	put_number(currentY,2,0,2);
	// 	put_number(dstX,2,2,0);
	// 	put_number(dstY,2,2,2);
	// }

	if((dstX) < currentX && grid[pos--] == 0)//move left
	{
		if(can_move_left(pos) == 1)
		{
			grid[pos] = 0;
			pos = s->pos--;
			grid[s->pos] = current_army_id;
			s->state = 1;//WALKING
			s->walk = SOLDIER_SIZE;//pixel DISTANCE
			s->direction = 3;//West / Left	
		}
	}
	else if((dstY < currentY) && grid[pos-128] == 0)//move up
	{
		if(can_move_up(pos) == 1)
		{
			grid[pos] = 0;
			pos = s->pos -= 128;
			grid[s->pos] = current_army_id;
			s->state = 1;
			s->walk = SOLDIER_SIZE;
			s->direction = 0;	
		}
	}
	else if(dstX > currentX && grid[pos++] == 0) //right
	{
		if(can_move_right(pos)==1)
		{
			grid[pos] = 0;
			pos = s->pos++;
			grid[s->pos] = current_army_id;
			s->state = 1;
			s->walk = SOLDIER_SIZE;
			s->direction = 1;
		}
	}
	else if((dstY > currentY) && grid[pos+128] == 0) // moved down
	{
		if(can_move_down(pos) == 1)
		{
			grid[pos] = 0;
			pos = s->pos+=128;
			grid[s->pos] = current_army_id;
			s->state = 1;
			s->walk = SOLDIER_SIZE;
			s->direction = 2;
		}
	}
}

int get_y_from_pos(int pos){
	return ((pos) / 128);
}

int get_x_from_pos(int pos){
	return ((pos) & 127);
}

void tick(){
	if(++tic > 6)
	{
		tic = 0; 
		++frame;
		soldierFrame += 0x100;
		altFrame += 0x100;
	}
	if(frame > 3){
		frame = 0;
		soldierFrame = 0x5000;
		altFrame = 0x5800;
	}
}

void controls(){
	j1 = joy(0);
	if (j1 & JOY_LEFT)
	{
		if(sx > 0){
			xOffset += 2;
			sx = (sx -= 2) & 1023;
			scroll(0, (sx >> 1), (sy >> 1), 0, 223, 0xC0);
		}
	}
	if (j1 & JOY_RIGHT)
	{
		if(sx < 1022){
			xOffset -= 2;
			sx = (sx +=2) & 1023;
			scroll(0, (sx >> 1), (sy >> 1), 0, 223, 0xC0);
		}
	}
	if (j1 & JOY_UP)
	{
		if(sy > 0)
		{
			yOffset += 2;
			sy = (sy -= 2) & 511;
			scroll(0, (sx >> 1), (sy >> 1), 0, 223, 0xC0);
		}
	}
	if (j1 & JOY_DOWN)
	{
		if(sy < 511)
		{
			yOffset -= 2;
			sy = (sy += 2) & 511;
			scroll(0, (sx >> 1), (sy >> 1), 0, 223, 0xC0);
		}
	}
}

int is_opp_adjacent(int gridpos){
	if(grid[gridpos-1] == 1) return gridpos-1;
	if(grid[gridpos+1] == 1) return gridpos+1;
	return 0;
}

/*************************************************************
 * 															 *
 *  				Soldier State Functions					 *
 * 															 *
 ************************************************************/

void soldier_state_machine(struct soldier *soldier){
	// if (soldier->state == 0) get_path(soldier->pos); soldier->state = 1;
	if(soldier->state == 0) soldier_idle_state(soldier);
	if(soldier->state == 1) soldier_move_state(soldier);
	if(soldier->state == 2) soldier_attack_state(soldier);
	if(soldier->state == 3) soldier_die_state(soldier);
}

void soldier_idle_state(struct soldier *soldier){
	int x;
	int y;
	struct Node *n;
	// n.ownX = 4;
	// n.ownY = 5;
	// n.fromX = 8;	// n.fromY = 5;
	// n.checked = 1;
	//node = malloc(sizeof(struct Node));// = {7, 5, 8, 5, 1};

	if(is_opp_adjacent(soldier->pos))
	{//then attack
		// put_number(1,1,0,0);
		soldier->state = 2;
		free(n);
	}
	else
	{
		// put_number(2,1,0,0);
		x = get_x_from_pos(soldier->pos);
		y = get_y_from_pos(soldier->pos);
		grid[soldier->pos] = 0;
		// if(opp++ <1){
		n = get_path(soldier->pos);

		// }
		
		if(x > n->ownX)
		{
			soldier->direction = 3;
		}
		else if (y > n->ownY)
		{
			soldier->direction = 0;	
		}
		else if (y < n->ownY)
		{
			soldier->direction = 2;
		}
		else if (x < n->ownX)
		{
			soldier->direction = 1;
		}
		// soldier->direction = 3;
		soldier->pos = (n->ownY * 128) + n->ownX;
		soldier->state = 1;
		soldier->walk = 32;
		grid[soldier->pos] = 2;
		free(n);
	}
}

void soldier_move_state(struct soldier *soldier){
	// put_number(3,2,5,0);
	if(soldier->walk == 0){
		soldier->state = 0;	
		spr_y((((soldier->pos) / 128) * SOLDIER_SIZE));
		spr_x((((soldier->pos) & 127) * SOLDIER_SIZE));
	}
	switch(soldier->direction)
	{
		case 0: 
			spr_y((((soldier->pos) / 128) * SOLDIER_SIZE) + soldier->walk);
			soldier->walk--;
			// (((sp->pos) / 128) * 32) + sp->walk;
			break;
		case 1: 
			spr_x((((soldier->pos) & 127) * SOLDIER_SIZE) - soldier->walk);
			soldier->walk--;
			break;
		case 2: 
			spr_y((((soldier->pos) / 128) * SOLDIER_SIZE) - soldier->walk);
			soldier->walk--;
			break;
		case 3: 
			spr_x((((soldier->pos) & 127) * SOLDIER_SIZE) + soldier->walk);		
			soldier->walk--;
		break;
	}
}

void soldier_attack_state(struct soldier *soldier){
	int r, nearest_opp;
	struct soldier* opponent;
	// spr_pattern(soldierFrame+0x400);

	if(soldier->tic++ == 24){
		r = rand() & 255;
		nearest_opp = is_opp_adjacent(soldier->pos);
		if(nearest_opp == 0){
			// put_number(r,3,0,0);
			soldier->tic = 0;
			soldier->state = 0;
			return;
		}
		// put_number(r,3,0,0);
		opponent = get_unit_from_position(nearest_opp);

		if(r > 190) {
			//opponent->active = 0;
			kill_unit(opponent);
		}
		soldier->tic = 0;
		soldier->state = 0;
		free(opponent);
	}
}

void soldier_die_state(struct soldier *soldier){

}

struct soldier* get_unit_from_position(int unit_position){
	int c;
	struct soldier *opponent;
	for(c=0, opponent = armyOne; c<ARMY_ONE_SIZE; ++c, ++opponent) //army one stuff
	{
		// put_number(opponent->pos,3,0,2);
		if(opponent->pos == unit_position){
			return opponent;
		}
	}
}

void kill_unit(struct soldier* soldier){
	soldier->active = 0;
	// put_number(soldier->pos,5,0,0);
	grid[soldier->pos] = 0;
	soldier->pos = 0;
	// current_opp_army_size -= 1;
	// spr_hide();
}

int can_move_up(int pos)
{
	if(pos > 128 && !grid[pos - 128])
	{
		return 1;
	}else{
		return 0;
	}
}

int can_move_right(int pos)
{
	if(get_x_from_pos(pos) < 128 && !grid[pos+1])
	{
		return 1;
	}else{
		return 0;
	}
}

int can_move_left(int pos)
{
	if(get_x_from_pos(pos) > 0 && !grid[pos - 1])
	{
		return 1;
	}else{
		return 0;
	}
}

int can_move_down(int pos)
{
	if(pos < 20 && !grid[pos + 128])
	{
		return 1;
	}else{
		return 0;
	}
}

/*************************************************************
 * 															 *
 *  				Soldier Move Functions                   *
 * 															 *
 ************************************************************/

int get_neighbors(int x, int y)
{
	int i, counter;

	counter = 0;
	
	if((x - 1) > -1)
	{
		neighbors[counter].ownX = x-1; 
		neighbors[counter].ownY = y; 
		neighbors[counter].fromX = x; 
		neighbors[counter].fromY = y; 
		neighbors[counter].checked = 1;
		counter++;
	}
	if((y - 1) > -1)
	{
		neighbors[counter].ownX = x; 
		neighbors[counter].ownY = y-1; 
		neighbors[counter].fromX = x; 
		neighbors[counter].fromY = y; 
		neighbors[counter].checked = 1;
		counter++;
	}
	if((y + 1) < 10)
	{
		neighbors[counter].ownX = x; 
		neighbors[counter].ownY = y+1; 
		neighbors[counter].fromX = x; 
		neighbors[counter].fromY = y; 
		neighbors[counter].checked = 1;
		counter++;
	}
	if((x + 1) < 10)
	{
		neighbors[counter].ownX = x+1; 
		neighbors[counter].ownY = y; 
		neighbors[counter].fromX = x; 
		neighbors[counter].fromY = y; 
		neighbors[counter].checked = 1;
		counter++;
	}

	for(i=counter-1; i<3; i++)
	{
		neighbors[i].checked = 0;
	}
	return counter-1;
}	

struct Node *get_path(int pos)
{
	struct Node path[100];
	struct Node *node;
	int x, y, fx, fy;
	int p = 0;
	int i = 0;
	int count = 0;
	int exit = 1;
	int path_pointer = 0;

	map[map_size].ownX = get_x_from_pos(pos);
	map[map_size].ownY = get_y_from_pos(pos);
	map[map_size].fromX = get_x_from_pos(pos);
	map[map_size].fromY = get_y_from_pos(pos);
	map[map_size].checked = 1;

	// put_number(map[map_size].ownX,2,12,0);
	// put_number(map[map_size].ownY,2,14,0);

	while(map[map_counter].checked == 1 && exit == 1)
	{	
		//THIS HAS TO BE FIXED HERE, IT'S EXITING EARLY BEFORE FINDING WHAT WE NEED BECAUSE AN "UNCHECKED" NODE IS BEING PUT IN SOMEWHERE (LIKELY INDEX 2) LOOK AT PUT NUMBER AT THE END OF THIS FUNCTION
		count = get_neighbors(map[map_counter].ownX,map[map_counter].ownY);
		i = 0;
		while(i <= count)	
		{
			p = (neighbors[i].ownY * 128) + neighbors[i].ownX;
			x = neighbors[i].ownX;
			y = neighbors[i].ownY;
			fx = neighbors[i].fromX;
			fy = neighbors[i].fromY;

			if(neighbors[i].checked == 1){
				if(grid[p] == 2 || grid[p] == 1) //if unit in this grid is current army id
				{

				}
				else if(grid[p] == 0) //if there is nothing in this grid
				{
					put_visited(x,y,fx,fy);
				}
				else
				{
					// put_number(map[0].ownX,2,8,0);
					// put_number(map[0].ownY,2,10,0);
					// put_number(fx,2,8,2);
					// put_number(fy,2,10,2);
					put_visited(x,y,fx,fy);
					exit = 0;

					i=10;
				}
			}
			i++;
		}
		map_counter++;
	}

	// put_number(map_size,2,0,0);
	// for(i=0; i<map_size+1; i++)
	// {
	// 	put_number(map[i].ownX,2,0,i);
	// 	put_number(map[i].ownY,2,2,i);		
	// 	put_number(map[i].fromX,2,4,i);
	// 	put_number(map[i].fromY,2,6,i);
	// }
	// put_number(map[map_size].ownX,2,0,0);
	// put_number(map[map_size].ownY,2,2,0);
	
	node = get(map[map_size].ownX,map[map_size].ownY);

	while(node->ownX != map[0].ownX || node->ownY != map[0].ownY){
		path[path_pointer].ownX = node->ownX;
		path[path_pointer++].ownY = node->ownY;
		node = get(node->fromX,node->fromY);
	}

	// for(i=0; i<path_pointer+1; i++)
	// {
	// 	put_number(path[i].ownX,2,0,i);
	// 	put_number(path[i].ownY,2,2,i);
	// }
	free(node);

	// for(i=0; i<path_pointer; i++)
	// {
	// }
	// put_number(node->ownY,3,0,0);
	map_counter=0;
	map_size=0;
	p = (path[path_pointer-1].ownY * 128) + path[path_pointer-1].ownX;
	// path_pointer=0;
	// put_number(path[path_pointer-1].ownX,2,12,2);
	// put_number(path[path_pointer-1].ownY,2,14,2);
	// if(is_opp_adjacent(p)){

	// }
	return &path[path_pointer-1];
}

int put_visited(int x, int y, int fx, int fy)
{
	int i = 0;
	int counter = 0;
	for(i=0; i<map_size+1; i++){
		if(map[i].ownX == x && map[i].ownY == y)
		{
			return 0;
		}
	}

	++map_size;
	map[map_size].ownX = x;
	map[map_size].ownY = y;
	map[map_size].fromX = fx;
	map[map_size].fromY = fy;
	map[map_size].checked = 1;
	return 1;
}

struct Node *get(int x, int y)
{
	int i = 0;
	//struct Node *node;// = malloc(sizeof(struct Node));
	for(i=0; i<map_size+1; i++){
		if(map[i].ownX == x && map[i].ownY == y)
		{
			return &map[i];//node;
		}
	}
	return -1;
}