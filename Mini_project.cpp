//============================================================================
// Name        : Mini_project.cpp
// Author      : Li Chen
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "ExpressionToChart.h"
#include "ChartToExpression.h"
using namespace std;
string expr_to_truthtable(int n, const string& expr) {
	ExpressionToChart ETC;
	return ETC.solve(n, expr);
}

string truthtable_to_expr(const string& truth_table) {
	ChartToExpression CTE;
	return CTE.solve(truth_table);
}

int main() {
	string expr;
	string truth_table;
	int n;
	//cin >> n;
	getline(cin, truth_table);
	//cout << expr_to_truthtable(n, expr);
	cout << truthtable_to_expr(truth_table);
	//ETC.solve();
	//cout << CTE.solve();
	//cout << "Hello\n";
	return 0;
}
