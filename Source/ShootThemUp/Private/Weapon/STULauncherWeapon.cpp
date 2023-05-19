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
	if (!GetWorld() || IsAmmoEmpty())return;//���������귵��
	UE_LOG(LogLauncherWeapon, Display, TEXT("Make Shot2"));
	FVector TraceStart, TraceEnd;
	if (!GetTraceData(TraceStart, TraceEnd)) return;//Ϊ׷�ٿ�ʼ�ͽ����㸳ֵ
	UE_LOG(LogLauncherWeapon, Display, TEXT("Make Shot3"));
	FHitResult HitResult;//���н�����ṹ�����ͣ�����ʱ�䡢���롢��ʼ�㡢��ײ�㡢�Ƿ���ײ����Ϣ

	FHitResult CheckHitResult;//��ǹ�ڷ����ļ�����߽��
	FVector CheckTraceStart, CheckTraceEnd;//��ǹ�ڷ����ļ����������յ�
	CheckTraceStart = GetMuzzleWorldLocation();
	CheckTraceEnd = TraceEnd;

	MakeHit(HitResult, TraceStart, TraceEnd, CheckHitResult, CheckTraceStart, CheckTraceEnd);
	UE_LOG(LogLauncherWeapon, Display, TEXT("Make Hit"));
	const FVector EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;//�յ�λ�ã�������з������յ�λ�ã����δ���з���׷���յ�λ��
	const FVector Direction = (EndPoint - GetMuzzleWorldLocation()).GetSafeNormal();
	//��÷���ĵ�λ������GetSafeNormal���ǻ�ȡ��λ����

	const FTransform SpawnTransform(FRotator::ZeroRotator, GetMuzzleWorldLocation());
	ASTUProjectile* Projectile = GetWorld()->SpawnActorDeferred<ASTUProjectile>( ProjectileClass, SpawnTransform);
	//�ӳٿ�ʼ����������
	if (Projectile)
	{
		Projectile->SetShotDirection(Direction);//���ù�������
		Projectile->SetOwner(GetOwner());//����ӵ����
		Projectile->FinishSpawning(SpawnTransform);//�������
	}
	//�������������

	DecreaseAmmo();
	//UGameplayStatics::FinishSpawningActor(Projectile, SpawnTransform);
	//�������Actor,��BeginDeferredActorSpawnFromClass���ʹ��
	
	SpawnMuzzleFX();//����ǹ����Ч
	UE_LOG(LogLauncherWeapon, Display, TEXT("Finish Make shot"));
}