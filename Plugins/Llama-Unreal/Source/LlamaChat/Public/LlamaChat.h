#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

/**
 * LlamaChat 모듈 - LLM 채팅 UI와 관리 시스템
 * 
 * 이 모듈은 LlamaCore의 기능을 활용하여
 * 사용자 친화적인 채팅 인터페이스를 제공합니다.
 */
class FLlamaChatModule : public IModuleInterface
{
public:
	/** IModuleInterface 구현 */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
    
	/**
	 * 싱글톤 인스턴스 가져오기
	 * @return LlamaChat 모듈의 싱글톤 인스턴스
	 */
	static inline FLlamaChatModule& Get()
	{
		return FModuleManager::LoadModuleChecked<FLlamaChatModule>("LlamaChat");
	}
    
	/**
	 * 모듈이 로드되었는지 확인
	 * @return 모듈 로드 여부
	 */
	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("LlamaChat");
	}
};