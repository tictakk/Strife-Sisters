typedef struct{
  char x,y,healthbars;
  int  atk_total, def_total, hp_total,
       hp_current, current_army_size,
       atk_buff, def_buff, army_start_size, to_kill;
  enum Unit_Type unit_type;
  struct Commander *commander;
} Army;

// Army p_l_army;
// Army p_r_army;
Army p_c_army;
// Army p_r_reserve;
// Army p_l_reserve;

// Army c_l_army;
// Army c_r_army;
Army c_c_army;
// Army c_r_reserve;
// Army c_l_reserve;

  // char p_left_reserve_size;
  // char p_right_reserve_size;
  // char p_left_size;
  // char p_right_size;
  // char p_center_size;
  //
  // char c_left_reserve_size;
  // char c_right_reserve_size;
  // char c_left_size;
  // char c_right_size;
  // char c_center_size;

char army_one_current_size;
char army_two_current_size;

// struct Commander *p_center_army;
// struct Commander *p_left_army;
// struct Commander *p_right_army;
// struct Commander *p_l_reserve;
// struct Commander *p_r_reserve;
//
// struct Commander *c_center_army;
// struct Commander *c_left_army;
// struct Commander *c_right_army;
// struct Commander *c_l_reserve;
// struct Commander *c_r_reserve;

// void reinforce(char current_army_size, char *reserves, char team_id);
