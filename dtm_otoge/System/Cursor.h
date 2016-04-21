#pragma once
class Cursor
{
private:
	int mX, mY;
	int mGHandle;
	int mSizeX, mSizeY;
	int mClick[2];
 
	int mBeganX[2], mDragX[2];

	int mStop[2];

	int click(int side);	//ここでのクリックはマウスが押されている状態全般を指す
public:
	Cursor();
	~Cursor();

	void update();
	void draw();

	int GetClick(int side);
	int GetDragX(int side);

	void SetStop(int i,int side);
};

