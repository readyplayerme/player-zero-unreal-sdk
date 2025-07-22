#pragma once

#include "CoreMinimal.h"
#include "JsonObjectConverter.h"
#include "Api/Common/WebApi.h"
#include "Api/Common/WebApiWithAuth.h"
#include "Models/CharacterFindByIdRequest.h"
#include "Models/CharacterFindByIdResponse.h"

DECLARE_DELEGATE_TwoParams(FOnCharacterFindResponse, FCharacterFindByIdResponse, bool);

class PLAYERZERO_API FCharacterApi : public FWebApi
{
public:
	
	FCharacterApi();
	virtual ~FCharacterApi() override;

	void FindByIdAsync(const FCharacterFindByIdRequest& Request, FOnCharacterFindResponse OnComplete);

protected:
	template <typename T>
	FString ConvertToJsonString(const T& Data);

	void HandleCharacterResponse(TSharedPtr<FApiRequest> ApiRequest, FHttpResponsePtr Response, bool bWasSuccessful);
	void HandleFindResponse(FHttpResponsePtr Response, bool bWasSuccessful);
private:
	FString BaseUrl;
};

template <typename T>
FString FCharacterApi::ConvertToJsonString(const T& Data)
{
	FString JsonString;
	FJsonObjectConverter::UStructToJsonObjectString(Data, JsonString);
	return JsonString;
}
