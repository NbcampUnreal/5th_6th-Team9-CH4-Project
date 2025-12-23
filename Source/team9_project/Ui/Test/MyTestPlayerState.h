#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MyTestPlayerState.generated.h"

UCLASS()
class TEAM9_PROJECT_API AMyTestPlayerState : public APlayerState
{
    GENERATED_BODY()

public:
    AMyTestPlayerState();

    // UI용 표시 이름 (복제로 모든 클라이언트에 전달됨)
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Lobby")
    FString DisplayName;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Lobby")
    bool bIsReady = false;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Lobby")
    int32 PlayerNumber = -1;

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};