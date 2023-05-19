// Shoot Them Up , All Right Reserved


#include "Player/AI/EQS/STUEnemyEnvQueryContext.h"
#include "EnvironmentQuery/EnvQueryTypes.h"//������ѯ����ͷ�ļ�
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"//������ѯ��Ŀ����ͷ�ļ�
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"//AI��ͼ������ͷ�ļ�������GetBlackboard����

void USTUEnemyEnvQueryContext::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	const auto QueryOwner = Cast<AActor>(QueryInstance.Owner.Get());//Owner��UObject����ָ�룬��ȡQueryInstanceʵ����ӵ���߲�ת��ΪAActor

	const auto Blackboard = UAIBlueprintHelperLibrary::GetBlackboard(QueryOwner);//��AI��ͼ�������л�ȡ�ڰ庯��������ΪAActor
	if (!Blackboard)return;

	const auto ContextActor = Blackboard->GetValueAsObject(EnemyActorKeyName);//��ȡ�ڰ��еĶ������(���ˣ���GetValueAsObject����ָ��UObject��ָ��
	UEnvQueryItemType_Actor::SetContextHelper(ContextData, Cast<AActor>(ContextActor));//���ڰ��е�������Ϊ�ﾳ�����е�Actor
}