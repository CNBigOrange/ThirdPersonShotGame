// Shoot Them Up , All Right Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUCoreTypes.h"
#include "STUBaseWeapon.generated.h"

//前置声明
class USTUWeaponComponent;
class USkeletalMeshComponent;//骨骼网格体
class UNiagaraSystem;
class UNiagaraComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUBaseWeapon : public AActor
{
	GENERATED_BODY()

public:

	ASTUBaseWeapon();

	virtual void StartFire();

	virtual void StopFire();

	FWeaponUIData GetUIData() const { return UIData; }//获得UIData的接口

	FAmmoData GetAmmoData() const { return CurrentAmmo; }//获得AmmoData接口

	FOnClipEmptySignature OnClipEmpty;//弹夹空的委托

	void ChangeClip();

	bool TryToAddAmmo(int32 ClipsAmount);

	bool IsNoClipsToReload() const;

	bool IsBulletsFull() const;

	bool IsAmmoEmpty() const;
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USkeletalMeshComponent* WeaponMesh;//骨骼网格体

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Socket")
	FName MuzzleSocketName = "MuzzleSocket";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category = "Weapon")
	float TraceMaxDistance = 100000.0f;//1虚幻单位等于1cm

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		FAmmoData DefaultAmmo //创建结构体
	{
		15, 
		10, 
		false
	};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
		FWeaponUIData UIData;//通过蓝图输入

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
		UNiagaraSystem* MuzzleFX;//生成枪口特效

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Health")
	//USTUHealthComponent* STUHealthComponent;

	virtual void BeginPlay() override;

	virtual void MakeShot();

	virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const;

	APlayerController* GetPlayerController() const;

	bool GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;

	FVector GetMuzzleWorldLocation() const;

	void MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd, FHitResult& CheckHitResult, const FVector& CheckTraceStart, const FVector& CheckTraceEnd);

	void DecreaseAmmo();

	
	bool IsAmmoFull() const;
	bool IsClipEmpty() const;

	void LogAmmo();

	UNiagaraComponent* SpawnMuzzleFX();//生成枪口特效函数

private:
	FAmmoData CurrentAmmo;
};
