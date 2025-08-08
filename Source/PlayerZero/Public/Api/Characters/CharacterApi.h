#pragma once

#include "CoreMinimal.h"
#include "JsonObjectConverter.h"
#include "Api/Common/WebApi.h"
#include "Models/CharacterFindByIdRequest.h"
#include "Models/CharacterFindByIdResponse.h"

DECLARE_DELEGATE_TwoParams(FOnCharacterFindResponse, FCharacterFindByIdResponse, bool);

class PLAYERZERO_API FCharacterApi : public FWebApi
{
=======
class RPMNEXTGEN_API FCharacterApi : public FWebApiWithAuth
{	
>>>>>>> origin/develop:Source/RpmNextGen/Public/Api/Characters/CharacterApi.h
public:
	
	FCharacterApi();
	virtual ~FCharacterApi() override;
	void FindByIdAsync(TSharedPtr<FCharacterFindByIdRequest> Request, FOnCharacterFindResponse OnComplete);

	void FindByIdAsync(const FCharacterFindByIdRequest& Request, FOnCharacterFindResponse OnComplete);

protected:
	template <typename T>
	FString ConvertToJsonString(const T& Data);
	
private:
	FString BaseUrl;
};
