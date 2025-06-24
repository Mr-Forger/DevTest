// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LlamaChatDataTypes.h"
#include "Components/ActorComponent.h"
#include "ChatManagerComponent.generated.h"

/**
 * UChatManagerComponent
 * - UI와 LlamaComponent 사이의 중간 관리자 역할 담당.
 * - 현재는 메세지 송수신을 담당합니다.
 *
 * 역할:
 * - 사용자 입력을 LLM에 전달
 * - LLM의 응답을 UI에 전달
 * - 에러 처리
 */

class ULlamaComponent;

/**
 * FOnChatMessageRecieved 델리게이트
 * - 채팅 메세지 수신 시 호출되는 델리게이트
 * @param Message 수신된 메세지
 * @param bIsFromUser true면 메세지를 보낸 주체가 User, false면 메세지를 보낸 주체가 LLM
 */
// DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnChatMessageReceived, const FString&, Message, bool, bIsFromUser);

/**
 * FOnChatErrorOccurred 델리게이트
 * - 에러 발생 시 호출되는 델리게이트
 * @param ErrorMessage 델리게이트가 호출되었을 때 뜨는 메세지
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChatErrorOccurred, const FString&, ErrorMessage);

/**
 * FOnModelChangeStatus 델리게이트
 *	- 모델이 변경되는지 체크하는 델리게이트
 *	@param StatusMessage - 모델의 상태를 알려주는 메세지
 *	@param bIsChanging - 현재 상태가 변했는지 안변했는지를 체크
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnModelChangeStatus, const FString&, StatusMessage, bool, bIsChanging);

// 새롭게 메세지를 전달받는 임시로 명명한 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChatMessageReceived, const FLlamaChatMessage&, ChatMessage);

// TODO: LLM 끼리 소통하는 일종의 기타 기능 개발 시작, 델리게이트 수정, 응답 및 콜백 함수 수정 예정
USTRUCT(Blueprintable)
struct FAvailableModel
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString DisplayName;

	UPROPERTY(BlueprintReadWrite)
	FString ModelFileName;

	UPROPERTY(BlueprintReadWrite)
	FString RelativePath;

	UPROPERTY(BlueprintReadWrite)
	float FileSize;

	FAvailableModel() : FileSize(0.0f) {}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LLAMACHAT_API UChatManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UChatManagerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// 사용자의 입력 메세지를 전송하는 함수
	UFUNCTION(BlueprintCallable, Category="Chat Manager")
	void SendUserMessage(const FString& UserMessage);

	// UPROPERTY(BlueprintAssignable, Category="Chat Manager|Event")
	// FOnChatMessageReceived OnChatMessageReceived;

	UPROPERTY(BlueprintAssignable, Category="Chat Manager|Event")
	FOnChatErrorOccurred OnChatErrorOccurred;

	UPROPERTY(BlueprintAssignable, Category="Chat Manager|Event")
	FOnModelChangeStatus OnModelChangeStatus;

	// 임시 명칭
	UPROPERTY(BlueprintAssignable, Category="Chat Manager|Event")
	FOnChatMessageReceived OnChatMessageReceived;

	UFUNCTION(BlueprintCallable, Category="Chat Setting")
	void ChangeSystemPrompt(const FString& Prompt, bool bResetContext = true);

	UFUNCTION(BlueprintPure, Category="Chat Setting")
	FString GetCurrentSystemPrompt() const;

	UFUNCTION(BlueprintCallable, Category="Model Management")
	TArray<FAvailableModel> GetAvailableModels();

	UFUNCTION(BlueprintCallable, Category="Model Management")
	void ChangeModel(const FString& ModelPath, bool bShowWarning = true);

	UFUNCTION(BlueprintPure, Category="Model Management")
	FString GetCurrentModelName() const;

	UPROPERTY()
	int32 ComponentSessionID = 0;
	
// Protected 섹션에 있는 함수는 상속 시에만 private로 받아와지므로 내부 함수라고 생각하면 됩니다.
protected:
	// LlamaComponent를 찾은 후 연결하는 함수
	void FindAndConnectLlamaComponent();

	// 사용자 메세지를 받고 LLM의 응답을 처리해주는 콜백 함수
	UFUNCTION()
	void HandleLlmResponse(const FString& Response);

	// LLM에서 에러 발생 시 처리하는 콜백 함수
	UFUNCTION()
	void HandleError(const FString& ErrorMessage, int32 ErrorCode);

	UFUNCTION()
	void HandleModelLoaded(const FString& ModelPath);

private:
	UPROPERTY()
	ULlamaComponent* LlamaComponent;

	bool bIsProcessingMessage;
	bool bIsChangingModel = false;

	FString CurrentModelPath;

		
};
