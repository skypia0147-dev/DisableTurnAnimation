# Disable Turn Animation

Skyrim Special Edition (SE/AE) 용 SKSE 플러그인.

## 개요
이 플러그인의 주된 목적은 스카이림에서 발생하는 **모든 턴(Turn) 애니메이션을 완전히 제거**하는 것입니다.

마우스를 돌릴 때마다 캐릭터가 버벅거리며 발을 돌리는 바닐라의 턴 애니메이션을 원천 차단(`TurnLeft`, `TurnRight` 이벤트 무시)합니다. 또한, 일반적인 방법으로는 차단되지 않던 **까다로운 예외 상황들까지 완벽하게 잡아냅니다**:
1. 무기를 꺼내거나 넣는(Equip / Unequip) 짧은 전환 순간
2. 캐릭터가 이동하다가 감속하여 정지하는 순간

이러한 예외 타이밍에도 `turnStop` 이벤트를 강제로 주입하여, 어떠한 상황에서도 캐릭터가 원치 않는 턴 애니메이션을 재생하지 않도록 부드럽게 방지합니다.

## 시스템 요구 사항
- Skyrim Special Edition (1.5.97) 또는 Anniversary Edition (1.6.x+)
- [SKSE64](https://skse.silverlock.org/)
- **[Address Library for SKSE Plugins](https://www.nexusmods.com/skyrimspecialedition/mods/32444)**: 필수 요구 사항

## 설치 방법
1. 모드 매니저(MO2, Vortex 등)를 통해 아카이브를 설치하거나,
2. `DisableTurnAnimation.dll` 파일을 `Data/SKSE/Plugins` 폴더에 직접 복사합니다.

## 호환성
* **True Directional Movement (TDM)**: 완벽 호환. 카메라는 분리 제어되며, 턴 억제 로직만 개입합니다.
* **CommonLibSSE-NG** 기반으로 제작되어 SE 및 AE 버전을 동시에 지원합니다 (독립적인 버전 확인 로직 불필요).

---

### 개발 문서 (Technical)
이 플러그인은 C++ 및 SKSE를 기반으로 작성되었습니다.
- 불필요한 메모리 하드코딩을 피하기 위해 **Address Library**를 활용하여 REL::ID로 함수를 호출합니다.
- 멀티스레드 환경에서의 안전성(Thread Safety)을 보장하기 위해 `std::atomic` 변수를 통해 이벤트 그래프와 메인 캐릭터 업데이트 루프 간의 동기화를 처리합니다.

**Author**: Smooth
