// Fill out your copyright notice in the Description page of Project Settings.

#include "Component/ChatManagerComponent.h"

#include "LlamaComponent.h"

// Sets default values for this component's properties
UChatManagerComponent::UChatManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	LlamaComponent = nullptr;
	bIsProcessingMessage = false;
}


// Called when the game starts
void UChatManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	FindAndConnectLlamaComponent();
	GetAvailableModels();
}

// 입력한 User 메세지 전송 함수
void UChatManagerComponent::SendUserMessage(const FString& UserMessage)
{
	if (UserMessage.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("메세지가 비었습니다!!"));
		return;
	}

	if (!LlamaComponent)
	{
		OnChatErrorOccurred.Broadcast(TEXT("LlamaComponent를 찾을 수 없습니다."));
		return;
	}

	// 실제 메시지 보낼 때 모델 상태 체크
	if (!LlamaComponent->IsModelLoaded())
	{
		OnChatErrorOccurred.Broadcast(TEXT("모델이 아직 로딩 중입니다. 잠시 후 다시 시도해주세요."));
		return;
	}

	if (bIsProcessingMessage)
	{
		UE_LOG(LogTemp, Warning, TEXT("메세지를 이미 전송했습니다!"));
		return;
	}

	bIsProcessingMessage = true; // 메세지를 전송하는 중인가요? 라는 뜻이기에 true여야 메세지를 전송 중이라는 의미이다.
	// OnChatMessageReceived.Broadcast(UserMessage, true);


	// 임시로 명명한 델리게이트 및 LlamaChatDataTypes 활용 (추후 수정 필요)
	FLlamaChatMessage NewChatMessage;
	NewChatMessage.Message = UserMessage;
	NewChatMessage.Sender = EChatSendType::User;
	NewChatMessage.SenderName = TEXT("User");

	OnChatMessageReceived.Broadcast(NewChatMessage);

	UE_LOG(LogTemp, Log, TEXT("LLM에게 메세지를 전송합니다. 메세지 내용: %s"), *UserMessage);
	LlamaComponent->InsertTemplatedPrompt(UserMessage, EChatTemplateRole::User, false, true);
}

void UChatManagerComponent::ChangeSystemPrompt(const FString& Prompt, bool bResetContext)
{
	if (!LlamaComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("LlamaComponent가 없습니다."));
		return;
	}

	LlamaComponent->ModelParams.SystemPrompt = Prompt;

	if (bResetContext)
	{
		LlamaComponent->ResetContextHistory(false);

		LlamaComponent->InsertTemplatedPrompt(Prompt, EChatTemplateRole::System, false, false);
	}
	UE_LOG(LogTemp, Log, TEXT("시스템 프롬프트 변경 완료"));
}

FString UChatManagerComponent::GetCurrentSystemPrompt() const
{
	if (LlamaComponent)
	{
		return LlamaComponent->ModelParams.SystemPrompt;
	}
	return TEXT("");
}

TArray<FAvailableModel> UChatManagerComponent::GetAvailableModels()
{
	TArray<FAvailableModel> AvailableModels;

	FString ModelPath = FPaths::ProjectSavedDir() / TEXT("Models");

	if (!FPaths::DirectoryExists(ModelPath))
	{
		UE_LOG(LogTemp, Warning, TEXT("폴더가 없습니다."));
		return AvailableModels;
	}

	// LLM 파일 찾기
	TArray<FString> FoundModelFiles;
	IFileManager& FileManager = IFileManager::Get();
	FileManager.FindFiles(FoundModelFiles, *(ModelPath / TEXT("*.gguf")), true, false); // 이 함수를 쓰면 한 번에 FoundModelFiles에 파일들의 이름이 저장된다.

	for (const FString& FileName : FoundModelFiles)
	{
		// 지역 변수 선언
		FAvailableModel Model;
		Model.ModelFileName = FileName;
		Model.RelativePath = TEXT("./") + FileName;

		//파일 크기 체크
		FString FullPath = ModelPath / FileName;
		int64 FileSize = FileManager.FileSize(*FullPath);
		Model.FileSize = FileSize / (1024 * 1024 * 1024);

		// 이름 표시하기
		FString BaseName = FPaths::GetBaseFilename(FileName);
		Model.DisplayName = FString::Printf(TEXT("%s (%.1fGB)"), *BaseName, Model.FileSize);

		AvailableModels.Add(Model);
		UE_LOG(LogTemp, Log, TEXT("모델: %s"), *Model.DisplayName);
	}
	UE_LOG(LogTemp, Log, TEXT("총 %d개의 모델을 찾았습니다."), AvailableModels.Num());
	return AvailableModels;
	
}

void UChatManagerComponent::ChangeModel(const FString& ModelPath, bool bShowWarning)
{
	// TODO: 250611 변경기능 여부 체크, 경고 메세지, 현재 모델 unload, 새 모델 경로 설정, 새 모델 로드, 업데이트

	// 검증 코드들
	if (!LlamaComponent)
	{
		OnChatErrorOccurred.Broadcast(TEXT("LlamaComponent가 없습니다."));
		return;
	}
	if (!LlamaComponent->IsModelLoaded())
	{
		OnChatErrorOccurred.Broadcast(TEXT("LLM 모델이 로드가 되지 않았습니다."));
	}
	if (bIsProcessingMessage || bIsChangingModel)
	{
		OnChatErrorOccurred.Broadcast(TEXT("다른 작업 중입니다. 나중에 다시 시도해주세요."));
		return;
	}

	FString BackupSystemPrompt = LlamaComponent->ModelParams.SystemPrompt;

	// 모델 변경 시작
	bIsChangingModel = true;
	OnModelChangeStatus.Broadcast(TEXT("모델 변경 중입니다."), true);

	//모델 경로 및 프롬프트 설정
	LlamaComponent->ModelParams.PathToModel = ModelPath;
	LlamaComponent->ModelParams.SystemPrompt = BackupSystemPrompt;

	UE_LOG(LogTemp, Log, TEXT("모델 언로드"));
	LlamaComponent->UnloadModel();

	UE_LOG(LogTemp, Log, TEXT("변경 된 모델 로드"));
	LlamaComponent->LoadModel(true);
	
	
}

FString UChatManagerComponent::GetCurrentModelName() const
{
	// TODO: 모델이 로드되어 있을 때, 해당 모델의 이름 및 경로를 가지고 오도록 만들기.
	
	// 검증 코드들
	if (!LlamaComponent)
	{
		return TEXT("컴포넌트가 없습니다.");
	}
	if (!LlamaComponent->IsModelLoaded())
	{
		return TEXT("모델이 로드되지 않았습니다.");
	}

	// 모델 이름 찾기
	FString ModelPath = LlamaComponent->ModelParams.PathToModel;
	FString ModelName = FPaths::GetBaseFilename(ModelPath);

	return FString::Printf(TEXT("현재 모델은 %s 입니다."), *ModelName);
}

void UChatManagerComponent::FindAndConnectLlamaComponent()
{
	AActor* Owner = GetOwner();
	if (!Owner)
	{
		UE_LOG(LogTemp, Warning, TEXT("ChatManagerComponent의 Owner가 없습니다."));
		return;
	}

	LlamaComponent = Owner->FindComponentByClass<ULlamaComponent>();

	if (LlamaComponent)
	{
		UE_LOG(LogTemp, Log, TEXT("ChatManagerComponent가 LlamaComponent를 찾았습니다. 콜백 함수 실행 중..."));
		LlamaComponent->OnResponseGenerated.AddDynamic(this, &UChatManagerComponent::HandleLlmResponse);
		LlamaComponent->OnError.AddDynamic(this, &UChatManagerComponent::HandleError);
		LlamaComponent->OnModelLoaded.AddDynamic(this, &UChatManagerComponent::HandleModelLoaded);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("LlamaComponent를 찾을 수 없습니다!"));
		OnChatErrorOccurred.Broadcast(TEXT("LlamaComponent를 찾을 수 없습니다"));
	}
}

void UChatManagerComponent::HandleLlmResponse(const FString& Response)
{
	UE_LOG(LogTemp, Log, TEXT("Received LLM response: %s"), *Response);

	bIsProcessingMessage = false; // 끝났으니까 false

	FLlamaChatMessage LlmChatMessage;
	LlmChatMessage.Message = Response;
	LlmChatMessage.Sender = EChatSendType::AI;
	LlmChatMessage.SenderName = TEXT("LLM");

	OnChatMessageReceived.Broadcast(LlmChatMessage);
}

void UChatManagerComponent::HandleError(const FString& ErrorMessage, int32 ErrorCode)
{
	UE_LOG(LogTemp, Warning, TEXT("LLM Error Occurred! %d: %s"), ErrorCode, *ErrorMessage);

	bIsProcessingMessage = false;

	if (bIsChangingModel)
	{
		bIsChangingModel = false;
		OnModelChangeStatus.Broadcast(TEXT("모델 변경 실패"), false);
	}

	FString Error = FString::Printf(TEXT("Error %d: %s"), ErrorCode, *ErrorMessage);
	OnChatErrorOccurred.Broadcast(Error);
}

void UChatManagerComponent::HandleModelLoaded(const FString& ModelPath)
{
	UE_LOG(LogTemp, Log, TEXT("모델 로드 완료! 모델 경로: %s"), *ModelPath);

	if (bIsChangingModel)
	{
		bIsChangingModel = false;

		FString ModelName = FPaths::GetBaseFilename(ModelPath);
		OnModelChangeStatus.Broadcast(FString::Printf(TEXT("%s 모델입니다."), *ModelName), false);

		CurrentModelPath.Empty();

		UE_LOG(LogTemp, Log, TEXT("모델 변경 성공"));
	}
}

