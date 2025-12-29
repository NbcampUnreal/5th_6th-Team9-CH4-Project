# 텔레포터 아이템 구현 작업 로그

**작업일**: 2025-12-29
**작업자**: Cho_SungMin

---

## 1. 개요

텔레포터 아이템의 로직을 완성하고, 네트워크 동기화 문제를 해결한 작업 내용입니다.

---

## 2. 수정된 파일 목록

| 파일 경로 | 수정 내용 |
|-----------|-----------|
| `Source/team9_project/Tile/TileManagerActor.h` | `GetTileCount()` 메서드 추가 |
| `Source/team9_project/Tile/TileManagerActor.cpp` | `GetTileCount()` 구현, `GetTile()` 범위 검증 추가 |
| `Source/team9_project/Item/Effects/ItemEffectBase_TileTarget.h` | Forward declaration 추가 |
| `Source/team9_project/Item/Effects/ItemEffectBase_TileTarget.cpp` | `InitializeTileList()` 완성 |
| `Source/team9_project/Item/Effects/Concrete/Effect_Teleporter.cpp` | TileManager 사용, NetMulticast RPC 호출 |
| `Source/team9_project/Item/Effects/Concrete/Effect_BrokenTeleporter.cpp` | TileManager 사용, NetMulticast RPC 호출 |
| `Source/team9_project/Player/PlayerCharacter.h` | `MultiRPC_Teleport(int32)` 선언 |
| `Source/team9_project/Player/PlayerCharacter.cpp` | `MultiRPC_Teleport` 구현 |
| `Source/team9_project/Player/MyPlayerState.h` | `TileIndex`, `CurrentHp`, `MaxHp`를 Replicated로 변경 |
| `Source/team9_project/Player/MyPlayerState.cpp` | `GetLifetimeReplicatedProps`에 복제 변수 추가 |

---

## 3. 주요 해결된 문제들

### 3.1 InitializeTileList() 미완성 문제

**문제**: `TotalTileCount`가 10으로 하드코딩되어 있었음

**해결**: TileManager에서 실제 타일 수를 가져오고, 플레이어의 현재 타일 인덱스로 초기화

```cpp
void UItemEffectBase_TileTarget::InitializeTileList()
{
    ATileManagerActor* TileManager = ATileManagerActor::Get(CurrentUser->GetWorld());
    TotalTileCount = TileManager->GetTileCount();

    // 플레이어의 현재 타일로 초기화
    ACameraPawn* CameraPawn = Cast<ACameraPawn>(CurrentUser);
    if (CameraPawn)
    {
        APlayerCharacter* PlayerChar = CameraPawn->GetPlayerCharacter();
        if (PlayerChar && PlayerChar->GetCurrentTile())
        {
            SelectedTileIndex = PlayerChar->GetCurrentTile()->GetIndex();
        }
    }
}
```

### 3.2 RPC에서 ATile* 포인터 전달 문제

**문제**: ATile이 Replicated가 아니어서 서버/클라이언트가 각자 스폰. 포인터 공유 불가.

**해결**: `ATile*` 대신 `int32 TileIndex`를 전달하고, 각 머신에서 TileManager를 통해 Tile을 찾음

```cpp
// 기존 (작동 안 함)
void MultiRPC_Teleport(ATile* TargetTile);

// 수정 후 (정상 작동)
void MultiRPC_Teleport(int32 TargetTileIndex);

void APlayerCharacter::MultiRPC_Teleport_Implementation(int32 TargetTileIndex)
{
    ATileManagerActor* TileManager = ATileManagerActor::Get(GetWorld());
    ATile* TargetTile = TileManager->GetTile(TargetTileIndex);
    // ...
}
```

### 3.3 텔레포트 후 이동 시 원래 위치로 돌아가는 문제

**문제**: `MyPlayerState`의 `TileIndex`가 Replicated가 아니어서 클라이언트에서 복제되지 않음

**해결**: `TileIndex`를 `UPROPERTY(Replicated)`로 선언하고 `GetLifetimeReplicatedProps`에 추가

```cpp
// MyPlayerState.h
UPROPERTY(Replicated)
int TileIndex;

// MyPlayerState.cpp
void AMyPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    // ...
    DOREPLIFETIME(ThisClass, TileIndex);
}
```

---

## 4. 네트워크 구조 이해

### 4.1 게임의 네트워크 특징

- **ATile**: Replicated가 아님. 서버/클라이언트가 각자 스폰
- **APlayerCharacter**: Replicated. 서버에서 스폰 후 클라이언트로 복제
- **이동 방식**: NetMulticast RPC로 모든 클라이언트에서 동일 로직 실행

### 4.2 올바른 RPC 파라미터

```
[올바른 방식]
- 기본 타입 (int32, float, FVector 등)
- Replicated Actor 포인터

[문제가 되는 방식]
- Non-Replicated Actor 포인터 (ATile* 등)
  → 클라이언트에서 nullptr 또는 유효하지 않은 포인터
```

---

## 5. 텔레포터 아이템 동작 흐름

### 5.1 정상 텔레포터 (TileTarget 타입)

```
1. 플레이어가 아이템 사용
2. ItemEffectBase_TileTarget::StartUse() → InitializeTileList()
3. A/D 키로 타일 순환 선택 (CycleTileTarget RPC)
4. Space/좌클릭으로 확정 (ConfirmItemUse)
5. Effect_Teleporter::ExecuteEffect()
   → PlayerChar->MultiRPC_Teleport(TargetIndex)
6. 서버 + 클라이언트에서 텔레포트 실행
```

### 5.2 망가진 텔레포터 (Instant 타입)

```
1. 플레이어가 아이템 사용
2. 즉시 Effect_BrokenTeleporter::ExecuteEffect()
3. 랜덤 타일 인덱스 선택
4. PlayerChar->MultiRPC_Teleport(RandomIndex)
5. 서버 + 클라이언트에서 텔레포트 실행
```

---

## 6. 향후 작업 (TODO)

- [ ] 선택된 타일 하이라이트 표시
- [ ] 마커/이펙트 스폰으로 목표 타일 시각화
- [ ] 카메라가 선택된 타일을 따라가기
- [ ] UI에 선택된 타일 정보 표시

---

## 7. 참고 사항

### 7.1 Z 오프셋

- 캐릭터 이동 시 Z 오프셋: **+140.0f** (`MultiRPCMove`와 동일)

### 7.2 타임아웃

- TileTarget 타입 아이템 제한 시간: **30초** (`TimeLimit = 30.0f`)

### 7.3 취소 동작

- ESC 키로 취소 시: 아이템 반환, 사용권 복구
- 타임아웃 시: 아이템 소멸, 사용권 소모
