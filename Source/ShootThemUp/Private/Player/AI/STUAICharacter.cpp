// Shoot Them Up , All Right Reserved


#include "Player/AI/STUAICharacter.h"
#include "AI/STUAIController.h"
#include "Components/STUCharacterMovementComponent.h"
#include "Components/STUAIWeaponComponent.h"
#include "Components/CapsuleComponent.h"//������ͷ�ļ�
#include "BrainComponent.h"//���������AIController������

ASTUAICharacter::ASTUAICharacter(const FObjectInitializer& ObjInit)  //Super(ObjInit)//����Ĺ��캯��,�̳и���ĳ�ʼ����
	:Super(ObjInit.SetDefaultSubobjectClass<USTUAIWeaponComponent>("WeaponComponent"))//���¶��常������ĺ���
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;//�����Զ�����AI
	AIControllerClass = ASTUAIController::StaticClass();//AI������������

	bUseControllerRotationYaw = false;
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->bUseControllerDesiredRotation = true;//ʹ�ÿ����������������ת
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 360.0f, 0.0f);
	}


}

void ASTUAICharacter::OnDeath()
{
	Super::OnDeath();//���ø����麯����ʹ�����麯���߼�����ִ��

	const auto STUController = Cast<AAIController>(Controller);//Controller��Pawn.h�е�ʵ��
	if (STUController && STUController->BrainComponent)
	{
		STUController->BrainComponent->Cleanup();//BrainComponent�е������Ϊ������
	}
}