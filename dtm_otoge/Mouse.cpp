#include "Mouse.h"
#include "define.h"


Mouse::Mouse()
{
	mGHandle[0] = LoadGraph("data/cursor.avi");
	mGHandle[1] = LoadGraph("data/cursor2.avi");
	mGHandle[2] = LoadGraph("data/cursor3.avi");
	mGHPath = LoadGraph("data/path.png");

	mCurrent = 0, mTime = 0,mClick=0;
	mSize = CSIZE;

	//全部再生しておく
	PlayMovieToGraph(mGHandle[0], DX_PLAYTYPE_LOOP);
	PlayMovieToGraph(mGHandle[1], DX_PLAYTYPE_LOOP);
	PlayMovieToGraph(mGHandle[2], DX_PLAYTYPE_LOOP);

	for (int i = 0; i < PATH_NUM; i++) {
		mPath[i].count = 0;
	}
}


Mouse::~Mouse()
{
}

void Mouse::update() {
	//mTime+=60;
	//mTime %= 5000; //5秒経ったらカウントをリセット


	if (mSize > CSIZE) mSize -= 2;
	
	if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0 || (GetMouseInput() & MOUSE_INPUT_RIGHT) != 0) {
		if (!mClick) mSize = CSIZE * 2;
		mClick = 1;
	}
	else {
		mClick = 0;
	}


	int MouseX, MouseY;
	GetMousePoint(&MouseX, &MouseY);

	int i;
	for (i = 0; i < PATH_NUM; i++) if (mPath[i].count == 0) break;	//使われていないのを探す
	mPath[i].count = PATH_NUM;
	mPath[i].x = MouseX;	mPath[i].y = MouseY;

	for (i = 0; i < PATH_NUM; i++) {
		if (mPath[i].count>0) mPath[i].count--;
	}
}

void Mouse::draw() {
	int MouseX, MouseY;
	GetMousePoint(&MouseX, &MouseY);

	//軌跡の描画
	for (int i = 0; i < PATH_NUM; i++) {
		if (MouseX != mPath[i].x && MouseY != mPath[i].y) {
			SetDrawBlendMode(DX_BLENDMODE_ADD, 100 / PATH_NUM * mPath[i].count);
			DrawGraph(mPath[i].x - 16, mPath[i].y - 16, mGHPath, TRUE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
	}

	try {
		//マウスカーソルの描画
		SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
		if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0 || (GetMouseInput() & MOUSE_INPUT_RIGHT) != 0) {
			if (GetMovieStateToGraph(mGHandle[2])) DrawExtendGraph(MouseX - mSize / 2, MouseY - mSize / 2, MouseX + mSize / 2, MouseY + mSize / 2, mGHandle[2], TRUE);	//ここでなぜかアクセス違反
		}
		else {
			if (GetMovieStateToGraph(mGHandle[mCurrent])) DrawExtendGraph(MouseX - mSize / 2, MouseY - mSize / 2, MouseX + mSize / 2, MouseY + mSize / 2, mGHandle[mCurrent], TRUE);
		}
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	catch (char* s) {
		printf(s);
	}

	mCurrent = 0;
}

void Mouse::SetCurrent() { mCurrent = 1; }
bool Mouse::GetClick() { return mSize == CSIZE * 2; }