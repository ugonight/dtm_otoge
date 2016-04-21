#include "fps.h"
#include <DxLib.h>
#include <math.h>


fps::fps()
{
	mStartTime = 0;
	mCount = 0;
	mFps = 0;
}


fps::~fps()
{
}

bool fps::Update() {
	if (mCount == 0) { //1フレーム目なら時刻を記憶
		mStartTime = GetNowCount();
	}
	if (mCount == N) { //60フレーム目なら平均を計算する
		int t = GetNowCount();
		mFps = 1000.f / ((t - mStartTime) / (float)N);
		mCount = 0;
		mStartTime = t;
	}
	mCount++;
	return true;
}

void fps::Draw() {
	DrawFormatString(0, 0, GetColor(255, 255, 255), "FPS:%.1f", mFps);
}

void fps::Wait() {
	int tookTime = GetNowCount() - mStartTime;	//かかった時間
	int waitTime = mCount * 1000 / FPS - tookTime;	//待つべき時間
	if (waitTime > 0) {
		Sleep(waitTime);	//待機
	}
}
