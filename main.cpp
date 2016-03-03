#include <iostream>
#include <cstdio>
#include <algorithm>
#include <stack>
#include <string>
#include <cstring>
#include <cctype>
#include <vector>
using namespace std;

const char Operators[] = {'|', '^', '&', '~'};
const int MAX_N = 10;

stack<char> Stack_Operator;
stack<bool> Stack_Number;
vector<int> loc[MAX_N];
vector<int> MinTerm;
vector<int> ImpContained[1 << MAX_N];
vector<int> MinTermCovered[1 << MAX_N];
bool table[1 << MAX_N][1 << MAX_N];
bool contained[1 << MAX_N];

int TotalVariables = 0;
int CountOne(int x) {
	int ans = 0;
	while (x) {
		if (x & 1) ans ++;
		x >>= 1;
	}
	return ans;
}

struct implication {
	int bit;
	int xterm;
	int ones;
	string exp;
	bool used;
	bool selected;
	implication() {
		bit = xterm = ones = used = 0;
		exp = "";
	}

	implication(int mask, int _xterm) {
		bit = mask;
		xterm = _xterm;
		ones = CountOne(mask);
		exp = "";
		for (int i = 0; i < TotalVariables; i++) {
			if (_xterm & 1)
				exp = exp + '-';
			else
				exp = exp + to_string((mask & 1) != 0);
			_xterm = _xterm >> 1;
			mask = mask >> 1;
		}
		used = selected = false;
	}

	string show() {
		string ans = "";
		for (int i = TotalVariables - 1; i >= 0; i--) {
			bool hidden = (xterm >> i) & 1;
			if (!hidden) {
				ans += ('A' + TotalVariables - 1 - i);
				if (!((bit >> i) & 1))
					ans += '\'';
			}
		}
		return ans;
	}

};

vector<implication> imp, roller;
vector<implication> primes;
bool cmp(const implication &a, const implication &b) {
	return a.ones < b.ones;
}



string filter(char *s) {
	string result = "";
	for (int i = 0; i < strlen(s); i++) {
		char ch = s[i];
		if (ch != ' ')
			result.push_back(ch);
	}
	return result;
}

void PushString(string &s, const char &ch) {
	s.push_back(ch);
	s.push_back(' ');
}


int priority(char Operator) {
    for (int i = 3; i >= 0; i--)
        if (Operators[i] == Operator)
            return i;
    return -1;
}

bool IsOperator(char ch) {
    return priority(ch) >= 0;
}

void GetTwoNumbers(stack<bool> &Stack, bool &first, bool &second) {

    second = Stack.top();
    Stack.pop();

    first = Stack.top();
    Stack.pop();
}

bool CalcEq(bool first, bool second, char op) {
	if (op == '|') return first | second;
	if (op == '^') return first ^ second;
	if (op == '&') return first & second;
}
string InfixToPostfix(const string &infix) {

	string postfix = "";
	while (!Stack_Operator.empty()) {
		Stack_Operator.pop();
	}

	for (auto ch : infix) {
		if (IsOperator(ch)) {
			while (!Stack_Operator.empty() && IsOperator(Stack_Operator.top()) && priority(Stack_Operator.top()) >= priority(ch)) {
				PushString(postfix, Stack_Operator.top());
                Stack_Operator.pop();
			}
			Stack_Operator.push(ch);
		}
		else {
			switch(ch) {
				case '(' :
					Stack_Operator.push(ch);
					break;
				case ')' :
					while(!Stack_Operator.empty() && Stack_Operator.top() != '(') {
						PushString(postfix, Stack_Operator.top());
						Stack_Operator.pop();
					}
					Stack_Operator.pop();
					break;
				default :
					PushString(postfix, ch);
			}
        }
    }
    while (!Stack_Operator.empty()) {
		PushString(postfix, Stack_Operator.top());
		Stack_Operator.pop();
    }

    postfix.pop_back();
    cout << postfix << endl;
    return postfix;
}

int SolvePostfix(const string &postfix) {
	while (!Stack_Number.empty()) {
		Stack_Number.pop();
	}

	for (auto ch : postfix) {
		if (IsOperator(ch)) {
			if (ch == '~') {
				bool x = Stack_Number.top();
				Stack_Number.pop();
				Stack_Number.push(!x);
			}
			else {
				bool x, y;
				GetTwoNumbers(Stack_Number, x, y);
				bool result = CalcEq(x, y, ch);
				Stack_Number.push(result);
			}
		}
		else
			if (ch != ' '){
				Stack_Number.push(ch - '0');
			}
	}

	int ans = Stack_Number.top();
	Stack_Number.pop();
	return ans;
}

void solve(const string &s) {
	string infix;
	for (int i = 0; i < MAX_N; i++)
		loc[i].clear();

    int len = s.length();
    for (int i = 0; i < len; i++)
		if (isalpha(s[i])) {
			loc[s[i] - 'A'].push_back(i);
		}
	int cnt;
	for (cnt = 0; loc[cnt].size() != 0; cnt++);
	for (int mask = (1 << cnt) - 1; mask >= 0; mask--) {
		infix = s;
		for (int i = 0; i < cnt; i++) {
			int value = (mask >> i) & 1;
			for (int j = 0; j < loc[i].size(); j++) {
				int pos = loc[i][j];
				infix[pos] = '0' + value;
			}
		}
		cout << infix << " = " << SolvePostfix(InfixToPostfix(infix)) << endl;
	}
}

void ExpressionToChart() {
	char InString[255];
    string s;
    gets(InString);
    s = filter(InString);
    solve(s);
}

bool CheckContained(const implication &imp, const int &x) {
	int t = x;
	for (int i = 0; i < TotalVariables; i++) {
		int value = t & 1;
		t >>= 1;
		if (imp.exp[i] == '-') continue;
		if (value != (imp.exp[i] - '0')) return false;
	}
	return true;
}

void Simplify() {
	for (int i = 0; i < MinTerm.size(); i++)
		if (MinTermCovered[i].size() == 1) {
			int index = MinTermCovered[i][0];
			primes[index].selected = true;
			for (int j = 0; j < ImpContained[index].size(); j++)
				contained[ImpContained[index][j]] = true;
		}
	while (true) {
		bool stop = true;
		for (int i = 0; i < MinTerm.size(); i++)
			if (contained[i] == false)
				stop = false;
		if (stop) break;
		for (int i = 0; i < MinTerm.size(); i++)
			if (contained[i] == false) {
				int index = MinTermCovered[i][0];
				primes[index].selected = true;
				for (int j = 0; j < ImpContained[index].size(); j++)
					contained[ImpContained[index][j]] = true;
			}
	}
}
void ChartToExpression() {
	memset(table, false, sizeof table);
	memset(contained, false, sizeof contained);
	imp.clear();
	int x;
	while ((scanf("%d", &x)) != EOF) {
		MinTerm.push_back(x);
	}
	sort(MinTerm.begin(), MinTerm.end());
	for (;(1 << TotalVariables) <= MinTerm[MinTerm.size() - 1]; TotalVariables ++);
	cout << TotalVariables << endl;
	for (int i = 0; i < MinTerm.size(); i++)
		imp.push_back(implication(MinTerm[i], 0));
	sort(imp.begin(), imp.end(), cmp);
	while (imp.size() > 0) {
		roller.clear();
		for (int i = 0; i < imp.size(); i++)
			for (int j = i + 1; j < imp.size(); j++)
				if (imp[i].exp == imp[j].exp) {
					imp.erase(imp.begin() + j);
				}
		for (int i = 0; i < imp.size(); i++)
			for (int j = i + 1; j < imp.size(); j++)
				if (imp[j].ones == imp[i].ones + 1 && imp[j].xterm == imp[i].xterm && CountOne(imp[j].bit ^ imp[i].bit) == 1) {
					imp[i].used = true;
					imp[j].used = true;
					implication NewImp = implication(imp[i].bit, imp[i].bit ^ imp[j].bit | imp[i].xterm);
					roller.push_back(NewImp);
				}
		for (int i = 0; i < imp.size(); i++)
			if (imp[i].used == false)
				primes.push_back(imp[i]);
		sort(roller.begin(), roller.end(), cmp);
		imp = roller;
		cout << "---------------------------" << endl;
		for (int i = 0; i < imp.size(); i++)
			cout << imp[i].ones << "\t" << imp[i].exp  << "\t" << (imp[i].used ? 'X' : ' ') << endl;
	}
	sort(primes.begin(), primes.end(), cmp);
	for (int i = 0; i < primes.size(); i++)
		ImpContained[i].clear();
	for (int i = 0; i < MinTerm.size(); i++)
		MinTermCovered[i].clear();
	for (int i = 0; i < primes.size(); i++)
		for (int j = 0; j < MinTerm.size(); j++)
			if (CheckContained(primes[i], MinTerm[j])) {
				table[i][j] = true;
				ImpContained[i].push_back(j);
				MinTermCovered[j].push_back(i);
			}
	for (int i = 0; i < MinTerm.size(); i++)
		cout << "  \t" << MinTerm[i];
	cout << endl;
    for (int i = 0; i < primes.size(); i++) {
		cout << primes[i].exp;
		for (int j = 0; j < MinTerm.size(); j++) {
			cout << "  \t";
			if (table[i][j]) cout << "X";
		}
		cout << endl;
    }
    Simplify();
    bool head = true;
	for (int i = 0; i < primes.size(); i++)
		if (primes[i].selected){
			if (!head) cout << " + ";
			cout << primes[i].show();
			head = false;
		}
}
int main() {
	//ExpressionToChart();
	ChartToExpression();
}

