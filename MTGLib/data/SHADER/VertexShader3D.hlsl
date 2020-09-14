cbuffer cbBuffer0 : register(b0)
{
	matrix WVP;
    matrix World;
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
	float4 posAAA	: SV_POSITION;
    float4 pos		: POSITION0;
	float2 uv		: TEXCOORD0;
	float4 nmlw		: TEXCOORD1;
};

void main( in VS_IN vi, out VS_OUT vo ) 
{	
	vo.posAAA = mul(vi.pos, WVP);
    vo.pos = mul(vi.pos, World);
    vo.nmlw = mul(vi.nml, World);
	vo.uv = vi.uv;
	
}