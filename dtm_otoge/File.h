#pragma once

#define DATA_N 9

#include <vector>

using namespace std;

//譜面ファイルの最後は空白行で終わること
//time,side,type,posX
//ノートタイプについてはControl::intToType()を参照

class File
{
public:
	File();
	~File();

	typedef struct DATA {
		char    mGenre[256];                // データのジャンル
		char    mTitle[256];                // データのタイトル
		char    mArtist[256];               // データの製作者
		float   fBpm;                       // データのテンポ（初期値は130）
		char	mDemo[256];					// デモ音源
		char	mMusic[256];				// 音源
		char	mJaket[256];				// ジャケット画像
		char	mLevel[4][256] = {
			"レベル1",
			"レベル2",
			"レベル3",
			"レベル4"
		};									// レベル毎の名称
		char	mScore[4][256];				// 譜面ファイル
		char	mRecord[256];				// 記録ファイル
	} DATA;

	typedef struct NOTE {
		int mTime;							// 降ってくるタイミング
		int mSide;							// サイド
		int mType;							// タイプ
		int mPosX;							// X座標
	} NOTE;

	bool GetData(const char* fileName, DATA *data); //曲情報を取得
	bool GetNotes(const char* fileName, vector<NOTE> *notes); //譜面情報を取得

private:
	//char* mDirec; //data.txtがあるディレクトリ

	int GetCommand(char *s);
	bool GetCommandString(char *src, char *dst);
	
	void GetDirec(char* direc,char* fileName);
	void InitData(DATA *data);

};

