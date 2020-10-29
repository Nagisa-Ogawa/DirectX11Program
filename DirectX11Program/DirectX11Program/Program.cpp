//=================================================
//Program.cpp
//
//=================================================

#include <Windows.h>

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

// �G���g���[�|�C���g
int WINAPI wWinMain(
	_In_ HINSTANCE hinstance,		// �C���X�^���X�n���h��
	_In_opt_ HINSTANCE hPrevInstance,	// ���NULL
	_In_ LPWSTR     lpCmdLine,		// �R�}���h���C������󂯎��������
	_In_ int		  nShowCmd			// �\�����
)
{
	// �쐬����E�B���h�E�N���X��
	const WCHAR CLASSNAME[] = L"GameWindow";
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
	wndClass.lpszClassName = CLASSNAME;
	wndClass.hIconSm = NULL;
	// �쐬�Ɏ��s�����Ȃ烁�b�Z�[�W�{�b�N�X���o���ďI��
	if (!RegisterClassEx(&wndClass)) {
		MessageBox(NULL, L"�E�B���h�E�N���X�̍쐬�Ɏ��s���܂����B", L"error", MB_OK);
		return -1;
	}

	// �N���C�A���g�̈�̃T�C�Y
	RECT rect = { 0,0,600,400 };
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
		return -1;
	}
	// �E�B���h�E�̕\��
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);
	MSG msg = {};
	// ���b�Z�[�W���[�v
	while (true) {
		// ���b�Z�[�W������Ȃ�
		if (PeekMessage(&msg, hWnd, 0, 0, PM_NOREMOVE)) {
			// ���b�Z�[�W���󂯎��
			if (!GetMessage(&msg, hWnd, 0, 0)) {
				break;
			}
			// �������b�Z�[�W�ɕϊ�
			TranslateMessage(&msg);
			// ���b�Z�[�W���E�B���h�E�v���V�[�W���ɑ���
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}