#define LIGHTENING_ART 1
#define FIRE_ART 2
#define ICE_ART 3

void load_art(char art_no, int x, int y)
{
  switch(art_no)
  {
    case LIGHTENING_ART:
    display_window(2,8,12,3);
    put_string("Lightening",3,9);
    create_lightening(x,y);
    break;
  }
}

void remove_art(char art_no)
{
  switch(art_no)
  {
    case LIGHTENING_ART:
    remove_lightening(0);
    break;
  }
}