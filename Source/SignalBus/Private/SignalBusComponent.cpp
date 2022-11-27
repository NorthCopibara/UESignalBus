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
	return SignalBusService; 
}

void USignalBusComponent::InitializeComponent()
{
	SignalBusService = Cast<USignalBusService>(USignalBusService::StaticClass()->GetDefaultObject());
	Super::InitializeComponent();
}

void USignalBusComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	SignalBusService->Clear();
	Super::EndPlay(EndPlayReason);
}

