/*
* Author: Ben Robbins
* Code from http://www.codeproject.com/Articles/1921/Singleton-Pattern-its-implementation-with-C
*/

#pragma once
class Singleton
{
public:
	static Singleton* getInstance();
	static bool exists;
private:
	Singleton(){}
	virtual ~Singleton(){ exists = false; }
	static Singleton* single;
};

