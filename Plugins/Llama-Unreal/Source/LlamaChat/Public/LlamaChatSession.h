// Copyright YourCompanyName, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LlamaDataTypes.h"    // FLLMModelParams, EChatTemplateRole, 델리게이트 타입 등 사용
#include "LlamaChatSession.generated.h"


class UChatManagerComponent;
class ULlamaComponent;

UCLASS()
class LLAMACHAT_API ALlamaChatSession : public AActor
{
    GENERATED_BODY()

public:
    ALlamaChatSession();

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Component", meta = (AllowPrivateAccess = "true"))
    ULlamaComponent* LlamaComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Component", meta = (AllowPrivateAccess = "true"))
    UChatManagerComponent* ChatManagerComponent;
    

    
    
  

    
};
