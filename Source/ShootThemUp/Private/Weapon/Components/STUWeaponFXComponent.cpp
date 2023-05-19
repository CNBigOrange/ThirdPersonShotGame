// Shoot Them Up , All Right Reserved


#include "Weapon/Components/STUWeaponFXComponent.h"
#include "NiagaraFunctionLibrary.h"//���Ǽ�����ͷ�ļ�
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Kismet/GameplayStatics.h"//��Ϸ��̬������
#include "Components/DecalComponent.h"//�������

USTUWeaponFXComponent::USTUWeaponFXComponent()
{

	PrimaryComponentTick.bCanEverTick = false;

}

void USTUWeaponFXComponent::PlayImpactFX(const FHitResult& Hit)//����Ϊ���н��
{
	auto ImpactData = DefaultImpactData;

	if (Hit.PhysMaterial.IsValid())//�����������ָ�������Ҫ��IsValid�����Ч��
	{
		const auto PhysMat = Hit.PhysMaterial.Get();//��Get������ȡ�������ԭʼָ�룬���в�λ��������ʷ��ظ�����

		if (ImpactDataMap.Contains(PhysMat))//��Contains�����������ͼ�ṹ���Ƿ���ڲ������ļ�
		{
			ImpactData = ImpactDataMap[PhysMat];//������еĶ���������������Ĭ�ϲ��ʸ���Ϊ��ǰ������ʣ�����[Key]��ȡvalue
		}
	}

	//niagara
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), //
	ImpactData.NiagaraEffect,//
	Hit.ImpactPoint,//
	Hit.ImpactNormal.Rotation());//�������Ǽ���ϵͳ�ĺ��� ����һ�����米�����󣬲����������Ǽ���ϵͳʵ��,������������λ�ã������ģ����ӷ��߷����������ΪĬ��ֵ���������Զ����ٲ���Ϊtrue

	//decal
	//��Ϸ��ܾ�̬�����е�������������������һ�����������ָ��
	auto DecalComponent = UGameplayStatics::SpawnDecalAtLocation(GetWorld(),//��������
	ImpactData.DecalData.Material,//���ʽӿ�
	ImpactData.DecalData.Size,//������С
	Hit.ImpactPoint,//����λ��
	Hit.ImpactNormal.Rotation());//���з��߷���

	if (DecalComponent)
	{
		DecalComponent->SetFadeOut(ImpactData.DecalData.LifeTime, ImpactData.DecalData.FadeOutTime);//����ʧ����������һ����ʼ�ӳ�ʱ�䣬������������ʱ�䣬����������ʧ������Actor��Ĭ��Ϊtrue
	}
	//DecalComponent->SetLifeSpan(ImpactData.DecalData.LifeTime);
}

