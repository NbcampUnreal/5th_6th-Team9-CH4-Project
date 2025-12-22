#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Team9GameInstance.generated.h"

UCLASS()
class TEAM9_PROJECT_API UTeam9GameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	void SetCurrentRound(int32 CurrentR);

	int32 GetCurrentRound();

	void PropertyInit();

private:
	int32 CurrentRound;
};
