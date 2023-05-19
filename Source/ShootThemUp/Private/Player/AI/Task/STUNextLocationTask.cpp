// Shoot Them Up , All Right Reserved


#include "Player/AI/Task/STUNextLocationTask.h"
#include "BehaviorTree/BlackboardComponent.h"//�ڰ����ͷ�ļ�
#include "AIController.h"//AI������ͷ�ļ�
#include "NavigationSystem.h"//����ϵͳͷ�ļ�

USTUNextLocationTask::USTUNextLocationTask() 
{
	NodeName = "Next Location";//��Ϊ���нڵ�����
}

EBTNodeResult::Type USTUNextLocationTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) 
//ִ��������,EBTNodeResult::TypeΪö���࣬������Succeeded,Failed,Aborted,InProgress
{
	const auto AIController = OwnerComp.GetAIOwner();//UBehaviorTreeComponent����еĻ�ȡ���AI����������
	const auto Blackboard = OwnerComp.GetBlackboardComponent();//��ȡ����ڰ庯��
	if (!AIController || !Blackboard)return EBTNodeResult::Failed;//�ڵ�ִ�н��ʧ��

	const auto Pawn = AIController->GetPawn();//��ȡ�����ߵ�����
	if (!Pawn)return EBTNodeResult::Failed;

	const auto NavSys = UNavigationSystemV1::GetCurrent(Pawn);//��ȡ��ǰ���ӵ���ϵͳ��ָ��
	if (!NavSys)return EBTNodeResult::Failed;

	FNavLocation NavLocation;//������������λ����Ϣ�Ľṹ��
	auto Location = Pawn->GetActorLocation();
	if (!SelfCenter)//�Ƿ����Լ�Ϊ���ģ���Ϊ��ʱ�����Ը�֪���Ķ���Ϊ����
	{
		auto CenterActor = Cast<AActor>(Blackboard->GetValueAsObject(CenterActorKey.SelectedKeyName));//��ȡ�ڰ���Կ���󣬸�֪���󣬸�ֵ�����Ķ���
		if (!CenterActor)return EBTNodeResult::Failed;
		Location = CenterActor->GetActorLocation();
	}

	const auto IsFound = NavSys->GetRandomReachablePointInRadius(Location, Radius, NavLocation);
	//��ȡ����ϵͳ�е�����㣬����һ������λ�ã����������뾶������������������λ����Ϣ
	if (!IsFound) return EBTNodeResult::Failed;

	Blackboard->SetValueAsVector(AimLocationKey.SelectedKeyName, NavLocation.Location);
	//�ڰ��е���������ֵ����������һ���ڰ���Կ���ƣ�������������λ��
	return EBTNodeResult::Succeeded;
}

