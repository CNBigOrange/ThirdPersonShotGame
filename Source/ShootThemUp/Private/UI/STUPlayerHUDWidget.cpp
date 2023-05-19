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

bool USTUPlayerHUDWidget::GetCurrentWeaponUIData(FWeaponUIData& UIData) const//USTUPlayerHUDWidget::GetWeaponUIData包含了USTUWeaponComponent::GetWeaponUIData(FWeaponUIData& UIData)函数，包装函数
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
	//获取状态名字，如果等于观察者返回true
}


//将获取组件改为了模板函数
//USTUWeaponComponent* USTUPlayerHUDWidget::GetWeaponComponent() const
//{
//	const auto Player = GetOwningPlayerPawn();//获得拥有的玩家棋子指针
//	if (!Player)return nullptr;
//
//	const auto Component = Player->GetComponentByClass(USTUWeaponComponent::StaticClass());//按类查找组件，返回的是相应组件的指针
//	//按类查找类型要加StaticClass()
//	const auto WeaponComponent = Cast<USTUWeaponComponent>(Component);//
//	return WeaponComponent;
//}
//
//USTUHealthComponent* USTUPlayerHUDWidget::GetHealthComponent() const
//{
//	const auto Player = GetOwningPlayerPawn();//获得拥有的玩家棋子指针
//	if (!Player)return nullptr;
//
//	const auto Component = Player->GetComponentByClass(USTUHealthComponent::StaticClass());//按类查找组件，返回的是相应组件的指针
//	//按类查找类型要加StaticClass()
//	const auto HealthComponent = Cast<USTUHealthComponent>(Component);//
//	return HealthComponent;
//}
