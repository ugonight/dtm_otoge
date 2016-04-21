#include "Title.h"
#include "Parent.h"
#include "Select.h"
#include "../define.h"
#include <math.h>

Title::Title()
{
	mFHandle = CreateFontToHandle("ÇlÇr ÇoÉSÉVÉbÉN", 30, 3, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	
	char s[128];
	
	DrawBox(WINDOW_W - 110, WINDOW_H - 50, WINDOW_W - 10, WINDOW_H - 30, GetColor(255,255,255), FALSE);
	//for (int i = 0; i < FNUM; i++) {
	//	sprintf_s(s, 100 ,"data/title/title%04d.png", i);
	//	mLogo[i] = LoadGraph(s);

	//	//êiíª
	//	DrawBox(WINDOW_W - 110, WINDOW_H - 50, WINDOW_W - 10 - (i / FNUM) * 100, WINDOW_H - 30, GetColor(255, 255, 255), TRUE);
	//}
	
	mGHTitle = LoadGraph("data/title.png");
	mGHLight = LoadGraph("data/title_e.png");
	mDestHandle = MakeScreen(WINDOW_W, WINDOW_H, TRUE);

	mBack[0] = LoadGraph("data/title_back.png");
	mBack[1] = LoadGraph("data/title_back_r.png");
	mBack[2] = LoadGraph("data/title_back_b.png");

	mSHandle = LoadSoundMem("data/ok.ogg");

	//int w, h;
	//GetGraphSize(mLogo[0], &w, &h);
	//mScreen = MakeScreen(w, h, TRUE);
	//mMaskHandle = LoadMask("data/title_m.png");
	mTime = mTime2 = 0;
	mMoveCount = 0;
}


Title::~Title()
{
	DeleteFontToHandle(mFHandle);
	//for (int i = 0; i < FNUM; i++) {
	//	DeleteGraph(mLogo[i]);
	//}
}

Base* Title::update(Parent* parent) {
	Base* next = this;

	//îwåi
	int rgb[3];
	HSVtoRGB(abs((int)(sin(mTime2 / 500.0) * 360.0)), 255, 255, rgb);
	SetDrawBright(rgb[0],rgb[1], rgb[2]);
	DrawGraph(0, 0, mBack[0], TRUE);
	SetDrawBright(255, 255, 255);

	SetDrawBlendMode(DX_BLENDMODE_ADD, (int)(sin(mTime2 / 30.0) * 80.0 + 125.0));
	DrawGraph(0, 0, mBack[1], TRUE);
	SetDrawBlendMode(DX_BLENDMODE_ADD, (int)(sin(mTime2 / 30.0 + 3.14) * 80.0 + 125.0));
	DrawGraph(0, 0, mBack[2], TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);


	//ÉçÉS
	mTime+=10;
	mTime %= WINDOW_W * 4;//FNUM;
	//mTime %= 2147483647;

	//DrawGraph(0, 0, mLogo[mTime], TRUE);
	GraphBlendRectBlt(mGHTitle, mGHLight, mDestHandle, 0, 0, WINDOW_W, WINDOW_H, WINDOW_W - mTime, 0, 0, 0, 255, DX_GRAPH_BLEND_ADD);
	DrawGraph(0, sin(3.14*2.0 / 400.0 * (double)mTime2) * 30, mDestHandle, TRUE);

	//int t = mTime * 5;
	//int ex = t %= WINDOW_W;
	
	//CreateMaskScreen();
	////DrawBox(0, 0, 640, 480, GetColor(255, 0, 0), TRUE);
	//DrawMask(WINDOW_W / 2 - 200, 100, mMaskHandle, DX_MASKTRANS_BLACK);
	//SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
	////DrawLine(WINDOW_W / 2 - 200, y, WINDOW_W / 2 + 200, y + 20, GetColor(255,255,255), 3);
	//if (ex > WINDOW_W / 2 - 200 && ex < WINDOW_W / 2 + 100)
	//	DrawGraph(ex, 100, mEffect, TRUE);
	//SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	//DeleteMaskScreen();

	
	mTime2++;
	mTime2 %= 32767;

	if (!mMoveCount) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)(sin(mTime2 / 10.0) * 80.0 + 125.0)); 
	}
	else {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)(sin(mTime2 / 2.0) * 80.0 + 125.0));
	}
	int w = GetDrawStringWidthToHandle("CLICK TO START", 14, mFHandle);
	int x = WINDOW_W / 2 - w / 2;
	DrawStringToHandle(x,350,"CLICK TO START",GetColor(255,255,255),mFHandle);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	int MouseX, MouseY;
	GetMousePoint(&MouseX, &MouseY);

	if (MouseX > x && MouseX < x + w && MouseY > 350 && MouseY < 380) parent->MouseCurrent();

	//ÉVÅ[ÉìëJà⁄
	if (parent->Click() && !mMoveCount) {
		mMoveCount = 50;
		ChangeVolumeSoundMem(VOL_SE, mSHandle);
		PlaySoundMem(mSHandle, DX_PLAYTYPE_BACK);
	}
	if (mMoveCount == 1) {
		next = new Select;
	}
	else if (mMoveCount > 0) {
		mMoveCount--; 
	}


	return next;
}

//HSVÅ®RGBïœä∑
//éQçl:http://www.technotype.net/tutorial/tutorial.php?fileId=%7BImage%20processing%7D&sectionId=%7B-converting-between-rgb-and-hsv-color-space%7D
void Title::HSVtoRGB(int h, int s, int v, int* rgb) {
	float f;
	int i, p, q, t;
	//int* rgb = new int[3];

	i = (int)floor(h / 60.0f) % 6;
	f = (float)(h / 60.0f) - (float)floor(h / 60.0f);
	p = (int)round(v * (1.0f - (s / 255.0f)));
	q = (int)round(v * (1.0f - (s / 255.0f) * f));
	t = (int)round(v * (1.0f - (s / 255.0f) * (1.0f - f)));

	switch (i) {
	case 0: rgb[0] = v; rgb[1] = t; rgb[2] = p; break;
	case 1: rgb[0] = q; rgb[1] = v; rgb[2] = p; break;
	case 2: rgb[0] = p; rgb[1] = v; rgb[2] = t; break;
	case 3: rgb[0] = p; rgb[1] = q; rgb[2] = v; break;
	case 4: rgb[0] = t; rgb[1] = p; rgb[2] = v; break;
	case 5: rgb[0] = v; rgb[1] = p; rgb[2] = q; break;
	}

	//return rgb;
}