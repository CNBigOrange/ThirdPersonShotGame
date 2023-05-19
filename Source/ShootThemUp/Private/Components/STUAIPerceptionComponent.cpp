// Shoot Them Up , All Right Reserved

#include "Components/STUAIPerceptionComponent.h"
#include "AIController.h"//AI������ͷ�ļ�
#include "STUUtils.h"//��ȡ���ģ�庯����ͷ�ļ�
#include "Components/STUHealthComponent.h"//�������
#include "Perception/AISense_Sight.h"//AI�Ӿ���֪ͷ�ļ�

AActor* USTUAIPerceptionComponent::GetClosestEnemy() const//��ȡ��֪��Χ���������
{
	TArray<AActor*> PercieveActors;//��֪���Ľ�ɫ����
	GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PercieveActors);
	//��ȡ��ǰ��֪���Ľ�ɫ���ɸ���������������һ����֪��ʽ������������ɫ����
	if (PercieveActors.Num() == 0)return nullptr;

	const auto Controller = Cast<AAIController>(GetOwner());
	if (!Controller)return nullptr;

	const auto Pawn = Controller->GetPawn();
	if (!Pawn)return nullptr;

	float BestDistance = MAX_FLT;//FLT_MAX��FLT_MIN����󸡵�������С������
	AActor* BestPawn = nullptr;
	for (const auto PercieveActor : PercieveActors)//��������
	{
		const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(PercieveActor);//��ȡ�������
		if (HealthComponent && !HealthComponent->IsDead())//TODO:check if enemies or not
		{
			const auto CurrentDistance = (PercieveActor->GetActorLocation() - Pawn->GetActorLocation()).Size();//һ��λ����������һ��λ�����������䳤��
			if (CurrentDistance < BestDistance)
			{
				BestDistance = CurrentDistance;
				BestPawn = PercieveActor;
			}
		}
	}
	return BestPawn;
}

