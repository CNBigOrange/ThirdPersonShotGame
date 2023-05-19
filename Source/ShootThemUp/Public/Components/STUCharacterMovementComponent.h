// Shoot Them Up , All Right Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "STUCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API USTUCharacterMovementComponent : public UCharacterMovementComponent//角色运动组件的派生类
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement",meta = (ClampMin = "1.5",ClampMax = "10.0"))//meta表示变化，ClampMin和ClampMax是变化区间范围
		float RunModifier = 2.0f;

	virtual float GetMaxSpeed() const override;//重写最大速度虚函数


};
