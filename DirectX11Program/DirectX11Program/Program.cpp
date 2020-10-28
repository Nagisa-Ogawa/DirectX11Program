//=================================================
//Program.cpp
//
//=================================================

#include <Windows.h>

// エントリーポイント
int WINAPI wWinMain(
	_In_ HINSTANCE hinstance,		// インスタンスハンドル
	_In_opt_ HINSTANCE hPrevInstance,	// 常にNULL
	_In_ LPWSTR     lpCmdLine,		// コマンドラインから受け取った引数
	_In_ int		  nShowCmd			// 表示状態
)
{
	// クライアント領域のサイズ
	RECT rect = { 0,0,600,300 };
	// クライアント領域のサイズからウィンドウのサイズを計算
	AdjustWindowRectEx(
		&rect,
		WS_CAPTION,
		NULL,
		WS_CAPTION
		);
	// ウィンドウを作成
	HWND hWnd = CreateWindowEx(
		0,	// 拡張ウィンドウのスタイル
		L"Static",						// システムで事前定義されているウィンドウクラス
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
	// 作成に失敗したら終了
	if (hWnd == NULL) {
		return -1;
	}
	// ウィンドウの表示
	ShowWindow(hWnd, SW_SHOW);
	// メッセージボックス
	MessageBoxW(hWnd, L"成功\n", L"作戦結果\n", MB_OK);
	return 0;
}