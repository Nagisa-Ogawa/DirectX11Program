//=================================================
//Program.cpp
//
//=================================================

#include <Windows.h>

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

// エントリーポイント
int WINAPI wWinMain(
	_In_ HINSTANCE hinstance,		// インスタンスハンドル
	_In_opt_ HINSTANCE hPrevInstance,	// 常にNULL
	_In_ LPWSTR     lpCmdLine,		// コマンドラインから受け取った引数
	_In_ int		  nShowCmd			// 表示状態
)
{
	// 作成するウィンドウクラスの
	const WCHAR CLASSNAME[] = L"GameWindow";
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
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);
	MSG msg = {};
	// メッセージループ
	while (true) {
		// メッセージがあるなら
		if (PeekMessage(&msg, hWnd, 0, 0, PM_NOREMOVE)) {
			// メッセージを受け取る
			if (!GetMessage(&msg, hWnd, 0, 0)) {
				break;
			}
			// 文字メッセージに変換
			TranslateMessage(&msg);
			// メッセージをウィンドウプロシージャに送る
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}