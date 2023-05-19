// Shoot Them Up , All Right Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUCoreTypes.h"
#include "STUBaseWeapon.generated.h"

//ǰ������
class USTUWeaponComponent;
class USkeletalMeshComponent;//����������
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

	FWeaponUIData GetUIData() const { return UIData; }//���UIData�Ľӿ�

	FAmmoData GetAmmoData() const { return CurrentAmmo; }//���AmmoData�ӿ�

	FOnClipEmptySignature OnClipEmpty;//���пյ�ί��

	void ChangeClip();

	bool TryToAddAmmo(int32 ClipsAmount);

	bool IsNoClipsToReload() const;

	bool IsBulletsFull() const;

	bool IsAmmoEmpty() const;
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USkeletalMeshComponent* WeaponMesh;//����������

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Socket")
	FName MuzzleSocketName = "MuzzleSocket";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category = "Weapon")
	float TraceMaxDistance = 100000.0f;//1��õ�λ����1cm

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		FAmmoData DefaultAmmo //�����ṹ��
	{
		15, 
		10, 
		false
	};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
		FWeaponUIData UIData;//ͨ����ͼ����

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
		UNiagaraSystem* MuzzleFX;//����ǹ����Ч

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

	UNiagaraComponent* SpawnMuzzleFX();//����ǹ����Ч����

private:
	FAmmoData CurrentAmmo;
};
