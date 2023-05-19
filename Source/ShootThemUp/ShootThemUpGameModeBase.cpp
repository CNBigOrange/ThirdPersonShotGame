// Copyright Epic Games, Inc. All Rights Reserved.


#include "ShootThemUpGameModeBase.h"
#include "Player/STUBaseCharacter.h"
#include "Player/STUPlayerController.h"
#include "UI/STUGameHUD.h"

AShootThemUpGameModeBase::AShootThemUpGameModeBase()//游戏模式
{
	DefaultPawnClass = ASTUBaseCharacter::StaticClass();//游戏模式对应的默认棋子类
	PlayerControllerClass = ASTUPlayerController::StaticClass();//游戏模式默认的玩家控制器类
	HUDClass = ASTUGameHUD::StaticClass();//游戏模式默认的HUD
}
