#pragma once

#include "CoreMinimal.h"
#include "Api/Common/WebApi.h"

DECLARE_DELEGATE_TwoParams(FOnGameEventSent, bool /*bSuccess*/, const FString& /*Response*/);

template<typename T, typename = void>
struct HasToken : std::false_type {};

template<typename T>
struct HasToken<T, std::void_t<decltype(std::declval<T>().Token)>> : std::true_type {};

template<typename T>
constexpr bool HasToken_v = HasToken<T>::value;

class PLAYERZERO_API FGameEventApi : public FWebApi
{
	
public:
	template<typename TEvent>
	void SendGameEventAsync(const TEvent& Event, FOnGameEventSent OnComplete);
	
	FGameEventApi();
	~FGameEventApi();
private:

	FString GetToken() const;
	FString Url;
};
