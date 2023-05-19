// Shoot Them Up , All Right Reserved


#include "Player/AI/STUAIController.h"
#include "AI/STUAICharacter.h"
#include "Components/STUAIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/STUAIWeaponComponent.h"
#include "STUUtils.h"//��ȡ���ģ�庯����ͷ�ļ�
#include "Weapon/STUBaseWeapon.h"

DEFINE_LOG_CATEGORY_STATIC(LogAIController, All, All);

ASTUAIController::ASTUAIController()
{
	STUAIPerceptionComponent = CreateDefaultSubobject<USTUAIPerceptionComponent>("STUAIPerceptionComponent");
	SetPerceptionComponent(*STUAIPerceptionComponent);
	//AI��֪�����Ҫʹ�����ø�֪�������������Ϊ���ã�UAIPerceptionComponent& InPerceptionComponent������ָ����Ҫ������
}

void ASTUAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	const auto STUCharacter = Cast<ASTUAICharacter>(InPawn);
	if (STUCharacter)
	{
		RunBehaviorTree(STUCharacter->BehaviorTreeAsset);//������Ϊ��
	}
}

void ASTUAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	const auto AimActor = GetFocusOnActor();//��ȡ����ĵ���
	SetFocus(AimActor);//���ý��㣬�ǽ�ɫ������󣬲���ΪAActor
	//AttackClosestEnemy(AimActor);
}

AActor* ASTUAIController::GetFocusOnActor() const
{
	if (!GetBlackboardComponent())return nullptr;//GetBlackboardComponent��AIControler�еĺ��������غڰ����
	return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(FocusOnKeyName));
}

void ASTUAIController::AttackClosestEnemy(AActor* ClosestEnemy)const
{
	if (!ClosestEnemy)return;
	const auto AI = Cast<AActor>(GetPawn());
	if (!AI)return;
	const auto Comp = STUUtils::GetSTUPlayerComponent<USTUAIWeaponComponent>(AI);//���Զ�ģ�庯����ȡ�������
	//const auto Comp = Cast<USTUWeaponComponent>(AI->GetComponentByClass(USTUWeaponComponent::StaticClass()));//StaticClass()����ʵ����

	if (!Comp)return;
	Comp->StartFire();
	UE_LOG(LogAIController,Display, TEXT("StartFire"));
}