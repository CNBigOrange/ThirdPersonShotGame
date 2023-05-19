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

	AddActorLocalRotation(FRotator(0.0f, RotationYaw, 0.0f));//���Actorԭ����ת
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
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);//������ײ��Ӧͨ��Ϊ����

	if (GetRootComponent())
	{
	
		GetRootComponent()->SetVisibility(false, true);//���ø�����ɼ��ԣ�����һ���Ƿ���Ⱦ�����������Ƿ����õĿɼ���Ӧ�����������
	}

	GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &ASTUBasePickup::Respawn, RespawnTime);
	//���ü�ʱ��,ִ���������ɣ�������ΪĬ�ϲ���bool InbLoop = false, float InFirstDelay = -1.f����ѭ������һ�ο�ʼ�ӳ�1��
}

void ASTUBasePickup::Respawn()
{
	GenerateRotationTaw();
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);//������ײ��Ӧͨ��Ϊ����
	if (GetRootComponent())
	{
		GetRootComponent()->SetVisibility(true, true);//���ø�����ɼ��ԣ�����һ���Ƿ���Ⱦ�����������Ƿ����õĿɼ���Ӧ�����������
	}
}

void ASTUBasePickup::GenerateRotationTaw()
{
	const auto Direction = FMath::RandBool() ? 1.0f : -1.0f;//���bool
	RotationYaw = FMath::RandRange(1.0F, 2.0F) * Direction;//���ȡֵ
}

bool ASTUBasePickup::CouldBeTaken() const//����ҩ���ɼ�ʱ�����AI�ڵ�ҩ���ɼ�ʱǰ����ҩ��bug
{
	return !GetWorldTimerManager().IsTimerActive(RespawnTimerHandle);
}