// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LlamaChatDataTypes.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EChatSendType : uint8
{
	User UMETA(DisplayName = "User"),
	AI UMETA(DisplayName = "AI Assistant"),
	FirstAI UMETA(DisplayName = "AI 1(LLM 대화 전용)"),
	SecondAI UMETA(DisplayName = "AI 2(LLM 대화 전용)"),
	System UMETA(DisplayName = "시스템")
};

UENUM(BlueprintType)
enum class EChatSessionType : uint8
{
	Standard UMETA(DisplayName = "표준 채팅"),
	AiAutoChat UMETA(DisplayName = "AI 끼리 채팅"),
	Group UMETA(DisplayName = "그룹 채팅 (아직 실현 가능성x)")
};



USTRUCT(BlueprintType)
struct FLlamaChatMessage
{
	GENERATED_BODY()

	// 메세지 내용
	UPROPERTY(BlueprintReadWrite)
	FString Message;

	// 송신자 유형
	UPROPERTY(BlueprintReadWrite)
	EChatSendType Sender = EChatSendType::User;

	// 송신자 이름
	UPROPERTY(BlueprintReadWrite)
	FString SenderName = TEXT("User");
};
