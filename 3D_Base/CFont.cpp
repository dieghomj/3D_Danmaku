#include "CFont.h"
#include "Source/CDirectX11.h"

using namespace std;

const TCHAR SHADER_NAME[] = _T("Data\\Shader\\DebugText.hlsl");


CFont::CFont()
{

}

CFont::~CFont()
{

}

HRESULT CFont::Init(CDirectX11& pDx11, LPCTSTR fontFilename, LPCTSTR fontFileTexture )
{
	m_pDx11 = &pDx11;
	m_pDevice11 = m_pDx11->GetDevice();		//���Ԃ͕ʂ̂Ƃ���ɂ���.���Ƃ����L���Ă���.
	m_pContext11 = m_pDx11->GetContext();	//���Ԃ͕ʂ̂Ƃ���ɂ���.���Ƃ����L���Ă���.

	//�V�F�[�_�쐬.
	if (FAILED(CreateShader()))
	{
		return E_FAIL;
	}
	//�|���S���쐬.
	if (FAILED(CreateModel()))
	{
		return E_FAIL;
	}
	if (FAILED(LoadFontData(fontFilename)))
	{
		return E_FAIL;
	}

	//�e�N�X�`���쐬.
	if (FAILED(CreateTexture(fontFileTexture)))
	{
		return E_FAIL;
	}
	//�T���v���쐬.
	if (FAILED(CreateSampler()))
	{
		return E_FAIL;
	}

	return S_OK;
}

void CFont::Render(LPCTSTR text, int x, int y)
{

}

//�V�F�[�_�쐬.
HRESULT CFont::CreateShader()
{
	ID3DBlob* pCompiledShader = nullptr;
	ID3DBlob* pErrors = nullptr;
	UINT uCompileFlag = 0;
#ifdef _DEBUG
	uCompileFlag =
		D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION;
#endif//#ifdef _DEBUG

	//HLSL����o�[�e�b�N�X�V�F�[�_�̃u���u���쐬.
	if (FAILED(
		D3DX11CompileFromFile(
			SHADER_NAME,	//�V�F�[�_�t�@�C�����iHLSL�t�@�C���j.
			nullptr,		//�}�N����`�̔z��ւ̃|�C���^�i���g�p�j.
			nullptr,		//�C���N���[�h�t�@�C���������C���^�[�t�F�C�X�ւ̃|�C���^�i���g�p�j.
			"VS_Main",		//�V�F�[�_�G���g���[�|�C���g�֐��̖��O.
			"vs_5_0",		//�V�F�[�_�̃��f�����w�肷�镶����i�v���t�@�C���j.
			uCompileFlag,	//�V�F�[�_�R���p�C���t���O.
			0,				//�G�t�F�N�g�R���p�C���t���O�i���g�p�j.
			nullptr,		//�X���b�h �|���v �C���^�[�t�F�C�X�ւ̃|�C���^�i���g�p�j.
			&pCompiledShader,//�u���u���i�[���郁�����ւ̃|�C���^.
			&pErrors,		//�G���[�ƌx���ꗗ���i�[���郁�����ւ̃|�C���^.
			nullptr)))		//�߂�l�ւ̃|�C���^�i���g�p�j.
	{
		_ASSERT_EXPR(false, _T("hlsl�ǂݍ��ݎ��s"));
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	//��L�ō쐬�����u���u����u�o�[�e�b�N�X�V�F�[�_�v���쐬.
	if (FAILED(
		m_pDevice11->CreateVertexShader(
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			nullptr,
			&m_pVertexShader)))	//(out)�o�[�e�b�N�X�V�F�[�_.
	{
		_ASSERT_EXPR(false, _T("�o�[�e�b�N�X�V�F�[�_�쐬���s"));
		return E_FAIL;
	}

	//���_�C���v�b�g���C�A�E�g���`.
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{
			"POSITION",						//�ʒu.
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,	//DXGI�̃t�H�[�}�b�g(32bit float�^*3).
			0,
			0,								//�f�[�^�̊J�n�ʒu.
			D3D11_INPUT_PER_VERTEX_DATA, 0
		},
		{
			"TEXCOORD",						//�e�N�X�`���ʒu.
			0,
			DXGI_FORMAT_R32G32_FLOAT,		//DXGI�̃t�H�[�}�b�g(32bit float�^*2).
			0,
			12,								//�f�[�^�̊J�n�ʒu.
			D3D11_INPUT_PER_VERTEX_DATA, 0
		}
	};
	//���_�C���v�b�g���C�A�E�g�̔z��v�f�����Z�o.
	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	//���_�C���v�b�g���C�A�E�g���쐬.
	if (FAILED(
		m_pDevice11->CreateInputLayout(
			layout,
			numElements,
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			&m_pVertexLayout)))	//(out)���_�C���v�b�g���C�A�E�g.
	{
		_ASSERT_EXPR(false, _T("���_�C���v�b�g���C�A�E�g�쐬���s"));
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);

	//HLSL����s�N�Z���V�F�[�_�̃u���u���쐬.
	if (FAILED(
		D3DX11CompileFromFile(
			SHADER_NAME,		//�V�F�[�_�t�@�C�����iHLSL�t�@�C���j.
			nullptr,			//�}�N����`�̔z��ւ̃|�C���^�i���g�p�j.
			nullptr,			//�C���N���[�h�t�@�C���������C���^�[�t�F�C�X�ւ̃|�C���^�i���g�p�j.
			"PS_Main",			//�V�F�[�_�G���g���[�|�C���g�֐��̖��O.
			"ps_5_0",			//�V�F�[�_�̃��f�����w�肷�镶����i�v���t�@�C���j.
			uCompileFlag,		//�V�F�[�_�R���p�C���t���O.
			0,					//�G�t�F�N�g�R���p�C���t���O�i���g�p�j.
			nullptr,			//�X���b�h �|���v �C���^�[�t�F�C�X�ւ̃|�C���^�i���g�p�j.
			&pCompiledShader,	//�u���u���i�[���郁�����ւ̃|�C���^.
			&pErrors,			//�G���[�ƌx���ꗗ���i�[���郁�����ւ̃|�C���^.
			nullptr)))			//�߂�l�ւ̃|�C���^�i���g�p�j.
	{
		_ASSERT_EXPR(false, _T("hlsl�ǂݍ��ݎ��s"));
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	//��L�ō쐬�����u���u����u�s�N�Z���V�F�[�_�v���쐬.
	if (FAILED(
		m_pDevice11->CreatePixelShader(
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			nullptr,
			&m_pPixelShader)))	//(out)�s�N�Z���V�F�[�_.
	{
		_ASSERT_EXPR(false, _T("�s�N�Z���V�F�[�_�쐬���s"));
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);

	//�R���X�^���g�i�萔�j�o�b�t�@�쐬.
	//�V�F�[�_�ɓ���̐��l�𑗂�o�b�t�@.
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;		//�R���X�^���g�o�b�t�@���w��.
	cb.ByteWidth = sizeof(SHADER_CONSTANT_BUFFER);	//�R���X�^���g�o�b�t�@�̃T�C�Y.
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;		//�������݂ŃA�N�Z�X.
	cb.MiscFlags = 0;	//���̑��̃t���O�i���g�p�j.
	cb.StructureByteStride = 0;	//�\���̂̃T�C�Y�i���g�p�j.
	cb.Usage = D3D11_USAGE_DYNAMIC;	//�g�p���@�F���ڏ�������.

	//�R���X�^���g�o�b�t�@�̍쐬.
	if (FAILED(
		m_pDevice11->CreateBuffer(
			&cb,
			nullptr,
			&m_pConstantBuffer)))
	{
		_ASSERT_EXPR(false, _T("�R���X�^���g�o�b�t�@�쐬���s"));
		return E_FAIL;
	}


	return S_OK;
}

//���f���쐬.
HRESULT CFont::CreateModel()
{

}

//�e�N�X�`���쐬.
HRESULT CFont::CreateTexture(LPCTSTR lpFileName)
{
	//�e�N�X�`���쐬.
	if (FAILED(D3DX11CreateShaderResourceViewFromFile(
		m_pDevice11,		//���\�[�X���g�p����f�o�C�X�̃|�C���^.
		lpFileName,			//�t�@�C����.
		nullptr, nullptr,
		&m_pTexture,		//(out)�e�N�X�`��.
		nullptr)))
	{
		_ASSERT_EXPR(false, _T("�e�N�X�`���쐬���s"));
		return E_FAIL;
	}

	return S_OK;
}

//�T���v���쐬.
HRESULT CFont::CreateSampler()
{

}

HRESULT CFont::LoadFontData(LPCTSTR lpFileName)
{
	ifstream fin;
	int i;
	char temp;


	// Create the font spacing buffer.
	m_Font = new FONTTYPE[95];

	// Read in the font size and spacing between chars.
	fin.open(lpFileName);
	if (fin.fail())
	{
		return false;
	}

	// Read in the 95 used ascii characters for text.
	for (i = 0; i < 95; i++)
	{
		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}
		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}

		fin >> m_Font[i].left;
		fin >> m_Font[i].right;
		fin >> m_Font[i].size;
	}

	// Close the file.
	fin.close();

	return S_OK;
}

//�t�H���g�����_�����O
void CFont::RenderFont(int FontIndex, float x, float y)
{

}

//���.
void CFont::Release()
{

}