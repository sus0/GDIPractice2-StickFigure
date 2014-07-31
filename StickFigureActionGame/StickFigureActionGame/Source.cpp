#include <windows.h>
#include <tchar.h>
#include <time.h>

#pragma comment(lib, "winmm.lib")

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768
#define WINDOW_TITLE L"STICK FIGURE ACTION GAME"

//------------------------------------------------------------------------------------------------------------
//Global Variables
//------------------------------------------------------------------------------------------------------------
HDC g_hdc = NULL, g_mdc = NULL, g_bufferdc = NULL;
HBITMAP g_hSprite[2] = {NULL}, g_hBackground = NULL;
DWORD g_tPrev = 0, g_tCurr = 0;
int g_iNo = 0, g_iX = 0, g_iY = 0;
int g_iDirection = 0;

//------------------------------------------------------------------------------------------------------------
//Forward Declaration
//------------------------------------------------------------------------------------------------------------

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL Game_Init(HWND hwnd);
VOID Game_Paint(HWND hwnd);
BOOL Game_ReleaseResources(HWND hwnd);


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
	MSG msg = {0};
	
	while(msg.message !=WM_QUIT)
	{
		if(PeekMessage( &msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			g_tCurr = GetTickCount();
			if(g_tCurr - g_tPrev >= 50)
			{
				Game_Paint(hwnd);
			}
		}
	
	}
	
	UnregisterClass(L"StickFigureGame", wndClass.hInstance);
	
	return 0;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hwnd);
			PostQuitMessage(0);
			break;

		case VK_LEFT:
			g_iX -= 10;
			g_iDirection = 0;
			if(g_iX < 0)
			{
				g_iX = 0;
			}
			break;


		case VK_RIGHT:
			g_iX +=10;
			g_iDirection = 1;
			if (g_iX > WINDOW_WIDTH - 75)
			{
				g_iX = WINDOW_WIDTH -75;
			}
			break;

		}
		break;

	case WM_DESTROY:
		Game_ReleaseResources(hwnd);
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	return 0;
}


BOOL Game_Init(HWND hwnd)
{
	HBITMAP blankBmp;
	
	g_hdc = GetDC(hwnd);
	g_mdc = CreateCompatibleDC(g_hdc);
	g_bufferdc = CreateCompatibleDC(g_hdc);
	blankBmp = CreateCompatibleBitmap(g_hdc, WINDOW_WIDTH, WINDOW_HEIGHT);

	g_iX = 0;
	g_iY = 600;
	g_iDirection = 3;
	g_iNo = 0;


	SelectObject(g_mdc, blankBmp);

	g_hSprite[0] = (HBITMAP)LoadImage(NULL,L"Media\\action_l.bmp", IMAGE_BITMAP, 1000, 400, LR_LOADFROMFILE);
	g_hSprite[1] = (HBITMAP)LoadImage(NULL,L"Media\\action_r.bmp", IMAGE_BITMAP, 1000, 400, LR_LOADFROMFILE);
	g_hBackground = (HBITMAP)LoadImage(NULL, L"Media\\background.bmp", IMAGE_BITMAP, WINDOW_WIDTH, WINDOW_HEIGHT, LR_LOADFROMFILE);

	Game_Paint(hwnd);
	return TRUE;
}

VOID Game_Paint(HWND hwnd)
{
	SelectObject(g_bufferdc, g_hBackground);
	BitBlt(g_mdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, g_bufferdc, 0, 0, SRCCOPY);

	SelectObject(g_bufferdc, g_hSprite[g_iDirection]);
	BitBlt(g_mdc, g_iX, g_iY, 100, 200, g_bufferdc, g_iNo*250+50, 200, SRCAND);
	BitBlt(g_mdc, g_iX, g_iY, 100, 200, g_bufferdc, g_iNo*250+50, 0, SRCPAINT);

	BitBlt(g_hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, g_mdc, 0, 0, SRCCOPY);

	g_tPrev = GetTickCount();
	g_iNo ++;

	if(g_iNo == 4)
	{
		g_iNo = 0;
	}

}

BOOL Game_ReleaseResources(HWND hwnd)
{
	DeleteObject(g_hBackground);
	
	for (int i = 0; i < 2; i++)
	{
		DeleteObject(g_hSprite[i]);
	}

	DeleteDC(g_bufferdc);
	DeleteDC(g_mdc);
	ReleaseDC(hwnd, g_hdc);
	
	return TRUE;
}



