#pragma once

#define PATH_NUM 30
#define CSIZE 64	//�J�[�\���̃T�C�Y

class Mouse
{
public:
	Mouse();
	~Mouse();

	void update();
	void draw();

	void SetCurrent();
	bool GetClick();	//�N���b�N���ꂽ�u�Ԃ�1��Ԃ�
private:
	int mGHandle[3];
	int mGHPath;
	bool mCurrent;
	bool mClick;
	int mTime;	//�Đ��ʒu
	int mSize;

	struct path {
		int x, y;
		int count;
	};

	struct path mPath[PATH_NUM];
};

