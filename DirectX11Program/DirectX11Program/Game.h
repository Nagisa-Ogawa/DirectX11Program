#pragma once
#include <Windows.h>

#define SAFE_RELEASE(p) if((p)!=nullptr){ (p)->Release(); (p)=nullptr;} 
// �A�v���P�[�V�����S�̂�\���\����
class Game
{
public:
	// ����������
	void Initialize(Game* game, const LPCWSTR windowTitle, const float width, const float height);
	// ���[�v�{��
	int Run();
private:
	// �E�B���h�E�̃n���h��
	HWND hWnd = NULL;
	// �쐬����E�B���h�E����
	LPCWSTR WindowTitle = L"�^�C�g��";
	// �쐬����E�B���h�E�̉���
	float ScreenWidth = 600.0f;
	// �쐬����E�B���h�E�̍���
	float ScreenHeight = 400.0f;

	// �E�B���h�E���쐬
	bool InitWindow();
};
