// Source code of fragment shader
#version 130

// Attributes passed from the vertex shader
in vec4 color_interp;
in vec2 uv_interp;

uniform int ghost;

// Texture sampler
uniform sampler2D onetex;

void main()
{
    // Sample texture
    vec4 color = texture2D(onetex, uv_interp);


    if(ghost == 1) {
        gl_FragColor = vec4(color.r, color.r, color.r, 1);
    }
    else if(ghost == 0) {
        // Assign color to fragment
        gl_FragColor = vec4(color.r, color.g, color.b, color.a);
    }


    // Check for transparency
    if(color.a < 1.0)
    {
         discard;
    }
}
