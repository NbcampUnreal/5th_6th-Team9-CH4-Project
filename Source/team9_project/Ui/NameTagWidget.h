#pragma once

#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "NameTagWidget.generated.h"

UCLASS()
class TEAM9_PROJECT_API UNameTagWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    void SetName(FString Name)
    {
        if (Text_Name) Text_Name->SetText(FText::FromString(Name));
    }

    UPROPERTY(meta = (BindWidget))
    UTextBlock* Text_Name;
};