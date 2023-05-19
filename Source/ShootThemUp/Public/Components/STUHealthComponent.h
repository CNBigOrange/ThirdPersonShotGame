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

	float GetHealth() const { return Health;}//���const��������Ϊ���溯������ı��κ�����

	UFUNCTION(BlueprintCallable)
	bool IsDead()const { return FMath::IsNearlyZero(Health); }
	//��ʹ��const����ʹ��const���ӳ���Ľ�׳��

	UFUNCTION(BlueprintCallable,Category = "Health")
		float GetHealthPercent() const { return Health / MaxHealth; }

	FOnDeath OnDeath;//����״̬ί��
	FOnHealthChanged OnHealthChanged;//�����ı�ί��

	bool TryToAddHealth(float HealthAmount);
	bool IsHealthFull() const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", meta = (ClampMin = "0.0", ClampMax = "1000.0"))
	float Health = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category = "Health",meta = (ClampMin = "0.0", ClampMax = "1000.0"))//MSB3037�����Ǻ��������������ͷ�ļ��������
	float MaxHealth = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal")//EditDefaultsOnlyֻ����ͼ�б༭��
	bool AutoHeal = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal")
	float HealUpdateTime = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal")
	float HealDelay = 3.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal")
	float HealModifier = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
		TSubclassOf<UCameraShakeBase> CameraShake;//��TSubclassOfģ��ȡUCameraShakeBase������
//public:
	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:


	FTimerHandle HealTimerHandle;//������ʱ��

	UFUNCTION()
	void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	//����һ�����ƻ���Actor�����������ƻ�ֵ�����������ƻ����ͣ������ģ������ƻ��Ŀ������������壺����ƻ���Actor

	void HealUpdate();
	void SetHealth(float NewHealth);

	void PlayCameraShake();
};