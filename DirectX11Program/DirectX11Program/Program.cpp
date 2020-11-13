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
	MSG msg = {};
	// メッセージループ
	while (true) {
		// メッセージがあるなら
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
			// メッセージを受け取る
			if (!GetMessage(&msg, NULL, 0, 0)) {
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