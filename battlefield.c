#include "battlefield.h"
#include "battle_ai.c"

// #incchr(battlefield,"map/sprites/battlefield2.pcx")
// #incpal(battlefieldpal, "map/sprites/battlefield2.pcx")
// #incbat(battlefieldbat, "map/sprites/battlefield2.pcx",0x1000,32,28)
#incspr(bf_menu, "map/sprites/bf_menu.pcx",0,0,2,4)
#incspr(be_menu2, "map/sprites/bf_menu.pcx",32,0,2,4)
#incpal(bf_menu_pal, "map/sprites/bf_menu.pcx")
// #incspr(pad, "map/sprites/direction_pad.pcx")

#incpal(bluepal,"map/sprites/bluetiles.pcx")
#incpal(redpal,"map/sprites/redtiles.pcx")

// #incspr(sld, "map/sprites/test.pcx")
// #incspr(sld1, "map/sprites/test1.pcx")
#incspr(msk,"map/sprites/msktpiece.pcx")
#incspr(sld,"map/sprites/swrdpiece.pcx")
#incspr(sld1,"map/sprites/sprpiece.pcx")
#incspr(actionbtn, "map/sprites/action.pcx")
#incpal(sldpal, "map/sprites/swrdpiece.pcx")
#incpal(sld1pal, "map/sprites/sprpiece.pcx")
#incpal(actionpal, "map/sprites/action.pcx")
#incpal(mskpal,"map/sprites/msktpiece.pcx")

const char tile_pal[28];
char g;
char id;
char first;
char *army_one;
char *army_two;
char mapper_size;

void battlefield_loop(char *deploy_one, char s_one, char *deploy_two, char s_two)
{
  turn = CPU;
  ai_timer = 20;
  // u = 198;
  done = 1;
  army_one = deploy_one;
  army_two = deploy_two;
  size_one = s_one;
  size_two = s_two;
  one_total = s_one;
  two_total = s_two;
  mapper_size = size_one+size_two;
  cmdr_fig_current = COMMANDER_FIGURE_BEGIN;
  cmdr_fig_pal = 19;
  num_of_bad_terrains = 7;

  untraversable_terrain[0] = 8;
  untraversable_terrain[1] = 12;
  untraversable_terrain[3] = 13;
  untraversable_terrain[4] = 15;
  untraversable_terrain[5] = 17;
  untraversable_terrain[6] = 22;

  init_battlefield();
  load_battlefield_map();

  // load_vram(0x1000, panel_gfx, 2048);
  set_font_color(13,14);
  set_font_pal(0);
  load_default_font(0,0x800);
  selector_mode = 0;

  init_units();
  load_sprites_();
  update_map();
  scroll(0, 0, 0, 0, 223, 0xC0);

  disp_on();
  satb_update();
  vsync();
  // put_string("actions:",17,2);
  entities[1].is_cmdr = 1;
  // init_ai();
  start_turn(CPU);
  entities[0].stamina = 2;
  entities[1].stamina = 1;
  while(one_total > 0 && one_total > 0)
  {
    if(turn == CPU)
    {
      ai();
    }
    else
    {
      ctrls();
      display_abs();
    }

    draw_selector();
    satb_update();
    vsync();
  }
  cleanup_battlefield();
  if(size_one==0)
  {
    return 0;
  }
  if(size_two==0)
  {
    return 1;
  }
  //whoops?
}

void init_units()
{
  char *u;
  char i;
  // entities = player_entities;

  for(i=0, u = army_one; i<size_one; i++, u++)
  {
    cmdr_vram_mapper[i].id = *u;
    cmdr_vram_mapper[i].address = cmdr_fig_current;
    set_sprite_to_location(*u,cmdr_fig_current,cmdr_fig_pal+i);
    cmdr_fig_current -= 0x200;
  }

  for(u = army_two; i<size_one+size_two; i++, u++)
  {
    cmdr_vram_mapper[i].id = *u;
    cmdr_vram_mapper[i].address = cmdr_fig_current;
    set_sprite_to_location(*u,cmdr_fig_current,cmdr_fig_pal+i);
    cmdr_fig_current -= 0x200;
  }
  cmdr_fig_current = COMMANDER_FIGURE_BEGIN;

  for(i=0, u=army_one; i<size_one; i++, u++)
  {
    // put_number(commanders[*u].id,3,0,10+i);
    switch(commanders[*u].army_type)
    {
      case 0:
      add_entity(INF,commanders[*u].army_size,PLAYER,cmdr_fig_pal+i,&infantry,1,*u,37+i);
      battle_grid[37+i] = num_of_entities;
      break;

      case 1:
      add_entity(SPEAR,commanders[*u].army_size,PLAYER,cmdr_fig_pal+i,&spears,1,*u,37+i);
      battle_grid[37+i] = num_of_entities;
      break;

      case 2:
      add_entity(MUSKET,commanders[*u].army_size,PLAYER,cmdr_fig_pal+i,&muskets,1,*u,37+i);
      battle_grid[37+i] = num_of_entities;
      break;

      default:
      add_entity(INF,commanders[*u].army_size,PLAYER,cmdr_fig_pal+i,&infantry,1,*u,37+i);
      battle_grid[37+i] = num_of_entities;
      break;
    }
  }
  // entities = ai_entities;
  for(i=0, u=army_two; i<size_two; i++,u++)
  {
    // put_number(commanders[*u].army_type,3,0,10+i);
    switch(commanders[*u].army_type)
    {
      case 0:
      add_entity(INF,commanders[*u].army_size,CPU,cmdr_fig_pal+i,&infantry,1,*u,198+i);
      battle_grid[198+i] = num_of_entities;
      break;

      case 1:
      add_entity(SPEAR,commanders[*u].army_size,CPU,cmdr_fig_pal+i,&spears,1,*u,198+i);
      battle_grid[198+i] = num_of_entities;
      break;

      case 2:
      add_entity(MUSKET,commanders[*u].army_size,CPU,cmdr_fig_pal+i,&muskets,1,*u,198+i);
      battle_grid[198+i] = num_of_entities;
      break;

      default:
      add_entity(INF,commanders[*u].army_size,CPU,cmdr_fig_pal+i,&infantry,1,*u,198+i);
      battle_grid[198+i] = num_of_entities;
      break;
    }
  }
  // entities = player_entities;
  cmdr_fig_pal = 19;
}

void init_battlefield()
{
  g=0;
  unit_selected = 0;
  selector_mode = 0;
  num_of_entities = 0;
  s_x = 0;
  s_y = 0;
  menu_x = -64;
  menu_y = -32;
  cursor_x = -16;
  cursor_y = -16;
  actions = ACTIONS_PER_TURN;
  menu_option = 0;
}

void load_battlefield_map()
{
  set_screen_size(SCR_SIZE_32x32);
  load_palette(0,battlefieldpal,1);
  // load_vram(TILE_VRAM,battlefield,0x1000);
  // load_background(battlefield,battlefieldpal,battlefieldbat,32,28);
  set_map_data(battlefieldbat,16,16);
  set_tile_data(battlefield,28,tile_pal,16);
  load_tile(0xe00);
  load_map(0,0,0,0,16,OVERWORLD_MAP_HEIGHT);
}

void load_sprites_()
{
  char *u;
  char i;
  load_palette(16,brskrpal,1);
  load_palette(17,sld1pal,1);
  load_palette(18,mskpal,1);

  load_palette(29,selectorpal,1);
  load_palette(30,bf_menu_pal,1);
  load_palette(31,actionpal,1);

  load_palette(1,bluepal,1);
  load_palette(2,redpal,1);

  load_vram(0x5000,sld,0x200);
  load_vram(0x5200,sld1,0x200);
  load_vram(0x5400,msk,0x200);
  load_vram(0x6200,bf_menu,0x200);
  load_vram(0x6400,be_menu2,0x200);
  load_vram(0x6600,actionbtn,0x80);

  load_vram(0x68C0,selector,0x40);
  load_vram(0x6900,cursor,0x40);

  display_sprites_();
}

void display_sprites_()
{
  spr_make(0,s_x,s_y,0x68C0,0,NO_FLIP|SZ_16x16,29,1);
  spr_make(1,cursor_x,cursor_y,0x6900,FLIP_MAS|SIZE_MAS,FLIP_X|SZ_16x16,29,1);
  spr_make(2,menu_x,menu_y,0x6200,FLIP_MAS|SIZE_MAS,NO_FLIP|SZ_32x64,30,1);
  spr_make(3,menu_x+32,menu_y,0x6400,FLIP_MAS|SIZE_MAS,NO_FLIP|SZ_32x64,30,1);
  load_actions();
  draw_actions();
}

int get_address_from_id(char id)
{
  char i;
  for(i=0; i<mapper_size; i++)
  {
    if(cmdr_vram_mapper[i].id == id)
    {
      return cmdr_vram_mapper[i].address;
    }
  }
}

void update_map()
{
  int squares, i, sprite_counter, entity_no, p, s, z;
  z=0;
  squares = 223;
  sprite_counter = 63;

  for(i = 0; i < squares; i++)
  {
    if(battle_grid[i] != 0)
    {
      entity_no = battle_grid[i]-1;

      s = entities[entity_no].army_size/15;//max_army_size
      if(entities[entity_no].is_cmdr)
      {
        p = get_address_from_id(entities[entity_no].id);
      }
      else
      {
        p = 0x5000 + (entities[entity_no].unit_type * 0x200);
      }
      if(s>1)
      {
        p += (0x40*(3-s));
      }
      else
      {
        p += 0x100 + (0x40*(1-s));
      }
      spr_make(sprite_counter--,abs_x_from_graph(i),abs_y_from_graph(i)-16,p,FLIP_MAS|SIZE_MAS,SZ_16x32,entities[entity_no].pal,1);

    }
  }
  // draw_actions();
}

void ctrls()
{
  char j;
  int abs;
  j = joytrg(0);

  if(j & JOY_UP)
  {
    if(selector_mode == SELECT_MODE || selector_mode == PLACE_MODE || selector_mode == SPLIT_MODE || selector_mode == ATTACK_MODE || selector_mode == MERGE_MODE)
    {
      selector_up();
    }
    else if(selector_mode == MENU_MODE)
    {
      if(menu_option == MENU_ATTACK)
      {
        return;
      }
      cursor_y -= 8;
      menu_option -= 1;
      spr_set(1);
      spr_y(cursor_y);
    }
  }

  if(j & JOY_DOWN)
  {
    if(selector_mode == SELECT_MODE || selector_mode == PLACE_MODE || selector_mode == SPLIT_MODE || selector_mode == ATTACK_MODE || selector_mode == MERGE_MODE)
    {
      selector_down();
    }
    else if(selector_mode == MENU_MODE)
    {
      if(menu_option == MENU_BACK)
      {
        return;
      }
      cursor_y += 8;
      menu_option += 1;
      spr_set(1);
      spr_y(cursor_y);
    }
  }

  if(j & JOY_RIGHT)
  {
    if(selector_mode == SELECT_MODE || selector_mode == PLACE_MODE || selector_mode == SPLIT_MODE || selector_mode == ATTACK_MODE || selector_mode == MERGE_MODE)
      selector_right();
  }

  if(j & JOY_LEFT)
  {
    if(selector_mode == SELECT_MODE || selector_mode == PLACE_MODE || selector_mode == SPLIT_MODE || selector_mode == ATTACK_MODE || selector_mode == MERGE_MODE)
      selector_left();
  }

  abs = graph_from_x_y(s_x,s_y);

  if(j & JOY_I)
  {
    if(selector_mode == SELECT_MODE)
    {
      if(battle_grid[abs] != 0)
      {
        unit_selected = abs;
        id = battle_grid[abs];
        selector_mode = MENU_MODE;
        spr_set(0);
        spr_hide();
        // display_unit_info(id);
        display_menu();
        update_map();
      }
    }

    else if(selector_mode == PLACE_MODE)
    {
      if(battle_grid[abs] == 0 && is_traversable(abs))
      {
        move_unit(abs,unit_selected);
      }
    }
    else if(selector_mode == ATTACK_MODE)
    {
      if(battle_grid[abs] != 0)
      {
        attack_unit(abs,unit_selected);
      }
    }
    else if(selector_mode == MENU_MODE)
    {
      switch(menu_option)
      {
        case MENU_ATTACK:
        if(check_action_cost(ATTACK_COST))
        {
          selector_mode = ATTACK_MODE;
          hide_menu();
          highlight_near_squares(unit_selected,0x2000,ATTACK_COST/2);
          display_selector();
        }
        break;

        case MENU_MOVE:
        if(check_action_cost(MOVE_COST))
        {
          // selector_mode = PLACE_MODE;
          // menu_option = MENU_ATTACK;
          // hide_menu();
          // highlight_near_squares(unit_selected,0x1000,actions/MOVE_COST);
          // display_selector();

          select_unit(unit_selected);
          hide_menu();
        }
        else
        {
          display_error_message("insufficient actions");
        }
        break;

        case MENU_SPLIT:
        if((entities[battle_grid[unit_selected]-1].army_size-1)/15)
        {
          if(check_action_cost(SPLIT_COST))
          {
            selector_mode = SPLIT_MODE;
            menu_option = MENU_ATTACK;
            highlight_near_squares(unit_selected,0x1000,1);
            hide_menu();
            display_selector();
          }
          else
          {
            display_error_message("insufficient actions");
          }
        }
        else
        {
          display_error_message("not enough units");
        }
        break;

        case MENU_MERGE:
        if(check_action_cost(MERGE_COST))
        {
          selector_mode = MERGE_MODE;
          menu_option = MENU_ATTACK;
          highlight_near_squares(unit_selected,0x1000,1);
          hide_menu();
          display_selector();
        }
        else
        {
            display_error_message("not enough actions");
        }
        break;

        case MENU_BACK:
        selector_mode = SELECT_MODE;
        menu_option = MENU_ATTACK;
        highlight_near_squares(unit_selected,0,1);
        hide_menu();
        display_selector();
        start_turn(CPU);
        break;
      }
    }
    else if(selector_mode == SPLIT_MODE)
    {
      if(battle_grid[abs] == 0 && square_adjacent(unit_selected,abs))
      {
        split_unit(abs,unit_selected);
        selector_mode = SELECT_MODE;
        update_map();
      }
    }
    else if(selector_mode == MERGE_MODE)
    {
      if(entities[battle_grid[abs]-1].team == entities[battle_grid[unit_selected]-1].team
         && entities[battle_grid[abs]-1].unit_type == entities[battle_grid[unit_selected]-1].unit_type
         && !entities[battle_grid[unit_selected]-1].is_cmdr
       )
      {
        merge_units(abs,unit_selected);
        selector_mode = SELECT_MODE;
        update_map();
      }
      else
      {
        display_error_message("invalid target");
      }
    }
  }

  if(j & JOY_II)
  {
    if(selector_mode == PLACE_MODE)
    {
      s_x = abs_x_from_graph(unit_selected);
      s_y = abs_y_from_graph(unit_selected);
      highlight_near_squares(unit_selected,0,3);
      spr_set(0);
      spr_x(s_x);
      spr_y(s_y);
      selector_mode = MENU_MODE;
      menu_option = MENU_ATTACK;
      display_menu();
      display_selector();
    }
    else if(selector_mode == MENU_MODE)
    {
      // hide_unit_info();
      hide_menu();
      highlight_near_squares(unit_selected,0,1);
      selector_mode = SELECT_MODE;
      display_selector();
    }
    else if(selector_mode == SPLIT_MODE || selector_mode == ATTACK_MODE || selector_mode == MERGE_MODE)
    {
      s_x = abs_x_from_graph(unit_selected);
      s_y = abs_y_from_graph(unit_selected);
      highlight_near_squares(unit_selected,0,1);
      spr_set(0);
      spr_x(s_x);
      spr_y(s_y);
      selector_mode = MENU_MODE;
      display_menu();
      display_selector();
    }
  }

  if(j & JOY_RUN)
  {
    actions = ACTIONS_PER_TURN;
    draw_actions();
  }
}

// void attack_unit(int src, int dst)
// {
//   char attacker, target;
//   attacker = battle_grid[dst]-1;
//   target = battle_grid[src]-1;
//   if(entities[attacker].team != entities[target].team)
//   {
//     hide_menu();
//     menu_x = -64;
//     menu_y = -64;
//     cursor_x = -32;
//     cursor_y = -32;
//     begin_battle(attacker,target,src,dst);
//     selector_mode = SELECT_MODE;
//   }
//   else
//   {
//     display_error_message("invalid target");
//   }
// }

void split_unit(int src, int dst)
{
  char pal;
  int entity_id;
  entity_id = battle_grid[dst]-1;
  reduce_army_size(entity_id,15);
  highlight_near_squares(dst,0,1);
  pal = entities[entity_id].unit_type + 16;
  add_entity(entities[entity_id].unit_type,15,entities[entity_id].team,pal,entities[entity_id].unit,0,entities[entity_id].id,src);
  battle_grid[src] = num_of_entities;
  deduct_actions(SPLIT_COST);
  if(entities[entity_id].team == PLAYER)
  {
    one_total++;
  }else
  {
    two_total++;
  }
}

void merge_units(int src, int dst)
{
  //units total needs to be <= 45, has to be same unit, cannot commanders into basic units
  char src_id, dst_id;
  src_id = battle_grid[src]-1;
  dst_id = battle_grid[dst]-1;
  if((entities[src_id].unit_type == entities[dst_id].unit_type) && (entities[src_id].army_size + entities[dst_id].army_size <= 45))
  {
    entities[src_id].army_size += entities[dst_id].army_size;
    remove_unit_from_grid(dst);
    highlight_near_squares(dst,0,1);
    deduct_actions(MERGE_COST);
    if(entities[src_id].team == PLAYER)
    {
      one_total--;
    }else
    {
      two_total--;
    }
  }
}

void selector_left()
{
  if(s_x > 0)
  {
    if(selector_mode == PLACE_MODE)
    {
      // if((valid_distance(-16,0,actions/MOVE_COST) && battle_grid[graph_from_x_y(s_x-16,s_y)] == 0) || battle_grid[graph_from_x_y(s_x-16,s_y)] == id)
      if(check_neighbors(graph_from_x_y(s_x-16,s_y),3,0) && is_traversable(graph_from_x_y(s_x-16,s_y)))
      {
        calc_move_cost(unit_selected,graph_from_x_y(s_x-16,s_y));
        update_selector_pos(-16,0);
      }
    }
    else if(selector_mode == SPLIT_MODE || selector_mode == ATTACK_MODE || selector_mode == MERGE_MODE)
    {
      if(valid_distance(-16,0,1) && is_traversable(graph_from_x_y(s_x-16,s_y)))
      {
        update_selector_pos(-16,0);
      }
    }
    else
    {
      update_selector_pos(-16,0);
    }
  }
}

void selector_right()
{
  if(s_x < 256 - 16)
  {
    if(selector_mode == PLACE_MODE)
    {
      // if((valid_distance(16,0,actions/MOVE_COST) && battle_grid[graph_from_x_y(s_x+16,s_y)] == 0) || battle_grid[graph_from_x_y(s_x+16,s_y)] == id) //WORKS IF OTHER DIRECTIONS ARE DOING THIS TOO
      if(check_neighbors(graph_from_x_y(s_x+16,s_y),3,0) && is_traversable(graph_from_x_y(s_x+16,s_y)))
      {
        calc_move_cost(unit_selected,graph_from_x_y(s_x+16,s_y));
        update_selector_pos(16,0);
      }
    }
    else if(selector_mode == SPLIT_MODE || selector_mode == ATTACK_MODE || selector_mode == MERGE_MODE)
    {
      if(valid_distance(16,0,1) && is_traversable(graph_from_x_y(s_x+16,s_y)))
      {
        update_selector_pos(16,0);
      }
    }
    else
    {
      update_selector_pos(16,0);
    }
  }
}

void selector_down()
{
  if(s_y < 224 - 16)
  {
    if(selector_mode == PLACE_MODE)
    {
      // if((valid_distance(0,16,actions/MOVE_COST) && battle_grid[graph_from_x_y(s_x,s_y+16)] == 0) || battle_grid[graph_from_x_y(s_x,s_y+16)] == id) //WORKS IF OTHER DIRECTIONS ARE DOING THIS TOO
      if(check_neighbors(graph_from_x_y(s_x,s_y+16),3,0) && is_traversable(graph_from_x_y(s_x,s_y+16)))
      {
        calc_move_cost(unit_selected,graph_from_x_y(s_x,s_y+16));
        update_selector_pos(0,16);
      }
    }
    else if(selector_mode == SPLIT_MODE || selector_mode == ATTACK_MODE || selector_mode == MERGE_MODE)
    {
      if(valid_distance(0,16,1) && is_traversable(graph_from_x_y(s_x,s_y+16)))
      {
        update_selector_pos(0,16);
      }
    }
    else
    {
      update_selector_pos(0,16);
    }
  }
}

void selector_up()
{
  if(s_y > 0)
  {
    if(selector_mode == PLACE_MODE)
    {
      // if((valid_distance(0,-16,actions/MOVE_COST) && battle_grid[graph_from_x_y(s_x,s_y-16)] == 0) || battle_grid[graph_from_x_y(s_x,s_y-16)] == id)
      if(check_neighbors(graph_from_x_y(s_x,s_y-16),3,0) && is_traversable(graph_from_x_y(s_x,s_y-16)))
      {
        calc_move_cost(unit_selected,graph_from_x_y(s_x,s_y-16));
        update_selector_pos(0,-16);
      }
    }
    else if(selector_mode == SPLIT_MODE || selector_mode == ATTACK_MODE || selector_mode == MERGE_MODE)
    {
      if(valid_distance(0,-16,1) && is_traversable(graph_from_x_y(s_x,s_y-16)))
      {
        update_selector_pos(0,-16);
      }
    }
    else
    {
      update_selector_pos(0,-16);
    }
  }
}

char valid_distance(int x, int y, int dist)
{
    int desired_x;
    int desired_y;
    int total;
    desired_x = (s_x + x) / 16;
    desired_y = (s_y + y) / 16;
    total = (abs(abs_x_from_graph(unit_selected)/16 - desired_x)) + (abs((abs_y_from_graph(unit_selected))/16 - desired_y));
    if(total <= dist)
    {
      return 1;
    }
    return 0;
}

int abs_x_from_graph(int pos)
{
  return graph_x_from_abs(pos)*16;
}

int abs_y_from_graph(int pos)
{
  return (graph_y_from_abs(pos)*16);
}

int graph_x_from_abs(int pos)
{
  return (pos % BF_X_TILES);
}

int graph_y_from_abs(int pos)
{
  return (pos / TILE_SIZE);
}

int graph_from_x_y(int x, int y)
{
  // put_number(x,3,1,1);
  // put_number(y,3,1,2);
  return (x/TILE_SIZE)+((y/TILE_SIZE)*TILE_SIZE);
}

void display_menu()
{
  // spr_set(2);
  if(s_x < 224)
  {
    menu_x = s_x + 16;
  }
  else
  {
    menu_x = s_x - 32;
  }
  if(s_y < 160)
  {
    menu_y = s_y + 16;
  }
  else
  {
    menu_y = s_y - 64;
  }
  menu_option = MENU_ATTACK;
  spr_set(2);
  spr_y(menu_y);
  spr_x(menu_x);
  spr_set(3);
  spr_y(menu_y);
  spr_x(menu_x+32);
  cursor_x = menu_x + 24;
  cursor_y = menu_y + 9;
  spr_set(1);
  spr_y(cursor_y);
  spr_x(cursor_x);
}

void reduce_army_size(char entity_id, char size)
{
  entities[entity_id].army_size -= size;
}

void highlight_near_squares(int square, int pal, char distance)
{
  if(distance)
  {
    if(square%16 > 0){//left
      if(battle_grid[(square)-1] == 0 && is_traversable(square-1))
      {
        change_background_palette(square-1,pal);
        highlight_near_squares(square-1,pal,distance-1);
      }
      else
      {
        // change_background_palette(square-1,pal);
      }
    }
    if(square%16 < 15)
    {
      if(battle_grid[(square)+1] == 0 && is_traversable(square+1))
      {
        change_background_palette(square+1,pal);
        highlight_near_squares(square+1,pal,distance-1);
      }
      else
      {
        // change_background_palette(square+1,pal);
      }
    }
    if(square/16 > 2)
    {
      if(battle_grid[(square)-16] == 0 && is_traversable(square-16))
      {
        change_background_palette(square-16,pal);
        highlight_near_squares(square-16,pal,distance-1);
      }
      else
      {
        // change_background_palette(square-16,pal);
      }
    }
    if(square/16 < 13)
    {
      if(battle_grid[(square)+16] == 0 && is_traversable(square+16))
      {
        change_background_palette(square+16,pal);
        highlight_near_squares(square+16,pal,distance-1);
      }
      else
      {
        // change_background_palette(square+16,pal);
      }
    }
  }
}

char check_neighbors(int square, int distance, int c)
{
  char result;
  result = 0;
  if(distance >= 0)
  {
    if(battle_grid[square] == id){ g = 0; return 1; }

    if((square)%16 > 0 && (battle_grid[(square)-1] == 0 || battle_grid[(square)-1] == id)){ result |= check_neighbors(square-1,distance-1, c+1); }
    if((square)%16 < 15 && (battle_grid[(square)+1] == 0 || battle_grid[(square)+1] == id)){ result |= check_neighbors(square+1,distance-1, c+1); }
    if((square)/16 > 2 && (battle_grid[(square)-16] == 0 || battle_grid[(square)-16] == id)){ result |= check_neighbors(square-16,distance-1, c+1); }
    if((square)/16 < 13 && (battle_grid[(square)+16] == 0 || battle_grid[(square)+16] == id)){ result |= check_neighbors(square+16,distance-1, c+1); }
  }
  return result;
}

char square_adjacent(int origin, int desired)
{
  int absolute;
  absolute = abs(origin - desired);

  if(absolute == 16) { return 1; }
  if(absolute == 1)
  {
    if(origin > desired) //moving left
    {
      if(desired%16 != 15)
      {
        return 1;
      }
    }
    else //moving right
    {
      if(desired%16 != 0)
      {
        return 1;
      }
    }
  }
    return 0;
}

void display_selector_pos()
{
  put_string("x:",1,1);
  put_number(s_x,3,3,1);
  put_string("y:",1,2);
  put_number(s_y,3,3,2);
}

void display_actions()
{
  put_string("actions:",10,1);
  put_number(actions,2,18,1);
}

void display_cost()
{
  put_string("cost:",10,2);
  put_number(cost,2,15,2);
  // actions -= (cost*2);
}

void display_abs()
{
  put_string("abs:",5,1);
  put_number(graph_from_x_y(s_x,s_y),3,10,1);
}

void begin_battle(char attacker, char target, int attacker_pos, int target_pos)
{
  char i, result, attacker_before, attacker_after, target_before, target_after;
  deduct_actions(ATTACK_COST);
  satb_update();
  disp_off();
  vsync();
  for(i=0; i<64; i++)
  {
    spr_hide(i);
  }
  cls();
  satb_update();
  reset_satb();
  vsync();

  attacker_before = entities[attacker].army_size;
  target_before = entities[target].army_size;

  result = battle_loop(attacker,target);

  attacker_after = entities[attacker].army_size;
  target_after = entities[target].army_size;

  commanders[entities[attacker].id].army_size -= (attacker_before - attacker_after);
  commanders[entities[target].id].army_size -= (target_before - target_after);
  switch(result)
  {
    case 0:
    //put_number(attacker,1,5,1); //unit_selected
    destroy_entity(target_pos);
    break;

    case 1:
    //put_number(target,1,5,1); //abs
    destroy_entity(attacker_pos);
    break;

    case 2: //nothing
    // put_number(8,1,5,1);
    break;

    default:
    // put_number(9,1,5,1);
    break;
  }

  load_battlefield_map();
  load_sprites_();

  // load_vram(0x1000, panel_gfx, 2048);
  // set_font_color(3,15);
  // set_font_pal(0);
  // load_default_font(0,0x800);
  // put_number(num_of_entities,3,5,2);
  update_map();
  selector_mode = 0;
  disp_on();
}

void cleanup_battlefield()
{
  int i;
  disp_off();

  for(i=0; i<MAP_SIZE; i++)
  {
    battle_grid[i] = 0;
  }
  for(i=0; i<MAX_ENTITIES; i++)
  {
    // entities[i]->unit = 0;
    entities[i].unit_type = 0;
    entities[i].army_size = 0;
    entities[i].is_cmdr = 0;
    entities[i].team = 0;
    entities[i].pal = 0;
    entities[i].id = 0;
  }
  for(i=0; i<mapper_size; i++)
  {
    cmdr_vram_mapper[i].id = 0;
    cmdr_vram_mapper[i].address = 0;
  }

  for(i=0; i<64; i++)
  {
    spr_hide(i);
  }
  satb_update();
  mapper_size = 0;
  size_one = 0;
  size_two = 0;
  one_total = 0;
  two_total = 0;
  army_one = 0;
  army_two = 0;
  s_y = -1 * y_offset; //why did I decide to use s_y and s_x varibles here???
  s_x = -1 * x_offset;

  reset_satb();
  // satb_update();
  cls();
}
