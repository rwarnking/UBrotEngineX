////////////////////////////////////////////////////////////////////////////////
// Filename: input.cpp
////////////////////////////////////////////////////////////////////////////////
#include "../../header/logic/input.h"


Input::Input()
{
	m_directInput = nullptr;
	m_keyboard = nullptr;
	m_mouse = nullptr;
	m_leftMouseButtonLock = false;
	m_rightMouseButtonLock = false;
}


bool Input::Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
{
	HRESULT result;


	// Store the screen size which will be used for positioning the mouse cursor.
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// Initialize the location of the mouse on the screen.
	m_mouseX = 0;
	m_mouseY = 0;

	// Initialize the main direct input interface.
	result = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Initialize the direct input interface for the keyboard.
	result = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Set the data format.  In this case since it is a keyboard we can use the predefined data format.
	result = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result))
	{
		return false;
	}

	// Set the cooperative level of the keyboard to not share with other programs.
	result = m_keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}

	// Now acquire the keyboard.
	result = m_keyboard->Acquire();
	if (FAILED(result))
	{
		return false;
	}

	// Initialize the direct input interface for the mouse.
	result = m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Set the data format for the mouse using the pre-defined mouse data format.
	result = m_mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(result))
	{
		return false;
	}

	// Set the cooperative level of the mouse to share with other programs.
	result = m_mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}

	// Acquire the mouse.
	result = m_mouse->Acquire();
	if (FAILED(result))
	{
		return false;
	}

	SetCursorPos(0, 0); // TODO: ?

	return true;
}


void Input::Shutdown()
{
	// Release the mouse.
	if (m_mouse)
	{
		m_mouse->Unacquire();
		m_mouse->Release();
		m_mouse = nullptr;
	}

	// Release the keyboard.
	if (m_keyboard)
	{
		m_keyboard->Unacquire();
		m_keyboard->Release();
		m_keyboard = nullptr;
	}

	// Release the main interface to direct input.
	if (m_directInput)
	{
		m_directInput->Release();
		m_directInput = nullptr;
	}

	return;
}


bool Input::Frame()
{
	bool result;


	// Read the current state of the keyboard.
	result = ReadKeyboard();
	if (!result)
	{
		return false;
	}

	// Read the current state of the mouse.
	result = ReadMouse();
	if (!result)
	{
		return false;
	}

	// Process the changes in the mouse and keyboard.
	ProcessInput();

	return true;
}


bool Input::ReadKeyboard()
{
	HRESULT result;

	// Get pressed keys and lock key
	for (int i = 0; i < 256; i++)
	{
		if ((m_keyboardState[i] & 0x80))
		{
			// Key is down so lock
			m_keyLock[i] = true;
		}
	}

	// Read the keyboard device.
	result = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
	if (FAILED(result))
	{
		// If the keyboard lost focus or was not acquired then try to get control back.
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_keyboard->Acquire();
		}
		else
		{
			return false;
		}
	}

	// Get pressed keys and release locks on key up
	for (int i = 0; i < 256; i++)
	{
		if (!(m_keyboardState[i] & 0x80))
		{
			// Key is up so release lock
			m_keyLock[i] = false;
		}
	}

	return true;
}


bool Input::ReadMouse()
{
	HRESULT result;


	if (m_mouseState.rgbButtons[0] & 0x80)
	{
		m_leftMouseButtonLock = true;
	}

	if (m_mouseState.rgbButtons[1] & 0x80)
	{
		m_rightMouseButtonLock = true;
	}

	// Read the mouse device.
	result = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
	if (FAILED(result))
	{
		// If the mouse lost focus or was not acquired then try to get control back.
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_mouse->Acquire();
		}
		else
		{
			return false;
		}
	}

	if (!(m_mouseState.rgbButtons[0] & 0x80))
	{
		m_leftMouseButtonLock = false;
	}

	if (!(m_mouseState.rgbButtons[1] & 0x80))
	{
		m_rightMouseButtonLock = false;
	}

	return true;
}


void Input::ProcessInput()
{
	// Update the location of the mouse cursor based on the change of the mouse location during the frame.
	m_mouseX += m_mouseState.lX;
	m_mouseY += m_mouseState.lY;

	// Ensure the mouse location doesn't exceed the screen width or height.
	if (m_mouseX < 0)
	{
		m_mouseX = 0;
	}
	if (m_mouseY < 0)
	{
		m_mouseY = 0;
	}

	if (m_mouseX > m_screenWidth)
	{
		m_mouseX = m_screenWidth;
	}
	if (m_mouseY > m_screenHeight)
	{
		m_mouseY = m_screenHeight;
	}

	return;
}


void Input::GetMouseLocation(int& mouseX, int& mouseY)
{
	mouseX = m_mouseX;
	mouseY = m_mouseY;
	return;
}

bool Input::IsKeyPressed(int KEY_NUM)
{
	// Do a bitwise and on the keyboard state to check if the 1 key is currently being pressed.
	if (m_keyboardState[KEY_NUM] & 0x80 && !m_keyLock[KEY_NUM])
	{
		m_keyLock[KEY_NUM] = true;
		return true;
	}

	return false;
}

bool Input::IsKeyDown(int KEY_NUM)
{
	// Do a bitwise and on the keyboard state to check if the 1 key is currently being pressed.
	if (m_keyboardState[KEY_NUM] & 0x80)
	{
		m_keyLock[KEY_NUM] = true;
		return true;
	}

	return false;
}


bool Input::IsLeftMouseButtonDown()
{
	// Check if the left mouse button is currently pressed.
	if (m_mouseState.rgbButtons[0] & 0x80 && !m_leftMouseButtonLock)
	{
		return true;
	}

	return false;
}


bool Input::IsRightMouseButtonDown()
{
	// Check if the right mouse button is currently pressed.
	if (m_mouseState.rgbButtons[1] & 0x80 && !m_rightMouseButtonLock)
	{
		return true;
	}

	return false;
}


void Input::SetKeyLock(int key)
{
	m_keyLock[key] = true;
}


void Input::SetMouseLock(int key)
{
	if (key == 0)
		m_leftMouseButtonLock = true;
	else if (key == 1)
		m_rightMouseButtonLock = true;
}