#include "stdafx.h"
#include <string>
#include <iostream>

using namespace std;

class A {
public:


};

std::ostream & operator<<(std::ostream & Str, A const & v) {
	// print something from v to str, e.g: Str << v.getX();
	Str << "HEllo";
	return Str;
}

/*int main()
{

	A a;
	cout << a;
	return 0;
}*/