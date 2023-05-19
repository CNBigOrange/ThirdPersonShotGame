// Shoot Them Up , All Right Reserved

#pragma once

#include "CoreMinimal.h"
#include "Weapon/STUBaseWeapon.h"
#include "STURifleWeapon.generated.h"

class USTUWeaponFXComponent;
class UNiagaraComponent;//���Ǽ������
class UNiagaraSystem;//���Ǽ���ϵͳ

UCLASS()
class SHOOTTHEMUP_API ASTURifleWeapon : public ASTUBaseWeapon
{
	GENERATED_BODY()
	
public:
	ASTURifleWeapon();

	virtual void StartFire() override;
	virtual void StopFire() override;

protected:
	virtual void BeginPlay() override;
	
	virtual void MakeShot() override;

	virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const override;

	UPROPERTY(VisibleAnywhere, Category = "VFX")
	USTUWeaponFXComponent* WeaponFXComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		float TimeBetweenShots = 0.1f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		float BulletSpread = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		float DamageAmount = 10.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category = "VFX")
		UNiagaraSystem* TraceFX;//������Ч���Ǽ���ϵͳ

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category = "VFX")
		FString TraceTargetName = "TraceTarget";//����Ŀ������
private:

	FTimerHandle ShotTimerHandle;//������ʱ��

	UPROPERTY()
		UNiagaraComponent* MuzzleFXComponent;

	void MakeDamage(const FHitResult& HitResult);
	void InitMuzzleFX();//��ʼ��ǹ����Ч
	void SetMuzzleFXVisibility(bool Visible);//����ǹ����Ч�ɼ���
	void SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd);//���ɵ�����Ч������Ϊ���������յ�
};
