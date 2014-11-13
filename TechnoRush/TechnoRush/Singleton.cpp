#include "Singleton.h"

bool Singleton::exists = false;
Singleton* Singleton::single = nullptr;

Singleton* Singleton::getInstance()
{
	if(!exists)
	{
		single = new Singleton();
		exists = true;
	}
	return single;
}