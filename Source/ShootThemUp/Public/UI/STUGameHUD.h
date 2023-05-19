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
	//	TSubclassOf<UUserWidget> PlayerHUDWidgetClass;//����һ��UUserWidget�����࣬T��ͷ��ʾģ����

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
		TArray<TSubclassOf<UUserWidget>>  PlayerHUDWidgets;
	//TSubclassOf<UUserWidget>ΪUUserWidget�����࣬����ͼ��ѡ��UUserWidget������

	virtual void BeginPlay() override;

private:
	void DrawCrossHair();

};
