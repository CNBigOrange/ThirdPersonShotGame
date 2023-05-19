// Shoot Them Up , All Right Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "STUCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API USTUCharacterMovementComponent : public UCharacterMovementComponent//��ɫ�˶������������
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement",meta = (ClampMin = "1.5",ClampMax = "10.0"))//meta��ʾ�仯��ClampMin��ClampMax�Ǳ仯���䷶Χ
		float RunModifier = 2.0f;

	virtual float GetMaxSpeed() const override;//��д����ٶ��麯��


};
