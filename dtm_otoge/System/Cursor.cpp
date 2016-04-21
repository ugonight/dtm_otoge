#include "Cursor.h"
#include "../define.h"
#include <DxLib.h>


Cursor::Cursor()
{
	mGHandle = LoadGraph("data/cursor.png");
	GetGraphSize(mGHandle, &mSizeX, &mSizeY);
	mY = CURSOR_Y;
	mStop[0] = mStop[1] = 0;
	mBeganX[0] = mBeganX[1] = 0;
}


Cursor::~Cursor()
{
}

int Cursor::click(int side) {
	if (!side) {
		return  (GetMouseInput() & MOUSE_INPUT_LEFT) != 0 || CheckHitKey(KEY_INPUT_Z) != 0;
	}
	else {
		return  (GetMouseInput() & MOUSE_INPUT_RIGHT) != 0 || CheckHitKey(KEY_INPUT_X) != 0;
	}

}


void Cursor::update() {
	//クリック判定
	int oneClick[2], MouseX[2], MouseY[2];

	for (int side = 0; side < 2; side++) {
		
		oneClick[side] = 0;	
		//マウスの位置を取得
		GetMousePoint(&MouseX[side], &MouseY[side]);

		if (click(side) != 0)
		{
			if (!mClick[side]) mBeganX[side] = MouseX[side];
			mClick[side] = 1;
		}
		else {
			mClick[side] = 0;
		}


	
		if (!mStop[side]) {
			//if (oneClick[side]) { 
			//	mBeganX[side] = MouseX[side]; 
			//}

			//mDragX[side] = 0;
			//カーソルドラッグ
			if (mClick[side]) {
				//if (windowMode) {
				//	mDragX[side] = (MouseX - mStartX[side]) * 2 / 3;
				//}
				//else {
				mDragX[side] = (MouseX[side] - mBeganX[side]);
				//}
			}
			else {
				mDragX[side] = 0;
				mBeganX[side] = MouseX[side];
			}
		}
		else {
			mDragX[side] = 0;
			mBeganX[side] = MouseX[side];
		}

	}

	int ScX, ScY;
	GetDrawScreenSize(&ScX, &ScY);
	//フルスクリーンの時は中央にマウスを固定
	if (!mClick[0] && !mClick[1] && !windowMode) {
		SetMousePoint(ScX / 2, ScY / 2);
		mDragX[0] = mDragX[1] = 0;
		mBeganX[0] = mBeganX[1] = ScX;
	}

}

void Cursor::draw() {
	for (int side = 0; side < 2; side++) {
		int cursorX = side ? CURSOR_X_RIGHT : CURSOR_X_LEFT;
		if (mClick[side]) {
			DrawExtendGraph(cursorX - mSizeX / 2 + mDragX[side] - 10, mY - mSizeY / 2 - 10, cursorX + mSizeX / 2 + mDragX[side] + 10, mY + mSizeY / 2 + 10, mGHandle, TRUE);
		}
		else {
			DrawGraph(cursorX - mSizeX / 2, mY - mSizeY / 2, mGHandle, TRUE);
		}
	}

	int MouseX, MouseY;
	GetMousePoint(&MouseX, &MouseY);
	/*DrawCircle(MouseX - 8, MouseY - 8, 16, GetColor(0, 0, 0), FALSE);;
	DrawCircle(mBeganX[0] - 8, 100 - 8, 16, GetColor(0, 0, 255), FALSE);
	DrawCircle(mBeganX[1] - 8, 100 - 8, 16, GetColor(255, 0, 0), FALSE);*/

	//if (mClick[LEFT]) {
	//	DrawExtendGraph(WINDOW_W / 4 - mSizeX / 2 + mDragX - 10, mY - mSizeY / 2 - 10, WINDOW_W / 4 + mSizeX / 2 + mDragX + 10, mY + mSizeY / 2 + 10, mGHandle, TRUE);
	//}
	//else {
	//	DrawGraph(WINDOW_W / 4 - mSizeX / 2, mY - mSizeY / 2, mGHandle, TRUE);
	//}

	//if (mClick[RIGHT]) {
	//	DrawExtendGraph(WINDOW_W * 3 / 4 - mSizeX / 2 + mDragX - 10, mY - mSizeY / 2 - 10, WINDOW_W * 3 / 4 + mSizeX / 2 + mDragX + 10, mY + mSizeY / 2 + 10, mGHandle, TRUE);
	//}
	//else {
	//	DrawGraph(WINDOW_W * 3 / 4 - mSizeX / 2, mY - mSizeY / 2, mGHandle, TRUE);
	//}
}

int Cursor::GetClick(int side) { return mClick[side]; }
int Cursor::GetDragX(int side) { return mDragX[side]; }

void Cursor::SetStop(int i, int side) { mStop[side] = i; }
