#define BASE_CRIT_CHANCE 5
#define BASE_HIT_CHANCE 204
// //hp, atk, def, int, res, spd
// const unsigned char COMMANDER_STATS_DISTRIBUTION_CHART[] = {
//   5,5,5,5,5,5, //rei
//   5,5,5,5,5,5, //violet
//   5,5,5,5,5,5, //kingsly
//   5,5,5,5,5,5, //tinker
//   5,5,5,5,5,5, //Teale
//   5,5,5,5,5,5  //Calien
// };

// const unsigned char GENERIC_STATS_DISTRIBUTION_CHART[] = {
//   5,5,5,5,5,5, //normal
//   5,5,5,5,5,5, //pierece
//   5,5,5,5,5,5, //axe
//   5,5,5,5,5,5, //missile
//   5,5,5,5,5,5, //magic
//   5,5,5,5,5,5, //bare
//   5,5,5,5,5,5//none
// };

//hp, atk, def, int, res, spd
const unsigned char STAT_GROWTH_CHART[] = {
  2,3,2,1,2,3, //DPS
  2,3,2,2,2,3, //ranged dps
  2,1,2,4,2,2, //magic dps
  3,2,2,3,2,3, //magic support
  4,1,2,1,2,1, //tank
  2,2,2,3,4,2, //resist
  3,4,4,3,4,3, //rei
  3,1,2,5,3,2, //violet
  4,4,2,3,3,1, //king
  4,1,2,4,2,1, //tinker
  2,2,1,2,2,4  //speed 
};