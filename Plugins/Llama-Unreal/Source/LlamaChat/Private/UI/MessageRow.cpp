// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MessageRow.h"

#include "LlamaChatDataTypes.h"
#include "Components/TextBlock.h"

// void UMessageRow::SetMessage(const FString& MessageText, bool bIsUserMessage)
// {
// 	if (MessageTextBlock)
// 	{
// 		FString Prefix = bIsUserMessage ? TEXT("User: ") : TEXT("LLM: ");
// 		MessageTextBlock->SetText(FText::FromString(Prefix + MessageText));
// 		
// 	}
// }

void UMessageRow::SetMessage(const FString& MessageText, EChatSendType SenderType)
{
	if (MessageTextBlock)
	{
		FString Prefix;
		FLinearColor TextColor = FLinearColor::White;
		
		switch (SenderType)
		{
		case EChatSendType::User:
			Prefix = TEXT("User: ");
			break;
		case EChatSendType::AI:
			Prefix = TEXT("AI: ");
			TextColor = FLinearColor::Green;
			break;
		case EChatSendType::System:
			Prefix = TEXT("[System] ");
			break;
		case EChatSendType::FirstAI:
			Prefix = TEXT("[AI 1] ");
			TextColor = FLinearColor::Blue;
			break;
		case EChatSendType::SecondAI:
			Prefix = TEXT("[AI 2] ");
			TextColor = FLinearColor::Red;
			break;
		default:
			Prefix = TEXT("");
		}

		MessageTextBlock->SetText(FText::FromString(Prefix + MessageText));
		MessageTextBlock->SetColorAndOpacity(FSlateColor(TextColor));
		
		
	}
}
