#define PANEL_VRAM 0x2000
#define OVERWORLD 0
#define SHOP_MENU 1
#define RECRUIT_MENU 2
#define CASTLE_MENU 3
#define OVERWORLD_MENU 4
#define PARTY_MENU 5
#define SWAP_MENU 6
#define UNIT_MENU 7
#define CMDR_MENU 8
#define PARTY_OPTION_MENU 9
#define ASSIGN_MENU 10
#define ARRANGE_MENU 11
#define BUY_SELECT_MENU 12
#define BUY_CMDR_UNIT_MENU 13
#define BUY_ITEM_MENU 14
#define UNIT_SLIDER_MENU 15
#define ROW_SELECT_MENU 16
#define SECONDS 60

char menu_state = 0;
char menu_options = 0;
char menu_cols = 0;
char submenu_state = 0;
char row_selected = 0;

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
char cycler = 0;
char current = 0;
char current_selected_castle = 0;
char commander_select_cursor = 0;
char cursor_column = 0;
char last_selected_cursor = 0;
char last_selected_col = 0;
char last_selected_row = 0;

char hz = 0;
int secs = 0;

//menus
void display_recruit_window(char unit_id)
{
  display_window(0,0,22,14);
  display_window(22,14,10,14);
  put_string("Recruit",8+s_x_relative,s_y_relative+1);
  put_string("Gems",s_x_relative+1,s_y_relative+11);
  put_string("Gold",s_x_relative+1,s_y_relative+12);
  put_number(player_gold,4,s_x_relative+6,s_y_relative+12);
  put_number(blue_crystal_count,4,s_x_relative+6,s_y_relative+11);

  display_recruit_menu(unit_id);

  print_unit_row_by_type(NORMAL,2,3);
  print_unit_row_by_type(PIERCE,2,4);
  print_unit_row_by_type(AXE,2,5);
  print_unit_row_by_type(MISSILE,2,6);
  print_unit_row_by_type(MAGIC,2,7);
  print_unit_row_by_type(UNARMED,2,8);

  load_cursor(1+(cursor_column*4),3+commander_select_cursor,SLIDER_ONE);

  menu_state = SHOP_MENU;
  menu_options = TYPE_COUNT;
  selected_unit = unit_id;
}

void display_recruit_menu(char unit)
{
  display_convoy_window(22,0);
  display_unit_stats_window(unit,0,14);
  display_unit_battle_info(unit,10,14);
}

void update_recruit_menu(char unit)
{
  update_convoy_window(22,0);
  update_unit_stats_window(unit,0,14);
  update_unit_battle_info(unit,10,14);
}

void display_unit_stats_window(char unit_id, char x, char y)
{
  display_window(x,y,10,14);
  update_unit_stats_window(unit_id,x,y);
}

void update_unit_stats_window(char unit_id, char x, char y)
{
  if(unit_id)
  {
    draw_32x32_sprite(unit_id,(x*8)+16,(y*8)+18);
    print_unit_fullname(unit_id,x+1,s_y_relative+y+1);
    print_unit_stats(unit_id,x+1,s_y_relative+y+7);
  }
  else
  {
    spr_hide(1);
  }
}

void display_convoy_window(char x, char y)
{
  display_window(x,y,10,14);
  put_string("Convoy",x+2,s_y_relative+y+1);
  update_convoy_window(x,y);
}

void update_convoy_window(char x, char y)
{
  list_party_units(s_x_relative+x+2,s_y_relative+y+3);
}

void display_unit_battle_info(char unit_id, char x, char y)
{
  display_window(x,y,12,14);
  update_unit_battle_info(unit_id,x,y);
}

void update_unit_battle_info(char unit_id, char x, char y)
{
  char l_x,l_y;
  l_y = s_y_relative+y;
  l_x = s_x_relative+x;

  put_string("Btl info",l_x+2,l_y+1);

  put_string("Upgrd-",l_x+1,l_y+4);
  put_number(300,3,l_x+7,l_y+4);

  put_string("Price-",l_x+1,l_y+3);
  put_number(100,3,l_x+7,l_y+3);

  put_string("Front",l_x+1,l_y+5);
  print_attack_type(unit_id,0,l_x+3,l_y+6);

  put_string("Middle",l_x+1,l_y+7); 
  print_attack_type(unit_id,1,l_x+3,l_y+8);

  put_string("Rear",l_x+1,l_y+9);
  print_attack_type(unit_id,2,l_x+3,l_y+10);

  put_string("Advt.",l_x+1,l_y+11);
  print_unit_advantage_position(unit_id,l_x+3,l_y+12);
}

//thoeretically, this window should never "update" outside of the cursor
void display_party_commanders_window(char cmdr_id, char x, char y)
{
  menu_state = PARTY_MENU;
  menu_options = party_size;
  menu_cols = 1;

  spr_hide(0);

  display_window(x,y,10,14);

  list_commanders(s_x_relative+x+2,y+s_y_relative+2);

  display_convoy_window(22,0);
  display_window(10,0,12,14);
  display_window(0,14,22,14);
  display_window(22,14,10,14);

  load_cursor(1,2,SLIDER_ONE);
}

void display_battle_group_window(char cmdr_id, char x, char y)
{
  // menu_state = CMDR_MENU;
  menu_options = 3;
  menu_cols = 2;

  remove_cursor();

  load_vram(0x68C0,selector,0x40);
  load_palette(28,selectorpal,1);

  display_window(x,y,22,14);
  update_battle_group_window(cmdr_id,x,y);
  satb_update();
}

void update_battle_group_window(char cmdr_id, char x, char y)
{
  char i, j, l_x, l_y, pts;

  l_x = x+s_x_relative;
  l_y = y+s_y_relative;

  j = 0; pts = 0;

  hide_npcs(5);
  load_cmdr_army_to_npcs(cmdr_id,0,0);

  for(i=0; i<9; i++)
  {
    if(party_commanders[cmdr_id].bg.units[i].unit->id)
    {
      draw_npc(j+5,((i%3)*24)+20,((i/3)*24)+(8*15),j++);
      pts += party_commanders[cmdr_id].bg.units[i].unit->points;
    }
  }

  spr_make(63,(cursor_column*24)+20,(commander_select_cursor*24)+144-4,0x68C0,0,NO_FLIP|SZ_16x16,28,1);

  put_string("Points",l_x+12,l_y+1);
  put_number(pts,2,l_x+13,l_y+2);
  put_char('/',l_x+15,l_y+2);
  put_number(party_commanders[cmdr_id].max_army_pts,2,l_x+16,l_y+2);
  
  put_string("Power",l_x+12,l_y+3);
  put_number(100,3,l_x+14,l_y+4);

  put_string("Ensign",l_x+12,l_y+5);
  put_string("---",l_x+14,l_y+6);

  put_string("Art",l_x+12,l_y+7);
  put_string("---",l_x+14,l_y+8);
  put_green_square(2,17);
  put_green_square(8,17);
  put_green_square(5,20);
  put_green_square(2,23);
  put_green_square(8,23);
}

void display_convoy_select_window(char x, char y)
{
  menu_cols = 1;
  commander_select_cursor = 0;
  menu_state = UNIT_MENU;
  menu_options = 8;
  cursor_column = 0;

  load_cursor(23,3,SLIDER_ONE);
  update_convoy_select(x,y);
}

void display_party_options_window(char x, char y)
{
  remove_cursor();
  load_cursor(x+1,y+2,SLIDER_ONE);
  put_string("Assign",s_x_relative+x+2,s_y_relative+y+2);
  put_string("Organize",s_x_relative+x+2,s_y_relative+y+3);
  put_string("Art",s_x_relative+x+2,s_y_relative+y+4);
  put_string("Ensign",s_x_relative+x+2,s_y_relative+y+5);
  put_string("Cancel",s_x_relative+x+2,s_y_relative+y+6);

  menu_options = 3;
  menu_state = PARTY_OPTION_MENU;
  commander_select_cursor = 0;
}

void update_convoy_select(char x, char y)
{
  update_convoy_window(x,y);
}

void load_cmdr_army_to_npcs(char cmdr_id);