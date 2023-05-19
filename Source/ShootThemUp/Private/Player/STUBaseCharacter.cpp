// Shoot Them Up , All Right Reserved


#include "Player/STUBaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"//控制组件头文件
#include "GameFramework/SpringArmComponent.h"
#include "Components/STUCharacterMovementComponent.h"//角色移动子类组件
#include "Components/STUHealthComponent.h"//健康组件，父类为ActorComponent
#include "Components/TextRenderComponent.h"//文本显示组件
#include "GameFramework/Controller.h"//控制器头文件
#include "Components/STUWeaponComponent.h"//武器组件
#include "Components/CapsuleComponent.h"//胶囊体头文件
#include "Weapon/STUBaseWeapon.h"

DEFINE_LOG_CATEGORY_STATIC(BaseCharacterLog, All, All);//虚幻日志宏

// Sets default values
ASTUBaseCharacter::ASTUBaseCharacter(const FObjectInitializer& ObjInit)
	:Super(ObjInit.SetDefaultSubobjectClass<USTUCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
	//特殊的构造函数，为了显式指定UCharacterMovementComponent的类，将USTUCharacterMovementComponent设置为默认的运动组件子类
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");//摄像机摇臂组件
	SpringArmComponent->SetupAttachment(GetRootComponent());//设置附着部位
	SpringArmComponent->bUsePawnControlRotation = true;//用pawn控制旋转
	SpringArmComponent->SocketOffset = FVector(0.0f, 0.0f, 0.0f);//插槽设置

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");//摄像机组件
	CameraComponent->SetupAttachment(SpringArmComponent);//连接到摄像机摇臂

	STUHealthComponent = CreateDefaultSubobject<USTUHealthComponent>("STUHealthComponent");//健康组件

	HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("HealthTextComponent");//文字组件
	HealthTextComponent->SetupAttachment(GetRootComponent());
	HealthTextComponent->SetOwnerNoSee(true);//拥有者看不到

	WeaponComponent = CreateDefaultSubobject<USTUWeaponComponent>("WeaponComponent");//武器组件
}

// Called when the game starts or when spawned
void ASTUBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	check(STUHealthComponent);//检查函数仅在开发和调试模式中运行，如果发行模式将会忽视检查函数不影响程序速度
	check(HealthTextComponent);//检查组件是否存在，不存在会触发断点
	check(GetCharacterMovement());
	check(GetMesh());

	//普通委托用Bind绑定，多播（组播）委托用AddUObject绑定，动态多播用AddDynamic绑定
	STUHealthComponent->OnDeath.AddUObject(this, &ASTUBaseCharacter::OnDeath);//绑定多播委托,当OnDeath.broadcast（）执行时，同时调用多个已绑定委托的函数，这里是&ASTUBaseCharacter::OnDeath
	//OnDeath委托只能在C++中使用，因此绑定使用AddUObject函数
	STUHealthComponent->OnHealthChanged.AddUObject(this, &ASTUBaseCharacter::OnHealthChanged);
	//在开始游戏时绑定健康改变委托，在委托调用时，调用参数二中的函数
	//将健康信息只在改变时才调用更改，优化性能
	OnHealthChanged(STUHealthComponent->GetHealth());

	//STUHealthComponent->OnDestroyWeapon.AddUObject(ASTUBaseWeapon*, &ASTUBaseWeapon::OnDestroyWeapon);?

	//Character.h里声明了DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLandedSignature, const FHitResult&, Hit)的委托，其实例为FLandedSignature LandedDelegate
	//ACharacter.h里虚函数virtual void Landed(const FHitResult& Hit)是在角色落地时调用的函数,也可以覆盖虚函数实现效果
	LandedDelegate.AddDynamic(this, &ASTUBaseCharacter::OnGroundLanded);

}

// Called every frame
void ASTUBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//const auto Health = STUHealthComponent->GetHealth();

	IsMovingForward = false;//每一帧将IsMovingForward初始化，解决冲刺结束后IsMovingForward没有变回false的BUG
	IsMovingRight = false;//每一帧将IsMovingRight初始化，解决侧移结束后IsMovingRight没有变回false的BUG
	//TakeDamage(0.1f, FDamageEvent{}, Controller, this);//每帧造成伤害
	//Pawn中的虚函数virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
}



// Called to bind functionality to input
void ASTUBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);//检查PlayerInputComponent是否存在
	//check(WeaponComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASTUBaseCharacter::MoveForward);//BindAxis第一个参数是映射的名称，第二参数是指向函数将要调用的对象的指针，第三参数是调用的函数的引用
	PlayerInputComponent->BindAxis("MoveRight", this, &ASTUBaseCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &ASTUBaseCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("TurnAround", this, &ASTUBaseCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAction("Jump", IE_Pressed,this, &ASTUBaseCharacter::Jump);//Jump是运动组件内置的函数
	//第一个参数：映射动作名称；第二个参数是按键信息的枚举元素还有IE_Released,IE_Repeat,IE_DoubleClick,IE_Axis,IE_MAX；第三个参数是函数调用的对象的指针，第四个参数是调用的函数的引用
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ASTUBaseCharacter::OnStartRunning);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &ASTUBaseCharacter::OnStopRunning);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponComponent, &USTUWeaponComponent::StartFire);//绑定开火
	PlayerInputComponent->BindAction("Fire", IE_Released, WeaponComponent, &USTUWeaponComponent::StopFire);//绑定开火
	PlayerInputComponent->BindAction("NextWeapon", IE_Released, WeaponComponent, &USTUWeaponComponent::NextWeapon);//切换武器
	PlayerInputComponent->BindAction("Reload", IE_Pressed, WeaponComponent, &USTUWeaponComponent::Reload);//装弹
	PlayerInputComponent->BindAction("Grapple", IE_Pressed, this, &ASTUBaseCharacter::Grapple);

}

void ASTUBaseCharacter::MoveForward(float Amount)
{
	if (Amount == 0)return;//优化代码
	IsMovingForward = Amount > 0.0f && !IsMovingRight;//判断是否向前移动
	AddMovementInput(GetActorForwardVector(), Amount);//AddMovementInput函数在每个pawn中都存在,GetActorForwardVector是角色指向正前方的向量
    //第一个参数是向量（朝向）除此之外还有GetActorRightVector和GetActorUpVector等，第二参数为缩放值（为1,0或-1），源代码中用于乘以参数一，控制前后和停止
	//FString IsMovingForwardStr = FString(IsMovingForward ? "IsMovingForward：true" : "false");
	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, *IsMovingForwardStr, false, FVector2D(3.f, 3.f));
	//}//在屏幕上输出文字
}

void ASTUBaseCharacter::MoveRight(float Amount)
{
	if (Amount == 0)return;//优化代码
	if (WantsToRun)Amount = 0;//解决可以斜着冲刺的BUG
	//IsMovingRight = Amount > 0.0f;
	AddMovementInput(GetActorRightVector(), Amount);//ActorRightVector角色的正右侧向量
}

void ASTUBaseCharacter::OnStartRunning()
{
	WantsToRun = true;
	
	RuningAnimInProgress = true;
	//FString WantsToRunStr = FString(WantsToRun ? "WantsToRun：true" : "false");
	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, *WantsToRunStr, false, FVector2D(3.f, 3.f));
	//}//在屏幕上输出文字
}

void ASTUBaseCharacter::OnStopRunning()
{
	WantsToRun = false;

	RuningAnimInProgress = false;
	//FString WantsToRunStr = FString(WantsToRun?"true":"false");
	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, *WantsToRunStr, false, FVector2D(3.f, 3.f));
	//}//在屏幕上输出文字
}



bool ASTUBaseCharacter::IsRunning() const//IsRunning在动画蓝图中每一帧都被调用，加const不会改变数据类型，但是可以返回正确的bool值
//当const在函数名前时修饰的是函数返回值，在函数名后时函数不能修改对象内的任何成员，只能读，不能写。
{
	//bool Running = WantsToRun&& IsMovingForward&& GetVelocity().IsZero();
	//FString RunningStr = FString(Running ? "true" : "false");//用FString构造函数赋值，三目运算符转换bool类型为字符串类型
	//UE_LOG(LogTemp, Warning, TEXT("%S"), *RunningStr);
	
	return WantsToRun && IsMovingForward && !GetVelocity().IsZero();//&&运算符如果第一个判断为false将会自动终止，不执行第二个判断，所以按照顺序判断是否跑，再判断方向和速度

	//判断是否本盘的三个条件：想要奔跑；正在向前移动；速度不为0
	//WantsToRun控制的是输入的数据，而IsMovingForWard和!GetVelocity().IsZero()控制的奔跑的前提条件
	//IsRunning是由Shift和前进键共同控制
}

float ASTUBaseCharacter::GetMovementDirection() const//获得方向函数，用以匹配二维混合空间中的Direction值
{
	if (GetVelocity().IsZero())return 0.0f;//如果速度向量为0，则返回0.0，优化代码，减少内存消耗
	const auto VelocityNormal = GetVelocity().GetSafeNormal();//创建速度的归一矢量
	const auto ForwardVector = GetActorForwardVector();//角色向前的矢量（默认归一）
	const auto Angle = FMath::Acos(FVector::DotProduct(VelocityNormal, ForwardVector));
	//DotProduct点积：求两个向量角度的余弦值cos；Acos反余弦函数，求cos角度对应的弧度值Radians
	const auto CrossProduct = FVector::CrossProduct( ForwardVector, VelocityNormal);//右手法则FowardVector为食指，VelocityNormal为中指
	//CrossProduct叉积：它的运算结果是一个向量,并且两个向量的叉积与这两个向量和垂直。A×B和B×A并不等同,A×B=-B×A
	//叉积存在负值，所以垂直的方向可能向上或向下，具体方向根据“右手法则”判断。
	const auto Degrees = FMath::RadiansToDegrees(Angle);//如果角度为0，则返回0
	return CrossProduct.IsZero() ? Degrees : Degrees * FMath::Sign(CrossProduct.Z);//三目运算符优化代码，向量叉积为0则两向量相等
	//RadiansToDegrees：弧度转为角度；Sign函数：取数据的符号（大于0返回1、等于0返回0、小于0返回-1）
	//Sign函数源代码：三目运算符嵌套三目运算符  return (A > (T)0) ? (T)1 : ((A < (T)0) ? (T)-1 : (T)0);
}

void ASTUBaseCharacter::OnDeath()
{
	UE_LOG(BaseCharacterLog, Display, TEXT("Player %s is dead"), *GetName());

	//PlayAnimMontage(DeathAnimMontage);//播放动画蒙太奇

	SetLifeSpan(5.0f);//设置生命时间

	//DisableInput(Cast<APlayerController>(this));//禁止输入

	GetCharacterMovement()->DisableMovement();//关闭运动组件

	if (Controller)//如果控制器存在
	{
		Controller->ChangeState(NAME_Spectating);//更换控制状态,死亡后进入移动摄像机观察者模式
	}
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);//解决角色死亡后胶囊体任然存在的Bug
	//ECollisionResponse （ECR)碰撞反应  还有一个枚举ECollisionChannel （ECC）碰撞通道 

	WeaponComponent->StopFire();

	//模拟死亡时的物理效果：
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);//激活物理碰撞为查询和物理
	GetMesh()->SetSimulatePhysics(true);//设置为模拟物理

}

void ASTUBaseCharacter::Grapple()
{
}

void ASTUBaseCharacter::OnHealthChanged(float Health)
{
	HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Health)));
	//用文本组件设置文本，FText类型由FString转换而来，而FString由Printf函数从float转换而来
}

void ASTUBaseCharacter::OnGroundLanded(const FHitResult& Hit)//FHitResult结构体里包含了法线、接触点等
{
	
	const auto FallVelocityZ = GetVelocity().Z;//Z轴的速度，掉落高度越高，速度越大
	UE_LOG(BaseCharacterLog, Display, TEXT("On landed: %f"), FallVelocityZ);

	if (-FallVelocityZ < LandedDamageVelocity.X)return;
	//FallVelocityZ为向下方向所以为负需要*-1，LandedDamageVelocity.X为伤害最小速度

	const auto FinalDamage = FMath::GetMappedRangeValueClamped(LandedDamageVelocity, LandedDamage, -FallVelocityZ);
	//GetMappedRangeValueClamped获得一个区间值的映射，参数一：输入范围；参数二：输出范围；参数三：输入值，最后返回输出值，根据速度对应的比例选择造成的伤害
	UE_LOG(BaseCharacterLog, Display, TEXT("FinalDamage: %f"), FinalDamage);
	TakeDamage(FinalDamage, FDamageEvent{}, nullptr, nullptr);//TakeDamage()里封装了伤害委托的调用
	//TakeDamage参数一：伤害值；参数二：伤害事件，这里传入一个空的结构体；参数三：事件发动者；参数四：破坏制造者
}



