// Shoot Them Up , All Right Reserved


#include "Pickups/STUHealthPickup.h"
#include "Components/STUHealthComponent.h"
#include "STUUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthPickup, All, All);

bool ASTUHealthPickup::GivePickupTo(APawn* PlayerPawn)//ͨ���˺������ӵ�������������ڽ��������ʵ������
{
	//UE_LOG(LogHealthPickup, Display, TEXT("HealthBox was taken"));

	const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(PlayerPawn);//��ý������

	if (!HealthComponent)return false;

	return HealthComponent->TryToAddHealth(HealthAmount);//���ý����������ӽ���ֵ�ĺ���
}