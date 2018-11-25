//
// Created by Memo on 2018/11/24.
//

#include "LL_1_grammer.h"

void LL_1_grammer::remove_direct_left_recursion()
{
    for(int i = 0; i < deduction.size(); i++)
    {
        if(deduction[i].left == deduction[i].right.substr(0,deduction[i].left.size()))//如果有左递归
        {
            deduction[i].is_left_recursion = true;
            string temp = deduction[i].left;
            for(int j = 0;j < deduction.size();j++)//将该非终结符左递归式全部标记
            {
                if(temp == deduction[j].left && deduction[j].left == deduction[j].right.substr(0,deduction[i].left.size()) && i!=j)
                    deduction[j].is_left_recursion = true;
            }
            for(int j = 0;j < deduction.size();j++)//消除左递归
            {
                if(deduction[j].left == temp && !deduction[j].is_left_recursion)
                    deduction[j].right.append(temp + "'");
                if(deduction[j].left == temp && deduction[j].is_left_recursion)
                {
                    deduction[j].left = temp + "'";
                    deduction[j].right.erase(0, temp.size());
                    deduction[j].right.append(temp + "'");
                    deduction[j].is_left_recursion = false;
                }
            }
            DEDUCTION mid;
            mid.left = temp + "'";
            mid.right = "epsilon";
            deduction.push_back(mid);
        }
    }

}
void LL_1_grammer::print_test()
{
    for(auto i: deduction)
        cout << i.left << "->" << i.right << endl;

}
void LL_1_grammer::init()
{
    nonterminal.emplace_back("E");
    nonterminal.emplace_back("T");
    nonterminal.emplace_back("F");
    terminal.emplace_back("+");
    terminal.emplace_back("-");
    terminal.emplace_back("*");
    terminal.emplace_back("/");
    terminal.emplace_back("(");
    terminal.emplace_back(")");
    terminal.emplace_back("num");//代表数字
}
