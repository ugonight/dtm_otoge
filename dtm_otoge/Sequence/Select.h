#pragma once
#include "Child.h"
#include "../File.h"

#include <vector>
#include <string>
#include <windows.h>

//�W���P�b�g
#define JAKET 200
#define SPEED 10

#define PI 3.14159265359F
//�����g�`
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
	bool mButtonOn[3][2];	//0:�{�^����������Ă��邩 1:�C�x���g�𔭐�������t���O

	vector<string> mSongList;	//�ȃf�[�^�t�@�C��
	vector<File::DATA *> mSongDataList;	//�f�[�^�\����
	vector<int> mJaketList;	//�W���P�b�g�̃C���[�W�n���h��
	vector<int> mDemoSoftList;	//�f�������̃T�E���h�\�t�g�n���h��
	vector<int> mDemoList;	//�f�������̃T�E���h�n���h��

	int mdJaket;	//�W���P�b�g�̈ړ���
	int mJaketPos[7];	//�W���P�b�g�̈ʒu
	int mJaketImage[7];	//�\������W���P�b�g�摜
	int mCurrentSong;	//�I������Ă����
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
	bool mExistScore;	//���ʂ����݂��邩
	int mSpeed,mSpeedClick;

	File* mFile;

	void searchSong(HANDLE hFind, WIN32_FIND_DATA FindFileData, char* direc, char* search);	//�ȃt�@�C����T��
	void drawJaket(int i);
	void setJaket(int i);
	void playDemo(int i);
	void showSpectrum();
	void GetRecord(int *score,int *combo,int *clear, int *num);
};

