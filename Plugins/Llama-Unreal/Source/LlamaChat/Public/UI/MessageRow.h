// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MessageRow.generated.h"

enum class EChatSendType : uint8;
/**
 * 
 */
UCLASS()
class LLAMACHAT_API UMessageRow : public UUserWidget
{
	GENERATED_BODY()

public:
	// // 기존 메세지를 띄우는 함수
	// UFUNCTION(BlueprintCallable, Category="Message")
	// void SetMessage(const FString& MessageText, bool bIsUserMessage);

	// 신규 메세지를 띄우는 함수
	UFUNCTION(BlueprintCallable, Category="Message")
	void SetMessage(const FString& MessageText, EChatSendType SenderType);
	

protected:
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* MessageTextBlock;

	// TODO: MessageRow가 LlamaChatTypes의 열거형에 맞춰서 올바른 답변을 하도록 수정
	// 통합 처리를 통해서 일관된 스타일을 유지하도록 수정
};
