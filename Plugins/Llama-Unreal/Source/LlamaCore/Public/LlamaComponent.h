// Copyright 2025-current Getnamo.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LlamaDataTypes.h"

#include "LlamaComponent.generated.h"

/**
* LLM(대규모 언어 모델)에 접근하기 위한 액터 컴포넌트 API입니다. 각 컴포넌트는 자체적인 모델 및 컨텍스트 상태를 래핑하며, 여러 개의 병렬 LLM을 허용합니다.
* 부모로부터 생명주기를 상속받으며, 일반적으로 시스템 유형의 액터에서는 레벨이 종료될 때 언로드됨을 의미합니다. 만약 레벨 전환 후에도
* LLM이 유지되기를 원한다면, LlamaSubsystem 사용을 고려하십시오.
*/
UCLASS(Category = "LLM", BlueprintType, meta = (BlueprintSpawnableComponent))
class LLAMACORE_API ULlamaComponent : public UActorComponent
{
    GENERATED_BODY()
public:
    ULlamaComponent(const FObjectInitializer &ObjectInitializer);
    ~ULlamaComponent();

    virtual void Activate(bool bReset) override;
    virtual void Deactivate() override;
    virtual void TickComponent(float DeltaTime,
                                ELevelTick TickType,
                                FActorComponentTickFunction* ThisTickFunction) override;

    // 주요 콜백, 생성된 각 토큰에 대해 업데이트됩니다.
    UPROPERTY(BlueprintAssignable)
    FOnTokenGeneratedSignature OnTokenGenerated;

    // 전체 응답이 수신되었을 때만 호출됩니다 (EOS 등). 일반적으로 대역폭 제한 작업이며, TGS(초당 토큰 생성 속도)가 주어집니다.
    UPROPERTY(BlueprintAssignable)
    FOnResponseGeneratedSignature OnResponseGenerated;

    // 구두점으로 응답을 분리하여 내보냅니다 (예: 문장 수준). 음성 생성 작업에 유용합니다.
    UPROPERTY(BlueprintAssignable)
    FOnPartialSignature OnPartialGenerated;

    // 일반적으로 처리량 제한 작업이며, PPS(초당 프롬프트 처리 속도)가 주어집니다.
    UPROPERTY(BlueprintAssignable)
    FOnPromptProcessedSignature OnPromptProcessed;

    // 임베딩 모드가 필요하며, 결과는 RAG(검색 증강 생성) 유형 작업에 적합합니다.
    UPROPERTY(BlueprintAssignable)
    FOnEmbeddingsSignature OnEmbeddings;

    // 모델이 생성을 멈출 때마다 발생합니다.
    UPROPERTY(BlueprintAssignable)
    FOnEndOfStreamSignature OnEndOfStream;

    UPROPERTY(BlueprintAssignable)
    FVoidEventSignature OnContextReset;

    UPROPERTY(BlueprintAssignable)
    FModelNameSignature OnModelLoaded;

    // 내부 오류를 포착합니다.
    UPROPERTY(BlueprintAssignable)
    FOnErrorSignature OnError;

    // 모델을 로드하기 전에 이 설정을 수정하십시오.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LLM Model Component")
    FLLMModelParams ModelParams;

    // 이 상태는 일반적으로 매 응답 후 업데이트됩니다.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LLM Model Component")
    FLLMModelState ModelState;

    // 설정
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LLM Model Component")
    bool bDebugLogModelOutput = false;

    // 복사 비용을 지불할지 여부를 토글합니다. 기본값은 true입니다.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LLM Model Component")
    bool bSyncPromptHistory = true;

    // ModelParams로부터 모델을 로드합니다. bForceReload가 true이면 이미 로드된 모델을 강제로 다시 로드합니다.
    UFUNCTION(BlueprintCallable, Category = "LLM Model Component")
    void LoadModel(bool bForceReload = true);

    UFUNCTION(BlueprintCallable, Category = "LLM Model Component")
    void UnloadModel();

    UFUNCTION(BlueprintPure, Category = "LLM Model Component")
    bool IsModelLoaded();


    // 프롬프트를 지워 새로운 컨텍스트를 허용합니다. 선택적으로 초기 시스템 프롬프트를 유지할 수 있습니다.
    UFUNCTION(BlueprintCallable, Category = "LLM Model Component")
    void ResetContextHistory(bool bKeepSystemPrompt = false);

    // LLM이 응답한 내용을 제거합니다.
    UFUNCTION(BlueprintCallable, Category = "LLM Model Component")
    void RemoveLastAssistantReply();

    // 사용자가 말한 내용과 LLM이 응답한 내용을 제거합니다.
    UFUNCTION(BlueprintCallable, Category = "LLM Model Component")
    void RemoveLastUserInput();

    // 주요 입력 함수입니다.
    UFUNCTION(BlueprintCallable, Category = "LLM Model Component")
    void InsertTemplatedPrompt(UPARAM(meta=(MultiLine=true)) const FString& Text, EChatTemplateRole Role = EChatTemplateRole::User, bool bAddAssistantBOS = false, bool bGenerateReply = true);

    UFUNCTION(BlueprintCallable, Category = "LLM Model Component")
    void InsertTemplatedPromptStruct(const FLlamaChatPrompt& ChatPrompt);

    // 추론 실행 전에 형식을 적용하지 않습니다.
    UFUNCTION(BlueprintCallable, Category = "LLM Model Component")
    void InsertRawPrompt(UPARAM(meta = (MultiLine = true)) const FString& Text, bool bGenerateReply = true);

    // 일반적으로 사용자로, 입력이 히스토리에 생성된 것처럼 가장하고 모든 다운스트림 함수가 트리거됩니다. 모델이 로드되지 않은 경우 KV 캐시는 업데이트되지 않습니다.
    UFUNCTION(BlueprintCallable, Category = "LLM Model Component - Impersonation via External API")
    void ImpersonateTemplatedPrompt(const FLlamaChatPrompt& ChatPrompt);

    // 외부 모델 추론을 우리 루프를 통해 공급하는 데 사용합니다 (예: 어시스턴트 토큰이 생성될 때). 다운스트림에서 로컬로 생성된 것처럼 가장합니다.
    UFUNCTION(BlueprintCallable, Category = "LLM Model Component - Impersonation via External API")
    void ImpersonateTemplatedToken(const FString& Token, EChatTemplateRole Role = EChatTemplateRole::Assistant, bool bIsEndOfStream = false);

    // 프롬프트를 수동으로 감싸고 싶을 때, 템플릿이 빈 문자열이면 기본 모델 템플릿이 적용됩니다. 참고: 이 함수는 스레드로부터 안전해야 하지만, 아직 철저히 테스트되지는 않았습니다.
    UFUNCTION(BlueprintPure, Category = "LLM Model Component")
    FString WrapPromptForRole(const FString& Text, EChatTemplateRole Role, const FString& OverrideTemplate);

    // 새 토큰 생성을 강제로 중지합니다.
    UFUNCTION(BlueprintCallable, Category = "LLM Model Component")
    void StopGeneration();

    UFUNCTION(BlueprintCallable, Category = "LLM Model Component")
    void ResumeGeneration();

    // 현재 포맷된 컨텍스트를 가져옵니다.
    UFUNCTION(BlueprintPure, Category = "LLM Model Component")
    FString RawContextHistory();

    UFUNCTION(BlueprintPure, Category = "LLM Model Component")
    FStructuredChatHistory GetStructuredChatHistory();

    // 이 함수는 임베딩 모드가 필요하며, 그렇지 않으면 실행되지 않습니다.
    UFUNCTION(BlueprintCallable, Category = "LLM Model Embedding Mode")
    void GeneratePromptEmbeddingsForText(const FString& Text);

private:
    class FLlamaNative* LlamaNative;
};