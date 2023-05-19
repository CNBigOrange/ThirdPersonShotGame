// Shoot Them Up , All Right Reserved


#include "Weapon/STUBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "Player/STUBaseCharacter.h"
#include "Components/STUWeaponComponent.h"
#include "STUCoreTypes.h"
#include "NiagaraFunctionLibrary.h"//尼亚加拉函数库
#include "NiagaraComponent.h"//尼亚加拉组件
#include "AIController.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseWeapon, All, All);

ASTUBaseWeapon::ASTUBaseWeapon()
{

	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	WeaponMesh->SetupAttachment(GetRootComponent());//设置附着

}

void ASTUBaseWeapon::BeginPlay()
{
	Super::BeginPlay();

	check(WeaponMesh);
	checkf(DefaultAmmo.Bullets > 0, TEXT("Bullets count couldn't be less or equal zero"));//检查子弹不能小于等于0
	checkf(DefaultAmmo.Clips > 0, TEXT("Clips count couldn't be less or equal zero"));

	CurrentAmmo = DefaultAmmo;
}

void ASTUBaseWeapon::StartFire()
{
	//UE_LOG(LogBaseWeapon, Display, TEXT("BaseWeapon Start Fire"));
}

void ASTUBaseWeapon::StopFire()
{

}

void ASTUBaseWeapon::MakeShot()
{

}

APlayerController* ASTUBaseWeapon::GetPlayerController() const
{
	const auto Player = Cast<ACharacter>(GetOwner());//Actor*转换为ACharacter,GetOwner()获取武器组件，转化为ACharacter
	if (!Player)return nullptr;
	//UE_LOG(LogBaseWeapon, Display, TEXT("Player IsValid"));
	return Player->GetController<APlayerController>();//模板函数，包装了类型转换{return Cast<T>(GetController());}
	//获取控制器并转换为APlayerController，ACharacter内包含APlayerController
}

bool ASTUBaseWeapon::GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const
{
	//UE_LOG(LogBaseWeapon, Display, TEXT("GetPlayerController begin"));
	const auto Controller = GetPlayerController();
	if (!Controller)//如果玩家控制器不存在，则可能为AI角色，尝试获取AIController
	{
		const auto Player = Cast<ACharacter>(GetOwner());
		if (!Player)return false;

		const auto AIController = Player->GetController<AAIController>();
		if (!AIController)return false;
			
			//UE_LOG(LogBaseWeapon, Display, TEXT("GetPlayerController successed"));
			AIController->GetPlayerViewPoint(ViewLocation, ViewRotation);
			return true;
	}
	//UE_LOG(LogBaseWeapon, Display, TEXT("GetPlayerController successed"));
	Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);//获得玩家控制器视点的位置和旋转，赋值给ViewLocation和ViewRotation
	return true;
}

FVector ASTUBaseWeapon::GetMuzzleWorldLocation() const//获取插槽位置并返回
{
	return WeaponMesh->GetSocketLocation(MuzzleSocketName);
}

bool ASTUBaseWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
	FVector ViewLocation;
	FRotator ViewRotation;
	UE_LOG(LogBaseWeapon, Display, TEXT("GetTraceData Begin"));
	if (!GetPlayerViewPoint(ViewLocation, ViewRotation))return false;
	UE_LOG(LogBaseWeapon, Display, TEXT("GetPlayerViewPoint Finished"));
	//const FTransform SocketTransform = WeaponMesh->GetSocketTransform(MuzzleSocketName);//FName不用带“”，FString需要带“”
	//获得插件位置变换信息
	TraceStart = ViewLocation;//起点位置坐标

	const FVector ShootDirection = ViewRotation.Vector();// SocketTransform.GetRotation().GetForwardVector();
	//FMath::VRandCone()
	TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
	return true;
}

void ASTUBaseWeapon::MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd,FHitResult& CheckHitResult,const FVector& CheckTraceStart, const FVector& CheckTraceEnd)//双射线检测
{
	if (!GetWorld())return;
	FCollisionQueryParams CollisionParams;//集合了碰撞查询的各类参数
	CollisionParams.AddIgnoredActor(GetOwner());//为查询参数添加忽视对象，解决某些情况下摄像机视点攻击自身bug
	CollisionParams.bReturnPhysicalMaterial = true;//是否返回物理材质为true，


	GetWorld()->LineTraceSingleByChannel(CheckHitResult, CheckTraceStart, CheckTraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);


	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);//给HitResult赋值
	//单通道线追踪，参数一：追踪命中结果；参数二：追踪起点；参数三：追踪终点；参数四：追踪碰撞通道ECollisionChannel，ECC_Visibility,ECC_Camera,ECC_WorldStatic都是碰撞通道
}

void ASTUBaseWeapon::DecreaseAmmo()
{
	if (CurrentAmmo.Bullets == 0)return;

	CurrentAmmo.Bullets--;
	//LogAmmo();

	if (IsClipEmpty() && !IsAmmoEmpty())//弹夹弹药为空且武器库不为空
	{
		OnClipEmpty.Broadcast(this);//发出委托,参数指向当前武器
		//const auto Player = Cast<ASTUBaseCharacter>(GetOwner());//GetOwner获得拥有者，然后转换为相应子类
		//Player->GetWeaponComponent()->Reload();//GetWeaponComponent为角色中自定义的接口函数
		//ChangeClip();
	}
}

bool ASTUBaseWeapon::IsAmmoEmpty() const
{
	return !CurrentAmmo.Infinite && CurrentAmmo.Clips == 0 && IsClipEmpty();
}

bool ASTUBaseWeapon::IsClipEmpty() const//弹夹为空
{
	return CurrentAmmo.Bullets == 0;
}

bool ASTUBaseWeapon::IsNoClipsToReload() const
{
	return  CurrentAmmo.Clips == 0;
}

bool ASTUBaseWeapon::IsBulletsFull() const
{
	return CurrentAmmo.Bullets == DefaultAmmo.Bullets;
}

void ASTUBaseWeapon::ChangeClip()
{
	if (CurrentAmmo.Clips == 0)return;//当弹夹为0无法攻击
	StopFire();//换弹夹需要调用虚函数停止攻击，在有计时器的子类武器中需要在换弹夹时清除计时器，解决机关枪鼠标键未松同时攻击和换弹夹的bug
	CurrentAmmo.Bullets = DefaultAmmo.Bullets;
	if (!CurrentAmmo.Infinite)
	{
		CurrentAmmo.Clips--;
	}
	//UE_LOG(LogBaseWeapon, Display, TEXT("--------Change Clip----------"));

}

void ASTUBaseWeapon::LogAmmo()
{
	FString AmmoInfo = "Ammo:" + FString::FromInt(CurrentAmmo.Bullets) + "/";
	AmmoInfo += CurrentAmmo.Infinite ? "Infinite" : FString::FromInt(CurrentAmmo.Clips);//无限子弹或者弹夹数量
	UE_LOG(LogBaseWeapon,Display, TEXT("%s"), *AmmoInfo);
}

bool ASTUBaseWeapon::IsAmmoFull() const
{
	return DefaultAmmo.Clips == CurrentAmmo.Clips && //弹夹为默认值
		DefaultAmmo.Bullets == CurrentAmmo.Bullets;//子弹为默认值
}

bool ASTUBaseWeapon::TryToAddAmmo(int32 ClipsAmount)//捡弹夹逻辑
{

	if (CurrentAmmo.Infinite || IsAmmoFull() || ClipsAmount <= 0)return false;

	if (IsAmmoEmpty())
	{
		UE_LOG(LogBaseWeapon, Display, TEXT("Ammo was empty!"));
		CurrentAmmo.Clips = FMath::Clamp(ClipsAmount, 0, DefaultAmmo.Clips + 1);//Clamp参数二：取值范围最小值，参数三：取值范围最大值，+1是为了将一个子弹填满且弹夹填满
		OnClipEmpty.Broadcast(this);
	}
	else if (CurrentAmmo.Clips < DefaultAmmo.Clips)
	{
		const auto NextClipsAmount = CurrentAmmo.Clips + ClipsAmount;
		if (DefaultAmmo.Clips - NextClipsAmount >= 0)
		{
			CurrentAmmo.Clips = NextClipsAmount;
			UE_LOG(LogBaseWeapon, Display, TEXT("Clips were added!"));
		}
		else
		{
			CurrentAmmo.Clips = DefaultAmmo.Clips;
			CurrentAmmo.Bullets = DefaultAmmo.Bullets;
			UE_LOG(LogBaseWeapon, Display, TEXT("Ammo is full now"));
		}
	}
	else
	{
		CurrentAmmo.Bullets = DefaultAmmo.Bullets;
		UE_LOG(LogBaseWeapon, Display, TEXT("Bullets were added"));
	}
	return true;
}

UNiagaraComponent* ASTUBaseWeapon::SpawnMuzzleFX()
{
	//生成粒子系统附着
	return UNiagaraFunctionLibrary::SpawnSystemAttached(MuzzleFX,//尼亚加拉系统实例
												 WeaponMesh,//武器网格体
												 MuzzleSocketName,//枪口插槽名称
												 FVector::ZeroVector,//位置
												 FRotator::ZeroRotator,//向量
												 EAttachLocation::SnapToTarget,//对齐到目标
												 true);//自动销毁
}