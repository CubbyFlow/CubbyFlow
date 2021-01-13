#define PI        3.14159265358979323
#define TWO_PI    6.28318530717958648
#define INFINITY  1000000.0
#define EPS 0.00001


struct Material
{
    vec3 albedo; 
    float metallic;
    float roughness;
    float ao;
};

struct Camera 
{ 
    mat4 viewProjection;
    vec3 position;
};

struct Light 
{ 
    vec3 position; 
    vec3 color;
};