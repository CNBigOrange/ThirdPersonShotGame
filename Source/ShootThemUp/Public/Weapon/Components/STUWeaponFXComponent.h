// Shoot Them Up , All Right Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUCoreTypes.h"
#include "STUWeaponFXComponent.generated.h"

class UNiagaraSystem;//向前声明粒子系统
class UPhysicalMaterial;//向前声明物理材质

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTTHEMUP_API USTUWeaponFXComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	USTUWeaponFXComponent();

	void PlayImpactFX(const FHitResult& Hit);//攻击粒子效果

protected:


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
		FImpactData DefaultImpactData;//创建粒子系统

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
		TMap<UPhysicalMaterial*, FImpactData> ImpactDataMap;
	//图结构，数据类型为键值对，用物理材质当做键（参数一），尼亚加拉系统当做值（参数二），命名为特效图
};
