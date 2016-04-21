#include "Child.h"
#include "Parent.h"

Child::Child()
{
}


Child::~Child()
{
}

Base* Child::update(Base* p) {
	Parent* parent = dynamic_cast< Parent* >(p);
	return update(parent);
}
