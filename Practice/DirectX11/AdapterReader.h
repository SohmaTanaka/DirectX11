#pragma once
#include "DirectX11.h"
#include "ErrorLogger.h"
#include <d3d11.h>
#include <dxgi.h>
#include <wrl/client.h>
#include <vector>

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"DirectXTK.lib")
#pragma comment(lib,"DXGI.lib")

class AdapterData
{
public:
	// ��IDXGIAdapter�ADXGI_ADAPTER_DESC�AIDXGIFactory��IDXGIAdapter1�y��DXGI_ADAPTER_DESC1�y��IDXGIFactory1�Ƃ̍��p�͕s�ł���i�t�����l�j
	AdapterData(IDXGIAdapter* pAdapter);
	//�i�[����A�_�v�^�ւ̃|�C���^
	IDXGIAdapter* pAdapter;
	//�A�_�v�^���̂��̂̐����i�O���t�B�b�N�J�[�h���A�������ʁA�������^�C�v�iGDDR5�Ƃ��j�Ȃǁj
	DXGI_ADAPTER_DESC description;
};

class AdapterReader
{
public:
	//�ÓI�Ȃ��̂ɂȂ����͕̂����̃E�B���h�E����̃A�N�Z�X���\�ɂ��邽��
	static std::vector<AdapterData> GetAdapters();
private:
	//�ÓI�Ȃ��̂ɂȂ����͕̂����̃E�B���h�E����̃A�N�Z�X���\�ɂ��邽��
	static std::vector<AdapterData> adapters;
};