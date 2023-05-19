// Shoot Them Up , All Right Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "STUGameHUD.generated.h"

class USTUPlayerHUDWidget;

UCLASS()
class SHOOTTHEMUP_API ASTUGameHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	virtual void DrawHUD() override;

protected:
	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	//	TSubclassOf<UUserWidget> PlayerHUDWidgetClass;//创建一个UUserWidget的子类，T开头表示模板类

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
		TArray<TSubclassOf<UUserWidget>>  PlayerHUDWidgets;
	//TSubclassOf<UUserWidget>为UUserWidget的子类，在蓝图中选择UUserWidget的子类

	virtual void BeginPlay() override;

private:
	void DrawCrossHair();

};
