#pragma once

#include "CoreMinimal.h"
#include "JsonObjectConverter.h"
#include "Api/Common/WebApi.h"
#include "Api/Common/WebApiWithAuth.h"

DECLARE_DELEGATE_TwoParams(FOnGameEventSent, bool /*bSuccess*/, const FString& /*Response*/);

class PLAYERZERO_API UGameEventApi : public FWebApi
{
	
public:
	template<typename TPayload>
	void SendGameEventAsync(const TPayload& RequestPayload, FOnGameEventSent OnComplete);

private:
	UGameEventApi();
	~UGameEventApi();
	FString GetToken() const;
	FString Url;
};
