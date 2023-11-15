// Do NOT add any other includes
// #pragma once

#include <string>
#include <vector>
#include <iostream>
#include "dict.h"
#include "Node.h"
using namespace std;

class SearchEngine
{
private:
    // You can add attributes/helper functions here
    // symtable is a AVL tree.
    // lib stores word count of a paragraph using hash that is combination of it's Book_code, page_no., parag_no.
    vector<SymbolTable *> lib;

    // word_ct :- Dict object that gives word count from complete inserted corpus.
    Dict word_ct;

    // gen_word_freq:- Dict object that gives word count for general corpus.
    Dict gen_word_freq;

public:
    /* Please do not touch the attributes and
    functions within the guard lines placed below  */
    /* ------------------------------------------- */
    SearchEngine();

    ~SearchEngine();

    void insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence);

    Node *search(string pattern, int n_matches, int p = 0);

    /* -----------------------------------------*/
    vector<float> relavency_q(vector<string> q);
};