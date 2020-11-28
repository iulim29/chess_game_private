#include <stdlib.h>
#include "chess_models.h"
#include "constants.h"

piece get_Bishop(int color1, int color2){
    piece bishop;
    bishop.square1_color = color1;
    bishop.square2_color = color2;
    bishop.piece_display = 'B';
    return bishop;
}
piece get_Rook(int color1, int color2){
    piece rook;
    rook.square1_color = color1;
    rook.square2_color = color2;
    rook.piece_display = 'R';
    return rook;
}
piece get_Knight(int color1, int color2){
    piece knight;
    knight.square1_color = color1;
    knight.square2_color = color2;
    knight.piece_display = 'N';
    return knight;
}
piece get_King(int color1, int color2){
    piece king;
    king.square1_color = color1;
    king.square2_color = color2;
    king.piece_display = 'K';
    return king;
}
piece get_Queen(int color1, int color2){
    piece queen;
    queen.square1_color = color1;
    queen.square2_color = color2;
    queen.piece_display = 'Q';
    return queen;
}
piece get_Pawn(int color1, int color2){
    piece pawn;
    pawn.square1_color = color1;
    pawn.square2_color = color2;
    pawn.piece_display = 'P';
    return pawn;
}


piece** get_ChessTable(){
    int i;
    piece **chess_table = (piece **)malloc(8 * sizeof(piece *));
    for (i=0; i < 8; i++)
        chess_table[i] = (piece *)calloc(8, sizeof(piece));
    chess_table[0][2] = get_Bishop(player_1_color, player_1_2_color);
    chess_table[7][2] = get_Bishop(player_2_color, player_2_2_color);
    chess_table[0][5] = get_Bishop(player_1_color, player_1_2_color);
    chess_table[7][5] = get_Bishop(player_2_color, player_2_2_color);
    chess_table[0][0] = get_Rook(player_1_color, player_1_2_color);
    chess_table[0][7] = get_Rook(player_1_color, player_1_2_color);
    chess_table[7][0] = get_Rook(player_2_color, player_2_2_color);
    chess_table[7][7] = get_Rook(player_2_color, player_2_2_color);
    chess_table[0][1] = get_Knight(player_1_color, player_1_2_color);
    chess_table[0][6] = get_Knight(player_1_color, player_1_2_color);
    chess_table[7][1] = get_Knight(player_2_color, player_2_2_color);
    chess_table[7][6] = get_Knight(player_2_color, player_2_2_color);
    chess_table[0][3] = get_King(player_1_color, player_1_2_color);
    chess_table[7][4] = get_King(player_2_color, player_2_2_color);
    chess_table[0][4] = get_Queen(player_1_color, player_1_2_color);
    chess_table[7][3] = get_Queen(player_2_color, player_2_2_color);
    for(i = 0; i < 8; i++){
        chess_table[1][i] = get_Pawn(player_1_color, player_1_2_color);
        chess_table[6][i] = get_Pawn(player_2_color, player_2_2_color);
    }


    return chess_table;
}
