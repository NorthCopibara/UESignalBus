// Fill out your copyright notice in the Description page of Project Settings.


#include "SignalBusComponent.h"


USignalBusComponent::USignalBusComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
}

USignalBusService* USignalBusComponent::GetSignalBus() const
{
	checkf(SignalBusService, TEXT("Signal bus was not initialization"))
	UE_LOG(LogTemp, Warning, TEXT("Test_Get"))
	return SignalBusService; 
}

void USignalBusComponent::InitializeComponent()
{
	SignalBusService = Cast<USignalBusService>(USignalBusService::StaticClass()->GetDefaultObject());
	UE_LOG(LogTemp, Warning, TEXT("Test_Init"))
	Super::InitializeComponent();
}

void USignalBusComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	//TODO: clear
	UE_LOG(LogTemp, Warning, TEXT("Test_Deinit"))
	SignalBusService->Clear();
	Super::EndPlay(EndPlayReason);
}

