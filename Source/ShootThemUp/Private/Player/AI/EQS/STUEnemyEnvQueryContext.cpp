// Shoot Them Up , All Right Reserved


#include "Player/AI/EQS/STUEnemyEnvQueryContext.h"
#include "EnvironmentQuery/EnvQueryTypes.h"//环境查询种类头文件
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"//环境查询项目种类头文件
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"//AI蓝图帮助库头文件，包含GetBlackboard函数

void USTUEnemyEnvQueryContext::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	const auto QueryOwner = Cast<AActor>(QueryInstance.Owner.Get());//Owner是UObject的弱指针，获取QueryInstance实例的拥有者并转换为AActor

	const auto Blackboard = UAIBlueprintHelperLibrary::GetBlackboard(QueryOwner);//用AI蓝图帮助库中获取黑板函数，参数为AActor
	if (!Blackboard)return;

	const auto ContextActor = Blackboard->GetValueAsObject(EnemyActorKeyName);//获取黑板中的对象变量(敌人），GetValueAsObject返回指向UObject的指针
	UEnvQueryItemType_Actor::SetContextHelper(ContextData, Cast<AActor>(ContextActor));//将黑板中敌人设置为语境数据中的Actor
}