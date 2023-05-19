// Shoot Them Up , All Right Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUCoreTypes.h"//自定义的核心种类，集合了结构体、委托等
#include "STUWeaponComponent.generated.h"


class ASTUBaseWeapon;//提前声明基础武器类

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTTHEMUP_API USTUWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	USTUWeaponComponent();

	virtual void StartFire();
	void StopFire();
	virtual void NextWeapon();
	void Reload();

	bool GetCurrentWeaponUIData(FWeaponUIData& UIData) const;//不会更改类的内部状态可以用const
	//获取当前武器的UIData
	bool GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const;
	//获取当前武器的AmmoData

	bool TryToAddAmmo(TSubclassOf<ASTUBaseWeapon> WeaponType, int32 ClipsAmount);

	ASTUBaseWeapon* GetCurrentWeaponForAI() { return CurrentWeapon; }
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		TArray<FWeaponData> WeaponData;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		FName WeaponEquipSocketName = "WeaponSocket";

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		FName WeaponArmorySocketName = "ArmorySocket";

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
		UAnimMontage* EquipAnimMontage;

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY()//所有指针声明都要添加UPROPERTY()否则虚幻的垃圾收集器会出现问题
		ASTUBaseWeapon* CurrentWeapon = nullptr;

	UPROPERTY()
		TArray<ASTUBaseWeapon*> Weapons;//武器数组,数组元素为指针，添加两个数组Classes和Weapons是为了使子类可以通过转换父类再用指针存放到数组中，并且可以通过蓝图添加数组元素

	bool CanFire() const;
	bool CanEquip() const;

	void EquipWeapon(int32 WeaponIndex);

	int32 CurrentWeaponIndex = 0;//武器数组下标
private:

	UPROPERTY()
	UAnimMontage* CurrentReloadAnimMontage = nullptr;


	bool EquipAnimInProgress = false;//装备动画进行中
	bool ReloadAnimInProgress = false;//装弹动画进行中


	void SpawnWeapons();

	void AttachWeaponToSocket(ASTUBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName);//将武器附着在插槽上


	void PlayAnimMontage(UAnimMontage* Animation);

	void InitAnimations();

	void OnEquipFinished(USkeletalMeshComponent* MeshComponent);
	void OnReloadFinished(USkeletalMeshComponent* MeshComponent);


	bool CanReload() const;

	void OnClipEmpty(ASTUBaseWeapon* AmmoEmptyWeapon);
};
