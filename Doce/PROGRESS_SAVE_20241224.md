# 인벤토리 & 아이템 블루프린트 복구 작업 진행 상황
## 2024-12-24 저장

---

## 프로젝트 정보
- **경로**: `D:\UnrealProject\5th_6th-Team9-CH4-Project`
- **엔진**: Unreal Engine 5 (C++)
- **상황**: Git 병합 중 블루프린트 손실, 처음부터 재생성 중

---

## ✅ 완료된 작업

### 0. 인벤토리 UI 완성
- **WBP_InventorySlot** - 개별 슬롯 위젯 완성
- **WBP_Inventory** - 인벤토리 컨테이너 완성
- **InventoryComponent 연동** - 완료
  - RefreshAllSlots 함수 구현
  - 슬롯 클릭 시 UseItem 호출
  - OnInventoryUpdated 델리게이트 바인딩
- **아이템 아이콘** - DT_ItemDataTable에 Icon 연결 완료

### 1. BP_RCCar (RC카 액터 블루프린트)
- **위치**: Content 폴더 내 (사용자 지정)
- **Parent Class**: Actor
- **구조**:
  ```
  BP_RCCar (Actor)
   └─ DefaultSceneRoot
       ├─ CarMesh (Static Mesh Component) - 임시 Cube
       └─ ExplosionRange (Sphere Component) - Radius: 300, Hidden
  ```
- **설정**: Replicates ✅, Always Relevant ✅

### 2. 이펙트 블루프린트 7개
모두 C++ 클래스를 상속받아 생성:

| 블루프린트명 | Parent Class | 추가 설정 |
|-------------|--------------|----------|
| BP_Effect_RCCar | Effect_RCCar | RCCarClass → BP_RCCar 연결 |
| BP_Effect_Shield | Effect_Shield | 없음 |
| BP_Effect_Shotgun | Effect_Shotgun | 없음 |
| BP_Effect_BaseballBat | Effect_BaseballBat | 없음 |
| BP_Effect_Teleporter | Effect_Teleporter | 없음 |
| BP_Effect_BrokenTeleporter | Effect_BrokenTeleporter | 없음 |
| BP_Effect_Beehive | Effect_Beehive | 없음 |

### 3. DT_ItemDataTable (데이터 테이블)
- **위치**: `Content/Item/Data/DT_ItemDataTable`
- **Row Structure**: FItemData
- **7개 Row 입력 완료**:

| Row Name | ItemID | ItemName | Description | DropWeight | OperationTimeLimit | EffectClass |
|----------|--------|----------|-------------|------------|-------------------|-------------|
| BaseballBat | BaseballBat | 야구방망이 | 마우스 방향 1타일 내 모든 플레이어에게 50 데미지 | 1.0 | 30.0 | BP_Effect_BaseballBat |
| Beehive | Beehive | 벌집 | 선택한 타일의 모든 플레이어에게 30 데미지 | 1.0 | 30.0 | BP_Effect_Beehive |
| Shield | Shield | 실드 | 피해 1회 완전 무효화 | 1.0 | 30.0 | BP_Effect_Shield |
| Shotgun | Shotgun | 샷건 | 마우스 방향으로 5발 발사, 총알당 10 데미지 | 1.0 | 30.0 | BP_Effect_Shotgun |
| Teleporter | Teleporter | 순간이동기 | 선택한 타일로 즉시 이동 | 1.0 | 30.0 | BP_Effect_Teleporter |
| BrokenTeleporter | BrokenTeleporter | 망가진 순간이동기 | 랜덤 타일로 즉시 이동 | 1.0 | 30.0 | BP_Effect_BrokenTeleporter |
| RCCar | RCCar | RC카 | WASD로 조작, Space로 폭발. 1타일 범위 50 데미지 | 1.0 | 30.0 | BP_Effect_RCCar |

### 4. ItemSubsystem 연결
- 코드에서 자동 로드하는 경로: `/Game/Item/Data/DT_ItemDataTable`
- 데이터 테이블을 해당 경로에 배치 완료

---

## 🔄 진행 중인 작업

### 이펙트 로직 보완 완료
모든 이펙트 데미지 로직 활성화됨:

| 이펙트 | 상태 | 설명 |
|--------|------|------|
| Effect_BaseballBat | ✅ 완료 | SphereOverlap + ApplyDamage (50 데미지) |
| Effect_Shotgun | ✅ 완료 | LineTrace 5발 + ApplyDamage (총알당 10 데미지) |
| Effect_Beehive | ✅ 완료 | 타일 선택 → 해당 타일 플레이어에게 30 데미지 |
| Effect_Shield | ✅ 완료 | ActiveEffect로 실드 등록 |
| Effect_Teleporter | ✅ 완료 | 선택 타일로 이동 |
| Effect_BrokenTeleporter | ✅ 완료 | 랜덤 타일로 이동 |
| Effect_RCCar | ✅ 완료 | RC카 스폰 + 폭발 데미지 |

---

## 📋 남은 작업

### 추가 UI (선택)
- WBP_ItemOperation (아이템 조작 중 UI - 타이머, 취소 버튼)
- WBP_InventoryFull (인벤토리 풀 팝업)

### 테스트
- 아이템 획득/사용 테스트
- 네트워크 동기화 테스트
- 각 이펙트 데미지 동작 확인

---

## 📁 관련 C++ 파일 경로

```
Source/team9_project/
├── Item/
│   ├── Data/
│   │   ├── ItemTypes.h           - 아이템 열거형
│   │   ├── ItemData.h            - FItemData 구조체
│   │   └── ItemUseContext.h      - 사용 컨텍스트
│   │
│   ├── Effects/
│   │   ├── ItemEffectBase.h/.cpp
│   │   ├── ItemEffectBase_Instant.h/.cpp
│   │   ├── ItemEffectBase_MouseAim.h/.cpp
│   │   ├── ItemEffectBase_TileTarget.h/.cpp
│   │   ├── ItemEffectBase_DirectControl.h/.cpp
│   │   └── Concrete/
│   │       ├── Effect_BaseballBat.h/.cpp
│   │       ├── Effect_Beehive.h/.cpp
│   │       ├── Effect_BrokenTeleporter.h/.cpp
│   │       ├── Effect_RCCar.h/.cpp
│   │       ├── Effect_Shield.h/.cpp
│   │       ├── Effect_Shotgun.h/.cpp
│   │       └── Effect_Teleporter.h/.cpp
│   │
│   └── ItemSubsystem.h/.cpp      - 아이템 데이터 관리
│
└── Inventory/
    └── InventoryComponent.h/.cpp - 인벤토리 컴포넌트
```

---

## 📖 FItemData 구조체 (참고)

```cpp
USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName ItemID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText ItemName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UTexture2D* Icon = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DropWeight = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float OperationTimeLimit = 30.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<UItemEffectBase> EffectClass;
};
```

---

## 📖 InventoryComponent 주요 함수/델리게이트 (참고)

```cpp
// 델리게이트
UPROPERTY(BlueprintAssignable)
FOnInventoryUpdated OnInventoryUpdated;  // 슬롯 업데이트 시

// 슬롯 배열
UPROPERTY(ReplicatedUsing=OnRep_Slots, BlueprintReadOnly)
TArray<FInventorySlot> Slots;  // 5개 슬롯

// 주요 함수
bool AddItem(FName ItemID);
bool RemoveItem(int32 SlotIndex);
bool UseItem(int32 SlotIndex);
FInventorySlot GetSlot(int32 SlotIndex) const;
bool IsSlotEmpty(int32 SlotIndex) const;
```

---

## 📖 문서 위치

- `Doce/ITEM_SYSTEM_GUIDE_FOR_CLAUDE_CODE.md` - 전체 가이드
- `Doce/README_ItemSystem_KR.md` - 시스템 설계서
- `Doce/ITEM_SYSTEM_SPEC.md` - 스펙 문서

---

## 다음 작업 제안

1. **WBP_Inventory 구조 단순화** - WBP_InventorySlot 없이 Button 5개 직접 배치
2. **InventoryComponent 연동** - 더 간단한 방식으로 재시도
3. **테스트** - PIE에서 아이템 획득/사용 확인
