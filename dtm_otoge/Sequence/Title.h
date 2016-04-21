#pragma once
#include "Child.h"

#define FNUM 222

class Parent;
class Select;

class Title :
	public Child
{
private:
	int mFHandle;
	int mTime,mTime2;
	int mBack[3]; //,mLogo[FNUM];
	int mGHTitle,mGHLight,mDestHandle;
	//int mScreen,mMaskHandle;

	int mSHandle;

	int mMoveCount;	//Ÿ‚ÌƒV[ƒ“‚ÉˆÚ“®‚·‚é‚Ü‚Å‚ÌŠÔ

	void HSVtoRGB(int h, int s, int v, int* rgb);

public:
	Title();
	~Title();
	
	Base* update(Parent*);
};

