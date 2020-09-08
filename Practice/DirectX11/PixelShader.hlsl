cbuffer alphaBuffer : register(b0)
{
    float alpha;
}

struct PS_INPUT
{
    float4 inPosition : SV_POSITION;
    //float3 inColor : COLOR;
    float2 inTexcood : TEXCOORD0;
};

Texture2D objTexture : TEXTURE : register(t0); //registerの数値は開始スロット
SamplerState objSamplerState : SAMPLER : register(s0); //registerの数値は開始スロット

float4 main(PS_INPUT input) : SV_TARGET
{
    float3 pixelColor = objTexture.Sample(objSamplerState, input.inTexcood);
    return float4(pixelColor, alpha);
}