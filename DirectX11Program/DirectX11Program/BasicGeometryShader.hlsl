struct VSOutput
{
	float4 position : SV_POSITION;
	float4 color : COLOR0;
};

struct GSOutput
{
	float4 position : SV_POSITION;
	float4 color : COLOR0;
};

[maxvertexcount(3)]
void main(
	triangle VSOutput input[3],
	inout TriangleStream< GSOutput > output
)
{
	[unroll]
	for (uint i = 0; i < 3; i++)
	{
		GSOutput element;
		element.position = input[i].position;
		element.color = input[i].color;
		output.Append(element);
	}
}