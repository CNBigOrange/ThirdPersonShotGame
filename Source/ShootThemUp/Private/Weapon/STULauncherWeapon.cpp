// Shoot Them Up , All Right Reserved


#include "Weapon/STULauncherWeapon.h"
#include "Weapon/STUProjectile.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogLauncherWeapon, All, All);

void ASTULauncherWeapon::StartFire()
{
	MakeShot();
	//UE_LOG(LogLauncherWeapon, Display, TEXT("LauncherWeapon Start Fire!"));
}

void ASTULauncherWeapon::MakeShot()
{
	UE_LOG(LogLauncherWeapon, Display, TEXT("Make Shot1"));
	if (!GetWorld() || IsAmmoEmpty())return;//武器库用完返回
	UE_LOG(LogLauncherWeapon, Display, TEXT("Make Shot2"));
	FVector TraceStart, TraceEnd;
	if (!GetTraceData(TraceStart, TraceEnd)) return;//为追踪开始和结束点赋值
	UE_LOG(LogLauncherWeapon, Display, TEXT("Make Shot3"));
	FHitResult HitResult;//命中结果，结构体类型，包含时间、距离、起始点、碰撞点、是否碰撞等信息

	FHitResult CheckHitResult;//从枪口发出的检测射线结果
	FVector CheckTraceStart, CheckTraceEnd;//从枪口发出的检测射线起点终点
	CheckTraceStart = GetMuzzleWorldLocation();
	CheckTraceEnd = TraceEnd;

	MakeHit(HitResult, TraceStart, TraceEnd, CheckHitResult, CheckTraceStart, CheckTraceEnd);
	UE_LOG(LogLauncherWeapon, Display, TEXT("Make Hit"));
	const FVector EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;//终点位置，如果命中返回名终点位置，如果未命中返回追踪终点位置
	const FVector Direction = (EndPoint - GetMuzzleWorldLocation()).GetSafeNormal();
	//获得方向的单位向量，GetSafeNormal就是获取单位向量

	const FTransform SpawnTransform(FRotator::ZeroRotator, GetMuzzleWorldLocation());
	ASTUProjectile* Projectile = GetWorld()->SpawnActorDeferred<ASTUProjectile>( ProjectileClass, SpawnTransform);
	//延迟开始生成抛射物
	if (Projectile)
	{
		Projectile->SetShotDirection(Direction);//设置攻击方向
		Projectile->SetOwner(GetOwner());//设置拥有者
		Projectile->FinishSpawning(SpawnTransform);//完成生成
	}
	//设置抛射物参数

	DecreaseAmmo();
	//UGameplayStatics::FinishSpawningActor(Projectile, SpawnTransform);
	//完成生成Actor,和BeginDeferredActorSpawnFromClass配合使用
	
	SpawnMuzzleFX();//生成枪口特效
	UE_LOG(LogLauncherWeapon, Display, TEXT("Finish Make shot"));
}