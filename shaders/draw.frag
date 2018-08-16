#version 430

uniform sampler2D imageTexture;
in vec2 UV;
out vec4 outColor;

void main()
{
	outColor = texture(imageTexture, UV);
	//outColor = vec4(texture(imageTexture, UV.xy).xyz, 1.0);
	//outColor = UV.xyyy;
}