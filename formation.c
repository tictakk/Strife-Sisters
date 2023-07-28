// typedef struct {
// 	Unit_Entity units[9];
//   char calling_stone;
//   char meter;
// } Battlegroup;

char *f_name_1 = "Militia";

char formation_militia[9] = {
  1,1,1,
  0,1,0,
  0,0,0
}

typedef struct {
  char *positions;
  char *formation_name;
} Formation;