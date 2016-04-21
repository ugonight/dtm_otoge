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
	if (mCount == 0) { //1�t���[���ڂȂ玞�����L��
		mStartTime = GetNowCount();
	}
	if (mCount == N) { //60�t���[���ڂȂ畽�ς��v�Z����
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
	int tookTime = GetNowCount() - mStartTime;	//������������
	int waitTime = mCount * 1000 / FPS - tookTime;	//�҂ׂ�����
	if (waitTime > 0) {
		Sleep(waitTime);	//�ҋ@
	}
}
