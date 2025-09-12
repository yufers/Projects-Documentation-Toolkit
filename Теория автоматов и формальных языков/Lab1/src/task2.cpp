#include <bits/stdc++.h>
using namespace std;

// Грамматика:
// 1. S -> a S b A
// 2. S -> a B
// 3. S -> A
// 4. A -> a A b S
// 5. A -> a B
// 6. A -> S
// 7. B -> b
// 8. B -> a A

struct Node
{
    char sym; // 'S','A','B' для нетерминалов или 'a','b' для терминалов
    int rule; // номер правила, использованного для раскрытия узла, -1 если не раскрыт (лист)
    vector<shared_ptr<Node>> ch;
    Node(char s = 0) : sym(s), rule(-1) {}
};

// Проверка: символ — нетерминал (заглавная буква)
bool is_nonterm(char c) { return c >= 'A' && c <= 'Z'; }
bool is_terminal(char c) { return !is_nonterm(c); }

// Отображение правил: номер_правила -> пара(lhs, rhs_string)
map<int, pair<char, string>> productions;

shared_ptr<Node> deep_copy(const shared_ptr<Node> &root)
{
    if (!root)
        return nullptr;
    auto n = make_shared<Node>(root->sym);
    n->rule = root->rule;
    for (auto &c : root->ch)
        n->ch.push_back(deep_copy(c));
    return n;
}

// Собрать листья (слева направо) в вектор
void collect_leaves(const shared_ptr<Node> &node, vector<shared_ptr<Node>> &leaves)
{
    if (node->ch.empty())
    {
        leaves.push_back(node);
    }
    else
    {
        for (auto &c : node->ch)
            collect_leaves(c, leaves);
    }
}

// Раскрыть лист 'leaf' с помощью продукции (lhs -> rhs) и установить номер правила
void expand_leaf(shared_ptr<Node> leaf, int rule_num, const string &rhs)
{
    leaf->rule = rule_num;
    leaf->ch.clear();
    for (char c : rhs)
    {
        auto child = make_shared<Node>(c);
        leaf->ch.push_back(child);
    }
}

// Проверить, что все листья — терминалы
bool leaves_all_terminals(const shared_ptr<Node> &root)
{
    vector<shared_ptr<Node>> leaves;
    collect_leaves(root, leaves);
    for (auto &l : leaves)
        if (is_nonterm(l->sym))
            return false;
    return true;
}

// Построить линейную скобочную форму:
string linear_form(const shared_ptr<Node> &node)
{
    if (node->ch.empty())
    {
        // лист: терминал или нераскрытый нетерминал
        string s(1, node->sym);
        return s;
    }
    else
    {
        string res;
        res.push_back(node->sym);
        if (node->rule != -1)
            res += to_string(node->rule);
        res += "(";
        bool first = true;
        for (auto &c : node->ch)
        {
            if (!first)
                res += "";
            res += linear_form(c);
            first = false;
        }
        res += ")";
        return res;
    }
}

// Собрать последовательность правил левого вывода из дерева:
// посещаем узел: если раскрыт (rule!=-1), добавляем правило, затем рекурсивно обходим детей слева направо
void collect_leftmost_seq(const shared_ptr<Node> &node, vector<int> &seq)
{
    if (node->ch.empty())
        return;
    if (node->rule != -1)
        seq.push_back(node->rule);
    for (auto &c : node->ch)
        collect_leftmost_seq(c, seq);
}

// Поиск в глубину/бэктрекинг для применения заданной последовательности правил
// возвращает true и устанавливает 'result_tree' при успехе
bool dfs_try(const shared_ptr<Node> &tree, const vector<int> &seq, int idx, shared_ptr<Node> &result_tree)
{
    if (idx == (int)seq.size())
    {
        // все правила применены: проверяем, что все листья терминальные
        if (leaves_all_terminals(tree))
        {
            result_tree = tree;
            return true;
        }
        else
            return false;
    }
    int rule_num = seq[idx];
    if (productions.find(rule_num) == productions.end())
        return false;
    char lhs = productions[rule_num].first;
    string rhs = productions[rule_num].second;

    // собрать листья текущего дерева (слева направо), попытаться раскрыть любой лист с символом lhs
    vector<shared_ptr<Node>> leaves;
    collect_leaves(tree, leaves);

    bool anyAttempt = false;
    for (size_t i = 0; i < leaves.size(); ++i)
    {
        if (leaves[i]->sym == lhs)
        {
            anyAttempt = true;
            auto newtree = deep_copy(tree);
            // найти i-й лист в копии:
            vector<shared_ptr<Node>> newleaves;
            collect_leaves(newtree, newleaves);
            // раскрыть
            expand_leaf(newleaves[i], rule_num, rhs);
            // рекурсия
            if (dfs_try(newtree, seq, idx + 1, result_tree))
                return true;
        }
    }
    // Если подходящего LHS не найдено -> тупик
    if (!anyAttempt)
        return false;
    return false;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Инициализация продукций грамматики
    productions[1] = {'S', string("aSbA")};
    productions[2] = {'S', string("aB")};
    productions[3] = {'S', string("A")};
    productions[4] = {'A', string("aAbS")};
    productions[5] = {'A', string("aB")};
    productions[6] = {'A', string("S")};
    productions[7] = {'B', string("b")};
    productions[8] = {'B', string("aA")};

    string line;
    if (!getline(cin, line))
        return 0;
    // обрезать пробелы
    while (!line.empty() && isspace(line.back()))
        line.pop_back();
    while (!line.empty() && isspace(line.front()))
        line.erase(line.begin());

    vector<int> seq;
    if (line.find(' ') != string::npos)
    {
        // разделить по пробелам (поддержка многозначных чисел)
        stringstream ss(line);
        int x;
        while (ss >> x)
            seq.push_back(x);
    }
    else
    {
        // нет пробелов -> считаем каждый символ цифрой (например, "12332")
        for (char c : line)
        {
            if (isdigit(c))
            {
                seq.push_back(c - '0');
            }
            else if (!isspace(c))
            {
                cerr << "Неожиданный символ во входе: '" << c << "'\n";
                return 0;
            }
        }
    }

    if (seq.empty())
    {
        cout << "Пустая последовательность правил. Выход.\n";
        return 0;
    }

    // стартовое дерево: один узел S
    auto start = make_shared<Node>('S');

    shared_ptr<Node> result_tree = nullptr;
    bool ok = dfs_try(start, seq, 0, result_tree);

    if (!ok)
    {
        cout << "Последовательность правил **НЕ** может быть применена для получения терминальной цепочки в заданной грамматике.\n";
        return 0;
    }

    // Построить линейную скобочную форму
    string lin = linear_form(result_tree);

    // Построить терминальную строку v (конкатенация листьев)
    vector<shared_ptr<Node>> leaves;
    collect_leaves(result_tree, leaves);
    string v;
    for (auto &l : leaves)
        v.push_back(l->sym);

    // Построить последовательность правил левого вывода из дерева
    vector<int> left_seq;
    collect_leftmost_seq(result_tree, left_seq);

    cout << "Последовательность правил может быть применена.\n";
    cout << "\nПолученная терминальная цепочка v = \"" << v << "\"\n";
    cout << "\nЛинейная скобочная форма дерева вывода:\n";
    cout << lin << "\n";
    cout << "\nПоследовательность правил при левом выводе, эквивалентном выводу v:\n";
    for (size_t i = 0; i < left_seq.size(); ++i)
    {
        if (i)
            cout << " ";
        cout << left_seq[i];
    }
    cout << "\n";
    return 0;
}
