#include <iostream>
#include <string>

#include "game.h"
#include "viewport.h"

using namespace std;

int main() {
	try {
		viewport v(640, 480);
		game g(v);
		
		g.init();
		g.run();
		g.stop();

	} catch (const string str) {
		cerr << str << endl;
		cin.get();
		return 1;
	}
}
