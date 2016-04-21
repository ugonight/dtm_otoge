#pragma once

class Child;
class fps;
class Mouse;

class Parent
{
public:
	void update();

	static void create();
	static void destroy();
	static Parent* instance();

	void MouseCurrent();
	bool Click();
private:
	Parent();
	~Parent();

	Child* mChild;
	fps* mFps;
	Mouse* mMouse;

	static Parent* mInstance;
};

