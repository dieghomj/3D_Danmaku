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
	m_pDevice11 = m_pDx11->GetDevice();		//実態は別のところにある.他とも共有している.
	m_pContext11 = m_pDx11->GetContext();	//実態は別のところにある.他とも共有している.

	//シェーダ作成.
	if (FAILED(CreateShader()))
	{
		return E_FAIL;
	}
	//板ポリゴン作成.
	if (FAILED(CreateModel()))
	{
		return E_FAIL;
	}
	if (FAILED(LoadFontData(fontFilename)))
	{
		return E_FAIL;
	}

	//テクスチャ作成.
	if (FAILED(CreateTexture(fontFileTexture)))
	{
		return E_FAIL;
	}
	//サンプラ作成.
	if (FAILED(CreateSampler()))
	{
		return E_FAIL;
	}

	return S_OK;
}

void CFont::Render(LPCTSTR text, int x, int y)
{

}

//シェーダ作成.
HRESULT CFont::CreateShader()
{
	ID3DBlob* pCompiledShader = nullptr;
	ID3DBlob* pErrors = nullptr;
	UINT uCompileFlag = 0;
#ifdef _DEBUG
	uCompileFlag =
		D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION;
#endif//#ifdef _DEBUG

	//HLSLからバーテックスシェーダのブロブを作成.
	if (FAILED(
		D3DX11CompileFromFile(
			SHADER_NAME,	//シェーダファイル名（HLSLファイル）.
			nullptr,		//マクロ定義の配列へのポインタ（未使用）.
			nullptr,		//インクルードファイルを扱うインターフェイスへのポインタ（未使用）.
			"VS_Main",		//シェーダエントリーポイント関数の名前.
			"vs_5_0",		//シェーダのモデルを指定する文字列（プロファイル）.
			uCompileFlag,	//シェーダコンパイルフラグ.
			0,				//エフェクトコンパイルフラグ（未使用）.
			nullptr,		//スレッド ポンプ インターフェイスへのポインタ（未使用）.
			&pCompiledShader,//ブロブを格納するメモリへのポインタ.
			&pErrors,		//エラーと警告一覧を格納するメモリへのポインタ.
			nullptr)))		//戻り値へのポインタ（未使用）.
	{
		_ASSERT_EXPR(false, _T("hlsl読み込み失敗"));
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	//上記で作成したブロブから「バーテックスシェーダ」を作成.
	if (FAILED(
		m_pDevice11->CreateVertexShader(
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			nullptr,
			&m_pVertexShader)))	//(out)バーテックスシェーダ.
	{
		_ASSERT_EXPR(false, _T("バーテックスシェーダ作成失敗"));
		return E_FAIL;
	}

	//頂点インプットレイアウトを定義.
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{
			"POSITION",						//位置.
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,	//DXGIのフォーマット(32bit float型*3).
			0,
			0,								//データの開始位置.
			D3D11_INPUT_PER_VERTEX_DATA, 0
		},
		{
			"TEXCOORD",						//テクスチャ位置.
			0,
			DXGI_FORMAT_R32G32_FLOAT,		//DXGIのフォーマット(32bit float型*2).
			0,
			12,								//データの開始位置.
			D3D11_INPUT_PER_VERTEX_DATA, 0
		}
	};
	//頂点インプットレイアウトの配列要素数を算出.
	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	//頂点インプットレイアウトを作成.
	if (FAILED(
		m_pDevice11->CreateInputLayout(
			layout,
			numElements,
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			&m_pVertexLayout)))	//(out)頂点インプットレイアウト.
	{
		_ASSERT_EXPR(false, _T("頂点インプットレイアウト作成失敗"));
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);

	//HLSLからピクセルシェーダのブロブを作成.
	if (FAILED(
		D3DX11CompileFromFile(
			SHADER_NAME,		//シェーダファイル名（HLSLファイル）.
			nullptr,			//マクロ定義の配列へのポインタ（未使用）.
			nullptr,			//インクルードファイルを扱うインターフェイスへのポインタ（未使用）.
			"PS_Main",			//シェーダエントリーポイント関数の名前.
			"ps_5_0",			//シェーダのモデルを指定する文字列（プロファイル）.
			uCompileFlag,		//シェーダコンパイルフラグ.
			0,					//エフェクトコンパイルフラグ（未使用）.
			nullptr,			//スレッド ポンプ インターフェイスへのポインタ（未使用）.
			&pCompiledShader,	//ブロブを格納するメモリへのポインタ.
			&pErrors,			//エラーと警告一覧を格納するメモリへのポインタ.
			nullptr)))			//戻り値へのポインタ（未使用）.
	{
		_ASSERT_EXPR(false, _T("hlsl読み込み失敗"));
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	//上記で作成したブロブから「ピクセルシェーダ」を作成.
	if (FAILED(
		m_pDevice11->CreatePixelShader(
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			nullptr,
			&m_pPixelShader)))	//(out)ピクセルシェーダ.
	{
		_ASSERT_EXPR(false, _T("ピクセルシェーダ作成失敗"));
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);

	//コンスタント（定数）バッファ作成.
	//シェーダに特定の数値を送るバッファ.
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;		//コンスタントバッファを指定.
	cb.ByteWidth = sizeof(SHADER_CONSTANT_BUFFER);	//コンスタントバッファのサイズ.
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;		//書き込みでアクセス.
	cb.MiscFlags = 0;	//その他のフラグ（未使用）.
	cb.StructureByteStride = 0;	//構造体のサイズ（未使用）.
	cb.Usage = D3D11_USAGE_DYNAMIC;	//使用方法：直接書き込み.

	//コンスタントバッファの作成.
	if (FAILED(
		m_pDevice11->CreateBuffer(
			&cb,
			nullptr,
			&m_pConstantBuffer)))
	{
		_ASSERT_EXPR(false, _T("コンスタントバッファ作成失敗"));
		return E_FAIL;
	}


	return S_OK;
}

//モデル作成.
HRESULT CFont::CreateModel()
{

}

//テクスチャ作成.
HRESULT CFont::CreateTexture(LPCTSTR lpFileName)
{
	//テクスチャ作成.
	if (FAILED(D3DX11CreateShaderResourceViewFromFile(
		m_pDevice11,		//リソースを使用するデバイスのポインタ.
		lpFileName,			//ファイル名.
		nullptr, nullptr,
		&m_pTexture,		//(out)テクスチャ.
		nullptr)))
	{
		_ASSERT_EXPR(false, _T("テクスチャ作成失敗"));
		return E_FAIL;
	}

	return S_OK;
}

//サンプラ作成.
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

//フォントレンダリング
void CFont::RenderFont(int FontIndex, float x, float y)
{

}

//解放.
void CFont::Release()
{

}