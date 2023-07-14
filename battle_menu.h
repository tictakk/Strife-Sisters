#define BS_SQUARE_TOP_LEFT 418
#define PICKER_LEFT 1
#define PICKER_RIGHT 2
#define PICKER_RIGHT_START_X 3
#define PICKER_LEFT_START_X 2

char target_type = 0;
char picker = 0;
char bs_index_y = 0;
char bs_index_x = 0;
char bs_min_index_y = 0;
char bs_min_index_x = 0;
char bs_max_index_y = 0;
char bs_max_index_x = 0;
char marked_targets[MAX_ARMY_SIZE];

char display_battle_selector(char is_attacker, char type)
// void display_battle_selector(char is_attacker, BattleUnit *battleunits, char cursor_pos)
{
  // put_number(picker,3,0,0);
  if(picker == 0)
  {
    // put_number(69,3,6,0);
    target_type = type;
    determine_picker(is_attacker,target_type == ALL_ALLIES);
    set_picker_params();
    set_target_params();
  }

  satb_update();
  clear_joy_events(0x1F);
  load_palette(14,redpal,1);
  highlight_target_type(0xE000);

  return battle_selector_picker(132,120);
}

char battle_selector_picker(int x, int y)
{
    char loop = 1;
    while(loop)
    {
        switch(joytrg(0))
        {
            case JOY_I:
            loop = 0;
            // highlight_target_type(0);
            mark_bs_targets();
            // clear_bs_settings();
            return 1;

            case JOY_II:
            // put_number(-1,3,0,0);
            // put_number(-1,3,6,0);
            loop = 0;
            unmark_all();
            highlight_target_type(0);
            clear_bs_settings();
            picker = 0;
            return 0;

            case JOY_RIGHT:
            bs_right();
            break;

            case JOY_DOWN:
            bs_down();
            break;

            case JOY_UP:
            bs_up();
            break;

            case JOY_LEFT:
            bs_left();
            break;
        }
        vsync();
    }
}

void bs_left()
{
  if(bs_index_x > bs_min_index_x)
  {
    highlight_target_type(0);
    bs_index_x -= 1;
    highlight_target_type(0xE000);
  }
}

void bs_up()
{
  if(bs_index_y <= bs_min_index_y){ return; }
  highlight_target_type(0);
  bs_index_y -= 1;
  highlight_target_type(0xE000);
}

void bs_down()
{
  if(bs_index_y >= bs_max_index_y) { return; }
  highlight_target_type(0);
  bs_index_y += 1;
  highlight_target_type(0xE000);
}

void bs_right()
{
  if(bs_index_x >= bs_max_index_x) { return; }
  highlight_target_type(0);
  bs_index_x += 1;
  highlight_target_type(0xE000);
}

void highlight_battle_square(int square, int pal)
{
  int i, j;
  for(i=0; i<3; i++)
  {
    for(j=0; j<4; j++)
    {
      change_background_pal_single(square+(j*32)+i,pal);
    }
  }
}

int calc_bs_pos(int x, int y)
{
  return BS_SQUARE_TOP_LEFT + (x * 5) + (y * 160);
}

void highlight_target_type(int pal)
{
  switch(target_type)
  {
    case SINGLE_HIT: highlight_single(pal); break;
    case MULTI_COL_2:
    case MULTI_COL_3: highlight_column(pal); break;
    case MULTI_ROW: highlight_row(pal); break;
    default: highlight_all(pal); break;
  }
}

void highlight_column(int pal)
{
  int i;
  for(i=0; i<3; i++)
  {
    highlight_battle_square(calc_bs_pos(bs_index_x,bs_index_y+i),pal);
  }
}

void highlight_row(int pal)
{
  int i;
  for(i=0; i<3; i++)
  {
    highlight_battle_square(calc_bs_pos(bs_index_x+i,bs_index_y),pal);
  }
}

void highlight_single(int pal)
{
  highlight_battle_square(calc_bs_pos(bs_index_x,bs_index_y),pal);
}

void highlight_all(int pal)
{
  int i, j;
  for(i=0; i<3; i++)
  {
    for(j=0; j<3; j++)
    {
      highlight_battle_square(calc_bs_pos(bs_index_x+i,bs_index_y+j),pal);
    }
  }
}

void set_picker_params()
{
  if(picker == PICKER_RIGHT)
  {
    bs_min_index_x = PICKER_RIGHT_START_X;
    bs_index_x = PICKER_RIGHT_START_X;
  }
  else
  {
    bs_index_x = 2;
    bs_min_index_x = 0;
    bs_max_index_x = 2;
  }
}

void set_target_params()
{
  // put_number(target_type,3,0,0);
  switch(target_type)
  {
    case SINGLE_HIT: set_target_single_params(); break;
    case MULTI_COL_2:
    case MULTI_COL_3: set_target_column_params(); break;
    case MULTI_ROW: set_target_row_params(); break;
    case ALL_ALLIES:
    case ALL_UNITS:
    case ALL_OPPONENTS: set_target_all_params(); break;
  }
}

void set_target_row_params()
{
  bs_index_x = (bs_index_x == 3)? bs_index_x : 0;
  bs_min_index_x = bs_index_x;
  bs_max_index_x = 0;
  bs_min_index_y = 0;
  bs_max_index_y = 2;
}

void set_target_column_params()
{
  bs_max_index_x = 2 + bs_min_index_x;
  bs_min_index_y = 0;
  bs_max_index_y = 0;
}

void set_target_single_params()
{
  // bs_min_index_x = 0;
  bs_max_index_x = 2 + bs_min_index_x;
  bs_min_index_y = 0;
  bs_max_index_y = 2;
}

void set_target_all_params()
{
  // bs_index_x %= 4;
  bs_index_x = (bs_index_x == 3)? bs_index_x : 0;
  bs_min_index_x = bs_index_x;//0;
  bs_max_index_x = bs_index_x;//0;
  bs_min_index_y = bs_index_y;//0;
  bs_max_index_y = bs_index_y;//0;
}

void mark_bs_targets()
{
  switch(target_type)
  {
    case SINGLE_HIT: mark_single(); break;
    case MULTI_COL_2:
    case MULTI_COL_3: mark_column(); break;
    case MULTI_ROW: mark_row(); break;
    case ALL_ALLIES:
    case ALL_UNITS:
    case ALL_OPPONENTS: mark_all(); break;
  }
}

void mark_column()
{
  char i;

  for(i=0; i<3; i++)
  {
    if(picker == PICKER_LEFT)
    {
      marked_targets[((2-bs_index_x)*3) + i] = 1;
    }
    if(picker == PICKER_RIGHT)
    {
      marked_targets[((bs_index_x - 3)*3) + i] = 1;
    }
  }
}

void mark_row()
{
  char i;
  for(i=0; i<3; i++)
  {
    marked_targets[(i*3)+bs_index_y] = 1;
  }
}

void mark_single()
{
  if(picker == PICKER_LEFT)
  {
    marked_targets[((2-bs_index_x)*3) + bs_index_y] = 1;
  }
  if(picker == PICKER_RIGHT)
  {
    marked_targets[((bs_index_x - 3)*3) + bs_index_y] = 1;
  }
}

void mark_all()
{
  char i;
  for(i=0; i<MAX_ARMY_SIZE; i++)
  {
    marked_targets[i] = 1;
  }
}

void unmark_all()
{
  char i;
  for(i=0; i<MAX_ARMY_SIZE; i++)
  {
    marked_targets[i] = 0;
  }
}

void print_bs_marked_menu()
{
  char i,j;
  for(i=0; i<3; i++)
  {
    for(j=0; j<3; j++)
    {
      put_number(marked_targets[(i*3)+j],2,i*2,j);
    }
  }
}

void determine_picker(char is_attacker, char target_allies)
{
  if(is_attacker && target_allies)
  {
    picker = PICKER_LEFT;
    return;
  }
  if(is_attacker && !target_allies)
  {
    picker = PICKER_RIGHT;
    return;
  }
  if(!is_attacker && target_allies)
  {
    picker = PICKER_RIGHT;
    return;
  }
  if(!is_attacker && !target_allies)
  {
    picker = PICKER_LEFT;
    return;
  }
}

void clear_bs_settings()
{
  bs_index_x = 0;
  bs_index_y = 0;
  bs_max_index_x = 0;
  bs_max_index_y = 0;
  bs_min_index_x = 0;
  bs_min_index_y = 0;
  // picker = 0;
  target_type = 0;
}