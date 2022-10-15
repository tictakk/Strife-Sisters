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

#define ATK_ATT 0
#define DEF_ATT 1
#define SPD_ATT 2
#define AP_ATT 3
#define HP_ATT 4
#define NO_ATT 5

typedef struct{
	char type, attribute, modifier, rarity;
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

	items[0].attribute = ATK_ATT;
	items[1].attribute = DEF_ATT;
	items[2].attribute = ATK_ATT;
	items[3].attribute = HP_ATT;
	items[4].attribute = DEF_ATT;
	items[5].attribute = DEF_ATT;
	items[6].attribute = DEF_ATT;
	items[7].attribute = DEF_ATT;
	items[8].attribute = SPD_ATT;
	items[9].attribute = SPD_ATT;
	items[10].attribute = SPD_ATT;
	items[11].attribute = SPD_ATT;
	items[12].attribute = HP_ATT;
	items[13].attribute = HP_ATT;
	items[14].attribute = NO_ATT;

	items[0].modifier = 1;
	items[1].modifier = 1;
	items[2].modifier = 2;
	items[3].modifier = 5;
	items[4].modifier = 2;
	items[5].modifier = 1;
	items[6].modifier = 2;
	items[7].modifier = 1;
	items[8].modifier = 2;
	items[9].modifier = 3;
	items[10].modifier = 3;
	items[11].modifier = 3;
	items[12].modifier = 3;
	items[13].modifier = 3;
	items[14].modifier = 0;
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

void display_attribute_string(char item_no, char x, char y)
{
	switch(items[item_no].attribute)
	{
		case ATK_ATT:
		put_string("ATK+",x,y);
		put_number(items[item_no].modifier,1,x+4,y);
		break;

		case DEF_ATT:
		put_string("DEF+",x,y);
		put_number(items[item_no].modifier,1,x+4,y);
		break;

		case SPD_ATT:
		put_string("SPD+",x,y);
		put_number(items[item_no].modifier,1,x+4,y);
		break;

		case AP_ATT:
		put_string("AP+",x,y);
		put_number(items[item_no].modifier,2,x+3,y);
		break;

		case HP_ATT:
		put_string("HP+",x,y);
		put_number(items[item_no].modifier,2,x+3,y);
		break;

		default:
		put_string("    ",x,y);
		put_string("   ",x+3,y);
		break;
	}
}

void display_item_info(char real_index, int x, int y)
{
}
