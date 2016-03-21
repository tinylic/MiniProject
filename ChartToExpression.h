/*
 * ChartToExpression.h
 *
 *  Created on: 2016年3月3日
 *      Author: tinylic
 */

#ifndef CHARTTOEXPRESSION_H_
#define CHARTTOEXPRESSION_H_

#include "constants.h"
#include "implication.h"

using namespace std;

class ChartToExpression {

private:
	vector<int> MinTerm;

	vector<int> MinTermCovered[1 << MAX_N];

	bool table[1 << MAX_N][1 << MAX_N];

	bool contained[1 << MAX_N];

	int TotalVariables;

	vector<implication> imp, roller;

	vector<implication> primes;

	bool CheckContained(const implication &imp, const int &x);

	int CountOne(int x);

	vector<implication*> UPI;
public:
	ChartToExpression();

	virtual ~ChartToExpression();

	void Simplify();

	void Quine_McCluskey();

	string solve(const string &truth_table);

	void ShowTable();
};

#endif /* CHARTTOEXPRESSION_H_ */
