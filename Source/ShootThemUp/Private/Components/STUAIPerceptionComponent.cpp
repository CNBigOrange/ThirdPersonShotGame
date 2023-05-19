// Shoot Them Up , All Right Reserved

#include "Components/STUAIPerceptionComponent.h"
#include "AIController.h"//AI控制器头文件
#include "STUUtils.h"//获取组件模板函数的头文件
#include "Components/STUHealthComponent.h"//健康组件
#include "Perception/AISense_Sight.h"//AI视觉感知头文件

AActor* USTUAIPerceptionComponent::GetClosestEnemy() const//获取感知范围内最近敌人
{
	TArray<AActor*> PercieveActors;//感知到的角色数组
	GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PercieveActors);
	//获取当前感知到的角色（可复数）函数，参数一：感知方式，参数二：角色数组
	if (PercieveActors.Num() == 0)return nullptr;

	const auto Controller = Cast<AAIController>(GetOwner());
	if (!Controller)return nullptr;

	const auto Pawn = Controller->GetPawn();
	if (!Pawn)return nullptr;

	float BestDistance = MAX_FLT;//FLT_MAX和FLT_MIN是最大浮点数和最小浮点数
	AActor* BestPawn = nullptr;
	for (const auto PercieveActor : PercieveActors)//遍历数组
	{
		const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(PercieveActor);//获取健康组件
		if (HealthComponent && !HealthComponent->IsDead())//TODO:check if enemies or not
		{
			const auto CurrentDistance = (PercieveActor->GetActorLocation() - Pawn->GetActorLocation()).Size();//一个位置向量减另一个位置向量再求其长度
			if (CurrentDistance < BestDistance)
			{
				BestDistance = CurrentDistance;
				BestPawn = PercieveActor;
			}
		}
	}
	return BestPawn;
}

