#include <windows.h>
#include <tchar.h>
#include <time.h>

#pragma comment(lib, "winmm.lib")

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768
#define WINDOW_TITLE L"STICK FIGURE ACTION GAME"


//------------------------------------------------------------------------------------------------------------
//Forward Declaration
//------------------------------------------------------------------------------------------------------------

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL Game_Init(HWND hwnd);
VOID Game_Main(HWND hwnd);
BOOL Game_ShutDownn(HWND hwnd);
VOID DeathCheck(int currentHp, bool isHero);
VOID Message_Insert(wchar_t* str);
VOID HeroAction_Logic();
VOID MonsterAction_Logic();


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wndClass = {0};
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = (HICON)::LoadImage(NULL, L"icon.ico", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE|LR_LOADFROMFILE);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = L"StickFigureGame";

	if(!RegisterClassEx( &wndClass ))
	{
		return -1;
	}

	HWND hwnd = CreateWindow(
		L"StickFigureGame",
		WINDOW_TITLE,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL
		);

	MoveWindow(hwnd, 250, 80, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	if(!Game_Init(hwnd))
	{
		MessageBox(hwnd, L"Failed to load game resources", L"Error", 0);
		return FALSE;
	}

	//do playSound here
	return 0;
}