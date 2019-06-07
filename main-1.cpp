
// Copyright (c) 2019, Matthew Chalifoux.


#include "stokenizer.cpp"
#include <fstream>
class FTokenizer{
public:
    const int MAX_BLOCK = MAX_BUFFER;
    // CONSTRUCTOR
    FTokenizer(char* fname);
    Token next_token();
    bool more();         //returns the current value of _more
    int pos();           //returns the value of _pos
    int block_pos();     //returns the value of _blockPos
    // Operator to get single token from FTokenizer f and put in it Token t
    friend FTokenizer& operator >> (FTokenizer& f, Token& t);
private:
    bool get_new_block(); //gets the new block from the file
    std::ifstream _f;   //file being tokenized
    STokenizer _stk;     //The STokenizer object to tokenize current block
    int _pos;           //Current position in the file
    int _blockPos;      //Current position in the current block
    bool _more;         //false if last token of the last block
                        //  has been processed and now we are at
                        //  the end of the last block.
};
// Function to extract tokens from a file
void test_f_tokenize_simple();

int main()
{
    test_f_tokenize_simple();

    return 0;
}

void test_f_tokenize_simple(){
    Token t;
    FTokenizer ftk("solitude.txt");
    ftk>>t;
    int token_count = 0;
    while (ftk.more()){
        if (t.type_string()=="ALPHA" ){
            token_count++;
            if(200 >= token_count){
            cout<<setw(10)<<token_count
                <<setw(3)<<left<<":"<<setw(25)<<left<<t.token_str()
                <<t.type_string()<<endl;
            }
        }
        ftk>>t;
    }
    cout<<"Tokens Found: "<<token_count<<endl;
    cout<<"=========="<<endl;
}
FTokenizer& operator >> (FTokenizer& f, Token& t)
// Operator to get single token from FTokenizer f and put in it Token t
{
    if(f.more() == true)
    // Will loop if file f has more tokens left to be proccessed
    {
        if(f.MAX_BLOCK == f._blockPos)
        // Will get new block of characters into _stk when _blockPos full
        {
            if(f.get_new_block())
            // Will get new block if not at f.eof();
            {
            }
        }
        string token = "";
        int type = 0;
        t = Token();
        f._stk >> t;
        f._blockPos = f._stk.get_pos();
    }
}
bool FTokenizer::get_new_block()
// Retrieves a new block of characters from _f
{
    if(!_f.eof())
    //Will get a new block if not end of file
    {
        char block[MAX_BLOCK + 1];
        _f.read(block, MAX_BLOCK);
        int j = 0;
        j = _f.gcount();
        _pos = _pos + j;
        block[_f.gcount()] = '\0';
//        int i = 0;
//        while(block[i] != '\0'){
//            cout << block[i];
//            ++i;
//        }
        STokenizer sub(block);
        _stk = sub;
        return true;
    }
    else if(_f.eof())
    // Will not get a new block if at end of file
    {
        return false;
    }
}
bool FTokenizer::more()
// returns true is there is a token left to be processed
{
    if(!_f.eof())
    // If not end of file more returns true
    {
        _more = true;
        return true;
    }
    else if(_f.eof() && _stk.more())
    // If it is the end of file but _stk.more is true, more returns true
    {
        _more = true;
        return true;
    }
    else if(_f.eof() && !_stk.more())
    // If it is end of file and _stk.more is not true, more returns false
    {
        _more = false;
        return false;
    }
}
FTokenizer::FTokenizer(char* fname)
// Constructor for Ftokenizer
{
    _f.open(fname);
    if(_f.fail()){
        cout << "Input file opening failed." << endl;
        exit(0);
    }
    _pos = 0;
    _blockPos = 0;
    get_new_block();
}
