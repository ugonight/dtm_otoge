#pragma once
class Notes
{
public:
	Notes();
	~Notes();

	void update();
	void draw();

	int GetY();
	int GetX();
	int GetSize();
	int GetSide();
	int GetShowFlag();
	int GetTime();
	int GetPair();
	int GetStop();

	//void SetShowE(int i);
	//void SetHoldS(Notes *note);
	void SetStop(int i);
	int checkMiss();

	enum NotesType {
		TYPE_NONE,
		TYPE_CLICK,
		TYPE_HOLD_S,
		TYPE_HOLD_E,
		TYPE_HOLD_VOID,
		TYPE_SLIDE_L,
		TYPE_SLIDE_R,
		TYPE_TOUCH,
		NUM
	};

	enum EndType {
		END_NOMAL,
		END_SLIDE_L,
		END_SLIDE_R,
		END_NUM
	};

	void SetNotes(int side, NotesType type);	//�m�[�g�����Z�b�g
	void SetNotesType(int side,NotesType type);	//�m�[�g�����Z�b�g(�^�C�v�̂�)
	void SetEndType(EndType type);
	void SetX(int x);
	void SetTime(long long t);
	void SetStartTime(long long t);
	void SetSpeed(int speed);
	void SetPair(int id);
	void DeleteNotes();
	NotesType GetNotesType();
	EndType GetEndType();

	void LoadNoteGraph();

private:
	int mX, mY;
	long long mTime; //�J�[�\���̃��C���Ƀm�[�g�������鎞��
	long long mStartTime; //�Ȃ��n�܂�������
	static int mGHandleL, mGHandleR;
	static int mGHholdL, mGHholdR, mGHholdLE, mGHholdRE,mGHSlideL[2],mGHSlideR[2],mGHtouch[2];
	static int mGHSlideD[2];	//�X���C�h�⏕���
	static int mSpeed;
	int mSizeX,mSizeY;
	int mShowFlag;
	int mSide;
	//int mShowE;	//�z�[���h�̏I�_�̃m�[�g���\������Ă��邩
	int mHoldPair;	//�z�[���h�̎n�_�ɑ΂���I�_�A�I�_�ɑ΂���n�_��ID
	NotesType mNotesType;
	EndType mEndType;

	int mStop = 0;
	int mMiss = 0;

	int mSlideD;

	static  int gh;

	
};

