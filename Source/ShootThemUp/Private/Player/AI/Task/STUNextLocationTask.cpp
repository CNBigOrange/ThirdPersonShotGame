// Shoot Them Up , All Right Reserved


#include "Player/AI/Task/STUNextLocationTask.h"
#include "BehaviorTree/BlackboardComponent.h"//黑板组件头文件
#include "AIController.h"//AI控制器头文件
#include "NavigationSystem.h"//导航系统头文件

USTUNextLocationTask::USTUNextLocationTask() 
{
	NodeName = "Next Location";//行为树中节点名字
}

EBTNodeResult::Type USTUNextLocationTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) 
//执行任务函数,EBTNodeResult::Type为枚举类，包含：Succeeded,Failed,Aborted,InProgress
{
	const auto AIController = OwnerComp.GetAIOwner();//UBehaviorTreeComponent组件中的获取组件AI控制器函数
	const auto Blackboard = OwnerComp.GetBlackboardComponent();//获取组件黑板函数
	if (!AIController || !Blackboard)return EBTNodeResult::Failed;//节点执行结果失败

	const auto Pawn = AIController->GetPawn();//获取控制者的棋子
	if (!Pawn)return EBTNodeResult::Failed;

	const auto NavSys = UNavigationSystemV1::GetCurrent(Pawn);//获取当前棋子导航系统的指针
	if (!NavSys)return EBTNodeResult::Failed;

	FNavLocation NavLocation;//描述导航体中位置信息的结构体
	auto Location = Pawn->GetActorLocation();
	if (!SelfCenter)//是否以自己为中心，当为否时，则以感知到的对象为中心
	{
		auto CenterActor = Cast<AActor>(Blackboard->GetValueAsObject(CenterActorKey.SelectedKeyName));//获取黑板秘钥对象，感知对象，赋值给中心对象
		if (!CenterActor)return EBTNodeResult::Failed;
		Location = CenterActor->GetActorLocation();
	}

	const auto IsFound = NavSys->GetRandomReachablePointInRadius(Location, Radius, NavLocation);
	//获取导航系统中的随机点，参数一：棋子位置，参数二：半径，参数三：导航体中位置信息
	if (!IsFound) return EBTNodeResult::Failed;

	Blackboard->SetValueAsVector(AimLocationKey.SelectedKeyName, NavLocation.Location);
	//黑板中的设置向量值函数，参数一：黑板秘钥名称，参数二：导航位置
	return EBTNodeResult::Succeeded;
}

