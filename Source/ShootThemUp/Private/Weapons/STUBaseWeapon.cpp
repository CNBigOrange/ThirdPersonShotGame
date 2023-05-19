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
#include "NiagaraFunctionLibrary.h"//���Ǽ���������
#include "NiagaraComponent.h"//���Ǽ������
#include "AIController.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseWeapon, All, All);

ASTUBaseWeapon::ASTUBaseWeapon()
{

	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	WeaponMesh->SetupAttachment(GetRootComponent());//���ø���

}

void ASTUBaseWeapon::BeginPlay()
{
	Super::BeginPlay();

	check(WeaponMesh);
	checkf(DefaultAmmo.Bullets > 0, TEXT("Bullets count couldn't be less or equal zero"));//����ӵ�����С�ڵ���0
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
	const auto Player = Cast<ACharacter>(GetOwner());//Actor*ת��ΪACharacter,GetOwner()��ȡ���������ת��ΪACharacter
	if (!Player)return nullptr;
	//UE_LOG(LogBaseWeapon, Display, TEXT("Player IsValid"));
	return Player->GetController<APlayerController>();//ģ�庯������װ������ת��{return Cast<T>(GetController());}
	//��ȡ��������ת��ΪAPlayerController��ACharacter�ڰ���APlayerController
}

bool ASTUBaseWeapon::GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const
{
	//UE_LOG(LogBaseWeapon, Display, TEXT("GetPlayerController begin"));
	const auto Controller = GetPlayerController();
	if (!Controller)//�����ҿ����������ڣ������ΪAI��ɫ�����Ի�ȡAIController
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
	Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);//�����ҿ������ӵ��λ�ú���ת����ֵ��ViewLocation��ViewRotation
	return true;
}

FVector ASTUBaseWeapon::GetMuzzleWorldLocation() const//��ȡ���λ�ò�����
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
	//const FTransform SocketTransform = WeaponMesh->GetSocketTransform(MuzzleSocketName);//FName���ô�������FString��Ҫ������
	//��ò��λ�ñ任��Ϣ
	TraceStart = ViewLocation;//���λ������

	const FVector ShootDirection = ViewRotation.Vector();// SocketTransform.GetRotation().GetForwardVector();
	//FMath::VRandCone()
	TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
	return true;
}

void ASTUBaseWeapon::MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd,FHitResult& CheckHitResult,const FVector& CheckTraceStart, const FVector& CheckTraceEnd)//˫���߼��
{
	if (!GetWorld())return;
	FCollisionQueryParams CollisionParams;//��������ײ��ѯ�ĸ������
	CollisionParams.AddIgnoredActor(GetOwner());//Ϊ��ѯ������Ӻ��Ӷ��󣬽��ĳЩ�����������ӵ㹥������bug
	CollisionParams.bReturnPhysicalMaterial = true;//�Ƿ񷵻��������Ϊtrue��


	GetWorld()->LineTraceSingleByChannel(CheckHitResult, CheckTraceStart, CheckTraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);


	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);//��HitResult��ֵ
	//��ͨ����׷�٣�����һ��׷�����н������������׷����㣻��������׷���յ㣻�����ģ�׷����ײͨ��ECollisionChannel��ECC_Visibility,ECC_Camera,ECC_WorldStatic������ײͨ��
}

void ASTUBaseWeapon::DecreaseAmmo()
{
	if (CurrentAmmo.Bullets == 0)return;

	CurrentAmmo.Bullets--;
	//LogAmmo();

	if (IsClipEmpty() && !IsAmmoEmpty())//���е�ҩΪ���������ⲻΪ��
	{
		OnClipEmpty.Broadcast(this);//����ί��,����ָ��ǰ����
		//const auto Player = Cast<ASTUBaseCharacter>(GetOwner());//GetOwner���ӵ���ߣ�Ȼ��ת��Ϊ��Ӧ����
		//Player->GetWeaponComponent()->Reload();//GetWeaponComponentΪ��ɫ���Զ���Ľӿں���
		//ChangeClip();
	}
}

bool ASTUBaseWeapon::IsAmmoEmpty() const
{
	return !CurrentAmmo.Infinite && CurrentAmmo.Clips == 0 && IsClipEmpty();
}

bool ASTUBaseWeapon::IsClipEmpty() const//����Ϊ��
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
	if (CurrentAmmo.Clips == 0)return;//������Ϊ0�޷�����
	StopFire();//��������Ҫ�����麯��ֹͣ���������м�ʱ����������������Ҫ�ڻ�����ʱ�����ʱ�����������ǹ����δ��ͬʱ�����ͻ����е�bug
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
	AmmoInfo += CurrentAmmo.Infinite ? "Infinite" : FString::FromInt(CurrentAmmo.Clips);//�����ӵ����ߵ�������
	UE_LOG(LogBaseWeapon,Display, TEXT("%s"), *AmmoInfo);
}

bool ASTUBaseWeapon::IsAmmoFull() const
{
	return DefaultAmmo.Clips == CurrentAmmo.Clips && //����ΪĬ��ֵ
		DefaultAmmo.Bullets == CurrentAmmo.Bullets;//�ӵ�ΪĬ��ֵ
}

bool ASTUBaseWeapon::TryToAddAmmo(int32 ClipsAmount)//�񵯼��߼�
{

	if (CurrentAmmo.Infinite || IsAmmoFull() || ClipsAmount <= 0)return false;

	if (IsAmmoEmpty())
	{
		UE_LOG(LogBaseWeapon, Display, TEXT("Ammo was empty!"));
		CurrentAmmo.Clips = FMath::Clamp(ClipsAmount, 0, DefaultAmmo.Clips + 1);//Clamp��������ȡֵ��Χ��Сֵ����������ȡֵ��Χ���ֵ��+1��Ϊ�˽�һ���ӵ������ҵ�������
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
	//��������ϵͳ����
	return UNiagaraFunctionLibrary::SpawnSystemAttached(MuzzleFX,//���Ǽ���ϵͳʵ��
												 WeaponMesh,//����������
												 MuzzleSocketName,//ǹ�ڲ������
												 FVector::ZeroVector,//λ��
												 FRotator::ZeroRotator,//����
												 EAttachLocation::SnapToTarget,//���뵽Ŀ��
												 true);//�Զ�����
}