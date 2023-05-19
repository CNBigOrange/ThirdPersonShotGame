// Shoot Them Up , All Right Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUProjectile.generated.h"//投射物头文件

class USphereComponent;//头文件中前置声明，cpp中包含组件头文件
class UProjectileMovementComponent;//发射物组件前置声明
class USTUWeaponFXComponent;//武器特效组件

UCLASS()
class SHOOTTHEMUP_API ASTUProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASTUProjectile();

	void SetShotDirection(const FVector& Direction) { ShotDirection = Direction; }

protected:
	
	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
		USphereComponent* CollisionComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		UProjectileMovementComponent* MovementComponent;//发射物组件

	UPROPERTY(VisibleAnywhere, Category = "VFX")
		USTUWeaponFXComponent* WeaponFXComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		float DamageRadius = 200.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		float DamageAmount = 50.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		bool DoFullDamage = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		float LifeSeconds = 5.0f;

	virtual void BeginPlay() override;

public:	
	// Called every frame
	
private:
	FVector ShotDirection;

	UFUNCTION(BlueprintCallable)
		void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit );
	//委托调用的函数

	AController* GetController() const;
};
