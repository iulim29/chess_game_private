#include<stdio.h>
#include "menu.h"
#include "constants.h"

char *Get_TextMenu(size_t line_size, FILE *game_menu){
    char *text_lung =(char *)malloc( 100 * sizeof (char));
    char *memory_file = (char *)calloc(100, sizeof(char));
    if(game_menu == NULL){
        printf("Huston, we have a problem");
        exit(1);
    }

    while(fgets(text_lung, line_size, game_menu)!= NULL){
         strcat(memory_file, text_lung);
    }

    return memory_file;
}

menu Get_Main_Menu(char *menu_txt, char *main_menu_sub_title){
    FILE *game_menu = fopen(menu_txt, "r");
    FILE *sub_title = fopen(main_menu_sub_title, "r");

    menu *main_menu =(menu *)malloc(sizeof(menu));
    main_menu->menu_txt = Get_TextMenu(line_si, game_menu);
    main_menu->under_table_txt = Get_TextMenu(line_si, sub_title);

    return *main_menu;
}

menu Get_Sub_Menu(char *menu_txt){
    char *memory_file;
    FILE *game_menu = fopen(menu_txt, "r");
    memory_file = Get_TextMenu(line_si, game_menu);
    menu *main_menu =(menu *)malloc(sizeof(menu));
    main_menu->menu_txt = memory_file;
    return *main_menu;
}
