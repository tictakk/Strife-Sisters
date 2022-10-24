#define TILE_SIZE 16
#define BF_X_TILES 16
#define BF_Y_TILES 12
#define MAP_SIZE 224

#define SELECT_MODE 0
#define PLACE_MODE 1
#define MENU_MODE 2
#define SPLIT_MODE 3
#define ATTACK_MODE 4
#define MERGE_MODE 5
#define OBSERVE_MODE 6
#define DIALOG_MODE 7
#define EXPLORE_MODE 8
#define EXPLORE_MENU_MODE 9
#define ACTION_MODE 10

#define MAX_ENTITIES 25
#define PLAYER 1
#define CPU 2
#define INF 0
#define SPEAR 1
#define MUSKET 2
#define FLYER 3
#define HOUND 4

#define ACTIONS_PER_TURN 6
#define MOVE_COST 2
#define SPLIT_COST 2
#define ATTACK_COST 2
#define MERGE_COST 2

#define MENU_ATTACK 1
#define MENU_MOVE 0
#define MENU_END 3
#define MENU_TURN 5
#define MENU_SPLIT 2
#define MENU_MERGE 99
#define MENU_BACK 4

#define ARCHER_ATTACK 8
#define SWORD_ATTACK 4
#define SPEAR_ATTACK 4

#define COMMANDER_FIGURE_BEGIN 0x6000

typedef struct{
  int pos;
  char army_size, team, id, actionable, objective;
} Entity;

int exp_gained;
int gold_gained;
unsigned char battle_grid[464];//MAP_SIZE];
Entity entities[MAX_ENTITIES];
char selector_mode, actions;
int sel_x, sel_y, turn, menu_x, menu_y, cursor_x, cursor_y, unit_selected, last_pos;
char num_of_entities, menu_option;
char one_total;
char two_total;
char cost;
char cmdr_fig_pal;
char current_turn;
int path[20];
char b_map_id;
char no_of_spawns;
char map_type;
char no_of_cpu_cmdrs;
char no_of_player_cmdrs;
char last_command;
// const char spear_attack[4] = { 2, 0, 2, 0};

const int archer_coord1[2] = {1,-16};
const int archer_coord2[2] = {0,-32};
const int archer_coord3[2] = {1,16};
const int archer_coord4[2] = {0,32};
const int archer_coord5[2] = {-1, -16};
const int archer_coord6[2] = {-1, 16};
const int archer_coord7[2] = {-2, 0};
const int archer_coord8[2] = {2, 0};

const int spear_coord1[2] = {0, -16};
const int spear_coord2[2] = {0, -32};
const int spear_coord3[2] = {0, 16};
const int spear_coord4[2] = {0, 32};
// const int spear_coord5[2] = {1, 0};
// const int spear_coord6[2] = {-1, 0};

const int sword_coord1[2] = {0, 16};
const int sword_coord2[2] = {1, 0};
const int sword_coord3[2] = {-1, 0};
const int sword_coord4[2] = {0, -16};

// struct Coords coords[9];
int coords[18];

void load_coords(char id)
{
  // switch((*entities[id].unit).unit_type)
  // {
  //   case SPEARS:
  //   coords[0] = spear_coord1[0];
  //   coords[1] = spear_coord1[1];
  //
  //   coords[2] = spear_coord2[0];
  //   coords[3] = spear_coord2[1];
  //
  //   coords[4] = spear_coord3[0];
  //   coords[5] = spear_coord3[1];
  //
  //   coords[6] = spear_coord4[0];
  //   coords[7] = spear_coord4[1];
  //   break;
  //
  //   case MUSKETS:
  //   coords[0] = archer_coord1[0];
  //   coords[1] = archer_coord1[1];
  //
  //   coords[2] = archer_coord2[0];
  //   coords[3] = archer_coord2[1];
  //
  //   coords[4] = archer_coord3[0];
  //   coords[5] = archer_coord3[1];
  //
  //   coords[6] = archer_coord4[0];
  //   coords[7] = archer_coord4[1];
  //
  //   coords[8] = archer_coord5[0];
  //   coords[9] = archer_coord5[1];
  //
  //   coords[10] = archer_coord6[0];
  //   coords[11] = archer_coord6[1];
  //
  //   coords[12] = archer_coord7[0];
  //   coords[13] = archer_coord7[1];
  //
  //   coords[14] = archer_coord8[0];
  //   coords[15] = archer_coord8[1];
  //   break;
  //
  //   case BLOBS:
  //   case FLYERS:
  //   case INFANTRY:
  //   default:
    coords[0] = sword_coord1[0];
    coords[1] = sword_coord1[1];

    coords[2] = sword_coord2[0];
    coords[3] = sword_coord2[1];

    coords[4] = sword_coord3[0];
    coords[5] = sword_coord3[1];

    coords[6] = sword_coord4[0];
    coords[7] = sword_coord4[1];
  //   break;
  // }
}

char get_pattern_length(char id)
{
  // switch((*entities[id].unit).unit_type)
  // {
  //   case BLOBS:
  //   case FLYERS:
  //   case INFANTRY: return SWORD_ATTACK;
  //   case SPEARS: return SPEAR_ATTACK;
  //   case MUSKETS: return ARCHER_ATTACK;
  //   default: return SWORD_ATTACK;
  // }
  return SWORD_ATTACK;
}

void add_entity(char team, char pal, char cmdr, char id, int pos)
{
  // put_number(pal,3,10,5+(g++));
  entities[num_of_entities].id = id;
  entities[num_of_entities].team = team;
  entities[num_of_entities].pos = pos;
  entities[num_of_entities].actionable = 1;
  entities[num_of_entities++].objective = 0; //should start at 0
  // add_npc(pos%16,pos/16,commanders[cmdr-16].sprite_type,pal);
  add_npc(pos%16,pos/16,commanders[id].sprite_type,pal);
}

void update_selector_pos(int x, int y)
{
  if(sy == 208 && y > 0 && s_y < 464)
  {
    yOffset -= 16;
    s_y += 16;
    update_map();
    scroll(0,s_x,s_y+32,32,224,0xC0);
  }
  else if(sy == 32 && y < 0 && s_y > 0)
  {
    yOffset += 16;
    s_y -= 16;
    update_map();
    scroll(0,s_x,s_y+32,32,224,0xC0);
  }
  sx += x;
  sy = min(max((sy + y),32),208);
  draw_selector();
}

void draw_selector()
{
  spr_set(SELECTOR);
  spr_x(sx);
  spr_y(sy);
}

void hide_menu()
{
  unhighlight();
  // hide_selector();
  hide_cursor();
  scroll(1,0,0,0,32,0x80);
}

void unhighlight()
{
  load_map(0,2,0,0,16,29);
}

void hide_cursor()
{
  spr_set(CURSOR);
  spr_hide();
}

void hide_selector()
{
  spr_set(SELECTOR);
  spr_hide();
}

char check_action_cost(char required_cost){ return 1; }

char destroy_entity(int id)
{
  //NEXT THING TO DO
  int i, id_location;
  char entity_id, killed;
  killed = 0;
  entity_id = id;

  // if(entities[entity_id].army_size == 0)
  // {
    battle_grid[entities[id].pos] = 0;
    killed = 1;
    if(entities[entity_id].team == PLAYER)
    {
      one_total--;
    }
    else
    {
      get_random_item_by_level(0);
      exp_gained += 25;//entities[entity_id].unit->exp;
      gold_gained += range(0,20);
      two_total--;
    }
    for(i=0; i<464; i++)
    {
      if(battle_grid[i] > entity_id+1)
      {
        battle_grid[i] -= 1;
      }
    }
    destroy_npc(entity_id);
    for(i=entity_id; i<num_of_entities; i++)
    {
      memcpy(&entities[i],&entities[i+1],sizeof(Entity)); //cpy dst from src
    }
    num_of_entities--;
  // }
  return killed;
}

void remove_unit_from_grid(int grid_pos)
{
  char entity_id, i;
  entity_id = battle_grid[grid_pos]-1;

  battle_grid[grid_pos] = 0;
  for(i=0; i<464; i++)
  {
    if(battle_grid[i] > entity_id)
    {
      battle_grid[i] -= 1;
    }
  }

  for(i=entity_id; i<num_of_entities-1; i++)
  {
    memcpy(&entities[i],&entities[i+1],sizeof(Entity));
  }
  reset_satb();
  display_sprites_();
  hide_menu();

  num_of_entities--;
}

void move_unit(int to, int from)
{
  char id;
  // hide_menu();
  id = battle_grid[from] - 1;

  last_pos = from;
  battle_grid[to] = battle_grid[from];
  battle_grid[from] = 0;
  entities[id].pos = to;
  unit_selected = to;

  // selector_mode = SELECT_MODE;
  update_map();
}

void select_unit(int unit)
{
  selector_mode = PLACE_MODE;
  menu_option = MENU_ATTACK;
  // hide_selector();

  draw_selector();
  // highlight(unit,0xC000,(*entities[battle_grid[unit]-1].unit).mov);
  highlight(unit,0xC000,3);
}

int calc_move_cost(int origin, int dest)
{
  int o_x, o_y, d_x, d_y, x, y;
  o_x = origin % 16;
  o_y = origin / 16;
  d_x = dest % 16;
  d_y = dest / 16;
  return (char)(abs(o_x - d_x) + abs(o_y - d_y)) * 2;
}

char attack_unit(int src, int dst)
{
  int attacker, target;//, result;
  attacker = battle_grid[dst]-1;
  target = battle_grid[src]-1;

  // put_number(attacker,3,15,15);
  // put_number(target,3,15,16);
  // return 2;
  // if(entities[attacker].actionable == 0)
  // {
  //   return 2;
  // }
  if(entities[attacker].team != entities[target].team)
  {
    // if(attackable(attacker,target,coords))
    if(1)
    {
      last_command = selector_mode;
      entities[attacker].actionable = 0;
      // deduct_stamina(attacker);
      // deduct_stamina(target);
      hide_menu();
      // menu_x = -64;
      // menu_y = -64;
      cursor_x = -32;
      cursor_y = -32;
      selector_mode = SELECT_MODE;
      return begin_battle(attacker,target,dst,src);
      // return 2;
      // selector_mode = SELECT_MODE;
      // return result;
      // entities[attacker].actionable = 0;
    }
    else
    {
      return -1;
    }
  }
  else
  {
    return -1;
  }

  return -1;
}

void clear_text_field()
{
  put_string("                ",1,1);
  put_string("                ",1,2);
}

void post_battle_dialog()
{
  clear_text_field();
  // print_items_gained();
  print_post_battle_info("Gold received",gold_gained);
  print_post_battle_info("EXP received",exp_gained);
  // distribute_exp();

  // print_post_battle_info("Rei exp",commanders[party[0]].exp);
  // print_post_battle_info();

  clear_text_field();
  //items gained
  //money gained
  //exp gained
  //level ups
  //spells learned
}

void print_items_gained()
{
  char i=0;
  while(i<drop_count)
  {
    clear_text_field();
    write_text(1,1,"received ");
    write_text(10,1,items[battle_items[i]].name);
    i++;
    wait_for_I_input();
  }
}

void print_post_battle_info(char *str, int value)
{
  clear_text_field();
  write_text(1,1,str);
  put_number(value,4,1,2);
  wait_for_I_input();
}

void print_level_up(){}
void print_spells_learned(){}

void distribute_exp()
{
  char i, lvl;
  int exp_per, offset;
  offset = 0;
  exp_per = exp_gained / no_of_player_cmdrs;
  for(i=0; i<no_of_player_cmdrs; i++)
  {
    lvl = commanders[party[i]].lvl;
    commanders[party[i]].exp += exp_per;
    commanders[party[i]].lvl = next_level(commanders[party[i]].lvl,commanders[party[i]].exp);
    if(commanders[party[i]].lvl > lvl)
    {
      commanders[party[i]].exp = 0;
      clear_text_field();
      offset = write_text(1,1,commanders[party[i]].name);
      write_text(1+offset,1," lvled up");
      offset = write_text(1,2,"level ");
      put_number(commanders[party[i]].lvl,2,1+offset,2);
      wait_for_I_input();
      print_new_stats(party[i]-16);
      //THIS IS NOT UPDATING THE UNIT STATS
      // memcpy(&unit_list[entities[0].id],commanders[party[i]].unit,sizeof(Unit));
      // wait_for_I_input();
    }
  }
}

void print_new_stats(char cmdr_id)
{
  int offset;
  char i;

  for(i=0; i<4; i++) //4 == num of attributes
  {
    clear_text_field();
    // scale_stat(cmdr_id,i);
    // switch(i)
    // {
    //   case ATK_ATTRIBUTE:
    //   offset = write_text(1,1,"Atk ");
    //   put_number(commanders[cmdr_id].unit->atk,3,1+offset,1);
    //   break;
    //
    //   case DEF_ATTRIBUTE:
    //   offset = write_text(1,1,"Def ");
    //   put_number(commanders[cmdr_id].unit->def,3,1+offset,1);
    //   break;
    //
    //   case SPD_ATTRIBUTE:
    //   offset = write_text(1,1,"Speed ");
    //   put_number(commanders[cmdr_id].unit->spd,3,1+offset,1);
    //   break;
    //
    //   case HP_ATTRIBUTE:
    //   offset = write_text(1,1,"HP ");
    //   put_number(commanders[cmdr_id].unit->hp,4,1+offset,1);
    //   break;
    // }
    wait_for_I_input();
  }
}

void undo()
{
  // battle_grid[unit_selected];
  set_cursor_pos(last_pos);
  move_unit(last_pos,unit_selected);
  print_menu();
  select_unit(unit_selected);
  hide_cursor();
}

void show_selectable_menu_options()
{
  if(selector_mode == ACTION_MODE)
  {
    set_font_pal(11);
    print_menu();
    set_font_pal(10);
    //check if anyone is attackable..
    if(1)
    {
      put_string("ATK",20,2);
    }
    put_string("END",24,2);
  }
  else
  {
    print_menu();
  }
}

void print_menu()
{
  put_string("MOV",20,1);
  put_string("ATK",20,2);
  put_string("ART",24,1);
  put_string("END",24,2);
  put_string("TLK",28,1);
  put_string("EXT",28,2);
}

void update_map();
void begin_battle(int src, int dst, int src_p, int dst_p);
void set_cursor_pos(int pos);
