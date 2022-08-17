#define TOTAL_COMMANDERS 25
#define ATK_ATTRIBUTE 0
#define DEF_ATTRIBUTE 1
#define SPD_ATTRIBUTE 2
#define HP_ATTRIBUTE 3

const int STAT_SCALER_TABLE[TOTAL_COMMANDERS * 4] =
{
//atk, def, spd, hp
  5, 5, 4, 3, //REI
  3, 5, 4, 5, //VIOLET
  4, 4, 4, 4, //king
  0, 0, 0, 0
};
