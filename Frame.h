// Definition of the Frame class

#ifndef Frame_class
#define Frame_class

#include <iostream>
#include <string>

using namespace std;
 
class Frame
{
	public:
   		Frame(){data="";parity=0;};
   		virtual ~Frame(){};

	//private:
		string data;
		int parity;
};

#endif
