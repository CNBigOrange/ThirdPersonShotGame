// Shoot Them Up , All Right Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUCoreTypes.h"//自定义的核心种类，集合了结构体、委托等
#include "STUPlayerHUDWidget.generated.h"


UCLASS()
class SHOOTTHEMUP_API USTUPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	float GetHealthPercent() const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	bool GetCurrentWeaponUIData(FWeaponUIData& UIData) const;//武器组件GetWeaponUIData函数的包装函数

	UFUNCTION(BlueprintCallable, Category = "UI")
	bool GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	bool IsPlayerAlive() const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	bool IsPlayerSpectating() const;

private:
	//USTUWeaponComponent* GetWeaponComponent() const;//获取当前武器组件的指针函数，以便复用
	////用到了USTUWeaponComponent，需向前申明
	//USTUHealthComponent* GetHealthComponent() const;
};
