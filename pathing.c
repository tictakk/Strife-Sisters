struct Node neighbors[8];
struct Node map[200];
// struct Node path[20];

// char path_counter = 0;

int get_neighbors(unsigned char x, unsigned char y, char size)
{
	int i, counter, width;
	width = 32;
	// width = (size)? 16 : 32;
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
	if((y + 1) < width)
	{
		neighbors[counter].ownX = x;
		neighbors[counter].ownY = y+1;
		neighbors[counter].fromX = x;
		neighbors[counter].fromY = y;
		neighbors[counter].checked = 1;
		counter++;
	}

	//right
	if((x + 1) < width)
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
	// 	for(i=counter; i<4; i++)
	// 	{
	// 		neighbors[i].checked = 0;
	// 	}
	// 	return counter;
	// }

	//up, left
	if((x - 1) >= 0 && (y - 1) >= 0)
	{
	  neighbors[counter].ownX = x-1;
	  neighbors[counter].ownY = y-1;
	  neighbors[counter].fromX = x;
	  neighbors[counter].fromY = y;
	  neighbors[counter].checked = 1;
	  counter++;
	}

	//up, right
	if((x + 1) < width && (y - 1) >= 0)
	{
	  neighbors[counter].ownX = x+1;
	  neighbors[counter].ownY = y-1;
	  neighbors[counter].fromX = x;
	  neighbors[counter].fromY = y;
	  neighbors[counter].checked = 1;
	  counter++;
	}

	//down, right
  if((x+1) < width && (y+1) < width)
  {
    neighbors[counter].ownX = x+1;
    neighbors[counter].ownY = y+1;
    neighbors[counter].fromX = x;
    neighbors[counter].fromY = y;
    neighbors[counter].checked = 1;
    counter++;
  }

	//down, left
  if((x - 1) >= 0 && (y + 1) < width)
  {
    neighbors[counter].ownX = x-1;
    neighbors[counter].ownY = y+1;
    neighbors[counter].fromX = x;
    neighbors[counter].fromY = y;
    neighbors[counter].checked = 1;
    counter++;
  }

	for(i=counter; i<9; i++)
	{
		neighbors[i].checked = 0;
	}
	return counter;
}

int get_path(int pos, int desired, int paths[20], char *big_map, char size)
{
  struct Node *node;
	struct Node path[20];

	int x, y, fx, fy, v, width;
	int p = 0;
	int i = 0;
	int count = 0;
	int exit = 1;
	int path_counter = 0;
  v = 0;

	// width = (size)? 16 : 32;
	width = 32;
  map_counter = 0;
  map_size = 0;

	map[map_size].ownX = pos & 31;//width-1;
	map[map_size].ownY = pos / 32;//width;
	map[map_size].fromX = pos & 31;//width-1;
	map[map_size].fromY = pos / 32;//width;
	map[map_size].checked = 1;
	// put_number(width,3,15,15);

	while(map[map_counter].checked == 1 && exit == 1 && map_counter < 200)
	{
		//THIS HAS TO BE FIXED HERE, IT'S EXITING EARLY BEFORE FINDING WHAT WE NEED BECAUSE AN "UNCHECKED" NODE IS BEING PUT IN SOMEWHERE (LIKELY INDEX 2) LOOK AT PUT NUMBER AT THE END OF THIS FUNCTION
		count = get_neighbors(map[map_counter].ownX,map[map_counter].ownY,size);

		i = 0;
		while(i < count)
		{
			p = (neighbors[i].ownY * 32) + neighbors[i].ownX;
			x = neighbors[i].ownX;
			y = neighbors[i].ownY;
			fx = neighbors[i].fromX;
			fy = neighbors[i].fromY;

			if(neighbors[i].checked == 1)
			{
          if(p == desired)
          {
  					put_visited(x,y,fx,fy);
            exit = 0;
            map_counter--;
            i=count;
  				}
					else if(*(big_map+p) != 0)
					// else if(overworld[p] != 0)
					{
            if(put_visited(x,y,fx,fy))
						{
							// put_number(p,3,12+(4*(v/20)),6+v%20);
							v++;
						}
  				}
			}
			i++;
		}
		map_counter++;
	}

  i = 0;
  node = &map[map_size];

	paths[i] = (node->ownY * 32) + node->ownX;
  path[i].ownX = node->ownX;
  path[i].ownY = node->ownY;
  path[i].fromX = node->fromX;
  path[i++].fromY = node->fromY;
  x = map[0].ownX;
  y = map[0].ownY;

  while(!compare(x, y, node->ownX, node->ownY))
  {
    node = &map[get(node->fromX,node->fromY)];
		paths[i] = (node->ownY * 32) + node->ownX;
    path[i].ownX = node->ownX;
    path[i].ownY = node->ownY;
    path[i].fromX = node->fromX;
    path[i++].fromY = node->fromY;
    path_counter++;
  }
	return path_counter - 1;
}

char compare(char ownX, char ownY, char fromX, char fromY)
{
  return ((ownX == fromX) && (ownY == fromY));
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
