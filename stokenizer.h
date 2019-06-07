// Copyright (c) 2019, Matthew Chalifoux.
#ifndef STOKENIZER_H
#define STOKENIZER_H
#include <iostream>
#include <cstring>
#include <iomanip>
#include <fstream>
using namespace std;
const int MAX_COLUMNS = 256;
const int MAX_ROWS = 8;
const int MAX_BUFFER = 999;
class Token{
public:
    // CONSTRUCTORS
    Token();
    Token(string str, int type);
    // THE BIG 3
    Token & operator =(const Token & h);
    Token(const Token &h);
    ~Token();
    // operator that outputs the Token class
    friend ostream& operator <<(ostream& outs, const Token& t);
    int type();                 // returns type of token as int
    string type_string();       // returns type of token as string
    string token_str();         // returns token
private:
    string _token;
    int _type;
};
class STokenizer{
public:
    // CONSTRUCTORS
    STokenizer();
    STokenizer(char str[]);
    // THE BIG 3
    STokenizer & operator =(const STokenizer & h);
    STokenizer(const STokenizer &h);
    ~STokenizer();
    bool done();                    //true: there are no more tokens
    bool more();                    //true: there are more tokens
    //extract one token (very similar to the way cin >> works)
    friend STokenizer& operator >> (STokenizer& s, Token& t);
    //set a new string as the input string
    void set_string(char str[]);    //sets a new string into STokenizer
    int get_pos();                  // returns _pos

    // THE STATE MACHINE
    // Fill all cells of the array with -1
    void init_table(int _table[][MAX_COLUMNS]);
    // Mark a range of cells in the array.
    void mark_cells(int row, int _table[][MAX_COLUMNS], int from, int to,
                    int state);
    // Mark this row and column
    void mark_cell(int row, int _table[][MAX_COLUMNS], int column, int state);
    // Mark this state (row) with a 1 (success)
    void mark_success(int _table[][MAX_COLUMNS], int state);
    // Mark this state (row) with a 0 (fail)
    void mark_fail(int _table[][MAX_COLUMNS], int state);
    // True if state is a success state
    bool is_success(int _table[][MAX_COLUMNS], int state);
    // Makes all colums in row zero that are -1 into a 7
    void neg_oneto7(int _table[][MAX_COLUMNS],int state);

private:
    //create table for all the tokens we will recognize
    //                      (e.g. doubles, words, etc.)
    void make_table(int _table[][MAX_COLUMNS]);
    //extract the longest string that match
    //      one of the acceptable token types
    bool get_token(int& type, string& token);
    //---------------------------------
    char _buffer[MAX_BUFFER + 1];       //input string
    int _pos;                       //current position in the string
    static int _table[MAX_ROWS][MAX_COLUMNS];
};
#endif // STOKENIZER_H
