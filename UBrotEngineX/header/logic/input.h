///////////////////////////////////////////////////////////////////////////////////////////////////
// Filename: input.h
///////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _INPUT_H_
#define _INPUT_H_


///////////////////////////////
// PRE-PROCESSING DIRECTIVES //
///////////////////////////////
#define DIRECTINPUT_VERSION 0x0800


/////////////
// LINKING //
/////////////
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")


//////////////
// INCLUDES //
//////////////
#include <dinput.h>


///////////////////////////////////////////////////////////////////////////////////////////////////
// Class name: Input
/// TODO
///////////////////////////////////////////////////////////////////////////////////////////////////
class Input
{
public:
	Input();

	bool Initialize(HINSTANCE, HWND, int, int);
	void Shutdown();
	bool Frame();

	void GetMouseLocation(int&, int&);

	/**
	 * Whether key \p KEY_NUM is pressed (once)
	 * @param KEY_NUM Direct Input key id
	 * @return
	 */
	bool IsKeyPressed(int KEY_NUM);
	/**
	* Whether key \p KEY_NUM is pressed
	* @param KEY_NUM Direct Input key id
	* @return
	*/
	bool IsKeyDown(int KEY_NUM);

	bool IsLeftMouseButtonDown();
	bool IsRightMouseButtonDown();

	void SetKeyLock(int);
	void SetMouseLock(int);

private:
	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();

private:
	IDirectInput8 * m_directInput;
	IDirectInputDevice8* m_keyboard;
	IDirectInputDevice8* m_mouse;

	unsigned char m_keyboardState[256];
	DIMOUSESTATE m_mouseState;
	bool m_keyLock[256];
	bool m_leftMouseButtonLock;
	bool m_rightMouseButtonLock;

	int m_screenWidth, m_screenHeight;
	int m_mouseX, m_mouseY;
};

#endif