// Shoot Them Up , All Right Reserved


#include "Dev/STUDevDamageActor.h"
#include "DrawDebugHelpers.h"//��ͷ�ļ�����ͼ�λ�Ԫ�Ĺ���
#include "Kismet/GameplayStatics.h"//�����������羲̬����

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

	DrawDebugSphere(GetWorld(), GetActorLocation(), Radius, 24, SphereColor);//���Ƶ�������,�ڲ���ģʽ����ʾ���ڷ���ģʽ�н�������ʾ
	//����һ����ǰ����ָ�룻��������Բ��λ�ã����������뾶�������ģ�ͼ�ηֶ����������壺ͼ����ɫ
	UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage, GetActorLocation(), Radius, DamageType, {}, this, nullptr, DoFullDamage, ECC_Visibility);
	//Ӧ�÷����˺�����������һ����ǰ����ָ�룻���������˺�ֵ����������Actor����λ�ã������ģ��뾶�������壺UDamageTypeClass;�����������ӣ�����ɣ��˺���Actor��{}Ϊ�����飻�����ߣ��˺�������
	// �����ˣ��˺��Ŀ������������ţ��Ƿ���������˺�������ʮ����ײͨ����ö�٣�����ʹ��ɫ�ͱ�ը������֮�������������������˺�
	//���� Ӧ�õ��˺�����ApplyRadialDamage()
}

