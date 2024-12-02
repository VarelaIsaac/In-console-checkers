#pragma once
#include <iostream>
#include <vector>
#include <iomanip> //needed for std::setw
#include "Move.h"


/* se cambio namespace por std::*/
using std::vector;
using std::ostream;
using std::cout;
using std::endl;
using std::setw;
using std::fixed;
using std::string;

const int N = 8;
enum pieces { WHITE = 1, BLACK, WHITE_KING, BLACK_KING };

extern vector<Field> color_fields_print;


// colors to print board
enum colors {
	color_white_field = 9,
	color_black_field = 1,
	color_black_pawn = 0,
	color_white_pawn = 15,
	color_move = 13,
	color_move_possibility = 4
};

extern const int draw_after_moves;  // Declaración externa
extern const bool turn_board;      // Declaración externa

class Board
{
	int board[N*N/2]; // it has 32 squares, only black squares
	int out_of_board = -1;

public:

	int moves_with_kings = 0;
	
	Board();
	Board(int board[N*N]);
	Board(int board[N][N]);
	Board(const Board&);

	bool static is_it_black_field(int i, int j) { return (i + j) % 2 == 0; }; // checks if cordinates given are pointing for black field
	bool is_black(int i, int j); // is pawn on this field black
	bool is_white(int i, int j); // is pawn on this field white

	int& operator()(int i, int j); // it access board. numbers i,j are cordinates of board and it returns board field that stores that field
	int& operator()(Field); // like opetator (int, int) but you give field
	int& operator[](string); // access to board for exmple "b2"
	int operator()(int i, int j) const;  // Agregar const aquí

	friend std::ostream& operator<<(std::ostream& out, const Board& p);  // Cambiado a const Board&

};

