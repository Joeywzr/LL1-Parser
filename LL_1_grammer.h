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
using namespace std;

typedef struct DEDUCTION{
    string left;
    string right;
    bool is_left_recursion = false;
}DEDUCTION;

class LL_1_grammer
{

    //����ʽ
    vector<DEDUCTION> deduction;
    //�ս���������ս����
    vector<string> nonterminal, terminal;
public:


    void remove_direct_left_recursion();

    void init();

    void print_test();

    void ll_pushback(DEDUCTION mid){deduction.push_back(mid);}




};


#endif //LL_1_LL_1_GRAMMER_H
