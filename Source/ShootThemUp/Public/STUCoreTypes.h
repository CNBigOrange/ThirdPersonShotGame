#pragma once
//自定义的核心种类，集合了结构体、委托等的头文件

#include "STUCoreTypes.generated.h"//手动添加生成工具

//Weapon
class ASTUBaseWeapon;//FAmmoData中使用BaseWeapon类，因此必须添加此类的前置说明

DECLARE_MULTICAST_DELEGATE_OneParam(FOnClipEmptySignature,ASTUBaseWeapon*);//当弹夹空了的委托,参数为装弹武器的指针

USTRUCT(BlueprintType)
struct FAmmoData//定义弹药类结构体
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		int32 Bullets;//子弹数

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", meta = (EditCondition = "!Infinite"))
		//EditCondition = "!Infinite" 编辑条件是：不是无限子弹
		int32 Clips;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		bool Infinite;
};

USTRUCT(BlueprintType)
struct FWeaponData//定义武器装弹结构体
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		TSubclassOf<ASTUBaseWeapon> WeaponClass;//TSubclassOf<T>创建T的子类实例

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		UAnimMontage* ReloadAnimMontage;//动画蒙太奇
};

//定义武器UI结构体（准信、图标等）
USTRUCT(BlueprintType)
struct FWeaponUIData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
		UTexture2D* MainIcon;//UTexture2D纹理数据类型，Icon图标

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
		UTexture2D* CrossHairIcon;
};

//Health
//委托 是一种泛型但类型安全的方式，可在C++对象上调用成员函数。可使用委托动态绑定到任意对象的成员函数，之后在该对象上调用函数，即使调用程序不知对象类型也可进行操作。
//声明委托DECLARE_MULTICAST_DELEGATE只在C++中使用，加DYNAMIC的委托可以在蓝图中使用
DECLARE_MULTICAST_DELEGATE(FOnDeath);//所有委托必须以大写F开头，没有参数
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float);//一个参数的仅在C++使用的委托
//DECLARE_MULTICAST_DELEGATE(FOnDestroyWeapon);//所有委托必须以大写F开头
//DECLARE_MULTICAST_DELEGATE(FOnClipEmpty);//当弹夹空了的委托



//VFX

class UNiagaraSystem;

USTRUCT(BlueprintType)
struct FDecalData//贴花数据
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
		UMaterialInterface* Material;//材质接口

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
		FVector Size = FVector(10.0f);//贴花大小，FVector重载使FVector三个方向均为同一个参数

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
		float LifeTime = 5.0f;//贴花生命的时间

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
		float FadeOutTime = 0.7f;//贴花渐隐时间
};

USTRUCT(BlueprintType)
struct FImpactData//撞击数据
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
		UNiagaraSystem* NiagaraEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
		FDecalData DecalData;
};