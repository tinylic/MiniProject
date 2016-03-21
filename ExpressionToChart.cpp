/*
 * ExpressionToChart.cpp
 *
 *  Created on: 2016年3月3日
 *      Author: tinylic
 */

#include "ExpressionToChart.h"



string ExpressionToChart::filter(const string &s) {
	string result = "";
	for (int i = 0; i < s.length(); i++) {
		char ch = s[i];
		if (ch != ' ')
			result.push_back(ch);
	}
	return result;
}

void ExpressionToChart::PushString(string &s, const char &ch) {
	s.push_back(ch);
	s.push_back(' ');
}


int ExpressionToChart::priority(char Operator) {
    for (int i = 3; i >= 0; i--)
        if (Operators[i] == Operator)
            return i;
    return -1;
}

bool ExpressionToChart::IsOperator(char ch) {
    return priority(ch) >= 0;
}

void ExpressionToChart::GetTwoNumbers(stack<bool> &Stack, bool &first, bool &second) {

    second = Stack.top();
    Stack.pop();

    first = Stack.top();
    Stack.pop();
}

bool ExpressionToChart::CalcEq(bool first, bool second, char op) {
	if (op == '|') return first | second;
	if (op == '^') return first ^ second;
	if (op == '&') return first & second;
}
string ExpressionToChart::InfixToPostfix(const string &infix) {

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
    //cout << postfix << endl;
    return postfix;
}

int ExpressionToChart::SolvePostfix(const string &postfix) {
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

string ExpressionToChart::solve(int n, const string &InString) {
    string s;
    s = filter(InString);
	string infix;
	for (int i = 0; i < MAX_N; i++)
		loc[i].clear();

    int len = s.length();
    for (int i = 0; i < len; i++)
		if (isalpha(s[i])) {
			loc[s[i] - 'A'].push_back(i);
		}
	//int cnt;
	//for (cnt = 0; loc[cnt].size() != 0; cnt++);
    string ans = "";
	for (int mask = (1 << n) - 1; mask >= 0; mask--) {
		infix = s;
		for (int i = 0; i < n; i++) {
			int value = (mask >> i) & 1;
			for (int j = 0; j < loc[i].size(); j++) {
				int pos = loc[i][j];
				infix[pos] = '0' + value;
			}
		}
		char result = '0' + SolvePostfix(InfixToPostfix(infix));
		ans = ans + result;
	}
	return ans;
}

ExpressionToChart::ExpressionToChart() {
	// TODO Auto-generated constructor stub

}

ExpressionToChart::~ExpressionToChart() {
	// TODO Auto-generated destructor stub
}

