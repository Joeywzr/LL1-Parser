//
// Created by Memo on 2018/11/24.
//

#include "LL_1_grammer.h"

//bool LL_1_grammer::compare(DEDUCTION x, DEDUCTION y)
//{
//    return x.left < y.left ;
//}

void LL_1_grammer::remove_direct_left_recursion()
{
    for (int i = 0; i < deduction.size(); i++) {
        if (deduction[i].left == deduction[i].right.substr(0, deduction[i].left.size()))//如果有左递归
        {
            deduction[i].is_left_recursion = true;
            string temp = deduction[i].left;
            for (int j = 0; j < deduction.size(); j++)//将该非终结符左递归式全部标记
            {
                if (temp == deduction[j].left &&
                    deduction[j].left == deduction[j].right.substr(0, deduction[i].left.size()) && i != j)
                    deduction[j].is_left_recursion = true;
            }
            for (int j = 0; j < deduction.size(); j++)//消除左递归
            {
                if (deduction[j].left == temp && !deduction[j].is_left_recursion)
                    deduction[j].right.append(temp + "'");
                if (deduction[j].left == temp && deduction[j].is_left_recursion) {
                    deduction[j].left = temp + "'";
                    deduction[j].right.erase(0, temp.size());
                    deduction[j].right.append(temp + "'");
                    deduction[j].is_left_recursion = false;
                }
            }
            DEDUCTION mid;
            mid.left = temp + "'";
            mid.right = "ε";
            deduction.push_back(mid);
            nonterminal.emplace_back(temp + "'");
            if (find(terminal.begin(), terminal.end(), "ε") == terminal.end())
                terminal.emplace_back("ε");
        }
    }
}

void LL_1_grammer::print_test()
{
    cout << "表达式" << endl;
    for (auto i: deduction)
        cout << i.left << "->" << i.right << endl;

    cout << "非终结符集" << endl;
    for (auto i: nonterminal)
        cout << i << endl;

    cout << "终结符集" << endl;
    for (auto i: terminal)
        cout << i << endl;

    cout << "first集" << endl;
    for(auto i:first)
    {
        cout << i.first << " : ";
        for(int j = 0;j < i.second.size();j++)
            if(j == i.second.size()-1)
                cout << i.second[j];
            else
                cout << i.second[j] << " , ";
        cout << endl;
    }

    cout << "follow集" << endl;
    for(auto i:follow)
    {
        cout << i.first << " : ";
        for(int j = 0;j < i.second.size();j++)
            if(j == i.second.size()-1)
                cout << i.second[j];
            else
                cout << i.second[j] << " , ";
        cout << endl;
    }

}

void LL_1_grammer::init()
{
    start = "E";
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


vector<string> LL_1_grammer::get_first_set(string to_get_first)//构建first集
{
    vector<string> mid;
    for (auto i:deduction)
    {
        if (i.left == to_get_first)
        {
            auto iter = first.find(to_get_first);
            if(iter != first.end())
                mid = first[to_get_first];
            string head = i.right.substr(0,1);
            for(auto nt:nonterminal)
            {
                if(head[0] == nt[0])
                {
                    return get_first_set(head);
                }
            }
            for(auto t:terminal)
            {
                if(head[0] == t[0])
                {
                    if (find(mid.begin(), mid.end(), t) == mid.end())//若没有该终结符，则加入first集
                        mid.push_back(t);
                    break;
                }
            }
        }
    }
    return mid;
}

void LL_1_grammer::first_set()
{
    for(auto i:nonterminal)
        first[i]=get_first_set(i);
}

void LL_1_grammer::follow_set()
{
    for(auto i:nonterminal)
        follow[i]=get_follow_set(i);
}
vector<string> LL_1_grammer::get_follow_set(string to_get_follow)//构建follow集
{
    if(!follow[to_get_follow].empty())
        return follow[to_get_follow];
    vector<string> mid;
    if(to_get_follow == "E")
        mid.emplace_back("$");
    for (auto i:deduction)
    {
        int cur_ch = i.right.find(to_get_follow);
        int length = to_get_follow.size();
        if (cur_ch < 0 || (length == 1 && i.right[cur_ch+1] == '\'') || to_get_follow == i.left)
            continue;

        if (cur_ch == i.right.size() - 1)
        {
            if(i.left != i.right.substr(cur_ch, cur_ch))
                continue;
            vector<string> temp = get_follow_set(i.left);
            for (auto i:temp)
            {
                if (i != "ε" && find(mid.begin(), mid.end(), i) == mid.end())
                    mid.push_back(i);
            }
        }
        else if (cur_ch == i.right.size() - 2)
        {
            string two_words = i.right.substr(cur_ch, i.right.size() - 1);
            if(i.left == two_words)
                continue;
            if (find(nonterminal.begin(), nonterminal.end(), two_words) != nonterminal.end())
            {
                vector<string> temp = get_follow_set(i.left);

                for (auto i:temp)
                {
                    if (find(mid.begin(),mid.end(), i) == mid.end())
                        mid.push_back(i);
                }
            }
            else
            {
                cur_ch++;
                string mid2 = i.right.substr(cur_ch, cur_ch);
                if ((find(terminal.begin(), terminal.end(), mid2) != terminal.end()) && (find(mid.begin(), mid.end(), mid2) == mid.end()))
                    mid.push_back(mid2);
                else
                {
                    vector<string> temp = get_first_set(mid2);

                    for (auto j:temp)
                    {
                        if (j != "ε")
                        {
                            if (find(mid.begin(), mid.end(), j) == mid.end())
                                mid.push_back(j);
                        }
                        else
                        {
                            vector<string> temp2 = get_follow_set(i.left);

                            for (auto k:temp2)
                            {
                                if (k != "ε" && find(mid.begin(), mid.end(), k) == mid.end())
                                    mid.push_back(k);
                            }
                        }
                    }
                }
            }
        }
        else
        {
            cur_ch++;
            string two_words = i.right.substr(cur_ch, i.right.size() - 1);
            if (find(nonterminal.begin(), nonterminal.end(), two_words) != nonterminal.end())
            {
                vector<string> temp = get_first_set(two_words);

                for (auto j:temp)
                {
                    if (j != "ε")
                    {
                        if (find(mid.begin(), mid.end(), j) == mid.end())
                            mid.push_back(j);
                    }
                    else
                    {
                        vector<string> temp2 = get_follow_set(i.left);

                        for (auto k:temp2)
                        {
                            if (find(mid.begin(), mid.end(), k) == mid.end())
                                mid.push_back(k);
                        }
                    }
                }
            }
        }
    }
    return mid;
}

void LL_1_grammer::analysis_table()
{
    cout << "-------------------------------------------------------------------------------" << endl;
    cout << setw(10);
    terminal.emplace_back("$");
    for(int i = 0;i < terminal.size();i++)
    {
        if(terminal[i]!="ε")
            cout << terminal[i] << setw(10);
    }
    cout <<endl;
    cout << "-------------------------------------------------------------------------------" << endl;

    for(auto i:nonterminal)
    {
        vector<string> line;
        cout <<"\n"<< i << setw(10);
        for(int j = 0;j < terminal.size();j++)
        {
            if(terminal[j] == "ε")
                continue;
            if(find(first[i].begin(),first[i].end(),"ε")!=first[i].end() && find(follow[i].begin(),follow[i].end(),terminal[j])!=follow[i].end())
                    line.emplace_back(i + "->ε");
            else if(find(first[i].begin(),first[i].end(),terminal[j])!=first[i].end())
            {
                for(auto k:deduction)
                {
                    if(k.left == i)
                    {
                        string temp = k.right.substr(0,1);
                        if(k.right[0] == terminal[j][0])
                            line.emplace_back(k.left + "->" + k.right);
                        else if(find(first[temp].begin(),first[temp].end(),terminal[j]) != first[temp].end())
                            line.emplace_back(k.left + "->" + k.right);
                    }
                }
            }
            else
                line.emplace_back(" ");
        }
        for(auto out:line)
            cout <<setiosflags(ios::right)<< setw(10) << out <<resetiosflags(ios::right);
    }
    cout << "\n";
}

void LL_1_grammer::analysis_program(string text)
{
    char a;
    int cur = 0;
    stack<string> buffer;
    buffer.push("$");
    buffer.push(start);
    text.append("$");

    cout << "-------------------------------------------------------------------------------" << endl;
    cout << setw(20) << "栈" << setw(20) << "输入" << setw(20) << "输出" << endl;

    do
    {
        x = buffer.top();
        a = text[cur];
        if(x == "$" && a == '$')
            break;
        //判断x是否为终结符号
        bool judge = (find(terminal.begin(), terminal.end(),x)==terminal.end())
        if(!judge)//是终结符号
        {
            if(x[0] == a)
            {
                cout << "match"
            }
        }
        else
        {

        }

    }while(1);
    cout << "-------------------------------------------------------------------------------" << endl;
}