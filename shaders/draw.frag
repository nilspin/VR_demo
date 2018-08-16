#version 430

uniform sampler2D imageTexture;
in vec2 texCoord;
out vec4 outColor;

void main()
{
	//vec3 color = //texture(imageTexture, texCoord).xyz;
	outColor = texCoord.xyyy;//vec4(color, 1.0f);
}