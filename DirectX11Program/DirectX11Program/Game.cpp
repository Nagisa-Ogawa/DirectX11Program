//=================================================
//Game.cpp
//
//=================================================
#include "Game.h"
#include <d3d11.h>
#include <wrl/client.h>

using namespace Microsoft::WRL;

// �v���g�^�C�v�錾
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// ����������
void Game::Initialize(Game* game, const LPCWSTR windowTitle, const float width, const float height) {
	game->WindowTitle = windowTitle;
	game->ScreenWidth = width;
	game->ScreenHeight = height;
}

// �E�B���h�E�̏�����������֐�
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
// ���[�v�{��
int Game::Run() {
	if (!InitWindow()) {
		MessageBox(NULL, L"�E�B���h�E�̏������Ɏ��s���܂����B", L"error", MB_OK);
		return -1;
	}

	// �f�o�C�X
	ComPtr<ID3D11Device> graphicsDevice = nullptr;
	// �f�o�C�X�R���e�L�X�g
	ComPtr<ID3D11DeviceContext> immediateContext = nullptr;
	// �X���b�v�`�F�[��
	ComPtr<IDXGISwapChain> swapChain = nullptr;
	// �g�p����featureLevel�̔z��
	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
	};
	// ���ۂɎg�p����featureLevel
	D3D_FEATURE_LEVEL getFeatureLevel;
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
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// �o�b�t�@�̎g����
	swapChainDesc.BufferCount = 2;	// �o�b�t�@�̐�
	swapChainDesc.OutputWindow = hWnd;	// �o�b�t�@��\������E�B���h�E
	swapChainDesc.Windowed = TRUE;	// �E�B���h�E�����邩�ǂ���
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

	// �f�o�C�X�ƃf�o�C�X�R���e�L�X�g�ƃX���b�v�`�F�[�����쐬
	hr = D3D11CreateDeviceAndSwapChain(
		NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, creationFlag, featureLevels, ARRAYSIZE(featureLevels),
		D3D11_SDK_VERSION, &swapChainDesc, &swapChain,
		&graphicsDevice, &getFeatureLevel, &immediateContext);

	// �f�o�C�X�ƃf�o�C�X�R���e�L�X�g�ƃX���b�v�`�F�[�����쐬�ł������`�F�b�N
	if (FAILED(hr)) {
		MessageBox(hWnd, L"�f�o�C�X���X���b�v�`�F�[���̍쐬�Ɏ��s���܂����B", L"error", MB_OK);
		return -1;
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
		return -1;
	}
	
	// ��ʃN���A�[�Ɏg�p����J���[
	FLOAT clearColor[] = { 53 / 255.0f, 70 / 255.0f, 166 / 255.0f, 1.0f };
	// �����_�[�^�[�Q�b�g�r���[
	ComPtr<ID3D11RenderTargetView> renderTargetViews[1];
	hr = graphicsDevice->CreateRenderTargetView(
		backBuffer.Get(),
		nullptr,
		&renderTargetViews[0]
	);
	if (FAILED(hr)) {
		MessageBox(hWnd, L"�����_�[�^�[�Q�b�g�r���[�̍쐬�Ɏ��s���܂����B", L"error", MB_OK);
		return -1;
	}

	MSG msg = {};
	// ���b�Z�[�W���[�v
	while (true) {
		// Direct3D�ɂ��`�揈��
		// �����_�[�^�[�Q�b�g��ݒ�
		immediateContext->OMSetRenderTargets(1,renderTargetViews->GetAddressOf(), nullptr);
		// ��ʂ��N���A�[
		immediateContext->ClearRenderTargetView(renderTargetViews[0].Get(), clearColor);
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



