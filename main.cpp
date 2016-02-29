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
int main() {
	char InString[255];
    string s;
    gets(InString);
    s = filter(InString);
    solve(s);
	//cout << SolvePostfix(InfixToPostfix(infix)) << endl;
}

