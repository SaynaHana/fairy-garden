// Source code of fragment shader
#version 130

// Attributes passed from the vertex shader
in vec4 color_interp;
in vec2 uv_interp;

// Texture sampler
uniform sampler2D onetex;

void main()
{
    // Sample texture
    // CHANGE: This uses uv coordinates which are much larger which makes the texture
    // Repeat a lot more
    vec4 color = texture2D(onetex, uv_interp * 16);

    // Assign color to fragment
    gl_FragColor = vec4(color.r, color.g, color.b, color.a);

    // Check for transparency
    if(color.a < 1.0)
    {
         discard;
    }
}
