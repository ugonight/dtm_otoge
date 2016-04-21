#pragma once

class fps
{
	int mStartTime;         //測定開始時刻
	int mCount;             //カウンタ
	float mFps;             //fps
	static const int N = 60;//平均を取るサンプル数
	static const int FPS = 60;	//設定したFPS
public:
	fps();
	~fps();

	bool Update();
	void Draw();
	void Wait();
};

