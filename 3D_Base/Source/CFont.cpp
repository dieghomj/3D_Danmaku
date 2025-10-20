#include "CFont.h"
#include "CDirectX11.h"

const TCHAR SHADER_NAME[] = _T("Data\\Shader\\SDFText.hlsl");

CFont::CFont()
	: m_pDx11(nullptr)
	, m_pDevice11(nullptr)
	, m_pContext11(nullptr)
	, m_pVertexShader(nullptr)
	, m_pVertexLayout(nullptr)
	, m_pPixelShader(nullptr)
	, m_pConstantBuffer(nullptr)
	, m_pVertexBuffer()
	, m_pSampleLinear(nullptr)
	, m_pTexture(nullptr)
	, m_Alpha(1.0f)
	, m_Color(1.f, 1.f, 1.f)
	, m_Kerning()
{
}

CFont::~CFont()
{
	Release();

	// These are managed elsewhere, just nullify
	m_pContext11 = nullptr;
	m_pDevice11 = nullptr;
	m_pDx11 = nullptr;
}

// 初期化関数.
HRESULT CFont::Init(CDirectX11& pDx11)
{
	m_pDx11 = &pDx11;
	m_pDevice11 = m_pDx11->GetDevice();
	m_pContext11 = m_pDx11->GetContext();

	// Create shaders
	if (FAILED(CreateShader()))
	{
		return E_FAIL;
	}

	// Create geometry
	if (FAILED(CreateModel()))
	{
		return E_FAIL;
	}

	// Load SDF texture atlas
	if (FAILED(CreateTexture(_T("Data\\Font\\ascii_sdf.png"))))
	{
		return E_FAIL;
	}

	// Create sampler
	if (FAILED(CreateSampler()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//解放関数.
void CFont::Release()
{
	SAFE_RELEASE(m_pSampleLinear);
	SAFE_RELEASE(m_pTexture);
	for (int i = SPRITE_MAX - 1; i >= 0; i--) {
		SAFE_RELEASE(m_pVertexBuffer[i]);
	}
	SAFE_RELEASE(m_pConstantBuffer);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pVertexLayout);
	SAFE_RELEASE(m_pVertexShader);

	m_pContext11 = nullptr;
	m_pDevice11 = nullptr;
}

// シェーダ作成.
HRESULT CFont::CreateShader()
{
	ID3DBlob* pCompiledShader = nullptr;
	ID3DBlob* pErrors = nullptr;
	UINT uCompileFlag = 0;
#ifdef _DEBUG
	uCompileFlag = D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION;
#endif

	//シェーダーのコンパイル.
	if (FAILED(
		D3DX11CompileFromFile(
			SHADER_NAME,
			nullptr,
			nullptr,
			"VS_Main",
			"vs_5_0",
			uCompileFlag,
			0,
			nullptr,
			&pCompiledShader,
			&pErrors,
			nullptr)))
	{
		_ASSERT_EXPR(false, _T("SDFhlsl読み込み失敗"));
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	// Create vertex shader
	if (FAILED(
		m_pDevice11->CreateVertexShader(
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			nullptr,
			&m_pVertexShader)))
	{
		_ASSERT_EXPR(false, _T("SDFバーテックスシェーダ作成失敗"));
		return E_FAIL;
	}

	// Define input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			0,
			D3D11_INPUT_PER_VERTEX_DATA, 0
		},
		{
			"TEXCOORD",
			0,
			DXGI_FORMAT_R32G32_FLOAT,
			0,
			12,
			D3D11_INPUT_PER_VERTEX_DATA, 0
		}
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	// Create input layout
	if (FAILED(
		m_pDevice11->CreateInputLayout(
			layout,
			numElements,
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			&m_pVertexLayout)))
	{
		_ASSERT_EXPR(false, _T("頂点インプットレイアウト作成失敗"));
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);

	// Compile pixel shader (SDF-aware)
	if (FAILED(
		D3DX11CompileFromFile(
			SHADER_NAME,
			nullptr,
			nullptr,
			"PS_Main",
			"ps_5_0",
			uCompileFlag,
			0,
			nullptr,
			&pCompiledShader,
			&pErrors,
			nullptr)))
	{
		_ASSERT_EXPR(false, _T("SDF hlsl読み込み失敗"));
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	// Create pixel shader
	if (FAILED(
		m_pDevice11->CreatePixelShader(
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			nullptr,
			&m_pPixelShader)))
	{
		_ASSERT_EXPR(false, _T("SDFピクセルシェーダ作成失敗"));
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);

	// Create constant buffer
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(SHADER_CONSTANT_BUFFER);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

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

HRESULT CFont::CreateTexture(LPCTSTR lpFileName)
{
	if (FAILED(D3DX11CreateShaderResourceViewFromFile(
		m_pDevice11,
		lpFileName,
		nullptr, nullptr,
		&m_pTexture,
		nullptr)))
	{
		_ASSERT_EXPR(false, _T("SDF texture loading failed"));
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CFont::CreateModel()
{
	// Constants matching your texture atlas layout
	constexpr float CHAR_W = 32.f;		// Width of each character cell in pixels
	constexpr float CHAR_H = 32.f;		// Height of each character cell in pixels
	constexpr float TEXTURE_W = 320.0f;	// Total texture width
	constexpr float TEXTURE_H = 320.0f;	// Total texture height

	// Calculate how many pixels each character occupies in the atlas
	// For a 10×10 grid on 128×128 texture: each cell is 12.8×12.8 pixels
	constexpr float CELL_W = TEXTURE_W / SPRITE_MAX_W;  // 12.8 pixels
	constexpr float CELL_H = TEXTURE_H / SPRITE_MAX_H;  // 12.8 pixels

	int count = 0;

	// Iterate row-by-row (top-to-bottom, left-to-right)
	// This matches ASCII ordering: space(32) at (0,0), '!'(33) at (1,0), etc.
	for (int y = 0; y < SPRITE_MAX_H; y++)     // Row first
	{
		for (int x = 0; x < SPRITE_MAX_W; x++)  // Column second
		{
			// Set kerning (horizontal advance for rendering)
			m_Kerning[count] = CHAR_W;

			// Calculate pixel coordinates in texture
			float left_px = x * CELL_W;
			float top_px = y * CELL_H;
			float right_px = left_px + CHAR_W;  // Use CHAR_W, not kerning!
			float bottom_px = top_px + CHAR_H;

			// Normalize to UV coordinates [0, 1]
			float left = left_px / TEXTURE_W;
			float top = top_px / TEXTURE_H;
			float right = right_px / TEXTURE_W;
			float bottom = bottom_px / TEXTURE_H;

			// Create quad vertices (triangle strip order)
			// DirectX UV convention: (0,0) = top-left, (1,1) = bottom-right
			VERTEX vertices[] =
			{
				// Position (world space)                UV coordinates
				D3DXVECTOR3(0.0f,   CHAR_H, 0.0f),   D3DXVECTOR2(left,  bottom),  // Bottom-left
				D3DXVECTOR3(0.0f,   0.0f,   0.0f),   D3DXVECTOR2(left,  top),     // Top-left
				D3DXVECTOR3(CHAR_W, CHAR_H, 0.0f),   D3DXVECTOR2(right, bottom),  // Bottom-right
				D3DXVECTOR3(CHAR_W, 0.0f,   0.0f),   D3DXVECTOR2(right, top)      // Top-right
			};
			UINT uVerMax = sizeof(vertices) / sizeof(vertices[0]);

			// Buffer description
			D3D11_BUFFER_DESC bd;
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(VERTEX) * uVerMax;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = 0;
			bd.MiscFlags = 0;
			bd.StructureByteStride = 0;

			// Subresource data
			D3D11_SUBRESOURCE_DATA InitData;
			InitData.pSysMem = vertices;

			// Create vertex buffer
			if (FAILED(m_pDevice11->CreateBuffer(&bd, &InitData, &m_pVertexBuffer[count])))
			{
				_ASSERT_EXPR(false, _T("Vertex buffer creation failed"));
				return E_FAIL;
			}

			count++;
		}
	}

	return S_OK;
}

HRESULT CFont::CreateSampler()
{
	D3D11_SAMPLER_DESC samDesc;
	ZeroMemory(&samDesc, sizeof(samDesc));
	samDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;	// Linear filtering
	samDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;		// CLAMP to avoid wrapping
	samDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samDesc.MipLODBias = 0.0f;
	samDesc.MaxAnisotropy = 1;
	samDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samDesc.MinLOD = 0;
	samDesc.MaxLOD = D3D11_FLOAT32_MAX;

	if (FAILED(m_pDevice11->CreateSamplerState(
		&samDesc, &m_pSampleLinear)))
	{
		_ASSERT_EXPR(false, _T("Sampler creation failed"));
		return E_FAIL;
	}

	return S_OK;
}

void CFont::RenderFont(int FontIndex, float x, float y, float FontSize)
{
	D3DXMATRIX	mWorld;
	D3DXMATRIX	mTrans, mScale;

	// Use consistent scale basis (fixed from original bug)
	float scale = FontSize / SPRITE_MAX_H;

	// Build world matrix
	D3DXMatrixScaling(&mScale, scale, scale, 1.0);
	D3DXMatrixTranslation(&mTrans, x, y, 0.f);
	mWorld = mScale * mTrans;

	// Update constant buffer
	D3D11_MAPPED_SUBRESOURCE pData;
	SHADER_CONSTANT_BUFFER cb;
	if (SUCCEEDED(
		m_pContext11->Map(m_pConstantBuffer,
			0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		D3DXMATRIX m = mWorld;
		D3DXMatrixTranspose(&m, &m);
		cb.mWorld = m;

		cb.vColor = D3DXVECTOR4(
			m_Color.x, m_Color.y, m_Color.z, m_Alpha);

		cb.fViewPortWidth = static_cast<float>(WND_W);
		cb.fViewPortHeight = static_cast<float>(WND_H);

		memcpy_s(pData.pData, pData.RowPitch,
			(void*)(&cb), sizeof(cb));

		m_pContext11->Unmap(m_pConstantBuffer, 0);
	}

	// Set vertex buffer
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	m_pContext11->IASetVertexBuffers(0, 1,
		&m_pVertexBuffer[FontIndex], &stride, &offset);

	// Enable alpha blending
	m_pDx11->SetAlphaBlend(true);

	// Draw quad
	m_pContext11->Draw(4, 0);

	// Disable alpha blending
	m_pDx11->SetAlphaBlend(false);
}

// Render text string
void CFont::Render(LPCTSTR text, int x, int y, float FontSize)
{
	// Set shaders
	m_pContext11->VSSetShader(m_pVertexShader, nullptr, 0);
	m_pContext11->PSSetShader(m_pPixelShader, nullptr, 0);

	// Set constant buffers
	m_pContext11->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	m_pContext11->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);

	// Set input layout
	m_pContext11->IASetInputLayout(m_pVertexLayout);

	// Set primitive topology
	m_pContext11->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// Set texture and sampler
	m_pContext11->PSSetSamplers(0, 1, &m_pSampleLinear);
	m_pContext11->PSSetShaderResources(0, 1, &m_pTexture);

	float fx = static_cast<float>(x);
	float fy = static_cast<float>(y);

	float scale = FontSize / SPRITE_MAX_H;

	// Render each character
	for (int i = 0; i < lstrlen(text); i++)
	{
		TCHAR font = text[i];
		int index = font - 33;	// ASCII offset

		// Render glyph
		RenderFont(index, fx, fy, FontSize);

		// Advance position
		fx += scale * m_Kerning[index];
	}
}



// In CFont.cpp, add a LoadAtlasJSON() method:
HRESULT CFont::LoadAtlasJSON(LPCTSTR jsonPath)
{
	// Read JSON file (use a library or manual parsing)
	// For each glyph in JSON:
	//   GlyphInfo info;
	//   info.uvLeft = atlasBounds.left / atlasWidth;
	//   info.uvTop = atlasBounds.bottom / atlasHeight;
	//   info.uvRight = atlasBounds.right / atlasWidth;
	//   info.uvBottom = atlasBounds.top / atlasHeight;
	//   info.advance = glyph.advance;
	//   m_GlyphMap[glyph.unicode] = info;


	return S_OK;
}