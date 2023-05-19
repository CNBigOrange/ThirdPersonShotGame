// Shoot Them Up , All Right Reserved


#include "Weapon/STUProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "DrawDebugHelpers.h"//���Ƶ������֣�ע���Ǹ���
#include "Kismet/GameplayStatics.h"
#include "Weapon/Components/STUWeaponFXComponent.h"

// Sets default values
ASTUProjectile::ASTUProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	CollisionComponent->InitSphereRadius(5.0f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//ECollisionEnabled::QueryOnly��ʾ����ѯ��ײ��������������������ECollisionEnabled::QueryAndPhysics��ʾ�Ȳ�ѯ���������������
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	//������ײ��Ӧ������ͨ��������Ҳ�������ò�ͬͨ��������ͬ����ײ��Ӧ
	CollisionComponent->bReturnMaterialOnMove = true;//����ײ��������÷�����ײ�������Ϊtrue
	SetRootComponent(CollisionComponent);//���ø�����

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");//��ʼ�����������
	MovementComponent->InitialSpeed = 2000.0f;//�ٶȴ�С
	MovementComponent->ProjectileGravityScale = 0.5f;//������С

	WeaponFXComponent = CreateDefaultSubobject<USTUWeaponFXComponent>("WeaponFXComponent");//����������Ч���
}

// Called when the game starts or when spawned
void ASTUProjectile::BeginPlay()
{
	Super::BeginPlay();

	check(MovementComponent);//���ָ���Ƿ�Ϊ��
	check(CollisionComponent);
	check(WeaponFXComponent);

	MovementComponent->Velocity = ShotDirection * MovementComponent->InitialSpeed;//�����ٶ� = ShotDirection������λ������ ���Գ�ʶ���ٶȣ�����Velocity��һ��FVector
	CollisionComponent->IgnoreActorWhenMoving(GetOwner(), true);//ͨ���˺������ý�ɫ�ƶ��к��Ե���ײ����
	CollisionComponent->OnComponentHit.AddDynamic(this, &ASTUProjectile::OnProjectileHit);
	//OnComponentHit�Ƕ�̬ί�У�����ײ������Դ���ί�У���Begin�󶨶�̬ί��

	SetLifeSpan(LifeSeconds);//ͨ����ʱ����������ʱ�䣬��ֹ�ӵ�̫�ർ���ڴ������
}

void ASTUProjectile:: OnProjectileHit( UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!GetWorld())return;

	MovementComponent->StopMovementImmediately();

	//make damage
	UGameplayStatics::ApplyRadialDamage(GetWorld(), //��ǰ���米��
		DamageAmount,                               //�˺�ֵ
		GetActorLocation(),                         //λ��
		DamageRadius,                               //�˺��뾶
		UDamageType::StaticClass(),                 //�˺�����
        {GetOwner()},                                         //��������˺��Ķ���
		this,                                       //�˺�������
		GetController(),                            //�˺�������
		DoFullDamage,                               //��ײͨ�����ɼ���
		ECC_Visibility);
	//Ӧ�÷����˺�����������һ����ǰ����ָ�룻���������˺�ֵ����������Actor����λ�ã������ģ��뾶�������壺UDamageTypeClass;�����������ӣ�����ɣ��˺���Actor��{}Ϊ�����飻�����ߣ��˺�������
	// �����ˣ��˺��Ŀ������������ţ��Ƿ���������˺�������ʮ����ײͨ����ö�٣�����ʹ��ɫ�ͱ�ը������֮�������������������˺�

	//DrawDebugSphere(GetWorld(), GetActorLocation(), DamageRadius, 24, FColor::Red, false, 3.0f);

	WeaponFXComponent->PlayImpactFX(Hit);//������Ч
	Destroy();
}


AController* ASTUProjectile::GetController() const
{
	const auto Pawn = Cast<APawn>(GetOwner());//���pawn�Ƿ����
	return Pawn ? Pawn->GetController() : nullptr;//��������򷵻�pawn�Ŀ�����
	//����GetController������ͬ����ͬ�������ò�ͬ
}