#define BASE_CRIT_CHANCE 5
#define BASE_HIT_CHANCE 230

//hp, atk, def, int, res, spd
const unsigned char STAT_GROWTH_CHART[] = {
  2,2,1,1,1,1, //DPS
  2,2,1,2,1,2, //ranged dps
  2,1,1,3,1,1, //magic dps
  3,1,1,3,2,2, //magic support
  3,2,1,1,1,1, //tank
  2,2,1,3,3,2, //resist
  3,2,2,2,2,2, //rei
  2,1,2,3,2,2, //violet
  2,3,2,3,3,1, //king
  3,1,2,3,2,1, //tinker
  2,2,1,2,2,3  //speed 
};