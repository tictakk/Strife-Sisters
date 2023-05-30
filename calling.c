#include "calling_stone.c"
//12? Calling stones, player must collect them throughought the game.
//Maybe theres a certain order the player must use the stones in to beat the
//final boss

char active_player_calling = CALLING_NONE;
char active_cpu_calling = CALLING_NONE;

void set_calling(char calling_id, char team)
{
  if(team == PLAYER)
  {
    active_player_calling = calling_id;
  }
  else
  {
    active_cpu_calling = calling_id;
  }
}

void reset_calling(char team)
{
  set_calling(CALLING_NONE,team);
}

void display_calling_background(int x, int y, char cmdr_id)
{
  scroll(0,0,y,32,224,0x80);
  display_abs_black_panel(0,(y/8)+4,32,7);
  load_portrait(party_commanders[cmdr_id].sprite_type,0);
  display_item(0,0,7,(y/8)+4);
  print_calling_name(party_commanders[cmdr_id].bg.calling_stone,11,10);
  sync(120);
  load_map(0,2,0,0,16,29);
  scroll(0,0,y,32,224,0xC0);
}

void print_calling_name(char calling_id, char x, char y)
{
  put_string(calling_stones[calling_id].name,x,y+(s_y/8));
}