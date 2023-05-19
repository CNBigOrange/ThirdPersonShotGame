// Shoot Them Up , All Right Reserved


#include "Weapon/Components/STUWeaponFXComponent.h"
#include "NiagaraFunctionLibrary.h"//尼亚加拉的头文件
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Kismet/GameplayStatics.h"//游戏静态函数库
#include "Components/DecalComponent.h"//贴花组件

USTUWeaponFXComponent::USTUWeaponFXComponent()
{

	PrimaryComponentTick.bCanEverTick = false;

}

void USTUWeaponFXComponent::PlayImpactFX(const FHitResult& Hit)//参数为命中结果
{
	auto ImpactData = DefaultImpactData;

	if (Hit.PhysMaterial.IsValid())//物理材质是弱指针对象，需要用IsValid检查有效性
	{
		const auto PhysMat = Hit.PhysMaterial.Get();//用Get函数获取物理材质原始指针，命中部位的物理材质返回给我们

		if (ImpactDataMap.Contains(PhysMat))//用Contains（）函数检查图结构中是否存在参数名的键
		{
			ImpactData = ImpactDataMap[PhysMat];//如果命中的对象存在物理材质则将默认材质覆盖为当前物理材质，利用[Key]获取value
		}
	}

	//niagara
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), //
	ImpactData.NiagaraEffect,//
	Hit.ImpactPoint,//
	Hit.ImpactNormal.Rotation());//生成尼亚加拉系统的函数 参数一：世界背景对象，参数二：尼亚加拉系统实例,参数三：生成位置，参数四：粒子法线方向，其余参数为默认值，其中有自动销毁参数为true

	//decal
	//游戏框架静态函数中的生成贴花函数，返回一个贴花组件的指针
	auto DecalComponent = UGameplayStatics::SpawnDecalAtLocation(GetWorld(),//背景世界
	ImpactData.DecalData.Material,//材质接口
	ImpactData.DecalData.Size,//贴花大小
	Hit.ImpactPoint,//命中位置
	Hit.ImpactNormal.Rotation());//命中法线方向

	if (DecalComponent)
	{
		DecalComponent->SetFadeOut(ImpactData.DecalData.LifeTime, ImpactData.DecalData.FadeOutTime);//逐渐消失函数，参数一：开始延迟时间，参数二：淡出时间，参数三：消失后销毁Actor，默认为true
	}
	//DecalComponent->SetLifeSpan(ImpactData.DecalData.LifeTime);
}

