#pragma once

#include "CoreMinimal.h"

struct FAvatarRenderConfig;
struct FCharacterConfig;

class FPlayerZeroConfigProcessor
{
public:
	static FString ProcessCharacter(const FCharacterConfig& Config);
	static FString ProcessRender(const FAvatarRenderConfig& Config);
};
