/*
 * ExpressionToChart.h
 *
 *  Created on: 2016年3月3日
 *      Author: tinylic
 */

#ifndef EXPRESSIONTOCHART_H_
#define EXPRESSIONTOCHART_H_

#include "constants.h"

const char Operators[] = {'|', '^', '&', '~'};


class ExpressionToChart {
private:
	stack<char> Stack_Operator;
	stack<bool> Stack_Number;
	vector<int> loc[MAX_N];

	string filter(const string &s);

	void PushString(string &s, const char &ch);

	int priority(char Operator);

	bool IsOperator(char ch);

	void GetTwoNumbers(stack<bool> &Stack, bool &first, bool &second);

	bool CalcEq(bool first, bool second, char op);

	string InfixToPostfix(const string &infix);

	int SolvePostfix(const string &postfix);

public:
	ExpressionToChart();

	virtual ~ExpressionToChart();

	string solve(int n, const string &InString);
};

#endif /* EXPRESSIONTOCHART_H_ */
