/*
 * implication.cpp
 *
 *  Created on: 2016年3月17日
 *      Author: tinylic
 */

#include "implication.h"

implication::implication() {
	bit = xterm = ones = used = 0;
	exp = "";
	ImpContained.clear();
}


implication::implication(int mask, int _xterm, int _TotalVariables) {
		bit = mask;
		xterm = _xterm;
		ones = CountOne(mask);
		TotalVariables = _TotalVariables;
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

implication::~implication() {
	// TODO Auto-generated destructor stub
}

int implication::CountOne(int x) {
	int ans = 0;
	while (x) {
		if (x & 1) ans ++;
		x >>= 1;
	}
	return ans;
}

string implication::show() {
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
