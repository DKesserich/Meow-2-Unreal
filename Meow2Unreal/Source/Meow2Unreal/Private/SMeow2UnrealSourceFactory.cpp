// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "SMeow2UnrealSourceFactory.h"
#include "Interfaces/IPv4/IPv4Endpoint.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"

#define LOCTEXT_NAMESPACE "Meow2UnrealSourceEditor"

void SMeow2UnrealSourceFactory::Construct(const FArguments& Args)
{
	OkClicked = Args._OnOkClicked;

	FIPv4Endpoint Endpoint;
	Endpoint.Address = FIPv4Address::Any;
	Endpoint.Port = 54321;

	ChildSlot
	[
		SNew(SBox)
		.WidthOverride(250)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Left)
				.FillWidth(0.5f)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("MeowFace Port Number", "Port Number"))
				]
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Fill)
				.FillWidth(0.5f)
				[
					SAssignNew(EditabledText, SEditableTextBox)
					.Text(FText::FromString(Endpoint.ToString()))
					.OnTextCommitted(this, &SMeow2UnrealSourceFactory::OnEndpointChanged)
				]
			]
			+ SVerticalBox::Slot()
			.HAlign(HAlign_Right)
			.AutoHeight()
			[
				SNew(SButton)
				.OnClicked(this, &SMeow2UnrealSourceFactory::OnOkClicked)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("Ok", "Ok"))
				]
			]
		]
	];
}

void SMeow2UnrealSourceFactory::OnEndpointChanged(const FText& NewValue, ETextCommit::Type)
{
	TSharedPtr<SEditableTextBox> EditabledTextPin = EditabledText.Pin();
	if (EditabledTextPin.IsValid())
	{
		FIPv4Endpoint Endpoint;
		if (!FIPv4Endpoint::Parse(NewValue.ToString(), Endpoint))
		{
			Endpoint.Address = FIPv4Address::Any;
			Endpoint.Port = 54321;
			EditabledTextPin->SetText(FText::FromString(Endpoint.ToString()));
		}
	}
}

FReply SMeow2UnrealSourceFactory::OnOkClicked()
{
	TSharedPtr<SEditableTextBox> EditabledTextPin = EditabledText.Pin();
	if (EditabledTextPin.IsValid())
	{
		FIPv4Endpoint Endpoint;
		if (FIPv4Endpoint::Parse(EditabledTextPin->GetText().ToString(), Endpoint))
		{
			OkClicked.ExecuteIfBound(Endpoint);
		}
	}
	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE