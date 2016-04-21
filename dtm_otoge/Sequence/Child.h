#pragma once
#include "Base.h"

class Parent;

class Child :
	public Base
{
public:
	Child();
	virtual ~Child();
	
	Base* update(Base*);
	virtual Base* update(Parent*) = 0;
};

