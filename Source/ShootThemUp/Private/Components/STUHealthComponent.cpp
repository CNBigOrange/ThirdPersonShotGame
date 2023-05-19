// Shoot Them Up , All Right Reserved


#include "Components/STUHealthComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"
#include "Engine/World.h"//����ͷ�ļ�
#include "TimerManager.h"//��ʱ��ͷ�ļ�

DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent, All, All);

// Sets default values for this component's properties
USTUHealthComponent::USTUHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;//����Ҫÿ֡����

}


// Called when the game starts
void USTUHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	check(MaxHealth > 0)

	SetHealth(MaxHealth);//���ÿ�ʼ��Ϸʱ����ֵ

	AActor* ComponentOwner = GetOwner();//��õ�ǰ�����ӵ����AActor*
	if (ComponentOwner)
	{
		//OnTakeAnyDamage����Actorͷ�ļ��������Ķ�̬�ಥί�У�����ӵ���������
		ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &USTUHealthComponent::OnTakeAnyDamage);//���ڶ�̬�鲥ί���ϵ���AddDynamic��������һ�������� UserClass* InUserObject�û��࣬�ڶ��������� typename FDelegate::template TMethodPtrResolver< UserClass >::FMethodPtr InMethodPtr,�˺���������
		//AddDynamic( UserObject, FuncName )�����ڶ�̬�鲥ί���ϵ���AddDynamic()�ĸ����ꡣ�Զ����ɺ��������ַ�����
		//BindDynamic�ǰ󶨶�̬ί�еĺ�
	}
}


void USTUHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)//��̬�ಥί����Actor��������
{
	if (Damage <= 0.0f || IsDead())return;//�Ż�����
	
	SetHealth(Health - Damage);//Clampǯ����������һ��������ֵ����������������ֵ����Сֵ����������������ֵ�����ֵ
	OnHealthChanged.Broadcast(Health);//�����ı�ʱ����һ�ν����ı�ί��


	if (IsDead())
	{
		OnDeath.Broadcast();//�ಥί�����������Ӷ������ί�У�Ȼ��ͨ�����öಥί�е�"Broadcast()"����һ����ͬʱִ�����ǡ��ಥί��ǩ������ʹ�÷���ֵ��
        //�ڶಥί���ϵ���"Broadcast()"���ǰ�ȫ�ģ���ʹ����û���κΰ�ʱҲ����ˡ�

		//OnDestroyWeapon.Broadcast();?
	}
	else if (AutoHeal&&GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(HealTimerHandle, this, &USTUHealthComponent::HealUpdate, HealUpdateTime, true, HealDelay);
		//���ü�ʱ��������һ����ʱ�����ã�FTimerHandle&������������ʱ���û����󣻲���������ʱ���������õķ����������ģ��������ʣ������壺�Ƿ�ѭ��������������ʼǰ�ӳ�
	}
	//UE_LOG(LogHealthComponent, Display, TEXT("Damage:%f"), Damage);
	//FString HealthStr = FString::SanitizeFloat(Health);//floatת��ΪFString
	//if (GEngine)
	//{
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, HealthStr, false, FVector2D(3.f, 3.f));
	//}
	// ���ڲ���
	//if (DamageType)
	//{
	//	if (DamageType->IsA<USTUFireDamageType>())//IsA��ģ�庯������������������Ϊģ�����������ö������ڴ������򷵻�true
	//	{
	//		UE_LOG(LogHealthComponent, Display, TEXT("So hooooooooot!"));
	//	}
	//	else if (DamageType->IsA<USTUIceDamageType>())
	//	{
	//		UE_LOG(LogHealthComponent, Display, TEXT("So coooooooooold!"));
	//	}
	//}

	PlayCameraShake();//����ʱ�������
}

void USTUHealthComponent::HealUpdate()
{
	if (IsDead())return;//�����������Ȼ������Ѫ��bug
	SetHealth(Health + HealModifier);

	if (IsHealthFull() && GetWorld())//IsNearlyEqual���������ݴ�С�Ա�ʹ��IsNearlyEqual��Լ�ȣ�
	{
		GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);//��ȡ����->��ȡʱ�������.���������ʱ������
	}
}

void USTUHealthComponent::SetHealth(float NewHealth)//��װ�������ú����Ա㸴�ø�����
{
	Health = FMath::Clamp(NewHealth, 0.0f, MaxHealth);//ǯ�κ�����СΪ0�����ΪMaxHealth
	OnHealthChanged.Broadcast(Health);//���ý����ı��ί��
}


bool USTUHealthComponent::TryToAddHealth(float HealthAmount)
{
	if (IsDead() || IsHealthFull())return false;//����ֵ������������false��������ִ��PickupWasTaken()
	SetHealth(Health + HealthAmount);
	return true;
}

bool USTUHealthComponent::IsHealthFull() const//�������const��Ҫ�ǳ�Ա��������Ϊ�ǳ�Ա������������Ϻ���ͷ�
{
	return FMath::IsNearlyEqual(Health, MaxHealth);
}

void USTUHealthComponent::PlayCameraShake()
{
	if (IsDead())return;//��������˳�

	const auto Player = Cast<APawn>(GetOwner());//��ȡ���ת��Ϊ������
	if (!Player)return;

	const auto Controller = Player->GetController<APlayerController>();//ͨ��APawn���GetController<>()���غ�������ȡ��ҿ�����
	if (!Controller || !Controller->PlayerCameraManager)return;//�жϿ��������Ƿ�������������������

	Controller->PlayerCameraManager->StartCameraShake(CameraShake);//���������������ƹ������еĿ�ʼ����������������붶���������TSubclassOf<UCameraShakeBase>
}