cbuffer cbBuffer0 : register(b0)
{
	matrix WVP;
};

struct VS_IN
{
	float4 pos		: POSITION0;
	float4 nml		: NORMAL0;
	float4 color	: COLOR0;
	float2 uv		: TEXCOORD0;
};

struct VS_OUT
{
	float4 pos		: SV_POSITION;
	float2 uv		: TEXCOORD0;
	float4 nmlw		: TEXCOORD2;
};

void main( in VS_IN vi, out VS_OUT vo ) 
{	
	vo.pos = mul(vi.pos, WVP);
	vo.uv = vi.uv;
	
}