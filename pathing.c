#include "battlefield.h"
struct Node neighbors[4];
struct Node map[65]; //a unit with a max move of 5 will fill 60 nodes max.

char visit_grid[352];

// int get_neighbors(unsigned char x, unsigned char y, char size)
int get_neighbors(int position)
{
	int i, counter;
	counter = 0;
	//left
	if(((position&0xF) - 1) > -1)
	{
		neighbors[counter].ownPos = position-1;
		neighbors[counter].fromPos = position;
		neighbors[counter].checked = 1;
		counter++;
	}
	//up
	if((position - 16) > -1)
	{
		neighbors[counter].ownPos = position-16;
		neighbors[counter].fromPos = position;
		neighbors[counter].checked = 1;
		counter++;
	}
	//down
	// if((position + 16) < 464)
	if((position + 16) < 352)
	{
		neighbors[counter].ownPos = position+16;
		neighbors[counter].fromPos = position;
		neighbors[counter].checked = 1;
		counter++;
	}

	//right
	if(((position&15) + 1) < 16)
	{
		neighbors[counter].ownPos = position + 1;
		neighbors[counter].fromPos = position;
		neighbors[counter].checked = 1;
		counter++;
	}

	for(i=counter; i<4; i++)
	{
		neighbors[i].checked = 0;
	}
	return counter;
}

int get_path(int pos, int desired, int paths[20], char *big_map, char team, int depth, char ignore_depth)
{
  struct Node *node;

	int p = 0;
	int fp = 0;
	int i = 0;
	int count = 0;
	int exit = 1;
	int path_counter = 0;
	int d_count = 0;
	int d_level = 0;
	int next_d = 0;
	char found = 0;
	char id = 0;

	map_counter = 0;
	map_size = 0;

	map[map_size].ownPos = pos;
	map[map_size].fromPos = pos;
	map[map_size].checked = 1;

	while(exit == 1 && d_level < depth && map_size >= map_counter)
	{
		if(map[map_counter].checked)
		{
			count = get_neighbors(map[map_counter].ownPos);

			i = 0;
			while(i < count)
			{
				p = neighbors[i].ownPos;
				fp = neighbors[i].fromPos;

				if(is_traversable(p) || ignore_depth <= d_level)
				{
					id = *(big_map+p);
		      if(p == desired)
		      {
						put_visited(p,fp,1);
		        exit = 0;
		        map_counter--;
		        i=count;
						found = 1;
		  		}
					else if(entities[id-1].team != team && id != 0 && !(ignore_depth<=d_level))
					{
            // put_number(p,4,0,0);
            // wait_for_I_input();
						if(put_visited(p,fp,0))
						{
							d_count++;
						}
					}
					else
					{
						if(entities[id-1].team == team && (d_level+1) == ignore_depth)
						{
							if(put_visited(p,fp,0))
							{
								d_count++;
							}
						}
						else
						{
							if(put_visited(p,fp,1))
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

  reset_grid();
	if(!found)
	{
		return -1;
	}
  i = 0;
  node = &map[map_size];
	paths[i++] = node->ownPos;

	while(!(node->ownPos == map[0].ownPos && node->fromPos == map[0].fromPos))
  {
		node = &map[get(node->fromPos)];
		paths[i++] = node->ownPos;
    path_counter++;
  }
	return path_counter-1;
}

void reset_grid()
{
  int i;
//   for(i=0; i<464; i++)
	for(i=0; i<352; i++)
  {
    visit_grid[i] = 0;
  }
}

char is_zero(char num, char size)
{
	if(size)
	{
		return 1;
	}
	else
	{
		return num != 0;
	}
}

// int put_visited(int x, int y, int fx, int fy, char checked)
int put_visited(int pos, int fpos, char checked)
{
  if(visit_grid[pos])
  {
    return 0;
  }

  visit_grid[pos] = 1;

	++map_size;
	map[map_size].ownPos = pos;
	map[map_size].fromPos = fpos;
	map[map_size].checked = checked;
	return 1;
}

unsigned char get(int pos)
{
  unsigned char i = 0;
  for(i=0; i<map_size; i++)
	{
	if(map[i].ownPos == pos)
    {
      return i;
    }
  }
  return 0;
}