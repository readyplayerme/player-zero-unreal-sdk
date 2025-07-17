#pragma once

#include "CoreMinimal.h"
#include "Api/Common/WebApiWithAuth.h"
#include "Models/CharacterFindByIdRequest.h"
#include "Models/CharacterFindByIdResponse.h"

DECLARE_DELEGATE_TwoParams(FOnCharacterFindResponse, TSharedPtr<FCharacterFindByIdResponse>, bool);

<<<<<<< HEAD:Source/PlayerZero/Public/Api/Characters/CharacterApi.h
class PLAYERZERO_API FCharacterApi : public FWebApiWithAuth
{
=======
class RPMNEXTGEN_API FCharacterApi : public FWebApiWithAuth
{	
>>>>>>> origin/develop:Source/RpmNextGen/Public/Api/Characters/CharacterApi.h
public:
	
	FCharacterApi();
	virtual ~FCharacterApi() override;
	void FindByIdAsync(TSharedPtr<FCharacterFindByIdRequest> Request, FOnCharacterFindResponse OnComplete);

private:
	FString BaseUrl;
	TMap<FString, FString> AssetByType = TMap<FString, FString>();
};
