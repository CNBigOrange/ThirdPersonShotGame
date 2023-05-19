// Shoot Them Up , All Right Reserved


#include "Weapon/STURifleWeapon.h"
#include "DrawDebugHelpers.h"
#include "Weapon/Components/STUWeaponFXComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Player/STUPlayerController.h"

DEFINE_LOG_CATEGORY_STATIC(LogRifleWeapon, All, All);

ASTURifleWeapon::ASTURifleWeapon()
{
	WeaponFXComponent = CreateDefaultSubobject<USTUWeaponFXComponent>("WeaponFXComponent");
}

void ASTURifleWeapon::BeginPlay()
{
	Super::BeginPlay();

	check(WeaponFXComponent);
}

void ASTURifleWeapon::StartFire()
{
	InitMuzzleFX();//初始化枪口特效
	MakeShot();
	const auto Player = Cast<ASTUPlayerController>(GetOwner());//当ASTUPlayerController不存在时，则为AIController，跳过下方if语句
	if (IsBulletsFull()&& Player)//开枪弹药减一之后如果子弹数量仍为满，则说明原来子弹为一，开枪后子弹重装了，装弹需要删除计时器，解决当子弹为一时，开枪创建了计时器后马上再换弹时任然持续射击的BUG
	{ 
		//UE_LOG(LogRifleWeapon, Display, TEXT("Bullet = 1"));
		StopFire();
		return;
	}
	GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &ASTURifleWeapon::MakeShot, TimeBetweenShots, true);
	//获取世界计时管理器，设置计时器
	//UE_LOG(LogRifleWeapon, Display, TEXT("RifleWeapon Start Fire"));
}

void ASTURifleWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(ShotTimerHandle);
	SetMuzzleFXVisibility(false);//设置枪口火焰可见性
}

void ASTURifleWeapon::MakeShot()
{
	UE_LOG(LogRifleWeapon, Display, TEXT("Make Shot1"));
	if (!GetWorld()||IsAmmoEmpty()||IsClipEmpty())//武器库用完返回
	{
		StopFire();
		return;
	}
	UE_LOG(LogRifleWeapon, Display, TEXT("Make Shot2"));
	FVector TraceStart, TraceEnd;
	if (!GetTraceData(TraceStart, TraceEnd)) //获取轨迹数据同时判断是否存在
	{
		StopFire();
		return;
	}
	UE_LOG(LogRifleWeapon, Display, TEXT("Make Shot3"));
	FHitResult HitResult;//命中结果，结构体类型，包含时间、距离、起始点、碰撞点、是否碰撞等信息  从视点发出的射线
	FHitResult CheckHitResult;//从枪口发出的检测射线结果
	FVector CheckTraceStart, CheckTraceEnd;//从枪口发出的检测射线起点终点
	CheckTraceStart = GetMuzzleWorldLocation();
	CheckTraceEnd = TraceEnd;

	MakeHit(HitResult, TraceStart, TraceEnd, CheckHitResult, CheckTraceStart, CheckTraceEnd);//给命中结果，跟踪起点终点赋值
	UE_LOG(LogRifleWeapon, Display, TEXT("Make Hit"));
	FVector TraceFXEnd = TraceEnd;//弹道终点
	//if (!HitResult.bBlockingHit && CheckHitResult.bBlockingHit)
	//{
	//	//UE_LOG(LogRifleWeapon, Display, TEXT("!HitResult.bBlockingHit && CheckHitResult.bBlockingHit"));
	//	CheckTraceEnd = CheckHitResult.ImpactPoint;
	//	MakeDamage(CheckHitResult);
	//	//WeaponFXComponent->PlayImpactFX(HitResult);
	//	//DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), HitResult.ImpactPoint, FColor::Red, false, 2.0f, 0, 2.0);

	//	//DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 5.0f, 12, FColor::Red, false, 1.0f);

	//	WeaponFXComponent->PlayImpactFX(CheckHitResult);//播放粒子效果
	//	SpawnTraceFX(GetMuzzleWorldLocation(), CheckTraceEnd);////生成弹道特效，参数为弹道起点和终点
	//}
	if (HitResult.bBlockingHit && !CheckHitResult.bBlockingHit)
	{
		//UE_LOG(LogRifleWeapon, Display, TEXT("HitResult.bBlockingHit && !CheckHitResult.bBlockingHit"));
		TraceEnd = HitResult.ImpactPoint;
		MakeDamage(HitResult);
		//WeaponFXComponent->PlayImpactFX(HitResult);
		//DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), HitResult.ImpactPoint, FColor::Red, false, 2.0f, 0, 2.0);

		//DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 5.0f, 12, FColor::Red, false, 1.0f);

		WeaponFXComponent->PlayImpactFX(HitResult);//播放粒子效果
		SpawnTraceFX(GetMuzzleWorldLocation(), TraceEnd);////生成弹道特效，参数为弹道起点和终点
	}
	else if (HitResult.bBlockingHit && CheckHitResult.bBlockingHit)
	{
		//float i = FVector::Dist(TraceStart, TraceEnd) - FVector::Dist(CheckTraceStart, CheckTraceEnd);//枪口到终点和相机到终点距离差值
		//if (HitResult.Distance < CheckHitResult.Distance + i-100)
		//{
			//UE_LOG(LogRifleWeapon, Display, TEXT("CheckHitResult"));
			//UE_LOG(LogRifleWeapon, Display, TEXT("%f"),i);
			//UE_LOG(LogRifleWeapon, Display, TEXT("%s"), *FString::SanitizeFloat(HitResult.Distance));
			//UE_LOG(LogRifleWeapon, Display, TEXT("%s"), *FString::SanitizeFloat(CheckHitResult.Distance));
			TraceEnd = HitResult.ImpactPoint;
			MakeDamage(HitResult);
			WeaponFXComponent->PlayImpactFX(HitResult);//播放粒子效果
			SpawnTraceFX(GetMuzzleWorldLocation(), TraceEnd);////生成弹道特效，参数为弹道起点和终点
		//}
		//else
		//{
		//	//UE_LOG(LogRifleWeapon, Display, TEXT("HitResult"));
		//	CheckTraceEnd = CheckHitResult.ImpactPoint;
		//	MakeDamage(CheckHitResult);
		//	WeaponFXComponent->PlayImpactFX(CheckHitResult);//播放粒子效果
		//	SpawnTraceFX(GetMuzzleWorldLocation(), CheckTraceEnd);////生成弹道特效，参数为弹道起点和终点
		//}
		//WeaponFXComponent->PlayImpactFX(HitResult);
		//DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), HitResult.ImpactPoint, FColor::Red, false, 2.0f, 0, 2.0);
		//DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 5.0f, 12, FColor::Red, false, 1.0f);
	}
	else
	{
		//UE_LOG(LogRifleWeapon, Display, TEXT("!HitResult.bBlockingHit && CheckHitResult.bBlockingHit"));
		//UE_LOG(LogRifleWeapon, Display, TEXT("%s"), *FString::SanitizeFloat(HitResult.Distance));//
		//UE_LOG(LogRifleWeapon, Display, TEXT("%s"), *FString::SanitizeFloat(CheckHitResult.Distance));
		SpawnTraceFX(GetMuzzleWorldLocation(), TraceFXEnd);////生成弹道特效，参数为弹道起点和终点
	    //DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), TraceEnd, FColor::Red, false, 1.0f, 0, 1.0);
	}
	
	DecreaseAmmo();
	UE_LOG(LogRifleWeapon, Display, TEXT("Finish make shot"));
}

bool ASTURifleWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
	FVector ViewLocation;
	FRotator ViewRotation;
	if (!GetPlayerViewPoint(ViewLocation, ViewRotation))return false;
	UE_LOG(LogRifleWeapon, Display, TEXT("GetPlayerViewPoint Succssed!"));
	//const FTransform SocketTransform = WeaponMesh->GetSocketTransform(MuzzleSocketName);//FName不用带“”，FString需要带“”
	//获得插件位置变换信息
	TraceStart = ViewLocation;//起点位置坐标
	const auto HalfRad = FMath::DegreesToRadians(BulletSpread);//子弹偏移数值
	const FVector ShootDirection = FMath::VRandCone(ViewRotation.Vector(), HalfRad);// SocketTransform.GetRotation().GetForwardVector();
	//FMath::VRandCone()
	TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
	return true;
}

void ASTURifleWeapon::MakeDamage(const FHitResult& HitResult)
{
	const auto DamagedActor = HitResult.GetActor();//获取命中对象
	if (!DamagedActor)return;

	if (HitResult.BoneName == "b_head")//爆头
	{
		DamageAmount = 100.0f;
		GEngine->AddOnScreenDebugMessage(0, 3.0f, FColor::Red, "ShotHead!", false, FVector2D(1.f, 1.f));
	}
	if (HitResult.BoneName == "b_spine1")//命中胸部
	{
		DamageAmount = 30.0f;
	}

	DamagedActor->TakeDamage(DamageAmount, FDamageEvent(), GetPlayerController(), this);
	//TakeDamage()函数：参数一：伤害值；参数二：伤害事件；参数三：伤害控制器，这里是控制武器的玩家controller；参数四：伤害造成者BaseWeapon
	DamageAmount = 10.0f;

	//UE_LOG(LogBaseWeapon, Display, TEXT("Bone:%s"), *HitResult.BoneName.ToString());//将攻击部位打印出来，HitResult里面包含了BoneName骨骼名字 Name of bone we hit (for skeletal meshes)
}

void ASTURifleWeapon::InitMuzzleFX()//初始化枪口特效
{
	if (!MuzzleFXComponent)//如果枪口特效组件不存在
	{
		MuzzleFXComponent = SpawnMuzzleFX();
	}
	SetMuzzleFXVisibility(true);
}

void ASTURifleWeapon::SetMuzzleFXVisibility(bool Visible)//设置枪口特效可见性
{
	if (MuzzleFXComponent)
	{
		MuzzleFXComponent->SetPaused(!Visible);//设置停止和启用尼亚加拉组件
		MuzzleFXComponent->SetVisibility(Visible, true);//设置可见性，参数一是否可见，参数二：是否传递给子类
	}
}

void ASTURifleWeapon::SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd)
{
	const auto TraceFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TraceFX, TraceStart);
	//在指定位置生成尼亚加拉系统，参数一：背景世界，参数二：跟踪特效，参数三：跟踪起点
	if (TraceFXComponent)
	{
		TraceFXComponent->SetNiagaraVariableVec3(TraceTargetName, TraceEnd);//尼亚加拉系统中的生成变量向量的函数，参数一：跟踪目标向量名称，参数二：跟踪目标向量值
		//SetNiagaraVariableVec3函数中Vec3对应的尼亚加拉系统蓝图中用户公开栏下的TraceTarget函数
	}
}