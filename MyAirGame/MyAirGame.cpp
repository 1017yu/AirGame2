#include <windows.h>
#include "resource.h"
#include <iostream>

// GLOBAL VARIABLE

HBITMAP hBmpAIR, hBmpBACK, hBmpBULLET, hBmpENEMY, hBmpEXPLOSION;
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void game_init(HWND hWnd, bool all_init);

#define max_no_of_bullet 10
int bn = 0;
int n;
static int nX = 10, nY = 100;
static int nX_back = 0;
static int nXb[max_no_of_bullet], nYb[max_no_of_bullet];  //bullet
static int nXe, nYe;  // Enemy
static int nAx, nAy;

static bool bullet[max_no_of_bullet] = { 0,0,0,0,0,0,0,0,0,0 };
static bool hit[max_no_of_bullet] = { 0,0,0,0,0,0,0,0,0,0 };
static bool enemy = false;

static HWND hButtStart, hButtExit, hButtStop;

int no_of_fire = 0;
int no_of_hit = 0;
char szText[100];
bool game_start = false;
bool game_multi = false;

HPEN hPen;
/*
static int nX1 = 600, nY1 = 0; // position of mouse 1
static int nX2 = 620, nY2 = 20; // position of mouse 2
static int nX3 = 560, nY3 = 0; // position of mouse 1
static int nX4 = 580, nY4 = 20; // position of mouse 2 */

// Win Main

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstanc, LPSTR lpCmdLine, int nShowCmd)

{
	HWND hWnd;
	MSG mSg;

	char szTitle[] = "My First Airplane Game";
	char szClass[] = "Class";
	WNDCLASSEX WndEx;

	WndEx.cbSize = sizeof(WndEx); //size of window class structure
	WndEx.style = NULL; // window style
	WndEx.lpfnWndProc = WndProc; //window procedure(function) pointer
	WndEx.cbClsExtra = 0; //additional memory size for widow class
	WndEx.cbWndExtra = 0; // additional memory size for window
	WndEx.hInstance = hInstance; // intance handle of the window procdeure
	WndEx.hIcon = LoadIcon(NULL, ""); //big icon
	WndEx.hIconSm = LoadIcon(hInstance, ""); //small icon
	WndEx.hCursor = LoadCursor(NULL, IDC_ARROW);//cursor
	WndEx.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);//background
	WndEx.lpszMenuName = "IDR_MENU";//menui
	WndEx.lpszClassName = szClass;//window class name
	WndEx.hIconSm = LoadIcon(hInstance, "");
	
	// 2. Register window calss -----------------------------------------------------

	RegisterClassEx(&WndEx);

	// 3. Create window ------------------------------------------------------------

	hWnd = CreateWindowEx(NULL, szClass, szTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 
		CW_USEDEFAULT,
		320 * 2, 240 * 2, NULL, NULL, hInstance, NULL);

	// 4. Show and Update window-----------------------------------------------------

	ShowWindow(hWnd, nShowCmd);
	
	hBmpAIR = LoadBitmap(hInstance, "IDB_AIR");
	hBmpBACK = LoadBitmap(hInstance, "IDB_BACK");
	hBmpBULLET = LoadBitmap(hInstance, "IDB_BULLET");
	hBmpENEMY = LoadBitmap(hInstance, "IDB_ENEMY");
	hBmpEXPLOSION = LoadBitmap(hInstance, "IDB_EXPLOSION");

	UpdateWindow(hWnd);
	InvalidateRect(hWnd, NULL, TRUE);
	

	// 5. Message loop --------------------------------------------------------------

	while (TRUE)
	{
		if (PeekMessage(&mSg, NULL, 0, 0, PM_NOREMOVE))
		{
			if (!GetMessage(&mSg, NULL, 0, 0))
				break;
			TranslateMessage(&mSg);
			DispatchMessage(&mSg);
		}
	}
	return mSg.wParam;
}

// game_init Procedure

void game_init(HWND hWnd, bool all_init)
{
	for (n = 0; n < max_no_of_bullet; n++) {
		KillTimer(hWnd, n + 1);
		bullet[n] = false;
		hit[n] = false;
	}
	bn = 0;
	no_of_fire = 0; no_of_hit = 0;
	if (all_init) {
		KillTimer(hWnd, max_no_of_bullet + 2);
		game_start = false;
		enemy = false;
	}
}

//HINSTANCE hInst;
//Call-back Procedure
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hDC, hMemDC;
	PAINTSTRUCT pS;
	// Menu Command, Message, Handling
	switch (uMsg)
	{
		case WM_CREATE:
			hButtStart = CreateWindow("BUTTON", "Game Start", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 195, 390, 90, 30, hWnd, (HMENU)1, NULL, NULL);
			hButtStop = CreateWindow("BUTTON", "Stop", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 290, 390, 90, 30, hWnd, (HMENU)2, NULL, NULL);
			hButtExit = CreateWindow("BUTTON", "Exit", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 390, 390, 90, 30, hWnd, (HMENU)3, NULL, NULL);
			return FALSE;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
			case 1:
				game_init(hWnd, true);
				SetTimer(hWnd, max_no_of_bullet + 2, 2000, NULL);
				enemy = true;
				game_start = true;
				break;

			case 2:
				game_init(hWnd, true);
				break;

			case 3:
				PostQuitMessage(0);
				break;
			}
			SetFocus(hWnd);

			case WM_LBUTTONDOWN:
				nAx = LOWORD(lParam);
				nAy = HIWORD(lParam);

				nX = nAx - 63;
				nY = nAy - 19;

			InvalidateRect(hWnd, NULL, TRUE);
			UpdateWindow(hWnd);
			return FALSE;

		case WM_RBUTTONDOWN:
			return FALSE;

		switch (LOWORD(wParam)) {


		case ID_CONTROL_START:
			game_init(hWnd, true);
			SetTimer(hWnd, max_no_of_bullet + 2, 2000, NULL);
			enemy = true;
			game_start = true;
			break; 

		case ID_CONTROL_STOP:
			game_init(hWnd, true);
			break;

		case ID_CONTROL_EXIT:
			PostQuitMessage(0);
			break;

		case ID_BULLET_SINGLE:
			game_init(hWnd, false);
			game_multi = false;
			break;

		case ID_BULLET_MULTI:
			game_init(hWnd, false);
			game_multi = true;
			break;
		}

		InvalidateRect(hWnd, NULL, TRUE);
		UpdateWindow(hWnd);
		return FALSE;

		case WM_KEYDOWN:
			switch (LOWORD(wParam))
			{
				case VK_UP:
					nY = nY - 5;
					break;
				case VK_DOWN:
					nY = nY + 5;
					break;
				case VK_LEFT:
					nX = nX - 5;
					nX_back = nX_back + 3;
					break;
				case VK_RIGHT:
					nX = nX + 5;
					nX_back = nX_back - 3;
					break;
				case VK_RETURN:  // GAME START
					game_init(hWnd, true);
					SetTimer(hWnd, max_no_of_bullet + 2, 2000, NULL);
					enemy = true;
					break;

				case VK_SPACE:  // Fire bullet 
					if (game_start) {
						no_of_fire++;

						nXb[bn] = nX + 127;
						nYb[bn] = nY;

						if (!bullet[bn]) {
							SetTimer(hWnd, bn + 1, 5, NULL);
							bullet[bn] = true;
						}
						if (game_multi) bn++;
						if (bn > max_no_of_bullet)
							bn = 0;
					}
					break;
			}
			InvalidateRect(hWnd, NULL, TRUE);
			UpdateWindow(hWnd);
			return FALSE;

		case WM_TIMER:
			hDC = GetDC(hWnd);
			hMemDC = CreateCompatibleDC(hDC);

			if (wParam == max_no_of_bullet + 2)  // Timer for enemy
			{ 
				nXe = (rand() % 300) + 100;
				nYe = (rand() % 250);
			}

			for (int n = 0; n < max_no_of_bullet; n++) {
				if (wParam == n + 1) {  // Timer for bullets
					nXb[n] += 5;
					if (nXb[n] > 640) 
					{
						KillTimer(hWnd, n + 1);
						bullet[n] = false;
					}
				}

				if (bullet[n]) {
					if (nXb[n] >= nXe - 15 && nXb[n] <= nXe + 15 && nYb[n] >= nYe - 15 && nYb[n] <= nYe + 15)
					{
						bullet[n] = false;
						//enemy = false;
						KillTimer(hWnd, n + 1);
						//KillTimer(hWnd, 2);
						hit[n] = true;
						no_of_hit++;
					}
				}
					/*
					hDC = BeginPaint(hWnd, &pS);
					hMemDC = CreateCompatibleDC(hDC);
					SelectObject(hMemDC, hBmpEXPLOSION);
					BitBlt(hDC, nXe, nYe, 32, 28, hMemDC, 0, 0, SRCCOPY);
					MessageBox(hWnd, "Enemy Killed", "Result", NULL);
					*/
			}
			DeleteDC(hMemDC);
			ReleaseDC(hWnd, hDC);
			DeleteObject(hMemDC);

			InvalidateRect(hWnd, NULL, TRUE);
			UpdateWindow(hWnd);
			return FALSE;

		case WM_PAINT:
			hDC = BeginPaint(hWnd, &pS);
			hMemDC = CreateCompatibleDC(hDC);

			if (nX > 640) nX = 0;
			else if (nX < 0) nX = 640;

			if (nY > 480) nY = 0;
			else if (nY < 0) nY = 480;

			if (nX_back > 640) nX_back = 0;
			else if (nX_back < 0) nX_back = 640;

			if (enemy) {
				SelectObject(hMemDC, hBmpENEMY);
				BitBlt(hDC, nXe, nYe, 32, 32, hMemDC, 0, 0, SRCCOPY);
			}
			for (int n = 0; n < max_no_of_bullet; n++) {
				if (bullet[n]) {
					SelectObject(hMemDC, hBmpBULLET);
					BitBlt(hDC, nXb[n], nYb[n], 32, 30, hMemDC, 0, 0, SRCCOPY);
				}

				if (hit[n]) {
					SelectObject(hMemDC, hBmpEXPLOSION);
					BitBlt(hDC, nXe, nYe, 32, 28, hMemDC, 0, 0, SRCCOPY);
					hit[n] = 0;
					Sleep(100);
				}

			}
			SelectObject(hMemDC, hBmpAIR);
			BitBlt(hDC, nX, nY, 127, 37, hMemDC, 0, 0, SRCCOPY);
			SelectObject(hMemDC, hBmpBACK);
			BitBlt(hDC, nX_back, 281, 640, 158, hMemDC, 0, 0, SRCCOPY);
			BitBlt(hDC, nX_back - 640, 281, 640, 158, hMemDC, 0, 0, SRCCOPY);

			if (game_start) { // 출력 메시지
				sprintf_s(szText, "No of fire : %d, No of hit: %d ", no_of_fire, no_of_hit);
				TextOut(hDC, 300, 10, szText, lstrlen(szText));
			}
			DeleteDC(hMemDC);
			ReleaseDC(hWnd, hDC);
			DeleteObject(hMemDC);
			EndPaint(hWnd, &pS);
			return FALSE;

		case WM_DESTROY:
			PostQuitMessage(0);
			return FALSE;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}