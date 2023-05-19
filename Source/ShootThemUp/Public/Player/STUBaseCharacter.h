// Shoot Them Up , All Right Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "STUBaseCharacter.generated.h"


//�����ǰ��������ͷ�ļ��У�cpp�ļ����ٰ��������ͷ�ļ�
class UCameraComponent;//��ǰ�����������
class USpringArmComponent;//��ǰ���������ҡ����
class USTUHealthComponent;//ǰ�������������
class UTextRenderComponent;//������Ⱦ���
class USTUWeaponComponent;//�������

UCLASS()
class SHOOTTHEMUP_API ASTUBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASTUBaseCharacter(const FObjectInitializer& ObjInit);
	//�вι��캯��������Ϊ��ʼ����,Ϊ����ʽָ��UCharacterMovementComponent����

	USTUWeaponComponent* GetWeaponComponent() const { return WeaponComponent; }//������ȡprotected�����ݵĽӿ�

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USpringArmComponent* SpringArmComponent;//���������ָ��

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Health")
	USTUHealthComponent* STUHealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Health")
	UTextRenderComponent* HealthTextComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Component")
	USTUWeaponComponent* WeaponComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* DeathAnimMontage;//������̫�����

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float LifeSpanOnDeath = 5.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FVector2D LandedDamageVelocity = FVector2D(900.0f, 1500.0f);//��¼����ٶȵ�������Сֵ

	UPROPERTY(EditDefaultsOnly, Category = "Damage")//��¼����˺���������Сֵ
	FVector2D LandedDamage = FVector2D(10.0f, 100.0f);


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnDeath();//������������̬���Է�����AI��ɫʱ����
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Movement")//����ͼ�п��Ա༭�ĺ����ĺ�
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