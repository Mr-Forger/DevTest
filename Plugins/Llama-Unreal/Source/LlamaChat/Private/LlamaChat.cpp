#include "LlamaChat.h"

#define LOCTEXT_NAMESPACE "FLlamaChatModule"

void FLlamaChatModule::StartupModule()
{
	// 모듈이 시작될 때 실행되는 코드
	UE_LOG(LogTemp, Log, TEXT("[LlamaChat] Module Started"));
    
	// 여기에 초기화 코드 추가 가능
	// 예: 커스텀 에셋 타입 등록, 에디터 확장 등
}

void FLlamaChatModule::ShutdownModule()
{
	// 모듈이 종료될 때 실행되는 코드
	UE_LOG(LogTemp, Log, TEXT("[LlamaChat] Module Shutdown"));
    
	// 여기에 정리 코드 추가 가능
	// 예: 할당된 리소스 해제, 등록 해제 등
}

#undef LOCTEXT_NAMESPACE

// 이 매크로가 반드시 있어야 모듈이 작동합니다!
IMPLEMENT_MODULE(FLlamaChatModule, LlamaChat)