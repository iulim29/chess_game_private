#include <stdio.h>
#include <stdlib.h>
#include "chess_models.h"
#include "constants.h"
#include <windows.h>
#include <string.h>
#include "menu.h"

#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif




int line = 196;
pixel screen[game_height][game_width];
int width = 7;// width = height * 2.5 because the width of char in pixels is about half of the height
int height = 3;


piece **chess_table;

char *Get_TextMenu(size_t line_size, FILE *game_menu);
char *Get_MenuOption(size_t line_size, FILE *menu_options);
char *Player_one_turn(size_t line_size, FILE *player_1_turn);
char *Player_two_turn(size_t line_size, FILE *player_2_turn);

void print_square(int, int, int color, piece chess_piece, int column, int row);



/*the function has to refresh the screen in order to render the game
left_margin- shifts to right the whole game
top_margin - shifts to bottom the whole game
*/
void render(int left_margin, int top_margin, HANDLE  hConsole){
    int i, j, k;
    for(i = 0; i < top_margin; i++){
        printf("\n");
    }
    for(i = 0; i < game_height; i++){
        // applies the left margin for each row


        for(k = 0; k < left_margin; k++){
            printf(" ");
        }
        //prints the game content
        for(j = 0; j < game_width; j++){
            SetConsoleTextAttribute(hConsole, screen[i][j].color);
            printf("%c", screen[i][j].pixel_display);
        }
        printf("\n");
    }
    SetConsoleTextAttribute(hConsole, white_color);
}

/*color - sets the color of the square
  chess_piece - sets the chess piece in the current square
  column - sets the table coordinates of the square on the width axis
  row - same as column but on the height axis
*/
void print_square(int left_margin, int top_margin, int color, piece chess_piece, int column, int row){
    int i, j;
    pixel a_pixel;
    for (i = 0; i < height; i++){
        for(j = 0; j < width; j++){
            if(i == height / 2 && j == width / 2 && chess_piece.piece_display != 0){
                if((row % 2 == 0 && column % 2 == 0) || (row % 2 != 0 && column % 2 != 0)){
                    a_pixel.color = chess_piece.square1_color;
                }else {
                    a_pixel.color = chess_piece.square2_color;
                }
                a_pixel.pixel_display = chess_piece.piece_display;
            } else {
                a_pixel.color = color;
                a_pixel.pixel_display = ' ';
            }
            screen[row * height + i + top_margin][column * width + j + left_margin] = a_pixel;
        }
    }
}

void print_row_label(int row, int top_margin){
    pixel a_pixel;
    a_pixel.color = white_color;
    a_pixel.pixel_display = 49 + row;
    screen[height / 2 + row * height + top_margin][0] = a_pixel;
}

void print_column_label(int column, int left_margin){
    pixel a_pixel;
    a_pixel.color = white_color;
    a_pixel.pixel_display = 97 +column;
    screen[0][width / 2 + column * width + left_margin] = a_pixel;
}

void print_table(piece** table){
    int i, j;
    int top_margin = 2;
    int left_margin = 4;
    for(i = 0; i < 8; i++){
        print_row_label(i, top_margin);
        for(j = 0; j < 8; j++) {
            print_column_label(j, left_margin);
            if((i % 2 == 0 && j % 2 == 0) || (i % 2 != 0 && j % 2 != 0)){
                print_square(left_margin, top_margin, white_background, table[i][j], j, i);
            } else{
                print_square(left_margin, top_margin, 0, table[i][j], j, i);
            }
        }
    }
}

void print_move(piece** table, char *move){
    move[0] = move[0] - 97;
    move[1] = move[1] - 49;
    move[2] = move[2] - 97;
    move[3] = move[3] - 49;
    table[(int)move[3]][(int)move[2]] = table[(int)move[1]][(int)move[0]];
    table[(int)move[1]][(int)move[0]].piece_display = 0;

}


void print_txt(char *txt, int left_margin, int top_margin) {
    int i, j = 0, k = 0;

    for(i = 0; txt[k] != NULL; j++, k++){
        screen[top_margin + i][left_margin + j].color = white_color;
        screen[top_margin + i][left_margin + j].pixel_display = txt[k];
        if(screen[top_margin + i][left_margin + j].pixel_display == '\n'){
           screen[top_margin + i][left_margin + j].pixel_display = NULL;
            i++;
            j = 0;
        }
        if(j + left_margin == 0){
            if(screen[i][0].pixel_display != ' '){
                while(screen[i][ j].pixel_display != ' '){
                    screen[i][j] .pixel_display  = ' ';
                    j--;
                    k--;
                }
            }
            j = 0;
            i++;
        }
    }
}


void print_menu(menu game_txt, int left_margin, int top_margin){
   print_txt(game_txt.menu_txt, left_margin, 0);
   print_txt(game_txt.under_table_txt, 1 , top_margin);
}

/*
https://docs.microsoft.com/en-us/windows/console/clearing-the-screen
*/
void cls(HANDLE hConsole)
{
    COORD coordScreen = { 0, 0 };    // home for the cursor
    DWORD cCharsWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD dwConSize;

    // Get the number of character cells in the current buffer.
    if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
    {
        return;
    }

    dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

    // Fill the entire screen with blanks.
    if (!FillConsoleOutputCharacter(hConsole,        // Handle to console screen buffer
                                    (TCHAR)' ',      // Character to write to the buffer
                                    dwConSize,       // Number of cells to write
                                    coordScreen,     // Coordinates of first cell
                                    &cCharsWritten)) // Receive number of characters written
    {
        return;
    }

    // Get the current text attribute.
    if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
    {
        return;
    }

    // Set the buffer's attributes accordingly.
    if (!FillConsoleOutputAttribute(hConsole,         // Handle to console screen buffer
                                    csbi.wAttributes, // Character attributes to use
                                    dwConSize,        // Number of cells to set attribute
                                    coordScreen,      // Coordinates of first cell
                                    &cCharsWritten))  // Receive number of characters written
    {
        return;
    }

    // Put the cursor at its home coordinates.
    SetConsoleCursorPosition(hConsole, coordScreen);
}



char *Get_MenuOption(size_t line_size, FILE *menu_options){
    char *text_lung =(char *)malloc( 100 * sizeof (char));
    char *memory_option = (char *)calloc(100, sizeof(char));
    if(menu_options == NULL){
        printf("Problem when displaying the desired text");
        exit(1);
    }

    while(fgets(text_lung, line_size, menu_options)!= NULL){
         strcat(memory_option, text_lung);
    }

    return memory_option;
}

char *Player_one_turn(size_t line_size, FILE *player_1_turn){

    char *text_lung1 =(char *)malloc( 100 * sizeof (char));
    char *memory_player1 = (char *)calloc(100, sizeof(char));
    if(player_1_turn == NULL){
        printf("Problem when displaying the desired text");
        exit(1);
    }

    while(fgets(text_lung1, line_size, player_1_turn)!= NULL){

         strcat(memory_player1, text_lung1);


    }
    return memory_player1;

}

char *Player_two_turn(size_t line_size, FILE *player_2_turn){

    char *text_lung =(char *)malloc( 100 * sizeof (char));
    char *memory_player2 = (char *)calloc(100, sizeof(char));
    if(player_2_turn == NULL){
        printf("Problem when displaying the desired text");
        exit(1);
    }

    while(fgets(text_lung, line_size, player_2_turn)!= NULL){

         strcat(memory_player2, text_lung);


    }
    return memory_player2;

}

menu *menu_choice(menu current_menu, char *action){
    if(strcmp(action, current_menu.exit_option) == 0){
        if(current_menu.previous_menu == NULL){
            exit(1);
        }
        return current_menu.previous_menu;
    }
    for(int i = 0; i < current_menu.sub_menu_size; i++){
        if(strcmp(current_menu.submenu[i].unique_action, action) == 0){
            return &current_menu.submenu[i];
        }
    }
    return &current_menu;
}

int main()
{
    char *memory_player1;
    char *memory_player2;
    char action[10];
    char move[10] = "play";
    int t = 0;
    chess_table = get_ChessTable();
    menu main_menu;
    main_menu = Get_Main_Menu("game_menu.txt", "main_menu_sub_title.txt");



    FILE *player_1_turn = fopen("player_1_turn.txt", "r");
    FILE *player_2_turn = fopen("player_2_turn.txt", "r");


    memory_player1 = Player_one_turn(line_si, player_1_turn);
    memory_player2 = Player_two_turn(line_si, player_2_turn);


    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    print_table(chess_table);
    print_menu(main_menu, 64, 28);
    //print_menu(memory_option, 4, 28);
    render(7, 3, hConsole);
    while(strcmp(action, "quit") != 0){

    scanf("%s", action);
    while(strcmp(action, "play") == 0){
        if(t == 0){
            print_table(chess_table);
            print_menu(main_menu, 64, 28);
            cls(hConsole);
            render(7, 3, hConsole);
            scanf("%s", move);
            print_move(chess_table, move);
            t = 1;
            }
        else if(t == 1){
            print_table(chess_table);
            print_menu(main_menu, 64, 28);
            cls(hConsole);
            render(7, 3, hConsole);
            scanf("%s", move);
            print_move(chess_table, move);
            t = 0;
            }
        }

    print_table(chess_table);
    print_menu(main_menu, 64, 28);

    cls(hConsole);
    render(7, 3, hConsole);

    }




    return 0;
}

