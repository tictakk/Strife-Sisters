typedef struct{
  char x,y,healthbars, to_kill;
  int  atk_total, def_total, hp_total,
       hp_current, current_army_size,
       atk_buff, def_buff, army_start_size;
       // to_kill, can_attack;
  enum Unit_Type unit_type;
  // struct Commander *commander;
} Army;

Army p_c_army;
Army c_c_army;
