#pragma once

#include "Child.h"
#include "../File.h"

#include <vector>
#include <string>

using namespace std;

class Control;
//class File;

class Play :
	public Child
{
public:

	Play(string DataFile,int level,int speed);
	~Play();

	Base* update(Parent*);
private:
	Control* mControl;
	File* mFile;

	File::DATA mData;
	int mLevel;

	vector<File::NOTE> mNotes;	//ÉmÅ[ÉgèÓïÒ

	int mGHready;
	int mSHBGM;
	int mFHtitle, mFHdetail;

	int mBright;
	int mReadyCount;
	bool mSetflag;

	long long mStartTime;

	int mGHResultBack[2], mGHJaket,mGHResult;
	int mCount, mResultCount;
	bool mNewrecord[2];
	int mGHnewrecord;
	int mClear;

	bool goSelect;

	int GHnum[11];
	int GHnum2[10];
	int GHRank[4];

	void update();
	void draw();

	void StartUpdate();
	void StartDraw();

	void ResultUpdate();
	void ResultDraw();

	void record();

};

