//=================================================
//Game.cpp
//
//=================================================
#include "Game.h"

// 作成するウィンドウ名の
const WCHAR windowTitle[] = L"GameWindow";
// 作成するウィンドウの横幅
const float Width = 600.0f;
// 作成するウィンドウの高さ
const float Height = 400.0f;


// ウィンドウプロシージャ(メッセージを処理する関数)
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg)
	{
	case WM_CLOSE:
		if (MessageBox(hWnd, L"ウィンドウを閉じますか？", L"警告", MB_YESNO) == IDYES) {
			// ウィンドウを閉じる
			DestroyWindow(hWnd);
		}
		return 0;
	case WM_DESTROY:
		// アプリケーションを終了
		PostQuitMessage(0);
		return 0;
	}

	// 処理しきれなかったメッセージはデフォルト関数に委託
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

/// <summary>
/// ウィンドウの初期化をする関数
/// </summary>
/// <param name="CLASSNAME">ウィンドウ名</param>
/// <param name="Width">ウィンドウの横幅</param>
/// <param name="Height">ウィンドウの縦幅</param>
/// <returns>作成の</returns>
bool InitWindow(const WCHAR CLASSNAME[], const float Width, const float Height) {
	// ウィンドウクラスを作成
	WNDCLASSEX wndClass = {};
	wndClass.cbSize = sizeof(WNDCLASSEX);	// ウィンドウクラスのサイズ
	wndClass.style = CS_HREDRAW;			// ウィンドウクラスの基本スタイル
	wndClass.lpfnWndProc = WndProc;			// ウィンドウプロシージャのポインタ
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hinstance;		// HINSTANCE
	wndClass.hIcon = NULL;
	wndClass.hCursor = NULL;
	wndClass.hbrBackground = (HBRUSH)COLOR_BACKGROUND;	// 背景
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = CLASSNAME;
	wndClass.hIconSm = NULL;
	// 作成に失敗したならメッセージボックスを出して終了
	if (!RegisterClassEx(&wndClass)) {
		MessageBox(NULL, L"ウィンドウクラスの作成に失敗しました。", L"error", MB_OK);
		return -1;
	}

	// クライアント領域のサイズ
	RECT rect = { 0,0,600,400 };
	// クライアント領域のサイズからウィンドウのサイズを計算
	AdjustWindowRectEx(&rect, WS_CAPTION, NULL, WS_CAPTION);
	// ウィンドウを作成
	HWND hWnd = CreateWindowEx(
		0,	// 拡張ウィンドウのスタイル
		L"GameWindow",				// 作成したウィンドウクラス
		L"Window",						// ウィンドウ名
		WS_OVERLAPPEDWINDOW,			// ウィンドウのスタイル
		CW_USEDEFAULT,					// ウィンドウのx座標
		CW_USEDEFAULT,					// ウィンドウのy座標
		(rect.right - rect.left),		// ウィンドウの横幅
		(rect.bottom - rect.top),		// ウィンドウの縦幅
		NULL,				// ウィンドウの親
		NULL,				// ウィンドウメニュー
		hinstance,			// HINSTANCE
		NULL				// 追加のデータを渡す構造体のポインター
	);
	// 作成に失敗したらメッセージボックスを出して終了
	if (hWnd == NULL) {
		MessageBox(NULL, L"ウィンドウの作成に失敗しました。", L"error", MB_OK);
		return -1;
	}
	// ウィンドウの表示
	ShowWindow(hWnd, nShowCmd);
	UpdateWindow(hWnd);

}

/// <summary>
/// ループ本体
/// </summary>
void Run() {

}


