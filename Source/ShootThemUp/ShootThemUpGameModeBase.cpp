// Copyright Epic Games, Inc. All Rights Reserved.


#include "ShootThemUpGameModeBase.h"
#include "Player/STUBaseCharacter.h"
#include "Player/STUPlayerController.h"
#include "UI/STUGameHUD.h"

AShootThemUpGameModeBase::AShootThemUpGameModeBase()//��Ϸģʽ
{
	DefaultPawnClass = ASTUBaseCharacter::StaticClass();//��Ϸģʽ��Ӧ��Ĭ��������
	PlayerControllerClass = ASTUPlayerController::StaticClass();//��ϷģʽĬ�ϵ���ҿ�������
	HUDClass = ASTUGameHUD::StaticClass();//��ϷģʽĬ�ϵ�HUD
}
