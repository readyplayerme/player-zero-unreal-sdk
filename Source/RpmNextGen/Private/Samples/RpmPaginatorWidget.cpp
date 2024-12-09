// Copyright Ready Player Me


#include "Samples/RpmPaginatorWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

void URpmPaginatorWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (PreviousButton)
	{
		PreviousButton->OnClicked.AddDynamic(this, &URpmPaginatorWidget::OnPrevButtonClicked);
	}

	if (NextButton)
	{
		NextButton->OnClicked.AddDynamic(this, &URpmPaginatorWidget::OnNextButtonClicked);
	}
	UpdateState(FPagination());
}

void URpmPaginatorWidget::UpdatePageText()
{
	if (PageText)
	{
		PageText->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), CurrentPage, TotalPages)));
	}
}

void URpmPaginatorWidget::UpdateState(const FPagination& Pagination)
{
	TotalPages = Pagination.TotalPages;
	CurrentPage = Pagination.Page;
	UpdatePageText();

	if (PreviousButton)
	{
		PreviousButton->SetIsEnabled(Pagination.Page > 1);
	}

	if (NextButton)
	{
		NextButton->SetIsEnabled(Pagination.Page < Pagination.TotalPages);
	}
}

void URpmPaginatorWidget::OnPrevButtonClicked()
{
	PreviousButton->SetIsEnabled(false);
	NextButton->SetIsEnabled(false);
	if(CurrentPage > 1)
	{
		CurrentPage--;
		UpdatePageText();
	}
	OnPreviousButtonEvent.Broadcast();
}

void URpmPaginatorWidget::OnNextButtonClicked()
{
	PreviousButton->SetIsEnabled(false);
	NextButton->SetIsEnabled(false);
	if(CurrentPage < TotalPages)
	{
		CurrentPage++;
		UpdatePageText();
	}
	OnNextButtonEvent.Broadcast();
}