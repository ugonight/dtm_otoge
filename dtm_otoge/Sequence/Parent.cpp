#include "Parent.h"
#include "Title.h"
#include "../fps.h"
#include "../Mouse.h"
#include "../define.h"

Parent* Parent::mInstance = 0;

void Parent::create() {
	mInstance = new Parent();
}

void Parent::destroy() {
	delete mInstance;
	mInstance = 0;
}

Parent* Parent::instance() {
	return mInstance;
}

Parent::Parent() :
	mChild(0)
{

	mChild = new Title();	//最初はタイトル

	mFps = new fps();
	mMouse = new Mouse();
}

void Parent::update() {
	Base* nextChild = mChild->update(this);	//Childクラスを継承しているクラスのupdateを実行
	
	//シーケンス遷移
	if (nextChild != mChild) {
		Child* casted = dynamic_cast< Child* >(nextChild);
		delete mChild; mChild = 0;
		mChild = casted;

		delete mMouse; mMouse = 0;
		mMouse = new Mouse();
	}
	nextChild = 0;

	//fps制御は常時行う
	mFps->Wait();
	mFps->Update();
	mFps->Draw();
	//マウスカーソルも常時表示
	mMouse->update();
	mMouse->draw();
}

Parent::~Parent()
{
	delete mChild;
	mChild = 0;

	InitGraph();
	InitSoftImage();
	InitSoundMem();
	InitFontToHandle();
}

void Parent::MouseCurrent() { mMouse->SetCurrent(); }
bool Parent::Click() { return mMouse->GetClick(); }