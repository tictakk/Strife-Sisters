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
#define ARMY_MENU 5
#define SWAP_MENU 6
#define DEPLOY_SELECT_MENU 7
#define RETURN_SELECT_MENU 8
#define PARTY_OPTION_MENU 9
#define DEPLOY_SWAP_MENU 10
#define ORGANIZE_SELECT_MENU 11
#define BUY_SELECT_MENU 12
#define FORMATION_MENU 13
#define TRAIN_MENU 14
#define TRAIN_OPTIONS_MENU 15
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
char selected_unit = 0;
char selected_cmdr = 0;
char selected_formation = 0;
char commander_select_cursor = 0;
char cursor_column = 0;
char last_selected_cursor = 0;
char last_selected_col = 0;

char primary_menu = 0;

//menus
void display_hire_window(char unit_id)
{
  display_window_rel(0,0,32,14);
  display_window_rel(22,14,10,14);
  put_string("Hire",14+s_x_relative,s_y_relative+1);
  put_string("Cost",s_x_relative+2,s_y_relative+12);
  update_gold_amount(2,9,6,9);

  display_hire_menu(unit_id);
  update_hire_menu(unlocked_units[commander_select_cursor]);
  
  set_font_pal(9);
  display_unit_types_row(2+s_x_relative,3+s_y_relative);
  set_font_pal(10);

  print_unit_row_by_type(NORMAL,2,4);
  print_unit_row_by_type(PIERCE,6,4);
  print_unit_row_by_type(AXE,10,4);
  print_unit_row_by_type(MISSILE,14,4);
  print_unit_row_by_type(MAGIC,18,4);
  print_unit_row_by_type(UNARMED,22,4);

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
  set_font_pal(GOLD_FONT);
  put_number(get_unit_cost(unit),3,s_x_relative+6,s_y_relative+12);
  set_font_pal(WHITE_FONT);
  // put_number(materials_count,4,s_x_relative+11,s_y_relative+11);
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
    draw_32x32_sprite(unit_id,(x*8)+16,(y*8)+10);
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
  set_menu_state(ARMY_MENU,party_size,1);
  primary_menu = ARMY_MENU;
  spr_hide(0);
  display_window_rel(x,y,10,14);
  list_commanders(s_x_relative+x+2,y+s_y_relative+2);

  display_convoy_window(22,0);
  display_window_rel(10,0,12,14);
  display_window_rel(0,14,22,14);
  // display_commander_window(selected_cmdr,ARMY_GROUP_UNITS_WINDOW_X,ARMY_GROUP_UNITS_WINDOW_Y);
  display_window_rel(ARMY_GROUP_UNITS_WINDOW_X,ARMY_GROUP_UNITS_WINDOW_Y,10,14);

  load_cursor(1,commander_select_cursor + 2,SLIDER_ONE);
  put_green_square(1,17);
  // load_palette(14,square_pal,2);
  draw_formation(selected_formation);
}

void display_battle_group_window(char x, char y)
{
  draw_formation(selected_formation);
  set_menu_state(menu_state,3,2);
  remove_cursor();

  load_selector(0x68C0,31);
  
  update_unit_stats_window(party_commanders[selected_cmdr].bg.units[selected_unit].id,ARMY_STATS_X,ARMY_STATS_Y,party_commanders[selected_cmdr].bg.units[selected_unit].level);
  refresh_battle_units();

  put_string("Front",s_x_relative+1,s_y_relative+25);
  put_string("Rear",s_x_relative+9,s_y_relative+15);

  display_selector(63,get_iso_x(1,17,commander_select_cursor,cursor_column),get_iso_y(1,17,commander_select_cursor,cursor_column)+14,31);

  // satb_update();
}

void update_battle_group_window(char cmdr, char x, char y, char offset, char endset)
{
  char i;

  for(i=offset; i<(npc_count-endset); i++)
  {
    // put_string("   ",s_x_relative,s_y_relative);
    // put_number(i,3,s_x_relative,s_y_relative);
    // wait_for_I_input();
    draw_iso_npc(5+i,x,y,npcs[i].pos_x,npcs[i].pos_y,i);
  }
}

void draw_iso_npc(char sprite_no, int iso_x, int iso_y, int sprite_x, int sprite_y, char spr_index)
{
  int x_start, y_start, x, y; 
  
  y_start = (iso_y * 8)-6;
  x_start = (iso_x * 8)+8;

  x = (sprite_x * 16) + (sprite_y * 16); //this works because it's starting from bottom left, ending top right.
  y = (sprite_y * 8) - (sprite_x * 8); //won't work same if going another direction.
  draw_npc(sprite_no+5,x_start+x,y_start+y,spr_index);
}

void display_return_select_menu()
{
  remove_cursor();
  reset_cursor();
  set_menu_state(RETURN_SELECT_MENU,CONVOY_OPTIONS,CONVOY_COLS);
  
  display_battle_group_window(ARMY_BATTLE_GROUP_WINDOW_X,ARMY_BATTLE_GROUP_WINDOW_Y);
}

void display_party_options_window(char x, char y)
{
  set_menu_state(PARTY_OPTION_MENU,5,0);
  remove_cursor();
  load_cursor(x+1,y+2,SLIDER_ONE);
  put_string("Deploy",s_x_relative+x+2,s_y_relative+y+2);
  put_string("Organize",s_x_relative+x+2,s_y_relative+y+3);
  put_string("Formation",s_x_relative+x+2,s_y_relative+y+4);
  put_string("Return",s_x_relative+x+2,s_y_relative+y+5);
  put_string("Dismiss",s_x_relative+x+2,s_y_relative+y+6);
  update_unit_stats_window(NO_UNIT,ARMY_STATS_X,ARMY_STATS_Y,1);
  last_selected_col = 0;
  last_selected_cursor = 0;
  commander_select_cursor = 0;
}

void display_dismiss_select_menu()
{
  //BUG: I think this is where organize select show the wrong unit when selecting organize, going back and selecting another cmdr, and selecting organize again
  set_menu_state(DISMISS_SELECT_MENU,CONVOY_OPTIONS,CONVOY_COLS);
  remove_cursor();
  reset_cursor();
  load_cursor(23,3,SLIDER_ONE);

  update_convoy_window(ARMY_CONVOY_WINDOW_X,ARMY_CONVOY_WINDOW_Y);
  update_unit_stats_window(party_units[0]&0xFF,13,14,1);
}

void display_deploy_select_menu()
{
  //BUG: I think this is where organize select show the wrong unit when selecting organize, going back and selecting another cmdr, and selecting organize again
  remove_cursor();
  reset_cursor();
  set_menu_state(DEPLOY_SELECT_MENU,CONVOY_OPTIONS,CONVOY_COLS);
  load_cursor(23+(last_selected_col*4),3+last_selected_cursor,SLIDER_ONE);
  update_convoy_window(ARMY_CONVOY_WINDOW_X,ARMY_CONVOY_WINDOW_Y);
  update_unit_stats_window(party_units[0]&0xFF,ARMY_STATS_X,ARMY_STATS_Y,1);
  commander_select_cursor = last_selected_cursor;
  cursor_column = last_selected_col;
}

void display_oraganize_select_menu(char x, char y)
{
  load_selector(0x68C0,31);
  set_menu_state(ORGANIZE_SELECT_MENU,3,2);
  reset_cursor();
  refresh_battle_units();
  display_selector(63,get_iso_x(x,y,commander_select_cursor,cursor_column),get_iso_y(x,y,commander_select_cursor,cursor_column)+14,31);
  if(primary_menu == TRAIN_MENU)
  {
    update_unit_stats_window(party_commanders[selected_cmdr].bg.units[0].id,23,0,1);
  }
  else
  {
    update_unit_stats_window(party_commanders[selected_cmdr].bg.units[0].id,ARMY_STATS_X,ARMY_STATS_Y,1);
  }
  // draw_formation(selected_formation);
  // satb_update();
}

void display_training_enemy_window()
{
  if(training_enemies[0])
  {
    draw_32x32_sprite(training_enemies[0],(23*8)+16,(0*8)+10);
    print_unit_fullname(training_enemies[0],s_x_relative+23,s_y_relative+6);
    put_string("Level",s_x_relative+23,s_y_relative+7);

    put_string("Cost",s_x_relative+23,s_y_relative+12);
    set_font_pal(GOLD_FONT);
    put_number(25*training_enemies_count,3,s_x_relative+28,s_y_relative+12);
    if(!owned_formations[training_enemies_formation])
    {
      put_string("new form",s_x_relative+23,s_y_relative+9);
    }
    else
    {
      put_string("        ",s_x_relative+23,s_y_relative+9);
    }
    // put_string("new form",s_x_relative+23,s_y_relative+9);
    put_number(map_level_table[map_no],2,s_x_relative+28,s_y_relative+7);
    set_font_pal(WHITE_FONT);
  }
  else
  {
    spr_hide(1);
  }
}

void display_formation_menu()
{
  hide_npcs(5);
  reset_npcs();
  formation_cursor = 0;
  set_menu_state(FORMATION_MENU,owned_formation_count,45);
  draw_formation(owned_formations[0]);
  put_string("Form.",s_x_relative+15,s_y_relative+15);
  put_number(formation_cursor+1,2,s_x_relative+14,s_y_relative+16);
  put_char('/',s_x_relative+16,s_y_relative+16);
  put_number(owned_formation_count,2,s_x_relative+17,s_y_relative+16);
}

void set_formation_menu()
{
  // put_number(formation_cursor,4,s_x_relative,s_y_relative);
  draw_formation(get_next_formation(formation_cursor));
  put_string("Form.",s_x_relative+15,s_y_relative+15);
  put_number(formation_cursor+1,2,s_x_relative+14,s_y_relative+16);
  put_char('/',s_x_relative+16,s_y_relative+16);
  put_number(owned_formation_count,2,s_x_relative+17,s_y_relative+16);
}

void display_train_select_menu()
{
  set_menu_state(TRAIN_MENU,party_size,0);
  spr_hide(0);
  display_window_rel(0,0,10,14);
  display_window_rel(22,0,10,14);
  list_commanders(s_x_relative+2,s_y_relative+2);
  update_gold_amount(5,11,4,12);

  display_window_rel(ARMY_BATTLE_GROUP_WINDOW_X,ARMY_BATTLE_GROUP_WINDOW_Y,32,14);
  display_train_options_window();

  put_green_square(1,17);
  put_green_square(19,17);

  put_string("VS",15+s_x_relative,20+s_y_relative);
  load_cursor(1,2,SLIDER_ONE);
  commander_select_cursor = 0;
}

void update_gold_amount(char x, char y, char x1, char y1)
{
  put_string("Gold",s_x_relative+x,s_y_relative+y);
  set_font_pal(GOLD_FONT);
  put_number(player_gold,5,s_x_relative+x1,s_y_relative+y1);
  set_font_pal(WHITE_FONT);
}

void display_train_options_window()
{
  display_window_rel(10,0,12,14);
  put_string("Generate",s_x_relative+12,s_y_relative+2);
  put_string("Organize",s_x_relative+12,s_y_relative+3);
  put_string("Begin",s_x_relative+12,s_y_relative+4);
}

void set_train_options_menu()
{
  // selected_cmdr = commander_select_cursor;
  reset_npcs();
  load_cmdr_army_to_npcs(selected_cmdr);
  load_cmdr_army_to_npcs(MAX_PARTY_COMMANDERS);

  set_menu_state(TRAIN_OPTIONS_MENU,3,0);
  remove_cursor();
  reset_cursor();
  
  load_cursor(11,2,SLIDER_ONE);
  commander_select_cursor = 0;
  refresh_battle_units();
  draw_formation(selected_formation);
}

void set_menu_state(char state, char options, char columns)
{
  menu_state = state;
  menu_rows = options;
  menu_cols = columns;
}

void reset_cursor()
{
  commander_select_cursor = 0;
  cursor_column = 0;
}

void option_select_menu_buttons()
{
  if(commander_select_cursor == 0)//generate
  {
    clear_commander_battle_group(enemy_commanders);
    // hide_npcs(5);
    reset_npcs();
    randomize_enemy_team();
    load_predefined_group_layout(training_enemies,MAX_PARTY_COMMANDERS,0);

    load_cmdr_army_to_npcs(selected_cmdr);
    load_cmdr_army_to_npcs(MAX_PARTY_COMMANDERS);

    refresh_battle_units();
    display_training_enemy_window();
    // update_battle_group_window(selected_cmdr,1,17,0,training_enemies_count);
    // update_battle_group_window(MAX_PARTY_COMMANDERS,19,17,npc_count-training_enemies_count,0);
  }

  if(commander_select_cursor == 1)//organize
  {
    display_oraganize_select_menu(1,17);
    return;
  }

  if(commander_select_cursor == 2)//begin
  {
    if(training_enemies_count)
    {
      start_training_battle(selected_cmdr);
      reload_overworld();
      display_train_select_menu();
      training_enemies_count = 0;
      set_menu_state(TRAIN_OPTIONS_MENU,3,0);
      remove_cursor();
      load_cursor(11,2,SLIDER_ONE);
      commander_select_cursor = 0;
      draw_formation(selected_formation);
      load_cmdr_army_to_npcs(selected_cmdr);
      // update_battle_group_window(selected_cmdr,1,17,0,training_enemies_count);
      refresh_battle_units();
    }
  }
}

void   refresh_battle_units()
{
  update_battle_group_window(selected_cmdr,1,17,0,training_enemies_count);

  if(primary_menu == TRAIN_MENU)
  {
    update_battle_group_window(MAX_PARTY_COMMANDERS,19,17,npc_count-training_enemies_count,0);
  }
}

void menu_back_button()
{
  	// return;
	if(menu_state == RECRUIT_MENU || menu_state == SHOP_MENU || menu_state == TRAIN_MENU)
	{
		clear_commander_select();
		display_castle_menu();
		return;
	}
	if(menu_state == CASTLE_MENU)
	{
    if(!demo_flag)
    {
      load_map(0,0,0,0,MAP_WIDTH,OVERWORLD_MAP_HEIGHT);
      spr_show(0);
      satb_update();
      menu_state = OVERWORLD;
      menu_rows = 0;
      commander_select_cursor = 0;
      last_selected_cursor = 0;
    }
		return;
	}
  if(menu_state == ORGANIZE_SELECT_MENU || menu_state == DEPLOY_SELECT_MENU
   || menu_state == RETURN_SELECT_MENU || menu_state == DISMISS_SELECT_MENU)
  {
    spr_hide(63);

    if(primary_menu == TRAIN_MENU)
    {
      update_unit_stats_window(NO_UNIT,23,0,1);
      set_train_options_menu();
    }
    else
    {
      // display_party_commanders_window(0,0);
      display_party_options_window(10,0);
    }
    return;
  }
  if(menu_state == PARTY_OPTION_MENU)
  {
    hide_npcs(5);
    // update_unit_stats_window(NO_UNIT,ARMY_STATS_X,ARMY_STATS_Y,1);
    cursor_column = 0;
    commander_select_cursor = selected_cmdr;
    display_party_commanders_window(0,0);
    return;
  }
  if(menu_state == DEPLOY_SWAP_MENU)
  {
    commander_select_cursor = swap_unit % 8;
    cursor_column = swap_unit / 8;
    set_menu_state(DEPLOY_SELECT_MENU,CONVOY_OPTIONS,CONVOY_COLS);
    update_unit_stats_window(party_units[swap_unit]&0xFF,13,14,1);

    // reset_cursor();
    commander_select_cursor = last_selected_cursor;
    cursor_column = last_selected_col;
    display_cursor();
    spr_hide(63);
    // satb_update();
    return;
  }
	if(menu_state == BUY_SELECT_MENU)
	{
		commander_select_cursor = last_selected_cursor;
    cursor_column = last_selected_col;

		menu_state = SHOP_MENU;
		menu_rows = 6;

		load_cursor(1+(cursor_column*4),3+commander_select_cursor,SLIDER_ONE);
    // check_unit_in_row(0,0);
		print_unit_row_by_type(unit_header[0].a_type,2,cursor_y);

		display_cursor();
		display_window_rel(0,12,16,16);
		display_window_rel(16,12,16,16);
		hide_npcs(5);
		reset_npcs();
		return;
	}
  if(menu_state == ARMY_MENU)
  {
    clear_commander_select();
    display_castle_menu();
    return;
  }
  if(menu_state == SWAP_MENU)
  {
    swap_unit = 0;
    refresh_battle_units();
    menu_state = ORGANIZE_SELECT_MENU;
    draw_formation(selected_formation);
    return;
  }
  if(menu_state == TRAIN_OPTIONS_MENU)
  {
    hide_npcs(5);
    reset_npcs();
    display_train_select_menu();
    return;
  }

  if(menu_state == FORMATION_MENU)
  {
    draw_formation(party_commanders[selected_cmdr].bg.formation);
    display_party_options_window(10,0);
    load_cmdr_army_to_npcs(selected_cmdr);
    refresh_battle_units();
    put_string("        ",s_x_relative+13,s_y_relative+15);
    put_string("        ",s_x_relative+13,s_y_relative+16);
    formation_cursor = 0;
    return;
  }
}