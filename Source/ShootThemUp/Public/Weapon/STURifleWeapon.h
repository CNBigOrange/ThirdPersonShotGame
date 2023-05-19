// Shoot Them Up , All Right Reserved

#pragma once

#include "CoreMinimal.h"
#include "Weapon/STUBaseWeapon.h"
#include "STURifleWeapon.generated.h"

class USTUWeaponFXComponent;
class UNiagaraComponent;//尼亚加拉组件
class UNiagaraSystem;//尼亚加拉系统

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
		UNiagaraSystem* TraceFX;//弹道特效尼亚加拉系统

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category = "VFX")
		FString TraceTargetName = "TraceTarget";//弹道目标名字
private:

	FTimerHandle ShotTimerHandle;//创建计时器

	UPROPERTY()
		UNiagaraComponent* MuzzleFXComponent;

	void MakeDamage(const FHitResult& HitResult);
	void InitMuzzleFX();//初始化枪口特效
	void SetMuzzleFXVisibility(bool Visible);//设置枪口特效可见性
	void SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd);//生成弹道特效，参数为弹道起点和终点
};
