// Shoot Them Up , All Right Reserved


#include "Player/STUBaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"//�������ͷ�ļ�
#include "GameFramework/SpringArmComponent.h"
#include "Components/STUCharacterMovementComponent.h"//��ɫ�ƶ��������
#include "Components/STUHealthComponent.h"//�������������ΪActorComponent
#include "Components/TextRenderComponent.h"//�ı���ʾ���
#include "GameFramework/Controller.h"//������ͷ�ļ�
#include "Components/STUWeaponComponent.h"//�������
#include "Components/CapsuleComponent.h"//������ͷ�ļ�
#include "Weapon/STUBaseWeapon.h"

DEFINE_LOG_CATEGORY_STATIC(BaseCharacterLog, All, All);//�����־��

// Sets default values
ASTUBaseCharacter::ASTUBaseCharacter(const FObjectInitializer& ObjInit)
	:Super(ObjInit.SetDefaultSubobjectClass<USTUCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
	//����Ĺ��캯����Ϊ����ʽָ��UCharacterMovementComponent���࣬��USTUCharacterMovementComponent����ΪĬ�ϵ��˶��������
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");//�����ҡ�����
	SpringArmComponent->SetupAttachment(GetRootComponent());//���ø��Ų�λ
	SpringArmComponent->bUsePawnControlRotation = true;//��pawn������ת
	SpringArmComponent->SocketOffset = FVector(0.0f, 0.0f, 0.0f);//�������

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");//��������
	CameraComponent->SetupAttachment(SpringArmComponent);//���ӵ������ҡ��

	STUHealthComponent = CreateDefaultSubobject<USTUHealthComponent>("STUHealthComponent");//�������

	HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("HealthTextComponent");//�������
	HealthTextComponent->SetupAttachment(GetRootComponent());
	HealthTextComponent->SetOwnerNoSee(true);//ӵ���߿�����

	WeaponComponent = CreateDefaultSubobject<USTUWeaponComponent>("WeaponComponent");//�������
}

// Called when the game starts or when spawned
void ASTUBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	check(STUHealthComponent);//��麯�����ڿ����͵���ģʽ�����У��������ģʽ������Ӽ�麯����Ӱ������ٶ�
	check(HealthTextComponent);//�������Ƿ���ڣ������ڻᴥ���ϵ�
	check(GetCharacterMovement());
	check(GetMesh());

	//��ͨί����Bind�󶨣��ಥ���鲥��ί����AddUObject�󶨣���̬�ಥ��AddDynamic��
	STUHealthComponent->OnDeath.AddUObject(this, &ASTUBaseCharacter::OnDeath);//�󶨶ಥί��,��OnDeath.broadcast����ִ��ʱ��ͬʱ���ö���Ѱ�ί�еĺ�����������&ASTUBaseCharacter::OnDeath
	//OnDeathί��ֻ����C++��ʹ�ã���˰�ʹ��AddUObject����
	STUHealthComponent->OnHealthChanged.AddUObject(this, &ASTUBaseCharacter::OnHealthChanged);
	//�ڿ�ʼ��Ϸʱ�󶨽����ı�ί�У���ί�е���ʱ�����ò������еĺ���
	//��������Ϣֻ�ڸı�ʱ�ŵ��ø��ģ��Ż�����
	OnHealthChanged(STUHealthComponent->GetHealth());

	//STUHealthComponent->OnDestroyWeapon.AddUObject(ASTUBaseWeapon*, &ASTUBaseWeapon::OnDestroyWeapon);?

	//Character.h��������DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLandedSignature, const FHitResult&, Hit)��ί�У���ʵ��ΪFLandedSignature LandedDelegate
	//ACharacter.h���麯��virtual void Landed(const FHitResult& Hit)���ڽ�ɫ���ʱ���õĺ���,Ҳ���Ը����麯��ʵ��Ч��
	LandedDelegate.AddDynamic(this, &ASTUBaseCharacter::OnGroundLanded);

}

// Called every frame
void ASTUBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//const auto Health = STUHealthComponent->GetHealth();

	IsMovingForward = false;//ÿһ֡��IsMovingForward��ʼ���������̽�����IsMovingForwardû�б��false��BUG
	IsMovingRight = false;//ÿһ֡��IsMovingRight��ʼ����������ƽ�����IsMovingRightû�б��false��BUG
	//TakeDamage(0.1f, FDamageEvent{}, Controller, this);//ÿ֡����˺�
	//Pawn�е��麯��virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
}



// Called to bind functionality to input
void ASTUBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);//���PlayerInputComponent�Ƿ����
	//check(WeaponComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASTUBaseCharacter::MoveForward);//BindAxis��һ��������ӳ������ƣ��ڶ�������ָ������Ҫ���õĶ����ָ�룬���������ǵ��õĺ���������
	PlayerInputComponent->BindAxis("MoveRight", this, &ASTUBaseCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &ASTUBaseCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("TurnAround", this, &ASTUBaseCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAction("Jump", IE_Pressed,this, &ASTUBaseCharacter::Jump);//Jump���˶�������õĺ���
	//��һ��������ӳ�䶯�����ƣ��ڶ��������ǰ�����Ϣ��ö��Ԫ�ػ���IE_Released,IE_Repeat,IE_DoubleClick,IE_Axis,IE_MAX�������������Ǻ������õĶ����ָ�룬���ĸ������ǵ��õĺ���������
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ASTUBaseCharacter::OnStartRunning);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &ASTUBaseCharacter::OnStopRunning);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponComponent, &USTUWeaponComponent::StartFire);//�󶨿���
	PlayerInputComponent->BindAction("Fire", IE_Released, WeaponComponent, &USTUWeaponComponent::StopFire);//�󶨿���
	PlayerInputComponent->BindAction("NextWeapon", IE_Released, WeaponComponent, &USTUWeaponComponent::NextWeapon);//�л�����
	PlayerInputComponent->BindAction("Reload", IE_Pressed, WeaponComponent, &USTUWeaponComponent::Reload);//װ��
	PlayerInputComponent->BindAction("Grapple", IE_Pressed, this, &ASTUBaseCharacter::Grapple);

}

void ASTUBaseCharacter::MoveForward(float Amount)
{
	if (Amount == 0)return;//�Ż�����
	IsMovingForward = Amount > 0.0f && !IsMovingRight;//�ж��Ƿ���ǰ�ƶ�
	AddMovementInput(GetActorForwardVector(), Amount);//AddMovementInput������ÿ��pawn�ж�����,GetActorForwardVector�ǽ�ɫָ����ǰ��������
    //��һ�����������������򣩳���֮�⻹��GetActorRightVector��GetActorUpVector�ȣ��ڶ�����Ϊ����ֵ��Ϊ1,0��-1����Դ���������ڳ��Բ���һ������ǰ���ֹͣ
	//FString IsMovingForwardStr = FString(IsMovingForward ? "IsMovingForward��true" : "false");
	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, *IsMovingForwardStr, false, FVector2D(3.f, 3.f));
	//}//����Ļ���������
}

void ASTUBaseCharacter::MoveRight(float Amount)
{
	if (Amount == 0)return;//�Ż�����
	if (WantsToRun)Amount = 0;//�������б�ų�̵�BUG
	//IsMovingRight = Amount > 0.0f;
	AddMovementInput(GetActorRightVector(), Amount);//ActorRightVector��ɫ�����Ҳ�����
}

void ASTUBaseCharacter::OnStartRunning()
{
	WantsToRun = true;
	
	RuningAnimInProgress = true;
	//FString WantsToRunStr = FString(WantsToRun ? "WantsToRun��true" : "false");
	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, *WantsToRunStr, false, FVector2D(3.f, 3.f));
	//}//����Ļ���������
}

void ASTUBaseCharacter::OnStopRunning()
{
	WantsToRun = false;

	RuningAnimInProgress = false;
	//FString WantsToRunStr = FString(WantsToRun?"true":"false");
	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, *WantsToRunStr, false, FVector2D(3.f, 3.f));
	//}//����Ļ���������
}



bool ASTUBaseCharacter::IsRunning() const//IsRunning�ڶ�����ͼ��ÿһ֡�������ã���const����ı��������ͣ����ǿ��Է�����ȷ��boolֵ
//��const�ں�����ǰʱ���ε��Ǻ�������ֵ���ں�������ʱ���������޸Ķ����ڵ��κγ�Ա��ֻ�ܶ�������д��
{
	//bool Running = WantsToRun&& IsMovingForward&& GetVelocity().IsZero();
	//FString RunningStr = FString(Running ? "true" : "false");//��FString���캯����ֵ����Ŀ�����ת��bool����Ϊ�ַ�������
	//UE_LOG(LogTemp, Warning, TEXT("%S"), *RunningStr);
	
	return WantsToRun && IsMovingForward && !GetVelocity().IsZero();//&&����������һ���ж�Ϊfalse�����Զ���ֹ����ִ�еڶ����жϣ����԰���˳���ж��Ƿ��ܣ����жϷ�����ٶ�

	//�ж��Ƿ��̵�������������Ҫ���ܣ�������ǰ�ƶ����ٶȲ�Ϊ0
	//WantsToRun���Ƶ�����������ݣ���IsMovingForWard��!GetVelocity().IsZero()���Ƶı��ܵ�ǰ������
	//IsRunning����Shift��ǰ������ͬ����
}

float ASTUBaseCharacter::GetMovementDirection() const//��÷�����������ƥ���ά��Ͽռ��е�Directionֵ
{
	if (GetVelocity().IsZero())return 0.0f;//����ٶ�����Ϊ0���򷵻�0.0���Ż����룬�����ڴ�����
	const auto VelocityNormal = GetVelocity().GetSafeNormal();//�����ٶȵĹ�һʸ��
	const auto ForwardVector = GetActorForwardVector();//��ɫ��ǰ��ʸ����Ĭ�Ϲ�һ��
	const auto Angle = FMath::Acos(FVector::DotProduct(VelocityNormal, ForwardVector));
	//DotProduct����������������Ƕȵ�����ֵcos��Acos�����Һ�������cos�Ƕȶ�Ӧ�Ļ���ֵRadians
	const auto CrossProduct = FVector::CrossProduct( ForwardVector, VelocityNormal);//���ַ���FowardVectorΪʳָ��VelocityNormalΪ��ָ
	//CrossProduct�����������������һ������,�������������Ĳ���������������ʹ�ֱ��A��B��B��A������ͬ,A��B=-B��A
	//������ڸ�ֵ�����Դ�ֱ�ķ���������ϻ����£����巽����ݡ����ַ����жϡ�
	const auto Degrees = FMath::RadiansToDegrees(Angle);//����Ƕ�Ϊ0���򷵻�0
	return CrossProduct.IsZero() ? Degrees : Degrees * FMath::Sign(CrossProduct.Z);//��Ŀ������Ż����룬�������Ϊ0�����������
	//RadiansToDegrees������תΪ�Ƕȣ�Sign������ȡ���ݵķ��ţ�����0����1������0����0��С��0����-1��
	//Sign����Դ���룺��Ŀ�����Ƕ����Ŀ�����  return (A > (T)0) ? (T)1 : ((A < (T)0) ? (T)-1 : (T)0);
}

void ASTUBaseCharacter::OnDeath()
{
	UE_LOG(BaseCharacterLog, Display, TEXT("Player %s is dead"), *GetName());

	//PlayAnimMontage(DeathAnimMontage);//���Ŷ�����̫��

	SetLifeSpan(5.0f);//��������ʱ��

	//DisableInput(Cast<APlayerController>(this));//��ֹ����

	GetCharacterMovement()->DisableMovement();//�ر��˶����

	if (Controller)//�������������
	{
		Controller->ChangeState(NAME_Spectating);//��������״̬,����������ƶ�������۲���ģʽ
	}
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);//�����ɫ������������Ȼ���ڵ�Bug
	//ECollisionResponse ��ECR)��ײ��Ӧ  ����һ��ö��ECollisionChannel ��ECC����ײͨ�� 

	WeaponComponent->StopFire();

	//ģ������ʱ������Ч����
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);//����������ײΪ��ѯ������
	GetMesh()->SetSimulatePhysics(true);//����Ϊģ������

}

void ASTUBaseCharacter::Grapple()
{
}

void ASTUBaseCharacter::OnHealthChanged(float Health)
{
	HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Health)));
	//���ı���������ı���FText������FStringת����������FString��Printf������floatת������
}

void ASTUBaseCharacter::OnGroundLanded(const FHitResult& Hit)//FHitResult�ṹ��������˷��ߡ��Ӵ����
{
	
	const auto FallVelocityZ = GetVelocity().Z;//Z����ٶȣ�����߶�Խ�ߣ��ٶ�Խ��
	UE_LOG(BaseCharacterLog, Display, TEXT("On landed: %f"), FallVelocityZ);

	if (-FallVelocityZ < LandedDamageVelocity.X)return;
	//FallVelocityZΪ���·�������Ϊ����Ҫ*-1��LandedDamageVelocity.XΪ�˺���С�ٶ�

	const auto FinalDamage = FMath::GetMappedRangeValueClamped(LandedDamageVelocity, LandedDamage, -FallVelocityZ);
	//GetMappedRangeValueClamped���һ������ֵ��ӳ�䣬����һ�����뷶Χ���������������Χ��������������ֵ����󷵻����ֵ�������ٶȶ�Ӧ�ı���ѡ����ɵ��˺�
	UE_LOG(BaseCharacterLog, Display, TEXT("FinalDamage: %f"), FinalDamage);
	TakeDamage(FinalDamage, FDamageEvent{}, nullptr, nullptr);//TakeDamage()���װ���˺�ί�еĵ���
	//TakeDamage����һ���˺�ֵ�����������˺��¼������ﴫ��һ���յĽṹ�壻���������¼������ߣ������ģ��ƻ�������
}



