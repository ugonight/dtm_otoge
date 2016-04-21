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

	int mNotesNum; //�m�[�g�̍ő吔

	vector<File::NOTE> mNotesInfo; //���ʏ��

	void controlNotes();	//�m�[�g�̐��Y
	void judge();	//����
	
	int canFall(int side);	//���Ƀz�[���h�I�_�����邩
	void holdRelate();	//�����O�m�[�g�̎n�_�ƏI�_�̈ˑ��֌W
	void drawRoot();	//�m�[�g�����Ԃ����`��

	Notes::NotesType intToType(int i);	//�������m�[�g�^�C�v�ɕϊ�

	void click();
	int clickTime[2];

	//int mHold[2];	//�z�[���h���\������Ă��邩
	bool mHoldOn[2];	//�z�[���h��������Ă��邩
	//Notes *mHoldS[2];	//�z�[���h�̎n�_�̃m�[�g
	//int mHoldE[2];	//���݉����Ă���n�_�z�[���h�ɑ΂���I�_�̃m�[�g�̔ԍ�
	void SetHoldRelate(); //�����O�m�[�g�̎n�_�ƏI�_�̃y�A���L�^

	int mSlideS[2];	//�J�[�\�����m�[�g�ɐG�ꂽ�Ƃ��̃h���b�O��
	bool mSlideOn[2]; //�X���C�h�����ǂ���

	int mTouchNum[2];	//�m�[�g�̘A����
	bool mTouchOn[2];	//�J�[�\�����m�[�g�ɐG��Ă�����

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
	Rank mTouchRank[2];	//�J�[�\�����m�[�g�ɐG��Ă������̈�ԗǂ������N

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
	int mFlash[10][3];	//�m�[�g���Ƃ������̉��o 1:�t���O 2:x���W 3:����
	void showFlash();

	int mSHandle[3][2];	//���ʉ��̎�ށE���E
	int mSE[10];

	long long mStartTime;	//�Ȃ̊J�n��������



	// �t�@�C���o�̓X�g���[���̏�����
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

