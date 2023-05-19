// Shoot Them Up , All Right Reserved


#include "Components/STUHealthComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"
#include "Engine/World.h"//世界头文件
#include "TimerManager.h"//计时器头文件

DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent, All, All);

// Sets default values for this component's properties
USTUHealthComponent::USTUHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;//不需要每帧调用

}


// Called when the game starts
void USTUHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	check(MaxHealth > 0)

	SetHealth(MaxHealth);//设置开始游戏时生命值

	AActor* ComponentOwner = GetOwner();//获得当前组件的拥有者AActor*
	if (ComponentOwner)
	{
		//OnTakeAnyDamage是在Actor头文件中声明的动态多播委托，并且拥有五个参数
		ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &USTUHealthComponent::OnTakeAnyDamage);//用于动态组播委托上调用AddDynamic函数，第一个参数： UserClass* InUserObject用户类，第二个参数： typename FDelegate::template TMethodPtrResolver< UserClass >::FMethodPtr InMethodPtr,伤害方法函数
		//AddDynamic( UserObject, FuncName )用于在动态组播委托上调用AddDynamic()的辅助宏。自动生成函数命名字符串。
		//BindDynamic是绑定动态委托的宏
	}
}


void USTUHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)//动态多播委托在Actor中已声明
{
	if (Damage <= 0.0f || IsDead())return;//优化代码
	
	SetHealth(Health - Damage);//Clamp钳函数，参数一：变量的值；参数二：变量赋值的最小值；参数三：变量赋值的最大值
	OnHealthChanged.Broadcast(Health);//健康改变时调用一次健康改变委托


	if (IsDead())
	{
		OnDeath.Broadcast();//多播委托允许您附加多个函数委托，然后通过调用多播委托的"Broadcast()"函数一次性同时执行它们。多播委托签名不得使用返回值。
        //在多播委托上调用"Broadcast()"总是安全的，即使是在没有任何绑定时也是如此。

		//OnDestroyWeapon.Broadcast();?
	}
	else if (AutoHeal&&GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(HealTimerHandle, this, &USTUHealthComponent::HealUpdate, HealUpdateTime, true, HealDelay);
		//设置计时器，参数一：计时器引用，FTimerHandle&；参数二：计时器用户对象；参数三：计时器触发调用的方法；参数四：调用速率；参数五：是否循环；参数六：开始前延迟
	}
	//UE_LOG(LogHealthComponent, Display, TEXT("Damage:%f"), Damage);
	//FString HealthStr = FString::SanitizeFloat(Health);//float转换为FString
	//if (GEngine)
	//{
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, HealthStr, false, FVector2D(3.f, 3.f));
	//}
	// 用于测试
	//if (DamageType)
	//{
	//	if (DamageType->IsA<USTUFireDamageType>())//IsA是模板函数，它将对象类型作为模板参数，如果该对象属于此类型则返回true
	//	{
	//		UE_LOG(LogHealthComponent, Display, TEXT("So hooooooooot!"));
	//	}
	//	else if (DamageType->IsA<USTUIceDamageType>())
	//	{
	//		UE_LOG(LogHealthComponent, Display, TEXT("So coooooooooold!"));
	//	}
	//}

	PlayCameraShake();//受伤时抖动相机
}

void USTUHealthComponent::HealUpdate()
{
	if (IsDead())return;//解决死亡后仍然触发加血的bug
	SetHealth(Health + HealModifier);

	if (IsHealthFull() && GetWorld())//IsNearlyEqual浮点型数据大小对比使用IsNearlyEqual（约等）
	{
		GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);//获取世界->获取时间管理器.调用清除计时器函数
	}
}

void USTUHealthComponent::SetHealth(float NewHealth)//封装健康设置函数以便复用更方便
{
	Health = FMath::Clamp(NewHealth, 0.0f, MaxHealth);//钳形函数最小为0，最大为MaxHealth
	OnHealthChanged.Broadcast(Health);//调用健康改变的委托
}


bool USTUHealthComponent::TryToAddHealth(float HealthAmount)
{
	if (IsDead() || IsHealthFull())return false;//生命值满或死亡返回false，将不会执行PickupWasTaken()
	SetHealth(Health + HealthAmount);
	return true;
}

bool USTUHealthComponent::IsHealthFull() const//函数体加const需要是成员函数，因为非成员函数在运行完毕后会释放
{
	return FMath::IsNearlyEqual(Health, MaxHealth);
}

void USTUHealthComponent::PlayCameraShake()
{
	if (IsDead())return;//如果死亡退出

	const auto Player = Cast<APawn>(GetOwner());//获取玩家转换为棋子类
	if (!Player)return;

	const auto Controller = Player->GetController<APlayerController>();//通过APawn类的GetController<>()重载函数，获取玩家控制器
	if (!Controller || !Controller->PlayerCameraManager)return;//判断控制器中是否存在玩家摄像器管理器

	Controller->PlayerCameraManager->StartCameraShake(CameraShake);//调用玩家摄像机控制管理器中的开始摄像机抖动，并输入抖动对象参数TSubclassOf<UCameraShakeBase>
}