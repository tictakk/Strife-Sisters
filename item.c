#define R_ONE 1
#define R_TWO 2
#define R_THREE 3
#define R_FOUR 4
#define R_FIVE 5
#define R_QUEST 6
#define R_BOSS 7

#define NUM_OF_RARITIES 7
#define NUM_OF_ITEMS 15
#define MAX_DROPPABLE_ITEMS 3

typedef struct{
	char type, attribute_num, modified, rarity;
	const char *name;
	const char *description;
} Item;

Item items[NUM_OF_ITEMS];
char battle_items[MAX_DROPPABLE_ITEMS];
char items_by_level[5];
char drop_count = 0;
char num_of_items_by_level = 0;

const char DROP_TABLE[] =
{
  53, 30, 0, 0, 0, 0, 0, //level one
  80, 53, 30, 0, 0, 0, 0, //level two
  102, 80, 53, 30, 0, 0, 0, //level three
  110, 102, 80, 53, 30, 0, 0, //level four
  110, 102, 80, 53, 30, 0, 0 //level five
};

void initialize_items()
{
  items[0].rarity = R_ONE;
  items[1].rarity = R_ONE;
  items[2].rarity = R_ONE;
  items[3].rarity = R_ONE;
  items[4].rarity = R_TWO;
  items[5].rarity = R_TWO;
  items[6].rarity = R_TWO;
  items[7].rarity = R_THREE;
  items[8].rarity = R_THREE;
  items[9].rarity = R_THREE;
  items[10].rarity = R_FOUR;
  items[11].rarity = R_FOUR;
  items[12].rarity = R_FOUR;
  items[13].rarity = R_QUEST;
  items[14].rarity = R_BOSS;
}

char get_drop_by_odds(char unit_level)
{
  char offset, item_level = 0;
  unsigned char roll = 0;
  roll = range(0,255);
  offset = 7;
  offset *= unit_level;
  while(DROP_TABLE[offset++] > roll)
  {
    item_level++;
  }
  return item_level;
}

void get_items_by_rarity(char level)
{
  char i = 0;
  for(i=0; i<NUM_OF_ITEMS; i++)
  {
    if(items[i].rarity == level)
    {
      items_by_level[num_of_items_by_level++] = i;
    }
  }
}

void get_random_item_by_level(char level)
{
  unsigned char r=0, i=0;
  int odds=0;
  get_items_by_rarity(get_drop_by_odds(level));

  if(drop_count > 2){ return 0; }
  if(num_of_items_by_level == 0){ return 0; }

  odds = 255 / num_of_items_by_level;
  r = rand() & 255;
  while(r > odds)
  {
    odds+=odds;
    i++;
  }
  battle_items[drop_count++] = items_by_level[i];
  num_of_items_by_level = 0;
}

void clear_battle_items()
{
	char i=0;
	for(i=0; i<MAX_DROPPABLE_ITEMS; i++)
	{
		battle_items[i] = 0;
	}
	drop_count = 0;
	num_of_items_by_level = 0;
}
