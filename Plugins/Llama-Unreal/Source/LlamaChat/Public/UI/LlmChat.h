// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LlmChat.generated.h"

struct FLlamaChatMessage;
enum class EChatSendType : uint8;
class UChatSetting;
class UChatManagerComponent;
class ALlamaChatSession;
/**
 * 
 */
// LlmChat.h
UCLASS()
class LLAMACHAT_API ULlmChat : public UUserWidget
{
	GENERATED_BODY()
    
public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, Category="LLM Chat")
	void SetLlmSession(ALlamaChatSession* ChatSession);

protected:
	// LlamaChatSession 참조
	UPROPERTY(BlueprintReadOnly, Category="LLM Chat")
	TObjectPtr<ALlamaChatSession> LlamaChatSessionRef;

	// ChatManagerComponent 참조
	UPROPERTY()
	TObjectPtr<UChatManagerComponent> ChatManagerComponent;

	UPROPERTY()
	TObjectPtr<UChatSetting> ChatSetting;

	UFUNCTION(BlueprintImplementableEvent, Category="LLM Chat")
	void OnLlmSessionReady();

	// UI 요소들
	UPROPERTY(meta=(BindWidget))
	class UScrollBox* ScrollBox;
    
	UPROPERTY(meta=(BindWidget))
	class UEditableText* MessageInputBox;
    
	UPROPERTY(meta=(BindWidget))
	class UButton* EnterButton;

	UPROPERTY(meta=(BindWidget))
	class UButton* SettingButton;
    
	// WBP_MessageRow
	UPROPERTY(EditDefaultsOnly, Category="Chat")
	TSubclassOf<class UMessageRow> MessageRowWidgetClass;

	// WBP_ChatSetting
	UPROPERTY(EditDefaultsOnly, Category="Chat")
	TSubclassOf<class UChatSetting> ChatSettingWidgetClass;

private:
	// 버튼 클릭 핸들러
	UFUNCTION()
	void OnEnterButtonClicked();
    
	// 엔터키 핸들러
	UFUNCTION()
	void HandleTextCommitted(const FText& Text, ETextCommit::Type CommitType);
    
	// 메시지 전송
	void SendMessage();
    
	// ChatManager 콜백
	// UFUNCTION()
	// void OnMessageReceived(const FString& Message, bool bIsUserMessage);
    
	UFUNCTION()
	void OnErrorOccurred(const FString& ErrorMessage);
	
	// EChatSendType SenderType;

	//아까 만든 임시 델리게이트 콜백 함수
	UFUNCTION()
	void OnChatMessageReceived(const FLlamaChatMessage& ChatMessage);

	// UI에 메세지 띄우는 새 함수
	void AddMessageToUI(const FString& Message, EChatSendType SenderType);

	UFUNCTION()
	void OnSettingButtonClicked();

};