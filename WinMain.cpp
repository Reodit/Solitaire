#include <Windows.h>
#include <gdiplus.h>
#include "GameLogic.h"

#pragma comment (lib, "Gdiplus.lib")

using namespace Gdiplus;

const wchar_t gClassName[]{ L"MyWindowClass" };

solitaire::GameLogic gLogic;

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd
)
{
	Gdiplus::GdiplusStartupInput gsi;
	ULONG_PTR gdiToken;

	Gdiplus::GdiplusStartup(&gdiToken, &gsi, nullptr);



	WNDCLASSEX wc{};

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpszClassName = gClassName;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpfnWndProc = WindowProc;
	wc.cbSize = sizeof(WNDCLASSEX);

	if (!RegisterClassEx(&wc))
	{
		MessageBox(nullptr, L"Failed To Register Window Class!", L"Error", MB_OK);
		return 0;
	}

	HWND hwnd;

	RECT windowRect{ 0, 0, 1024, 768 };
	AdjustWindowRect(&windowRect, WS_POPUPWINDOW | WS_CAPTION, false);

	hwnd = CreateWindowEx(
		0,
		gClassName,
		L"Solitaire",
		WS_POPUPWINDOW | WS_CAPTION,
		0, 0,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		nullptr,
		nullptr,
		hInstance,
		nullptr
	);

	if (hwnd == nullptr)
	{
		MessageBox(nullptr, L"Failed To Create Window!", L"Error", MB_OK);
		return 0;
	}

	gLogic.Initialize(hwnd);
	ShowWindow(hwnd, nShowCmd);
	UpdateWindow(hwnd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	gLogic.Release();

	Gdiplus::GdiplusShutdown(gdiToken);
	return (int)msg.wParam;
}

void OnPaint(HWND hwnd)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hwnd, &ps);

	Gdiplus::Graphics graphics(hdc);
	gLogic.Draw(graphics);

	EndPaint(hwnd, &ps);
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
		OnPaint(hWnd);
		break;

	case WM_LBUTTONUP:
		gLogic.OnClick(LOWORD(lParam), HIWORD(lParam));
		break;

	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
