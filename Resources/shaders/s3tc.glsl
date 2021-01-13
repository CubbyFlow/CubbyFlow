#version 330 core
precision highp float;
precision highp int;
const float offset = 128.0 / 255.0;//0.5;
uniform sampler2D ScreenTexture;
out uvec4 retVal;

float saturate(float v) { return clamp(v, -1, 1); }
vec2 saturate(vec2 v) { return clamp(v, -1, 1); }
vec3 saturate(vec3 v) { return clamp(v, -1, 1); }
vec4 saturate(vec4 v) { return clamp(v, -1, 1); }

//-----------------------------------------------------------------------
vec4 toCoCgAY(vec4 rgba)
//-----------------------------------------------------------------------
{
    float Y  = (   rgba.r + 2*rgba.g + rgba.b ) * 0.25;
    float Co = ( 2*rgba.r - 2*rgba.b          ) * 0.25 + offset;
    float Cg = (  -rgba.r + 2*rgba.g - rgba.b ) * 0.25 + offset;

    return vec4(Co, Cg, rgba.a, Y);
}

//-----------------------------------------------------------------------
void ExtractColorBlockCoCgAY(
    out vec4 col[16] ,
    sampler2D image,
    ivec2 coord,
    int mip)
//-----------------------------------------------------------------------
{
    for (int i= 0; i<4; i++) {
        for (int j= 0; j<4; j++) {
            vec4 color= texelFetch(image, coord + ivec2(j,i), mip);
            //color = sqrt(color);
            col[i*4+j] = toCoCgAY(color);
        }
    }
}

void FindMinMaxColorsBox(vec4 block[16] , out vec4 mincol, out vec4 maxcol)
{
    mincol = block[0];
    maxcol = block[0];

    for (int i= 1; i < 16; i++) {
        mincol = min(mincol, block[i]);
        maxcol = max(maxcol, block[i]);
    }
}

vec3 RoundAndExpand(vec3 v, out uint w)
{
    ivec3 c = ivec3(round(v * vec3(31, 63, 31)));
    w = uint((c.r << 11) | (c.g << 5) | c.b);

    c.rb = (c.rb << 3) | (c.rb >> 2);
    c.g = (c.g << 2) | (c.g >> 4);

    return vec3(c * (1.0/255));
}

uint EmitEndPointsDXT1(inout vec4 mincol, inout vec4 maxcol)
{
    vec4 inset = (maxcol - mincol)/16 - (8/255.0)/16;
    mincol = saturate(mincol + inset);
    maxcol = saturate(maxcol - inset);

    uvec2 OUT;
    maxcol.rgb = RoundAndExpand(maxcol.rgb, OUT.x);
    mincol.rgb = RoundAndExpand(mincol.rgb, OUT.y);

    // We have to do this in case we select an alternate diagonal.
    if(OUT.x < OUT.y)
    {
        vec3 tmp = mincol.rgb;
        mincol.rgb = maxcol.rgb;
        maxcol.rgb = tmp;
        return OUT.y | (OUT.x << 16U);
    }

    return OUT.x | (OUT.y << 16U);
}

uint EmitIndicesDXT1_noalpha(vec4 block[16] , vec4 mincol, vec4 maxcol)
{
    const float RGB_RANGE= 3;  //divisions of min-max range

    vec3 dir= (maxcol.rgb - mincol.rgb);
    vec3 origin= maxcol.rgb + dir.rgb / (2.0 * RGB_RANGE);   //half the division above max
    dir /= dot(dir, dir);

    // Compute indices
    uint indices= 0U;
    for (int i= 0; i < 16; i++)
    {
        uint index= uint(saturate(dot(origin - block[i].rgb, dir)) * RGB_RANGE);
        indices |= index << uint(i * 2);
    }

    uint i0= (indices & 0x55555555U);
    uint i1= (indices & 0xAAAAAAAAU) >> 1U;
    indices = ((i0 ^ i1) << 1U) | i1;

    // Output indices
    return indices;
}

uint EmitEndPointsAlpha(inout float mincol, inout float maxcol)
{
    float inset = (maxcol - mincol) / 32.0 - (16.0/255) / 32.0;
    mincol = saturate(mincol + inset);
    maxcol = saturate(maxcol - inset);

    uint c0 = uint(round(mincol * 255));
    uint c1 = uint(round(maxcol * 255));

    return (c0 << 8U) | c1;
}

uvec2 EmitIndicesAlpha(vec4 block[16] , float minAlpha, float maxAlpha)
{
    const int ALPHA_RANGE= 7;

    float bias= maxAlpha + (maxAlpha - minAlpha) / (2.0 * ALPHA_RANGE);
    float scale= 1.0f / (maxAlpha - minAlpha);

    uvec2 indices= uvec2(0U);

    for (int i= 0; i < 6; i++)
    {
        uint index= uint(saturate((bias - block[i].a) * scale) * ALPHA_RANGE);
        indices.x |= index << uint(3 * i);
    }

    for (int i= 6; i < 16; i++)
    {
        uint index= uint(saturate((bias - block[i].a) * scale) * ALPHA_RANGE);
        indices.y |= index << uint(3 * i - 18);
    }

    uvec2 i0= (indices >> 0U) & 0x09249249U;
    uvec2 i1= (indices >> 1U) & 0x09249249U;
    uvec2 i2= (indices >> 2U) & 0x09249249U;

    i2 ^= i0 & i1;
    i1 ^= i0;
    i0 ^= (i1 | i2);

    indices.x = (i2.x << 2U) | (i1.x << 1U) | i0.x;
    indices.y = (((i2.y << 2U) | (i1.y << 1U) | i0.y) << 2U) | (indices.x >> 16U);
    indices.x <<= 16U;

    return indices;
}

void SelectCoCgDiagonal(vec4 block[16] , inout vec2 minColor, inout vec2 maxColor)
{
    vec2 mid= (maxColor + minColor) * 0.5;

    float cov= 0;
    for (int i= 0; i < 16; i++)
    {
        vec2 t= block[i].xy - mid;
        cov += t.x * t.y;
    }
    if (cov < 0) {
        float tmp= maxColor.y;
        maxColor.y = minColor.y;
        minColor.y = tmp;
    }
}

void main() {
    ivec2 coord = ivec2(4 * gl_FragCoord.xy) & ivec2(~3);
    vec4 readBlock[16];
    
    ExtractColorBlockCoCgAY(readBlock, ScreenTexture, coord, 0);
    
    vec4 mincol, maxcol;
    FindMinMaxColorsBox(readBlock, mincol, maxcol);

    uvec4 OUT;
    SelectCoCgDiagonal(readBlock, mincol.xy, maxcol.xy);

    // Output CoCg in DXT1 block.
    OUT.z = EmitEndPointsDXT1(mincol, maxcol);
    OUT.w = EmitIndicesDXT1_noalpha(readBlock, mincol, maxcol);

    // Output Y in DXT5 alpha block.
    OUT.x = EmitEndPointsAlpha(mincol.a, maxcol.a);

    uvec2 indices= EmitIndicesAlpha(readBlock, mincol.a, maxcol.a);
    OUT.x |= indices.x;
    OUT.y = indices.y;

    retVal = OUT;
}