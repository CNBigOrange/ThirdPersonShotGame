// Shoot Them Up , All Right Reserved


#include "Components/STUWeaponComponent.h"
#include "Weapon/STUBaseWeapon.h"
#include "GameFramework/Character.h"
#include "Animations/STUEquipFinishedAnimNotify.h"
#include "Player/STUBaseCharacter.h"
#include "Animations/STUReloadFinishedAnimNotify.h"
#include "Animations/AnimUtil.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeaponComponent, All, All);

constexpr static int32 WeaponNum = 2; //constexpr �ؼ��ֵĹ�����ʹָ���ĳ������ʽ����ڳ������׶μ��������������������صȵ��������н׶Ρ�

USTUWeaponComponent::USTUWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void USTUWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	checkf(WeaponData.Num() == WeaponNum, TEXT("Our character can hold only %i weapon items"), WeaponNum);//�����������

	InitAnimations();
	SpawnWeapons();

	EquipWeapon(CurrentWeaponIndex);
}

void USTUWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)//����ʱ�Զ����õĺ�������ɫ��ɾ��ǰ��ɾ����ɫ�ϵ����
{
	CurrentWeapon = nullptr;//��ǰ�����ÿ�
	for (auto Weapon : Weapons)//ѭ����������
	{
		Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);//��Actor����ɾ������
		Weapon->SetLifeSpan(5.0f);
	}
	Weapons.Empty();//�����������

	Super::EndPlay(EndPlayReason);//�����ĸ����к���Ӧ������
}

void USTUWeaponComponent::SpawnWeapons()
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());//��ӵ���߸�ֵ���ֲ������������
	if (!Character|| !GetWorld())return;

	ASTUBaseWeapon* Weapon = nullptr;
	for (auto OneWeaponData : WeaponData)//��Χѭ��,Ȼ������ͼ�г�ʼ�����飬���Ϊ��������Ϸ�����ֶϵ�
	{
		 Weapon = GetWorld()->SpawnActor<ASTUBaseWeapon>(OneWeaponData.WeaponClass);//WeaponClasses������ͼ�����Ԫ��ת�������Weapons������
		//��������ת��������ASTUBaseWeapon��������ASTUBaseWeapon���͵�ָ��
		if (!Weapon)continue;//C++ �е� continue ����е��� break ��䡣��������ǿ����ֹ��continue ��������ǰѭ���еĴ��룬ǿ�ȿ�ʼ��һ��ѭ����

		Weapon->SetOwner(Character);
		Weapon->OnClipEmpty.AddUObject(this, &USTUWeaponComponent::OnClipEmpty);//�󶨵���Ϊ��ί��
		Weapons.Add(Weapon);//��ӵ���������

	}
	AttachWeaponToSocket(Weapon, Character->GetMesh(), WeaponArmorySocketName);//�������뵹��������
}

void USTUWeaponComponent::AttachWeaponToSocket(ASTUBaseWeapon* Weapon,USceneComponent* SceneComponent,const FName& SocketName)
{
	if (!Weapon || !SceneComponent)return;

	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
	//EAttachmentRule������KeepRelative��������ԣ�;KeepWorld���������磩;SnapToTarget(���뵽Ŀ�꣩ 
	Weapon->AttachToComponent(SceneComponent, AttachmentRules, SocketName);
	//AttachToComponent���ŵ������������һ�����������ָ�룬�����������Ź��򣬲��������������SocketName

}

void USTUWeaponComponent::EquipWeapon(int32 WeaponIndex)
{
	if (WeaponIndex < 0 || WeaponIndex >= Weapons.Num())
	{
		UE_LOG(LogWeaponComponent, Warning, TEXT("Invalid weapon index"));
		return;
	}
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character)return;

	if (CurrentWeapon)//��Ϊ��ָ��ִ��ѭ����
	{
		CurrentWeapon->StopFire();//ֹͣ�л�ǰ�����ķ���״̬
		AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(),WeaponArmorySocketName);//�л�����������
	}

	CurrentWeapon = Weapons[WeaponIndex];//���õ�ǰ����
	//��׼������ʱ��ȡ��ǰ��Ӧ�������ṹ��
	//CurrentReloadAnimMontage = WeaponData[WeaponIndex].ReloadAnimMontage;//����һ����˳����Ҷ�Ӧ�Ķ�����̫��
	//��������
	const auto CurrentWeaponData = WeaponData.FindByPredicate([&](const FWeaponData& Data) {//��ķ�ﺯ��[&]��������{������}
		return Data.WeaponClass == CurrentWeapon->GetClass();                               //����������һ��ʱ����true
															  });
	CurrentReloadAnimMontage = CurrentWeaponData ? CurrentWeaponData->ReloadAnimMontage : nullptr;//��ֵ��ǰ��Ӧ��װ������

	AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponEquipSocketName);

	EquipAnimInProgress = true;//װ��������ʼ
	PlayAnimMontage(EquipAnimMontage);
}

void USTUWeaponComponent::StartFire()
{
	const ASTUBaseCharacter* Player = Cast<ASTUBaseCharacter>(GetOwner());//ʵ���ܲ�ʱ��ֹ��� ,�����õ��Ǿ�̬player������������δʵ��
	//FString PlayerStr = Player->IsRunning() ? "true" : "false";
	if (!CanFire()|| Player->RuningAnimInProgress) return;//�ܲ�ʱ��ֹ��� Cast<ASTUBaseCharacter>(GetOwner())->IsRunning()
	//if (Player->IsRunning()) return;
	CurrentWeapon->StartFire();
	//UE_LOG(LogWeaponComponent, Display, TEXT("Start Fire"));
}

void USTUWeaponComponent::StopFire()
{
	if (!CurrentWeapon)return;
	CurrentWeapon->StopFire();
}

void USTUWeaponComponent::NextWeapon()
{
	if (!CanEquip())return;//��ֹ�޼��ʱ����л�����

	CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
	//TArray.Num()������Ԫ�ظ�����ȡģ��ֹ�±곬�����鷶Χ
	EquipWeapon(CurrentWeaponIndex);//װ����ǰ����
}

void USTUWeaponComponent::PlayAnimMontage(UAnimMontage* Animation)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character)return;

	Character->PlayAnimMontage(Animation);
}

void USTUWeaponComponent::InitAnimations()
{
	auto EquipFinishedNotify = AnimUtils::FindNotifyByClass<USTUEquipFinishedAnimNotify>(EquipAnimMontage);
	if (EquipFinishedNotify)
	{
		EquipFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnEquipFinished);
	}
	else
	{
		UE_LOG(LogWeaponComponent, Error, TEXT("Equip anim notify is forgotten to set"));//���װ�������Ƿ�����
		checkNoEntry();//��Ч��check��false��
	}


	for (auto OneWeaponData : WeaponData)
	{
		auto ReloadFinishedNotify = AnimUtils::FindNotifyByClass<USTUReloadFinishedAnimNotify>(OneWeaponData.ReloadAnimMontage);
		if (!ReloadFinishedNotify)
		{
			UE_LOG(LogWeaponComponent, Error, TEXT("Reload anim notify is forgotten to set"));//���װ�������Ƿ�����
			checkNoEntry();//��Ч��check��false��
		}
		
			ReloadFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnReloadFinished);
		
	}
}

void USTUWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComponent)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character|| MeshComponent!=Character->GetMesh())return;

		EquipAnimInProgress = false;//װ����������

}

void USTUWeaponComponent::OnReloadFinished(USkeletalMeshComponent* MeshComponent)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character || MeshComponent != Character->GetMesh())return;

	ReloadAnimInProgress = false;//װ����������

}

bool USTUWeaponComponent::CanFire() const
{
	return 	CurrentWeapon && !EquipAnimInProgress &&!ReloadAnimInProgress;//����������װ��ʱ�������
}

bool USTUWeaponComponent::CanEquip() const
{
	return !EquipAnimInProgress && !ReloadAnimInProgress;
}

bool USTUWeaponComponent::CanReload() const
{
	return 	CurrentWeapon && !EquipAnimInProgress && !ReloadAnimInProgress && !CurrentWeapon->IsNoClipsToReload();//����������װ��ʱ������װ��
}

void USTUWeaponComponent::Reload()
{
	if (!CanReload())return;
	ReloadAnimInProgress = true;

	if (CurrentWeapon)
	{
		CurrentWeapon->ChangeClip();
	}

	//UE_LOG(LogWeaponComponent, Warning, TEXT("Reload"));
	PlayAnimMontage(CurrentReloadAnimMontage);
}

bool USTUWeaponComponent::GetCurrentWeaponUIData(FWeaponUIData& UIData) const//�����������ڲ�״̬������const
{
	if (CurrentWeapon)
	{
		UIData = CurrentWeapon->GetUIData();
		return true;
	}
	return false;
}

bool USTUWeaponComponent::GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const
{
	if (CurrentWeapon)
	{
		AmmoData = CurrentWeapon->GetAmmoData();
		return true;
	}
	return false;
}

void USTUWeaponComponent::OnClipEmpty(ASTUBaseWeapon* AmmoEmptyWeapon)
{
	if (!AmmoEmptyWeapon)return;

	//���һ������Ϊ�գ��л���һ��������ҩʱ���յ������޷������е�bug
	if (CurrentWeapon == AmmoEmptyWeapon)//�����ǰ������װ������һ����ֱ��װ��
	{
		Reload();
	}
	else//�����ǰ������װ��������һ����ֱ�ӱ����������飬����ȷ������������
	{
		for (const auto Weapon : Weapons)
		{
			if (Weapon == AmmoEmptyWeapon)
			{
				Weapon->ChangeClip();
			}
		}
	}
}

bool USTUWeaponComponent::TryToAddAmmo(TSubclassOf<ASTUBaseWeapon> WeaponType, int32 ClipsAmount)
{

	for (const auto Weapon : Weapons)
	{
		
		if (Weapon && Weapon->IsA(WeaponType))//IsA�����жϵ�ǰֵ�Ƿ��������ͬ
		{
			return Weapon->TryToAddAmmo(ClipsAmount);
		}
	}

	return false;
}