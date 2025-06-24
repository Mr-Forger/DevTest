// Fill out your copyright notice in the Description page of Project Settings.


#include "LlamaChatUISubsystem.h"

#include "LlamaChatSession.h"
#include "Kismet/GameplayStatics.h"
#include "UI/LlmChat.h"

void ULlamaChatUISubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	ChatWidgetClass = LoadClass<ULlmChat>(nullptr, TEXT("/Game/Widget/WBP_LlmChat.WBP_LlmChat_C"));
	if (ChatWidgetClass)
	{
		UE_LOG(LogTemp, Log, TEXT("Found ChatWidgetClass"));
		return; 
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ChatWidgetClass를 로드할 수 없습니다. 경로를 확인하세요."));
	}

	UE_LOG(LogTemp, Log, TEXT("UI 서브시스템이 초기화 되었습니다."));
}

void ULlamaChatUISubsystem::Deinitialize()
{
	Super::Deinitialize();

	if (ChatWidget)
	{
		ChatWidget->RemoveFromParent();
		ChatWidget = nullptr;
		UE_LOG(LogTemp, Log, TEXT("채팅 위젯을 정리하였습니다."));
	}
}

void ULlamaChatUISubsystem::ShowChatUI()
{
	UE_LOG(LogTemp, Log, TEXT("LlamaChatUISubsystem: ShowChatUI 함수 호출"));

	if (ChatWidget && ChatWidget->IsInViewport())
	{
		UE_LOG(LogTemp, Warning, TEXT("UI가 이미 있습니다."));
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("월드를 찾을 수 없습니다."));
		return;
	}

	// if (!ChatSession)
	// {
	// 	TArray<AActor*> FoundActors;
	// 	UGameplayStatics::GetAllActorsOfClass(World, ALlamaChatSession::StaticClass(), FoundActors);
	// 	
	// 	if (FoundActors.Num() > 0)
	// 	{
	// 		ChatSession = Cast<ALlamaChatSession>(FoundActors[0]);
	// 	}
	// 	else
	// 	{
	// 		UE_LOG(LogTemp, Error, TEXT("ChatSesion을 못 찾았습니다."));
	// 		return;
	// 	}
	//
	// }
	
	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Error, TEXT("컨트롤러를 찾을 수 없습니다."));
		return;
	}

	ChatWidget = CreateWidget<ULlmChat>(PlayerController, ChatWidgetClass);
	if (!ChatWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("위젯 생성x"));
		return;
	}
	UE_LOG(LogTemp, Log, TEXT("위젯 생성 성공"));

	ChatWidget->SetLlmSession(ChatSession);
	ChatWidget->AddToViewport(10);

	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(ChatWidget->TakeWidget());
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	PlayerController->SetInputMode(InputMode);
	PlayerController->bShowMouseCursor = true;
}

void ULlamaChatUISubsystem::HideChatUI()
{
	if (ChatWidget && ChatWidget->IsInViewport())
	{
		ChatWidget->RemoveFromParent();
        
		// 입력 모드를 게임으로 되돌리기
		if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
		{
			FInputModeGameOnly InputMode;
			PlayerController->SetInputMode(InputMode);
			PlayerController->bShowMouseCursor = false;
		}
        
		UE_LOG(LogTemp, Log, TEXT("채팅 UI를 숨겼습니다"));
	}
}

void ULlamaChatUISubsystem::RegisterChatSession(ALlamaChatSession* LlamaChatSession)
{
	if (!LlamaChatSession)
	{
		UE_LOG(LogTemp, Warning, TEXT("ChatSession이 없습니다."));
		return;
	}

	ChatSession = LlamaChatSession;
	UE_LOG(LogTemp, Log, TEXT("ChatSession이 등록되었습니다."));
}

void ULlamaChatUISubsystem::UnregisterChatSession()
{
	ChatSession = nullptr;
	UE_LOG(LogTemp, Log, TEXT("ChatSession 등록 해제되었습니다."));
}
