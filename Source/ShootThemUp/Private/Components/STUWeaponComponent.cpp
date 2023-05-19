// Shoot Them Up , All Right Reserved


#include "Components/STUWeaponComponent.h"
#include "Weapon/STUBaseWeapon.h"
#include "GameFramework/Character.h"
#include "Animations/STUEquipFinishedAnimNotify.h"
#include "Player/STUBaseCharacter.h"
#include "Animations/STUReloadFinishedAnimNotify.h"
#include "Animations/AnimUtil.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeaponComponent, All, All);

constexpr static int32 WeaponNum = 2; //constexpr 关键字的功能是使指定的常量表达式获得在程序编译阶段计算出结果的能力，而不必等到程序运行阶段。

USTUWeaponComponent::USTUWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void USTUWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	checkf(WeaponData.Num() == WeaponNum, TEXT("Our character can hold only %i weapon items"), WeaponNum);//检查武器数量

	InitAnimations();
	SpawnWeapons();

	EquipWeapon(CurrentWeaponIndex);
}

void USTUWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)//结束时自动调用的函数，角色被删除前先删除角色上的组件
{
	CurrentWeapon = nullptr;//当前武器置空
	for (auto Weapon : Weapons)//循环武器数组
	{
		Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);//从Actor身上删除附着
		Weapon->SetLifeSpan(5.0f);
	}
	Weapons.Empty();//清除武器数组

	Super::EndPlay(EndPlayReason);//结束的父类中函数应最后调用
}

void USTUWeaponComponent::SpawnWeapons()
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());//将拥有者赋值到局部玩家类型数据
	if (!Character|| !GetWorld())return;

	ASTUBaseWeapon* Weapon = nullptr;
	for (auto OneWeaponData : WeaponData)//范围循环,然后在蓝图中初始化数组，如果为空运行游戏则会出现断点
	{
		 Weapon = GetWorld()->SpawnActor<ASTUBaseWeapon>(OneWeaponData.WeaponClass);//WeaponClasses中在蓝图定义的元素转换后放入Weapons数组中
		//把武器类转换到父类ASTUBaseWeapon，并生成ASTUBaseWeapon类型的指针
		if (!Weapon)continue;//C++ 中的 continue 语句有点像 break 语句。但它不是强迫终止，continue 会跳过当前循环中的代码，强迫开始下一次循环。

		Weapon->SetOwner(Character);
		Weapon->OnClipEmpty.AddUObject(this, &USTUWeaponComponent::OnClipEmpty);//绑定弹夹为空委托
		Weapons.Add(Weapon);//添加到武器数组

	}
	AttachWeaponToSocket(Weapon, Character->GetMesh(), WeaponArmorySocketName);//武器插入倒武器库插槽
}

void USTUWeaponComponent::AttachWeaponToSocket(ASTUBaseWeapon* Weapon,USceneComponent* SceneComponent,const FName& SocketName)
{
	if (!Weapon || !SceneComponent)return;

	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
	//EAttachmentRule包含：KeepRelative（保持相对）;KeepWorld（保持世界）;SnapToTarget(对齐到目标） 
	Weapon->AttachToComponent(SceneComponent, AttachmentRules, SocketName);
	//AttachToComponent附着到组件函数参数一：附着组件的指针，参数二：附着规则，参数三：插槽名字SocketName

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

	if (CurrentWeapon)//不为空指针执行循环体
	{
		CurrentWeapon->StopFire();//停止切换前武器的发射状态
		AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(),WeaponArmorySocketName);//切换武器到背部
	}

	CurrentWeapon = Weapons[WeaponIndex];//设置当前武器
	//在准备武器时获取当前对应的武器结构体
	//CurrentReloadAnimMontage = WeaponData[WeaponIndex].ReloadAnimMontage;//方法一：按顺序查找对应的动画蒙太奇
	//方法二：
	const auto CurrentWeaponData = WeaponData.FindByPredicate([&](const FWeaponData& Data) {//拉姆达函数[&]（参数）{函数体}
		return Data.WeaponClass == CurrentWeapon->GetClass();                               //当武器种类一样时返回true
															  });
	CurrentReloadAnimMontage = CurrentWeaponData ? CurrentWeaponData->ReloadAnimMontage : nullptr;//赋值当前对应的装弹动画

	AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponEquipSocketName);

	EquipAnimInProgress = true;//装备动画开始
	PlayAnimMontage(EquipAnimMontage);
}

void USTUWeaponComponent::StartFire()
{
	const ASTUBaseCharacter* Player = Cast<ASTUBaseCharacter>(GetOwner());//实现跑步时禁止射击 ,这里获得的是静态player！！！？？？未实现
	//FString PlayerStr = Player->IsRunning() ? "true" : "false";
	if (!CanFire()|| Player->RuningAnimInProgress) return;//跑步时禁止射击 Cast<ASTUBaseCharacter>(GetOwner())->IsRunning()
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
	if (!CanEquip())return;//防止无间隔时间的切换武器

	CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
	//TArray.Num()求数组元素个数，取模防止下标超出数组范围
	EquipWeapon(CurrentWeaponIndex);//装备当前武器
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
		UE_LOG(LogWeaponComponent, Error, TEXT("Equip anim notify is forgotten to set"));//检查装备动画是否设置
		checkNoEntry();//等效于check（false）
	}


	for (auto OneWeaponData : WeaponData)
	{
		auto ReloadFinishedNotify = AnimUtils::FindNotifyByClass<USTUReloadFinishedAnimNotify>(OneWeaponData.ReloadAnimMontage);
		if (!ReloadFinishedNotify)
		{
			UE_LOG(LogWeaponComponent, Error, TEXT("Reload anim notify is forgotten to set"));//检查装弹动画是否设置
			checkNoEntry();//等效于check（false）
		}
		
			ReloadFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnReloadFinished);
		
	}
}

void USTUWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComponent)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character|| MeshComponent!=Character->GetMesh())return;

		EquipAnimInProgress = false;//装备动画结束

}

void USTUWeaponComponent::OnReloadFinished(USkeletalMeshComponent* MeshComponent)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character || MeshComponent != Character->GetMesh())return;

	ReloadAnimInProgress = false;//装备动画结束

}

bool USTUWeaponComponent::CanFire() const
{
	return 	CurrentWeapon && !EquipAnimInProgress &&!ReloadAnimInProgress;//更换武器、装弹时不能射击
}

bool USTUWeaponComponent::CanEquip() const
{
	return !EquipAnimInProgress && !ReloadAnimInProgress;
}

bool USTUWeaponComponent::CanReload() const
{
	return 	CurrentWeapon && !EquipAnimInProgress && !ReloadAnimInProgress && !CurrentWeapon->IsNoClipsToReload();//更换武器、装弹时不能再装弹
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

bool USTUWeaponComponent::GetCurrentWeaponUIData(FWeaponUIData& UIData) const//不会更改类的内部状态可以用const
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

	//解决一种武器为空，切换另一种武器捡弹药时，空弹武器无法换弹夹的bug
	if (CurrentWeapon == AmmoEmptyWeapon)//如果当前武器和装弹武器一样则直接装弹
	{
		Reload();
	}
	else//如果当前武器和装弹武器不一样则直接遍历武器数组，将正确的武器换弹夹
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
		
		if (Weapon && Weapon->IsA(WeaponType))//IsA函数判断当前值是否与参数相同
		{
			return Weapon->TryToAddAmmo(ClipsAmount);
		}
	}

	return false;
}