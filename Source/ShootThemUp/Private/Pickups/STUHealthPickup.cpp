// Shoot Them Up , All Right Reserved


#include "Pickups/STUHealthPickup.h"
#include "Components/STUHealthComponent.h"
#include "STUUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthPickup, All, All);

bool ASTUHealthPickup::GivePickupTo(APawn* PlayerPawn)//通过此函数连接到健康组件，再在健康组件中实现治疗
{
	//UE_LOG(LogHealthPickup, Display, TEXT("HealthBox was taken"));

	const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(PlayerPawn);//获得健康组件

	if (!HealthComponent)return false;

	return HealthComponent->TryToAddHealth(HealthAmount);//调用健康组件中添加健康值的函数
}