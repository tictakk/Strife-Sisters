// #include <stdlib.h>
#include "malloc.c"
#include "pathing.h"

// int x, y, exit;
// struct Node neighbors;
// struct Node path;
// struct Node tmp;
// struct Location goal;


// struct Node get_path()
// {
//     int x, y, exit;
//     struct Node neighbors;
//     struct Node path;
//     struct Node tmp;
//     struct Location goal;

//     while(map && !exit){
//     neighbors = get_neighbors(map);
//         while(neighbors){
//         x = neighbors->key->x;
//         y = neighbors->key->y;
//         if(grid[x][y] != 1){
//             if(put_visited(neighbors->key,neighbors->value,visited)){
//             put_visited(neighbors->key,neighbors->value,map);
//             }
//         }
//         if(grid[x][y] == 2){
//             exit = 1;
//             goal->x = x;
//             goal->y = y;
//         }
//         neighbors = neighbors->next;
//         }
//         map = map->next;
//     }

//     path->key = goal;
//     // printf("x: %lu, y: %lu\n", goal.x,goal.y);

//     while(!cmp(goal,start)){
//         tmp = get_node(goal,visited);
//         goal = tmp->value;
//         put_visited(tmp->key,tmp->value,path);
//         // printf("x: %lu, y: %lu\n", goal.x,goal.y);
//     }
//     path = path->next;
// }

struct Location *get_neighbors(int x, int y)
{
  struct Location from;
  struct Node *tmp, *results;

  int i;
  from.x = x;
  from.y = y;
  i = 0;
  results->key.x = x;
  results->key.y = y;
  tmp = results;

  if((x - 1) > -1){
    tmp->key.x = x-1;
    tmp->key.y = y;
    tmp->value.x = from.x;
    tmp->value.y = from.y;
    i++;
  }
  if((y - 1) > -1){
    if(i!=0){
      tmp->next = malloc(sizeof(struct Node));
      tmp = tmp->next;
    }
    tmp->key.x = x;
    tmp->key.y = y-1;
    tmp->value.x = from.x;
    tmp->value.y = from.y;
    i++;
  }
  if((y + 1) < 10){
    if(i!=0){
      tmp->next = malloc(sizeof(struct Node));
      tmp = tmp->next;
    }
    tmp->key.y = y+1;
    tmp->key.x = x;
    tmp->value.x = from.x;
    tmp->value.y = from.y;
    i++;
  }
  if((x + 1) < 10){
    if(i!=0){
      tmp->next = malloc(sizeof(struct Node));
      tmp = tmp->next;
    }
    tmp = tmp->next;
    tmp->key.x = x+1;
    tmp->key.y = y;
    tmp->value.x = from.x;
    tmp->value.y = from.y;
  }
  free(tmp);
  return *results;
}

int get_neighbors(int x, int y)
{
	struct Node tmp, results;
	int i, counter;

	counter = 0;
	tmp = results;


	if((x - 1) > -1)
	{

		neighbors[counter].ownX = x-1; 
		neighbors[counter].ownY = y; 
		neighbors[counter].fromX = x; 
		neighbors[counter].fromY = y; 
		neighbors[counter].checked = 1;
		counter++;
		// if(map_counter == 0)
		// {
		// 	put_number(neighbors[0].ownX,2,1,1);
		// 	put_number(neighbors[0].checked,2,0,0);
		// }
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
	for(i=counter; i<4; i++)
	{
		neighbors[i].checked = 0;
	}
	return counter-1;
}	

get_path(int pos)
{
	int x, y, fx, fy;
	int p = 0;
	int i = 0;
	int count = 0;
	int exit = 1;
	// struct Node node;
	// put_number(pos,3,0,0);

	map[map_size].ownX = get_x_from_pos(pos);
	map[map_size].ownY = get_y_from_pos(pos);
	map[map_size].fromX = get_x_from_pos(pos);
	map[map_size].fromY = get_y_from_pos(pos);
	map[map_size].checked = 1;

	while(map[map_counter].checked == 1 && exit == 1)
	{
		//THIS HAS TO BE FIXED HERE, IT'S EXITING EARLY BEFORE FINDING WHAT WE NEED BECAUSE AN "UNCHECKED" NODE IS BEING PUT IN SOMEWHERE (LIKELY INDEX 2) LOOK AT PUT NUMBER AT THE END OF THIS FUNCTION
		count = get_neighbors(map[map_size].ownX,map[map_size].ownY);
		i = 0;
		while(i < count-1)
		{
			p = (neighbors[i].ownY * 128) + neighbors[i].ownX;
			x = neighbors[i].ownX;
			y = neighbors[i].ownY;
			fx = neighbors[i].fromX;
			fy = neighbors[i].fromY;

			if(neighbors[i].checked == 1){
				if(grid[p] != 2){
					put_visited(x,y,fx,fy);
				}else{
					put_visited(x,y,fx,fy);
					exit = 0;
					map_counter--;
					i=count;
				}
			}
			i++;
		}
		map_counter++;
	}

}

int put_visited(int x, int y, int fx, int fy)
{
	int i = 0;
	int counter = 0;
	for(i=0; i<map_size; i++){
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