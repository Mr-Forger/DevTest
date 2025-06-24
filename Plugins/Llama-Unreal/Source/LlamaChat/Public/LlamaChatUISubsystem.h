// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "LlamaChatUISubsystem.generated.h"

class ALlamaChatSession;
class ULlmChat;
/**
 * 
 */
UCLASS()
class LLAMACHAT_API ULlamaChatUISubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable, Category="UI Subsystem")
	void ShowChatUI();

	UFUNCTION(BlueprintCallable, Category="UI Subsystem")
	void HideChatUI();

	UFUNCTION(BlueprintCallable, Category="Chat System")
	void RegisterChatSession(ALlamaChatSession* LlamaChatSession);

	UFUNCTION(BlueprintCallable, Category="Chat System")
	void UnregisterChatSession();

	UFUNCTION(BlueprintCallable, Category="Chat System")
	ALlamaChatSession* GetCurrentChatSession() const { return ChatSession; }

private:
	UPROPERTY()
	ALlamaChatSession* ChatSession;
	
	UPROPERTY()
	ULlmChat* ChatWidget;

	UPROPERTY()
	TSubclassOf<ULlmChat> ChatWidgetClass;
	
};
