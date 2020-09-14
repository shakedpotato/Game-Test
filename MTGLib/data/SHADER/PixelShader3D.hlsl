
struct PS_IN
{
    float4 posAAA   : SV_POSITION;
    float4 pos      : POSITION0;
    float2 uv       : TEXCOORD0;
    float4 nmlw     : TEXCOORD1;
};


Texture2D       myTex       : register( t0 );
SamplerState    mySampler   : register( s0 );

float4 main(in PS_IN pi ) : SV_TARGET
{
    float3 lightPos = float3(100.0f, 100.0f, 100.0f);
    float3 vecFromLight = lightPos - pi.pos.xyz;
    vecFromLight = normalize(vecFromLight);
    float3 normal = normalize(pi.nmlw);
    
    float3 diffuse = (max(dot(normal, vecFromLight), 0.0f) * 0.5f + 0.5f);
	
    float4 color = myTex.Sample(mySampler, pi.uv) * float4(diffuse, 1.0f);
    return color;
    // return float4(1.0f, 0.0f, 0.0f, 1.0f);
}