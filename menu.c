void load_box(int x, int y, int m_size, int m_length, int location)
{
	int i, num_of_sprites;
	i = 0; num_of_sprites = total_sprites;
	for(i=0; i<m_size;i++,num_of_sprites++)
	{
		display_sprite(x + ((i%m_length)*16),y + ((i/m_length)*16),location+(i*0x40),FLIP_MAS|SIZE_MAS,4);
	}
}

void load_menu(int m_size, int m_length, int x, int y)
{
	current_menu_size = m_size;
	load_box(x,y,m_size,m_length,0x5C00);
	spr_set(total_sprites - m_size - CURSOR_SIZE);
}

void load_prebattle_menu()
{
	current_menu = PRE_BATTLE_MENU;
	load_vram(0x5C00,pre,0x280);
	load_cursor(-2,2);
	load_menu(10,5,0,0);
}

void load_in_battle_menu()
{
	current_menu = BATTLE_MENU;
	load_vram(0x5C00,menu,0x500);
	load_cursor(-2,2);
	load_menu(20,5,0,0);
	resize_menu_choices(5);
}

void load_arts_menu()
{
	current_menu = ARTS_MENU;
	load_vram(0x5C00,artsmenu,0x500);
	load_cursor(-2,2);
	load_menu(15,5,0,0);
	resize_menu_choices(0);
}

void load_formation_menu()
{
	current_menu = 32;//FORMATION_MENU;
	load_vram(0x5C00,formation,0x500);
	load_cursor(-2,2);
	load_menu(15,5,0,0);
	resize_menu_choices(3);
}

void load_castle_menu(char k)
{
	current_menu = CASTLE_MENU;
	kingdom = k;
	load_vram(0x5C00,castlemenu,0x8C0);
	load_cursor(-2,2);
	load_menu(35,5,0,0);
	menu_loop(5);
}

void load_castle_info_menu()
{
	current_menu = CASTLE_INFO_MENU;
	load_vram(0x5C00,castleinfo,0x8C0);
	load_kingdom(kingdom);
	load_menu(35,7,0,0);
	resize_menu_choices(0);
	// menu_loop(0);
}

void resize_menu_choices(char choices)
{
	current_menu_choices = choices;
	menu_min = current_menu_choices * 16;
	menu_max = menu_min + 16;
}

// void load_portrait(char cmdr_id, int x, int y)
// {
// 	load_palette(17,portraitpal,1);
// 	switch(cmdr_id)
// 	{
// 		case CMDR:
// 		load_vram(PLAYER_PORTRAIT_VRAM,portrait1,0x200);
// 		break;
//
// 		case BEAST:
// 		load_vram(PLAYER_PORTRAIT_VRAM,portrait2,0x200);
// 		break;
//
// 		case SAMURAI:
// 		load_vram(PLAYER_PORTRAIT_VRAM,portrait3,0x200);
// 		break;
// 	}
// 	load_vram(P_BORDER_VRAM,pborder,0x240);
// 	load_palette(19,pborderpal,1);
//
// 	spr_make(48,x-8,y+8,PLAYER_PORTRAIT_VRAM,FLIP_MAS|SIZE_MAS,NO_FLIP|SZ_32x32,17,1);
// 	spr_make(49,x+32-8,y+8,PLAYER_PORTRAIT_VRAM+0x100,FLIP_MAS|SIZE_MAS,NO_FLIP|SZ_32x32,17,1);
//
// 	spr_make(50,x,y,P_BORDER_VRAM,FLIP_MAS|SIZE_MAS,NO_FLIP|SZ_16x16,19,1);
// 	spr_make(51,x+16,y,P_BORDER_VRAM+0x40,FLIP_MAS|SIZE_MAS,NO_FLIP|SZ_16x16,19,1);
// 	spr_make(52,x+32,y,P_BORDER_VRAM+0x80,FLIP_MAS|SIZE_MAS,NO_FLIP|SZ_16x16,19,1);
//
// 	spr_make(53,x,16+y,P_BORDER_VRAM+0xC0,FLIP_MAS|SIZE_MAS,NO_FLIP|SZ_16x16,19,1);
// 	spr_make(54,x+16,16+y,P_BORDER_VRAM+0x100,FLIP_MAS|SIZE_MAS,NO_FLIP|SZ_16x16,19,1);
// 	spr_make(55,x+32,16+y,P_BORDER_VRAM+0x140,FLIP_MAS|SIZE_MAS,NO_FLIP|SZ_16x16,19,1);
//
// 	spr_make(56,x,32+y,P_BORDER_VRAM+0x180,FLIP_MAS|SIZE_MAS,NO_FLIP|SZ_16x16,19,1);
// 	spr_make(57,x+16,32+y,P_BORDER_VRAM+0x1C0,FLIP_MAS|SIZE_MAS,NO_FLIP|SZ_16x16,19,1);
// 	spr_make(58,x+32,32+y,P_BORDER_VRAM+0x200,FLIP_MAS|SIZE_MAS,NO_FLIP|SZ_16x16,19,1);
//
// }

void display_dialogue_box(int x, int y, char sprite_offset)
{
	char i;
	for(i=0; i < 24; i++)
	{
		spr_make(sprite_offset+i,x+((i%6)*32),y+((i/6)*16),DIALOGUE_VRAM+(i*0x80),FLIP_MAS|SIZE_MAS,NO_FLIP|SZ_32x16,19,1);
	}
	// spr_make(4,0,32,DIALOGUE_VRAM,FLIP_MAS|SIZE_MAS,NO_FLIP|SZ_32x16,19,1);
	// spr_make(5,32,32,DIALOGUE_VRAM+0x80,FLIP_MAS|SIZE_MAS,NO_FLIP|SZ_32x16,19,1);
	// spr_make(6,64,32,DIALOGUE_VRAM+0x100,FLIP_MAS|SIZE_MAS,NO_FLIP|SZ_32x16,19,1);
	// spr_make(7,96,32,DIALOGUE_VRAM+0x180,FLIP_MAS|SIZE_MAS,NO_FLIP|SZ_32x16,19,1);
	// spr_make(8,128,32,DIALOGUE_VRAM+0x200,FLIP_MAS|SIZE_MAS,NO_FLIP|SZ_32x16,19,1);
	// spr_make(9,160,32,DIALOGUE_VRAM+0x280,FLIP_MAS|SIZE_MAS,NO_FLIP|SZ_32x16,19,1);
	// spr_make(10,0,64,DIALOGUE_VRAM+0x300,FLIP_MAS|SIZE_MAS,NO_FLIP|SZ_32x16,19,1);
}

// void load_dialog()
// {
// 	load_vram(0x5C00,borders,0x240);
// 	load_vram(0x5E40,dialog,0xC0);
// 	display_dialog();
// }

void load_kingdom(char kingdom)
{
	switch(kingdom)
	{
		case TAROS: load_vram(0x6040,taros,0xC0); break;
		case KAYTOKEN: load_vram(0x6040,kaytoken,0xC0); break;
		case SANGAI: load_vram(0x6040,sangai,0xC0); break;
		case KODEN: load_vram(0x6040,koden,0xC0); break;
		case BAKURIA: load_vram(0x6040,bakuria,0xC0); break;
	}
}

void menu_loop(char ms)
{
	char exit, selection, joy;
	// int select_y;
	// select_y = 0;
	exit = 1;
	resize_menu_choices(ms);

	while(exit)
	{
		vsync();
		joy = joytrg(0);

		if(joy & JOY_DOWN)
		{
			if(current_menu_choices)
			{
				// select_y = (spr_get_y() + 16) % menu_max;
				// spr_y(select_y+34);
				spr_y((spr_get_y() + 16) % menu_max);
			}
		}
		if(joy & JOY_UP)
		{
			if(current_menu_choices)
			{
				// select_y = (spr_get_y() + menu_min) % menu_max;
				// spr_y(select_y+34);
				spr_y((spr_get_y() + menu_min) % menu_max);
			}
		}
		if(joy & JOY_I)
		{
			selection = (spr_get_y() / 16);

			switch(current_menu)
			{
				case PRE_BATTLE_MENU:
				spr_hide(--total_sprites);
				// clear_menu(10);
				clear_menu();
				exit = prebattle_select(selection);
				break;

				case BATTLE_MENU:
				spr_hide(--total_sprites);
				// clear_menu(20);
				clear_menu();
				break;

				case FORMATION_MENU:
				spr_hide(--total_sprites);
				// clear_menu(15);
				clear_menu();
				exit = formation_select(selection);
				break;

				case ARTS_MENU:
				spr_hide(--total_sprites);
				// clear_menu(15);
				clear_menu();
				break;

				case CASTLE_MENU:
				spr_hide(--total_sprites);
				// clear_menu(35);
				clear_menu();
				exit = castle_menu_select(selection);
				break;
			}
		}
		if(joy & JOY_II)
		{
			switch(current_menu)
			{
				case BATTLE_MENU:
				spr_hide(--total_sprites);
				// clear_menu(20);
				clear_menu();
				exit = 0;
				break;

				case ARTS_MENU:
				spr_hide(--total_sprites);
				// clear_menu(15);
				clear_menu();
				load_in_battle_menu();
				break;

				case 3:
				spr_hide(--total_sprites);
				// clear_menu(15);
				clear_menu();
				load_prebattle_menu();
				menu_min = 2 * 8;
				menu_max = menu_min + 8;
				break;

				case CASTLE_MENU:
				spr_hide(--total_sprites);
				// clear_menu(35);
				clear_menu();
				exit = 0;
				break;

				case CASTLE_INFO_MENU:
				// clear_menu(35);
				clear_menu();
				exit = 0;
				break;
			}
		}
		// if(joy & JOY_RUN)
		// {
		// 	switch(current_menu)
		// 	{
		// 		case 1:
		// 		if(ms)
		// 		{
		// 			spr_hide(--total_sprites);
		// 		}
		// 		clear_menu(35);
		// 		exit = 0;
		// 		break;
		// 	}
		// }
		satb_update();
	}
}
