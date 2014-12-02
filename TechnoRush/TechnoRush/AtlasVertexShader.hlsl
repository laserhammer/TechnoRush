
cbuffer perModel : register(b0)
{
	matrix world;
	matrix view;
	matrix projection;
	float4 color;
};

cbuffer atlas : register(b2)
{
	float2 displacement;
	float2 scale;
};

struct VertexShaderInput
{
	float3 position		: POSITION;
	float4 color		: COLOR;
	float2 uv			: TEXCOORD0;
	float3 normal		: NORMAL;
};

struct VertexToPixel
{
	float4 position		: SV_POSITION;	
	float4 color		: COLOR;
	float2 uv			: TEXCOORD0;
};


VertexToPixel main(VertexShaderInput input)
{

	VertexToPixel output;

	matrix worldViewProj = mul(mul(world, view), projection);
	output.position = mul(float4(input.position, 1.0f), worldViewProj);

	output.color = input.color * float4(color.xyz, 1.0f);
	float2 outputUV = float2(input.uv.x * scale.x + displacement.x, input.uv.y * scale.y + displacement.y);
	output.uv = outputUV;

	return output;
}