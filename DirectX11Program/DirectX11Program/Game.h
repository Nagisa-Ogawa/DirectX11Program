#pragma once
#include <Windows.h>

#define SAFE_RELEASE(p) if((p)!=nullptr){ (p)->Release(); (p)=nullptr;} 
// アプリケーション全体を表す構造体
class Game
{
public:
	// 初期化処理
	void Initialize(Game* game, const LPCWSTR windowTitle, const float width, const float height);
	// ループ本体
	int Run();
private:
	// ウィンドウのハンドル
	HWND hWnd = NULL;
	// 作成するウィンドウ名の
	LPCWSTR WindowTitle = L"タイトル";
	// 作成するウィンドウの横幅
	float ScreenWidth = 600.0f;
	// 作成するウィンドウの高さ
	float ScreenHeight = 400.0f;

	// ウィンドウを作成
	bool InitWindow();
};
