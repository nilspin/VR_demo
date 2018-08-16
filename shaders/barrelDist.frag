#version 430

uniform sampler2D imageTexture;
in vec2 UV;
out vec4 outColor;

float pow2(float d)
{
	return d*d;
}

float pow4(float d)
{
	return pow2(d);
}

vec2 barrel(vec2 o, float d)
{
	vec2 temp = o*(1.0 + 0.22*pow2(d) + 0.25*pow4(d));
	temp = 0.5*(temp+1);
	return temp;
}

void main()
{
	vec2 temp;
	vec2 xy = 2.0*UV - vec2(1);
	float d = length(xy);
	if(d<1.0)
	{
		temp = barrel(xy, d);
	}
	else
	{
		temp = xy;
	}

	outColor = texture(imageTexture, temp);
}