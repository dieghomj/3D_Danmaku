/*********************************************************************
*	MSDF Font Shader
*	Multi-channel Signed Distance Field for high-quality text rendering
*	Supports both SDF (single-channel) and MSDF (multi-channel) modes
**/

Texture2D g_Texture : register(t0);
SamplerState g_samLinear : register(s0);

cbuffer per_mesh : register(b0)
{
    matrix g_mWorld : packoffset(c0);
    float4 g_Color : packoffset(c4); // RGBA (alpha used for global transparency)
    float g_ViewPortW : packoffset(c5);
    float g_ViewPortH : packoffset(c6);
    float g_PxRange : packoffset(c7); // Distance field range in pixels (typically 4-8)
    // Note: Add padding if needed for 16-byte alignment
};

struct VS_OUTPUT
{
    float4 Pos : SV_Position;
    float2 UV : TEXCOORD0;
};

// Vertex shader (unchanged)
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

// Helper function: Calculate median of three values
// This is the key to MSDF rendering - reconstructs true distance from RGB channels
float median(float r, float g, float b)
{
    return max(min(r, g), min(max(r, g), b));
}

// Pixel shader for MSDF rendering
float4 PS_Main(VS_OUTPUT input) : SV_Target
{
    // Sample all three channels (RGB contain distance information)
    float3 msd = g_Texture.Sample(g_samLinear, input.UV).rgb;
    
    // Calculate median distance (reconstructs the true signed distance)
    float sd = median(msd.r, msd.g, msd.b);
    
    // Calculate screen-space distance
    // fwidth gives us the rate of change, allowing scale-independent rendering
    float2 unitRange = fwidth(input.UV); // Texture size (adjust if needed)
    float screenPxDistance = (sd - 0.2) * g_PxRange / length(unitRange);
    
    // Calculate opacity with smooth antialiasing
    float opacity = clamp(screenPxDistance + 0.2, 0.0, 1.0);
    
    // Mix with fallback for single-channel SDF (uses red channel only)
    // Uncomment the following lines if you want automatic fallback:
    //float sdfOpacity = smoothstep(0.5 - fwidth(sd) * 0.5, 0.5 + fwidth(sd) * 0.5, sd);
    //opacity = lerp(sdfOpacity, opacity, 1.0); // Use MSDF by default
    
    // Combine text color with calculated opacity
    float4 outColor = float4(g_Color.rgb, g_Color.a * (1-opacity));
    
    return outColor;
}

// Alternative: Single-channel SDF fallback pixel shader
// Use this if you want to support both SDF and MSDF textures
float4 PS_Main_SDF(VS_OUTPUT input) : SV_Target
{
    // Read distance from red channel only
    float dist = g_Texture.Sample(g_samLinear, input.UV).r;
    
    // Calculate smoothing width based on screen-space derivatives
    float smoothing = fwidth(dist) * 0.4;
    
    // Create smooth alpha transition around edge (0.5 threshold)
    float alpha = smoothstep(0.4 - smoothing, 0.4 + smoothing, dist);
    
    // Combine text color with calculated alpha
    float4 outColor = float4(g_Color.rgb, g_Color.a * alpha);
    
    return outColor;
}