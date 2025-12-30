#include "MiniGame/Racing/T9_RacingInGameWidget.h"
#include "Player/MyPlayerState.h"
#include "MiniGame/Racing/T9_FinishLineTrigger.h"
#include "GameFramework/GameStateBase.h"
#include "EngineUtils.h" 
#include "Components/TextBlock.h"

void UT9_RacingInGameWidget::NativeConstruct()
{
	Super::NativeConstruct();
    for (TActorIterator<AT9_FinishLineTrigger> It(GetWorld()); It; ++It)
    {
        CachedFinishLine = *It;
        break;
    }
}

void UT9_RacingInGameWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
    UpdateRanking();
}

void UT9_RacingInGameWidget::UpdateRanking()
{
    if (CachedFinishLine)
    {
        TArray<FRacingDistanceInfo> Distances;

        AGameStateBase* GS = GetWorld()->GetGameState();
        if (!GS) return;
        for (APlayerState* PS : GS->PlayerArray)
        {
            AMyPlayerState* MyPS = Cast<AMyPlayerState>(PS);
            if (!MyPS) continue;

            APawn* Pawn = PS->GetPawn();
            if (!Pawn) continue;

            float Dist = FVector::Dist(
                Pawn->GetActorLocation(),
                CachedFinishLine->GetActorLocation()
            );

            Distances.Add({
                MyPS->GetPlayerNumber(),
                Dist
                });
        }
        Distances.Sort([](
            const FRacingDistanceInfo& A,
            const FRacingDistanceInfo& B)
            {
                return A.Distance < B.Distance;
            });

        UpdateRankingUI(Distances);
    }
}

void UT9_RacingInGameWidget::UpdateRankingUI(const TArray<FRacingDistanceInfo>& Distances)
{
    if (Ranking)
    {
        FString RankingString;
        for (int32 i = 0; i < Distances.Num(); i++)
        {
            RankingString += FString::Printf(
                TEXT("%dÀ§ : Player %d (%.1fm)\n"),
                i + 1,
                Distances[i].PlayerId,
                Distances[i].Distance
            );
        }

        Ranking->SetText(FText::FromString(RankingString));
    }
}
