### 사용방법 (5.5 버전으로 개발되었습니다)
우측 상단의 Code를 클릭 후 Download ZIP을 클릭해주세요
![image](https://github.com/user-attachments/assets/61f1b2f5-915e-4949-8876-710199da8fa7)
압축을 풀고 Saved 폴더로 들어가 Models라는 폴더를 새로 만들어 줍니다.
https://huggingface.co/unsloth/gemma-3-4b-it-GGUF
해당 링크로 가서 16bit 모델을 다운받아줍니다.
![image](https://github.com/user-attachments/assets/6c37922c-4b6f-4ea8-9d63-c1560c917f4c)

이 모델을 Saved/Models 폴더에다가 저장 또는 옮깁니다.

프로젝트를 실행 한 후 아웃라이너에 BP_LlamaChatSession이 배치되어있는지 체크 하고 없으면 컨텐츠 브라우저에서 검색 후 배치합니다.
BP_LlamaChatSession을 실행 시킨 후 좌측 상단에서 LlamaComponent를 클릭 후 우측의 디테일 패널에서 Path to Model 항목에서 경로를 이미지와 같이 지정해주세요. (./ 를 입력 후 Saved/Models폴더에 있는 이름을 그대로 복사 붙여넣기 하셔도 됩니다.)
![image](https://github.com/user-attachments/assets/2a8b6583-9f30-4faf-bb9b-c5e5da414d60)

이렇게 한 후 에디터를 플레이를 하고 키보드 1번 키를 누르면 채팅창이 활성화 됩니다.
![image](https://github.com/user-attachments/assets/259249b3-5669-4688-bcb8-e6af1b9a19c6)

최초 실행 시 모델 로드에 시간이 좀 걸립니다. 모델이 로드가 완료가 되면 output log에 다음과 같이 뜹니다.
![image](https://github.com/user-attachments/assets/cdefce26-0061-43b4-9fca-618f07509996)

이제 자유롭게 사용하시면 됩니다.

### 시스템 프롬프트 변경 및 모델 변경 방법
채팅 UI의 우측 상단의 흰 색 버튼을 클릭합니다.
![image](https://github.com/user-attachments/assets/f3c15afa-477b-4b31-9a4b-76a37b143d2d)

다음과 같은 UI가 뜨며, 여기서 시스템 프롬프트와 모델을 변경할 수 있습니다. 자세한 것은 노션에 있습니다.
단, 시스템 프롬프트에서 액터 배치 테스트를 위한 좌표 제공 프롬프트 프리셋은 "Auto Actor Spanwer(Dev)" 라는 이름으로 되어있습니다.

안되는 부분이 있으시면 카톡으로 연락 주세요!




