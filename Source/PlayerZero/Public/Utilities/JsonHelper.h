#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"

class PLAYERZERO_API FJsonHelper
{
public:
	/** Recursively removes empty strings, nulls, and empty nested objects/arrays */
	static void RemoveEmptyValuesRecursive(TSharedPtr<FJsonObject> JsonObject);

	/** Converts UStruct to cleaned JSON string */
	template<typename T>
	static bool StructToCleanJsonString(const T& Struct, FString& OutJson);
};
