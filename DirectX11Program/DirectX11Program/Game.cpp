//=================================================
//Game.cpp
//
//=================================================
#include "Game.h"
#include <d3d11.h>
#include <wrl/client.h>

using namespace Microsoft::WRL;

// プロトタイプ宣言
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// 初期化処理
void Game::Initialize(Game* game, const LPCWSTR windowTitle, const float width, const float height) {
	game->WindowTitle = windowTitle;
	game->ScreenWidth = width;
	game->ScreenHeight = height;
}

// ウィンドウの初期化をする関数
bool Game::InitWindow() {
	HINSTANCE hinstance = GetModuleHandle(NULL);
	// クラス名
	const wchar_t CLASS_NAME[] = L"GameWindow";
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
	wndClass.lpszClassName = CLASS_NAME;
	wndClass.hIconSm = NULL;
	// 作成に失敗したならメッセージボックスを出して終了
	if (!RegisterClassEx(&wndClass)) {
		MessageBox(NULL, L"ウィンドウクラスの作成に失敗しました。", L"error", MB_OK);
		return false;
	}

	// クライアント領域のサイズ
	RECT rect = { 0,0,(LONG)ScreenWidth,(LONG)ScreenHeight };
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
		return false;
	}
	// ウィンドウの表示
	ShowWindow(hWnd, SW_SHOWNORMAL);
	UpdateWindow(hWnd);
	this->hWnd = hWnd;

	return true;
}
// ループ本体
int Game::Run() {
	if (!InitWindow()) {
		MessageBox(NULL, L"ウィンドウの初期化に失敗しました。", L"error", MB_OK);
		return -1;
	}

	// デバイス
	ComPtr<ID3D11Device> graphicsDevice = nullptr;
	// デバイスコンテキスト
	ComPtr<ID3D11DeviceContext> immediateContext = nullptr;
	// スワップチェーン
	ComPtr<IDXGISwapChain> swapChain = nullptr;
	// 使用するfeatureLevelの配列
	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
	};
	// 実際に使用するfeatureLevel
	D3D_FEATURE_LEVEL getFeatureLevel;
	// デバイス作成時のオプションフラグ
	UINT creationFlag = 0;
	if (_DEBUG) {
		// デバッグ時にのみデバック用のオプションを追加する
		creationFlag |= D3D11_CREATE_DEVICE_DEBUG;
	}
	HRESULT hr;
	// スワップチェーンについて設定する構造体
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferDesc.Width = (UINT)ScreenWidth;	// バッファの横幅
	swapChainDesc.BufferDesc.Height = (UINT)ScreenHeight;	// バッファの高さ
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;	// フォーマット
	swapChainDesc.BufferDesc.RefreshRate = { 60,1 };	// リフレッシュレート(1/60)
	swapChainDesc.SampleDesc = { 1,0 };	/// マルチサンプリング（未使用）
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// バッファの使い方
	swapChainDesc.BufferCount = 2;	// バッファの数
	swapChainDesc.OutputWindow = hWnd;	// バッファを表示するウィンドウ
	swapChainDesc.Windowed = TRUE;	// ウィンドウ化するかどうか
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

	// デバイスとデバイスコンテキストとスワップチェーンを作成
	hr = D3D11CreateDeviceAndSwapChain(
		NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, creationFlag, featureLevels, ARRAYSIZE(featureLevels),
		D3D11_SDK_VERSION, &swapChainDesc, &swapChain,
		&graphicsDevice, &getFeatureLevel, &immediateContext);

	// デバイスとデバイスコンテキストとスワップチェーンを作成できたかチェック
	if (FAILED(hr)) {
		MessageBox(hWnd, L"デバイスかスワップチェーンの作成に失敗しました。", L"error", MB_OK);
		return -1;
	}

	// バックバッファー
	ComPtr<ID3D11Texture2D> backBuffer = nullptr;
	// バッファーを取得
	hr = swapChain->GetBuffer(
		0,	// 取得するバッファーのインデックス
		__uuidof(ID3D11Texture2D),	// 取得するバッファーのタイプ
		&backBuffer	// 取得したバッファーを格納するポインタ
	);
	if (FAILED(hr)) {
		MessageBox(hWnd, L"バックバッファーの作成に失敗しました。", L"error", MB_OK);
		return -1;
	}
	
	// 画面クリアーに使用するカラー
	FLOAT clearColor[] = { 53 / 255.0f, 70 / 255.0f, 166 / 255.0f, 1.0f };
	// レンダーターゲットビュー
	ComPtr<ID3D11RenderTargetView> renderTargetViews[1];
	hr = graphicsDevice->CreateRenderTargetView(
		backBuffer.Get(),
		nullptr,
		&renderTargetViews[0]
	);
	if (FAILED(hr)) {
		MessageBox(hWnd, L"レンダーターゲットビューの作成に失敗しました。", L"error", MB_OK);
		return -1;
	}

	MSG msg = {};
	// メッセージループ
	while (true) {
		// Direct3Dによる描画処理
		// レンダーターゲットを設定
		immediateContext->OMSetRenderTargets(1,renderTargetViews->GetAddressOf(), nullptr);
		// 画面をクリアー
		immediateContext->ClearRenderTargetView(renderTargetViews[0].Get(), clearColor);
		// バックバッファをディスプレイに表示
		hr = swapChain->Present(1, 0);
		if (FAILED(hr))
		{
			MessageBox(hWnd,
				L"グラフィックデバイスが物理的に取り外されたか、ドライバーがアップデートされました。",
				L"エラー", MB_OK);
			return -1;
		}


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

	// 開放処理
	//SAFE_RELEASE(graphicsDevice);
	//SAFE_RELEASE(immediateContext);

	return (int)msg.wParam;

}


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



