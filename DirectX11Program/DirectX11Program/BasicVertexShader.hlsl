cbuffer ConstantBuffer
{
	matrix World;         //���[���h�ϊ��s��
	matrix View;          //�r���[�ϊ��s��
	matrix Projection;    //�����ˉe�ϊ��s��
	matrix WorldViewProjection;	// WVP�s��
}

struct VSInput
{
	float4 position : POSITION;		// ���[�J�����W
	float3 normal : NORMAL;		// �@���f�[�^
};

struct VSOutput
{
	float4 position : SV_POSITION;	// ���[���h���W
	float4 color : COLOR0;	// �F�f�[�^
};

VSOutput main(VSInput input)
{
	VSOutput output;
	// ���[���h���W�ɕϊ�
	output.position = mul(input.position, WorldViewProjection);
	output.color = float4(1.0f, 0.0f, 0.0f, 0.0f);
	return output;
}
