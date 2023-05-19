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
	InitMuzzleFX();//��ʼ��ǹ����Ч
	MakeShot();
	const auto Player = Cast<ASTUPlayerController>(GetOwner());//��ASTUPlayerController������ʱ����ΪAIController�������·�if���
	if (IsBulletsFull()&& Player)//��ǹ��ҩ��һ֮������ӵ�������Ϊ������˵��ԭ���ӵ�Ϊһ����ǹ���ӵ���װ�ˣ�װ����Ҫɾ����ʱ����������ӵ�Ϊһʱ����ǹ�����˼�ʱ���������ٻ���ʱ��Ȼ���������BUG
	{ 
		//UE_LOG(LogRifleWeapon, Display, TEXT("Bullet = 1"));
		StopFire();
		return;
	}
	GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &ASTURifleWeapon::MakeShot, TimeBetweenShots, true);
	//��ȡ�����ʱ�����������ü�ʱ��
	//UE_LOG(LogRifleWeapon, Display, TEXT("RifleWeapon Start Fire"));
}

void ASTURifleWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(ShotTimerHandle);
	SetMuzzleFXVisibility(false);//����ǹ�ڻ���ɼ���
}

void ASTURifleWeapon::MakeShot()
{
	UE_LOG(LogRifleWeapon, Display, TEXT("Make Shot1"));
	if (!GetWorld()||IsAmmoEmpty()||IsClipEmpty())//���������귵��
	{
		StopFire();
		return;
	}
	UE_LOG(LogRifleWeapon, Display, TEXT("Make Shot2"));
	FVector TraceStart, TraceEnd;
	if (!GetTraceData(TraceStart, TraceEnd)) //��ȡ�켣����ͬʱ�ж��Ƿ����
	{
		StopFire();
		return;
	}
	UE_LOG(LogRifleWeapon, Display, TEXT("Make Shot3"));
	FHitResult HitResult;//���н�����ṹ�����ͣ�����ʱ�䡢���롢��ʼ�㡢��ײ�㡢�Ƿ���ײ����Ϣ  ���ӵ㷢��������
	FHitResult CheckHitResult;//��ǹ�ڷ����ļ�����߽��
	FVector CheckTraceStart, CheckTraceEnd;//��ǹ�ڷ����ļ����������յ�
	CheckTraceStart = GetMuzzleWorldLocation();
	CheckTraceEnd = TraceEnd;

	MakeHit(HitResult, TraceStart, TraceEnd, CheckHitResult, CheckTraceStart, CheckTraceEnd);//�����н������������յ㸳ֵ
	UE_LOG(LogRifleWeapon, Display, TEXT("Make Hit"));
	FVector TraceFXEnd = TraceEnd;//�����յ�
	//if (!HitResult.bBlockingHit && CheckHitResult.bBlockingHit)
	//{
	//	//UE_LOG(LogRifleWeapon, Display, TEXT("!HitResult.bBlockingHit && CheckHitResult.bBlockingHit"));
	//	CheckTraceEnd = CheckHitResult.ImpactPoint;
	//	MakeDamage(CheckHitResult);
	//	//WeaponFXComponent->PlayImpactFX(HitResult);
	//	//DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), HitResult.ImpactPoint, FColor::Red, false, 2.0f, 0, 2.0);

	//	//DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 5.0f, 12, FColor::Red, false, 1.0f);

	//	WeaponFXComponent->PlayImpactFX(CheckHitResult);//��������Ч��
	//	SpawnTraceFX(GetMuzzleWorldLocation(), CheckTraceEnd);////���ɵ�����Ч������Ϊ���������յ�
	//}
	if (HitResult.bBlockingHit && !CheckHitResult.bBlockingHit)
	{
		//UE_LOG(LogRifleWeapon, Display, TEXT("HitResult.bBlockingHit && !CheckHitResult.bBlockingHit"));
		TraceEnd = HitResult.ImpactPoint;
		MakeDamage(HitResult);
		//WeaponFXComponent->PlayImpactFX(HitResult);
		//DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), HitResult.ImpactPoint, FColor::Red, false, 2.0f, 0, 2.0);

		//DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 5.0f, 12, FColor::Red, false, 1.0f);

		WeaponFXComponent->PlayImpactFX(HitResult);//��������Ч��
		SpawnTraceFX(GetMuzzleWorldLocation(), TraceEnd);////���ɵ�����Ч������Ϊ���������յ�
	}
	else if (HitResult.bBlockingHit && CheckHitResult.bBlockingHit)
	{
		//float i = FVector::Dist(TraceStart, TraceEnd) - FVector::Dist(CheckTraceStart, CheckTraceEnd);//ǹ�ڵ��յ��������յ�����ֵ
		//if (HitResult.Distance < CheckHitResult.Distance + i-100)
		//{
			//UE_LOG(LogRifleWeapon, Display, TEXT("CheckHitResult"));
			//UE_LOG(LogRifleWeapon, Display, TEXT("%f"),i);
			//UE_LOG(LogRifleWeapon, Display, TEXT("%s"), *FString::SanitizeFloat(HitResult.Distance));
			//UE_LOG(LogRifleWeapon, Display, TEXT("%s"), *FString::SanitizeFloat(CheckHitResult.Distance));
			TraceEnd = HitResult.ImpactPoint;
			MakeDamage(HitResult);
			WeaponFXComponent->PlayImpactFX(HitResult);//��������Ч��
			SpawnTraceFX(GetMuzzleWorldLocation(), TraceEnd);////���ɵ�����Ч������Ϊ���������յ�
		//}
		//else
		//{
		//	//UE_LOG(LogRifleWeapon, Display, TEXT("HitResult"));
		//	CheckTraceEnd = CheckHitResult.ImpactPoint;
		//	MakeDamage(CheckHitResult);
		//	WeaponFXComponent->PlayImpactFX(CheckHitResult);//��������Ч��
		//	SpawnTraceFX(GetMuzzleWorldLocation(), CheckTraceEnd);////���ɵ�����Ч������Ϊ���������յ�
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
		SpawnTraceFX(GetMuzzleWorldLocation(), TraceFXEnd);////���ɵ�����Ч������Ϊ���������յ�
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
	//const FTransform SocketTransform = WeaponMesh->GetSocketTransform(MuzzleSocketName);//FName���ô�������FString��Ҫ������
	//��ò��λ�ñ任��Ϣ
	TraceStart = ViewLocation;//���λ������
	const auto HalfRad = FMath::DegreesToRadians(BulletSpread);//�ӵ�ƫ����ֵ
	const FVector ShootDirection = FMath::VRandCone(ViewRotation.Vector(), HalfRad);// SocketTransform.GetRotation().GetForwardVector();
	//FMath::VRandCone()
	TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
	return true;
}

void ASTURifleWeapon::MakeDamage(const FHitResult& HitResult)
{
	const auto DamagedActor = HitResult.GetActor();//��ȡ���ж���
	if (!DamagedActor)return;

	if (HitResult.BoneName == "b_head")//��ͷ
	{
		DamageAmount = 100.0f;
		GEngine->AddOnScreenDebugMessage(0, 3.0f, FColor::Red, "ShotHead!", false, FVector2D(1.f, 1.f));
	}
	if (HitResult.BoneName == "b_spine1")//�����ز�
	{
		DamageAmount = 30.0f;
	}

	DamagedActor->TakeDamage(DamageAmount, FDamageEvent(), GetPlayerController(), this);
	//TakeDamage()����������һ���˺�ֵ�����������˺��¼������������˺��������������ǿ������������controller�������ģ��˺������BaseWeapon
	DamageAmount = 10.0f;

	//UE_LOG(LogBaseWeapon, Display, TEXT("Bone:%s"), *HitResult.BoneName.ToString());//��������λ��ӡ������HitResult���������BoneName�������� Name of bone we hit (for skeletal meshes)
}

void ASTURifleWeapon::InitMuzzleFX()//��ʼ��ǹ����Ч
{
	if (!MuzzleFXComponent)//���ǹ����Ч���������
	{
		MuzzleFXComponent = SpawnMuzzleFX();
	}
	SetMuzzleFXVisibility(true);
}

void ASTURifleWeapon::SetMuzzleFXVisibility(bool Visible)//����ǹ����Ч�ɼ���
{
	if (MuzzleFXComponent)
	{
		MuzzleFXComponent->SetPaused(!Visible);//����ֹͣ���������Ǽ������
		MuzzleFXComponent->SetVisibility(Visible, true);//���ÿɼ��ԣ�����һ�Ƿ�ɼ������������Ƿ񴫵ݸ�����
	}
}

void ASTURifleWeapon::SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd)
{
	const auto TraceFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TraceFX, TraceStart);
	//��ָ��λ���������Ǽ���ϵͳ������һ���������磬��������������Ч�����������������
	if (TraceFXComponent)
	{
		TraceFXComponent->SetNiagaraVariableVec3(TraceTargetName, TraceEnd);//���Ǽ���ϵͳ�е����ɱ��������ĺ���������һ������Ŀ���������ƣ�������������Ŀ������ֵ
		//SetNiagaraVariableVec3������Vec3��Ӧ�����Ǽ���ϵͳ��ͼ���û��������µ�TraceTarget����
	}
}