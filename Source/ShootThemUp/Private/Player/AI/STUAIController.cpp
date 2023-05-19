// Shoot Them Up , All Right Reserved


#include "Player/AI/STUAIController.h"
#include "AI/STUAICharacter.h"
#include "Components/STUAIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/STUAIWeaponComponent.h"
#include "STUUtils.h"//获取组件模板函数的头文件
#include "Weapon/STUBaseWeapon.h"

DEFINE_LOG_CATEGORY_STATIC(LogAIController, All, All);

ASTUAIController::ASTUAIController()
{
	STUAIPerceptionComponent = CreateDefaultSubobject<USTUAIPerceptionComponent>("STUAIPerceptionComponent");
	SetPerceptionComponent(*STUAIPerceptionComponent);
	//AI感知组件需要使用设置感知组件函数，参数为引用（UAIPerceptionComponent& InPerceptionComponent）所以指针需要解引用
}

void ASTUAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	const auto STUCharacter = Cast<ASTUAICharacter>(InPawn);
	if (STUCharacter)
	{
		RunBehaviorTree(STUCharacter->BehaviorTreeAsset);//生成行为树
	}
}

void ASTUAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	const auto AimActor = GetFocusOnActor();//获取面向的敌人
	SetFocus(AimActor);//设置焦点，是角色面向对象，参数为AActor
	//AttackClosestEnemy(AimActor);
}

AActor* ASTUAIController::GetFocusOnActor() const
{
	if (!GetBlackboardComponent())return nullptr;//GetBlackboardComponent是AIControler中的函数，返回黑板组件
	return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(FocusOnKeyName));
}

void ASTUAIController::AttackClosestEnemy(AActor* ClosestEnemy)const
{
	if (!ClosestEnemy)return;
	const auto AI = Cast<AActor>(GetPawn());
	if (!AI)return;
	const auto Comp = STUUtils::GetSTUPlayerComponent<USTUAIWeaponComponent>(AI);//用自定模板函数获取武器组件
	//const auto Comp = Cast<USTUWeaponComponent>(AI->GetComponentByClass(USTUWeaponComponent::StaticClass()));//StaticClass()将类实例化

	if (!Comp)return;
	Comp->StartFire();
	UE_LOG(LogAIController,Display, TEXT("StartFire"));
}