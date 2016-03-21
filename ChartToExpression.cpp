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

void ChartToExpression::Simplify() {
	for (unsigned i = 0; i < MinTerm.size(); i++)
		if (MinTermCovered[i].size() == 1) {
			int index = MinTermCovered[i][0];
			primes[index].selected = true;
			for (unsigned j = 0; j < primes[index].ImpContained.size(); j++)
				contained[primes[index].ImpContained[j]] = true;
		}

	//petrick
	UPI.clear();
	for (unsigned i = 0; i < primes.size(); i++)
		if (primes[i].selected == false)
			UPI.push_back(&primes[i]);
	bool tContained[1 << MAX_N];
	int MinOne = 1 << MAX_N;
	int result = 0;
	for (int mask = 0; mask < (1 << (UPI.size())); mask++) {
		memset(tContained, false, sizeof tContained);
		for (unsigned i = 0; i < UPI.size(); i++) {
			int value = (mask >> i) & 1;
			implication *mImp = UPI[i];
			if (value) {
				for (unsigned j = 0; j < mImp->ImpContained.size(); j++)
					tContained[mImp->ImpContained[j]] = true;
			}
		}
		bool valid = true;
		for (unsigned i = 0; i < MinTerm.size(); i++)
			if (contained[i] == false && tContained[i] == false)
				valid = false;
		if (valid) {
			int ones = CountOne(mask);
			if (MinOne > ones) {
				MinOne = ones;
				result = mask;
			}
		}
	}
	for (int i = 0; i < UPI.size(); i++) {
		int value = (result >> i) & 1;
		if (value)
			UPI[i]->selected = true;
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
		cout << "---------------------------" << endl;
		for (int i = 0; i < imp.size(); i++)
			cout << imp[i].ones << "\t" << imp[i].exp << "\t"
					<< (imp[i].used ? 'X' : ' ') << endl;
	}
}
string ChartToExpression::solve(const string &truth_table) {
	memset(table, false, sizeof table);
	memset(contained, false, sizeof contained);
	imp.clear();
	//get min terms
	for (int i = 0; i < truth_table.length(); i++)
		if (truth_table[i] == '1') {
			MinTerm.push_back(truth_table.length() - i - 1);
		}
	sort(MinTerm.begin(), MinTerm.end());

	//get variable numbers
	for (; (1 << TotalVariables) <= MinTerm[MinTerm.size() - 1];
			TotalVariables++);

	//get initial implications
	for (int i = 0; i < MinTerm.size(); i++)
		imp.push_back(implication(MinTerm[i], 0, TotalVariables));
	sort(imp.begin(), imp.end());

	Quine_McCluskey();

	sort(primes.begin(), primes.end());
	for (int i = 0; i < MinTerm.size(); i++)
		MinTermCovered[i].clear();
	for (int i = 0; i < primes.size(); i++)
		for (int j = 0; j < MinTerm.size(); j++)
			if (CheckContained(primes[i], MinTerm[j])) {
				table[i][j] = true;
				primes[i].ImpContained.push_back(j);
				MinTermCovered[j].push_back(i);
			}


	ShowTable();

	Simplify();

	bool head = true;
	string ans = "";
	for (int i = 0; i < primes.size(); i++)
		if (primes[i].selected) {
			if (!head)
				ans = ans + " + ";
			ans = ans + primes[i].show();
			head = false;
		}
	return ans;
}

void ChartToExpression::ShowTable() {
	for (int i = 0; i <= TotalVariables; i++)
		cout << " ";
	for (int i = 0; i < MinTerm.size(); i++)
		cout << "\t" << MinTerm[i];
	cout << endl;
	for (int i = 0; i < primes.size(); i++) {
		cout << primes[i].exp << " ";
		for (int j = 0; j < MinTerm.size(); j++) {
			if (table[i][j])
				cout << "X";
			cout << "\t";
		}
		cout << endl;
	}
}
