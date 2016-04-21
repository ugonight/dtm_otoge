#include "Control.h"
#include "../define.h"
#include "../File.h"
#include <math.h>
#include <vector>


//テスト用変数
//int underNotes[2];

Control::Control(vector<File::NOTE> notes, int speed)
{
	mCursor = new Cursor();
	mNotesInfo = notes;
	mNotesNum = mNotesInfo.size();

	mNotes[0]->SetSpeed(speed);
	mNotes[0]->LoadNoteGraph();
	for (int i = 0; i < mNotesNum; i++) {
		mNotes[i] = new Notes();
		mNotes[i]->SetNotes(mNotesInfo[i].mSide, intToType(mNotesInfo[i].mType));
		mNotes[i]->SetTime(mNotesInfo[i].mTime);
		mNotes[i]->SetX(mNotesInfo[i].mPosX);

		if (mNotesInfo[i].mType == 2) {
			mNotes[i]->SetEndType(Notes::END_NOMAL);
		}
		else if (mNotesInfo[i].mType == 6) {
			mNotes[i]->SetEndType(Notes::END_SLIDE_L);
		}
		else if (mNotesInfo[i].mType == 7) {
			mNotes[i]->SetEndType(Notes::END_SLIDE_R);
		}

	}
	SetHoldRelate();
	
	//mHold[0] = mHold[1] = 0;
	mHoldOn[0] = mHoldOn[1] = 0;
	mSlideOn[0] = mSlideOn[1] = 0;
	mTouchNum[0] = mTouchNum[1] = 0;
	mTouchOn[0] = mTouchOn[1] = 0;
	mTouchRank[0] = mTouchRank[1] = RANK_NONE;
	mScore = 1000000 % mNotesNum;
	mCombo = mMaxCombo = 0;
	mSRank = RANK_NONE;
	for (int i = 0; i < 5; i++) mRankCount[i] = 0;
	mStartTime = -1;

	mGHholding[LEFT] = LoadGraph("data/holdL_n.png");
	mGHholding[RIGHT] = LoadGraph("data/holdR_n.png");
	mGHback = LoadGraph("data/back.png");
	mGHsarea = LoadGraph("data/sarea.png");

	mFontHandle = CreateFontToHandle(NULL, 40, 3);

	mSHandle[0][0] = LoadSoundMem("data/seL.ogg");
	mSHandle[0][1] = LoadSoundMem("data/seR.ogg");
	mSHandle[1][0] = LoadSoundMem("data/se2L.ogg");
	mSHandle[1][1] = LoadSoundMem("data/se2R.ogg");
	mSHandle[2][0] = LoadSoundMem("data/se3L.ogg");
	mSHandle[2][1] = LoadSoundMem("data/se3R.ogg");

	mGHRankImage[RANK_MISS] = LoadGraph("data/RANK_MISS.png");
	mGHRankImage[RANK_BAD_L] = LoadGraph("data/RANK_BAD_L.png");
	mGHRankImage[RANK_BAD_F] = LoadGraph("data/RANK_BAD_F.png");
	mGHRankImage[RANK_NICE_L] = LoadGraph("data/RANK_NICE_L.png");
	mGHRankImage[RANK_NICE_F] = LoadGraph("data/RANK_NICE_F.png");
	mGHRankImage[RANK_GREAT_L] = LoadGraph("data/RANK_GREAT_L.png");
	mGHRankImage[RANK_GREAT_F] = LoadGraph("data/RANK_GREAT_F.png");
	mGHRankImage[RANK_PERFECT] = LoadGraph("data/RANK_PERFECT.png");
	mGHFlash = LoadGraph("data/flash.png");
	LoadDivGraph("data/num.png", 11, 11, 1, 50, 70, GHnum);
	LoadDivGraph("data/num2.png", 10, 10, 1, 27, 40, GHnum2);
	mGHscore = LoadGraph("data/score.png");
	mGHcombo = LoadGraph("data/combo.png");

	for (int i = 0; i > 10; i++) {
		mSE[i] = -1;	//停止中
		mFlash[i][0] = 0;
	}

	//ofs.open("testLog.txt");
}


Control::~Control()
{
	delete mCursor;
	for (int i = 0; i < mNotesNum; i++) delete mNotes[i];

	ofs.close();
}

void Control::SetHoldRelate() {
	for (int id = 0; id < mNotesNum; id++) {
		if (mNotes[id]->GetNotesType() == Notes::TYPE_HOLD_S) {
			int time = 0, idE = 0;
			for (int i = 0; i < mNotesNum; i++) {
				if (mNotes[i]->GetTime() > time) time = mNotes[i]->GetTime();	//時間の最大値を記録
			}
			//始点に対応する終点を検索
			for (int i = 0; i < mNotesNum; i++) {
				if (mNotes[i]->GetSide() == mNotes[id]->GetSide() && mNotes[i]->GetTime() > mNotes[id]->GetTime() && mNotes[i]->GetTime() <= time) {
					idE = i;
					time = mNotes[i]->GetTime();
				}
			}
			//ホールドのペアを記録
			mNotes[id]->SetPair(idE);
			mNotes[idE]->SetPair(id);
		}
	}
}

void Control::controlNotes() {
	int i = 0, side = GetRand(1);
	while (mNotes[i]->GetShowFlag() && i < mNotesNum - 1) i++;	//使われてるノートを飛ばす
	static int span = 10;
	static int count = 0; count++; count %= span;

	if (/*!GetRand(10) &&*/ !count) {
		if (!mTouchNum[side]) {
			switch (GetRand(Notes::NUM - 1))
			{
			case Notes::TYPE_CLICK:
				if (canFall(side) && !mTouchNum[side]) {
					mNotes[i]->SetNotes(side, Notes::TYPE_CLICK);
				}
				span = 20;
				//if (!mHold[side] && side) mNotes[i]->SetNotes(1, side, Notes::TYPE_CLICK);
				break;
			case Notes::TYPE_HOLD_S:
				if (canFall(side) && !mTouchNum[side]) {
					mNotes[i]->SetNotes(side, Notes::TYPE_HOLD_S);
					//mHold[side]++;
					//mNotes[i]->SetShowE(0);
					//mHoldS[side] = mNotes[i];
				}
				span = 30;
				/*if (!mHold[side] && side) { mNotes[i]->SetNotes(1, side, Notes::TYPE_HOLD_S); mHold[side]++; mNotes[i]->SetShowE(0); mHoldS[side] = mNotes[i]; }*/
				break;
			case Notes::TYPE_HOLD_E:
				if (!canFall(side) && !mTouchNum[side]) {
					mNotes[i]->SetNotes(side, Notes::TYPE_HOLD_E);
					//mHold[side] = 0;
					//mHoldS[side]->SetShowE(1);
					//mNotes[i]->SetHoldS(mHoldS[side]); 
					switch (GetRand(Notes::END_NUM)) {
					case Notes::END_NOMAL:	mNotes[i]->SetEndType(Notes::END_NOMAL); break;
					case Notes::END_SLIDE_L:mNotes[i]->SetEndType(Notes::END_SLIDE_L); break;
					case Notes::END_SLIDE_R:mNotes[i]->SetEndType(Notes::END_SLIDE_R); break;
					}
				}
				span = 30;
				break;
			case Notes::TYPE_SLIDE_L:
				if (canFall(side) /*&& canFall(!side)*/ && !mTouchNum[side]) {
					mNotes[i]->SetNotes(side, Notes::TYPE_SLIDE_L);
				}
				span = 20;
				break;
			case Notes::TYPE_SLIDE_R:
				if (canFall(side)/* && canFall(!side)*/ && !mTouchNum[side]) {
					mNotes[i]->SetNotes(side, Notes::TYPE_SLIDE_R);
				}
				span = 20;
				break;
			case Notes::TYPE_TOUCH:
				if (canFall(side) && !mTouchNum[!side]) {
					if (!mTouchNum[side]) mTouchNum[side] = GetRand(9) + 1;
					mNotes[i]->SetNotes(side, Notes::TYPE_TOUCH);
					if (side) {
						mNotes[i]->SetX(GetRand(WINDOW_W/2)+WINDOW_W/2);
					}
					else {
						mNotes[i]->SetX(GetRand(WINDOW_W / 2));
					}
					mTouchNum[side]--;
				}
				span = 40;
				break;
			default:
				break;
			}
		}
		else {
			if (!mTouchNum[side]) mTouchNum[side] = GetRand(9) + 1;
			mNotes[i]->SetNotes(side, Notes::TYPE_TOUCH);
			if (side) {
				mNotes[i]->SetX(GetRand(WINDOW_W / 2) + WINDOW_W / 2);
			}
			else {
				mNotes[i]->SetX(GetRand(WINDOW_W / 2));
			}
			mTouchNum[side]--;
		}
	}
}

int Control::canFall(int side) {
	int yE,yS,y;

	//一番上にある始点を検索
	y = WINDOW_H;
	for (int i = 0; i < mNotesNum; i++) {
		if (mNotes[i]->GetShowFlag() && mNotes[i]->GetY() < y && mNotes[i]->GetSide() == side&& mNotes[i]->GetNotesType() == Notes::TYPE_HOLD_S) {
			y = mNotes[i]->GetY();
		}
	}
	yS = y;
	//一番上にある終点を検索
	y = WINDOW_H;
	for (int i = 0; i < mNotesNum; i++) {
		if (mNotes[i]->GetShowFlag() && mNotes[i]->GetY() < y && mNotes[i]->GetSide() == side&& mNotes[i]->GetNotesType() == Notes::TYPE_HOLD_E) {
			y = mNotes[i]->GetY();
		}
	}
	yE = y;
	return yS >= yE;
}

void Control::holdRelate() {
	for (int side = LEFT; side < 2;side++) {
		int yE, yS, y, idS, idE;
		idS = idE = -1;
		//一番下にある始点を検索
		y = 0;
		for (int i = 0; i < mNotesNum; i++) {
			if (mNotes[i]->GetShowFlag() && mNotes[i]->GetY() > y && mNotes[i]->GetSide() == side && mNotes[i]->GetNotesType() == Notes::TYPE_HOLD_S) {
				y = mNotes[i]->GetY(); idS = i;
			}
		}
		yS = y;
		if (idS >= 0) {
			//一番下にある終点を検索
			y = 0;
			//for (int i = 0; i < mNotesNum; i++) {
			//	if (mNotes[i]->GetShowFlag() && mNotes[i]->GetY() > y && mNotes[i]->GetSide() == side && mNotes[i]->GetNotesType() == Notes::TYPE_HOLD_E) {
			//		y = mNotes[i]->GetY(); idE = i;
			//	}
			//}
			//yE = y;
			idE = mNotes[idS]->GetPair();
			yE = mNotes[idE]->GetY();

			//mHoldE[side] = -1;
			mCursor->SetStop(0, side);
			if (mHoldOn[side]) {
				//mHoldE[side] = idE; //ホールド終点のノートを記録しておく

				if (mNotes[idE]->GetEndType() == Notes::END_SLIDE_L || mNotes[idE]->GetEndType() == Notes::END_SLIDE_R) {
					//if (CURSOR_Y - yE < mNotes[idE]->GetSize()) {
					if (evaluate(mNotes[idE]->GetTime()) >= RANK_NICE_L) {
						mCursor->SetStop(0, side);	//終点がスライドの時は終点付近でカーソル固定を解除する
						//終点スライドの判定
						int dx, dy, size, time;
						size = mNotes[idE]->GetSize();
						time = mNotes[idE]->GetTime();
						switch (mNotes[idE]->GetEndType())
						{
						case Notes::END_SLIDE_L:
							dx = mCursor->GetDragX(side);
							dy = CURSOR_Y - mNotes[idE]->GetY();
							//カーソルとノートが重なったとき
							//if (sqrt(pow(dx, 2) + pow(dy, 2)) <= size && mCursor->GetClick(side) && !mSlideOn[side]) {
							if (evaluate(time) > RANK_NONE && mCursor->GetClick(side) && !mSlideOn[side]) {
								mSlideS[side] = dx;	//スライド開始時のスライド量を記録
								mSlideOn[side] = 1;
							}
							if (dx - mSlideS[side] < -size / 2 && evaluate(time) > RANK_NONE && mCursor->GetClick(side) && mSlideOn[side]) {
								record(evaluate(time), side);
								mNotes[idE]->DeleteNotes();
								mNotes[idS]->DeleteNotes(); //始点を消す
								mSlideS[side] = 0;
								mSlideOn[side] = 0;
								mHoldOn[side] = 0;
								mCursor->SetStop(0, side);
								clickTime[side] = -1;
							}
							break;
						case Notes::END_SLIDE_R:
							dx = mCursor->GetDragX(side);
							dy = CURSOR_Y - mNotes[idE]->GetY();
							//カーソルとノートが重なったとき
							//if (sqrt(pow(dx, 2) + pow(dy, 2)) <= size && mCursor->GetClick(side) && !mSlideOn[side])
							if (evaluate(time) > RANK_NONE && mCursor->GetClick(side) && !mSlideOn[side]) {
								mSlideS[side] = dx;	//スライド開始時のスライド量を記録
								mSlideOn[side] = 1;
							}
							if (dx - mSlideS[side] > size / 2 && evaluate(time) > RANK_NONE && mCursor->GetClick(side) && mSlideOn[side]) {
								record(evaluate(time), side);
								mNotes[idE]->DeleteNotes();
								mNotes[idS]->DeleteNotes(); //始点を消す
								mSlideS[side] = 0;
								mSlideOn[side] = 0;
								mHoldOn[side] = 0;
								mCursor->SetStop(0, side);
								clickTime[side] = -1;
							}
							break;
						}
					}
					else if (CURSOR_Y - yE > mNotes[idE]->GetSize()) {
						mCursor->SetStop(1, side);
					}
				}
				else if (CURSOR_Y - yE > mNotes[idE]->GetSize()) {
					mCursor->SetStop(1, side);	//カーソルのスライドを止める
				}
			}


			if (yE >= yS && yE > 0) {
				//mNotes[idE]->DeleteNotes();	//対応する始点が消えてる終点を消す		
				if (mHoldOn[side]) {
					//mHoldOn[side] = 0;
					//mNotes[idS]->DeleteNotes();	//押しすぎたホールドの始点を消す
					if (/*mNotes[idE]->GetShowFlag() &&*/ (mNotes[idE]->GetEndType() == Notes::END_SLIDE_L || mNotes[idE]->GetEndType() == Notes::END_SLIDE_R)) {
						//if (yE - CURSOR_Y > mNotes[idE]->GetSize() / 2) {	//この時に始点から上に道が伸びてしまうのが直せない…
						if (evaluate(mNotes[idE]->GetTime()) < RANK_NICE_F) {
							record(RANK_MISS, side);
							mNotes[idS]->DeleteNotes();	//押しすぎたホールドの始点を消す
							mNotes[idE]->DeleteNotes();
							mHoldOn[side] = 0;
						}
					}
					else {
						record(RANK_PERFECT, side);	//離し忘れはパーフェクトにしてあげる
						mNotes[idS]->DeleteNotes();	//押しすぎたホールドの始点を消す
						mNotes[idE]->DeleteNotes();
						mHoldOn[side] = 0;
					}

				}
				else {
					if (mNotes[idE]->GetY() > 0) {
						mNotes[idE]->DeleteNotes();	//対応する始点が消えてる終点を消す	
						mHoldOn[side] = 0;
					}
				}
			}

		}


		if (!mHoldOn[side]) {
			mCursor->SetStop(0, side);
			StopSoundMem(mSHandle[2][side]);
		}
		if (!mNotes[idE]->GetShowFlag()) mNotes[idS]->DeleteNotes();
	}
}

void Control::judge() {
	int id, y, size, time;
	int dx, dy;
	Rank rank;
	
	//左右ノートそれぞれについて順に判定
	for (int side = 0; side < 2; side++) {
		//一番下にあるノートについて判定
		y = 0; id = -1;
		for (int j = 0; j < mNotesNum; j++) if (mNotes[j]->GetY() <= y) y = mNotes[j]->GetY();	//一番上にあるノートの位置を取得
		for (int j = 0; j < mNotesNum; j++) {
			//if (mNotes[j]->GetShowFlag() && mNotes[j]->GetY() > y && mNotes[j]->GetSide() == side) {
			if (mNotes[j]->GetShowFlag() && mNotes[j]->GetY() > y && mNotes[j]->GetSide() == side) {
				y = mNotes[j]->GetY(); id = j;
			}
		}

		//underNotes[side] = id;
	
		//画面に何もないときは処理しない
		if (id >= 0 && (evaluate(mNotes[id]->GetTime()) > RANK_MISS || mNotes[id]->GetStop())) {
			size = mNotes[id]->GetSize();
			time = mNotes[id]->GetTime();
			//mNotes[id]->SetStop(0);

			switch (mNotes[id]->GetNotesType()) {
			case Notes::TYPE_CLICK:
				//ワンクリックされていたら
				if (clickTime[side] == 1) {
					//printfDx("%d,%d\n",i,y);
					rank = evaluate(time);
					record(rank,side);
					if (rank != RANK_NONE) {
						//printfDx("%d,%d,%d\n", id, y, abs(y - CURSOR_Y));
						clickTime[side] = -1;
						mNotes[id]->DeleteNotes();
					}
				}
				break;
			case Notes::TYPE_HOLD_S:
				//マウスが押しっぱなしにされてるとき
				if (mCursor->GetClick(side) /*&& mHoldOn[side] >= 0*/) {
					//最初に押されたとき
					if (clickTime[side] == 1 && !mHoldOn[side]) {
						rank = evaluate(time);
						record(rank,side);
						
						if (rank != RANK_NONE) {
							//printfDx("%d,%d\n", id, y);
							
							if (rank == RANK_NICE_F || rank == RANK_BAD_F || rank == RANK_NICE_L || rank == RANK_BAD_L) {
								//mHold[side] = 0;
								//mNotes[id]->DeleteNotes(); //NICE判定以下ならノートを切る
								mNotes[id]->SetNotesType(side, Notes::TYPE_HOLD_VOID);
								mNotes[mNotes[id]->GetPair()]->DeleteNotes();
							}
							else {
								mHoldOn[side] = 1;
								ChangeVolumeSoundMem(VOL_SE, mSHandle[2][side]);
								PlaySoundMem(mSHandle[2][side], DX_PLAYTYPE_BACK);
								//mNotes[id]->SetStop(1);
							}

							//mNotes[i]->SetNotes(0);
							clickTime[side] = -1;
						}
					}

					//押され続けている間はノートを止めておく
					if (mHoldOn[side]) { 
						mNotes[id]->SetStop(1);
						//printfDx("%d\n", y);
					}
				}
				else {
					//長押しを切ったとき
					if (mHoldOn[side]) {
						//if (mHoldE[side] >= 0) { 
						if (mNotes[mNotes[id]->GetPair()]->GetY() >= 0) {
							record(evaluate(mNotes[mNotes[id]->GetPair()]->GetTime()),side);
						} else {
							record(RANK_MISS,side);
						}
						//mHold[side] = 0;
						mNotes[id]->DeleteNotes();
						mNotes[mNotes[id]->GetPair()]->DeleteNotes();
						//mNotes[id]->SetStop(0);
						mHoldOn[side] = 0;
						StopSoundMem(mSHandle[2][side]);
					}
				}

				break;
			case Notes::TYPE_HOLD_E:
				//終点が一番下に来ることはないのでholdRelate()に記述
				if (mNotes[id]->GetPair() >= 0) {
					if (!mNotes[mNotes[id]->GetPair()]->GetShowFlag()) {
						mNotes[id]->DeleteNotes();
						record(RANK_MISS, side);
					}
				}
				break;
			case Notes::TYPE_SLIDE_L:
				dx = mCursor->GetDragX(side);
				dy = CURSOR_Y - mNotes[id]->GetY();
				//カーソルとノートが重なったとき
				//if (sqrt(pow(dx, 2) + pow(dy, 2)) <= size && mCursor->GetClick(side) && !mSlideOn[side]) {
				if (abs(dx) < size * 2 && evaluate(time) > RANK_NONE && mCursor->GetClick(side) && !mSlideOn[side]) {
				//if (mSlideS[side] < 0) {
					mSlideS[side] = dx;	//スライド開始時のスライド量を記録
					mSlideOn[side] = 1;
				//} else/* if (sqrt(pow(dx, 2) + pow(dy, 2)) < size)*/ {
				}
				if (dx - mSlideS[side] < -size / 2 && evaluate(time) > RANK_NONE && mCursor->GetClick(side) && mSlideOn[side]) {
					//evaluate(y, size);
					record(evaluate(time),side);
					mNotes[id]->DeleteNotes();
					mSlideS[side] = 0;
					mSlideOn[side] = 0;
				}
					
				break;
			case Notes::TYPE_SLIDE_R:
				//int dx, dy;
				dx = mCursor->GetDragX(side);
				dy = CURSOR_Y - mNotes[id]->GetY();
				//カーソルとノートが重なったとき
				//if (sqrt(pow(dx, 2) + pow(dy, 2)) <= size && mCursor->GetClick(side) && !mSlideOn[side]) {
				if (abs(dx) < size * 2 && evaluate(time) > RANK_NONE && mCursor->GetClick(side) && !mSlideOn[side]) {
					mSlideS[side] = dx;	//スライド開始時のスライド量を記録
					mSlideOn[side] = 1;
				} 
				if (dx - mSlideS[side] > size / 2 && evaluate(time) > RANK_NONE && mCursor->GetClick(side) && mSlideOn[side]) {
					record(evaluate(time),side);
					mNotes[id]->DeleteNotes();
					mSlideS[side] = 0;
					mSlideOn[side] = 0;
				}

				break;
			case Notes::TYPE_TOUCH:
				int cursorX = side ? CURSOR_X_RIGHT : CURSOR_X_LEFT;
				dx = cursorX + mCursor->GetDragX(side) - mNotes[id]->GetX();
				dy = CURSOR_Y - y;
				//カーソルとノートが触れたとき
				//if (sqrt(pow(dx, 2) + pow(dy, 2)) < 48.0) {
				if (abs(dx) < 48 && evaluate(time) > RANK_NICE_F && mCursor->GetClick(side)) {
					if (!mTouchOn[side]) mTouchOn[side] = 1;
					mTouchRank[side] = mTouchRank[side] < evaluate(time) ? evaluate(time) : mTouchRank[side];	//最良のランクを記録
					if (mTouchRank[side] == RANK_PERFECT) {	
						record(RANK_PERFECT,side, mNotes[id]->GetX());
						mNotes[id]->DeleteNotes();
						mTouchOn[side] = 0;
						mTouchRank[side] = RANK_NONE;
					}
					//printfDx("%d\n", mTouchRank[side]);
				} else {
					if (mTouchOn[side]) {
						record(mTouchRank[side], side, mNotes[id]->GetX());
						mNotes[id]->DeleteNotes();
						mTouchOn[side] = 0;
						mTouchRank[side] = RANK_NONE;
					}
				}
				break;
			}
		}
	}
}

Control::Rank Control::evaluate(int time) {
	LONGLONG lltime = time - (GetNowHiPerformanceCount() - mStartTime);
	Rank result = RANK_NONE;

	if (lltime < 50000 && lltime > -50000) {
		result = RANK_PERFECT;
	}
	else if (lltime < 100000 && lltime > -100000) {
		result = lltime < 0 ? RANK_GREAT_L : RANK_GREAT_F;
	}
	else if (lltime < 150000 && lltime > -150000) {
		result = lltime < 0 ? RANK_NICE_L : RANK_NICE_F;
	}
	else if (lltime < 200000  &&  lltime > -200000) {
		result = lltime < 0 ? RANK_BAD_L : RANK_BAD_F;
	}


	return result;
}

void Control::record(Rank rank,int side, int x) {
	mSRank = rank;
	int dScore=0;
	switch (rank) {
	case RANK_NONE: break;
	case RANK_MISS:
		//mSRank="MISS";
		mSRankColor = GetColor(50, 50, 50);
		mSRankTime = 100;
		dScore = (1000000 / mNotesNum) * 0.1;
		mCombo = 0;
		mRankCount[4]++;
		break;
	case RANK_BAD_L:
		//mSRank = "BAD";
		mSRankColor = GetColor(0, 0, 200);
		mSRankTime = 100;
		dScore =  (1000000 / mNotesNum) * 0.2;
		mCombo = 0;
		mRankCount[3]++;
		break;
	case RANK_BAD_F:
		//mSRank = "BAD";
		mSRankColor = GetColor(0, 0, 255);
		mSRankTime = 100;
		dScore =  (1000000 / mNotesNum) * 0.2;
		mCombo = 0;
		mRankCount[3]++;
		break;
	case RANK_NICE_L:
		//mSRank = "NICE";
		mSRankColor = GetColor(0, 200, 0);
		mSRankTime = 100;
		dScore =  (1000000 / mNotesNum) * 0.4;
		mCombo = 0;
		mRankCount[2]++;
		break;
	case RANK_NICE_F:
		//mSRank = "NICE";
		mSRankColor = GetColor(0, 255, 0);
		mSRankTime = 100;
		dScore =  (1000000 / mNotesNum) * 0.4;
		mCombo = 0;
		mRankCount[2]++;
		break;
	case RANK_GREAT_L:
		//mSRank = "GREAT";
		mSRankColor = GetColor(200, 133, 0);
		mSRankTime = 100;
		dScore =  (1000000 / mNotesNum) * 0.8;
		mCombo++;
		mRankCount[1]++;
		break;
	case RANK_GREAT_F:
		//mSRank = "GREAT";
		mSRankColor = GetColor(255, 153, 0);
		mSRankTime = 100;
		dScore =  (1000000 / mNotesNum) * 0.8;
		mCombo++;
		mRankCount[1]++;
		break;
	case RANK_PERFECT:
		//mSRank = "PERFECT";
		mSRankColor = GetColor(255, 0, 0);
		mSRankTime = 100;
		dScore =  1000000 / mNotesNum;
		mCombo++;
		mRankCount[0]++;
		break;
	}

	mScore += dScore;
	mdScore = dScore / 8;

	mMaxCombo = mMaxCombo < mCombo ? mCombo : mMaxCombo;

	//効果音の再生
	if (rank == RANK_NICE_F || rank == RANK_NICE_L) {
		ChangeVolumeSoundMem(VOL_SE, mSHandle[1][side]);
		PlaySoundMem(mSHandle[1][side], DX_PLAYTYPE_BACK); 
	}
	if (rank >= RANK_GREAT_L) {
		ChangeVolumeSoundMem(VOL_SE, mSHandle[0][side]); 
		PlaySoundMem(mSHandle[0][side], DX_PLAYTYPE_BACK); 
	}

	int posX;
	if (rank >= RANK_GREAT_L) {
		for (int i = 0; i < 10; i++) {
			if (!mFlash[i][0]) { 
				mFlash[i][0] = 1; 
				if (x < 0) {
					posX = side ? CURSOR_X_RIGHT : CURSOR_X_LEFT;
				}
				else {
					posX = x;
				}
				mFlash[i][1] = posX;
				mFlash[i][2] = 99;
				break;
			}
		}
	}
}

void Control::drawRoot() {
	int id, yS,yE,y/*, side=0*/, maxY, posX;
	Notes *HoldS/*, *HoldE*/;	//始点ノート
	//左右ノートそれぞれについて順に判定
	for (int side = 0; side < 2; side++) {
		//ホールド
		//一番下にあるノートから見ていく
		id = -1; /*HoldS = HoldE = 0;*/ maxY = WINDOW_H;
		while (1) {

			//始点を検索
			yS = -1;
			for (int j = 0; j < mNotesNum; j++) {
				if (mNotes[j]->GetShowFlag() && mNotes[j]->GetY() > yS  && mNotes[j]->GetSide() == side && mNotes[j]->GetNotesType() == Notes::TYPE_HOLD_S) {
					if (mNotes[j]->GetY() >= 0 && mNotes[j]->GetY() < maxY) {
						yS = mNotes[j]->GetY(); id = j;
					}
				}
			}
			if (yS < 0) break;	//始点が見つからなかったらループを抜ける
			//printfDx("y:%d\n",y);
			//HoldS = mNotes[id];

			//終点を検索
			yE = 0;
			//for (int j = 0; j < mNotesNum; j++) {
			//	if (mNotes[j]->GetShowFlag() && mNotes[j]->GetY() > yE && mNotes[j]->GetSide() == side && mNotes[j]->GetNotesType() == Notes::TYPE_HOLD_E) {
			//		if (mNotes[j]->GetY() >= 0 && mNotes[j]->GetY() < maxY) {
			//			yE = mNotes[j]->GetY(); id = j;
			yE = mNotes[mNotes[id]->GetPair()]->GetY();
			//		}
			//	}
			//}
			//HoldE = mNotes[id];

			//描画
			posX = side ? CURSOR_X_RIGHT : CURSOR_X_LEFT;
			if (yS > yE) { 
				DrawExtendGraph(posX - 32, yE, posX + 32, yS, mGHholding[side], TRUE); 			
			}
			//DrawFormatString(posX - 32, (yE + yS) / 2, GetColor(200, 200, 200), "S:%d,E:%d", yS, yE);
			//始点を探す範囲を縮める。この時終点がなかった場合はループを抜ける
			//maxY = yE;
			maxY = yS;
			if (!yE) break;
		}

		//スライド
		//一番下にあるノートから見ていく
		id = -1; HoldS = 0; maxY = WINDOW_H;
		while (1) {
			//始点を検索
			y = 0;
			for (int j = 0; j < mNotesNum; j++) {
				if (mNotes[j]->GetShowFlag() && mNotes[j]->GetY() > y && mNotes[j]->GetSide() == side && mNotes[j]->GetNotesType() == Notes::TYPE_TOUCH) {
					if (mNotes[j]->GetY() >= 0 && mNotes[j]->GetY() < maxY) {
						y = mNotes[j]->GetY(); id = j;
					}
				}
			}
			if (!y) break;	//始点が見つからなかったらループを抜ける
							//printfDx("y:%d\n",y);
			HoldS = mNotes[id];
			//終点を検索
			maxY = y;
			y = 0;
			for (int j = 0; j < mNotesNum; j++) {
				if (mNotes[j]->GetShowFlag() && mNotes[j]->GetY() > y && mNotes[j]->GetSide() == side /*&& mNotes[j]->GetNotesType() == Notes::TYPE_TOUCH*/) {
					if (mNotes[j]->GetY() >= 0 && mNotes[j]->GetY() < maxY) {
						y = mNotes[j]->GetY(); id = j;
					}
				}
			}
			//HoldE = mNotes[id];
			if (mNotes[id]->GetNotesType() == Notes::TYPE_TOUCH) {
				//描画
				posX = side ? CURSOR_X_RIGHT : CURSOR_X_LEFT;
				int Cr = side ? GetColor(255, 0, 0) : GetColor(0, 0, 255);
				if (HoldS->GetY() > y) {
					//DrawExtendGraph(posX - 32, y, posX + 32, HoldS->GetY(), mGHholding[side], TRUE);
					DrawLine(HoldS->GetX(), HoldS->GetY(), mNotes[id]->GetX(), mNotes[id]->GetY(), Cr, 3);
				}
				//DrawFormatString(posX - 32, (HoldS->GetY() + y) / 2, GetColor(0, 0, 0), "S:%d,E:%d", HoldS->GetY(), y);
				//始点を探す範囲を縮める。この時終点がなかった場合はループを抜ける
				ofs << "side=" << side << ",sY=" << HoldS->GetY() << ",eY=" << y << ",maxY=" << maxY << std::endl;
				//maxY = y;
			}
			if (!y) break;
		}

		//同時押し
		for (int j = 0; j < mNotesNum; j++) {
			if (mNotes[j]->GetShowFlag() && mNotes[j]->GetY() > 0 && mNotes[j]->GetSide() == 0) {
				for (int k = 0; k < mNotesNum; k++) {
					if (mNotes[k]->GetShowFlag() && mNotes[k]->GetY() > 0 && mNotes[k]->GetSide() == 1) {
						if (abs(mNotes[j]->GetTime() - mNotes[k]->GetTime()) < 10000) {
							DrawLine(CURSOR_X_LEFT + 32, mNotes[j]->GetY(), CURSOR_X_RIGHT - 32, mNotes[k]->GetY(), GetColor(100, 100, 50), 3);
							break;
						}
					}
				}
			}
		}
	}
}


void Control::click() {
	for (int side = 0; side < 2; side++) {
		if (mCursor->GetClick(side) && clickTime[side] >= 0) {
			clickTime[side]++;
		}
		if (!mCursor->GetClick(side)) {
			clickTime[side] = 0;
			mSlideOn[side] = 0;	//ついでにスライドのフラグも切る
			//mHoldOn[side] = 0;	//ホールドもついでに
		}
	}

}

void Control::update() {
	//controlNotes();

	mCursor->update();

	for (int i = 0; i < mNotesNum; i++) {
		if (mNotes[i]->GetShowFlag()) mNotes[i]->update();
		if (mNotes[i]->checkMiss() && GetNowHiPerformanceCount() > mStartTime)
			record(RANK_MISS, mNotes[i]->GetSide());
	}
	mSRankTime--;
	if (mScore > mSscore) {
		mSscore += mdScore;
	}
	else if (mScore < mSscore) {
		mSscore = mScore;
	}
	


	click();
	judge();
	holdRelate();
}


void Control::draw() {
	SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
	DrawGraph(0,0,mGHback,TRUE);
	DrawGraph(0, CURSOR_Y - 32, mGHsarea, TRUE);
	//SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	mCursor->draw();
	if (mStartTime > 0) {
		drawRoot();
		for (int i = 0; i < mNotesNum; i++) {
			if (mNotes[i]->GetShowFlag() && mNotes[i]->GetY() > -64) {
				mNotes[i]->draw();

				//DrawFormatString(mNotes[i]->GetX() - 32, mNotes[i]->GetY() - 32, GetColor(0, 0, 0), "%d,%d,%d", i, mNotes[i]->GetY(), mNotes[i]->GetNotesType());

			}
		}
	}


	int Cr = GetColor(122,122,122);
	//DrawFormatString(0, WINDOW_H - 15, Cr, "HoldOn:%d uNote:%d SlideS:%d DragX:%d SlidOn:%d", mHoldOn[0],underNotes[0], mSlideS[0], mCursor->GetDragX(0),mSlideOn[0]);
	//DrawFormatString(WINDOW_W / 2, WINDOW_H - 15, Cr, "HoldOn:%d uNote:%d SlideS:%d DragX:%d SlideOn:%d", mHoldOn[1],underNotes[1], mSlideS[1], mCursor->GetDragX(1),mSlideOn[1]);
	//for (int i = 0; i < 31; i++) {
	//	DrawFormatString(WINDOW_W / 3, i * 15, Cr, "[%d]show:%d,side:%d,y:%d,type:%d.miss:%d", i, mNotes[i]->GetShowFlag(), mNotes[i]->GetSide(), mNotes[i]->GetY(), mNotes[i]->GetNotesType(), mNotes[i]->checkMiss());
	//}

	//DrawFormatString(0, 0, GetColor(255, 255, 255), "Score:%d",mSscore);
	DrawGraph(0, -3, mGHscore, TRUE);
	int score = mSscore;
	for (int i = 6; i >= 0; i--) {
		int num = score / (int)pow(10, i);
		DrawGraph((6 - i) * 30 + 170, 0, GHnum[num], TRUE);
		score -= (int)pow(10, i) * num;
	}

	//DrawFormatString(0, 15, GetColor(255, 255, 255), "Combo:%d", mCombo);
	SetDrawBlendMode(DX_BLENDMODE_ADD, 255 *  (100 - mSRankTime) / 10);
	if (mCombo > 5) {
		DrawGraph(WINDOW_W - 160, 0, mGHcombo, TRUE);
		int combo = mCombo;
		int digit = 0;
		for (int i = 6; i >= 0; i--) {
			if (combo / (int)pow(10, i) != 0) {
				digit = i + 1;	//桁数を取得
				break;
			}
		}
		for (int i = 0; i < digit; i++) {
			int num = (combo % (int)pow(10, i + 1)) / (int)pow(10, i);
			int x = WINDOW_W - 80 - ((27 * digit) / 2) + (digit - i - 1) * 27;
			DrawGraph(x, 60, GHnum2[num], TRUE);
		}
	}
	SetDrawBlendMode(DX_BLENDMODE_ADD, 255);

	//DrawFormatString(0, 30, GetColor(255, 255, 255), "MaxCombo:%d", mMaxCombo);
	//DrawFormatString(0, 45, GetColor(255, 255, 255), "HoldOn:%d,%d", mHoldOn[0],mHoldOn[1]);
	//int j=0;
	//for (int i = 0; i < mNotesNum; i++) {
	//	if (mNotes[i]->GetShowFlag()) {
	//		DrawFormatString(WINDOW_W / 3, j * 15, GetColor(100, 100, 100), "%d,y:%d,time:%d,type:%d", i, mNotes[i]->GetY(),mNotes[i]->GetTime(),mNotes[i]->GetNotesType());
	//		j++;
	//	}
	//}
	char s[10];
	
	if (mSRankTime > 0) {
		/*int x = WINDOW_W / 2 - GetDrawStringWidthToHandle(mSRank, strlen(mSRank), mFontHandle) / 2;
		strcpy_s(s,10,mSRank);
		DrawStringToHandle(x, 180, mSRank, mSRankColor, mFontHandle); */

		int gx, gy, x;
		GetGraphSize(mGHRankImage[mSRank], &gx, &gy);
		if (mSRankTime > 90) {
			//gx /= mSRankTime - 90;
			//gy /= mSRankTime - 90;
			gx -= (mSRankTime - 90) * 20 * gx / (gx + gy);
			gy -= (mSRankTime - 90) * 20 * gy / (gx + gy);
			DrawExtendGraph(WINDOW_W / 2 - gx / 2, 150 - gy / 2, WINDOW_W / 2 + gx / 2, 150 + gy / 2, mGHRankImage[mSRank], TRUE);
		}
		else {
			x = WINDOW_W / 2 - gx / 2;
			DrawGraph(x, 150 - gy / 2, mGHRankImage[mSRank], TRUE);
		}
	}

	showFlash();
}

Notes::NotesType Control::intToType(int i) {
	switch (i) {
	case 0: return Notes::TYPE_CLICK; break;
	case 1: return Notes::TYPE_HOLD_S; break;
	case 2: case 6: case 7: return Notes::TYPE_HOLD_E; break;
	//case 3: return Notes::TYPE_HOLD_VOID; break;
	case 3: return Notes::TYPE_SLIDE_L; break;
	case 4: return Notes::TYPE_SLIDE_R; break;
	case 5: return Notes::TYPE_TOUCH; break;
	default: return Notes::TYPE_NONE; break;
	}
}

void Control::SetStartTime(long long time) {
	mStartTime = time; 
	//mNotes[0]->SetStartTime(mStartTime);
	for (int i = 0; i < mNotesNum; i++) mNotes[i]->SetStartTime(mStartTime);
}

void Control::showFlash() {
	int gx, gy, ex;
	GetGraphSize(mGHFlash, &gx, &gy);
	for (int i = 0; i < 10; i++) {
		if (mFlash[i][0]) {
			if (mFlash[i][2] < 0) { 
				mFlash[i][0] = 0; 
				break;
			}
			mFlash[i][2]-=5;

			ex = gx * (100 - mFlash[i][2]) / 20;
			SetDrawBlendMode(DX_BLENDMODE_ADD, 255 * mFlash[i][2] / 100);
			DrawExtendGraph(mFlash[i][1] - ex / 2, CURSOR_Y - ex / 2, mFlash[i][1] + ex / 2, CURSOR_Y + ex / 2, mGHFlash, TRUE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
	}
}

bool Control::GetGameEnd() {
	bool result = true;
	for (int i = 0; i < mNotesNum; i++) {
		if (mNotes[i]->GetShowFlag())
		{
			result = false;
			break;
		}
	}

	return result;
}

int Control::GetCombo() { return mMaxCombo; }
int Control::GetRankCount(int rank) { return mRankCount[rank]; }
int Control::GetScore() { return mScore; }
int Control::GetNotesNum() { return mNotesNum; }