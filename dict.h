// Do NOT add any other includes
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;
class SymNode
{
public:
    string key;
    int height;
    long long address = 1;

    SymNode *par = NULL;
    SymNode *left = NULL;
    SymNode *right = NULL;

    SymNode();
    SymNode(string k, long long ct = 1);

    SymNode *LeftLeftRotation();   // case 1
    SymNode *RightRightRotation(); // case 2
    SymNode *LeftRightRotation();  // case 3
    SymNode *RightLeftRotation();  // case 4

    ~SymNode();
};
class SymbolTable
{
public:
    int size = 0;
    SymNode *root = NULL;

public:
    SymbolTable();
    ~SymbolTable();
    void insert(string k, long long ct);
    void assign_val(string k, int v);
    long long search(string k);
};

class Hash
{
public:
    vector<SymbolTable *> symtb;
    Hash();
    int Hash_val(string str);
    void assign_val(string w, long long ct);
    void insert(string str, long long ct);
    long long val(string str);
    void wto(string file_name);
    ~Hash();
};
class Dict
{
private:
    Hash hash;

public:
    /* Please do not touch the attributes and
    functions within the guard lines placed below  */
    /* ------------------------------------------- */
    Dict();

    ~Dict();

    void insert_sentence(long long count, string sentence);

    long long get_word_count(string word);
    void assign_word_ct(string w, int ct);
    void dump_dictionary(string filename);

    /* -----------------------------------------*/
};