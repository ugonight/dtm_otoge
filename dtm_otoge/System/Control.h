#pragma once
#include "Cursor.h"
#include "Notes.h"
#include "../File.h"
#include "../define.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

class Control
{
private:
	Cursor *mCursor;
	Notes *mNotes[NOTES_NUM];

	int mNotesNum; //ノートの最大数

	vector<File::NOTE> mNotesInfo; //譜面情報

	void controlNotes();	//ノートの生産
	void judge();	//判定
	
	int canFall(int side);	//下にホールド終点があるか
	void holdRelate();	//ロングノートの始点と終点の依存関係
	void drawRoot();	//ノートを結ぶあれを描く

	Notes::NotesType intToType(int i);	//数字をノートタイプに変換

	void click();
	int clickTime[2];

	//int mHold[2];	//ホールドが表示されているか
	bool mHoldOn[2];	//ホールドが押されているか
	//Notes *mHoldS[2];	//ホールドの始点のノート
	//int mHoldE[2];	//現在押している始点ホールドに対する終点のノートの番号
	void SetHoldRelate(); //ロングノートの始点と終点のペアを記録

	int mSlideS[2];	//カーソルがノートに触れたときのドラッグ量
	bool mSlideOn[2]; //スライド中かどうか

	int mTouchNum[2];	//ノートの連鎖数
	bool mTouchOn[2];	//カーソルがノートに触れていたか

	int mGHholding[2];
	int mGHback, mGHsarea;

	enum Rank {
		RANK_NONE,
		RANK_MISS,
		RANK_BAD_L,
		RANK_BAD_F,
		RANK_NICE_L,
		RANK_NICE_F,
		RANK_GREAT_L,
		RANK_GREAT_F,
		RANK_PERFECT
	};

	Rank evaluate(int time);
	void record(Rank rank,int side, int x = -1);
	Rank mTouchRank[2];	//カーソルがノートに触れていた時の一番良いランク

	int mScore, mCombo, mMaxCombo;
	int mRankCount[5];

	Rank mSRank;
	int mGHRankImage[9];
	int mSRankColor;
	int mSRankTime;
	int mFontHandle;
	int mSscore, mdScore;
	int GHnum[11];
	int GHnum2[10];
	int mGHscore;
	int mGHcombo;

	int mGHFlash;
	int mFlash[10][3];	//ノートをとった時の演出 1:フラグ 2:x座標 3:時間
	void showFlash();

	int mSHandle[3][2];	//効果音の種類・左右
	int mSE[10];

	long long mStartTime;	//曲の開始した時間



	// ファイル出力ストリームの初期化
	std::ofstream ofs;
public:
	Control(vector<File::NOTE> Notes, int speed);
	~Control();

	void update();
	void draw();

	void SetStartTime(long long time);
	bool GetGameEnd();

	int GetScore();
	int GetCombo();
	int GetRankCount(int rank);
	int GetNotesNum();
};

