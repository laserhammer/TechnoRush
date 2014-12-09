cbuffer stage : register(b3)
{
	unsigned int stage;
	float blur;
	float2 filler;
};

struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float4 color		: COLOR;
	float2 uv			: TEXCOORD0;
};

Texture2D myTexture : register(t0);
Texture2D base : register(t2);
SamplerState TrilinearSampler : register(s0);

float4 main(VertexToPixel input) : SV_TARGET
{
	float2 dir = float2((input.uv.x - 0.5f) * 2.0f, (input.uv.y - 0.5f) * 2.0f);
	float2 texCoord = input.uv;

	//this will be our RGBA sum
	float4 sum = float4(0.0, 0.0, 0.0, 0.0);

	//the amout to blur, i.e. how far off center to sampel from 
	//1.0 -> blur by one pixel
	//2.0 -> blur by two pixels etc.
	float radius = blur;
	float blur = radius / 1000.0f;

	//the direction of our blur
	//(1.0, 0.0) -> x-axis blur
	//(0.0, 1.0) -> y-axis blur
	float hstep = dir.x;
	float vstep = dir.y;

	//apply blurring, using a 9-tap filter with predefined gaussian weights
	sum += myTexture.Sample(TrilinearSampler, float2(texCoord.x - 8.0f*blur*hstep, texCoord.y - 8.0*blur*vstep));// * 0.0162162162;
	sum += myTexture.Sample(TrilinearSampler, float2(texCoord.x - 7.0f*blur*hstep, texCoord.y - 7.0*blur*vstep));// * 0.0540540541;
	sum += myTexture.Sample(TrilinearSampler, float2(texCoord.x - 6.0f*blur*hstep, texCoord.y - 6.0*blur*vstep));// * 0.1216216216;
	sum += myTexture.Sample(TrilinearSampler, float2(texCoord.x - 5.0f*blur*hstep, texCoord.y - 5.0*blur*vstep));// * 0.1945945946;

	sum += myTexture.Sample(TrilinearSampler, float2(texCoord.x - 4.0f*blur*hstep, texCoord.y - 4.0*blur*vstep));// * 0.0162162162;
	sum += myTexture.Sample(TrilinearSampler, float2(texCoord.x - 3.0f*blur*hstep, texCoord.y - 3.0*blur*vstep));// * 0.0540540541;
	sum += myTexture.Sample(TrilinearSampler, float2(texCoord.x - 2.0f*blur*hstep, texCoord.y - 2.0*blur*vstep));// * 0.1216216216;
	sum += myTexture.Sample(TrilinearSampler, float2(texCoord.x - 1.0f*blur*hstep, texCoord.y - 1.0*blur*vstep));// * 0.1945945946;

	sum += myTexture.Sample(TrilinearSampler, texCoord);// * 0.2270270270;
	sum /= 9.0f;
	//sum += myTexture.Sample(TrilinearSampler, float2(texCoord.x + 1.0f*blur*hstep, texCoord.y + 1.0*blur*vstep)) * 0.1945945946;
	//sum += myTexture.Sample(TrilinearSampler, float2(texCoord.x + 2.0f*blur*hstep, texCoord.y + 2.0*blur*vstep)) * 0.1216216216;
	//sum += myTexture.Sample(TrilinearSampler, float2(texCoord.x + 3.0f*blur*hstep, texCoord.y + 3.0*blur*vstep)) * 0.0540540541;
	//sum += myTexture.Sample(TrilinearSampler, float2(texCoord.x + 4.0f*blur*hstep, texCoord.y + 4.0*blur*vstep)) * 0.0162162162;

	//multiply by vertex color and return
	return float4(sum.r, sum.g, sum.b, 1.0);
}