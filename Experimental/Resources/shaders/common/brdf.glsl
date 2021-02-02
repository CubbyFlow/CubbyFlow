//-----------------------------------------------------------------------
float DistributionGGX(in vec3 N, in vec3 H, in float a)
//-----------------------------------------------------------------------
{
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float nom    = a2;
    float denom  = (NdotH2 * (a2 - 1.0) + 1.0);
    denom        = PI * denom * denom;
	
    return nom / denom;
}

//-----------------------------------------------------------------------
float GeometrySchlickGGX(in float NdotV, in float k)
//-----------------------------------------------------------------------
{
    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return nom / denom;
}
  
//-----------------------------------------------------------------------
float GeometrySmith(in vec3 N, in vec3 V, in vec3 L, in float k)
//-----------------------------------------------------------------------
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx1 = GeometrySchlickGGX(NdotV, k);
    float ggx2 = GeometrySchlickGGX(NdotL, k);
	
    return ggx1 * ggx2;
}

//-----------------------------------------------------------------------
vec3 fresnelSchlick(in float cosTheta, in vec3 F0)
//-----------------------------------------------------------------------
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}