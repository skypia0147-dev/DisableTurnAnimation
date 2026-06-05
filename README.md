# Disable Turn Animation

Skyrim Special Edition용 SKSE 플러그인으로, 플레이어가 제자리에서 회전할 때 재생되는 턴(Turn) 애니메이션을 비활성화합니다.

## 기능
- 플레이어의 애니메이션 그래프 변수 `fTurnAngle`을 강제로 `0.0`으로 유지합니다.
- 제자리 회전 시 발을 움직이는 애니메이션(Pivot)이 차단되어 즉각적인 회전 느낌을 줍니다.

## 요구 사항
- [CommonLibSSE-NG](https://github.com/CharmedBaryon/CommonLibSSE-NG) (빌드 시 필요)
- [Address Library for SKSE Plugins](https://www.nexusmods.com/skyrimspecialedition/mods/32444)

## 빌드 방법
1. CMake를 사용하여 프로젝트를 구성합니다.
2. `vcpkg`를 통해 의존성을 자동으로 설치합니다.
3. 빌드된 `DisableTurnAnimation.dll` 파일을 `Data/SKSE/Plugins` 폴더에 복사합니다.

## 개발자
- Smooth
