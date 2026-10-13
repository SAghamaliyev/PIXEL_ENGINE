#version 330 core

out vec4 fragColor;

in vec2 texCoord;

uniform sampler2D texture2D;
uniform vec4 color;
uniform bool isColorTintEnabled;

void main() {
    vec4 textureColor = texture(texture2D, texCoord);

    if (isColorTintEnabled) {
        fragColor = vec4(textureColor.rgb * color.rgb, color.a);
    } else {
        fragColor = vec4(textureColor.rgb, 1.0f);
    }
}
