#version 330 core

in struct fragment_data
{
    vec4 position;
    vec4 normal;
    vec4 color;
    vec2 texture_uv;
    vec3 light_position;
} fragment;

uniform sampler2D texture_sampler;
uniform sampler2D shadow_sampler;

out vec4 FragColor;

uniform vec3 camera_position;
uniform vec3 color; // object color
uniform float ambiant  = 0.2;
uniform float diffuse  = 0.8;
uniform float specular = 0.5;

uniform vec3 light;
//vec3 light = 10 * camera_position;

float bias = 0.000001;

void main()
{
    vec3 n = normalize(fragment.normal.xyz);
    vec3 u = normalize(light-fragment.position.xyz);
    vec3 r = reflect(u,n);
    vec3 t = normalize(fragment.position.xyz-camera_position);

    float min_depth = texture(shadow_sampler, fragment.light_position.xy).r;
    float depth = fragment.light_position.z;
    float brightness = (depth <= min_depth+bias) ? 1.0f : 0.0f;

    float diffuse_value  = brightness * diffuse * clamp( dot(u,n), 0.0, 1.0);
    float specular_value = brightness * specular * pow( clamp( dot(r,t), 0.0, 1.0), 128.0);


    vec3 white = vec3(1.0);
    vec4 color_texture = texture(texture_sampler, fragment.texture_uv);
    vec3 c = (ambiant+diffuse_value)*color.rgb*fragment.color.rgb*color_texture.rgb + specular_value*white;

    FragColor = vec4(c, color_texture.a*fragment.color.a);
}
