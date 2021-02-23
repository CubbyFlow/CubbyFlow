#define PI        3.14159265358979323
#define TWO_PI    6.28318530717958648
#define INFINITY  1000000.0
#define EPS 0.001


struct Material
{
    vec3 albedo;
    float metallic;
    float roughness;
    float ao;
    float refractionRatio;
};

struct Camera
{
    mat4 view;
    mat4 projection;
    mat4 model;
    vec3 position;
};

struct Light
{
    vec3 position;
    vec3 color;
};