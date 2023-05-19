// Shoot Them Up , All Right Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUCoreTypes.h"
#include "STUHealthComponent.generated.h"

class UCameraShakeBase;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTTHEMUP_API USTUHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USTUHealthComponent();

	float GetHealth() const { return Health;}//添加const函数，因为里面函数不会改变任何数据

	UFUNCTION(BlueprintCallable)
	bool IsDead()const { return FMath::IsNearlyZero(Health); }
	//能使用const尽量使用const增加程序的健壮性

	UFUNCTION(BlueprintCallable,Category = "Health")
		float GetHealthPercent() const { return Health / MaxHealth; }

	FOnDeath OnDeath;//死亡状态委托
	FOnHealthChanged OnHealthChanged;//健康改变委托

	bool TryToAddHealth(float HealthAmount);
	bool IsHealthFull() const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", meta = (ClampMin = "0.0", ClampMax = "1000.0"))
	float Health = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category = "Health",meta = (ClampMin = "0.0", ClampMax = "1000.0"))//MSB3037错误是宏参数输入错误或者头文件输入错误
	float MaxHealth = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal")//EditDefaultsOnly只在蓝图中编辑，
	bool AutoHeal = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal")
	float HealUpdateTime = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal")
	float HealDelay = 3.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal")
	float HealModifier = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
		TSubclassOf<UCameraShakeBase> CameraShake;//用TSubclassOf模板取UCameraShakeBase的子类
//public:
	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:


	FTimerHandle HealTimerHandle;//创建计时器

	UFUNCTION()
	void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	//参数一：被破坏的Actor，参数二：破坏值，参数三：破坏类型，参数四：负责破坏的控制器，参数五：造成破坏的Actor

	void HealUpdate();
	void SetHealth(float NewHealth);

	void PlayCameraShake();
};