#include "Api/Files/FileApi.h"
#include "HttpModule.h"
#include "PlayerZero.h"
#include "Api/Files/Models/FileData.h"
#include "Interfaces/IHttpResponse.h"

FFileApi::FFileApi()
{
}

FFileApi::~FFileApi()
{
}

void FFileApi::LoadFileFromUrl(const FString& URL)
{
	const FFileData& File = FFileData(URL);
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->OnProcessRequestComplete().BindRaw(this, &FFileApi::FileRequestComplete, File);
	HttpRequest->SetURL(URL);
	HttpRequest->SetVerb("GET");
	HttpRequest->ProcessRequest();
}

void FFileApi::FileRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful, FFileData File)
{
	
	if (bWasSuccessful && Response.IsValid() && Response->GetContentLength() > 0)
	{
		TArray<uint8> Content = Response->GetContent();
		OnAssetFileRequestComplete.ExecuteIfBound(File, Content);
		return;
	}
	UE_LOG(LogPlayerZero, Warning, TEXT("Failed to load file from URL. Try loading from cache"));
	OnAssetFileRequestComplete.ExecuteIfBound(File, TArray<uint8>());
}