#ifndef CHESS_MODELS_H_INCLUDED
#define CHESS_MODELS_H_INCLUDED

typedef struct Piece{
    int column, row;
    int square1_color, square2_color;
    char piece_display;
}piece;

typedef struct Pixel{
    int color;
    char pixel_display;

}pixel;

typedef struct PixelArray{
    int length;
    pixel *pixel_data;
}pixel_array;




piece** get_ChessTable();

#endif // CHESS_MODELS_H_INCLUDED
