//定数バッファ
cbuffer myCbuffer : register(b0)
{
    //水平演算指定 column_major → デフォルト（OpenGL用と思われる）
    //並列積和指定 row_major（DirectX標準）→こちらで基本は正しく動く
    float4x4 mat;
}

struct VS_INPUT
{
    float3 inPos : POSITION;
    //float3 inColor : COLOR;
    float2 inTexcoord : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 outPosition : SV_POSITION;
    //float3 outColor : COLOR;
    float2 outTexcoord : TEXCOORD;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    output.outPosition = mul(float4(input.inPos, 1.0f), mat);
    output.outTexcoord = input.inTexcoord;
    return output;
}