#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestNode.generated.h"

UCLASS()
class TEAM9_PROJECT_API ATestNode : public AActor
{
	GENERATED_BODY()
	
public:
	ATestNode();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	ATestNode* GetNextTile();

public:
	UPROPERTY(EditAnywhere)
	ATestNode* NextNode;

	UPROPERTY()
	FVector WorldLocation;

	UPROPERTY(EditAnywhere);
	UStaticMeshComponent* MeshComp;
};
