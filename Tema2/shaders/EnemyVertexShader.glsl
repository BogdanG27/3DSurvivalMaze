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

float rand(vec2 co){
  return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main()
{
    // TODO(student): Send output to fragment shader
    frag_color = v_color;
    vec3 newPosition;
    newPosition = v_position + rand(tex_coord) * v_normal;
    gl_Position = Projection * View * Model * vec4(newPosition, 1.0 );
    
    // TODO(student): Compute gl_Position
}