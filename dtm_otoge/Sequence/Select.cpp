#include "Select.h"

#include "Parent.h"
#include "Play.h"
//#include "../File.h"
#include "../define.h"

#include <math.h>
#include <stdio.h>

#include <fstream>
using namespace std;

Select::Select()
{
	InitGraph();
	InitSoftImage();
	InitSoundMem();
	InitFontToHandle();

	GHBlack = LoadGraph("data/black.png");
	DrawGraph(0, 0, GHBlack, FALSE);

	PROGRESS_FRAME
	GHBack[0] = LoadGraph("data/select_back.png");
	GHBack[1] = LoadGraph("data/select_back_p.png");

	PROGRESS(1,7)
	GHDirec[LEFT] = LoadGraph("data/left.png");
	GHDirec[RIGHT] = LoadGraph("data/right.png");
	GHstatus = LoadGraph("data/status.png");
	GHbutton[0][0] = LoadGraph("data/button1_off.png");
	GHbutton[0][1] = LoadGraph("data/button1_on.png");
	GHbutton[1][0] = LoadGraph("data/button2.png");
	GHbutton[2][0] = LoadGraph("data/button3_off.png");
	GHbutton[2][1] = LoadGraph("data/button3_on.png");
	GHbutton[2][2] = LoadGraph("data/button3_void.png");
	GHspeed[0] = LoadGraph("data/speed_L.png");
	GHspeed[1] = LoadGraph("data/speed_C.png");
	GHspeed[2] = LoadGraph("data/speed_R.png");
	GHclose = LoadGraph("data/close.png");
	LoadDivGraph("data/num.png", 11, 11, 1, 50, 70, GHnum);
	mFontHandle = CreateFontToHandle("�l�r �S�V�b�N", 18, 3, DX_FONTTYPE_ANTIALIASING);
	mFontHandleS = CreateFontToHandle("�l�r �S�V�b�N", 14, 3, DX_FONTTYPE_ANTIALIASING);
	mSHandle[0] = LoadSoundMem("data/ok.ogg");
	mSHandle[1] = LoadSoundMem("data/change.ogg");
	mTime = mSceneTime = 0;
	mdJaket = 0;
	mLevel = DEF_LEVEL;
	mSpeed = DEF_SPEED;
	mCurrentSong = DEF_SONG;
	mLevelColorNow[0] = mLevelColor[0][0]; mLevelColorNow[1] = mLevelColor[0][1]; mLevelColorNow[2] = mLevelColor[0][2];
	mExistScore = 0;

	PROGRESS(2, 7)
	w = (WINDOW_W - 84 * 2) / 6;
	//�W���P�b�g�̏����ʒu
	mJaketPos[0] = 0;
	mJaketPos[1] = 84;
	mJaketPos[2] = w + 84;
	mJaketPos[3] = WINDOW_W / 2 - JAKET / 2;
	mJaketPos[4] = -w - 84 - JAKET + WINDOW_W;
	mJaketPos[5] = - 84 - JAKET + WINDOW_W;
	mJaketPos[6] = -JAKET + WINDOW_W;

	PROGRESS(3, 7)
	//�ȃf�[�^�t�@�C���̌���
	HANDLE hFind;					//�����n���h��
	WIN32_FIND_DATA FindFileData;	//�t�@�C���f�[�^
	hFind = FindFirstFile("songs\\*.*", &FindFileData);	//song���̑S�Ẵt�@�C���E�f�B���N�g���ɂ��Č���
	searchSong(hFind, FindFileData, "songs\\", "data.txt");	//songs���ɂ���humen.txt��T��
	FindClose(hFind);	//�����n���h�������

	PROGRESS(4, 7)
	vector<string>::iterator it = mSongList.begin();
	File::DATA data;
	int i = 0;
	while (it != mSongList.end()) {
		//printfDx("%s\n", it->c_str());
		mFile->GetData(it->c_str(), &data);
		//printfDx(" %s\n", data.mTitle);
		//printfDx(" %s\n", data.mGenre);
		//printfDx(" %s\n", data.mJaket);
		File::DATA *d = new File::DATA;
		*d = data;
		mSongDataList.push_back(d);
		mJaketList.push_back(LoadGraph(data.mJaket));
		mDemoSoftList.push_back(LoadSoftSound(data.mDemo));
		mDemoList.push_back(LoadSoundMemFromSoftSound(mDemoSoftList[i]));
		++it;
		i++;
	}

	PROGRESS(5, 7)
	//�����g�`�p�O���t�B�b�N
	mScreenHandle = MakeARGB8ColorSoftImage(WIDTH, HIGHT);
	mScreenHandle2 = MakeARGB8ColorSoftImage(HIGHT * 2, HIGHT * 2);
	mGHbox = LoadSoftImage("data/box.png");

	PROGRESS(6, 7)
	setJaket(mCurrentSong);
	ChangeVolumeSoundMem(VOL_BGM, mDemoList[mCurrentSong]);
	PlaySoundMem(mDemoList[mCurrentSong], DX_PLAYTYPE_LOOP);

	PROGRESS(7, 7)
}


Select::~Select()
{
	StopSoundMem(mDemoList[mCurrentSong]);
	DEF_LEVEL = mLevel;
	DEF_SONG = mCurrentSong;
	DEF_SPEED = mSpeed;
	//InitGraph();
	//InitSoftImage();
	//InitSoundMem();
	//InitFontToHandle();
}

Base* Select::update(Parent* parent) {
	Base* next = this;

	int mouse = 0;
	update(&mouse);
	draw();

	if (mouse) parent->MouseCurrent();

	if (mButtonOn[2][1]) {
		mSceneTime++;
	}
	if (mSceneTime > 0) {
		mSceneTime++;
		if (mSceneTime > 50) {
			int br = 255 * (((double)mSceneTime - 50.0) / 50.0);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, br);
			DrawGraph(0, 0, GHBlack, FALSE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
	}
	if (mSceneTime > 100) {
		next = new Play(mSongList[mCurrentSong], mLevel, mSpeed);
	}



	return next;
}

void Select::update(int *mouse) {
	//�_�ŗp�^�C�}�[
	mTime++;
	mTime %= 32767;

	int MouseX, MouseY;
	GetMousePoint(&MouseX, &MouseY);

	//�W���P�b�g�ړ��J�n
	if (mdJaket == 0) {
		if (MouseX > 20 && MouseX < 84 && MouseY >  WINDOW_H * 2 / 5 - 32 && MouseY < WINDOW_H * 2 / 5 + 32) {	//��
			*mouse = 1; //�}�E�X�����点��
			if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {
				mdJaket = -1;
				ChangeVolumeSoundMem(VOL_SE, mSHandle[1]);
				PlaySoundMem(mSHandle[1], DX_PLAYTYPE_BACK);
			}
		}
		if (MouseX > WINDOW_W - 84 && MouseX < WINDOW_W - 20 && MouseY >  WINDOW_H * 2 / 5 - 32 && MouseY < WINDOW_H * 2 / 5 + 32) {	//��
			*mouse = 1;
			if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {
				mdJaket = 1;
				ChangeVolumeSoundMem(VOL_SE, mSHandle[1]);
				PlaySoundMem(mSHandle[1], DX_PLAYTYPE_BACK);
			}
		}
	}
	else if (abs(mdJaket) == SPEED) {
		if (mdJaket == SPEED) {
			setJaket(mCurrentSong - 1);
		}
		else {
			setJaket(mCurrentSong + 1);
		}
		mdJaket = 0;
		playDemo(mCurrentSong);
	}
	else {
		playDemo(-1);
		if (mdJaket > 0) {
			mdJaket++;
		}
		else {
			mdJaket--;
		}
	}

	//�{�^���������ꂽ���ǂ���	
	mButtonOn[0][1] = mButtonOn[1][1] = mButtonOn[2][1] = 0; //�S���I�t�ɂ��Ă���
	//�ݒ�
	/*
	if (MouseX > 710 && MouseX < 860 && MouseY > 5 && MouseY < 59) {
		*mouse = 1;
		if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) { 
			mButtonOn[0][0] = 1; 
		}
		else {
			if (mButtonOn[0][0]) {
				mButtonOn[0][1] = 1;
			}
			mButtonOn[0][0] = 0;
		}
	}
	else {
		mButtonOn[0][0] = 0;
	}*/
	//���x���ύX
	if (MouseX > 65 && MouseX < 155 && MouseY > 360 && MouseY < 456) {
		*mouse = 1;
		if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {
			if (!mButtonOn[1][0]) {
				mButtonOn[1][1] = 1;
			}
			mButtonOn[1][0] = 1;
		}
		else {

			mButtonOn[1][0] = 0;
		}
	}
	else {
		mButtonOn[1][0] = 0;
	}

	//����
	if (mExistScore) {
		if (MouseX > 610 && MouseX < 800 && MouseY > 360 && MouseY < 460) {
			*mouse = 1;
			if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {
				mButtonOn[2][0] = 1;
			}
			else {
				if (mButtonOn[2][0]) {
					mButtonOn[2][1] = 1;
					ChangeVolumeSoundMem(VOL_SE, mSHandle[0]);
					PlaySoundMem(mSHandle[0], DX_PLAYTYPE_BACK);
				}
				mButtonOn[2][0] = 0;
			}
		}
		else {
			mButtonOn[2][0] = 0;
		}
	}


	//���x���ύX
	if (mButtonOn[1][1]) 
		mLevel++; mLevel %= 4;

	if (mLevelColorNow[0] != mLevelColor[mLevel][0] || mLevelColorNow[1] != mLevelColor[mLevel][1] || mLevelColorNow[2] != mLevelColor[mLevel][2]) {
		if (mLevelColorNow[0] < mLevelColor[mLevel][0]) {
			mLevelColorNow[0] += 5;
		}
		else {
			mLevelColorNow[0] -= 5;
		}
		if (mLevelColorNow[1] < mLevelColor[mLevel][1]) {
			mLevelColorNow[1] += 5;
		}
		else {
			mLevelColorNow[1] -= 5;
		}
		if (mLevelColorNow[2] < mLevelColor[mLevel][2]) {
			mLevelColorNow[2] += 5;
		}
		else {
			mLevelColorNow[2] -= 5;
		}
	}

	//����
	if (MouseX > 0 && MouseX < 64 && MouseY > 0 && MouseY < 64) {
		*mouse = 1;
		if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {
			exitApplication = 1;
		}
	}

	//���ʂ����݂��邩
	mExistScore = strcmp(mSongDataList[mCurrentSong]->mScore[mLevel], "");
	
	//�X�s�[�h�ύX
	if (MouseX > 150 && MouseX < 180 && MouseY > 380 && MouseY < 440) {
		*mouse = 1;
		if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {
			if (!mSpeedClick) mSpeed--; 
			mSpeedClick = 1;
		}
		else {
			mSpeedClick = 0;
		}
	}
	if (MouseX > 260 && MouseX < 290 && MouseY > 380 && MouseY < 440) {
		*mouse = 1;
		if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {
			if (!mSpeedClick) mSpeed++;
			mSpeedClick = 1;
		}
		else {
			mSpeedClick = 0;
		}
	}
	//mSpeed %= 11;
	if (!mSpeed) { mSpeed = 10; }
	else if (mSpeed == 11) { mSpeed = 1; }
}

void Select::draw() {
	//�w�i
	DrawGraph(0, 0, GHBack[0], TRUE);
	SetDrawBlendMode(DX_BLENDMODE_ADD, (int)(sin(mTime / 100.0) * 80.0 + 125.0));
	DrawGraph(0, (int)(sin(mTime / 100.0) * 80.0 + 125.0), GHBack[1], TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	showSpectrum();

	//���
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)(sin(mTime / 10.0) * 80.0 + 125.0));
	DrawGraph(20, WINDOW_H * 2 / 5 - 32, GHDirec[LEFT], TRUE);
	DrawGraph(WINDOW_W - 84, WINDOW_H * 2 / 5 - 32, GHDirec[RIGHT], TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//�W���P�b�g
	if (mdJaket == 0) {
		//drawJaket(0);
		//drawJaket(6);
		drawJaket(1);
		drawJaket(5);
		drawJaket(2);
		drawJaket(4);
		drawJaket(3);
	}
	else if (mdJaket > 0) {
		drawJaket(0);
		//drawJaket(6);
		drawJaket(1);
		drawJaket(5);
		drawJaket(4);
		drawJaket(3);
		drawJaket(2);
	}
	else {
		//drawJaket(0);
		drawJaket(6);
		drawJaket(1);
		drawJaket(5);
		drawJaket(2);
		drawJaket(3);
		drawJaket(4);
	}

	//�X�e�[�^�X
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
	DrawGraph(0, 0, GHstatus, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	//File::DATA data;
	//mFile->GetData(mSongList[mCurrentSong].c_str(), &data);
	int combo, score, clear, num;
	GetRecord(&score, &combo, &clear, &num);
	DrawFormatStringToHandle(90, 325, GetColor(20, 20, 20), mFontHandle, "%s  / %s", mSongDataList[mCurrentSong]->mTitle, mSongDataList[mCurrentSong]->mArtist);
	DrawFormatStringToHandle(300, 370, GetColor(20, 20, 20), mFontHandleS, "�R���{");
	DrawFormatStringToHandle(400, 370, GetColor(20, 20, 20), mFontHandleS, "%07d / %07d", combo, num);
	DrawFormatStringToHandle(300, 400, GetColor(20, 20, 20), mFontHandleS, "�X�R�A");
	DrawFormatStringToHandle(400, 400, GetColor(20, 20, 20), mFontHandleS, "%07d", score);
	DrawFormatStringToHandle(300, 430, GetColor(20, 20, 20), mFontHandleS, "�N���A��");
	DrawFormatStringToHandle(400, 430, GetColor(20, 20, 20), mFontHandleS, "%07d", clear);


	//�{�^��
	//DrawGraph(710, 5, GHbutton[0][mButtonOn[0][0]], TRUE);	//�ݒ�

	SetDrawBright(mLevelColorNow[0], mLevelColorNow[1], mLevelColorNow[2]);
	DrawGraph(65, 360, GHbutton[1][0], TRUE);	//���x��
	int w = GetDrawStringWidthToHandle(mSongDataList[mCurrentSong]->mLevel[mLevel], strlen(mSongDataList[mCurrentSong]->mLevel[mLevel]), mFontHandleS);
	DrawFormatStringToHandle(110 - w / 2, 445, GetColor(100, 100, 100), mFontHandleS, "%s", mSongDataList[mCurrentSong]->mLevel[mLevel]);
	SetDrawBright(255, 255, 255);

	if (mExistScore) {
		DrawGraph(610, 360, GHbutton[2][mButtonOn[2][0]], TRUE);	//����
	}
	else {
		DrawGraph(610, 360, GHbutton[2][2], TRUE);
	}

	//�X�s�[�h
	DrawStringToHandle(192, 370, "�X�s�[�h", GetColor(0, 0, 150), mFontHandleS);
	DrawGraph(150, 370, GHspeed[0], TRUE);
	DrawGraph(190, 390, GHspeed[1], TRUE);
	DrawGraph(260, 370, GHspeed[2], TRUE);
	DrawExtendGraph(195, 375, 195 + 50, 375 + 70, GHnum[mSpeed], TRUE);

	//����
	DrawGraph(0, 0, GHclose, TRUE);
}

void Select::drawJaket(int i) {
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
	
	int move = 0;
	int ext = 0;
	
		//�g��
		if (i < 3) {
			ext = (int)( i * 6.0 + 6.0 * ((double)mdJaket / SPEED));
		}
		else if (i == 3) {
			ext = (int)(i * 6.0 - 6.0 * (abs((double)mdJaket) / SPEED));
		}
		else if (i > 3) {
			ext = (int)((6 - i) * 6.0 - 6.0 * ((double)mdJaket / SPEED));
		}

	if (mdJaket > 0) {	//�E�ɓ����Ƃ�
		move = (mJaketPos[i + 1] - mJaketPos[i]) * ((double)mdJaket / SPEED);

		//�����x
		if (i == 0) {
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * ((double)mdJaket / SPEED));
		}
		else if (i == 5) {
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * ((SPEED - (double)mdJaket) / SPEED));
		}

		//���邳
		//if (i != 2) {
		//	SetDrawBright(200, 200, 200);
		//}
	}
	else if (mdJaket < 0) {	//���ɓ����Ƃ�
		move = (mJaketPos[i] - mJaketPos[i - 1]) * ((double)mdJaket / SPEED);

		//�����x
		if (i == 6) {
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * (-(double)mdJaket / SPEED));
		}
		else if (i == 1) {
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * ((SPEED + (double)mdJaket) / SPEED));
		}

		//���邳
		//if (i != 4) {
		//	SetDrawBright(200, 200, 200);
		//}

	}
	
	//���邳
	if (i != 3) {
		SetDrawBright(150, 150, 150);
	}

	if (i==3) SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

	//DrawBox(mJaketPos[i] - ext + move, WINDOW_H * 2 / 5 - JAKET / 2 - ext, mJaketPos[i] + JAKET + ext + move, WINDOW_H * 2 / 5 + JAKET / 2 + ext, GetColor(255, 255, 255), TRUE);
	DrawExtendGraph(mJaketPos[i] - ext + move, WINDOW_H * 2 / 5 - JAKET / 2 - ext, mJaketPos[i] + JAKET + ext + move, WINDOW_H * 2 / 5 + JAKET / 2 + ext, mJaketImage[i], TRUE);
	DrawBox(mJaketPos[i] - ext + move, WINDOW_H * 2 / 5 - JAKET / 2 - ext, mJaketPos[i] + JAKET + ext + move, WINDOW_H * 2 / 5 + JAKET / 2 + ext, GetColor(0, 0, 0), FALSE);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	SetDrawBright(255, 255, 255);
}



void Select::searchSong(HANDLE hFind, WIN32_FIND_DATA FindFileData, char* direc, char* search) {
	if (hFind == INVALID_HANDLE_VALUE) return;	//�֐������s������I��

	do {
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {	//�t�@�C���������f�B���N�g���̏ꍇ
			if (strcmp(FindFileData.cFileName, "..") && strcmp(FindFileData.cFileName, ".")) {	//���̊K�w��������
																								//printf("%s (DIR)\n", FindFileData.cFileName);
				char s[CHAR_MAX] = "", s2[CHAR_MAX] = "";
				sprintf_s(s, CHAR_MAX, "%s%s\\", direc, FindFileData.cFileName);	//���݂̃f�B���N�g��\���̃f�B���N�g��
				sprintf_s(s2, CHAR_MAX, "%s\\*.*", s);								//���݂̃f�B���N�g��\���̃f�B���N�g��\*.*
				HANDLE hFind2 = FindFirstFile(s2, &FindFileData);				//���̃f�B���N�g���ɂ��Č���
				searchSong(hFind2, FindFileData, s, search);						//�ċA
				FindClose(hFind2);
			}
		}
		else {
			//printf("%s%s\n", direc, FindFileData.cFileName);
			if (!strcmp(FindFileData.cFileName, search)) {		//�T�������t�@�C�����ƈ�v���Ă�����
				//printf("%s%s\n", direc, FindFileData.cFileName);//�t�@�C���p�X��\��
				string s = direc, s2 = FindFileData.cFileName;
				mSongList.push_back(s + s2);
			}
		}
	} while (FindNextFile(hFind, &FindFileData));		//�t�@�C����������Ȃ��Ȃ�܂Ō���

}

void Select::setJaket(int i) {

	int max = mSongList.size();
	int former = i;


	//�^�񒆂��Z�b�g
	if (max <= i) {
		mJaketImage[3] = mJaketList[0];
		mCurrentSong = 0;
		former = 0;
	} else if (i < 0) {
		mJaketImage[3] = mJaketList[max-1];
		mCurrentSong = max-1;
		former = max-1;
	} else {
		mJaketImage[3] = mJaketList[i];
		mCurrentSong = i;
	}

	//�T�C�h���Z�b�g
	for (int j = 4; j < 7; j++) {
		if (max > former + 1) {
			mJaketImage[j] = mJaketList[former + 1];
			former = former + 1;
		}
		else {
			mJaketImage[j] = mJaketList[0];
			former = 0;
		}
	}
	former = mCurrentSong;
	for (int j = 2; j >= 0; j--) {
		if (0 <= former - 1) {
			mJaketImage[j] = mJaketList[former - 1];
			former = former - 1;
		}
		else {
			mJaketImage[j] = mJaketList[max - 1];
			former = max - 1;
		}
	}
}

void Select::playDemo(int i) {
	//�S�Ă̍Đ����~
	vector<int>::iterator it = mDemoList.begin();
	while (it != mDemoList.end()) {
		StopSoundMem(*it);
		it++;
	}
	if (i != -1) { ChangeVolumeSoundMem(VOL_BGM, mDemoList[i]); PlaySoundMem(mDemoList[i], DX_PLAYTYPE_LOOP); }
}

void Select::showSpectrum() {
	// ���݂̍Đ��ʒu���擾
	int SamplePos = GetCurrentPositionSoundMem(mDemoList[mCurrentSong]);

	// ���݂̍Đ��ʒu���� 4096 �T���v�����g�p���Ď��g�����z�𓾂�
	GetFFTVibrationSoftSound(mDemoSoftList[mCurrentSong], -1, SamplePos, BUFFERLENGTH, mParamList, BUFFERLENGTH);

	//SetDrawScreen(ScreenHandle);
	//ClearDrawScreen();
	FillSoftImage(mScreenHandle, 0, 0, 0, 0);
	FillSoftImage(mScreenHandle2, 0, 0, 0, 0);

	//time++;

	int boxW = WIDTH / 41;

	int x = -1;
	static int a = 0;
	int i,j = 0;
	int k = 0;
	// ���g�����z����ʂɕ`�悷��
	for (i = 0; i < BUFFERLENGTH; i++)
	{
		if ((int)(log10((double)i) * 10) != x) {
			k++;
			x = (int)(log10((double)i) * 10);

			float Param;

			// �֐�����擾�ł���l��`��ɓK�����l�ɒ���
			Param = pow(mParamList[i], 0.5F) * 4.0f;

			//x = log10(i);
			// �c����`��
			//DrawBox(i * 5, HIGHT - (int)(Param * HIGHT), i * 5 + 2, HIGHT, GetColor(255, 255, 0), TRUE);

			for (j = 0; (j * 6) < HIGHT; j++) {
				if ((int)(Param * HIGHT) > (j - 1) * 6) {
					BltSoftImage(0, 0, boxW, 8, mGHbox, (k - 1) * boxW, HIGHT - j * 6 - 4, mScreenHandle);
				}
			}

		}


	}
	//if (!a) printfDx("%d,%f\n", k, x);
	//DrawSoftImage(0, 300, ScreenHandle);



	int red, green, blue, alpha;
	double rad;
	//�ɍ��W�ϊ�
	for (i = 0; i < HIGHT; i++)
	{
		for (j = 0; j < WIDTH; j++) {
			GetPixelSoftImage(mScreenHandle, j, i, &red, &green, &blue, &alpha);

			rad = ((double)j / (double)(WIDTH)) * (2 * PI) + (double)mTime / (2 * PI * 50);
			DrawPixelSoftImage(mScreenHandle2, (int)(i * cos(rad)) + HIGHT, (int)(i * sin(rad)) + HIGHT, red, green, blue, alpha);
			//DrawPixelSoftImage(ScreenHandle2, j, i, red, green, blue, alpha);
		}
		//if (!a) printfDx("%n");
	}

	SetDrawBlendMode(DX_BLENDMODE_SUB, 80);
	DrawExtendGraph(-100, -100, 500, 500, CreateGraphFromSoftImage(mScreenHandle2), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void Select::GetRecord(int *score, int *combo, int *clear, int *num) {
	ifstream ifs(mSongDataList[mCurrentSong]->mRecord, ios::in | ios::binary);
	int sc=0, co=0, cl=0, nu = 0;
	if (!ifs) {
		sc = co = cl = nu = 0;
	}
	else {
		ifs.seekg(mLevel * 4 * sizeof(int));
		ifs.read((char*)&sc, sizeof(sc));
		ifs.read((char*)&co, sizeof(co));
		ifs.read((char*)&cl, sizeof(cl));
		ifs.read((char*)&nu, sizeof(nu));
	}
	ifs.close();
	*score = sc; *combo = co; *clear = cl; *num = nu;
}