#include "Sequence/Parent.h"
#include "define.h"

bool windowMode, exitApplication;
int DEF_LEVEL = 0, DEF_SONG = 0, DEF_SPEED = 1;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	// ��ʃ��[�h�̐ݒ�
	SetGraphMode(WINDOW_W, WINDOW_H, 16);
	SetWindowSizeExtendRate(1.5);
	SetBackgroundColor(0, 0, 0);
	SetFontSize(15);
	SetFullScreenResolutionMode(DX_FSRESOLUTIONMODE_DESKTOP);
	SetFullScreenScalingMode(DX_FSSCALINGMODE_NEAREST);
	SetAlwaysRunFlag(TRUE);

	if (MessageBox(NULL, "�t���X�N���[���ŋN�����܂����H", "�N���I�v�V����", MB_YESNO) == IDYES) {
			ChangeWindowMode(0);
			windowMode = 0;
		}
	else {
		ChangeWindowMode(1);
		windowMode = 1;
	}
	exitApplication = 0;
		
	SetMainWindowText("now loading...");
	DxLib_Init();
	PROGRESS_FRAME
	SetDrawScreen(DX_SCREEN_BACK);

	//�C���X�^���X�̐��Y
	if (!Parent::instance()) {
		
		Parent::create();
	}

	SetMainWindowText("Slip Beats");

	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0 && ((CheckHitKey(KEY_INPUT_LALT) == 0 && CheckHitKey(KEY_INPUT_RALT) == 0) || CheckHitKey(KEY_INPUT_F4) == 0) && !exitApplication) {
		Parent::instance()->update();
	}


	DxLib_End();
	return 0;
}