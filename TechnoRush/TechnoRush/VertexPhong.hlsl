
cbuffer perModel : register(b0)
{
	matrix world;
	matrix view;
	matrix projection;
	float4 color;
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
	float3 normal		: NORMAL0;
	float3 worldPos		: POSITION0;
};

VertexToPixel main(VertexShaderInput input)
{
	VertexToPixel output;

	matrix worldViewProj = mul(mul(world, view), projection);
	output.position = mul(float4(input.position, 1.0f), worldViewProj);
	output.worldPos = output.position;

	output.color = input.color * float4(color.xyz, 1.0f);
	float3 newNormal = float3(input.normal.xy, -input.normal.z);
	output.normal = mul(newNormal, (float3x3)world);
	output.uv = float2(input.uv.x, -input.uv.y);

	return output;
}

