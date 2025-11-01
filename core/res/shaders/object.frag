#version 330 core

in vec2 texture_coords;
out vec4 frag_colour;

uniform sampler2D object_texture;
uniform bool is_textured;
uniform bool is_animated;

uniform int columns;
uniform int rows;

uniform vec4 colour;
uniform int index;

void main() {
  vec2 pos = vec2(index % int(columns), int(index / columns) * -1);
  
  if (is_textured) {
    if (!is_animated) frag_colour = colour * texture(object_texture, texture_coords);
    else              frag_colour = colour * texture(object_texture, vec2((texture_coords.x / columns) + pos.x * (1.0f / columns), (texture_coords.y / rows) + pos.y * (1.0f / rows)));
  } else {
    frag_colour = colour;
  }
}
