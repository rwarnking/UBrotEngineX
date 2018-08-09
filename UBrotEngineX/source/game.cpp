///////////////////////////////////////////////////////////////////////////////////////////////////
// Filename: game.cpp
///////////////////////////////////////////////////////////////////////////////////////////////////
#include "../header/game.h"
#include <chrono> // TODO delete
#include <array> // TODO delete
#include <string> // TODO delete

using namespace std;

namespace ubrot
{

Game::Game()
{
	m_renderer = nullptr;
	m_scene = nullptr;
	m_timer = nullptr;
}


Game::Game(const Game& other)
{
}


Game::~Game()
{
}


bool Game::Initialize()
{
	int screenWidth, screenHeight;
	bool result;


	screenWidth = 0;
	screenHeight = 0;

	auto start = std::chrono::system_clock::now();

	// Prepare the application's window and store the width and height of the window
	InitializeWindows(screenWidth, screenHeight);
#

	// Create and initialize a renderer object used to render scenes
	m_renderer = std::make_unique<graphics::Renderer>();//new Renderer();
	if (!m_renderer)
	{
		return false;
	}

	result = m_renderer->Initialize(screenWidth, screenHeight, m_hwnd);
	if (!result)
	{
		MessageBox(m_hwnd, L"Could not initialize the Renderer object.", L"Error", MB_OK);
		return false;
	}

	// Create and initialize a scence object that represents one scene (models, lights, etc.)
	m_scene = std::make_unique<Scene>();//new Scene();
	if (!m_scene)
	{
		return false;
	}

	result = m_scene->Initialize(m_renderer->GetD3D().GetDevice());
	if (!result)
	{
		MessageBox(m_hwnd, L"Could not initialize the Scene object.", L"Error", MB_OK);
		return false;
	}

	// Create and initialize a timer object to calculate FPS
	m_timer = std::make_unique<Timer>();
	if (!m_timer)
	{
		return false;
	}

	result = m_timer->Initialize();
	if (!result)
	{
		MessageBox(m_hwnd, L"Could not initialize the Timer object.", L"Error", MB_OK);
		return false;
	}

	auto end = chrono::system_clock::now();
	auto sum = chrono::duration_cast<chrono::milliseconds>(end - start).count();

	MessageBox(m_hwnd, std::to_wstring(sum).c_str(), L"Initialization time in ms", MB_OK);

	return true;
}


void Game::Shutdown()
{
	if (m_renderer)
	{
		m_renderer->Shutdown();
	}

	ShutdownWindows();


	return;
}


void Game::Run()
{
	MSG msg;
	bool done, result;


	ZeroMemory(&msg, sizeof(MSG));
	int count = 0; // TODO: delete
	std::array<long long, 10000> looptime;

	// Game loop that manages the rendering of the current scene
	done = false;
	while (!done)
	{
		auto start = std::chrono::system_clock::now();

		// Handle (WINDOWS) window messages
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// Exit the loop if the window was closed or some other exit condition is met
		if (msg.message == WM_QUIT || count == 9999)
		{
			done = true;
		}
		else
		{
			result = Frame();
			if (!result)
			{
				done = true;
			}
		}

		auto end = chrono::system_clock::now();
		looptime[count] = chrono::duration_cast<chrono::microseconds>(end - start).count();
		count++;
	}

	auto sum = 0LL;
	for (auto x : looptime)
	{
		sum += x;
	}
	sum = sum / looptime.size();

	MessageBox(m_hwnd, std::to_wstring(sum).c_str(), L"Error", MB_OK);

	return;
}


bool Game::Frame()
{
	bool result;


	result = true;
	// Update all utility functions
	m_timer->Frame();
	float x = m_timer->GetTime();

	// Render the scene
	m_renderer->Process(*m_scene);

	// Execute the game logic

	return result;
}


void Game::InitializeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;


	// Get an external pointer to this object.
	ApplicationHandle = this;

	// Create a reference to this instance
	m_hinstance = GetModuleHandle(NULL);
	m_applicationName = L"UBrotEngineX"; // TODO: rename according to game title

	// Define default values for the window settings
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	RegisterClassEx(&wc);

	// Determine the current monitor resolution
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Determine the size of the window. In fullscreen mode, the maximum size is chosen and
	// the winow is placed in the upper left corner of the screen. Otherwise, a predefined
	// size is chosen and the window is centred on the screen.
	if (FULL_SCREEN)
	{
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		posX = posY = 0;

		SCREEN_WIDTH = screenWidth;
		SCREEN_HEIGHT = screenHeight;
	}
	else
	{
		screenWidth = SCREEN_WIDTH;
		screenHeight = SCREEN_HEIGHT;

		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}


	// Create the window and store a handle to it
	m_hwnd = CreateWindowEx(
		WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL
	);

	// Bring the window up on the screen and set it as the main focus.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	ShowCursor(false);

	return;
}


void Game::ShutdownWindows()
{
	ShowCursor(true);

	// Change the display settings if leaving full screen mode
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Release all components of the window
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	ApplicationHandle = NULL;

	return;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		// Check if the window is being destroyed.
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}

	// Check if the window is being closed.
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}

	// Pass all other messages to the message handler.
	default:
	{
		return DefWindowProc(hwnd, umessage, wparam, lparam);
	}
	}
}

}
