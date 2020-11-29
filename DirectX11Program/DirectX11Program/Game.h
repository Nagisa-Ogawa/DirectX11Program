#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <wrl/client.h>

#define SAFE_RELEASE(p) if((p)!=nullptr){ (p)->Release(); (p)=nullptr;} 

using namespace Microsoft::WRL;

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


	// �f�o�C�X
	ComPtr<ID3D11Device> graphicsDevice = nullptr;
	// �f�o�C�X�R���e�L�X�g
	ComPtr<ID3D11DeviceContext> immediateContext = nullptr;
	// �X���b�v�`�F�[��
	ComPtr<IDXGISwapChain> swapChain = nullptr;
	// �����_�[�^�[�Q�b�g�r���[
	ComPtr<ID3D11RenderTargetView> renderTargetViews[1];
	// �o�b�N�o�b�t�@���V�F�[�_�[�ŗ��p���邽�߂̃��\�[�X�r���[
	ComPtr<ID3D11ShaderResourceView> renderTargetResourceView = nullptr;
	// �[�x�X�e���V���o�b�t�@�̃t�H�[�}�b�g
	const DXGI_FORMAT depthStencilFormat = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
	// �[�x�X�e���V���r���[
	ComPtr<ID3D11DepthStencilView> depthStencilView = nullptr;
	// �[�x�X�e���V�����V�F�[�_�[�ŗ��p���邽�߂̃��\�[�X�r���[
	ComPtr<ID3D11ShaderResourceView> depthStencilResoureceView = nullptr;
	// ��ʃN���A�[�Ɏg�p����J���[
	FLOAT clearColor[4] = { 53 / 255.0f, 70 / 255.0f, 166 / 255.0f, 1.0f };
	D3D11_VIEWPORT viewports[1] = {};
	// �g�p����featureLevel�̔z��
	D3D_FEATURE_LEVEL featureLevels[5] = {
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
	};
	// ���ۂɎg�p����featureLevel
	D3D_FEATURE_LEVEL getFeatureLevel;

	// �E�B���h�E���쐬
	bool InitWindow();
	// �O���t�B�b�N�f�o�C�X���쐬
	bool InitGraphicsDevice();
};
