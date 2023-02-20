#define CALLING_STONE_COUNT 13

#define CALLING_NONE 0
#define CALLING_HEAL 1
#define CALLING_DOUBLE_ATTACK 2
#define CALLING_DEF_BUFF 3
#define CALLING_ATK_BUFF 4
#define CALLING_MOVE_BUFF 5
#define CALLING_MOVE_DEBUFF 6
#define CALLING_SEVEN 7
#define CALLING_EIGHT 8
#define CALLING_NINE 9
#define CALLING_TEN 10
#define CALLING_ELEVEN 11
#define CALLING_TWELVE 12

//calling names
const char *calling0 = "N/A";
const char *calling1 = "Healing";
const char *calling2 = "Frenzy";
const char *calling3 = "Iron Walls";
const char *calling4 = "Brutalize";
const char *calling5 = "Quick Feet";
const char *calling6 = "Stone Feet";
//

const char *stone_name = "Happy Stone";

typedef struct{
  char effect, cost;
  char *name, *s_name;
} Calling_Stone;

Calling_Stone calling_stones[CALLING_STONE_COUNT];

init_stones()
{
  calling_stones[0].cost = 127;
  calling_stones[0].effect = EFFECT_ATK_5;
  calling_stones[0].name = calling0;
  calling_stones[0].s_name = stone_name;

  calling_stones[1].cost = 4;
  calling_stones[1].effect = EFFECT_HEAL;
  calling_stones[1].name = calling1;
  calling_stones[1].s_name = stone_name;
  
  calling_stones[2].cost = 4;
  calling_stones[2].effect = EFFECT_HEAL;
  calling_stones[2].name = calling2;
  calling_stones[2].s_name = stone_name;

  calling_stones[3].cost = 4;
  calling_stones[3].effect = EFFECT_DEF_5;
  calling_stones[3].name = calling3;
  calling_stones[3].s_name = stone_name;

  calling_stones[4].cost = 4;
  calling_stones[4].effect = EFFECT_ATK_5;
  calling_stones[4].name = calling4;
  calling_stones[4].s_name = stone_name;

  calling_stones[5].cost = 4;
  calling_stones[5].effect = EFFECT_NONE;
  calling_stones[5].name = calling5;
  calling_stones[5].s_name = stone_name;

  calling_stones[6].cost = 4;
  calling_stones[6].effect = EFFECT_NONE;
  calling_stones[6].name = calling6;
  calling_stones[6].s_name = stone_name;

  calling_stones[7].cost = 4;
  calling_stones[7].effect = EFFECT_NONE;
  calling_stones[7].name = calling0;
  calling_stones[7].s_name = stone_name;

  calling_stones[8].cost = 4;
  calling_stones[8].effect = EFFECT_NONE;
  calling_stones[8].name = calling0;
  calling_stones[8].s_name = stone_name;

  calling_stones[9].cost = 4;
  calling_stones[9].effect = EFFECT_NONE;
  calling_stones[9].name = calling0;
  calling_stones[9].s_name = stone_name;

  calling_stones[10].cost = 4;
  calling_stones[10].effect = EFFECT_NONE;
  calling_stones[10].name = calling0;
  calling_stones[10].s_name = stone_name;

  calling_stones[11].cost = 4;
  calling_stones[11].effect = EFFECT_NONE;
  calling_stones[11].name = calling0;
  calling_stones[11].s_name = stone_name;

  calling_stones[12].cost = 4;
  calling_stones[12].effect = EFFECT_NONE;
  calling_stones[12].name = calling0;
  calling_stones[12].s_name = stone_name;
}