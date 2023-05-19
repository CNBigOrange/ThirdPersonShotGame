// Shoot Them Up , All Right Reserved


#include "Dev/STUDevDamageActor.h"
#include "DrawDebugHelpers.h"//该头文件绘制图形基元的功能
#include "Kismet/GameplayStatics.h"//包含各类世界静态函数

// Sets default values
ASTUDevDamageActor::ASTUDevDamageActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	SetRootComponent(SceneComponent);
}

// Called when the game starts or when spawned
void ASTUDevDamageActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASTUDevDamageActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DrawDebugSphere(GetWorld(), GetActorLocation(), Radius, 24, SphereColor);//绘制调试球体,在测试模式下显示，在发行模式中将不会显示
	//参数一：当前世界指针；参数二：圆心位置；参数三：半径；参数四：图形分段数；参数五：图形颜色
	UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage, GetActorLocation(), Radius, DamageType, {}, this, nullptr, DoFullDamage, ECC_Visibility);
	//应用放射伤害函数，参数一：当前世界指针；参数二：伤害值；参数三：Actor中心位置；参数四：半径；参数五：UDamageTypeClass;参数六：忽视（不造成）伤害的Actor，{}为空数组；参数七：伤害制造者
	// 参数八：伤害的控制器；参数九：是否造成满额伤害；参数十：碰撞通道的枚举，可以使角色和爆炸的震中之间有其他物体而不造成伤害
	//还有 应用点伤害函数ApplyRadialDamage()
}

