#include "AdapterReader.h"

//�ÓI�ȕϐ��̒�`
std::vector<AdapterData> AdapterReader::adapters;

AdapterData::AdapterData(IDXGIAdapter* pAdapter)
{
	this->pAdapter = pAdapter;
	HRESULT hr = pAdapter->GetDesc(&this->description);

	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "IDXGIAdapter����description�̎擾�Ɏ��s");
	}
}

std::vector<AdapterData> AdapterReader::GetAdapters()
{
	//�V���O���g���p�^�[��
	if (adapters.size() > 0)	//�������������ς�ł�����
		return adapters;

	// IDXGIFactory���g��ꂽ�ꍇ�AIDXGIFactory1�Ƃ̕��p�s��
	Microsoft::WRL::ComPtr<IDXGIFactory> pFactory;

	// IDXGIFactory�I�u�W�F�N�g�̍쐬
	HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(pFactory.GetAddressOf()));
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "IDXGIFactory�ł̃r�f�I�A�_�v�^�[�̗񋓂Ɏ��s");
		exit(-1);
	}

	IDXGIAdapter* pAdapter;
	UINT index = 0;

	//�C���f�b�N�X�ɃA�_�v�^�[�����݂�����o�^���Ď���T��
	//while���̂������̐^���Ɂu;�v������Ɩ������[�v�ɂȂ邼......�C��t����......
	while (SUCCEEDED(pFactory->EnumAdapters(index,&pAdapter)))
	{
		adapters.push_back(AdapterData(pAdapter));
		index += 1;
	}

	return adapters;
}
