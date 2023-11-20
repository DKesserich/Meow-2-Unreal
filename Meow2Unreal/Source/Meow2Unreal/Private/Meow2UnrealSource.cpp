// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "Meow2UnrealSource.h"

#include "ILiveLinkClient.h"
#include "LiveLinkTypes.h"
#include "Roles/LiveLinkAnimationRole.h"
#include "Roles/LiveLinkAnimationTypes.h"

#include "Async/Async.h"
#include "Common/UdpSocketBuilder.h"
#include "HAL/RunnableThread.h"
#include "Json.h"
#include "Sockets.h"
#include "SocketSubsystem.h"

#define LOCTEXT_NAMESPACE "JSONLiveLinkSource"

#define RECV_BUFFER_SIZE 1024 * 1024

FMeow2UnrealSource::FMeow2UnrealSource(FIPv4Endpoint InEndpoint)
: Socket(nullptr)
, Stopping(false)
, Thread(nullptr)
, WaitTime(FTimespan::FromMilliseconds(100))
{
	// defaults
	DeviceEndpoint = InEndpoint;

	SourceStatus = LOCTEXT("SourceStatus_DeviceNotFound", "Device Not Found");
	SourceType = LOCTEXT("JSONLiveLinkSourceType", "Meow2Unreal");
	SourceMachineName = LOCTEXT("JSONLiveLinkSourceMachineName", "localhost");

	//setup socket
	if (DeviceEndpoint.Address.IsMulticastAddress())
	{
		Socket = FUdpSocketBuilder(TEXT("JSONSOCKET"))
			.AsNonBlocking()
			.AsReusable()
			.BoundToPort(DeviceEndpoint.Port)
			.WithReceiveBufferSize(RECV_BUFFER_SIZE)

			.BoundToAddress(FIPv4Address::Any)
			.JoinedToGroup(DeviceEndpoint.Address)
			.WithMulticastLoopback()
			.WithMulticastTtl(2);
					
	}
	else
	{
		Socket = FUdpSocketBuilder(TEXT("JSONSOCKET"))
			.AsNonBlocking()
			.AsReusable()
			.BoundToAddress(DeviceEndpoint.Address)
			.BoundToPort(DeviceEndpoint.Port)
			.WithReceiveBufferSize(RECV_BUFFER_SIZE);
	}

	RecvBuffer.SetNumUninitialized(RECV_BUFFER_SIZE);

	if ((Socket != nullptr) && (Socket->GetSocketType() == SOCKTYPE_Datagram))
	{
		SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);

		Start();

		SourceStatus = LOCTEXT("SourceStatus_Receiving", "Receiving");
	}
}

FMeow2UnrealSource::~FMeow2UnrealSource()
{
	Stop();
	if (Thread != nullptr)
	{
		Thread->WaitForCompletion();
		delete Thread;
		Thread = nullptr;
	}
	if (Socket != nullptr)
	{
		Socket->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(Socket);
	}
}

void FMeow2UnrealSource::ReceiveClient(ILiveLinkClient* InClient, FGuid InSourceGuid)
{
	Client = InClient;
	SourceGuid = InSourceGuid;
}


bool FMeow2UnrealSource::IsSourceStillValid() const
{
	// Source is valid if we have a valid thread and socket
	bool bIsSourceValid = !Stopping && Thread != nullptr && Socket != nullptr;
	return bIsSourceValid;
}


bool FMeow2UnrealSource::RequestSourceShutdown()
{
	Stop();

	return true;
}
// FRunnable interface

void FMeow2UnrealSource::Start()
{
	ThreadName = "JSON UDP Receiver ";
	ThreadName.AppendInt(FAsyncThreadIndex::GetNext());
	
	Thread = FRunnableThread::Create(this, *ThreadName, 128 * 1024, TPri_AboveNormal, FPlatformAffinity::GetPoolThreadMask());
}

void FMeow2UnrealSource::Stop()
{
	Stopping = true;
}

uint32 FMeow2UnrealSource::Run()
{
	TSharedRef<FInternetAddr> Sender = SocketSubsystem->CreateInternetAddr();
	
	while (!Stopping)
	{
		if (Socket->Wait(ESocketWaitConditions::WaitForRead, WaitTime))
		{
			uint32 Size;

			while (Socket->HasPendingData(Size))
			{
				int32 Read = 0;

				if (Socket->RecvFrom(RecvBuffer.GetData(), RecvBuffer.Num(), Read, *Sender))
				{
					if (Read > 0)
					{
						TSharedPtr<TArray<uint8>, ESPMode::ThreadSafe> ReceivedData = MakeShareable(new TArray<uint8>());
						ReceivedData->SetNumUninitialized(Read);
						memcpy(ReceivedData->GetData(), RecvBuffer.GetData(), Read);
						AsyncTask(ENamedThreads::GameThread, [this, ReceivedData]() { HandleReceivedData(ReceivedData); });
					}
				}
			}
		}
	}
	return 0;
}

void FMeow2UnrealSource::HandleReceivedData(TSharedPtr<TArray<uint8>, ESPMode::ThreadSafe> ReceivedData)
{
	FString JsonString;
JsonString.Empty(ReceivedData->Num());
for (uint8& Byte : *ReceivedData.Get())
{
	JsonString += TCHAR(Byte);
}
//UE_LOG(LogTemp, Warning, TEXT("This is the data: %s"), *JsonString);



TSharedPtr<FJsonObject> JsonObject;
TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
// UE_LOG(LogTemp, Warning, TEXT("Some warning message2"));


if (FJsonSerializer::Deserialize(Reader, JsonObject))
{
	//Since this will only work with MeowFace, our LiveLink Subject is gonna be MeowFace. Will look into exposing this in the UI on the off chance someone wants to connect multiple MeowFaces to the same session.
	FName SubjectName("MeowFace");

	FLiveLinkStaticDataStruct StaticDataStruct = FLiveLinkStaticDataStruct(FLiveLinkSkeletonStaticData::StaticStruct());
	FLiveLinkSkeletonStaticData& StaticData = *StaticDataStruct.Cast<FLiveLinkSkeletonStaticData>();

	FLiveLinkFrameDataStruct FrameDataStruct = FLiveLinkFrameDataStruct(FLiveLinkAnimationFrameData::StaticStruct());
	FLiveLinkAnimationFrameData& FrameData = *FrameDataStruct.Cast<FLiveLinkAnimationFrameData>();

	

	const TArray<TSharedPtr<FJsonValue>>* BlendShapeArray;
	if (JsonObject->TryGetArrayField(TEXT("BlendShapes"), BlendShapeArray))
	{
		//Initialize StaticData and FrameData to the length of the BlendShapes array +3 because we need to add some more properties later and appending to the array causes problems
		StaticData.PropertyNames.SetNumUninitialized(BlendShapeArray->Num()+3);
		FrameData.PropertyValues.SetNumUninitialized(BlendShapeArray->Num()+3);

		//For every item in the BlendShapes array convert them into JSON object and write their values into the StaticData and FrameData arrays.
		for (int BlendIdx = 0; BlendIdx < BlendShapeArray->Num(); ++BlendIdx)
		{
			const TSharedPtr<FJsonValue>& Shape = (*BlendShapeArray)[BlendIdx];
			const TSharedPtr<FJsonObject> ShapeObject = Shape->AsObject();
			FString KeyName;
			if (ShapeObject->TryGetStringField(TEXT("k"), KeyName))
			{
				//UE_LOG(LogTemp, Log, TEXT("ShapeKey %s"), *KeyName);
				StaticData.PropertyNames[BlendIdx] = FName(*KeyName);
			}
			float KeyValue;
			if (ShapeObject->TryGetNumberField(TEXT("v"), KeyValue))
			{
				//UE_LOG(LogTemp, Log, TEXT("Shape Value %f"), KeyValue);
				FrameData.PropertyValues[BlendIdx] = KeyValue;
			}
		}
		//Combine Left and Right Head Rolls from MeowFace into a single headRoll property			
		StaticData.PropertyNames[BlendShapeArray->Num()] = "headRoll";
		float LeftRoll;
		StaticData.FindPropertyValue(FrameData, "headRollLeft", LeftRoll);
		float RightRoll;
		StaticData.FindPropertyValue(FrameData, "headRollRight", RightRoll);
		FrameData.PropertyValues[BlendShapeArray->Num()] = (LeftRoll + (RightRoll * -1));

		//Combine Head Up and Head Down into a single headPitch property
		StaticData.PropertyNames[BlendShapeArray->Num()+1] = "headPitch";
		float HeadDown;
		StaticData.FindPropertyValue(FrameData, "headDown", HeadDown);
		float HeadUp;
		StaticData.FindPropertyValue(FrameData, "headUp", HeadUp);
		FrameData.PropertyValues[BlendShapeArray->Num()+1] = (HeadUp + (HeadDown * -1));

		//Combine headLeft and headRight into a single headYaw property
		StaticData.PropertyNames[BlendShapeArray->Num()+2] = "headYaw";
		float HeadLeft;
		StaticData.FindPropertyValue(FrameData, "headLeft", HeadLeft);
		float HeadRight;
		StaticData.FindPropertyValue(FrameData, "headRight", HeadRight);
		FrameData.PropertyValues[BlendShapeArray->Num()+2] = (HeadLeft + (HeadRight * -1));
	}		
	else
	{
		UE_LOG(LogTemp, Log, TEXT("No BlendShape Array Found"));
	}

	

	Client->PushSubjectStaticData_AnyThread({ SourceGuid, SubjectName }, ULiveLinkAnimationRole::StaticClass(), MoveTemp(StaticDataStruct));		
	Client->PushSubjectFrameData_AnyThread({ SourceGuid, SubjectName }, MoveTemp(FrameDataStruct));
	EncounteredSubjects.Add(SubjectName);
	}
}

#undef LOCTEXT_NAMESPACE
