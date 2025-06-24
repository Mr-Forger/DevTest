// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ChatSetting.h"

#include "Component/ChatManagerComponent.h"
#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "Components/MultiLineEditableText.h"
#include "Components/TextBlock.h"

void UChatSetting::NativeConstruct()
{
	Super::NativeConstruct();

	if (CloseButton && ApplyButton && CancelButton && ChangeModelButton)
	{
		CloseButton->OnClicked.AddDynamic(this, &UChatSetting::OnCloseButtonClicked);
		ApplyButton->OnClicked.AddDynamic(this, &UChatSetting::OnApplyButtonClicked);
		CancelButton->OnClicked.AddDynamic(this, &UChatSetting::OnCancelButtonClicked);
		ChangeModelButton->OnClicked.AddDynamic(this, &UChatSetting::OnChangeModelButtonClicked);
	}

	PromptPresets.Add(FPromptPreset(TEXT("Assistant"), TEXT("You are a helpful assistant. Always answer in Korean.")));
	PromptPresets.Add(FPromptPreset(TEXT("Warrior"), TEXT("You are Warrior. Always answer in Korean.")));
	PromptPresets.Add(FPromptPreset(TEXT("Writer"), TEXT("You are Creative Writer. Always answer in Korean.")));
	PromptPresets.Add(FPromptPreset(TEXT("Auto Actor Spawner(Dev)"), TEXT("현재 300*300*300의 공간에 있습니다. 사용자가 좌표를 제공해줘 라고 요청하면 해당 공간의 범위내에서 무작위로 좌표를 오직 [x: "", y: "", z: ""] 형식으로만 답변해주세요.")));
	PromptPresets.Add(FPromptPreset(TEXT("Custom"), TEXT("")));
	

	if (SelectComboBox)
	{
		SelectComboBox->ClearOptions();
		for (const FPromptPreset& Preset : PromptPresets)
		{
			SelectComboBox->AddOption(Preset.PresetName);
		}

		SelectComboBox->SetSelectedOption(TEXT("Select"));
		SelectComboBox->OnSelectionChanged.AddDynamic(this, &UChatSetting::OnPresetSelected);
	}

	if (ModelSelectComboBox)
	{
		ModelSelectComboBox->OnSelectionChanged.AddDynamic(this, &UChatSetting::OnModelSelected);
	}

	
}

void UChatSetting::ShowSetting()
{
	SetVisibility(ESlateVisibility::Visible);

	RefreshModelList();
}

void UChatSetting::HideSetting()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void UChatSetting::SetChatManagerComponent(UChatManagerComponent* ManagerComponent)
{
	ChatManagerComponent = ManagerComponent;
}

void UChatSetting::OnCloseButtonClicked()
{
	HideSetting();
}

void UChatSetting::OnApplyButtonClicked()
{
	// TODO: 시스템 프롬프트를 선택 후 Apply 버튼을 눌렸을 시에 성공적으로 프롬프트가 적용되게 하기.
	if (!ChatManagerComponent || !SystemPromptInputTextBox)
	{
		UE_LOG(LogTemp, Warning, TEXT("ChatManagerComponent 또는 시스템 프롬프트 입력이 없습니다."));
		return;
	}

	FString NewPrompt = SystemPromptInputTextBox->GetText().ToString();
	
	if (NewPrompt.IsEmpty())
	{
		return;
	}
	
	ChatManagerComponent->ChangeSystemPrompt(NewPrompt, true);
	UE_LOG(LogTemp, Log, TEXT("시스템 프롬프트가 %s 로 변경되었습니다."), *NewPrompt);
}

void UChatSetting::OnCancelButtonClicked()
{
	HideSetting();
}

void UChatSetting::OnPresetSelected(FString SelectedPreset, ESelectInfo::Type SelectionType)
{
	if (SelectionType == ESelectInfo::Direct)
	{
		return;
	}
	/*
	 * TODO
	 * 프롬프트 프리셋을 정한 후 결정하는 코드 작성
	 */
	if (SelectedPreset == TEXT("Custom"))
	{
		SystemPromptInputTextBox->SetText(FText::GetEmpty());
		SystemPromptInputTextBox->SetHintText(FText::FromString(TEXT("원하는 AI의 성격을 만들어주세요!")));
	}
	
	for (FPromptPreset& Preset: PromptPresets)
	{
		if (Preset.PresetName == SelectedPreset)
		{
			if (SystemPromptInputTextBox)
			{
				SystemPromptInputTextBox->SetText(FText::FromString(Preset.PromptText));
				UE_LOG(LogTemp, Log, TEXT("프리셋: %s"), *SelectedPreset);
			}
			break;
		}
	}
}

void UChatSetting::OnModelSelected(FString SelectedModel, ESelectInfo::Type SelectionType)
{
	if (SelectionType == ESelectInfo::Direct)
	{
		return;
	}
	
	for (const FAvailableModel& Model : AvailableModels)
	{
		if (Model.DisplayName == SelectedModel)
		{
			SelectedModelPath = Model.RelativePath;
			break;
		}
	}
}

void UChatSetting::OnChangeModelButtonClicked()
{
	if (SelectedModelPath.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("모델 선택 해주세요"));
		return;
	}

	if (!ChatManagerComponent)
	{
		return;
	}

	ChatManagerComponent->ChangeModel(SelectedModelPath, true);
}
	

void UChatSetting::RefreshModelList()
{
		if (!ChatManagerComponent || !ModelSelectComboBox)
		{
			return;
		}

		if (CurrentModelTextBlock)
		{
			FString CurrentModel = ChatManagerComponent->GetCurrentModelName();
			CurrentModelTextBlock->SetText(FText::FromString(CurrentModel));
		}

		AvailableModels = ChatManagerComponent->GetAvailableModels();

		ModelSelectComboBox->ClearOptions();
		ModelSelectComboBox->AddOption(TEXT("Select Model"));

		for (const FAvailableModel& Model : AvailableModels)
		{
			ModelSelectComboBox->AddOption(Model.DisplayName);
		}
		ModelSelectComboBox->SetSelectedOption(TEXT("Select Model"));
}

