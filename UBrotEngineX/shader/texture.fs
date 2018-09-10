////////////////////////////////////////////////////////////////////////////////
// Filename: texture.fs
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////
Texture2D shader_tex : register(t0);


///////////////////
// SAMPLE STATES //
///////////////////
SamplerState sample_state : register(s0);


//////////////
// TYPEDEFS //
//////////////
struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
};


////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 TextureFragmentShader(PixelInputType input) : SV_TARGET
{
    float4 texture_color;

    texture_color = shader_tex.Sample(sample_state, input.uv);

    return texture_color;
}
