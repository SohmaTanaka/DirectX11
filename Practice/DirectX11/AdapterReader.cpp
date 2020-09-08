#include "AdapterReader.h"

//静的な変数の定義
std::vector<AdapterData> AdapterReader::adapters;

AdapterData::AdapterData(IDXGIAdapter* pAdapter)
{
	this->pAdapter = pAdapter;
	HRESULT hr = pAdapter->GetDesc(&this->description);

	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "IDXGIAdapterからdescriptionの取得に失敗");
	}
}

std::vector<AdapterData> AdapterReader::GetAdapters()
{
	//シングルトンパターン
	if (adapters.size() > 0)	//もし初期化が済んでいたら
		return adapters;

	// IDXGIFactoryが使われた場合、IDXGIFactory1との併用不可
	Microsoft::WRL::ComPtr<IDXGIFactory> pFactory;

	// IDXGIFactoryオブジェクトの作成
	HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(pFactory.GetAddressOf()));
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "IDXGIFactoryでのビデオアダプターの列挙に失敗");
		exit(-1);
	}

	IDXGIAdapter* pAdapter;
	UINT index = 0;

	//インデックスにアダプターが存在したら登録して次を探す
	//while文のかっこの真後ろに「;」を入れると無限ループになるぞ......気を付けろ......
	while (SUCCEEDED(pFactory->EnumAdapters(index,&pAdapter)))
	{
		adapters.push_back(AdapterData(pAdapter));
		index += 1;
	}

	return adapters;
}
