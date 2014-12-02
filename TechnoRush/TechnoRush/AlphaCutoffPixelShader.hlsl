
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float4 color		: COLOR;
	float2 uv			: TEXCOORD0;
};

Texture2D myTexture : register(t0);
SamplerState TrilinearSampler : register(s0);

float4 main(VertexToPixel input) : SV_TARGET
{
	float4 texSample = input.color * myTexture.Sample(TrilinearSampler, input.uv);
	clip(texSample.w < 0.1f ? -1:1);
	return texSample;
}