using UnrealBuildTool;

public class LlamaChat : ModuleRules
{
    public LlamaChat(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        
       
        PublicDependencyModuleNames.AddRange(new string[] 
        {
            "Core",
            "CoreUObject",
            "Engine",
            "UMG",              
            "Slate",            
            "SlateCore",
            "LlamaCore"
        });

        // Private 폴더에서만 사용할 모듈들
        PrivateDependencyModuleNames.AddRange(new string[] 
        {
            "EditorWidgets"     // 에디터 UI (필요시)
        });
        
        // 헤더 파일 경로 설정
        PublicIncludePaths.AddRange(new string[] 
        {
            // 필요시 추가
        });
        
        PrivateIncludePaths.AddRange(new string[] 
        {
            // 필요시 추가
        });
    }
}