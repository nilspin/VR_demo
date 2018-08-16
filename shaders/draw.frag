#version 430

//uniform sampler2D imageTexture;
in vec2 texCoord;
out vec4 outColor;

void main()
{
	//outColor = vec4(texture(imageTexture, texCoord.xy).xyz, 1.0);
	outColor = texCoord.xyyy;//vec4(color, 1.0f);
}