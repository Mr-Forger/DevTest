// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/LlmChat.h"

#include "LlamaChatSession.h"
#include "LlamaComponent.h"
#include "Component/ChatManagerComponent.h"
#include "UI/MessageRow.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Components/ScrollBox.h"
#include "Components/ScrollBoxSlot.h"
#include "Components/TextBlock.h"
#include "UI/ChatSetting.h"


void ULlmChat::NativeConstruct()
{
	Super::NativeConstruct();

	if (EnterButton)
	{
		EnterButton->OnClicked.AddDynamic(this, &ULlmChat::OnEnterButtonClicked);
	}

	if (MessageInputBox)
	{
		MessageInputBox->OnTextCommitted.AddDynamic(this, &ULlmChat::HandleTextCommitted);
	}

	if (SettingButton)
	{
		SettingButton->OnClicked.AddDynamic(this, &ULlmChat::OnSettingButtonClicked);
	}
}

void ULlmChat::SetLlmSession(ALlamaChatSession* ChatSession)
{
	LlamaChatSessionRef = ChatSession;

	if (!ChatSession) return;

	ChatManagerComponent = ChatSession->ChatManagerComponent;

	if (ChatManagerComponent)
	{
		// ChatManagerComponent->OnChatMessageReceived.AddDynamic(this, &ULlmChat::OnMessageReceived);
		ChatManagerComponent->OnChatErrorOccurred.AddDynamic(this, &ULlmChat::OnErrorOccurred);
		ChatManagerComponent->OnChatMessageReceived.AddDynamic(this, &ULlmChat::OnChatMessageReceived);

		UE_LOG(LogTemp, Log, TEXT("UI가 ChatManagerComponent와 연결되었습니다."));
	}

	OnLlmSessionReady();
}

void ULlmChat::OnEnterButtonClicked()
{
	SendMessage();
}

void ULlmChat::HandleTextCommitted(const FText& Text, ETextCommit::Type CommitType)
{
	if (CommitType == ETextCommit::OnEnter)
	{
		SendMessage();
	}
}

void ULlmChat::SendMessage()
{
	if (!MessageInputBox || !ChatManagerComponent) return;

	FString UserMessage = MessageInputBox->GetText().ToString();
	if (UserMessage.IsEmpty()) return;

	ChatManagerComponent->SendUserMessage(UserMessage);

	MessageInputBox->SetText(FText::GetEmpty());

	MessageInputBox->SetKeyboardFocus();
}

// void ULlmChat::OnMessageReceived(const FString& Message, bool bIsUserMessage)
// {
// 	// if (!ScrollBox || !MessageRowWidgetClass) return;
// 	//
// 	// UMessageRow* NewRow = CreateWidget<UMessageRow>(GetOwningPlayer(), MessageRowWidgetClass);
// 	// if (!NewRow) return;
// 	//
// 	// NewRow->SetMessage(Message, bIsUserMessage);
// 	//
// 	// UScrollBoxSlot* NewSlot = Cast<UScrollBoxSlot>(ScrollBox->AddChild(NewRow));
// 	// NewSlot->SetPadding(FMargin(0, 0, 0, 10));
// 	// ScrollBox->ScrollToEnd();
//
// 	EChatSendType SenderType = bIsUserMessage ? EChatSendType::User : EChatSendType::AI;
// 	// if (SenderType == EChatSendType::User)
// 	// {
// 	// 	bIsUserMessage = true;
// 	// }
// 	// else if (SenderType == EChatSendType::AI || SenderType == EChatSendType::FirstAI || SenderType == EChatSendType::SecondAI)
// 	// {
// 	// 	bIsUserMessage = false;
// 	// }
// 	AddMessageToUI(Message, SenderType);
// }

void ULlmChat::AddMessageToUI(const FString& Message, EChatSendType SenderType)
{
	if (!ScrollBox || !MessageRowWidgetClass) return;

	UMessageRow* NewRow = CreateWidget<UMessageRow>(GetOwningPlayer(), MessageRowWidgetClass);
	if (!NewRow) return;

	// 새로운 SetMessageNew 함수 사용
	NewRow->SetMessage(Message, SenderType);

	UScrollBoxSlot* NewSlot = Cast<UScrollBoxSlot>(ScrollBox->AddChild(NewRow));
	NewSlot->SetPadding(FMargin(0, 0, 0, 10));
	ScrollBox->ScrollToEnd();
}

void ULlmChat::OnErrorOccurred(const FString& ErrorMessage)
{
	// FString Error = FString::Printf(TEXT("%s"), *ErrorMessage);
	// OnMessageReceived(Error, false);

	AddMessageToUI(ErrorMessage, EChatSendType::System);
}

void ULlmChat::OnChatMessageReceived(const FLlamaChatMessage& ChatMessage)
{
	AddMessageToUI(ChatMessage.Message, ChatMessage.Sender);
}


void ULlmChat::OnSettingButtonClicked()
{
 	if (!ChatSetting)
	{
		if (ChatSettingWidgetClass)
		{
			ChatSetting = CreateWidget<UChatSetting>(GetOwningPlayer(), ChatSettingWidgetClass);

			if (ChatSetting && ChatManagerComponent)
			{
				ChatSetting->SetChatManagerComponent(ChatManagerComponent);
			}
			
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("WBP_ChatSetting이 설정되지 않았습니다."));
		}
	}

	if (ChatSetting)
	{
		if (!ChatSetting->IsInViewport())
		{
			ChatSetting->AddToViewport(200);
		}
		ChatSetting->ShowSetting();
	}
}

