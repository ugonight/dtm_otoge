#include "Play.h"
#include "Select.h"

#include "../System/Control.h"
#include "../File.h"

#include <fstream>
using namespace std;

Play::Play(string DataFile,int level,int speed)
{
	InitGraph();
	InitSoftImage();
	InitSoundMem();
	InitFontToHandle();

	
	mFile = new File();

	mFile->GetData(DataFile.c_str(), &mData);
	mFile->GetNotes(mData.mScore[level], &mNotes);
	mLevel = level;

	mControl = new Control(mNotes, speed);
	
	//vector<File::NOTE>::iterator it = mNotes.begin();
	//while (it != mNotes.end()) {
	//	printfDx("%d,%d,%d,%d\n", it->mTime, it->mSide, it->mType, it->mPosX);
	//	++it;
	//}

	
	mGHready = LoadGraph("data/detail.png");
	mGHResultBack[0] = LoadGraph("data/result_back.png");
	mGHResultBack[1] = LoadGraph("data/result_back2.png");
	mGHResult = LoadGraph("data/result.png");
	mGHJaket = LoadGraph(mData.mJaket);
	mSHBGM = LoadSoundMem(mData.mMusic);
	mFHtitle = CreateFontToHandle("ＭＳ ゴシック", 25, 3, DX_FONTTYPE_ANTIALIASING_EDGE_8X8, -1, 2);
	mFHdetail = CreateFontToHandle("ＭＳ ゴシック", 20, 3, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	LoadDivGraph("data/num.png", 11, 11, 1, 50, 70, GHnum);
	LoadDivGraph("data/num2.png", 10, 10, 1, 27, 40, GHnum2);
	LoadDivGraph("data/rank.png", 4, 4, 1, 70, 79, GHRank);
	mGHnewrecord = LoadGraph("data/newrecord.png");

	mBright = 0;
	mReadyCount = 0;
	mCount = 0;
	mResultCount = 0;
	mSetflag = false;

	goSelect = false;
}


Play::~Play()
{
	delete mControl;
	delete mFile;
	//InitGraph();
	//InitSoftImage();
	//InitSoundMem();
	//InitFontToHandle();
	DeleteSoundMem(mSHBGM);
}

Base* Play::update(Parent* parent) {
	Base* next = this;

	update();
	draw();

	if (goSelect) next = new Select();

	return next;
}

void Play::update() {
	mCount++;
	mCount %= 1145141919810;

	if (mReadyCount != -1) { 
		StartUpdate(); 
	}
	else {
		static int fourbeat = (60000000 / mData.fBpm) * 4;
		//if (!mControl->GetGameEnd())
			mControl->update();

		//一小節待つ
		if (GetNowHiPerformanceCount() - mStartTime >= fourbeat && !CheckSoundMem(mSHBGM)) {
			ChangeVolumeSoundMem(VOL_BGM, mSHBGM);
			PlaySoundMem(mSHBGM, DX_PLAYTYPE_BACK);
			
		}
		if (!mSetflag) { 
			mControl->SetStartTime(mStartTime + fourbeat);
			mSetflag = true;
		}
	}

	if (mControl->GetGameEnd()) {
		mResultCount++;
		if (mBright < 255) mBright++;

		if (mBright >= 255 && (GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {
			goSelect = true;
		}

		if (mBright == 1) record();
	}

	if (CheckHitKey(KEY_INPUT_ESCAPE) != 0) goSelect = true;
}

void Play::draw() {
	if (mReadyCount != -1) StartDraw();

	mControl->draw();

	if (mControl->GetGameEnd()) ResultDraw();
}

void Play::StartUpdate() {
	if (mBright < 255 && mReadyCount == 0) {
		mBright += 3;
	}
	else {
		if (mReadyCount == 0) mReadyCount = 1;
	}

	if (mReadyCount > 0) {
		mReadyCount++;
	}

	if (mReadyCount > 100) {
		if (mBright > 0) {
			mBright -= 3;
		}
		else {
			mReadyCount = -1;
			mStartTime =  GetNowHiPerformanceCount();
		}
	}

}

void Play::StartDraw() {
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, mBright);
	DrawGraph(WINDOW_W / 4, WINDOW_H / 4, mGHready, TRUE);
	int w;
	w = GetDrawStringWidthToHandle(mData.mTitle, strlen(mData.mTitle), mFHtitle);
	DrawFormatStringToHandle(WINDOW_W / 2 - w / 2, WINDOW_H / 4 + 100, GetColor(255, 255, 255), mFHtitle, mData.mTitle);
	w = GetDrawStringWidthToHandle(mData.mArtist, strlen(mData.mArtist), mFHdetail);
	DrawFormatStringToHandle(WINDOW_W / 2 - w / 2, WINDOW_H / 4 + 150, GetColor(255, 255, 255), mFHdetail, mData.mArtist);
	w = GetDrawStringWidthToHandle(mData.mGenre, strlen(mData.mGenre), mFHdetail);
	DrawFormatStringToHandle(WINDOW_W / 2 - w / 2, WINDOW_H / 4 + 180, GetColor(255, 255, 255), mFHdetail, mData.mGenre);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void Play::ResultUpdate() {

}

void Play::ResultDraw() {
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, mBright);
	
	//背景
	DrawGraph(0, 0, mGHResultBack[0], TRUE);
	//ジャケット
	int sizeX = sin(3.14 * 2 / 400 * mCount) * 150;
	int sizeY = sin(3.14 * 2 / 300 * mCount) * 20;
	DrawModiGraph(WINDOW_W / 2 - sizeX, 50 - sizeY, WINDOW_W / 2 + sizeX, 50 - sizeY, WINDOW_W / 2 + sizeX, 350 - sizeY, WINDOW_W / 2 - sizeX , 350 - sizeY, mGHJaket, TRUE);
	//フレーム
    DrawGraph(0, (255 - mBright) / 5, mGHResult, TRUE);
	
	//タイトル・レベル
	int w;
	w = GetDrawStringWidthToHandle(mData.mTitle, strlen(mData.mTitle), mFHtitle);
	DrawFormatStringToHandle(WINDOW_W / 2 - w / 2, 100, GetColor(255, 255, 255), mFHtitle, mData.mTitle);
	w = GetDrawStringWidthToHandle(mData.mLevel[mLevel], strlen(mData.mLevel[mLevel]), mFHdetail);
	int Cr = 0;
	switch (mLevel)
	{
	case 0:
		Cr = GetColor(0, 255, 0);
		break;
	case 1:
		Cr = GetColor(0, 0, 255);
		break;
	case 2:
		Cr = GetColor(255, 255, 0);
		break;
	case 3:
		Cr = GetColor(255, 0, 0);
		break;
	default:
		break;
	}
	DrawFormatStringToHandle(WINDOW_W / 2 - w / 2, 140, Cr, mFHdetail, mData.mLevel[mLevel]);

	//PERFECT
	int score = (mBright < 255 - 70) ? GetRand(1000) : mControl->GetRankCount(0);
	for (int i = 3; i >= 0; i--) {
		int num = score / (int)pow(10, i);
		SetDrawBright(255, 100, 100);
		DrawGraph((3 - i) * 27 + 330, 185, GHnum2[num], TRUE);
		score -= (int)pow(10, i) * num;
	}
	//GREAT
	score = (mBright < 255 - 60) ? GetRand(1000) : mControl->GetRankCount(1);
	for (int i = 3; i >= 0; i--) {
		int num = score / (int)pow(10, i);
		SetDrawBright(255, 255, 100);
		DrawGraph((3 - i) * 27 + 330, 245, GHnum2[num], TRUE);
		score -= (int)pow(10, i) * num;
	}
	//NICE
	score = (mBright < 255 - 50) ? GetRand(1000) : mControl->GetRankCount(2);
	for (int i = 3; i >= 0; i--) {
		int num = score / (int)pow(10, i);
		SetDrawBright(100, 255, 100);
		DrawGraph((3 - i) * 27 + 690, 165, GHnum2[num], TRUE);
		score -= (int)pow(10, i) * num;
	}
	//BAD
	score = (mBright < 255 - 40) ? GetRand(1000) : mControl->GetRankCount(3);
	for (int i = 3; i >= 0; i--) {
		int num = score / (int)pow(10, i);
		SetDrawBright(100, 100, 255);
		DrawGraph((3 - i) * 27 + 690, 220, GHnum2[num], TRUE);
		score -= (int)pow(10, i) * num;
	}
	//MISS
	score = (mBright < 255 - 30) ? GetRand(1000) : mControl->GetRankCount(4);
	for (int i = 3; i >= 0; i--) {
		int num = score / (int)pow(10, i);
		SetDrawBright(100, 100, 100);
		DrawGraph((3 - i) * 27 + 690, 270, GHnum2[num], TRUE);
		score -= (int)pow(10, i) * num;
	}
	SetDrawBright(255, 255, 255);

	//コンボ
	score = (mBright < 255 - 20) ? GetRand(1000) : mControl->GetCombo();
	for (int i = 3; i >= 0; i--) {
		int num = score / (int)pow(10, i);
		DrawGraph((3 - i) * 27 + 115, 360, GHnum[num], TRUE);
		score -= (int)pow(10, i) * num;
	}
	int rank = -1;
	if (mControl->GetCombo() == mControl->GetNotesNum()) {
		rank = 0;
	}
	else if (mControl->GetCombo() >= mControl->GetNotesNum() * 3 / 4) {
		rank = 1;
	}
	else if (mControl->GetCombo() >= mControl->GetNotesNum() / 2) {
		rank = 2;
	}
	else if (mControl->GetCombo() >= mControl->GetNotesNum() / 4) {
		rank = 3;
	}
	if (mBright > 225) {
		int ex = (mBright > 235) ? 0 : (235 - mBright) * 10;
		if (rank >= 0) DrawExtendGraph(250 - ex, 330 - ex, 250 + 70 + ex, 330 + 79 + ex, GHRank[rank], TRUE);
		if (mNewrecord[1]) DrawExtendGraph(180 - ex, 285 - ex, 300 + ex, 365 + ex, mGHnewrecord, TRUE);
	}

	//クリア回数
	score = (mBright < 255 - 10) ? GetRand(1000) : mClear;
	for (int i = 3; i >= 0; i--) {
		int num = score / (int)pow(10, i);
		DrawExtendGraph((3 - i) * 15 + 140, 435, (3 - i) * 15 + 140 + 30, 480, GHnum[num], TRUE);
		score -= (int)pow(10, i) * num;
	}
    rank = -1;
	if (mClear >= 100) {
		rank = 0;
	}
	else if (mClear >= 50) {
		rank = 1;
	}
	else if (mClear >= 25) {
		rank = 2;
	}
	else if (mClear >= 10) {
		rank = 3;
	}
	if (mBright > 235) {
		int ex = (mBright > 245) ? 0 : (245 - mBright) * 10;
		if (rank >= 0) DrawExtendGraph(205 - ex, 410 - ex, 265 + ex, 475 + ex, GHRank[rank], TRUE);
	}


	//スコア
	score = (mBright < 255) ? GetRand(1000000) : mControl->GetScore();
	for (int i = 6; i >= 0; i--) {
		int num = score / (int)pow(10, i);
		DrawExtendGraph((6 - i) * 50 + 340, 390, (6 - i) * 50 + 340 + 60, 490, GHnum[num], TRUE);
		score -= (int)pow(10, i) * num;
	}
	rank = -1;
	if (mControl->GetScore() == 1000000) {
		rank = 0;
	}
	else if (mControl->GetScore() >= 950000) {
		rank = 1;
	}
	else if (mControl->GetScore() >= 900000) {
		rank = 2;
	}
	else if (mControl->GetScore() >= 800000) {
		rank = 3;
	}
	if (mBright > 245) {
		int ex = (255 - mBright) * 10;
		if (rank >= 0) DrawExtendGraph(700 - ex, 345 - ex, 840 + ex, 480 + ex, GHRank[rank], TRUE);
		if (mNewrecord[0]) DrawExtendGraph(800 - 180 - ex, 400 - 120 - ex, 800 + ex, 400 + ex, mGHnewrecord, TRUE);
	}

	
	SetDrawBlendMode(DX_BLENDMODE_ADD, 25);
	DrawGraph(0,100 + sin(3.14 * 2 / 1000 * mCount) * 100, mGHResultBack[1], TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void  Play::record() {
	ifstream ifs(mData.mRecord, ios::in | ios::binary);
	int score[4] = { 0,0,0,0 }, combo[4] = { 0,0,0,0 }, clear[4] = { 0,0,0,0 }, notesNum[4] = { 0,0,0,0 };
	mNewrecord[0] = mNewrecord[1] = 0;
	if (!ifs) {
		mNewrecord[0] = mNewrecord[1] = 1;
		score[mLevel] = mControl->GetScore();
		combo[mLevel] = mControl->GetCombo();
	}
	else {
		//ifs.seekg(mLevel * 3 * sizeof(int));
		for (int i = 0; i < 4; i++) {
			ifs.read((char*)&score[i], sizeof(score[i]));
			ifs.read((char*)&combo[i], sizeof(combo[i]));
			ifs.read((char*)&clear[i], sizeof(clear[i]));
			ifs.read((char*)&notesNum[i], sizeof(notesNum[i]));
		}
		//ifs.read((char*)&score[mLevel], sizeof(score[mLevel]));
		//ifs.read((char*)&combo[mLevel], sizeof(combo[mLevel]));
		//ifs.read((char*)&clear[mLevel], sizeof(clear[mLevel]));

		if (score[mLevel] < mControl->GetScore()) {
			mNewrecord[0] = 1;
			score[mLevel] = mControl->GetScore();
		}
		if (combo[mLevel] < mControl->GetCombo()) {
			mNewrecord[1] = 1;
			combo[mLevel] = mControl->GetCombo();
		}
	}
	ifs.close();

	clear[mLevel]++;
	mClear = clear[mLevel];
	notesNum[mLevel] = mControl->GetNotesNum();

	ofstream ofs(mData.mRecord, ios::out | ios::binary | ios::trunc);
	if (!ofs) return;

	//ofs.seekp(mLevel * 3 * sizeof(int));
	for (int i = 0; i < 4; i++) {
		ofs.write((char*)&score[i], sizeof(int));
		ofs.write((char*)&combo[i], sizeof(int));
		ofs.write((char*)&clear[i], sizeof(int));
		ofs.write((char*)&notesNum[i], sizeof(int));
	}

	ofs.close();
}