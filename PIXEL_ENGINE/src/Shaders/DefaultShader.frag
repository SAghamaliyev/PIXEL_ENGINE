#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D OurTexture2D;
uniform vec4 OurColor;
uniform bool isColorActive;

void main()
{
	vec4 textureColor = texture(OurTexture2D, TexCoord);

	if (isColorActive) {
		FragColor = vec4(textureColor.rgb * OurColor.rgb, OurColor.a);
	}
	else {
		FragColor = vec4(textureColor.rgb, 1.0);
	}
}
