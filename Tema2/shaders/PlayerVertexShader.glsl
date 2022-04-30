#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec2 tex_coord;
layout(location = 2) in vec3 v_normal;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 v_color;

// Output
// TODO(student): Output values to fragment shader
out vec3 frag_color;

void main()
{
    // TODO(student): Send output to fragment shader
    frag_color = v_color;

    // TODO(student): Compute gl_Position
    gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}