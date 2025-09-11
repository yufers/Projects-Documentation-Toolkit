#include <bits/stdc++.h>
using namespace std;

/* ---------- 1. Структура правила и инициализация грамматики ---------- */
struct Rule {
    string lhs;   // левые члены (непустой символ)
    string rhs;   // правые члены
    int    num;   // номер правила
};

static const vector<Rule> rules = {
    {"S", "aSbA", 1},
    {"S", "aB",   2},
    {"S", "A",    3},
    {"A", "aAbS", 4},
    {"A", "aB",   5},
    {"A", "S",    6},
    {"B", "b",    7},
    {"B", "aA",   8}
};

/* ---------- 2. Узел дерева вывода ---------- */
struct Node {
    string  symbol;          // символ
    bool    terminal;        // true – терминал
    int     ruleNum = -1;    // номер правила, которым он был развернут
    vector<Node*> children;  // потомки

    Node(const string& s, bool t = true) : symbol(s), terminal(t) {}
};

/* ---------- 3. Поиск правила по номеру ---------- */
const Rule* findRule(int num) {
    for (const auto& r : rules)
        if (r.num == num) return &r;
    return nullptr;
}

void printNode(Node* n, string &out) {
    if (n->terminal) {
        out += n->symbol;
        return;
    }
    out += '(' + n->symbol + to_string(n->ruleNum);
    for (Node* c : n->children) printNode(c, out);
    out += ')';
}

/* ---------- 5. Главная функция ---------- */
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    /* ---------- ввод последовательности номеров правил ---------- */
    vector<int> seqRules;
    int x;
    while (cin >> x) seqRules.push_back(x);

    /* ---------- инициализация дерева и списка текущих нод ---------- */
    Node* root = new Node("S", false);
    vector<Node*> curList = {root};

    bool ok = true;

    for (int ruleNum : seqRules) {
        const Rule* r = findRule(ruleNum);
        if (!r) { ok = false; break; }

        /* поиск левого самого первого нерегулярного символа */
        size_t pos = 0;
        while (pos < curList.size() && curList[pos]->terminal) ++pos;
        if (pos == curList.size()) { ok = false; break; }

        Node* target = curList[pos];
        if (target->symbol != r->lhs) { ok = false; break; }

        /* развернуть правило */
        target->ruleNum = r->num;
        target->terminal = false;
        target->children.clear();

        /* создаём потомков и обновляем список текущих нод */
        vector<Node*> newNodes;
        for (char ch : r->rhs) {
            if (isupper(ch)) {               // нетерминал
                Node* child = new Node(string(1, ch), false);
                target->children.push_back(child);
                newNodes.push_back(child);
            } else {                         // терминал
                Node* child = new Node(string(1, ch), true);
                target->children.push_back(child);
                newNodes.push_back(child);
            }
        }

        curList.erase(curList.begin() + pos, curList.begin() + pos + 1);
        curList.insert(curList.begin() + pos, newNodes.begin(), newNodes.end());
    }

    /* ---------- проверка, что все стали терминалами ---------- */
    for (Node* n : curList)
        if (!n->terminal) ok = false;

    /* ---------- вывод результата ---------- */
    if (!ok) {
        cout << "Невозможно применить заданную последовательность правил.\n";
        return 0;
    }

    /* строка левого вывода */
    string derivation;
    for (Node* n : curList) derivation += n->symbol;
    cout << "Возможно применить заданную последовательность правил.\n";
    cout << "Левый вывод: " << derivation << '\n';

    /* линейная скобочная форма */
    string linear;
    printNode(root, linear);
    cout << "Линейная скобочная форма: " << linear << '\n';

    return 0;
}