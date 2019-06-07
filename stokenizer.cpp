// Copyright (c) 2019, Matthew Chalifoux.

#include "stokenizer.h"
int STokenizer::_table[MAX_ROWS][MAX_COLUMNS];

int STokenizer::get_pos()
// returns private member _pos
{
    return _pos;
}
string Token::type_string()
// takes private member _type and turns into proper string and returns it
{
    string h;
    if(1 == _type){
        h = "UNKNOWN";
    }
    else if(2 == _type){
        h = "SPACE";
    }
    else if(3 == _type){
        h = "NUMBER";
    }
    else if(4 == _type){
        h = "ALPHA";
    }
    else if(5 == _type){
        h = "SPECIAL";
    }
    return h;
}
int Token::type()
// returns int type of token
{
    return _type;
}
string Token::token_str()
// returns token string
{
    return _token;
}
Token::Token(string str, int type)
// Constructor
{
    _token = str;
    _type = type;
}
Token &Token::operator =(const Token & h)
// Assignment construtor
{
    _token = h._token;
    _type = h._type;
}
Token::Token(const Token &h)
// Copy constructor
{
    _token = h._token;
    _type = h._type;
}
Token::~Token()
// Deconstructor
{}
STokenizer::STokenizer(const STokenizer &s)
// Constructor
{
    strcpy(_buffer,s._buffer);
    _buffer[strlen(_buffer)] = '\0';
    _pos = s._pos;
    make_table(_table);
}
STokenizer &STokenizer::operator =(const STokenizer &s)
// Assignment constructor
{
    strcpy(_buffer,s._buffer);
    _buffer[strlen(_buffer)] = '\0';
    _pos = s._pos;
    make_table(_table);
}
STokenizer::~STokenizer()
// Deconstructor
{

}
Token::Token()
//constructor
{
    _token = "";
    _type = 0;
}
ostream& operator <<(ostream& outs, const Token& t)
// outputs a token to the screen
{
    string sub;
    sub = t._token;
    outs << "|" << sub << "|" << endl;
    return outs;
}
STokenizer& operator >> (STokenizer& s, Token& t)
// takes a token from s and puts it in t
{
    if(!s.done())
    // if s is not at end of char[], it gets next token
    {
        string token = "";
        int type;
        type = 0;
        STokenizer tester;
        s.get_token(type,token);
        Token sub(token,type);
        t = sub;
        return s;
    }
    else if(s.done()){
        ++s._pos;
    }
}
bool STokenizer::done()
// returns true or false based on STokenizer being at end of char[];
{
    if(_pos  >= strlen(_buffer)){
        return true;
    }
    else if(_pos  < strlen(_buffer)){
        return false;
    }
}
bool STokenizer::more()
// returns true or false based on there being more characters to process
{
    if(_pos <= strlen(_buffer)){
        return true;
    }
    else if(_pos > strlen(_buffer)){
        return false;
    }
}
void STokenizer::set_string(char str[])
// sets a new string in STokenizer
{
    strcpy(_buffer,str);
    _buffer[strlen(_buffer)] = '\0';
    _pos = 0;
}
STokenizer::STokenizer(char str[])
// constructor
{
    strcpy(_buffer, str);
    _buffer[strlen(_buffer)] = '\0';
    _pos = 0;
    make_table(_table);
}
STokenizer::STokenizer()
// constructor
{
    _buffer[MAX_BUFFER] = '\0';
    _pos = 0;
    make_table(_table);
}
bool STokenizer::get_token(int& type, string &token)
// gets next token
{
    int start_pos = _pos;
    int cpy_start_pos = _pos;
    int cpy_state = 0;
    int state = 0;
    int j = 0;
    bool over = false;
//    cout << "_pos = " << _pos << endl;
//    cout << "_table[state][_buffer[_pos]] = " << _table[state][_buffer[_pos]] << endl;
    if(0 == _table[0][_buffer[_pos]])
    // if character is a special character, it gets proccessed first
    {
        type = 5;
        token = "*";
        over = true;
        ++_pos;
    }
    if(over == false)
    // if character is not special character
    {
        while(-1 != _table[state][_buffer[_pos]] && ('\0' != _buffer[_pos]))
        //moves _pos of _buffer till end of token and gets correct state
        {
            state = _table[state][_buffer[_pos]];
            ++_pos;
        }
        if(is_success(_table,state))
        // if token finishes in success state, processed here
        {
            for(int i = start_pos; i < _pos; ++i){
                token = token + _buffer[i];
            }
            if(7 == state){
                type = 1;
            }
            else if(6 == state){
                type = 2;
            }
            else if(5 == state){
                type = 4;
            }
            else if(4 == state){
                type = 4;
            }
            else if(3 == state || 1 == state){
                type = 3;
            }
        }
        else if(!is_success(_table,state))
        // if token does not end in a success state, it is proccessed here
        {
            if (2 == state)
            // figures out if token is unknown or number type
            {
                state = _table[cpy_state][_buffer[cpy_start_pos]];
                if(1 == state){
                    --_pos;
                    for(int i = start_pos; i < _pos; ++i){
                        token = token + _buffer[i];
                    }
                    type = 3;
                }
                else if(1 != state){
                    token = token + _buffer[start_pos];
                    type = 1;
                }
            }
//            else if(0 == state){
//                token = token + _buffer[start_pos];
//                type = 2;
//            }
        }
    }
}
void STokenizer::init_table(int _table[][MAX_COLUMNS])
// sets table to -1
{
    for(int i = 0; i < MAX_ROWS; ++i){
        for(int j = 0; j < MAX_COLUMNS; ++j){
            _table[i][j] = -1;
        }
    }
}
void STokenizer::mark_cell(int row, int _table[][MAX_COLUMNS], int column, int state)
// sets individual cell
{
    _table[row][column] = state;
}
void STokenizer::mark_cells(int row, int _table[][MAX_COLUMNS], int from, int to,
                int state)
// sets a group of cells
{
    for(int i = from; i <= to; ++i){
        _table[row][i] = state;
    }
}
void STokenizer::mark_fail(int _table[][MAX_COLUMNS], int state)
// marks state as fail
{
    _table[state][0] = 0;
}
void STokenizer::mark_success(int _table[][MAX_COLUMNS], int state)
// marks state as success
{
    _table[state][0] = 1;
}
bool STokenizer::is_success(int _table[][MAX_COLUMNS], int state)
// checks to see if state is a success state or fail state
{
    if(_table[state][0] == 1){
        return true;
    }
    else{
        return false;
    }
}
void STokenizer::neg_oneto7(int _table[][MAX_COLUMNS], int state)
// sets everything from column 1 row 0 that is still -1 to 7
{
    for(int i = 1; i < MAX_COLUMNS; ++i){
        if(-1 == _table[0][i]){
            _table[0][i] = 7;
        }
    }
}
void STokenizer::make_table(int _table[][MAX_COLUMNS])
// puts all the build tables together to build properly
{
    init_table(_table);
    mark_success(_table,1);
    mark_fail(_table,2);
    mark_success(_table,3);
    mark_success(_table,4);
    mark_success(_table,5);
    mark_success(_table,6);
    mark_success(_table,7);
    mark_cells(0,_table,'.','.',2);
    mark_cells(0,_table,'0','9',1);
    mark_cells(0,_table,'A','z',4);
    mark_cell(0,_table,' ',6);
    mark_cells(1,_table,'0','9',1);
    mark_cells(1,_table,'.','.',2);
    mark_cells(2,_table,'0','9',3);
    mark_cells(3,_table,'0','9',3);
    mark_cells(4,_table,'0','9',5);
    mark_cells(4,_table,'A','z',4);
    mark_cells(5,_table,'0','9',5);
    mark_cells(5,_table,'A','z',4);
    neg_oneto7(_table,0);
}
