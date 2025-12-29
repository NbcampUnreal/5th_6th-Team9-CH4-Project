#include "MiniGame/T9_MiniGameResultWidget.h"
#include "Components/TextBlock.h"

void UT9_MiniGameResultWidget::SetResultText(const FText& NewText)
{
	if (Result)
	{
		Result->SetText(NewText);
	}
}
