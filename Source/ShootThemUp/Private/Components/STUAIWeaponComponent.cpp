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
	int32 NextIndex = (CurrentWeaponIndex + 1) % Weapons.Num();//��һ��������ŵ��ڵ�ǰ���+1
	while (NextIndex != CurrentWeaponIndex)//��ǰ��Ų�������һ�ѱ�ſ�ʼѭ��
	{
		if (!Weapons[NextIndex]->IsAmmoEmpty())break;//�����һ�������޵�ҩ��ֹѭ��
		NextIndex = (NextIndex + 1) % Weapons.Num();//�����һ�������е�ҩ�򽫱��+1
	}

	if (CurrentWeaponIndex != NextIndex)//�����һ�������뵱ǰ������һ��
	{
		CurrentWeaponIndex = NextIndex;//�򽫵�ǰ�����л�Ϊ��һ������
		EquipWeapon(CurrentWeaponIndex);//�����л���������
	}
}