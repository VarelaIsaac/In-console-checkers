#include <algorithm>
#include <iostream>
#include <list>
#include <thread>
#include <chrono>
#include <sstream>
#include <string>

#include "Board.h"
#include "Move.h"
#include "Pawn.h"
#include "Bot.h"

using std::list;
using std::thread;
using std::cout;
using std::endl;
using std::string;
using std::ostream;
using std::getline;
using std::cin;

// Constants
const int MAX_DEPTH = 8; // Depth for bot evaluation
const int DRAW_AFTER_MOVES = 10; // After how many moves with kings there will be a draw
const bool TURN_BOARD = false; // If false, board is printed with white on the bottom

// Function declarations
void move_player(Board&, int);
void move_computer(Board&, Bot&);
bool is_end_of_game(Board&, int who_move);
Move read_move(Board&, list<Move>& possible_moves);
int num(string number);
string pretty_number(int n);
void print_winner(const string& winner);
void print_game_state(const Board& b, Bot& bot);

int main()
{
    bool start_position = true;
    int who_move = WHITE;
    int player_color = WHITE;
    int bot_color = BLACK;

    Bot bot(bot_color);
    Board b;

    // Initialize the board
    if (start_position)
    {
        b = Board();
    }
    else
    {
        int custom_board[64] = { 0 };
        b = Board(custom_board);

        // Custom setup of pieces (if needed)
        b["b8"] = 2;
        b["a7"] = 4;
        b["g1"] = 3;
    }

    // Main game loop
    while (!is_end_of_game(b, who_move))
    {
        print_game_state(b, bot);

        // Decide who has the move
        if (who_move == player_color)
        {
            cout << endl << "Move: player" << endl;
            move_player(b, player_color);
            who_move = bot_color;
        }
        else
        {
            cout << endl << "Move: bot" << endl;
            move_computer(b, bot);
            who_move = player_color;
        }
    }

    return 0;
}

void move_player(Board& b, int color)
{
    list<Move> l;
    generate_all_moves(l, b, color);

    Move players_move = read_move(b, l);
    make_move(b, players_move);
}

Move read_move(Board& b, list<Move>& possible_moves)
{
    cout << "Available moves:" << endl;
    int i = 0;
    for (auto a : possible_moves)
        cout << ++i << ". " << a;

    cout << "Your move: ";
    string player_move;
    getline(cin, player_move);

    if (player_move.empty())
    {
        cout << "Move is not correct" << endl;
        return read_move(b, possible_moves);
    }

    int number = num(player_move);
    if (number > 0 && number <= possible_moves.size())
    {
        int j = 1;
        for (auto& move : possible_moves)
        {
            if (j++ == number)
                return move;
        }
    }

    if (player_move.length() == 2)
    {
        string player_move2;
        getline(cin, player_move2);
        player_move += " " + player_move2;
    }

    Move move_p;
    for (int i = 0; i < player_move.length(); i += 2)
    {
        move_p.add(Field(player_move[i + 1] - '1', player_move[i] - 'a'));
        i++;
    }

    for (auto& move : possible_moves)
    {
        if (move == move_p)
            return move;
    }

    cout << "Move is not correct" << endl;
    return read_move(b, possible_moves);
}

void move_computer(Board& b, Bot& bot)
{
    Move bot_move = bot.pickBestMove(b, MAX_DEPTH);
    make_move(b, bot_move);
}

int num(string move)
{
    int x = 0;
    for (int i = 0; i < move.length(); i++)
    {
        if (move[i] - '0' >= 0 && move[i] - '0' <= 9)
        {
            x *= 10;
            x += (move[i] - '0');
        }
        else
        {
            break;
        }
    }
    return x;
}

bool is_end_of_game(Board& b, int who_move)
{
    bool white = false, black = false;

    // Check if pawns are still on the board
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (b.is_black(i, j)) black = true;
            if (b.is_white(i, j)) white = true;
        }
    }

    vector<Field> color_fields_print2 = color_fields_print;

    bool end = false;
    list<Move> l;
    if (!generate_all_moves(l, b, who_move)) // If no moves left for the player
    {
        print_winner(who_move == WHITE ? "BLACK" : "WHITE");
        end = true;
    }
    else if (!black) // White wins
    {
        print_winner("WHITE");
        end = true;
    }
    else if (!white) // Black wins
    {
        print_winner("BLACK");
        end = true;
    }
    else if (DRAW_AFTER_MOVES <= b.moves_with_kings) // Draw
    {
        print_winner("DRAW");
        end = true;
    }

    color_fields_print = color_fields_print2;

    return end;
}

string pretty_number(int n)
{
    string number = "";
    int three = 0;
    while (n > 0)
    {
        if (three == 3)
        {
            three = 0;
            number = "." + number;
        }
        number = (char)('0' + n % 10) + number;
        n = n / 10;
        three++;
    }
    return number;
}

void print_winner(const string& winner)
{
    system("cls");
    cout << "Game Over! " << winner << " WINS!!!" << endl;
}

void print_game_state(const Board& b, Bot& bot)
{
    system("cls");
    cout << b;
    cout << "Evaluate: " << bot.Evaluate(b) << endl;
    cout << "Bot evaluate: " << Bot::bot_eval << endl;
    cout << "Number of evaluated positions: " << pretty_number(Bot::number_of_evaluate_function) << endl;
}
