//=================================================
//Program.cpp
//
//=================================================

#include "Game.h"
#include <crtdbg.h>

// エントリーポイント
int WINAPI wWinMain(
	_In_ HINSTANCE hinstance,		// インスタンスハンドル
	_In_opt_ HINSTANCE hPrevInstance,	// 常にNULL
	_In_ LPWSTR     lpCmdLine,		// コマンドラインから受け取った引数
	_In_ int		  nShowCmd			// 表示状態
)
{
	// メモリリークを検出
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Game game;
	// アプリケーションの起動設定
	game.Initialize(&game,L"Game", 600.0f, 400.0f);
	return	game.Run();
}