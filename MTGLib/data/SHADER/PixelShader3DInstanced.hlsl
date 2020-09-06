
struct PS_IN
{
	float4 pos		: SV_POSITION;
	float2 uv		: TEXCOORD0;
	float4 nmlw		: TEXCOORD2;
};


Texture2D       myTex       : register( t0 );
SamplerState    mySampler   : register( s0 );

float4 main(in PS_IN pi ) : SV_TARGET
{
	float4 color = myTex.Sample( mySampler, pi.uv ); 
    return color;
    // return float4(1.0f, 0.0f, 0.0f, 1.0f);
}