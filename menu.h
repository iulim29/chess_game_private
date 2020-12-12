#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

typedef struct Menu{
    char *menu_txt;
    struct Menu *submenu;
    struct Menu *previous_menu;
    char *unique_action;
    char *exit_option;
    char *under_table_txt;
    int sub_menu_size;
}menu;

menu Get_Main_Menu(char *menu_txt, char *main_menu_sub_title);

#endif // MENU_H_INCLUDED
