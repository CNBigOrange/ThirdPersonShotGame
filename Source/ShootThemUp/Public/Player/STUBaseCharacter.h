// Shoot Them Up , All Right Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "STUBaseCharacter.generated.h"


//组件的前置声明在头文件中，cpp文件中再包含组件的头文件
class UCameraComponent;//提前申明摄像机类
class USpringArmComponent;//提前声明摄像机摇臂类
class USTUHealthComponent;//前置声明健康组件
class UTextRenderComponent;//文字渲染组件
class USTUWeaponComponent;//武器组件

UCLASS()
class SHOOTTHEMUP_API ASTUBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASTUBaseCharacter(const FObjectInitializer& ObjInit);
	//有参构造函数，参数为初始化器,为了显式指定UCharacterMovementComponent的类

	USTUWeaponComponent* GetWeaponComponent() const { return WeaponComponent; }//创建获取protected中数据的接口

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USpringArmComponent* SpringArmComponent;//组件都是用指针

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Health")
	USTUHealthComponent* STUHealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Health")
	UTextRenderComponent* HealthTextComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Component")
	USTUWeaponComponent* WeaponComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* DeathAnimMontage;//动画蒙太奇组件

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float LifeSpanOnDeath = 5.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FVector2D LandedDamageVelocity = FVector2D(900.0f, 1500.0f);//记录落地速度的最大和最小值

	UPROPERTY(EditDefaultsOnly, Category = "Damage")//记录落地伤害的最大和最小值
	FVector2D LandedDamage = FVector2D(10.0f, 100.0f);


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnDeath();//将死亡函数多态，以方便在AI角色时调用
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Movement")//在蓝图中可以编辑的函数的宏
	bool IsRunning() const;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	float GetMovementDirection() const;

	UPROPERTY()
		bool RuningAnimInProgress = false;
private:
	bool WantsToRun = false;
	bool IsMovingForward = false;
	bool IsMovingRight = false;


	void MoveForward(float Amount);
	void MoveRight(float Amount);
	void OnStartRunning();
	void OnStopRunning();

	//UPROPERTY(EditAnywhere)
	//	UStaticMeshComponent* TmpMesh;
	//UPROPERTY(EditAnywhere)
	//	class UCableComponent* Cable;
	//UPROPERTY(EditAnywhere)
	//	class UTimelineComponent* MyTimeline;
	//UPROPERTY(EditAnywhere)
	//	class UCurveFloat* MyCurve;

	void Grapple();

	void OnHealthChanged(float Health);

	UFUNCTION()
	void OnGroundLanded(const FHitResult& Hit);


};