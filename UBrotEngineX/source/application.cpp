///////////////////////////////////////////////////////////////////////////////////////////////////
// Filename: application.cpp
///////////////////////////////////////////////////////////////////////////////////////////////////
#include "../header/game.h"


#define _CRTDBG_MAP_ALLOC #include <stdlib.h> #include <crtdbg.h>
/*
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
*/
#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif


int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ PSTR pScmdline,
	_In_ int iCmdshow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetBreakAlloc(1282);

	auto game = std::make_unique<ubrot::Game>();
	bool result;


	// Erzeuge das game Objekt in dem der game-loop zu finden ist
	//game = new Game;
	if (!game)
	{
		return 0;
	}

	// Starte den game-loop
	result = game->Initialize();
	if (result)
	{
		game->Run();
	}

	// Lösche den Pointer zum Game Objekt
	game->Shutdown();
	game = nullptr;

	_CrtDumpMemoryLeaks();

	return 0;
}