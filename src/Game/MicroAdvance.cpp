#include "MicroAdvance.h"
//=============================================================================
// Header
//=============================================================================
#if defined(_MSC_VER)
#	pragma warning(push)
#endif // _MSC_VER

#include <unordered_map>

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif // _MSC_VER
//-----------------------------------------------------------------------------
//=============================================================================
// Global var
//=============================================================================
//-----------------------------------------------------------------------------
std::unordered_map<std::string, Texture2D> FileTextures2D;
std::unordered_map<std::string, TextureCube> FileTexturesCubes;
//-----------------------------------------------------------------------------
//=============================================================================
// ResourceCacheSystem
//=============================================================================
//-----------------------------------------------------------------------------
ShaderProgram* ResourceCacheSystem::LoadShaderProgram(const char* fileName)
{
	assert(false);
	return nullptr;
}
//-----------------------------------------------------------------------------
Texture2D* ResourceCacheSystem::LoadTexture2D(const char* fileName, const Texture2DInfo& textureInfo)
{
	auto it = FileTextures2D.find(fileName);
	if( it != FileTextures2D.end() )
	{
		return &it->second;
	}
	else
	{
		LogPrint("Load texture: " + std::string(fileName));

		Texture2D texture;
		if( !texture.Create(fileName, textureInfo) || !texture.IsValid() )
			return nullptr;

		FileTextures2D[fileName] = texture;
		return &FileTextures2D[fileName];
	}
}
//-----------------------------------------------------------------------------
void ResourceCacheSystem::Clear()
{
	for( auto it = FileTextures2D.begin(); it != FileTextures2D.end(); ++it )
		it->second.Destroy();
	FileTextures2D.clear();
}
//-----------------------------------------------------------------------------
bool ResourceCacheSystem::IsLoad(const Texture2D& texture)
{
	for( auto it = FileTextures2D.begin(); it != FileTextures2D.end(); ++it )
	{
		if( texture == it->second )
			return true;
	}
	return false;
}
//-----------------------------------------------------------------------------