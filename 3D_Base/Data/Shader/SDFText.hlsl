/*********************************************************************
*	SDF Font Shader
*	Uses signed distance field for high-quality scalable text rendering
**/

Texture2D g_Texture : register(t0);
SamplerState g_samLinear : register(s0);

cbuffer per_mesh : register(b0)
{
    matrix g_mWorld : packoffset(c0);
    float4 g_Color : packoffset(c4); // RGBA (alpha used for global transparency)
    float g_ViewPortW : packoffset(c5);
    float g_ViewPortH : packoffset(c6);
};

struct VS_OUTPUT
{
    float4 Pos : SV_Position;
    float2 UV : TEXCOORD0;
};

// Vertex shader
VS_OUTPUT VS_Main(
	float4 Pos : POSITION,
	float2 UV : TEXCOORD)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    output.Pos = mul(Pos, g_mWorld);
	
	// Convert to NDC (Normalized Device Coordinates)
    output.Pos.x = (output.Pos.x / g_ViewPortW) * 2.f - 1.f;
    output.Pos.y = 1.f - (output.Pos.y / g_ViewPortH) * 2.f;
	
    output.UV = UV;
	
    return output;
}

// Pixel shader for SDF rendering
float4 PS_Main(VS_OUTPUT input) : SV_Target
{
	// Read distance from red channel
	// SDF texture should encode distance where 0.5 = glyph edge
    float dist = g_Texture.Sample(g_samLinear, input.UV).r;
	
	// Calculate smoothing width based on screen-space derivatives
	// This provides automatic antialiasing at any scale
    float smoothing = fwidth(dist) * 0.5; // Adjust multiplier for sharpness (0.3-1.0)
	
	// Create smooth alpha transition around edge (0.5 threshold)
    float alpha = smoothstep(0.5 - smoothing, 0.5 + smoothing, dist);
	
	// Combine text color with calculated alpha
    float4 outColor = float4(g_Color.rgb, g_Color.a * alpha);
	
    return outColor;
}