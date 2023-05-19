// Shoot Them Up , All Right Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUBasePickup.generated.h"

class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUBasePickup : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ASTUBasePickup();

protected:
	
	virtual void BeginPlay() override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;//Actor自带的虚函数，在Actor重叠时会调用

	UPROPERTY(VisibleAnywhere,Category = "Pickup")
	USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, Category = "Pickup")
		UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
		float RespawnTime = 5.0f;

public:	
	
	virtual void Tick(float DeltaTime) override;

	bool CouldBeTaken() const;
private:
	float RotationYaw = 0.0f;
	FTimerHandle RespawnTimerHandle;

	virtual bool GivePickupTo(APawn* PlayerPawn);//基类中虚函数，传入一个参数指针用以获得相应的武器、健康组件

	void PickupWasTaken();
	void Respawn();
	void GenerateRotationTaw();
};
