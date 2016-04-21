#pragma once
#include "Child.h"
#include "../File.h"

#include <vector>
#include <string>
#include <windows.h>

//ジャケット
#define JAKET 200
#define SPEED 10

#define PI 3.14159265359F
//音声波形
#define BUFFERLENGTH 16384
#define HIGHT 100
#define WIDTH HIGHT * 2 * PI

using namespace std;

//class File;

class Select :
	public Child
{
public:
	Select();
	~Select();

	Base* update(Parent*);

	void update(int *mouse);
	void draw();
private:
	int GHBlack;
	int GHBack[2];
	int GHDirec[2];
	int GHstatus;
	int GHbutton[3][3];
	int GHspeed[3];
	int GHnum[11];
	int GHclose;
	int mFontHandle,mFontHandleS;
	int mSHandle[2];
	int mTime, mSceneTime;
	bool mButtonOn[3][2];	//0:ボタンが押されているか 1:イベントを発生させるフラグ

	vector<string> mSongList;	//曲データファイル
	vector<File::DATA *> mSongDataList;	//データ構造体
	vector<int> mJaketList;	//ジャケットのイメージハンドル
	vector<int> mDemoSoftList;	//デモ音源のサウンドソフトハンドル
	vector<int> mDemoList;	//デモ音源のサウンドハンドル

	int mdJaket;	//ジャケットの移動量
	int mJaketPos[7];	//ジャケットの位置
	int mJaketImage[7];	//表示するジャケット画像
	int mCurrentSong;	//選択されている曲
	int w;

	int mScreenHandle, mScreenHandle2;
	int mGHbox;
	float mParamList[BUFFERLENGTH];

	int mLevel;
	const int mLevelColor[4][3] = {
		{ 0 ,255 , 0 },
		{ 0 ,255 , 255 },
		{ 255 ,255 , 0 },
		{ 255 ,0 , 0 }
	};
	int mLevelColorNow[3];
	bool mExistScore;	//譜面が存在するか
	int mSpeed,mSpeedClick;

	File* mFile;

	void searchSong(HANDLE hFind, WIN32_FIND_DATA FindFileData, char* direc, char* search);	//曲ファイルを探す
	void drawJaket(int i);
	void setJaket(int i);
	void playDemo(int i);
	void showSpectrum();
	void GetRecord(int *score,int *combo,int *clear, int *num);
};

