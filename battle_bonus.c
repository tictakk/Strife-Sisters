#define BONUS_NONE 0
#define BONUS_SWORD 1
#define BONUS_POLE 2
#define BONUS_AXE 4
#define BONUS_BOW 8
#define BONUS_MAGIC 16
#define BONUS_BARE 32
#define BONUS_BEAST 64

//BARE BONUS IS BARE UNITS TAKE A PORTION OF DAMAGE 

char check_battle_bonus(char bonus_mask, char bonus_type)
{
  return (bonus_mask&bonus_type)? 1 : 0;
}