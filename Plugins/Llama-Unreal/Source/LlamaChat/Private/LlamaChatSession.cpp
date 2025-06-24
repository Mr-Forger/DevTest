// Copyright YourCompanyName, Inc. All Rights Reserved.

#include "LlamaChatSession.h"

#include "LlamaChatUISubsystem.h"
#include "LlamaComponent.h"
#include "Component/ChatManagerComponent.h"

ALlamaChatSession::ALlamaChatSession()
{
    PrimaryActorTick.bCanEverTick = false;

    LlamaComponent = CreateDefaultSubobject<ULlamaComponent>(TEXT("LlamaComponent"));
    ChatManagerComponent = CreateDefaultSubobject<UChatManagerComponent>(TEXT("ChatManagerComponent"));
    
}

void ALlamaChatSession::BeginPlay()
{
    Super::BeginPlay();

    if (LlamaComponent)
    {
        LlamaComponent->LoadModel(true);  // 이게 있어야 모델 로드 가능
        
    }

    if (UWorld* World = GetWorld())
    {
        if (ULlamaChatUISubsystem* UISubsystem = World->GetSubsystem<ULlamaChatUISubsystem>())
        {
            UISubsystem->RegisterChatSession(this); 
        }
    }
}

void ALlamaChatSession::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    if (UWorld* World = GetWorld())
    {
        if (ULlamaChatUISubsystem* UISubsystem = World->GetSubsystem<ULlamaChatUISubsystem>())
        {
            UISubsystem->UnregisterChatSession();
        }
    }
    
    Super::EndPlay(EndPlayReason);
}

void ALlamaChatSession::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
