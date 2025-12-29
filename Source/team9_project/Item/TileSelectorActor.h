// Cho_SungMin - 타일 선택 시각화 액터
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TileSelectorActor.generated.h"

class UStaticMeshComponent;
class UMaterialInstanceDynamic;

/**
 * 타일 선택 시 해당 타일 위에 표시되는 시각적 마커
 * 카메라가 이 액터를 따라가면서 타일 선택 UI를 제공
 * Replicated - 모든 클라이언트에서 보임
 */
UCLASS()
class TEAM9_PROJECT_API ATileSelectorActor : public AActor
{
	GENERATED_BODY()

public:
	ATileSelectorActor();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// 타겟 위치로 이동 (서버에서 호출 -> 모든 클라이언트에 복제)
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_SetTargetLocation(FVector NewLocation);

	// 셀렉터 표시/숨김
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_SetVisible(bool bVisible);

	// 현재 타겟 위치 반환 (카메라 추적용)
	UFUNCTION(BlueprintPure, Category = "TileSelector")
	FVector GetTargetLocation() const { return TargetLocation; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USceneComponent> RootComp;

	// 시각적 표시 메시 (화살표, 원 등)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> SelectorMesh;

	// 타겟 위치
	UPROPERTY(BlueprintReadOnly, Category = "TileSelector")
	FVector TargetLocation;

	// 부유 애니메이션용
	UPROPERTY(EditDefaultsOnly, Category = "TileSelector|Animation")
	float FloatAmplitude = 30.0f;

	UPROPERTY(EditDefaultsOnly, Category = "TileSelector|Animation")
	float FloatSpeed = 3.0f;

	UPROPERTY(EditDefaultsOnly, Category = "TileSelector|Animation")
	float RotationSpeed = 90.0f;

	// 타일 위 높이 오프셋
	UPROPERTY(EditDefaultsOnly, Category = "TileSelector")
	float HeightOffset = 200.0f;

private:
	float TimeAccumulator = 0.0f;
	FVector BaseLocation;
};
