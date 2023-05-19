// Shoot Them Up , All Right Reserved


#include "Components/STUAIWeaponComponent.h"
#include "Weapon/STUBaseWeapon.h"

DEFINE_LOG_CATEGORY_STATIC(LogAIWeaponComponent, All, All);

void USTUAIWeaponComponent::StartFire()
{
	if (!CanFire())return;

	if (CurrentWeapon->IsAmmoEmpty())
	{
		NextWeapon();
	}
	else
	{
		//if(!CurrentWeapon)return;
		CurrentWeapon->StartFire();
		//UE_LOG(LogAIWeaponComponent, Display, TEXT("Start Fire"));
	}
}

void USTUAIWeaponComponent::NextWeapon()
{
	if (!CanEquip())return;
	int32 NextIndex = (CurrentWeaponIndex + 1) % Weapons.Num();//下一把武器编号等于当前编号+1
	while (NextIndex != CurrentWeaponIndex)//当前编号不等于下一把编号开始循环
	{
		if (!Weapons[NextIndex]->IsAmmoEmpty())break;//如果下一把武器无弹药终止循环
		NextIndex = (NextIndex + 1) % Weapons.Num();//如果下一把武器有弹药则将编号+1
	}

	if (CurrentWeaponIndex != NextIndex)//如果下一把武器与当前武器不一样
	{
		CurrentWeaponIndex = NextIndex;//则将当前武器切换为下一把武器
		EquipWeapon(CurrentWeaponIndex);//调用切换武器函数
	}
}