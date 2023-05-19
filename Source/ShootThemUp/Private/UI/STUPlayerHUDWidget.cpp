// Shoot Them Up , All Right Reserved


#include "UI/STUPlayerHUDWidget.h"
#include "Components/STUHealthComponent.h"
#include "Components/STUWeaponComponent.h"
#include "STUUtils.h"

float USTUPlayerHUDWidget::GetHealthPercent() const
{
	const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(GetOwningPlayerPawn());
	if (!HealthComponent)return 0.0f;

	return HealthComponent->GetHealthPercent();
}

bool USTUPlayerHUDWidget::GetCurrentWeaponUIData(FWeaponUIData& UIData) const//USTUPlayerHUDWidget::GetWeaponUIData������USTUWeaponComponent::GetWeaponUIData(FWeaponUIData& UIData)��������װ����
{
	const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(GetOwningPlayerPawn());
	if (!WeaponComponent)return false;

	return WeaponComponent->GetCurrentWeaponUIData(UIData);
}

bool USTUPlayerHUDWidget::GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const//
{
	const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(GetOwningPlayerPawn());
	if (!WeaponComponent)return false;

	return WeaponComponent->GetCurrentWeaponAmmoData(AmmoData);
}

bool USTUPlayerHUDWidget::IsPlayerAlive() const
{
	const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(GetOwningPlayerPawn());
	return HealthComponent && !HealthComponent->IsDead();
}


bool USTUPlayerHUDWidget::IsPlayerSpectating() const
{
	const auto Controller = GetOwningPlayer();
	return Controller && Controller->GetStateName() == NAME_Spectating;
	//��ȡ״̬���֣�������ڹ۲��߷���true
}


//����ȡ�����Ϊ��ģ�庯��
//USTUWeaponComponent* USTUPlayerHUDWidget::GetWeaponComponent() const
//{
//	const auto Player = GetOwningPlayerPawn();//���ӵ�е��������ָ��
//	if (!Player)return nullptr;
//
//	const auto Component = Player->GetComponentByClass(USTUWeaponComponent::StaticClass());//���������������ص�����Ӧ�����ָ��
//	//�����������Ҫ��StaticClass()
//	const auto WeaponComponent = Cast<USTUWeaponComponent>(Component);//
//	return WeaponComponent;
//}
//
//USTUHealthComponent* USTUPlayerHUDWidget::GetHealthComponent() const
//{
//	const auto Player = GetOwningPlayerPawn();//���ӵ�е��������ָ��
//	if (!Player)return nullptr;
//
//	const auto Component = Player->GetComponentByClass(USTUHealthComponent::StaticClass());//���������������ص�����Ӧ�����ָ��
//	//�����������Ҫ��StaticClass()
//	const auto HealthComponent = Cast<USTUHealthComponent>(Component);//
//	return HealthComponent;
//}
