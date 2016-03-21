/*
 * implication.h
 *
 *  Created on: 2016年3月17日
 *      Author: tinylic
 */

#ifndef IMPLICATION_H_
#define IMPLICATION_H_

#include "constants.h"

class implication {

private:
	friend bool operator < (const implication &a, const implication &b) {
			return a.ones < b.ones;
	}

public:
	int bit;
	int xterm;
	int ones;
	string exp;
	bool used;
	bool selected;
	vector<int> ImpContained;


	int CountOne(int x);

	int TotalVariables;

	implication();
	virtual ~implication();

	implication(int mask, int _xterm, int _TotalVariables);

	string show();

};

#endif /* IMPLICATION_H_ */
