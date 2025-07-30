#include "Utilities/JsonHelper.h"
#include "JsonObjectConverter.h"
#include "Api/GameEvents/Events/AvatarSessionHeartbeatEvent.h"
#include "Api/GameEvents/Events/AvatarSessionStartedEvent.h"
#include "Api/GameEvents/Events/GameMatchStartedEvent.h"
#include "Api/GameEvents/Events/GameSessionStartedEvent.h"

void FJsonHelper::RemoveEmptyValuesRecursive(TSharedPtr<FJsonObject> JsonObject)
{
	TArray<FString> KeysToRemove;

	for (const auto& Pair : JsonObject->Values)
	{
		const TSharedPtr<FJsonValue>& Value = Pair.Value;
		if (!Value.IsValid())
		{
			KeysToRemove.Add(Pair.Key);
			continue;
		}

		switch (Value->Type)
		{
		case EJson::String:
			if (Value->AsString().IsEmpty())
			{
				KeysToRemove.Add(Pair.Key);
			}
			break;

		case EJson::Object:
			{
				TSharedPtr<FJsonObject> NestedObject = Value->AsObject();
				if (NestedObject.IsValid())
				{
					RemoveEmptyValuesRecursive(NestedObject);
					if (NestedObject->Values.Num() == 0)
					{
						KeysToRemove.Add(Pair.Key);
					}
				}
				break;
			}

		case EJson::Array:
			{
				TArray<TSharedPtr<FJsonValue>> CleanedArray;
				for (const auto& Elem : Value->AsArray())
				{
					if (Elem.IsValid())
					{
						if (Elem->Type == EJson::String && Elem->AsString().IsEmpty())
						{
							continue;
						}
						CleanedArray.Add(Elem);
					}
				}
				if (CleanedArray.Num() == 0)
				{
					KeysToRemove.Add(Pair.Key);
				}
				break;
			}

		default:
			break;
		}
	}

	for (const FString& Key : KeysToRemove)
	{
		JsonObject->RemoveField(Key);
	}
}

template<typename T>
bool FJsonHelper::StructToCleanJsonString(const T& Struct, FString& OutJson)
{
	TSharedRef<FJsonObject> JsonObject = MakeShared<FJsonObject>();
	if (!FJsonObjectConverter::UStructToJsonObject(TBaseStructure<T>::Get(), &Struct, JsonObject, 0, 0, nullptr))
	{
		return false;
	}

	RemoveEmptyValuesRecursive(JsonObject);

	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutJson);
	return FJsonSerializer::Serialize(JsonObject, Writer);
}

// Required for template linking
template bool FJsonHelper::StructToCleanJsonString<FAvatarSessionStartedEnded>(const FAvatarSessionStartedEnded& Struct, FString& OutJson);
template bool FJsonHelper::StructToCleanJsonString<FAvatarSessionHeartbeat>(const FAvatarSessionHeartbeat& Struct, FString& OutJson);
template bool FJsonHelper::StructToCleanJsonString<FGameMatchStartedEvent>(const FGameMatchStartedEvent& Struct, FString& OutJson);
template bool FJsonHelper::StructToCleanJsonString<FGameSessionStartedEvent>(const FGameSessionStartedEvent& Struct, FString& OutJson);
