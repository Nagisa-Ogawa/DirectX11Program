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
	MSG msg = {};
	// ���b�Z�[�W���[�v
	while (true) {
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

	return (int)msg.wParam;
}