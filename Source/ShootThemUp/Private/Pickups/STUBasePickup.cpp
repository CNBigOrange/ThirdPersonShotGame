// Shoot Them Up , All Right Reserved


#include "Pickups/STUBasePickup.h"
#include "Components/SphereComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogBasePickup, All, All);


// Sets default values
ASTUBasePickup::ASTUBasePickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>("CollisionComponent");
	CollisionComponent->InitSphereRadius(50.0f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	CollisionComponent->SetupAttachment(RootComponent);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	StaticMeshComponent->SetupAttachment(CollisionComponent);
}

// Called when the game starts or when spawned
void ASTUBasePickup::BeginPlay()
{
	Super::BeginPlay();
	
	check(CollisionComponent);

	GenerateRotationTaw();
}

// Called every frame
void ASTUBasePickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorLocalRotation(FRotator(0.0f, RotationYaw, 0.0f));//添加Actor原地旋转
}

void ASTUBasePickup::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	
	UE_LOG(LogBasePickup, Display, TEXT("4"));
	const auto Pawn = Cast<APawn>(OtherActor);
	if (GivePickupTo(Pawn))
	{
		PickupWasTaken();
	}
	//UE_LOG(LogBasePickup, Display, TEXT("Pickup was taken"));
}

bool ASTUBasePickup::GivePickupTo(APawn* PlayerPawn)
{
	return true;
}

void ASTUBasePickup::PickupWasTaken()
{
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);//设置碰撞反应通道为忽视

	if (GetRootComponent())
	{
	
		GetRootComponent()->SetVisibility(false, true);//设置根组件可见性，参数一：是否被渲染，参数二：是否将设置的可见性应用于所有组件
	}

	GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &ASTUBasePickup::Respawn, RespawnTime);
	//设置计时器,执行重新生成，后两个为默认参数bool InbLoop = false, float InFirstDelay = -1.f，不循环、第一次开始延迟1秒
}

void ASTUBasePickup::Respawn()
{
	GenerateRotationTaw();
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);//设置碰撞反应通道为忽视
	if (GetRootComponent())
	{
		GetRootComponent()->SetVisibility(true, true);//设置根组件可见性，参数一：是否被渲染，参数二：是否将设置的可见性应用于所有组件
	}
}

void ASTUBasePickup::GenerateRotationTaw()
{
	const auto Direction = FMath::RandBool() ? 1.0f : -1.0f;//随机bool
	RotationYaw = FMath::RandRange(1.0F, 2.0F) * Direction;//随机取值
}

bool ASTUBasePickup::CouldBeTaken() const//当弹药不可见时，解决AI在弹药不可见时前往弹药的bug
{
	return !GetWorldTimerManager().IsTimerActive(RespawnTimerHandle);
}