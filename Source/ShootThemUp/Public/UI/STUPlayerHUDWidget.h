// Shoot Them Up , All Right Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUCoreTypes.h"//�Զ���ĺ������࣬�����˽ṹ�塢ί�е�
#include "STUPlayerHUDWidget.generated.h"


UCLASS()
class SHOOTTHEMUP_API USTUPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	float GetHealthPercent() const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	bool GetCurrentWeaponUIData(FWeaponUIData& UIData) const;//�������GetWeaponUIData�����İ�װ����

	UFUNCTION(BlueprintCallable, Category = "UI")
	bool GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	bool IsPlayerAlive() const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	bool IsPlayerSpectating() const;

private:
	//USTUWeaponComponent* GetWeaponComponent() const;//��ȡ��ǰ���������ָ�뺯�����Ա㸴��
	////�õ���USTUWeaponComponent������ǰ����
	//USTUHealthComponent* GetHealthComponent() const;
};
