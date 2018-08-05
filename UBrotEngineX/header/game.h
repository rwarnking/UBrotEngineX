///////////////////////////////////////////////////////////////////////////////////////////////////
// Filename: game.h
///////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _GAME_H_
#define _GAME_H_


///////////////////////////////
// PRE-PROCESSING DIRECTIVES //
///////////////////////////////
#define WIN32_LEAN_AND_MEAN


//////////////
// INCLUDES //
//////////////
#include <windows.h>
#include <vector>
#include <memory> // TODO wo soll das jetzt hin wenn man das ueberall brauch ?


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "graphics/renderer.h"
#include "timer.h"


/////////////
// GLOBALS //
/////////////
extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;


///////////////////////////////////////////////////////////////////////////////////////////////////
// Class name: Game
/// The game class manages the game loop, renderer and scenes. It also initializes the
/// application's window and stores a handle to it.
///////////////////////////////////////////////////////////////////////////////////////////////////
class Game
{
public:
	/**
	* A constructor where all the member pointers are initialised to null.
	*/
	Game();
	/**
	* Empty copy contructor
	* @param other to copy
	*/
	Game(const Game& other);
	~Game();

	/**
	 * Calls \c InitializeWindows and initialized all important objects (e.g. renderer, scenes)
	 */
	bool Initialize();
	/**
	 * Shuts down by calling shut down for every member (where possible) and freeing allocated
	 * memory. Also calls \c ShutdownWindows to close the window.
	 */
	void Shutdown();
	/**
	 * Acts as the game loop. Here, window messages are processed and frames are rendered.
	 * This loop runs as long as the user does not close the application.
	 */
	void Run();

private:
	/**
	 * Calls all necessary functions (matrix stuff) and updates (includes rendering).
	 */
	bool Frame();
	/**
	 * Constructs the window to which the application is rendered. Determines the currently used
	 * screen resolution and stores them in \p screenWidth and \p screenHeight. The constructed
	 * window is borderless and (depending on the settings) is either set to fullscreen or a
	 * 800x600 window.
     * @param[out] screenWidth
	 * @param[out] screenHeight
	 */
	void InitializeWindows(int& screenWidth, int& screenHeight);
	/**
	 * Resets the screen settings and releases both the window and its handle.
	 */
	void ShutdownWindows();

private:
	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;

	std::unique_ptr<Renderer> m_renderer;
	std::unique_ptr<Scene> m_scene;
	std::unique_ptr<Timer> m_timer;
};


/////////////////////////
// FUNCTION PROTOTYPES //
/////////////////////////
/**
 * This function is called by Windows for window messages, the name if which was passed
 * to the settings in \c InitializeWindows
 * @param hwnd
 * @param umessage
 * @param wparam
 * @param lparam
 */
static LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);


/////////////
// GLOBALS //
/////////////
static Game* ApplicationHandle = 0;

#endif