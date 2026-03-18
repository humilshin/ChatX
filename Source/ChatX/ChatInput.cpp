// Fill out your copyright notice in the Description page of Project Settings.


#include "ChatInput.h"

#include "Components/EditableTextBox.h"
#include "BasePlayerController.h"

void UChatInput::NativeConstruct()
{
	Super::NativeConstruct();

	if (EditableTextBox_ChatInput->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatInputTextCommitted) == false)
	{
		EditableTextBox_ChatInput->OnTextCommitted.AddDynamic(this, &ThisClass::OnChatInputTextCommitted);		
	}	
}

void UChatInput::NativeDestruct()
{
	Super::NativeDestruct();

	if (EditableTextBox_ChatInput->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatInputTextCommitted) == true)
	{
		EditableTextBox_ChatInput->OnTextCommitted.RemoveDynamic(this, &ThisClass::OnChatInputTextCommitted);
	}
}

void UChatInput::SetInputEnabled(bool bEnabled)
{
	if (IsValid(EditableTextBox_ChatInput) == true)
	{
		EditableTextBox_ChatInput->SetIsEnabled(bEnabled);
	}
}

void UChatInput::OnChatInputTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		APlayerController* OwningPlayerController = GetOwningPlayer();
		if (IsValid(OwningPlayerController) == true)
		{
			ABasePlayerController* OwningBasePC = Cast<ABasePlayerController>(OwningPlayerController);
			if (IsValid(OwningBasePC) == true)
			{
				OwningBasePC->SetChatMessageString(Text.ToString());

				EditableTextBox_ChatInput->SetText(FText());
			}
		}
	}
}
