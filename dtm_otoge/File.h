#pragma once

#define DATA_N 9

#include <vector>

using namespace std;

//���ʃt�@�C���̍Ō�͋󔒍s�ŏI��邱��
//time,side,type,posX
//�m�[�g�^�C�v�ɂ��Ă�Control::intToType()���Q��

class File
{
public:
	File();
	~File();

	typedef struct DATA {
		char    mGenre[256];                // �f�[�^�̃W������
		char    mTitle[256];                // �f�[�^�̃^�C�g��
		char    mArtist[256];               // �f�[�^�̐����
		float   fBpm;                       // �f�[�^�̃e���|�i�����l��130�j
		char	mDemo[256];					// �f������
		char	mMusic[256];				// ����
		char	mJaket[256];				// �W���P�b�g�摜
		char	mLevel[4][256] = {
			"���x��1",
			"���x��2",
			"���x��3",
			"���x��4"
		};									// ���x�����̖���
		char	mScore[4][256];				// ���ʃt�@�C��
		char	mRecord[256];				// �L�^�t�@�C��
	} DATA;

	typedef struct NOTE {
		int mTime;							// �~���Ă���^�C�~���O
		int mSide;							// �T�C�h
		int mType;							// �^�C�v
		int mPosX;							// X���W
	} NOTE;

	bool GetData(const char* fileName, DATA *data); //�ȏ����擾
	bool GetNotes(const char* fileName, vector<NOTE> *notes); //���ʏ����擾

private:
	//char* mDirec; //data.txt������f�B���N�g��

	int GetCommand(char *s);
	bool GetCommandString(char *src, char *dst);
	
	void GetDirec(char* direc,char* fileName);
	void InitData(DATA *data);

};

