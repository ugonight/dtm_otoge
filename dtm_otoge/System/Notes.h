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

	void SetNotes(int side, NotesType type);	//ノートをリセット
	void SetNotesType(int side,NotesType type);	//ノートをリセット(タイプのみ)
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
	long long mTime; //カーソルのラインにノートが落ちる時間
	long long mStartTime; //曲が始まった時間
	static int mGHandleL, mGHandleR;
	static int mGHholdL, mGHholdR, mGHholdLE, mGHholdRE,mGHSlideL[2],mGHSlideR[2],mGHtouch[2];
	static int mGHSlideD[2];	//スライド補助矢印
	static int mSpeed;
	int mSizeX,mSizeY;
	int mShowFlag;
	int mSide;
	//int mShowE;	//ホールドの終点のノートが表示されているか
	int mHoldPair;	//ホールドの始点に対する終点、終点に対する始点のID
	NotesType mNotesType;
	EndType mEndType;

	int mStop = 0;
	int mMiss = 0;

	int mSlideD;

	static  int gh;

	
};

