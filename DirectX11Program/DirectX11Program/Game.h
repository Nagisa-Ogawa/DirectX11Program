#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <wrl/client.h>

#define SAFE_RELEASE(p) if((p)!=nullptr){ (p)->Release(); (p)=nullptr;} 

using namespace Microsoft::WRL;

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


	// デバイス
	ComPtr<ID3D11Device> graphicsDevice = nullptr;
	// デバイスコンテキスト
	ComPtr<ID3D11DeviceContext> immediateContext = nullptr;
	// スワップチェーン
	ComPtr<IDXGISwapChain> swapChain = nullptr;
	// レンダーターゲットビュー
	ComPtr<ID3D11RenderTargetView> renderTargetViews[1];
	// バックバッファをシェーダーで利用するためのリソースビュー
	ComPtr<ID3D11ShaderResourceView> renderTargetResourceView = nullptr;
	// 深度ステンシルバッファのフォーマット
	const DXGI_FORMAT depthStencilFormat = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
	// 深度ステンシルビュー
	ComPtr<ID3D11DepthStencilView> depthStencilView = nullptr;
	// 深度ステンシルをシェーダーで利用するためのリソースビュー
	ComPtr<ID3D11ShaderResourceView> depthStencilResoureceView = nullptr;
	// 画面クリアーに使用するカラー
	FLOAT clearColor[4] = { 53 / 255.0f, 70 / 255.0f, 166 / 255.0f, 1.0f };
	D3D11_VIEWPORT viewports[1] = {};
	// 使用するfeatureLevelの配列
	D3D_FEATURE_LEVEL featureLevels[5] = {
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
	};
	// 実際に使用するfeatureLevel
	D3D_FEATURE_LEVEL getFeatureLevel;

	// ウィンドウを作成
	bool InitWindow();
	// グラフィックデバイスを作成
	bool InitGraphicsDevice();
};
