/*
 * ChartToExpression.cpp
 *
 *  Created on: 2016年3月3日
 *      Author: tinylic
 */

#include "ChartToExpression.h"

ChartToExpression::ChartToExpression() {
	// TODO Auto-generated constructor stub
	TotalVariables = 0;
}

ChartToExpression::~ChartToExpression() {
	// TODO Auto-generated destructor stub
}

int ChartToExpression::CountOne(int x) {
	int ans = 0;
	while (x) {
		if (x & 1)
			ans++;
		x >>= 1;
	}
	return ans;
}

bool ChartToExpression::CheckContained(const implication &imp, const int &x) {
	int t = x;
	for (int i = 0; i < imp.TotalVariables; i++) {
		int value = t & 1;
		t >>= 1;
		if (imp.exp[i] == '-')
			continue;
		if (value != (imp.exp[i] - '0'))
			return false;
	}
	return true;
}

void ChartToExpression::mul(vector<int> &a, const vector<int> &b) {
	vector<int> v;
	for (int i = 0; i < a.size(); i++)
		for (int j = 0; j < b.size(); j++)
			v.push_back(a[i] | b[j]);
	sort(v.begin(), v.end());
	v.erase(unique(v.begin(), v.end()), v.end());
	for (int i = 0; i < v.size() - 1; i++)
		for (int j = v.size() - 1; j > i; --j) {
			int z = v[i] & v[j];
			if ((z & v[i]) == v[i])
				v.erase(v.begin() + j);
			else if ((z & v[j]) == v[j]) {
				size_t t = v[i];
				v[i] = v[j];
				v[j] = t;
				v.erase(v.begin() + j);
				j = v.size();
			}
		}
	a = v;
}

void ChartToExpression::Simplify() {
	//petrick
	M0.clear();
	for (int i = 0; i < (int)primes.size(); i++)
		if (table[i][0])
			M0.push_back(1 << i);
	for (int k = 1; k < (int)MinTerm.size(); k++) {
		M1.clear();
		for (int i = 0; i < (int)primes.size(); i++)
			if (table[i][k])
				M1.push_back(1 << i);
		mul(M0, M1);
	}
}

void ChartToExpression::Quine_McCluskey() {
	//q-m
	while (imp.size() > 0) {
		roller.clear();
		for (int i = 0; i < imp.size(); i++)
			for (int j = i + 1; j < imp.size(); j++)
				if (imp[i].exp == imp[j].exp) {
					imp.erase(imp.begin() + j);
				}
		for (int i = 0; i < imp.size(); i++)
			for (int j = i + 1; j < imp.size(); j++)
				if (imp[j].ones == imp[i].ones + 1
						&& imp[j].xterm == imp[i].xterm
						&& CountOne(imp[j].bit ^ imp[i].bit) == 1) {
					imp[i].used = true;
					imp[j].used = true;
					implication NewImp = implication(imp[i].bit,
							(imp[i].bit ^ imp[j].bit) | imp[i].xterm,
							TotalVariables);
					roller.push_back(NewImp);
				}
		for (int i = 0; i < imp.size(); i++)
			if (imp[i].used == false)
				primes.push_back(imp[i]);
		sort(roller.begin(), roller.end());
		imp = roller;
	}
}
string ChartToExpression::solve(const string &truth_table) {
	memset(table, false, sizeof table);
	MinTerm.clear();
	primes.clear();
	imp.clear();
	//cerr << truth_table << endl;
	if (truth_table.length() == 0) {
		throw EmptyStringError { };
	}
	//get variable numbers
	for (TotalVariables = 1; (1 << TotalVariables) < truth_table.length();
			TotalVariables++)
		;
	if (truth_table.length() != (1 << TotalVariables)) {
		throw InvalidLengthError { };
	}

	for (int i = 0; i < truth_table.length(); i++) {
		if (truth_table[i] != '0' && truth_table[i] != '1') {
			throw InvalidCharError { };
		}
		if (truth_table[i] == '1') {
			MinTerm.push_back(truth_table.length() - i - 1);
		}
	}
	if (MinTerm.size() == truth_table.length())
		return "1";
	if (MinTerm.size() == 0)
		return "0";
	sort(MinTerm.begin(), MinTerm.end());

	//get initial implications
	for (int i = 0; i < MinTerm.size(); i++)
		imp.push_back(implication(MinTerm[i], 0, TotalVariables));
	sort(imp.begin(), imp.end());

	Quine_McCluskey();

	sort(primes.begin(), primes.end());
	for (int i = 0; i < primes.size(); i++)
		for (int j = 0; j < MinTerm.size(); j++)
			if (CheckContained(primes[i], MinTerm[j]))
				table[i][j] = true;

	//ShowTable();

	Simplify();

	int MinOne = CountOne(M0[0]);
	int mask = 0;
	for (int i = 1; i < M0.size(); i++) {
		if (MinOne > CountOne(M0[i])) {
			MinOne = CountOne(M0[i]);
			mask = i;
		}
	}
	bool head = true;
	string ans = "";
	for (int i = 0; i < primes.size(); i++)
		if ((M0[mask] >> i) & 1) {
			if (!head)
				ans = ans + " | ";
			ans = ans + primes[i].show();
			head = false;
		}
	return ans;
}

void ChartToExpression::ShowTable() {
	for (int i = 0; i <= TotalVariables; i++)
		cerr << " ";
	for (int i = 0; i < MinTerm.size(); i++)
		cerr << "\t" << MinTerm[i];
	cerr << endl;
	for (int i = 0; i < primes.size(); i++) {
		cerr << primes[i].exp << " ";
		for (int j = 0; j < MinTerm.size(); j++) {
			if (table[i][j])
				cerr << "X";
			cerr << "\t";
		}
		cerr << endl;
	}
}
