#include "battlefield.h"
struct Node neighbors[4];
struct Node map[100];
// struct Node path[20];

int get_neighbors(unsigned char x, unsigned char y, char size)
{
	int i, counter, width;
	// width = 32;
	width = 16;//(size)? 16 : 32;
	counter = 0;
	//left
	if((x - 1) > -1)
	{
		neighbors[counter].ownX = x-1;
		neighbors[counter].ownY = y;
		neighbors[counter].fromX = x;
		neighbors[counter].fromY = y;
		neighbors[counter].checked = 1;
		counter++;
	}

	//up
	if((y - 1) > -1)
	{
		neighbors[counter].ownX = x;
		neighbors[counter].ownY = y-1;
		neighbors[counter].fromX = x;
		neighbors[counter].fromY = y;
		neighbors[counter].checked = 1;
		counter++;
	}
	//down
	if((y + 1) < 29) //BREAKS OVERWORLD
	{
		neighbors[counter].ownX = x;
		neighbors[counter].ownY = y+1;
		neighbors[counter].fromX = x;
		neighbors[counter].fromY = y;
		neighbors[counter].checked = 1;
		counter++;
	}

	//right
	if((x + 1) < 16)
	{
		neighbors[counter].ownX = x+1;
		neighbors[counter].ownY = y;
		neighbors[counter].fromX = x;
		neighbors[counter].fromY = y;
		neighbors[counter].checked = 1;
		counter++;
	}

	// if(size)
	// {
		for(i=counter; i<4; i++)
		{
			neighbors[i].checked = 0;
		}
		return counter;
	// }

	//up, left
	// if((x - 1) >= 0 && (y - 1) >= 0)
	// {
	//   neighbors[counter].ownX = x-1;
	//   neighbors[counter].ownY = y-1;
	//   neighbors[counter].fromX = x;
	//   neighbors[counter].fromY = y;
	//   neighbors[counter].checked = 1;
	//   counter++;
	// }
	//
	// //up, right
	// if((x + 1) < width && (y - 1) >= 0)
	// {
	//   neighbors[counter].ownX = x+1;
	//   neighbors[counter].ownY = y-1;
	//   neighbors[counter].fromX = x;
	//   neighbors[counter].fromY = y;
	//   neighbors[counter].checked = 1;
	//   counter++;
	// }
	//
	// //down, right
  // if((x+1) < width && (y+1) < width)
  // {
  //   neighbors[counter].ownX = x+1;
  //   neighbors[counter].ownY = y+1;
  //   neighbors[counter].fromX = x;
  //   neighbors[counter].fromY = y;
  //   neighbors[counter].checked = 1;
  //   counter++;
  // }
	//
	// //down, left
  // if((x - 1) >= 0 && (y + 1) < width)
  // {
  //   neighbors[counter].ownX = x-1;
  //   neighbors[counter].ownY = y+1;
  //   neighbors[counter].fromX = x;
  //   neighbors[counter].fromY = y;
  //   neighbors[counter].checked = 1;
  //   counter++;
  // }
	//
	// for(i=counter; i<9; i++)
	// {
	// 	neighbors[i].checked = 0;
	// }
	// return counter;
}

int get_path(int pos, int desired, int paths[20], char *big_map, char size, int depth, char ignore)
{
  struct Node *node;

	int x, y, fx, fy, v, width;
	int p = 0;
	int i = 0;
	int count = 0;
	int exit = 1;
	int path_counter = 0;
	int d_count = 0;
	int d_level = 0;
	int next_d = 0;
	char found = 0;
	char id = 0;
  v = 0;
	// width = (size)? 16 : 32;
	width = 16;
  map_counter = 0;
  map_size = 0;

	map[map_size].ownX = pos & 15;
	map[map_size].ownY = pos / 16;
	map[map_size].fromX = pos & 15;
	map[map_size].fromY = pos / 16;
	map[map_size].checked = 1;

	while(exit == 1 && d_level < depth)
	// while(map[map_counter].checked == 1 && exit == 1 && d_level < depth)
	{
		//THIS HAS TO BE FIXED HERE, IT'S EXITING EARLY BEFORE FINDING WHAT WE NEED BECAUSE AN "UNCHECKED" NODE IS BEING PUT IN SOMEWHERE (LIKELY INDEX 2) LOOK AT PUT NUMBER AT THE END OF THIS FUNCTION
		count = get_neighbors(map[map_counter].ownX,map[map_counter].ownY,size);
		i = 0;
		if(map[map_counter].checked)
		{ //this could probably go above count = get_ne... to make it more effecient
			while(i < count)
			{
				p = (neighbors[i].ownY * width) + neighbors[i].ownX;
				x = neighbors[i].ownX;
				y = neighbors[i].ownY;
				fx = neighbors[i].fromX;
				fy = neighbors[i].fromY;

				if(is_traversable(p) || ignore)
				// if(neighbors[i].checked == 1 && is_traversable(p))
				{
					id = *(big_map+p);
	          if(p == desired)
	          {
	  					put_visited(x,y,fx,fy,1);
	            exit = 0;
	            map_counter--;
	            i=count;
							found = 1;
	  				}
						else if(is_zero(id,size))//THIS BREAKS PATHING FOR OVERWORLD
						{
							if(entities[id-1].team != size && id != 0)
							{
								if(put_visited(x,y,fx,fy,0))
								{
									d_count++;
								}
							}
							else
							{
								if(put_visited(x,y,fx,fy,1))
								{
									d_count++;
								}
							}
	  				}
				}
				i++;
			}
		}
		if(map_counter == next_d)
		{
			next_d = d_count;
			d_level++;
		}
		map_counter++;
	}

	if(!found)
	{
		return -1;
	}
  i = 0;
  node = &map[map_size];

	paths[i++] = ((node->ownY * width) + node->ownX);
  x = map[0].ownX;
  y = map[0].ownY;

  while(!compare(x, y, node->ownX, node->ownY))
  {
    node = &map[get(node->fromX,node->fromY)];
		paths[i++] = ((node->ownY * width) + node->ownX);
    path_counter++;
  }

	return path_counter-1;
}

char is_zero(char num, char size)
{
	if(size)
	{
		// if(num == 0)
		// {
		// 	return 1;
		// }
		// else if(size == entities[num-1].team || size == entities[num-1].team)
		// {
		// 	return 1;
		// }
		return 1;
	}
	else
	{
		// return 0;
		return num != 0;
	}
}

char compare(char ownX, char ownY, char fromX, char fromY)
{
  return ((ownX == fromX) && (ownY == fromY));
}

int put_visited(int x, int y, int fx, int fy, char checked)
{
	int i = 0;
	for(i=0; i<map_size+1; i++)
	{
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
	map[map_size].checked = checked;
	return 1;
}

unsigned char get(char x, char y)
{
  unsigned char i = 0;
  for(i=0; i<map_size; i++){
    if(map[i].ownX == x && map[i].ownY == y)
    {
      return i;
      //return 0;
    }
  }
  return 0;
}
