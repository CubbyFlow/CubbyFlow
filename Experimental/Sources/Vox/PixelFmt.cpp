/*************************************************************************
> File Name: PixemFmt.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Simple filesystem class
> Created Time: 2020/07/29
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Vox/PixelFmt.hpp>
#include <Vox/Renderer.hpp>

#include <glad/glad.h>

namespace Vox {
    
    const PixelFmtDesc __pfds[] = 
    {
/*PF_UNKNOWN*/                      {0, },
/*PF_R8*/                           {GL_R8, GL_RED, GL_UNSIGNED_BYTE, 1, 1, true, PixelFmt::PF_UNKNOWN, "PF_R8", false},
/*PF_R16*/                          {GL_R16, GL_RED, GL_UNSIGNED_SHORT, 2, 1, true, PixelFmt::PF_UNKNOWN, "PF_R16", false},
/*PF_RG8*/                          {GL_RG8, GL_RG, GL_UNSIGNED_BYTE, 2, 2, true, PixelFmt::PF_UNKNOWN, "PF_RG8", false},
/*PF_RG16*/                         {GL_RG16, GL_RG, GL_UNSIGNED_SHORT, 4, 2, true, PixelFmt::PF_UNKNOWN, "PF_RG16", false},
/*PF_RGB8*/                         {GL_RGB8, GL_RGB, GL_UNSIGNED_BYTE, 3, 3, true, PixelFmt::PF_SRGB8, "PF_RGB8", false},
/*PF_RGBA8*/                        {GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, 4, 4, true, PixelFmt::PF_SRGB8_ALPHA8, "PF_RGBA8", false},
/*PF_RGBA16*/                       {GL_RGBA16, GL_RGBA, GL_UNSIGNED_SHORT, 8, 4, true, PixelFmt::PF_UNKNOWN, "PF_RGBA16", false},
/*PF_SRGB8*/                        {GL_SRGB8, GL_RGB, GL_UNSIGNED_BYTE, 3, 3, true, PixelFmt::PF_UNKNOWN, "PF_SRGB8", false},
/*PF_SRGB8_ALPHA8*/                 {GL_SRGB8_ALPHA8, GL_RGBA, GL_UNSIGNED_BYTE, 4, 4, true, PixelFmt::PF_UNKNOWN, "PF_SRGB8_ALPHA8", false},
/*PF_R16F*/                         {GL_R16F, GL_RED, GL_HALF_FLOAT, 2, 1, true, PixelFmt::PF_UNKNOWN, "PF_R16F", false},
/*PF_RG16F*/                        {GL_RG16F, GL_RG, GL_HALF_FLOAT, 4, 2, true, PixelFmt::PF_UNKNOWN, "PF_RG16F", false},
/*PF_RGB16F*/                       {GL_RGB16F, GL_RGB, GL_HALF_FLOAT, 6, 3, true, PixelFmt::PF_UNKNOWN, "PF_RGB16F", false},
/*PF_RGBA16F*/                      {GL_RGBA16F, GL_RGBA, GL_HALF_FLOAT, 8, 4, true, PixelFmt::PF_UNKNOWN, "PF_RGBA16F", false},
/*PF_R32F*/                         {GL_R32F, GL_RED, GL_FLOAT, 4, 1, true, PixelFmt::PF_UNKNOWN, "PF_R32F", false},
/*PF_RG32F*/                        {GL_RG32F, GL_RG, GL_FLOAT, 8, 2, true, PixelFmt::PF_UNKNOWN, "PF_RG32F", false},
/*PF_RGB32F*/                       {GL_RGB32F, GL_RGB, GL_FLOAT, 12, 3, true, PixelFmt::PF_UNKNOWN, "PF_RGB32F", false},
/*PF_RGBA32F*/                      {GL_RGBA32F, GL_RGBA, GL_FLOAT, 16, 4, true, PixelFmt::PF_UNKNOWN, "PF_RGBA32F", false},
/*PF_R8I*/                          {GL_R8I, GL_RED_INTEGER, GL_BYTE, 1, 1, true, PixelFmt::PF_UNKNOWN, "PF_R8I", false},
/*PF_R8UI*/                         {GL_R8UI, GL_RED_INTEGER, GL_UNSIGNED_BYTE, 1, 1, true, PixelFmt::PF_UNKNOWN, "PF_R8UI", false},
/*PF_R16I*/                         {GL_R16I, GL_RED_INTEGER, GL_SHORT, 2, 1, true, PixelFmt::PF_UNKNOWN, "PF_R16I", false},
/*PF_R16UI*/                        {GL_R16UI, GL_RED_INTEGER, GL_UNSIGNED_SHORT, 2, 1, true, PixelFmt::PF_UNKNOWN, "PF_R16UI", false},
/*PF_R32I*/                         {GL_R32I, GL_RED_INTEGER, GL_INT, 4, 1, true, PixelFmt::PF_UNKNOWN, "PF_R32I", false},
/*PF_R32UI*/                        {GL_R32UI, GL_RED_INTEGER, GL_UNSIGNED_INT, 4, 1, true, PixelFmt::PF_UNKNOWN, "PF_R32UI", false},
/*PF_RG8I*/                         {GL_RG8I, GL_RG_INTEGER, GL_BYTE, 2, 2, true, PixelFmt::PF_UNKNOWN, "PF_RG8I", false},
/*PF_RG8UI*/                        {GL_RG8UI, GL_RG_INTEGER, GL_UNSIGNED_BYTE, 2, 2, true, PixelFmt::PF_UNKNOWN, "PF_RG8UI", false},
/*PF_RG16I*/                        {GL_RG16I, GL_RG_INTEGER, GL_SHORT, 4, 2, true, PixelFmt::PF_UNKNOWN, "PF_RG16I", false},
/*PF_RG16UI*/                       {GL_RG16UI, GL_RG_INTEGER, GL_UNSIGNED_SHORT, 4, 2, true, PixelFmt::PF_UNKNOWN, "PF_RG16UI", false},
/*PF_RG32I*/                        {GL_RG32I, GL_RG_INTEGER, GL_INT, 8, 2, true, PixelFmt::PF_UNKNOWN, "PF_RG32I", false},
/*PF_RG32UI*/                       {GL_RG32UI, GL_RG_INTEGER, GL_UNSIGNED_INT, 8, 2, true, PixelFmt::PF_UNKNOWN, "PF_RG32UI", false},
/*PF_RGB8I*/                        {GL_RGB8I, GL_RGB_INTEGER, GL_BYTE, 3, 3, true, PixelFmt::PF_UNKNOWN, "PF_RGB8I", false},
/*PF_RGB8UI*/                       {GL_RGB8UI, GL_RGB_INTEGER, GL_UNSIGNED_BYTE, 3, 3, true, PixelFmt::PF_UNKNOWN, "PF_RGB8UI", false},
/*PF_RGB16I*/                       {GL_RGB16I, GL_RGB_INTEGER, GL_SHORT, 6, 3, true, PixelFmt::PF_UNKNOWN, "PF_RGB16I", false},
/*PF_RGB16UI*/                      {GL_RGB16UI, GL_RGB_INTEGER, GL_UNSIGNED_SHORT, 6, 3, true, PixelFmt::PF_UNKNOWN, "PF_RGB16UI", false},
/*PF_RGB32I*/                       {GL_RGB32I, GL_RGB_INTEGER, GL_INT, 12, 3, true, PixelFmt::PF_UNKNOWN, "PF_RGB32I", false},
/*PF_RGB32UI*/                      {GL_RGB32UI, GL_RGB_INTEGER, GL_UNSIGNED_INT, 12, 3, true, PixelFmt::PF_UNKNOWN, "PF_RGB32UI", false},
/*PF_RGBA8I*/                       {GL_RGBA8I, GL_RGBA_INTEGER, GL_BYTE, 4, 4, true, PixelFmt::PF_UNKNOWN, "PF_RGBA8I", false},
/*PF_RGBA8UI*/                      {GL_RGBA8UI, GL_RGBA_INTEGER, GL_UNSIGNED_BYTE, 4, 4, true, PixelFmt::PF_UNKNOWN, "PF_RGBA8UI", false},
/*PF_RGBA16I*/                      {GL_RGBA16I, GL_RGBA_INTEGER, GL_SHORT, 8, 4, true, PixelFmt::PF_UNKNOWN, "PF_RGBA16I", false},
/*PF_RGBA16UI*/                     {GL_RGBA16UI, GL_RGBA_INTEGER, GL_UNSIGNED_SHORT, 8, 4, true, PixelFmt::PF_UNKNOWN, "PF_RGBA16UI", false},
/*PF_RGBA32I*/                      {GL_RGBA32I, GL_RGBA_INTEGER, GL_INT, 16, 4, true, PixelFmt::PF_UNKNOWN, "PF_RGBA32I", false},
/*PF_RGBA32UI*/                     {GL_RGBA32UI, GL_RGBA_INTEGER, GL_UNSIGNED_INT, 16, 4, true, PixelFmt::PF_UNKNOWN, "PF_RGBA32UI", false},
/*PF_DEPTH_COMPONENT16*/            {GL_DEPTH_COMPONENT16, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, 4, 1, true, PixelFmt::PF_UNKNOWN, "PF_DEPTH_COMPONENT16", false},
/*PF_DEPTH_COMPONENT24*/            {GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT, GL_FLOAT, 4, 1, true, PixelFmt::PF_UNKNOWN, "PF_DEPTH_COMPONENT24", false},
/*PF_DEPTH_COMPONENT24_STENCIL8*/   {GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, 4, 1, true, PixelFmt::PF_UNKNOWN, "PF_DEPTH_COMPONENT24_STENCIL8", false},
/*PF_DEPTH_COMPONENT32*/            {GL_DEPTH_COMPONENT32, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, 4, 1, true, PixelFmt::PF_UNKNOWN, "PF_DEPTH_COMPONENT32", false},
/*PF_DEPTH_COMPONENT32F*/           {GL_DEPTH_COMPONENT32F, GL_DEPTH_COMPONENT, GL_FLOAT, 4, 1, true, PixelFmt::PF_UNKNOWN, "PF_DEPTH_COMPONENT32F", false},
/*PF_DEPTH_COMPONENT32F_STENCIL8*/  {GL_DEPTH32F_STENCIL8, GL_DEPTH_STENCIL, GL_FLOAT_32_UNSIGNED_INT_24_8_REV, 8, 1, true, PixelFmt::PF_UNKNOWN, "PF_DEPTH_COMPONENT32F_STENCIL8", false},
/*PF_RGB565*/                       {GL_RGB5, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, 2, 3, true, PixelFmt::PF_UNKNOWN, "PF_RGB565", false},
/*PF_END*/                          {0, },
    };
};