//=================================================
//Game.cpp
//
//=================================================
#include <DirectXMath.h>
#include <DirectXColors.h>
#include "Game.h"
#include "BasicVertexShader.h"
#include "BasicGeometryShader.h"
#include "BasicPixelShader.h"

using namespace DirectX;

// プロトタイプ宣言
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// 初期化処理
void Game::Initialize(Game* game, const LPCWSTR windowTitle, const float width, const float height) {
	game->WindowTitle = windowTitle;
	game->ScreenWidth = width;
	game->ScreenHeight = height;
}

// ウィンドウを作成する関数
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

// グラフィックデバイスを作成する関数
bool Game::InitGraphicsDevice() {
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
	swapChainDesc.BufferUsage =		// バッファの使用方法
		DXGI_USAGE_RENDER_TARGET_OUTPUT |	// シェーダーリソースとして使用することを設定
		DXGI_USAGE_SHADER_INPUT;	
	// バッファの使い方
	swapChainDesc.BufferCount = 2;	// バッファの数
	swapChainDesc.OutputWindow = hWnd;	// バッファを表示するウィンドウ
	swapChainDesc.Windowed = TRUE;	// ウィンドウ化するかどうか
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

	// デバイスとデバイスコンテキストとスワップチェーンを作成
	hr = D3D11CreateDeviceAndSwapChain(
		NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, creationFlag, featureLevels, ARRAYSIZE(featureLevels),
		D3D11_SDK_VERSION, &swapChainDesc, swapChain.GetAddressOf(),
		graphicsDevice.GetAddressOf(), &getFeatureLevel, immediateContext.GetAddressOf());

	// デバイスとデバイスコンテキストとスワップチェーンを作成できたかチェック
	if (FAILED(hr)) {
		MessageBox(hWnd, L"デバイスかスワップチェーンの作成に失敗しました。", L"error", MB_OK);
		return FALSE;
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
		return FALSE;
	}
	// バックバッファにアクセスするためのレンダーターゲットビューを作成
	hr = graphicsDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, renderTargetViews[0].GetAddressOf());
	if (FAILED(hr)) {
		MessageBox(hWnd, L"レンダーターゲットビューの作成に失敗しました。", L"error", MB_OK);
		return FALSE;
	}
	// バックバッファにシェーダーからアクセスするためのリソースビューを作成
	hr = graphicsDevice->CreateShaderResourceView(backBuffer.Get(), NULL, renderTargetResourceView.GetAddressOf());
	if (FAILED(hr)) {
		MessageBox(hWnd, L"レンダーターゲットリソースビューの作成に失敗しました。", L"error", MB_OK);
		return FALSE;
	}


	// テクスチャのフォーマットを設定
	DXGI_FORMAT textureFormat = depthStencilFormat;
	DXGI_FORMAT resourceFormat = depthStencilFormat;
	switch (depthStencilFormat)
	{
	case DXGI_FORMAT_D16_UNORM:
		textureFormat = DXGI_FORMAT_R16_TYPELESS;
		resourceFormat = DXGI_FORMAT_R16_UNORM;
		break;
	case DXGI_FORMAT_D24_UNORM_S8_UINT:
		textureFormat = DXGI_FORMAT_R24G8_TYPELESS;
		resourceFormat = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		break;
	case DXGI_FORMAT_D32_FLOAT:
		textureFormat = DXGI_FORMAT_R32_TYPELESS;
		resourceFormat = DXGI_FORMAT_R32_FLOAT;
		break;
	case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
		textureFormat = DXGI_FORMAT_R32G8X24_TYPELESS;
		resourceFormat = DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS;
		break;
	}
	// 深度ステンシルバッファについての記述をする構造体
	D3D11_TEXTURE2D_DESC depthStencilDesc = {};
	// 深度ステンシルバッファ
	ComPtr<ID3D11Texture2D> depthStencilBuffer = nullptr;
	depthStencilDesc.Width = swapChainDesc.BufferDesc.Width;
	depthStencilDesc.Height = swapChainDesc.BufferDesc.Height;
	depthStencilDesc.MipLevels = 1;		// ミップマップのレベル
	depthStencilDesc.ArraySize = 1;		// テクスチャ配列のサイズ
	depthStencilDesc.Format = textureFormat;	// テクスチャのフォーマット
	depthStencilDesc.SampleDesc = swapChainDesc.SampleDesc;	// マルチサンプリング設定
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;	// テクスチャをどのように読み書きするか
	depthStencilDesc.BindFlags =
		D3D11_BIND_DEPTH_STENCIL |	// 深度ステンシルに設定
		D3D11_BIND_SHADER_RESOURCE;	// シェーダーリソースとして使用することを設定
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;
	// 深度ステンシルバッファを作成
	hr = graphicsDevice->CreateTexture2D(
		&depthStencilDesc, NULL, depthStencilBuffer.GetAddressOf()
	);
	if (FAILED(hr)) {
		MessageBox(hWnd, L"深度ステンシルバッファの作成に失敗しました。", L"error", MB_OK);
		return FALSE;
	}
	// 深度ステンシルビューについて記述する構造体
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
	depthStencilViewDesc.Format = depthStencilFormat;	// 深度ステンシルビューのフォーマット
	// マルチサンプリングをしていたらテクスチャの種類を変更
	if (depthStencilDesc.SampleDesc.Count > 0) {
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	}
	else {
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;
	}
	depthStencilViewDesc.Texture2D.MipSlice = 0;	// 最初に使用するミップマップのレベル

	// 深度ステンシルビューを作成
	hr = graphicsDevice->CreateDepthStencilView(
		depthStencilBuffer.Get(), &depthStencilViewDesc, depthStencilView.GetAddressOf()
	);

	// 深度ステンシルにシェーダからアクセスするためのリソースビューについての記述をする構造体
	D3D11_SHADER_RESOURCE_VIEW_DESC depthStencilResourceViewDesc = {};
	depthStencilResourceViewDesc.Format = resourceFormat;
	if (depthStencilDesc.SampleDesc.Count > 0) {
		depthStencilResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
	}
	else {
		depthStencilResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		depthStencilResourceViewDesc.Texture2D.MostDetailedMip = 0;
		depthStencilResourceViewDesc.Texture2D.MipLevels = 1;
	}
	// 深度ステンシルにシェーダーからアクセスするためのリソースビューを作成
	hr = graphicsDevice->CreateShaderResourceView(
		depthStencilBuffer.Get(),
		&depthStencilResourceViewDesc,
		depthStencilResoureceView.GetAddressOf());
	if (FAILED(hr)) {
		MessageBox(hWnd, L"深度ステンシル リソース ビューを作成できませんでした。", L"エラー", MB_OK);
		return FALSE;
	}

	// ビューポート
	viewports[0].Width = static_cast<FLOAT>(ScreenWidth);
	viewports[0].Height = static_cast<FLOAT>(ScreenHeight);
	viewports[0].MinDepth = 0.0f;
	viewports[0].MaxDepth = 1.0f;
	viewports[0].TopLeftX = 0.0f;
	viewports[0].TopLeftY = 0.0f;

	return true;
}

// ループ本体
int Game::Run() {
	// ウィンドウを作成
	if (!InitWindow()) {
		MessageBox(NULL, L"ウィンドウの初期化に失敗しました。", L"error", MB_OK);
		return -1;
	}
	// グラフィックデバイスを作成
	if (!InitGraphicsDevice()) {
		MessageBox(NULL, L"グラフィックデバイスの初期化に失敗しました。", L"error", MB_OK);
		return -1;
	}

	HRESULT hr = S_OK;

	// 頂点バッファ
	ComPtr<ID3D11Buffer> vertexBuffer = nullptr;

	// 頂点データに含めるデータ
	struct VertexData
	{
		XMFLOAT3 position;	// 位置データ
		XMFLOAT3 normal;	// 法線データ
	};

	VertexData vertexData[] = {
		{ { -1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, -1.0f } },
		{ {  0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, -1.0f } },
		{ {  1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, -1.0f } },
	};

	// 頂点バッファについて記述する構造体
	D3D11_BUFFER_DESC vertexBufferDesc = {};
	vertexBufferDesc.ByteWidth = sizeof(vertexData);	// バッファのサイズ
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// バッファの使用方法(頂点バッファとして使用)
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;	// 頂点バッファとして使うなら0
	// 頂点バッファを作成
	hr = graphicsDevice->CreateBuffer(&vertexBufferDesc, NULL, vertexBuffer.GetAddressOf());
	if (FAILED(hr)) {
		MessageBox(hWnd, L"頂点バッファを作成できませんでした。", L"エラー", MB_OK);
		return FALSE;
	}

	// 作成したバッファにデータを転送
	immediateContext->UpdateSubresource(vertexBuffer.Get(), 0, NULL, vertexData, 0, 0);

	// インデックスデータ
	UINT indices[] = { 0,1,2 };

	// インデックスバッファについて記述する構造体
	D3D11_BUFFER_DESC indexBufferDesc = {};
	indexBufferDesc.ByteWidth = sizeof(indices);
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;	// インデックスバッファとして使用
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;
	// インデックスバッファ
	ComPtr<ID3D11Buffer> indexBuffer = nullptr;
	// インデックスバッファを作成
	hr = graphicsDevice->CreateBuffer(&indexBufferDesc, NULL, indexBuffer.GetAddressOf());
	if (FAILED(hr)) {
		MessageBox(hWnd, L"インデックスバッファを作成できませんでした。", L"エラー", MB_OK);
		return FALSE;
	}

	// 作成したバッファにデータを転送
	immediateContext->UpdateSubresource(indexBuffer.Get(), 0, NULL, indices, 0, 0);

	// 定数バッファーを介してシェーダーに毎フレーム送るデータ
	struct MatricesPerFrame {
		DirectX::XMFLOAT4X4 worldMatrix;
		DirectX::XMFLOAT4X4 viewMatrix;
		DirectX::XMFLOAT4X4 projectionMatrix;
		DirectX::XMFLOAT4X4 worldViewProjectionMatrix;
	};
	// 定数バッファーを作成
	ComPtr<ID3D11Buffer> constantBuffer = nullptr;
	{
		// 作成するバッファについての記述
		D3D11_BUFFER_DESC bufferDesc = { 0 };
		bufferDesc.ByteWidth = sizeof(MatricesPerFrame);	// 作成するバッファーのサイズ
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;		// バッファーの使用用途
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;		// 定数バッファーとして使用
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;
		graphicsDevice->CreateBuffer(&bufferDesc, nullptr, constantBuffer.GetAddressOf());
		if (FAILED(hr)) {
			MessageBox(hWnd, L"定数バッファを作成できませんでした。", L"エラー", MB_OK);
			return FALSE;
		}
	}

	// 頂点シェーダーを作成
	ComPtr<ID3D11VertexShader> vertexShader = nullptr;
	hr = graphicsDevice->CreateVertexShader(
		g_BasicVertexShader,
		ARRAYSIZE(g_BasicVertexShader),
		NULL,
		vertexShader.GetAddressOf()
	);
	if (FAILED(hr)) {
		OutputDebugString(L"頂点シェーダーの作成に失敗しました。");
	}

	// ジオメトリーシェーダーを作成
	ComPtr<ID3D11GeometryShader> geometryShader = nullptr;
	hr = graphicsDevice->CreateGeometryShader(g_BasicGeometryShader,ARRAYSIZE(g_BasicGeometryShader),NULL,geometryShader.GetAddressOf()
	);
	if (FAILED(hr)) {
		OutputDebugString(L"ジオメトリ―シェーダーの作成に失敗しました。");
	}

	// ピクセルシェーダーを作成
	ComPtr<ID3D11PixelShader> pixelShader = nullptr;
	hr = graphicsDevice->CreatePixelShader(g_BasicPixelShader, ARRAYSIZE(g_BasicPixelShader), NULL, pixelShader.GetAddressOf());
	if (FAILED(hr)){
		OutputDebugString(L"ピクセルシェーダーの作成に失敗しました。");
	}


	D3D11_INPUT_ELEMENT_DESC inputElementDesc[] = {
		{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"NORMAL",  0,DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0}
	};
	// 入力レイアウトを作成
	ComPtr<ID3D11InputLayout> inputLayout = nullptr;
	hr = graphicsDevice->CreateInputLayout(
		inputElementDesc, ARRAYSIZE(inputElementDesc),		// 入力レイアウトについての記述とその配列の数
		g_BasicVertexShader, ARRAYSIZE(g_BasicVertexShader),	// 入力を受け取る頂点シェーダーのバイトコードとサイズ
		inputLayout.GetAddressOf()
	);
	if (FAILED(hr)) {
		OutputDebugString(L"入力レイアウトを作成できませんでした。");
	}

	MSG msg = {};
	// メッセージループ
	while (true) {
		// 定数バッファーへ転送するデータソースを準備
		XMMATRIX worldMatrix = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
		// ビュー行列を計算
		XMVECTOR eyePosition = XMVectorSet(0, 1, -10, 1);
		XMVECTOR focusPosition = XMVectorSet(0, 1, 0, 1);
		XMVECTOR upDirection = XMVectorSet(0, 1, 0, 0);
		XMMATRIX viewMatrix =
			XMMatrixLookAtLH(eyePosition, focusPosition, upDirection);
		// プロジェクション行列を計算
		float fovAngleY = 60.0f;
		float aspectRatio = 640 / 480.0f;
		float nearZ = 0.3f;
		float farZ = 1000.0f;
		XMMATRIX projectionMatrix =
			XMMatrixPerspectiveFovLH(XMConvertToRadians(fovAngleY), aspectRatio, nearZ, farZ);

		MatricesPerFrame matricesPerFrame = {};
		XMStoreFloat4x4(
			&matricesPerFrame.worldMatrix,
			XMMatrixTranspose(worldMatrix));
		XMStoreFloat4x4(
			&matricesPerFrame.viewMatrix,
			XMMatrixTranspose(viewMatrix));
		XMStoreFloat4x4(
			&matricesPerFrame.projectionMatrix,
			XMMatrixTranspose(projectionMatrix));
		XMStoreFloat4x4(
			&matricesPerFrame.worldViewProjectionMatrix,
			XMMatrixTranspose(worldMatrix* viewMatrix* projectionMatrix));
		// 定数バッファーを更新
		immediateContext->UpdateSubresource(constantBuffer.Get(), 0, NULL, &matricesPerFrame, 0, 0);

		// レンダーターゲットを設定
		immediateContext->OMSetRenderTargets(1,renderTargetViews->GetAddressOf(), nullptr);
		// 画面をクリアー
		immediateContext->ClearRenderTargetView(renderTargetViews[0].Get(), clearColor);
		// 深度ステンシルをクリア
		immediateContext->ClearDepthStencilView(
			depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0
		);
		// ビューポートを設定
		immediateContext->RSSetViewports(1,viewports);

		// Direct3Dによる描画処理
		// 頂点バッファを設定
		ID3D11Buffer* vertexBuffers[1] = { vertexBuffer.Get() };
		UINT strides[1] = { sizeof(VertexData) };
		UINT offsets[1] = { 0 };
		// インプットアセンブリーに頂点バッファを送る
		immediateContext->IASetVertexBuffers(
			0,
			ARRAYSIZE(vertexBuffers),
			vertexBuffers, strides, offsets);
		// 頂点シェーダーをセット
		immediateContext->VSSetShader(vertexShader.Get(), NULL, 0);
		// ジオメトリーシェーダーをセット
		immediateContext->GSSetShader(geometryShader.Get(), NULL, 0);
		// ピクセルシェーダーをセット
		immediateContext->PSSetShader(pixelShader.Get(), NULL, 0);

		// 頂点シェーダーに定数バッファーを設定
		ComPtr<ID3D11Buffer> constantBuffers[1] = { constantBuffer.Get() };
		immediateContext->VSSetConstantBuffers(0, 1, constantBuffers->GetAddressOf());

		// 頂点バッファーと頂点シェーダーの組み合わせに対応した入力レイアウトを設定
		immediateContext->IASetInputLayout(inputLayout.Get());
		// プリミティブトポロジーとしてトライアングルをセット
		immediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// インデックスバッファを設定
		immediateContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		// 描画
		immediateContext->DrawIndexed(ARRAYSIZE(indices), 0, 0);

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



