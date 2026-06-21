// Copyright Epic Games, Inc. All Rights Reserved.

#include "Game/MenuGameMode.h"
#include "UI/W_MainMenu.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

AMenuGameMode::AMenuGameMode()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AMenuGameMode::BeginPlay()
{
	Super::BeginPlay();
	ShowMenu();
}

void AMenuGameMode::ShowMenu()
{
	if (!MainMenuWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("MainMenuWidgetClass is not set in MenuGameMode!"));
		return;
	}

	if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
	{
		UW_MainMenu* MenuWidget = CreateWidget<UW_MainMenu>(PC, MainMenuWidgetClass);
		if (MenuWidget)
		{
			MenuWidget->AddToViewport(0);
		}
	}
}
