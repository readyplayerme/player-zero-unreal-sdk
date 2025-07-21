#include "Api/Files/FileApi.h"
#include "HttpModule.h"
#include "PlayerZero.h"
#include "Api/Assets/Models/Asset.h"
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
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->OnProcessRequestComplete().BindRaw(this, &FFileApi::FileRequestComplete);
	HttpRequest->SetURL(URL);
	HttpRequest->SetVerb("GET");
	HttpRequest->ProcessRequest();
}

void FFileApi::LoadAssetFileFromUrl(const FString& URL, FFileData File)
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->OnProcessRequestComplete().BindRaw(this, &FFileApi::AssetFileRequestComplete, File);
	HttpRequest->SetURL(URL);
	HttpRequest->SetVerb("GET");
	HttpRequest->ProcessRequest();
}

void FFileApi::FileRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	FString URL = Request->GetURL();
	FString FileName;
	if(URL.Contains(".glb") || URL.Contains(".png"))
	{
		FileName = FPaths::GetCleanFilename(Request->GetURL());
	}
	
	if (bWasSuccessful && Response.IsValid() && Response->GetContentLength() > 0)
	{
		TArray<uint8> Content = Response->GetContent();
		OnFileRequestComplete.ExecuteIfBound(&Content, FileName);
		return;
	}
	UE_LOG(LogPlayerZero, Error, TEXT("Failed to load file from URL"));
	OnFileRequestComplete.ExecuteIfBound(nullptr, FileName);
}

void FFileApi::AssetFileRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful, FFileData File)
{
	
	if (bWasSuccessful && Response.IsValid() && Response->GetContentLength() > 0)
	{
		TArray<uint8> Content = Response->GetContent();
		OnAssetFileRequestComplete.ExecuteIfBound(&Content, File);
		return;
	}
	UE_LOG(LogPlayerZero, Warning, TEXT("Failed to load file from URL. Try loading from cache"));
	OnAssetFileRequestComplete.ExecuteIfBound(nullptr, File);
}

bool FFileApi::LoadFileFromPath(const FString& Path, TArray<uint8>& OutContent)
{
	if (!FPaths::FileExists(Path))
	{
		UE_LOG(LogPlayerZero, Error, TEXT("Path does not exist %s"), *Path);
		return false;
	}

	return FFileHelper::LoadFileToArray(OutContent, *Path);
}
