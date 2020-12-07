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

// �v���g�^�C�v�錾
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// ����������
void Game::Initialize(Game* game, const LPCWSTR windowTitle, const float width, const float height) {
	game->WindowTitle = windowTitle;
	game->ScreenWidth = width;
	game->ScreenHeight = height;
}

// �E�B���h�E���쐬����֐�
bool Game::InitWindow() {
	HINSTANCE hinstance = GetModuleHandle(NULL);
	// �N���X��
	const wchar_t CLASS_NAME[] = L"GameWindow";
	// �E�B���h�E�N���X���쐬
	WNDCLASSEX wndClass = {};
	wndClass.cbSize = sizeof(WNDCLASSEX);	// �E�B���h�E�N���X�̃T�C�Y
	wndClass.style = CS_HREDRAW;			// �E�B���h�E�N���X�̊�{�X�^�C��
	wndClass.lpfnWndProc = WndProc;			// �E�B���h�E�v���V�[�W���̃|�C���^
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hinstance;		// HINSTANCE
	wndClass.hIcon = NULL;
	wndClass.hCursor = NULL;
	wndClass.hbrBackground = (HBRUSH)COLOR_BACKGROUND;	// �w�i
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = CLASS_NAME;
	wndClass.hIconSm = NULL;
	// �쐬�Ɏ��s�����Ȃ烁�b�Z�[�W�{�b�N�X���o���ďI��
	if (!RegisterClassEx(&wndClass)) {
		MessageBox(NULL, L"�E�B���h�E�N���X�̍쐬�Ɏ��s���܂����B", L"error", MB_OK);
		return false;
	}

	// �N���C�A���g�̈�̃T�C�Y
	RECT rect = { 0,0,(LONG)ScreenWidth,(LONG)ScreenHeight };
	// �N���C�A���g�̈�̃T�C�Y����E�B���h�E�̃T�C�Y���v�Z
	AdjustWindowRectEx(&rect, WS_CAPTION, NULL, WS_CAPTION);
	// �E�B���h�E���쐬
	HWND hWnd = CreateWindowEx(
		0,	// �g���E�B���h�E�̃X�^�C��
		L"GameWindow",				// �쐬�����E�B���h�E�N���X
		L"Window",						// �E�B���h�E��
		WS_OVERLAPPEDWINDOW,			// �E�B���h�E�̃X�^�C��
		CW_USEDEFAULT,					// �E�B���h�E��x���W
		CW_USEDEFAULT,					// �E�B���h�E��y���W
		(rect.right - rect.left),		// �E�B���h�E�̉���
		(rect.bottom - rect.top),		// �E�B���h�E�̏c��
		NULL,				// �E�B���h�E�̐e
		NULL,				// �E�B���h�E���j���[
		hinstance,			// HINSTANCE
		NULL				// �ǉ��̃f�[�^��n���\���̂̃|�C���^�[
	);
	// �쐬�Ɏ��s�����烁�b�Z�[�W�{�b�N�X���o���ďI��
	if (hWnd == NULL) {
		MessageBox(NULL, L"�E�B���h�E�̍쐬�Ɏ��s���܂����B", L"error", MB_OK);
		return false;
	}
	// �E�B���h�E�̕\��
	ShowWindow(hWnd, SW_SHOWNORMAL);
	UpdateWindow(hWnd);
	this->hWnd = hWnd;

	return true;
}

// �O���t�B�b�N�f�o�C�X���쐬����֐�
bool Game::InitGraphicsDevice() {
	// �f�o�C�X�쐬���̃I�v�V�����t���O
	UINT creationFlag = 0;
	if (_DEBUG) {
		// �f�o�b�O���ɂ̂݃f�o�b�N�p�̃I�v�V������ǉ�����
		creationFlag |= D3D11_CREATE_DEVICE_DEBUG;
	}
	HRESULT hr;
	// �X���b�v�`�F�[���ɂ��Đݒ肷��\����
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferDesc.Width = (UINT)ScreenWidth;	// �o�b�t�@�̉���
	swapChainDesc.BufferDesc.Height = (UINT)ScreenHeight;	// �o�b�t�@�̍���
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;	// �t�H�[�}�b�g
	swapChainDesc.BufferDesc.RefreshRate = { 60,1 };	// ���t���b�V�����[�g(1/60)
	swapChainDesc.SampleDesc = { 1,0 };	/// �}���`�T���v�����O�i���g�p�j
	swapChainDesc.BufferUsage =		// �o�b�t�@�̎g�p���@
		DXGI_USAGE_RENDER_TARGET_OUTPUT |	// �V�F�[�_�[���\�[�X�Ƃ��Ďg�p���邱�Ƃ�ݒ�
		DXGI_USAGE_SHADER_INPUT;	
	// �o�b�t�@�̎g����
	swapChainDesc.BufferCount = 2;	// �o�b�t�@�̐�
	swapChainDesc.OutputWindow = hWnd;	// �o�b�t�@��\������E�B���h�E
	swapChainDesc.Windowed = TRUE;	// �E�B���h�E�����邩�ǂ���
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

	// �f�o�C�X�ƃf�o�C�X�R���e�L�X�g�ƃX���b�v�`�F�[�����쐬
	hr = D3D11CreateDeviceAndSwapChain(
		NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, creationFlag, featureLevels, ARRAYSIZE(featureLevels),
		D3D11_SDK_VERSION, &swapChainDesc, swapChain.GetAddressOf(),
		graphicsDevice.GetAddressOf(), &getFeatureLevel, immediateContext.GetAddressOf());

	// �f�o�C�X�ƃf�o�C�X�R���e�L�X�g�ƃX���b�v�`�F�[�����쐬�ł������`�F�b�N
	if (FAILED(hr)) {
		MessageBox(hWnd, L"�f�o�C�X���X���b�v�`�F�[���̍쐬�Ɏ��s���܂����B", L"error", MB_OK);
		return FALSE;
	}

	// �o�b�N�o�b�t�@�[
	ComPtr<ID3D11Texture2D> backBuffer = nullptr;
	// �o�b�t�@�[���擾
	hr = swapChain->GetBuffer(
		0,	// �擾����o�b�t�@�[�̃C���f�b�N�X
		__uuidof(ID3D11Texture2D),	// �擾����o�b�t�@�[�̃^�C�v
		&backBuffer	// �擾�����o�b�t�@�[���i�[����|�C���^
	);
	if (FAILED(hr)) {
		MessageBox(hWnd, L"�o�b�N�o�b�t�@�[�̍쐬�Ɏ��s���܂����B", L"error", MB_OK);
		return FALSE;
	}
	// �o�b�N�o�b�t�@�ɃA�N�Z�X���邽�߂̃����_�[�^�[�Q�b�g�r���[���쐬
	hr = graphicsDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, renderTargetViews[0].GetAddressOf());
	if (FAILED(hr)) {
		MessageBox(hWnd, L"�����_�[�^�[�Q�b�g�r���[�̍쐬�Ɏ��s���܂����B", L"error", MB_OK);
		return FALSE;
	}
	// �o�b�N�o�b�t�@�ɃV�F�[�_�[����A�N�Z�X���邽�߂̃��\�[�X�r���[���쐬
	hr = graphicsDevice->CreateShaderResourceView(backBuffer.Get(), NULL, renderTargetResourceView.GetAddressOf());
	if (FAILED(hr)) {
		MessageBox(hWnd, L"�����_�[�^�[�Q�b�g���\�[�X�r���[�̍쐬�Ɏ��s���܂����B", L"error", MB_OK);
		return FALSE;
	}


	// �e�N�X�`���̃t�H�[�}�b�g��ݒ�
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
	// �[�x�X�e���V���o�b�t�@�ɂ��Ă̋L�q������\����
	D3D11_TEXTURE2D_DESC depthStencilDesc = {};
	// �[�x�X�e���V���o�b�t�@
	ComPtr<ID3D11Texture2D> depthStencilBuffer = nullptr;
	depthStencilDesc.Width = swapChainDesc.BufferDesc.Width;
	depthStencilDesc.Height = swapChainDesc.BufferDesc.Height;
	depthStencilDesc.MipLevels = 1;		// �~�b�v�}�b�v�̃��x��
	depthStencilDesc.ArraySize = 1;		// �e�N�X�`���z��̃T�C�Y
	depthStencilDesc.Format = textureFormat;	// �e�N�X�`���̃t�H�[�}�b�g
	depthStencilDesc.SampleDesc = swapChainDesc.SampleDesc;	// �}���`�T���v�����O�ݒ�
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;	// �e�N�X�`�����ǂ̂悤�ɓǂݏ������邩
	depthStencilDesc.BindFlags =
		D3D11_BIND_DEPTH_STENCIL |	// �[�x�X�e���V���ɐݒ�
		D3D11_BIND_SHADER_RESOURCE;	// �V�F�[�_�[���\�[�X�Ƃ��Ďg�p���邱�Ƃ�ݒ�
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;
	// �[�x�X�e���V���o�b�t�@���쐬
	hr = graphicsDevice->CreateTexture2D(
		&depthStencilDesc, NULL, depthStencilBuffer.GetAddressOf()
	);
	if (FAILED(hr)) {
		MessageBox(hWnd, L"�[�x�X�e���V���o�b�t�@�̍쐬�Ɏ��s���܂����B", L"error", MB_OK);
		return FALSE;
	}
	// �[�x�X�e���V���r���[�ɂ��ċL�q����\����
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
	depthStencilViewDesc.Format = depthStencilFormat;	// �[�x�X�e���V���r���[�̃t�H�[�}�b�g
	// �}���`�T���v�����O�����Ă�����e�N�X�`���̎�ނ�ύX
	if (depthStencilDesc.SampleDesc.Count > 0) {
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	}
	else {
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;
	}
	depthStencilViewDesc.Texture2D.MipSlice = 0;	// �ŏ��Ɏg�p����~�b�v�}�b�v�̃��x��

	// �[�x�X�e���V���r���[���쐬
	hr = graphicsDevice->CreateDepthStencilView(
		depthStencilBuffer.Get(), &depthStencilViewDesc, depthStencilView.GetAddressOf()
	);

	// �[�x�X�e���V���ɃV�F�[�_����A�N�Z�X���邽�߂̃��\�[�X�r���[�ɂ��Ă̋L�q������\����
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
	// �[�x�X�e���V���ɃV�F�[�_�[����A�N�Z�X���邽�߂̃��\�[�X�r���[���쐬
	hr = graphicsDevice->CreateShaderResourceView(
		depthStencilBuffer.Get(),
		&depthStencilResourceViewDesc,
		depthStencilResoureceView.GetAddressOf());
	if (FAILED(hr)) {
		MessageBox(hWnd, L"�[�x�X�e���V�� ���\�[�X �r���[���쐬�ł��܂���ł����B", L"�G���[", MB_OK);
		return FALSE;
	}

	// �r���[�|�[�g
	viewports[0].Width = static_cast<FLOAT>(ScreenWidth);
	viewports[0].Height = static_cast<FLOAT>(ScreenHeight);
	viewports[0].MinDepth = 0.0f;
	viewports[0].MaxDepth = 1.0f;
	viewports[0].TopLeftX = 0.0f;
	viewports[0].TopLeftY = 0.0f;

	return true;
}

// ���[�v�{��
int Game::Run() {
	// �E�B���h�E���쐬
	if (!InitWindow()) {
		MessageBox(NULL, L"�E�B���h�E�̏������Ɏ��s���܂����B", L"error", MB_OK);
		return -1;
	}
	// �O���t�B�b�N�f�o�C�X���쐬
	if (!InitGraphicsDevice()) {
		MessageBox(NULL, L"�O���t�B�b�N�f�o�C�X�̏������Ɏ��s���܂����B", L"error", MB_OK);
		return -1;
	}

	HRESULT hr = S_OK;

	// ���_�o�b�t�@
	ComPtr<ID3D11Buffer> vertexBuffer = nullptr;

	// ���_�f�[�^�Ɋ܂߂�f�[�^
	struct VertexData
	{
		XMFLOAT3 position;	// �ʒu�f�[�^
		XMFLOAT3 normal;	// �@���f�[�^
	};

	VertexData vertexData[] = {
		{ { -1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, -1.0f } },
		{ {  0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, -1.0f } },
		{ {  1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, -1.0f } },
	};

	// ���_�o�b�t�@�ɂ��ċL�q����\����
	D3D11_BUFFER_DESC vertexBufferDesc = {};
	vertexBufferDesc.ByteWidth = sizeof(vertexData);	// �o�b�t�@�̃T�C�Y
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// �o�b�t�@�̎g�p���@(���_�o�b�t�@�Ƃ��Ďg�p)
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;	// ���_�o�b�t�@�Ƃ��Ďg���Ȃ�0
	// ���_�o�b�t�@���쐬
	hr = graphicsDevice->CreateBuffer(&vertexBufferDesc, NULL, vertexBuffer.GetAddressOf());
	if (FAILED(hr)) {
		MessageBox(hWnd, L"���_�o�b�t�@���쐬�ł��܂���ł����B", L"�G���[", MB_OK);
		return FALSE;
	}

	// �쐬�����o�b�t�@�Ƀf�[�^��]��
	immediateContext->UpdateSubresource(vertexBuffer.Get(), 0, NULL, vertexData, 0, 0);

	// �C���f�b�N�X�f�[�^
	UINT indices[] = { 0,1,2 };

	// �C���f�b�N�X�o�b�t�@�ɂ��ċL�q����\����
	D3D11_BUFFER_DESC indexBufferDesc = {};
	indexBufferDesc.ByteWidth = sizeof(indices);
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;	// �C���f�b�N�X�o�b�t�@�Ƃ��Ďg�p
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;
	// �C���f�b�N�X�o�b�t�@
	ComPtr<ID3D11Buffer> indexBuffer = nullptr;
	// �C���f�b�N�X�o�b�t�@���쐬
	hr = graphicsDevice->CreateBuffer(&indexBufferDesc, NULL, indexBuffer.GetAddressOf());
	if (FAILED(hr)) {
		MessageBox(hWnd, L"�C���f�b�N�X�o�b�t�@���쐬�ł��܂���ł����B", L"�G���[", MB_OK);
		return FALSE;
	}

	// �쐬�����o�b�t�@�Ƀf�[�^��]��
	immediateContext->UpdateSubresource(indexBuffer.Get(), 0, NULL, indices, 0, 0);

	// �萔�o�b�t�@�[����ăV�F�[�_�[�ɖ��t���[������f�[�^
	struct MatricesPerFrame {
		DirectX::XMFLOAT4X4 worldMatrix;
		DirectX::XMFLOAT4X4 viewMatrix;
		DirectX::XMFLOAT4X4 projectionMatrix;
		DirectX::XMFLOAT4X4 worldViewProjectionMatrix;
	};
	// �萔�o�b�t�@�[���쐬
	ComPtr<ID3D11Buffer> constantBuffer = nullptr;
	{
		// �쐬����o�b�t�@�ɂ��Ă̋L�q
		D3D11_BUFFER_DESC bufferDesc = { 0 };
		bufferDesc.ByteWidth = sizeof(MatricesPerFrame);	// �쐬����o�b�t�@�[�̃T�C�Y
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;		// �o�b�t�@�[�̎g�p�p�r
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;		// �萔�o�b�t�@�[�Ƃ��Ďg�p
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;
		graphicsDevice->CreateBuffer(&bufferDesc, nullptr, constantBuffer.GetAddressOf());
		if (FAILED(hr)) {
			MessageBox(hWnd, L"�萔�o�b�t�@���쐬�ł��܂���ł����B", L"�G���[", MB_OK);
			return FALSE;
		}
	}

	// ���_�V�F�[�_�[���쐬
	ComPtr<ID3D11VertexShader> vertexShader = nullptr;
	hr = graphicsDevice->CreateVertexShader(
		g_BasicVertexShader,
		ARRAYSIZE(g_BasicVertexShader),
		NULL,
		vertexShader.GetAddressOf()
	);
	if (FAILED(hr)) {
		OutputDebugString(L"���_�V�F�[�_�[�̍쐬�Ɏ��s���܂����B");
	}

	// �W�I���g���[�V�F�[�_�[���쐬
	ComPtr<ID3D11GeometryShader> geometryShader = nullptr;
	hr = graphicsDevice->CreateGeometryShader(g_BasicGeometryShader,ARRAYSIZE(g_BasicGeometryShader),NULL,geometryShader.GetAddressOf()
	);
	if (FAILED(hr)) {
		OutputDebugString(L"�W�I���g���\�V�F�[�_�[�̍쐬�Ɏ��s���܂����B");
	}

	// �s�N�Z���V�F�[�_�[���쐬
	ComPtr<ID3D11PixelShader> pixelShader = nullptr;
	hr = graphicsDevice->CreatePixelShader(g_BasicPixelShader, ARRAYSIZE(g_BasicPixelShader), NULL, pixelShader.GetAddressOf());
	if (FAILED(hr)){
		OutputDebugString(L"�s�N�Z���V�F�[�_�[�̍쐬�Ɏ��s���܂����B");
	}


	D3D11_INPUT_ELEMENT_DESC inputElementDesc[] = {
		{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"NORMAL",  0,DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0}
	};
	// ���̓��C�A�E�g���쐬
	ComPtr<ID3D11InputLayout> inputLayout = nullptr;
	hr = graphicsDevice->CreateInputLayout(
		inputElementDesc, ARRAYSIZE(inputElementDesc),		// ���̓��C�A�E�g�ɂ��Ă̋L�q�Ƃ��̔z��̐�
		g_BasicVertexShader, ARRAYSIZE(g_BasicVertexShader),	// ���͂��󂯎�钸�_�V�F�[�_�[�̃o�C�g�R�[�h�ƃT�C�Y
		inputLayout.GetAddressOf()
	);
	if (FAILED(hr)) {
		OutputDebugString(L"���̓��C�A�E�g���쐬�ł��܂���ł����B");
	}

	MSG msg = {};
	// ���b�Z�[�W���[�v
	while (true) {
		// �萔�o�b�t�@�[�֓]������f�[�^�\�[�X������
		XMMATRIX worldMatrix = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
		// �r���[�s����v�Z
		XMVECTOR eyePosition = XMVectorSet(0, 1, -10, 1);
		XMVECTOR focusPosition = XMVectorSet(0, 1, 0, 1);
		XMVECTOR upDirection = XMVectorSet(0, 1, 0, 0);
		XMMATRIX viewMatrix =
			XMMatrixLookAtLH(eyePosition, focusPosition, upDirection);
		// �v���W�F�N�V�����s����v�Z
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
		// �萔�o�b�t�@�[���X�V
		immediateContext->UpdateSubresource(constantBuffer.Get(), 0, NULL, &matricesPerFrame, 0, 0);

		// �����_�[�^�[�Q�b�g��ݒ�
		immediateContext->OMSetRenderTargets(1,renderTargetViews->GetAddressOf(), nullptr);
		// ��ʂ��N���A�[
		immediateContext->ClearRenderTargetView(renderTargetViews[0].Get(), clearColor);
		// �[�x�X�e���V�����N���A
		immediateContext->ClearDepthStencilView(
			depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0
		);
		// �r���[�|�[�g��ݒ�
		immediateContext->RSSetViewports(1,viewports);

		// Direct3D�ɂ��`�揈��
		// ���_�o�b�t�@��ݒ�
		ID3D11Buffer* vertexBuffers[1] = { vertexBuffer.Get() };
		UINT strides[1] = { sizeof(VertexData) };
		UINT offsets[1] = { 0 };
		// �C���v�b�g�A�Z���u���[�ɒ��_�o�b�t�@�𑗂�
		immediateContext->IASetVertexBuffers(
			0,
			ARRAYSIZE(vertexBuffers),
			vertexBuffers, strides, offsets);
		// ���_�V�F�[�_�[���Z�b�g
		immediateContext->VSSetShader(vertexShader.Get(), NULL, 0);
		// �W�I���g���[�V�F�[�_�[���Z�b�g
		immediateContext->GSSetShader(geometryShader.Get(), NULL, 0);
		// �s�N�Z���V�F�[�_�[���Z�b�g
		immediateContext->PSSetShader(pixelShader.Get(), NULL, 0);

		// ���_�V�F�[�_�[�ɒ萔�o�b�t�@�[��ݒ�
		ComPtr<ID3D11Buffer> constantBuffers[1] = { constantBuffer.Get() };
		immediateContext->VSSetConstantBuffers(0, 1, constantBuffers->GetAddressOf());

		// ���_�o�b�t�@�[�ƒ��_�V�F�[�_�[�̑g�ݍ��킹�ɑΉ��������̓��C�A�E�g��ݒ�
		immediateContext->IASetInputLayout(inputLayout.Get());
		// �v���~�e�B�u�g�|���W�[�Ƃ��ăg���C�A���O�����Z�b�g
		immediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// �C���f�b�N�X�o�b�t�@��ݒ�
		immediateContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		// �`��
		immediateContext->DrawIndexed(ARRAYSIZE(indices), 0, 0);

		// �o�b�N�o�b�t�@���f�B�X�v���C�ɕ\��
		hr = swapChain->Present(1, 0);
		if (FAILED(hr))
		{
			MessageBox(hWnd,
				L"�O���t�B�b�N�f�o�C�X�������I�Ɏ��O���ꂽ���A�h���C�o�[���A�b�v�f�[�g����܂����B",
				L"�G���[", MB_OK);
			return -1;
		}

		// ���b�Z�[�W������Ȃ�
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
			// ���b�Z�[�W���󂯎��
			if (!GetMessage(&msg, NULL, 0, 0)) {
				break;
			}
			// �������b�Z�[�W�ɕϊ�
			TranslateMessage(&msg);
			// ���b�Z�[�W���E�B���h�E�v���V�[�W���ɑ���
			DispatchMessage(&msg);
		}
	}

	// �J������
	//SAFE_RELEASE(graphicsDevice);
	//SAFE_RELEASE(immediateContext);

	return (int)msg.wParam;

}


// �E�B���h�E�v���V�[�W��(���b�Z�[�W����������֐�)
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg)
	{
	case WM_CLOSE:
		if (MessageBox(hWnd, L"�E�B���h�E����܂����H", L"�x��", MB_YESNO) == IDYES) {
			// �E�B���h�E�����
			DestroyWindow(hWnd);
		}
		return 0;
	case WM_DESTROY:
		// �A�v���P�[�V�������I��
		PostQuitMessage(0);
		return 0;
	}

	// ����������Ȃ��������b�Z�[�W�̓f�t�H���g�֐��Ɉϑ�
	return DefWindowProc(hWnd, msg, wParam, lParam);
}



