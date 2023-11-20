// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
#include "Meow2UnrealSourceFactory.h"
#include "Meow2UnrealSource.h"
#include "SMeow2UnrealSourceFactory.h"

#define LOCTEXT_NAMESPACE "Meow2UnrealSourceFactory"

FText UMeow2UnrealSourceFactory::GetSourceDisplayName() const
{
	return LOCTEXT("SourceDisplayName", "MeowFace To Unreal");
}

FText UMeow2UnrealSourceFactory::GetSourceTooltip() const
{
	return LOCTEXT("SourceTooltip", "Creates a connection to a MeowFace stream");
}

TSharedPtr<SWidget> UMeow2UnrealSourceFactory::BuildCreationPanel(FOnLiveLinkSourceCreated InOnLiveLinkSourceCreated) const
{
	return SNew(SMeow2UnrealSourceFactory)
		.OnOkClicked(SMeow2UnrealSourceFactory::FOnOkClicked::CreateUObject(this, &UMeow2UnrealSourceFactory::OnOkClicked, InOnLiveLinkSourceCreated));
}

TSharedPtr<ILiveLinkSource> UMeow2UnrealSourceFactory::CreateSource(const FString& InConnectionString) const
{
	FIPv4Endpoint DeviceEndPoint;
	if (!FIPv4Endpoint::Parse(InConnectionString, DeviceEndPoint))
	{
		return TSharedPtr<ILiveLinkSource>();
	}

	return MakeShared<FMeow2UnrealSource>(DeviceEndPoint);
}

void UMeow2UnrealSourceFactory::OnOkClicked(FIPv4Endpoint InEndpoint, FOnLiveLinkSourceCreated InOnLiveLinkSourceCreated) const
{
	InOnLiveLinkSourceCreated.ExecuteIfBound(MakeShared<FMeow2UnrealSource>(InEndpoint), InEndpoint.ToString());
}

#undef LOCTEXT_NAMESPACE