#include "Notes.h"
#include "../define.h"
#include <DxLib.h>
#include <math.h>

int Notes::mGHandleL = -1;	int Notes::mGHandleR = -1;
int Notes::mGHholdL = -1;	int Notes::mGHholdR = -1;
int Notes::mGHholdLE = -1;	int Notes::mGHholdRE = -1;
int Notes::mGHSlideD[2] = { -1,-1 };
int Notes::mGHSlideL[2] = { -1,-1 };	int Notes::mGHSlideR[2] = { -1,-1 };
int Notes::mGHtouch[2] = { -1,-1 };
int Notes::mSpeed = 0;
//long long Notes::mStartTime = 0;
//int Notes::mGHholdingL = -1;	int Notes::mGHholdingR = -1;

Notes::Notes()
{
	//if (mGHandleL == -1) mGHandleL = LoadGraph("data/notesL.png");
	//if (mGHandleR == -1) mGHandleR = LoadGraph("data/notesR.png");
	//if (mGHholdL == -1) mGHholdL = LoadGraph("data/holdL_s.png");
	//if (mGHholdR == -1) mGHholdR = LoadGraph("data/holdR_s.png");
	//if (mGHholdLE == -1) mGHholdLE = LoadGraph("data/holdL_e.png");
	//if (mGHholdRE == -1) mGHholdRE = LoadGraph("data/holdR_e.png");
	//if (mGHSlideL[0] == -1) mGHSlideL[0] = LoadGraph("data/slideL_L.png");
	//if (mGHSlideL[1] == -1) mGHSlideL[1] = LoadGraph("data/slideR_L.png");
	//if (mGHSlideR[0] == -1) mGHSlideR[0] = LoadGraph("data/slideL_R.png");
	//if (mGHSlideR[1] == -1) mGHSlideR[1] = LoadGraph("data/slideR_R.png");
	//if (mGHtouch[0] == -1) mGHtouch[0] = LoadGraph("data/touchL.png");
	//if (mGHtouch[1] == -1) mGHtouch[1] = LoadGraph("data/touchR.png");
	//if (mGHSlideD[0] == -1) mGHSlideD[0] = LoadGraph("data/direcL.png");
	//if (mGHSlideD[1] == -1) mGHSlideD[1] = LoadGraph("data/direcR.png");

	//if (mGHholdingL == -1)mGHholdingL = LoadGraph("data/holdL_n.png");
	//if (mGHholdingR == -1)mGHholdingR = LoadGraph("data/holdR_n.png");
	//GetGraphSize(mGHandleL, &mSizeX, &mSizeY);
	mShowFlag = 0;
	mMiss = 0;
	mTime = 0;
	//mHoldS = 0;
}

void Notes::LoadNoteGraph() {
	 mGHandleL = LoadGraph("data/notesL.png");
	 mGHandleR = LoadGraph("data/notesR.png");
	 mGHholdL = LoadGraph("data/holdL_s.png");
	 mGHholdR = LoadGraph("data/holdR_s.png");
	 mGHholdLE = LoadGraph("data/holdL_e.png");
	 mGHholdRE = LoadGraph("data/holdR_e.png");
	 mGHSlideL[0] = LoadGraph("data/slideL_L.png");
	 mGHSlideL[1] = LoadGraph("data/slideR_L.png");
	 mGHSlideR[0] = LoadGraph("data/slideL_R.png");
	 mGHSlideR[1] = LoadGraph("data/slideR_R.png");
	 mGHtouch[0] = LoadGraph("data/touchL.png");
	 mGHtouch[1] = LoadGraph("data/touchR.png");
	 mGHSlideD[0] = LoadGraph("data/direcL.png");
	 mGHSlideD[1] = LoadGraph("data/direcR.png");
	 //mStartTime = 0;
}


Notes::~Notes()
{
}

void Notes::update() {
	LONGLONG lltime = mTime - (GetNowHiPerformanceCount() - mStartTime);
	if (!mStop /*&& (GetNowHiPerformanceCount() - mStartTime) > 0*/) {
		//mY += 2;
		mY = -WINDOW_H;
		LONGLONG y = - ((double)lltime / 1000000) * 100 * mSpeed + CURSOR_Y;
		if (y < WINDOW_H * 2) {
			mY = y;
		}
		//printfDx("%d,%d,%d,%d\n",lltime, y, mNotesType, mX);
	}

	if (mY > 0 && mStartTime > 0 && GetNowHiPerformanceCount() > mStartTime && lltime < -200000 && mShowFlag && !mStop && !mMiss) {
		mShowFlag = 0; 
		if (mNotesType != TYPE_HOLD_VOID) mMiss = 1;
		//printfDx("%d,%d,%d,%d\n", mTime, lltime, (int)GetNowHiPerformanceCount(), (int)mStartTime);
	}

	mSlideD+=5;
	mSlideD %= 100;
}

void Notes::draw() {
	switch (mNotesType)
	{
	case Notes::TYPE_CLICK:
		if(!mSide){
			DrawGraph(WINDOW_W / 4 - mSizeX / 2, mY - mSizeY / 2, mGHandleL, TRUE);
		} else {
			DrawGraph(WINDOW_W * 3 / 4 - mSizeX / 2, mY - mSizeY / 2, mGHandleR, TRUE);
		}
		break;
	case Notes::TYPE_HOLD_S:
		//終点ノートが不明の場合は画面上方から伸ばす
		//if (!mShowE) {
		//	if (mShowFlag) {
		//		if (!mSide) {
		//			DrawExtendGraph(WINDOW_W / 4 - mSizeX / 2, 0, WINDOW_W / 4 + mSizeX / 2, mY, mGHholdingL, TRUE);
		//		} else {
		//			DrawExtendGraph(WINDOW_W * 3 / 4 - mSizeX / 2, 0, WINDOW_W * 3 / 4 + mSizeX / 2, mY, mGHholdingR, TRUE);
		//		}
		//	} else {
		//		if (!mSide) {
		//			DrawExtendGraph(WINDOW_W / 4 - mSizeX / 2, 0, WINDOW_W / 4 + mSizeX / 2, CURSOR_Y , mGHholdingL, TRUE);
		//		}
		//		else {
		//			DrawExtendGraph(WINDOW_W * 3 / 4 - mSizeX / 2, 0, WINDOW_W * 3 / 4 + mSizeX / 2, CURSOR_Y, mGHholdingR, TRUE);
		//		}
		//	}
		//}

		if (!mSide) {
			DrawGraph(WINDOW_W / 4 - mSizeX / 2, mY - mSizeY / 2, mGHholdL, TRUE);
		}
		else {
			DrawGraph(WINDOW_W * 3 / 4 - mSizeX / 2, mY - mSizeY / 2, mGHholdR, TRUE);
		}
		break;
	case Notes::TYPE_HOLD_E:
		//始点ノートが消えている場合はカーソル位置まで伸ばす
		//if (!mHoldS) {
		//	if (mShowFlag) {
		//		if (!mSide) {
		//			DrawExtendGraph(WINDOW_W / 4 - mSizeX / 2, mY, WINDOW_W / 4 + mSizeX / 2, CURSOR_Y, mGHholdingL, TRUE);
		//		}
		//		else {
		//			DrawExtendGraph(WINDOW_W * 3 / 4 - mSizeX / 2, mY, WINDOW_W * 3 / 4 - mSizeX / 2, CURSOR_Y, mGHholdingR, TRUE);
		//		}
		//	}
		//}
		//else {
		//	if (mShowFlag) {
		//		if (!mSide) {
		//			DrawExtendGraph(WINDOW_W / 4 - mSizeX / 2, mY, WINDOW_W / 4 + mSizeX / 2, mHoldS->GetY(), mGHholdingL, TRUE);
		//		}
		//		else {
		//			DrawExtendGraph(WINDOW_W * 3 / 4 - mSizeX / 2, mY, WINDOW_W * 3 / 4 - mSizeX / 2, mHoldS->GetY(), mGHholdingR, TRUE);
		//		}
		//	}
		//}

		int gh;
		switch (mEndType) {
		case END_NOMAL:
			gh = !mSide ? mGHholdLE : mGHholdRE; 
			if (!mSide) {
				DrawGraph(WINDOW_W / 4 - mSizeX / 2, mY - mSizeY / 2, gh, TRUE);
			}
			else {
				DrawGraph(WINDOW_W * 3 / 4 - mSizeX / 2, mY - mSizeY / 2, gh, TRUE);
			}
			break;
		case END_SLIDE_L:
			gh = mGHSlideL[mSide]; 
			if (!mSide) {
				DrawGraph(WINDOW_W / 4 - mSizeX / 2, mY - mSizeY / 2, gh, TRUE);
				DrawGraph(WINDOW_W / 4 - mSizeX / 2 - mSlideD, mY - mSizeY / 2, mGHSlideD[0], TRUE);
			}
			else {
				DrawGraph(WINDOW_W * 3 / 4 - mSizeX / 2, mY - mSizeY / 2, gh, TRUE);
				DrawGraph(WINDOW_W * 3 / 4 - mSizeX / 2 - mSlideD, mY - mSizeY / 2, mGHSlideD[0], TRUE);
			}
			break;
		case END_SLIDE_R:
			gh = mGHSlideR[mSide]; 
			if (!mSide) {
				DrawGraph(WINDOW_W / 4 - mSizeX / 2, mY - mSizeY / 2, gh, TRUE);
				DrawGraph(WINDOW_W / 4 - mSizeX / 2 + mSlideD, mY - mSizeY / 2, mGHSlideD[1], TRUE);
			}
			else {
				DrawGraph(WINDOW_W * 3 / 4 - mSizeX / 2, mY - mSizeY / 2, gh, TRUE);
				DrawGraph(WINDOW_W * 3 / 4 + mSlideD, mY - mSizeY / 2, mGHSlideD[1], TRUE);
			}
			break;
		}

		break;
	case TYPE_HOLD_VOID:
		// 描画ブレンドモードをアルファブレンド（５０％）にする
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
		if (!mSide) {
			DrawGraph(WINDOW_W / 4 - mSizeX / 2, mY - mSizeY / 2, mGHholdL, TRUE);
		}
		else {
			DrawGraph(WINDOW_W * 3 / 4 - mSizeX / 2, mY - mSizeY / 2, mGHholdR, TRUE);
		}
		// 描画ブレンドモードをノーブレンドにする
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		
		break;
	case TYPE_SLIDE_L:
		if (!mSide) {
			DrawGraph(WINDOW_W / 4 - mSizeX / 2, mY - mSizeY / 2, mGHSlideL[0], TRUE);
			DrawGraph(WINDOW_W / 4 - mSizeX / 2 - mSlideD, mY - mSizeY / 2, mGHSlideD[0], TRUE);
		}
		else {
			DrawGraph(WINDOW_W * 3 / 4 - mSizeX / 2, mY - mSizeY / 2, mGHSlideL[1], TRUE);
			DrawGraph(WINDOW_W * 3 / 4 - mSizeX / 2 - mSlideD, mY - mSizeY / 2, mGHSlideD[0], TRUE);
		}
		break;
	case TYPE_SLIDE_R:
		if (!mSide) {
			DrawGraph(WINDOW_W / 4 - mSizeX / 2, mY - mSizeY / 2, mGHSlideR[0], TRUE);
			DrawGraph(WINDOW_W / 4 + mSlideD, mY - mSizeY / 2, mGHSlideD[1], TRUE);
		}
		else {
			DrawGraph(WINDOW_W * 3 / 4 - mSizeX / 2, mY - mSizeY / 2, mGHSlideR[1], TRUE);
			DrawGraph(WINDOW_W * 3 / 4 + mSlideD, mY - mSizeY / 2, mGHSlideD[1], TRUE);
		}
		break;
	case TYPE_TOUCH:
		if (!mSide) {
			DrawGraph(mX - mSizeX / 2, mY - mSizeY / 2, mGHtouch[0], TRUE);
		}
		else {
				DrawGraph(mX - mSizeX / 2, mY - mSizeY / 2, mGHtouch[1], TRUE);
		}
		break;
	default:
		break;
	}

}

int Notes::GetY() { return mY; }
int Notes::GetX() { return mX; }
int Notes::GetSize() { return mSizeX; }
int Notes::GetSide() { return mSide; }
int Notes::GetShowFlag() { return mShowFlag; }
int Notes::GetTime() { return mTime; }
int Notes::GetPair() { return mHoldPair; }
int Notes::GetStop() { return mStop; }

//void Notes::SetShowE(int i) { mShowE = i; }
//void Notes::SetHoldS(Notes *n) { mHoldS = n; }
void Notes::SetStop(int i) { mStop = i; }
int Notes::checkMiss() {
	int result = mMiss;
	mMiss = 0;
	return result;
}

void Notes::SetNotes(int side, NotesType type) {
	mY = -mSizeY; 
	if (type != TYPE_TOUCH) { mX = side ? CURSOR_X_RIGHT : CURSOR_X_LEFT; }
	mSide = side;
	mNotesType = type;
	mShowFlag = 1;
	mStop = 0;
	switch (type) {
	case TYPE_CLICK:
	case TYPE_HOLD_S:
	case TYPE_HOLD_E:
	case TYPE_HOLD_VOID:
	case TYPE_SLIDE_L:
	case TYPE_SLIDE_R: GetGraphSize(mGHandleL, &mSizeX, &mSizeY); break;
	case TYPE_TOUCH: GetGraphSize(mGHtouch[0], &mSizeX, &mSizeY); break;
	}
	
}

void Notes::SetNotesType(int side, NotesType type) {
	//mY = -mSizeY;
	mSide = side;
	mNotesType = type;
	//mShowFlag = 1;
	//mStop = 0;
}
void Notes::SetEndType(EndType type) {
	mEndType = type;
}

void Notes::SetX(int x) {
	mX = x;
}

void Notes::SetTime(long long t) {
	mTime = t;
}

void Notes::SetStartTime(long long t) {
	mStartTime = t;
}

void Notes::SetSpeed(int speed) {
	mSpeed = speed;
}

void Notes::SetPair(int id) {
	mHoldPair = id;
}

void Notes::DeleteNotes() {
	mShowFlag = 0;
	mSide = -1;
	mNotesType = TYPE_NONE;
}

Notes::NotesType Notes::GetNotesType() { return mNotesType; }
Notes::EndType Notes::GetEndType() { return mEndType; }