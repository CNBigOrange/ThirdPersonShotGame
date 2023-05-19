// Shoot Them Up , All Right Reserved


#include "Components/STUCharacterMovementComponent.h"
#include "Player/STUBaseCharacter.h"

float USTUCharacterMovementComponent::GetMaxSpeed() const//����ٶȺ�������
{
	const float MaxSpeed = Super::GetMaxSpeed();//����һ�������������踸��UCharacterMovementComponent������ֵ
	const ASTUBaseCharacter* Player = Cast<ASTUBaseCharacter>(GetPawnOwner());//��Ҫ����Player/STUBaseCharacter.h
	return Player && Player->IsRunning()?MaxSpeed*RunModifier:MaxSpeed;//&&����������һ������Ϊ���򲻻�����ڶ�������ֱ�����false
	//Player���ж��Ƿ�Ϊ��ָ�룬MaxSeep*RunModifier�Ǳ����ٶ�
	//��Ŀ���������return���ٺ�������
}