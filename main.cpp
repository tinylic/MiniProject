#include <iostream>
#include <cstdio>
#include <algorithm>
#include <stack>
#include <string>
#include <cstring>
#include <cctype>

using namespace std;

const char Operators[] = {'|', '^', '&', '~'};

stack<char> Stack_Operator;
stack<int> Stack_Number;

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

void GetTwoNumbers(stack<int> &Stack, int &first, int &second) {

    second = Stack.top();
    Stack.pop();

    first = Stack.top();
    Stack.pop();
}

int CalcEq(int first, int second, char op) {
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
				int x = Stack_Number.top();
				Stack_Number.pop();
				Stack_Number.push(~x);
			}
			else {
				int x, y;
				GetTwoNumbers(Stack_Number, x, y);
				int result = CalcEq(x, y, ch);
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

int main() {
	char InString[255];
    string infix, postfix;
    gets(InString);
    infix = filter(InString);
	cout << SolvePostfix(InfixToPostfix(infix)) << endl;
}

