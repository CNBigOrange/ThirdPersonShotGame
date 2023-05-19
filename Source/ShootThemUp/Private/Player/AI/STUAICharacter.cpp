// Shoot Them Up , All Right Reserved


#include "Player/AI/STUAICharacter.h"
#include "AI/STUAIController.h"
#include "Components/STUCharacterMovementComponent.h"
#include "Components/STUAIWeaponComponent.h"
#include "Components/CapsuleComponent.h"//胶囊体头文件
#include "BrainComponent.h"//大脑组件是AIController的子类

ASTUAICharacter::ASTUAICharacter(const FObjectInitializer& ObjInit)  //Super(ObjInit)//特殊的构造函数,继承父类的初始化器
	:Super(ObjInit.SetDefaultSubobjectClass<USTUAIWeaponComponent>("WeaponComponent"))//重新定义父类组件的函数
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;//设置自动控制AI
	AIControllerClass = ASTUAIController::StaticClass();//AI控制器的种类

	bUseControllerRotationYaw = false;
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->bUseControllerDesiredRotation = true;//使用控制器控制所需的旋转
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 360.0f, 0.0f);
	}


}

void ASTUAICharacter::OnDeath()
{
	Super::OnDeath();//调用父类虚函数，使父类虚函数逻辑正常执行

	const auto STUController = Cast<AAIController>(Controller);//Controller是Pawn.h中的实例
	if (STUController && STUController->BrainComponent)
	{
		STUController->BrainComponent->Cleanup();//BrainComponent中的清除行为树函数
	}
}