#pragma once
#include <Mof.h>

// 名前の置き換え
using TexturePtr = std::unique_ptr<CTexture>;
using RefTexturePtr = TexturePtr&;