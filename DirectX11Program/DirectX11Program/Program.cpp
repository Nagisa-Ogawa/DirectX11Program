//=================================================
//Program.cpp
//
//=================================================

#include "Game.h"
#include <crtdbg.h>

// �G���g���[�|�C���g
int WINAPI wWinMain(
	_In_ HINSTANCE hinstance,		// �C���X�^���X�n���h��
	_In_opt_ HINSTANCE hPrevInstance,	// ���NULL
	_In_ LPWSTR     lpCmdLine,		// �R�}���h���C������󂯎��������
	_In_ int		  nShowCmd			// �\�����
)
{
	// ���������[�N�����o
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Game game;
	// �A�v���P�[�V�����̋N���ݒ�
	game.Initialize(&game,L"Game", 600.0f, 400.0f);
	return	game.Run();
}