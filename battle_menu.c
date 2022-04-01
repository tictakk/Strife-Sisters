// #include "battle.h"
//
// #define B_LEFT 2
// #define B_RIGHT 3
// #define B_CENTER 1
// #define B_L_RESERVE 4
// #define B_R_RESERVE 15
//
// struct Menu *b_menu;
// char cur_pos = 0;
// char branch = 0;
// char cursor_offset = 12;
//
// void display_menu(struct Menu *m)
// {
//   char i;
//   c_menu();
//   b_menu = m;
//   put_string(b_menu->title,14,0);
//   for(i=0; i<(b_menu->size); i++)
//   {
//     put_string(menu_items[b_menu->item_id[i]].text,13,2+i);
//   }
//   cur_pos = 0;
//   update_cursor();
// }
//
// void update_cursor()
// {
//   spr_set(0);
//   cur_pos%=b_menu->size;
//   spr_y(cursor_offset + (cur_pos * 8));
// }
//
// void select()
// {
//   char sel;
//   switch(menu_items[b_menu->item_id[cur_pos]].action)
//   {
//     case NEW_MENU:
//     new_menu();
//     break;
//
//     case REINFORCE:
//     sel = menu_items[b_menu->item_id[cur_pos]].command;
//     if(sel == LEFT_M)
//     {
//       reinforce(army_one_current_size,&p_left_reserve_size,1);
//     }
//     else
//     {
//       reinforce(army_one_current_size,&p_right_reserve_size,1);
//     }
//     break;
//
//
//     default:
//     break;
//   }
// }
//
// void back()
// {
//   c_menu();
//   display_menu(&menus[b_menu->parent]);
// }
//
// void new_menu()
// {
//   char sel, parent;
//   sel = menu_items[b_menu->item_id[cur_pos]].command;
//   parent = b_menu->id;
//   menus[sel].parent = parent;
//   if(parent == 0)
//   {
//     branch = menus[sel].id;
//   }
//   display_menu(&menus[sel]);
// }
//
// void c_menu()
// {
//   int p[1];
//   int vaddr;
//   char i, j;
//
//   p[0] = (0x6400 >> 4) + 0x1000;
//
//   for(i=0; i<8; i++)
//   {
//     for(j=0; j<8; j++)
//     {
//       vaddr = ((32*i)+12)+j;
//       load_vram(vaddr,p,1);
//     }
//   }
// }
