//
// Created by Memo on 2018/11/24.
//

#ifndef LL_1_LL_1_GRAMMER_H
#define LL_1_LL_1_GRAMMER_H

#include <iostream>
#include <algorithm>
#include <vector>
#include <deque>
#include <map>
#include <string>
#include <iomanip>
#include <stack>

using namespace std;

typedef struct DEDUCTION
{
    string left;
    string right;
    bool is_left_recursion = false;
} DEDUCTION;


class LL_1_grammer
{

    //产生式
    vector<DEDUCTION> deduction;
    //终结符集、非终结符集
    vector<string> nonterminal, terminal;
    //first集、follow集
    map<string, vector<string>> first, follow;
    //起始符
    string start;

    string x;

    string table[7][10];

public:


    void remove_direct_left_recursion();

    void init();

    void print_test();

    void ll_pushback(DEDUCTION mid) { deduction.push_back(mid); }

//    bool compare(DEDUCTION x, DEDUCTION y);

    void first_set();

    void follow_set();

    vector<string> get_first_set(string to_get_first);

    vector<string> get_follow_set(string to_get_follow);

    void analysis_table();

    void analysis_program(string text);

    int number_non(string s);

    int number_ter(string s);

    string process(string input);
};

#endif //LL_1_LL_1_GRAMMER_H
