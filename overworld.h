//ARMY MENU WINDOW LOCATIONS
#define ARMY_CONVOY_WINDOW_X 22
#define ARMY_CONVOY_WINDOW_Y 0
#define ARMY_BATTLE_GROUP_WINDOW_X 0
#define ARMY_BATTLE_GROUP_WINDOW_Y 14
#define ARMY_STATS_X 13
#define ARMY_STATS_Y 14
#define ARMY_GROUP_UNITS_WINDOW_X 22
#define ARMY_GROUP_UNITS_WINDOW_Y 14

//ARMY MENU VARS ... "options" in this instance actually means "rows"
#define ARMY_BATTLE_GROUP_COLS 2 //cols is always one less than the actual amount of cols

//HIRE MENU WINDOW LOCATIONS
#define HIRE_STATS_WINDOW_X 0
#define HIRE_STATS_WINDOW_Y 14
#define HIRE_CONVOY_WINDOW_X 22
#define HIRE_CONVOY_WINDOW_Y 0
#define HIRE_UNIT_INFO_WINDOW_X 10
#define HIRE_UNIT_INFO_WINDOW_Y 14

#define OVERWORLD 0
#define SHOP_MENU 1
#define RECRUIT_MENU 2
#define CASTLE_MENU 3
#define OVERWORLD_MENU 4
#define PARTY_MENU 5
#define SWAP_MENU 6
#define DEPLOY_SELECT_MENU 7
#define RETURN_SELECT_MENU 8
#define PARTY_OPTION_MENU 9
#define DEPLOY_SWAP_MENU 10
#define ORGANIZE_SELECT_MENU 11
#define BUY_SELECT_MENU 12
#define BUY_CMDR_UNIT_MENU 13
#define BUY_ITEM_MENU 14
#define UNIT_SLIDER_MENU 15
#define ROW_SELECT_MENU 16
#define DISMISS_SELECT_MENU 17
#define SECONDS 60

#define CONVOY_OPTIONS 8
#define CONVOY_COLS 1

char menu_state = 0;
char menu_cols = 0;
char submenu_state = 0;

//party
enum Direction party_direction;
char party_moving = 0;
int party_pos_x = 0;
int party_pos_y = 0;
int party_pos_last = 0;
char party_walk = 0;
int party_last_pos = 0;

char swap_unit = 0;
char swap_palette = 0;
char selected_item = 0;
char selected_unit = 0;
char selected_cmdr = 0;
char current_selected_castle = 0;
char commander_select_cursor = 0;
char cursor_column = 0;
char last_selected_cursor = 0;
char last_selected_col = 0;
char last_selected_row = 0;

//menus
void display_hire_window(char unit_id)
{
  display_window_rel(0,0,32,14);
  display_window_rel(22,14,10,14);
  put_string("Hire",14+s_x_relative,s_y_relative+1);
  put_string("Ore",s_x_relative+1,s_y_relative+11);
  put_string("Gold",s_x_relative+1,s_y_relative+12);
  put_number(player_gold,4,s_x_relative+11,s_y_relative+12);
  put_number(materials_count,4,s_x_relative+11,s_y_relative+11);

  display_hire_menu(unit_id);

  set_font_pal(9);
  display_unit_types_row(2+s_x_relative,3+s_y_relative);
  set_font_pal(10);

  print_unit_row_by_type(NORMAL,2,4);
  print_unit_row_by_type(PIERCE,6,4);
  print_unit_row_by_type(AXE,10,4);
  print_unit_row_by_type(MISSILE,14,4);
  print_unit_row_by_type(MAGIC,18,4);
  print_unit_row_by_type(UNARMED,22,4);
  print_unit_row_by_type(NONE,26,4);

  load_cursor(1+(cursor_column*4),4+commander_select_cursor,SLIDER_ONE);

  menu_state = SHOP_MENU;
  menu_rows = TYPE_COUNT;
  selected_unit = unit_id;
}

void display_hire_menu(char unit)
{
  display_convoy_window(HIRE_CONVOY_WINDOW_X,HIRE_CONVOY_WINDOW_Y+14);
  display_unit_stats_window(unit,HIRE_STATS_WINDOW_X,HIRE_STATS_WINDOW_Y);
  display_unit_battle_info(unit,HIRE_UNIT_INFO_WINDOW_X,HIRE_UNIT_INFO_WINDOW_Y);
}

void update_hire_menu(char unit)
{
  display_convoy_window(HIRE_CONVOY_WINDOW_X,HIRE_CONVOY_WINDOW_Y+14);
  update_unit_stats_window(unit,0,14,1);
  update_unit_battle_info(unit,10,14);
  put_number(player_gold,4,s_x_relative+11,s_y_relative+12);
  put_number(materials_count,4,s_x_relative+11,s_y_relative+11);
}

void display_unit_stats_window(char unit_id, char x, char y)
{
  display_window_rel(x,y,10,14);
  update_unit_stats_window(unit_id,x,y,1);
}

void update_unit_stats_window(char unit_id, char x, char y, char level)
{
  if(unit_id > 0)
  {
    draw_32x32_sprite(unit_id,(x*8)+28,(y*8)+10);
  }
  else
  {
    spr_hide(1);
  }
  print_unit_stats(unit_id,x+1,s_y_relative+y+6,level);
  // print_unit_fullname(unit_id,x+3,s_y_relative+y+1);
}

void display_convoy_window(char x, char y)
{
  display_window_rel(x,y,10,14);
  put_string("Convoy",x+2,s_y_relative+y+1);
  update_convoy_window(x,y);
}

void update_convoy_window(char x, char y)
{
  list_party_units(s_x_relative+x+2,s_y_relative+y+3);
}

void display_unit_battle_info(char unit_id, char x, char y)
{
  display_window_rel(x,y,12,14);
  update_unit_battle_info(unit_id,x,y);
}

void update_unit_battle_info(char unit_id, char x, char y)
{
  char l_x,l_y;
  l_y = s_y_relative+y;
  l_x = s_x_relative+x;

  // put_string("Price",l_x+1,l_y+1);
  // put_string("Upgrade",l_x+1,l_y+3);
  set_font_pal(GOLD_FONT);
  put_string("Targets",l_x+1,l_y+3);
  put_string("Advantage",l_x+1,l_y+8);
  put_char('*',l_x+1,l_y+9);
  put_char('+',l_x+1,l_y+10);
  put_char('}',l_x+1,l_y+11);
  put_char('~',l_x+1,l_y+12);

  // set_font_pal(GOLD_FONT);  
  put_char('F',l_x+1,l_y+4);
  put_char('M',l_x+1,l_y+5);
  put_char('R',l_x+1,l_y+6);
  set_font_pal(WHITE_FONT);

  if(unit_id > 0)
  {
    load_unit_header(unit_id,0);
    // get_plus_adv(unit_header[0].a_type);
    // get_plus_plus_adv(unit_header[0].a_type);
    get_advantages(unit_header[0].a_type);

    print_unit_fullname(unit_id,l_x+1,l_y+1);
    print_attack_type(unit_header[0].attacks[0],1,l_x+2,l_y+4);
    print_attack_type(unit_header[0].attacks[1],1,l_x+2,l_y+5);
    print_attack_type(unit_header[0].attacks[2],1,l_x+2,l_y+6);
    print_unit_attack_icon(adv_plus_plus,l_x+3,l_y+9);
    print_unit_attack_icon(adv_plus[0],l_x+3,l_y+10);
    print_unit_attack_icon(adv_plus[1],l_x+4,l_y+10);
    print_unit_attack_icon(disadv_minus[0],l_x+3,l_y+11);
    print_unit_attack_icon(disadv_minus[1],l_x+4,l_y+11);
    print_unit_attack_icon(disadv_minus_minus,l_x+3,l_y+12);
  }
  else
  {
    put_string("         ",l_x+1,l_y+1);
    put_string("         ",l_x+1,l_y+3);
    put_string("         ",l_x+1,l_y+4);
    put_string("         ",l_x+1,l_y+5);
    put_string("         ",l_x+1,l_y+6);

    put_string("         ",l_x+1,l_y+8);
    put_string("         ",l_x+1,l_y+9);
    put_string("         ",l_x+1,l_y+10);
    put_string("         ",l_x+1,l_y+11);
    put_string("         ",l_x+1,l_y+12);
  }
}

//thoeretically, this window should never "update" outside of the cursor
void display_party_commanders_window(char x, char y)
{
  set_party_menu_state();

  spr_hide(0);
  display_window_rel(x,y,10,14);
  list_commanders(s_x_relative+x+2,y+s_y_relative+2);

  display_convoy_window(22,0);
  display_window_rel(10,0,12,14);
  display_window_rel(0,14,22,14);
  display_commander_window(selected_cmdr,ARMY_GROUP_UNITS_WINDOW_X,ARMY_GROUP_UNITS_WINDOW_Y);

  load_cursor(1,commander_select_cursor + 2,SLIDER_ONE);
}

void display_battle_group_window(char x, char y)
{
  set_menu_state(menu_state,3,2);
  remove_cursor();

  load_vram(0x68C0,selector,0x40);
  load_palette(28,selectorpal,1);
  
  update_unit_stats_window(party_commanders[selected_cmdr].bg.units[selected_unit].id,x+13,y,party_commanders[selected_cmdr].bg.units[selected_unit].level);
  update_battle_group_window(selected_cmdr,x,y);

  display_selector(63,get_iso_x(1,17,commander_select_cursor,cursor_column),get_iso_y(1,17,commander_select_cursor,cursor_column)+14,28);
  satb_update();
}

void update_battle_group_window(char x, char y)
{
  char i, j, l_x, l_y, pts;

  l_x = x+s_x_relative;
  l_y = y+s_y_relative;

  j = 0; pts = 0;

  hide_npcs(5);
  load_cmdr_army_to_npcs(selected_cmdr,0,0);
  for(i=0; i<9; i++)
  {
    if(party_commanders[selected_cmdr].bg.units[i].id)
    {
      load_unit_header(party_commanders[selected_cmdr].bg.units[i].id,0);
      // draw_npc(j+5,((i/3)*32)+16,((i%3)*32)+130,j++);
      draw_iso_npc(j,1,17,i/3,i%3,j++);
      pts += unit_header[0].points;
    }
  }
  
  put_green_square(1,17);
  // read_tile_data(1,17);
  put_string("Front",s_x_relative+1,s_y_relative+25);
  put_string("Rear",s_x_relative+9,s_y_relative+15);

  update_party_commander_window(ARMY_GROUP_UNITS_WINDOW_X,ARMY_GROUP_UNITS_WINDOW_Y);
}

void draw_iso_npc(char sprite_no, int iso_x, int iso_y, int sprite_x, int sprite_y, char spr_index)
{
  int x_start, y_start, x, y; 
  
  y_start = (iso_y * 8)-6;
  x_start = (iso_x * 16);

  x = (sprite_x * 16) + (sprite_y * 16); //this works because it's starting from bottom left, ending top right.
  y = (sprite_y * 8) - (sprite_x * 8); //won't work same if going another direction.
  draw_npc(sprite_no+5,x_start+x,y_start+y,spr_index);
}

void display_return_select_menu()
{
  remove_cursor();
  reset_cursor();
  set_return_select_state();
  
  display_battle_group_window(ARMY_BATTLE_GROUP_WINDOW_X,ARMY_BATTLE_GROUP_WINDOW_Y);
}

void display_party_options_window(char x, char y)
{
  set_party_options_state();
  remove_cursor();
  load_cursor(x+1,y+2,SLIDER_ONE);
  put_string("Deploy",s_x_relative+x+2,s_y_relative+y+2);
  put_string("Organize",s_x_relative+x+2,s_y_relative+y+3);
  put_string("Return",s_x_relative+x+2,s_y_relative+y+4);
  put_string("Dismiss",s_x_relative+x+2,s_y_relative+y+5);

  commander_select_cursor = 0;
}

void display_dismiss_select_menu()
{
  set_dismiss_select_state();
  remove_cursor();
  reset_cursor();
  load_cursor(23,3,SLIDER_ONE);

  update_convoy_window(ARMY_CONVOY_WINDOW_X,ARMY_CONVOY_WINDOW_Y);
  update_unit_stats_window(party_units[0],13,14,1);
}

void display_deploy_select_menu()
{
  remove_cursor();
  reset_cursor();
  set_deploy_select_state();
  load_cursor(23,3,SLIDER_ONE);
  update_convoy_window(ARMY_CONVOY_WINDOW_X,ARMY_CONVOY_WINDOW_Y);
  update_unit_stats_window(party_units[0],ARMY_STATS_X,ARMY_STATS_Y,1);
  update_battle_group_window(ARMY_BATTLE_GROUP_WINDOW_X,ARMY_BATTLE_GROUP_WINDOW_Y); 
}

void display_oraganize_select_menu()
{
  menu_state = ORGANIZE_SELECT_MENU;
  reset_cursor();
  display_battle_group_window(ARMY_BATTLE_GROUP_WINDOW_X,ARMY_BATTLE_GROUP_WINDOW_Y);
}

void set_dismiss_select_state()
{
  set_menu_state(DISMISS_SELECT_MENU,CONVOY_OPTIONS,CONVOY_COLS);
}

void set_deploy_select_state()
{
  set_menu_state(DEPLOY_SELECT_MENU,CONVOY_OPTIONS,CONVOY_COLS);
}

void set_return_select_state()
{
  set_menu_state(RETURN_SELECT_MENU,CONVOY_OPTIONS,CONVOY_COLS);
}

void set_party_options_state()
{
  set_menu_state(PARTY_OPTION_MENU,4,0);
}

void set_party_menu_state()
{
  set_menu_state(PARTY_MENU,party_size,1);
}

void set_menu_state(char state, char options, char columns)
{
  menu_state = state;
  menu_rows = options;
  menu_cols = columns;
}

void display_commander_window(char cmdr_id, char x, char y)
{
  display_window_rel(x,y,10,14);
  // load_portrait(party_commanders[cmdr_id].sprite_type,0);
  // display_item(0,0,s_x_relative+x+1,s_y_relative+y+2);
  update_party_commander_window(x,y);
}

void update_party_commander_window(char x, char y)
{
  char x_rel, y_rel;
  x_rel = s_x_relative + x;
  y_rel = s_y_relative + y;

  put_string("F",x_rel+1,y_rel+1);
  print_unit_fullname(party_commanders[selected_cmdr].bg.units[0].id,x_rel+2,y_rel+2);
  print_unit_fullname(party_commanders[selected_cmdr].bg.units[1].id,x_rel+2,y_rel+3);
  print_unit_fullname(party_commanders[selected_cmdr].bg.units[2].id,x_rel+2,y_rel+4);

  put_string("M",x_rel+1,y_rel+5);
  print_unit_fullname(party_commanders[selected_cmdr].bg.units[3].id,x_rel+2,y_rel+6);
  print_unit_fullname(party_commanders[selected_cmdr].bg.units[4].id,x_rel+2,y_rel+7);
  print_unit_fullname(party_commanders[selected_cmdr].bg.units[5].id,x_rel+2,y_rel+8);

  put_string("R",x_rel+1,y_rel+9);
  print_unit_fullname(party_commanders[selected_cmdr].bg.units[6].id,x_rel+2,y_rel+10);
  print_unit_fullname(party_commanders[selected_cmdr].bg.units[7].id,x_rel+2,y_rel+11);
  print_unit_fullname(party_commanders[selected_cmdr].bg.units[8].id,x_rel+2,y_rel+12);

  // put_string("Pow",x_rel+2,y_rel+10);
  // put_number(99,3,x_rel+6,y_rel+10);

  // put_string("BP",x_rel+2,y_rel+11);
  // put_number(get_commander_battle_points(selected_cmdr),3,x_rel+4,y_rel+11);
  // put_char('/',x_rel+7,y_rel+11);
  // put_number(party_commanders[selected_cmdr].max_bp,2,x_rel+8,y_rel+11);
}

void reset_cursor()
{
  commander_select_cursor = 0;
  cursor_column = 0;
}

void load_cmdr_army_to_npcs(char cmdr_id);