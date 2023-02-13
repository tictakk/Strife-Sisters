void execute_calling(char cmdr_id)
{
}

void play_calling_scene()
{
  
}

void heal_calling(char cmdr_id)
{
  char i;
  for(i=0; i<9; i++)
  {
    if(party_commanders[cmdr_id].bg.units[i].unit->id)
    {
      // put_number(party_commanders[cmdr_id].bg.units[i],3,0,i);
      party_commanders[cmdr_id].bg.units[i].hp = party_commanders[cmdr_id].bg.units[i].unit->hp;
    }
  }
}