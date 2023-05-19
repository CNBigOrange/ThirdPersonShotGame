// Shoot Them Up , All Right Reserved


#include "Components/STUCharacterMovementComponent.h"
#include "Player/STUBaseCharacter.h"

float USTUCharacterMovementComponent::GetMaxSpeed() const//最大速度函数定义
{
	const float MaxSpeed = Super::GetMaxSpeed();//创建一个变量给他赋予父类UCharacterMovementComponent变量的值
	const ASTUBaseCharacter* Player = Cast<ASTUBaseCharacter>(GetPawnOwner());//需要包含Player/STUBaseCharacter.h
	return Player && Player->IsRunning()?MaxSpeed*RunModifier:MaxSpeed;//&&运算符如果第一个参数为假则不会运算第二个参数直接输出false
	//Player是判断是否为空指针，MaxSeep*RunModifier是奔跑速度
	//三目运算符配上return减少函数体量
}