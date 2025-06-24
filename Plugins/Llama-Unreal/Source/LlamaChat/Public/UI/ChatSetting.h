// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChatSetting.generated.h"

struct FAvailableModel;

USTRUCT()
struct FPromptPreset
{
	GENERATED_BODY()

	FString PresetName;
	FString PromptText;

	FPromptPreset() {}

	FPromptPreset(const FString& PresetName, const FString& PromptText) : PresetName(PresetName), PromptText(PromptText) {}
};

class UChatManagerComponent;
/**
 * 
 */
UCLASS()
class LLAMACHAT_API UChatSetting : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, Category="Setting")
	void ShowSetting();

	UFUNCTION(BlueprintCallable, Category="Setting")
	void HideSetting();

	void SetChatManagerComponent(UChatManagerComponent* ManagerComponent);

protected:
	UPROPERTY(meta=(BindWidget))
	class UButton* CloseButton;

	UPROPERTY(meta=(BindWidget))
	class UComboBoxString* SelectComboBox;

	UPROPERTY(meta=(BindWidget))
	class UMultiLineEditableText* SystemPromptInputTextBox;

	UPROPERTY(meta=(BindWidget))
	class UButton* ApplyButton;

	UPROPERTY(meta=(BindWidget))
	class UButton* CancelButton;

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* CurrentModelTextBlock;

	UPROPERTY(meta=(BindWidget))
	class UComboBoxString* ModelSelectComboBox;

	UPROPERTY(meta=(BindWidget))
	class UButton* ChangeModelButton;

private:
	UFUNCTION()
	void OnCloseButtonClicked();

	UFUNCTION()
	void OnApplyButtonClicked();

	UFUNCTION()
	void OnCancelButtonClicked();

	UFUNCTION()
	void OnPresetSelected(FString SelectedPreset, ESelectInfo::Type SelectionType);

	UFUNCTION()
	void OnModelSelected(FString SelectedModel, ESelectInfo::Type SelectionType);

	UFUNCTION()
	void OnChangeModelButtonClicked();

	void RefreshModelList();

	UPROPERTY()
	UChatManagerComponent* ChatManagerComponent;

	TArray<FPromptPreset> PromptPresets;

	TArray<FAvailableModel> AvailableModels;
	FString SelectedModelPath;

	
};
