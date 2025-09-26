#include <bits/stdc++.h>
using namespace std;

map<int, pair<char, string>> rules = {
    {1, {'S', "aSbA"}},
    {2, {'S', "aB"}},
    {3, {'S', "A"}},
    {4, {'A', "aAbS"}},
    {5, {'A', "aB"}},
    {6, {'A', "S"}},
    {7, {'B', "b"}},
    {8, {'B', "aA"}}
};

bool isNonterm(char c) { return c >= 'A' && c <= 'Z'; }

void updateLsf(std::string &lsf, char lhs, std::string &rhs, int rule)
{
    for (int j = 0; j < (int)lsf.size(); j++)
    {
        if (lsf[j] == lhs && (j + 1 == lsf.size() || !isdigit(lsf[j + 1])))
        {
            // формируем раскрытие: N<rule>(rhs)
            string expanded;
            for (char c : rhs)
            {
                expanded += c; // терминал
            }
            lsf = lsf.substr(0, j) + string(1, lhs) + to_string(rule) + "(" + expanded + ")" + lsf.substr(j + 1);
            break;
        }
    }
}
// применяет правило к текущей цепочке и одновременно обновляет ЛСФ
string apply_rule_once(string cur, int rule, string &lsf) {
    char lhs = rules[rule].first;
    string rhs = rules[rule].second;

    // ищем первый подходящий нетерминал и строим новую цепочку
    int found = -1;
    for (int i = 0; i < (int)cur.size(); i++) {
        if (cur[i] == lhs) {
            found = i;
            break;
        }
    }

    if (found== -1) {
        cout << "Невозможно применить правило" << endl;
        return "";
    }

    string step = cur.substr(0, found+1) + to_string(rule) + cur.substr(found+1);
    cout << step << " => ";
    cur = cur.substr(0, found) + rhs + cur.substr(found + 1);


    // теперь аналогично заменить в LSF
    // находим первый тот же нетерминал в LSF
    updateLsf(lsf, lhs, rhs, rule);

    return cur;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // читаем последовательность правил
    string line;
    getline(cin, line);
    vector<int> rules;
    stringstream ss(line);
    int r;
    while (ss >> r) rules.push_back(r);

    // начальное состояние
    string cur = "S";
    string lsf = "S";

    // пошаговое применение правил
    for (int rule : rules) {
        cur = apply_rule_once(cur, rule, lsf);
        if (cur == "") {
            return 0;
        }
    }
    // финальная цепочка
    cout << cur << "\n";

    // Линейная скобочная форма (строилась сразу!)
    cout << "\nЛинейная скобочная форма дерева вывода:\n" << lsf << "\n";

    cout << "\nПоследовательность правил при левом выводе:\n";
    for (char c : lsf) if (isdigit(c)) cout << c << " ";
    cout << "\n";
    for (char c : lsf) if (c >= 'a' && c <= 'z') cout << c << " ";
    cout << "\n";

    return 0;
}
