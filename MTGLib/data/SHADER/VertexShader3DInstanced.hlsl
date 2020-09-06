cbuffer cbBuffer0 : register(b0)
{
	matrix WVP;
};

struct VS_IN
{
	float3 pos		: POSITION0;
	float3 nml		: NORMAL0;
	float4 color	: COLOR0;
	float2 uv		: TEXCOORD0;
    
	float3 instancePos : TEXCOORD1;
	 uint instatnceID : SV_InstanceID;
};

struct VS_OUT
{
	float4 pos		: SV_POSITION;
	float2 uv		: TEXCOORD0;
	float4 nmlw		: TEXCOORD2;
};

void main( in VS_IN vi, out VS_OUT vo ) 
{	
	vi.pos += vi.instancePos;
	vo.pos = mul(float4(vi.pos.xyz, 1.0f), WVP);
	vo.uv = vi.uv;
	
}