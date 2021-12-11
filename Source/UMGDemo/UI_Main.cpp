// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Main.h"

UUI_Main::UUI_Main(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

bool UUI_Main::Initialize()
{
	if (Super::Initialize() == false)
		return false;

	return true;
}