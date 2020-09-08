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
	// ※IDXGIAdapter、DXGI_ADAPTER_DESC、IDXGIFactoryはIDXGIAdapter1及びDXGI_ADAPTER_DESC1及びIDXGIFactory1との混用は不可である（逆も同様）
	AdapterData(IDXGIAdapter* pAdapter);
	//格納するアダプタへのポインタ
	IDXGIAdapter* pAdapter;
	//アダプタそのものの説明（グラフィックカード名、メモリ量、メモリタイプ（GDDR5とか）など）
	DXGI_ADAPTER_DESC description;
};

class AdapterReader
{
public:
	//静的なものになったのは複数のウィンドウからのアクセスを可能にするため
	static std::vector<AdapterData> GetAdapters();
private:
	//静的なものになったのは複数のウィンドウからのアクセスを可能にするため
	static std::vector<AdapterData> adapters;
};