// Shoot Them Up , All Right Reserved


#include "Weapon/STUProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "DrawDebugHelpers.h"//绘制调试助手，注意是复数
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
	//ECollisionEnabled::QueryOnly表示仅查询碰撞结果，不会产生物理交互，ECollisionEnabled::QueryAndPhysics表示既查询结果还发生物理交互
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	//设置碰撞反应对所有通道产生，也可以设置不同通道产生不同的碰撞反应
	CollisionComponent->bReturnMaterialOnMove = true;//在碰撞组件中设置返回碰撞物理材质为true
	SetRootComponent(CollisionComponent);//设置根主件

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");//初始化发射物组件
	MovementComponent->InitialSpeed = 2000.0f;//速度大小
	MovementComponent->ProjectileGravityScale = 0.5f;//重力大小

	WeaponFXComponent = CreateDefaultSubobject<USTUWeaponFXComponent>("WeaponFXComponent");//创建武器特效组件
}

// Called when the game starts or when spawned
void ASTUProjectile::BeginPlay()
{
	Super::BeginPlay();

	check(MovementComponent);//检查指针是否为空
	check(CollisionComponent);
	check(WeaponFXComponent);

	MovementComponent->Velocity = ShotDirection * MovementComponent->InitialSpeed;//设置速度 = ShotDirection（方向单位向量） 乘以初识化速度，这里Velocity是一个FVector
	CollisionComponent->IgnoreActorWhenMoving(GetOwner(), true);//通过此函数设置角色移动中忽略的碰撞对象，
	CollisionComponent->OnComponentHit.AddDynamic(this, &ASTUProjectile::OnProjectileHit);
	//OnComponentHit是动态委托，是碰撞组件内自带的委托，在Begin绑定动态委托

	SetLifeSpan(LifeSeconds);//通过计时器设置声明时间，防止子弹太多导致内存的消耗
}

void ASTUProjectile:: OnProjectileHit( UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!GetWorld())return;

	MovementComponent->StopMovementImmediately();

	//make damage
	UGameplayStatics::ApplyRadialDamage(GetWorld(), //当前世界背景
		DamageAmount,                               //伤害值
		GetActorLocation(),                         //位置
		DamageRadius,                               //伤害半径
		UDamageType::StaticClass(),                 //伤害种类
        {GetOwner()},                                         //忽视造成伤害的对象
		this,                                       //伤害制造者
		GetController(),                            //伤害控制器
		DoFullDamage,                               //碰撞通道：可见的
		ECC_Visibility);
	//应用放射伤害函数，参数一：当前世界指针；参数二：伤害值；参数三：Actor中心位置；参数四：半径；参数五：UDamageTypeClass;参数六：忽视（不造成）伤害的Actor，{}为空数组；参数七：伤害制造者
	// 参数八：伤害的控制器；参数九：是否造成满额伤害；参数十：碰撞通道的枚举，可以使角色和爆炸的震中之间有其他物体而不造成伤害

	//DrawDebugSphere(GetWorld(), GetActorLocation(), DamageRadius, 24, FColor::Red, false, 3.0f);

	WeaponFXComponent->PlayImpactFX(Hit);//播放特效
	Destroy();
}


AController* ASTUProjectile::GetController() const
{
	const auto Pawn = Cast<APawn>(GetOwner());//检查pawn是否存在
	return Pawn ? Pawn->GetController() : nullptr;//如果存在则返回pawn的控制者
	//两个GetController作用域不同，虽同名但作用不同
}