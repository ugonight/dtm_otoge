#pragma once

#define PATH_NUM 30
#define CSIZE 64	//カーソルのサイズ

class Mouse
{
public:
	Mouse();
	~Mouse();

	void update();
	void draw();

	void SetCurrent();
	bool GetClick();	//クリックされた瞬間に1を返す
private:
	int mGHandle[3];
	int mGHPath;
	bool mCurrent;
	bool mClick;
	int mTime;	//再生位置
	int mSize;

	struct path {
		int x, y;
		int count;
	};

	struct path mPath[PATH_NUM];
};

