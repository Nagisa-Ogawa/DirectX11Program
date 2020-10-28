//=================================================
//Program.cpp
//
//=================================================

#include <Windows.h>

// �G���g���[�|�C���g
int WINAPI wWinMain(
	_In_ HINSTANCE hinstance,		// �C���X�^���X�n���h��
	_In_opt_ HINSTANCE hPrevInstance,	// ���NULL
	_In_ LPWSTR     lpCmdLine,		// �R�}���h���C������󂯎��������
	_In_ int		  nShowCmd			// �\�����
)
{
	// �N���C�A���g�̈�̃T�C�Y
	RECT rect = { 0,0,600,300 };
	// �N���C�A���g�̈�̃T�C�Y����E�B���h�E�̃T�C�Y���v�Z
	AdjustWindowRectEx(
		&rect,
		WS_CAPTION,
		NULL,
		WS_CAPTION
		);
	// �E�B���h�E���쐬
	HWND hWnd = CreateWindowEx(
		0,	// �g���E�B���h�E�̃X�^�C��
		L"Static",						// �V�X�e���Ŏ��O��`����Ă���E�B���h�E�N���X
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
	// �쐬�Ɏ��s������I��
	if (hWnd == NULL) {
		return -1;
	}
	// �E�B���h�E�̕\��
	ShowWindow(hWnd, SW_SHOW);
	// ���b�Z�[�W�{�b�N�X
	MessageBoxW(hWnd, L"����\n", L"��팋��\n", MB_OK);
	return 0;
}