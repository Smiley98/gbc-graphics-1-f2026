#version 430 core

in vec3 position;
in vec3 normal;
in vec2 tcoord;

out vec4 frag_colour;

void main()
{
    // Identity view and proj transforms, so position is in NDC [-1..1] range
    vec3 pos_ndc = position;

    vec3 color_space = pos_ndc * 0.5 + 0.5; // [-1..1] --> [0..1]
    frag_colour = vec4(pos_ndc, 1.0);
}
