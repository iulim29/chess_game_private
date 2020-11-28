#include <stdio.h>
#include <stdlib.h>
#include "chess_models.h"
#include "constants.h"
#include <windows.h>
#include <string.h>

#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif




int line = 196;
pixel screen[game_height][game_width];
int width = 7;// width = height * 2.5 because the width of char in pixels is about half of the height
int height = 3;


piece chess_table[8][8];

char *Get_TextMenu(size_t line_size, FILE *game_menu);

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



void print_menu(char *txt, int left_margin, int top_margin){
    int i, j = 0, k = 0;
    int limit = game_width - 1;
    for(i = 0; txt[k] != NULL; j++, k++){
        screen[i][left_margin + j].color = white_color;
        screen[i][left_margin + j].pixel_display = txt[k];
        if(screen[i][left_margin + j].pixel_display == '\n'){
           screen[i][left_margin + j].pixel_display = "";
            i++;
            j = 0;


        }
        if(j + left_margin == limit){
            if(screen[i][limit].pixel_display != ' '){
                while(screen[i][left_margin + j].pixel_display != ' '){
                    screen[i][left_margin + j] .pixel_display  = ' ';
                    j--;
                    k--;

                }
           // i++;
            }
            j = 0;
            i++;
        }


    }
}





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






int main()
{
    char *memory_file;
    char action[10];
    FILE *game_menu = fopen("game_menu.txt", "r");
    memory_file = Get_TextMenu(line_si, game_menu);
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    print_table(get_ChessTable());
    print_menu(memory_file, 64, 7);
    render(7, 3, hConsole);
    while(strcmp(action, "quit") != 0){

    scanf("%s", action);
    print_table(get_ChessTable());
    print_menu(memory_file, 64, 7);
//    DWORD mode = 0;
//    if (!GetConsoleMode(hConsole, &mode))
//    {
//        return -1;
//    }
//
//    // Hold original mode to restore on exit to be cooperative with other command-line apps.
//    const DWORD originalMode = mode;
//    mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
//
//    // Try to set the mode.
//    if (!SetConsoleMode(hConsole, mode))
//    {
//        return -1;
//    }
//
//    // Write the sequence for clearing the display.
//    DWORD written = 0;
//    PCWSTR sequence = L"\x1b[2J";
//    if (!WriteConsoleW(hConsole, sequence, ARRAYSIZE(sequence), &written, NULL))
//    {
//        // If we fail, try to restore the mode on the way out.
//        SetConsoleMode(hConsole, originalMode);
//        return -1;
//    }
//
//    // To also clear the scroll back, emit L"\x1b[3J" as well.
//    // 2J only clears the visible window and 3J only clears the scroll back.
//
//    // Restore the mode on the way out to be nice to other command-line applications.
//    SetConsoleMode(hConsole, originalMode);
    cls(hConsole);
    render(7, 3, hConsole);

    }




    return 0;
}


char *Get_TextMenu(size_t line_size, FILE *game_menu){

    char *text_lung =(char *)malloc( 100 * sizeof (char));
    char *memory_file = (char *)calloc(100, sizeof(char));
    if(game_menu == NULL){
        printf("Huston, we have a problem");
        exit(1);
    }

    while(fgets(text_lung,line_size,game_menu)!= NULL){

         strcat(memory_file, text_lung);


    }
    return memory_file;
}


