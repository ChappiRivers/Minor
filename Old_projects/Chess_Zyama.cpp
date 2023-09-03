#include <iostream>
#include <string>
#include <vector>
#include <cmath>

// #define NO_KING
using namespace std;

enum Index {a, b, c, d, e, f, g, h};

string warn_1 = "Wrong placement";
string warn_2 = "Illegal move";
string warn_3 = "Illegal command";
string warn_4 = "Out of range";
string warn_K_1 = "No king";
string warn_K_2 = "King is under attack";
string chars[8]{"a", "b", "c", "d", "e", "f", "g", "h"};

enum Color
{
    BLACK,
    WHITE
};

class Piece;

class Board
{
public:
    virtual Piece *get_piece(int row, int col) const = 0;
    virtual bool under_beat(Color _color, int x, int y) const = 0;
    virtual bool is_piece(int x, int y) const = 0;
};

class _Piece
{
public:
    Color color;
    int cost;
    int x;
    int y;
    _Piece(Color _color, int _x, int _y, int _cost) : color(_color), x(_x), y(_y), cost(_cost) {}

    Color get_color() const 
    {
        return color;
    }
    virtual string get_str() const = 0;
};

class Movable: virtual public _Piece
{
public:
    virtual bool can_move(int x, int y, const Board &cb) const = 0;
};
class Capturable: virtual public _Piece
{
public:
    virtual bool can_capture(int x, int y, const Board &cb) const = 0;
    virtual bool can_beat(int x, int y, const Board &cb) const = 0;
};
class Piece: public Movable, public Capturable {};

class Pawn : public Piece
{
public:
    Pawn(Color _color, int _x, int _y) : _Piece(_color, _x, _y, 1)  {}

    string get_str() const override
    {
        if (color == Color::BLACK)
            return "♟ ";
        else
            return "♙ ";
    }

    bool can_move(int _x, int _y, const Board &cb) const override
    {
        if (color == Color::WHITE)
        {
            if (((x == _x) && (y == _y - 1) && (cb.get_piece(8 - _y, _x) == nullptr)) || ((cb.get_piece(8 - _y, _x) == nullptr) && (x == _x) && (y == _y - 2) && (y == 2)))
                return 1;
            else
                return 0;
        }
        else
        {
            if (((x == _x) && (y == _y + 1) && (cb.get_piece(8 - _y, _x) == nullptr)) || ((cb.get_piece(8 - _y, _x) == nullptr) && (x == _x) && (y == _y + 2) && (y == 7)))
                return 1;
            else
                return 0;
        }
    }

    bool can_beat(int _x, int _y, const Board &cb) const override
    {
        if (color == Color::WHITE)
        {
            if (((_x == x - 1) || (_x == x + 1)) && (_y == y + 1))
            {
                return 1;
            }
        }
        else
        {
            if (((_x == x - 1) || (_x == x + 1)) && (_y == y - 1))
            {
                return 1;
            }
        }
        return 0;
    }

    bool can_capture(int _x, int _y, const Board &cb) const override
    {
        if (can_beat(_x, _y, cb) && (cb.get_piece(8 - _y, _x) != nullptr) && (cb.get_piece(8 - _y, _x)->color != color))
            return 1;
        return 0;
    }

    // взятие на проходе
};

class King: public Piece
{
public:
    King(Color _color, int _x, int _y) : _Piece(_color, _x, _y, 0) {}

    string get_str() const override
    {
        if (color == Color::BLACK)
            return "♚ ";
        else
            return "♔ ";
    }

    bool can_beat(int _x, int _y, const Board &cb) const override
    {
        if (x == _x && y == _y)
            return 0;
        if (((x - _x <= 1) && (x - _x >= -1)) && ((y - _y <= 1) && (y - _y >= -1)))
            return 1;
        else
            return 0;
    }

    bool can_move(int _x, int _y, const Board &cb) const override
    {
        if ((cb.get_piece(8 - _y, _x) == nullptr) && can_beat(_x, _y, cb) && !cb.under_beat(color, _x, _y))
            return 1;
        else
            return 0;
    }

    bool can_capture(int _x, int _y, const Board &cb) const override
    {
        if (can_beat(_x, _y, cb) && (cb.get_piece(8 - _y, _x) != nullptr) && (cb.get_piece(8 - _y, _x)->color != color))
            return 1;
        return 0;
    }
};

class Bishop: public Piece
{
public:
    Bishop(Color _color, int _x, int _y) : _Piece(_color, _x, _y, 3) {}

    string get_str() const override
    {
        if (color == Color::BLACK)
            return "♝ ";
        else
            return "♗ ";
    }

    bool can_beat(int _x, int _y, const Board &cb) const override
    {
        if (x == _x && y == _y)
            return 0;
        if (_y - _x - y + x == 0)
        {
            if (_y > y)
            {
                for(int i = 1; i < _y - y; i++)
                {
                    if (cb.is_piece(8 - y - i, x + i))
                        return 0;
                }
                return 1;
            }
            else if (_y < y)
            {
                for(int i = 1; i < -_y + y; i++)
                {
                    if (cb.is_piece(8 - y + i, x - i))
                        return 0;
                }
                return 1;
            }
            return 0;
        }  
        else if (_y + _x - y - x == 0)
        {
            if (_y > y)
            {
                for(int i = 1; i < _y - y; i++)
                {
                    if (cb.is_piece(8 - y - i, x - i))
                        return 0;
                }
                return 1;
            }
            else if (_y < y)
            {
                for(int i = 1; i < -_y + y; i++)
                {
                    if (cb.is_piece(8 - y + i, x + i))
                        return 0;
                }
                return 1;
            }
            return 0;
        }
        else
            return 0;
    }

    bool can_move(int _x, int _y, const Board &cb) const override
    {
        if ((cb.get_piece(8 - _y, _x) == nullptr) && can_beat(_x, _y, cb))
            return 1;
        else
            return 0;
    }

    bool can_capture(int _x, int _y, const Board &cb) const override
    {
        if (can_beat(_x, _y, cb) && (cb.get_piece(8 - _y, _x) != nullptr) && (cb.get_piece(8 - _y, _x)->color != color))
            return 1;
        return 0;
    }
};

class Rock: public Piece
{
public:
    Rock(Color _color, int _x, int _y) : _Piece(_color, _x, _y, 5) {}

    string get_str() const override
    {
        if (color == Color::BLACK)
            return "♜ ";
        else
            return "♖ ";
    }

    bool can_beat(int _x, int _y, const Board &cb) const override
    {
        if (x == _x && y == _y)
            return 0;
        if (_x == x)
        {
            if (_y > y)
            {
                for(int i = 1; i < _y - y; i++)
                {
                    if (cb.is_piece(8 - y - i, x))
                        return 0;
                }
                return 1;
            }
            else if (_y < y)
            {
                for(int i = 1; i < -_y + y; i++)
                {
                    if (cb.is_piece(8 - y + i, x))
                        return 0;
                }
                return 1;
            }
            return 0;
        }  
        else if (_y == y)
        {
            if (_x > x)
            {
                for(int i = 1; i < _x - x; i++)
                {
                    if (cb.is_piece(8 - y, x + i))
                        return 0;
                }
                return 1;
            }
            else if (_x < x)
            {
                for(int i = 1; i < x - _x; i++)
                {
                    if (cb.is_piece(8 - y, x - i))
                        return 0;
                }
                return 1;
            }
            return 0;
        }
        else
            return 0;
    }

    bool can_move(int _x, int _y, const Board &cb) const override
    {
        if ((cb.get_piece(8 - _y, _x) == nullptr) && can_beat(_x, _y, cb))
            return 1;
        else
            return 0;
    }

    bool can_capture(int _x, int _y, const Board &cb) const override
    {
        if (can_beat(_x, _y, cb) && (cb.get_piece(8 - _y, _x) != nullptr) && (cb.get_piece(8 - _y, _x)->color != color))
            return 1;
        return 0;
    }
};

class Queen: public Piece
{
public:
    Queen(Color _color, int _x, int _y) : _Piece(_color, _x, _y, 10) {}

    string get_str() const override
    {
        if (color == Color::BLACK)
            return "♛ ";
        else
            return "♕ ";
    }

    bool can_beat(int _x, int _y, const Board &cb) const override
    {
        Bishop* B = new Bishop(color, x, y);
        Rock* R = new Rock(color, x, y);
        if (R->can_beat(_x, _y, cb) || B->can_beat(_x, _y, cb))
            return 1;
        return 0;
    }

    bool can_move(int _x, int _y, const Board &cb) const override
    {
        if ((cb.get_piece(8 - _y, _x) == nullptr) && can_beat(_x, _y, cb))
            return 1;
        else
            return 0;
    }

    bool can_capture(int _x, int _y, const Board &cb) const override
    {
        if (can_beat(_x, _y, cb) && (cb.get_piece(8 - _y, _x) != nullptr) && (cb.get_piece(8 - _y, _x)->color != color))
            return 1;
        return 0;
    }
};

class Night : public Piece
{
public:
    Night(Color _color, int _x, int _y) : _Piece(_color, _x, _y, 3) {}

    string get_str() const override
    {
        if (color == Color::BLACK)
            return "♞ ";
        else
            return "♘ ";
    }

    bool can_beat(int _x, int _y, const Board &cb) const override
    {
        if (abs(_x - x) + abs(_y - y) == 3)
            return 1;
        return 0;
    }

    bool can_move(int _x, int _y, const Board &cb) const override
    {
        if ((cb.get_piece(8 - _y, _x) == nullptr) && can_beat(_x, _y, cb))
            return 1;
        else
            return 0;
    }

    bool can_capture(int _x, int _y, const Board &cb) const override
    {
        if (can_beat(_x, _y, cb) && (cb.get_piece(8 - _y, _x) != nullptr) && (cb.get_piece(8 - _y, _x)->color != color))
            return 1;
        return 0;
    }
};

class ChessBoard : public Board
{
public:
    vector<string> moves;
    Piece *board[8][8];
    ChessBoard()
    {
        for (int y = 0; y < 8; y++)
        {
            for (int x = 0; x < 8; x++)
            {
                board[x][y] = nullptr;
            }
        }
    }

    bool on_board(int x, int y) const
    {
        if (x < 0 || x > 8 || y < 0 || y > 8)
        {
            return 0;
        }
        return 1;
    }

    Piece *get_piece(int y, int x) const
    {
        if (!on_board(x, y))
        {
            throw warn_4;
        }
        return board[y][x];
    }

    void add_piece(Piece *p)
    {
        if (board[8 - p->y][p->x] != nullptr)
        {
            throw warn_1;
        }
        board[8 - p->y][p->x] = p;
    }

    int check_king(int color) const
    {
        #ifndef NO_KING
        try
        {
            for (int y = 0; y < 8; y++)
            {
                for (int x = 0; x < 8; x++)
                {
                    if (board[x][y] && (board[x][y]->cost == 0) && (board[x][y]->color == color))
                    {
                        throw board[x][y];
                    }
                }
            }
            // throw warn_K_1;
            return 1;
        }
        catch(Piece* temp)
        {
            // cout << temp->get_str() << endl; 
            if (under_beat((Color) color, temp->x, temp->y))
            // throw warn_K_2;
                return 2;
        }
        #endif
        return 0;
    }

    void step(string mv)
    {
        Piece *p;
        int y = mv[1] - '0';
        int x = mv[0] - 'a';
        if ((p = get_piece(8 - y, x)) == nullptr)
            throw warn_2;

        y = mv[4] - '0';
        x = mv[3] - 'a';
        if (!on_board(x, y))
            throw warn_4;

        moves.push_back(mv);

        if (p->can_move(x, y, *this))
        {
            int tmp_x = p->x, tmp_y = p->y;
            board[8 - y][x] = board[8 - p->y][p->x];
            board[8 - p->y][p->x] = nullptr;
            p->x = x;
            p->y = y;

            int rez = check_king((Color) (moves.size() % 2));
            if (rez == 1)
                throw warn_K_1;
            if (rez == 2)
            {
                // cout << *this << endl;
                p->x = tmp_x;
                p->y = tmp_y;
                board[8 - p->y][p->x] = board[8 - y][x];
                board[8 - y][x] = nullptr;
                moves.pop_back();
                throw warn_K_2;
            }
            return;
        }
        // cout << "can't move" << endl;
        if (p->can_capture(x, y, *this))
        {
            int tmp_x = p->x, tmp_y = p->y;
            Piece *P = board[8 - y][x];
            board[8 - y][x] = board[8 - p->y][p->x];
            board[8 - p->y][p->x] = nullptr;
            p->x = x;
            p->y = y;

            int rez = check_king((Color) (moves.size() % 2));
            if (rez == 1)
                throw warn_K_1;
            if (rez == 2)
            {
                p->x = tmp_x;
                p->y = tmp_y;
                board[8 - p->y][p->x] = board[8 - y][x];
                board[8 - y][x] = P;
                P = nullptr;
                moves.pop_back();
                throw warn_K_2;
            }
            return;
        }

        moves.pop_back();
        // cout << "can't capture" << endl;
        throw warn_2;
    }

    void pop_back()
    {
        moves.pop_back();
    }

    bool under_beat(Color _color, int x, int y) const
    {
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                if (board[i][j] && board[i][j]->color != _color && board[i][j]->can_beat(x, y, *this)) 
                {
                    return 1;
                }
            }
        }
        return 0;
    }

    bool is_piece(int x, int y) const 
    {
        // cout << x << ' ' << y << endl;
        return board[x][y];
    }

    friend ostream &operator<<(ostream &os, const ChessBoard &cb)
    {
        for (int i = 0; i < 8; i++)
        {
            cout << 8 - i << " ";
            for (int j = 0; j < 8; j++)
            {
                if (cb.board[i][j] == nullptr)
                {
                    if ((i + j) % 2 == 1)
                        cout << "◼ ";
                    else
                        cout << "◻ ";
                }
                else
                    cout << cb.board[i][j]->get_str();
            }
            os << endl;
        }
        os << "  ";
        for (int i = 0; i < 8; i++)
            os << chars[i] << " ";
        os << endl;
        for (int i = 0; i < cb.moves.size(); i++)
        {
            cout << i + 1 << ". " << cb.moves[i] << endl;
        }
        return os;
    }

    void new_round()
    {
        Rock* R1 = new Rock(Color::WHITE, a, 1);
        Rock* R2 = new Rock(Color::WHITE, h, 1);
        Night* N1 = new Night(Color::WHITE, b, 1);
        Night* N2 = new Night(Color::WHITE, g, 1);
        Bishop* B1 = new Bishop(Color::WHITE, c, 1);
        Bishop* B2 = new Bishop(Color::WHITE, f, 1);
        King* K1 = new King(Color::WHITE, e, 1);
        Queen* Q1 = new Queen(Color::WHITE, d, 1);
        add_piece(R1);
        add_piece(R2);
        add_piece(N1);
        add_piece(N2);
        add_piece(B1);
        add_piece(B2);
        add_piece(K1);
        add_piece(Q1);

        Rock* R3 = new Rock(Color::BLACK, a, 8);
        Rock* R4 = new Rock(Color::BLACK, h, 8);
        Night* N3 = new Night(Color::BLACK, b, 8);
        Night* N4 = new Night(Color::BLACK, g, 8);
        Bishop* B3 = new Bishop(Color::BLACK, c, 8);
        Bishop* B4 = new Bishop(Color::BLACK, f, 8);
        King* K2 = new King(Color::BLACK, e, 8);
        Queen* Q2 = new Queen(Color::BLACK, d, 8);
        add_piece(R3);
        add_piece(R4);
        add_piece(N3);
        add_piece(N4);
        add_piece(B3);
        add_piece(B4);
        add_piece(K2);
        add_piece(Q2);

        for(int i = 0; i < 8; i++)
        {
            board[6][i] = new Pawn(Color::WHITE, i, 2);
        }
        for(int i = 0; i < 8; i++)
        {
            board[1][i] = new Pawn(Color::BLACK, i, 7);
        }
    }

    ~ChessBoard()
    {
        for (int y = 0; y < 8; y++)
        {
            for (int x = 0; x < 8; x++)
            {
                if (board[y][x])
                    delete board[y][x];
            }
        }
    }
};

int main()
{
    ChessBoard bd;
    string mv;
    int turn = 0;
    bd.new_round();
    cout << bd;
    while (1)
    {
        try
        {
            turn++;
            if (turn % 2) 
                cout << "♔ " << endl;
            else
                cout << "♚ " << endl;
            cout << turn << ". ";
            getline(cin, mv);
            if (mv == "out")
                break;
            bd.step(mv);
            cout << bd;
        }
        catch (string &s)
        {
            cout << s << " on turn " << turn << endl;
            turn--;
            cout << bd;
            cout << "try again" << endl;
        }
    }
}