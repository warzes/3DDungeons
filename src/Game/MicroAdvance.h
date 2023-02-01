#pragma once

//=============================================================================
// Header
//=============================================================================
#include "MicroEngine.h"

//=============================================================================
// Config
//=============================================================================
#define USE_RESOURCE_CACHE_SYSTEM 1
#define USE_SIMPLE_PERLIN_NOISE 1

//=============================================================================
// File Resource Cache System
//=============================================================================
#if USE_RESOURCE_CACHE_SYSTEM
namespace ResourceCacheSystem
{
	ShaderProgram* LoadShaderProgram(const char* fileName);
	Texture2D* LoadTexture2D(const char* fileName);
	TextureCube* LoadTextureCube(const char* fileName);

	void Clear();
} // ResourceCacheSystem
#endif // USE_RESOURCE_CACHE_SYSTEM

//=============================================================================
// Simple Perlin Noise
//=============================================================================
#if USE_SIMPLE_PERLIN_NOISE

#endif // USE_SIMPLE_PERLIN_NOISE