#pragma once
//�Զ���ĺ������࣬�����˽ṹ�塢ί�еȵ�ͷ�ļ�

#include "STUCoreTypes.generated.h"//�ֶ�������ɹ���

//Weapon
class ASTUBaseWeapon;//FAmmoData��ʹ��BaseWeapon�࣬��˱�����Ӵ����ǰ��˵��

DECLARE_MULTICAST_DELEGATE_OneParam(FOnClipEmptySignature,ASTUBaseWeapon*);//�����п��˵�ί��,����Ϊװ��������ָ��

USTRUCT(BlueprintType)
struct FAmmoData//���嵯ҩ��ṹ��
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		int32 Bullets;//�ӵ���

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", meta = (EditCondition = "!Infinite"))
		//EditCondition = "!Infinite" �༭�����ǣ����������ӵ�
		int32 Clips;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		bool Infinite;
};

USTRUCT(BlueprintType)
struct FWeaponData//��������װ���ṹ��
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		TSubclassOf<ASTUBaseWeapon> WeaponClass;//TSubclassOf<T>����T������ʵ��

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		UAnimMontage* ReloadAnimMontage;//������̫��
};

//��������UI�ṹ�壨׼�š�ͼ��ȣ�
USTRUCT(BlueprintType)
struct FWeaponUIData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
		UTexture2D* MainIcon;//UTexture2D�����������ͣ�Iconͼ��

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
		UTexture2D* CrossHairIcon;
};

//Health
//ί�� ��һ�ַ��͵����Ͱ�ȫ�ķ�ʽ������C++�����ϵ��ó�Ա��������ʹ��ί�ж�̬�󶨵��������ĳ�Ա������֮���ڸö����ϵ��ú�������ʹ���ó���֪��������Ҳ�ɽ��в�����
//����ί��DECLARE_MULTICAST_DELEGATEֻ��C++��ʹ�ã���DYNAMIC��ί�п�������ͼ��ʹ��
DECLARE_MULTICAST_DELEGATE(FOnDeath);//����ί�б����Դ�дF��ͷ��û�в���
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float);//һ�������Ľ���C++ʹ�õ�ί��
//DECLARE_MULTICAST_DELEGATE(FOnDestroyWeapon);//����ί�б����Դ�дF��ͷ
//DECLARE_MULTICAST_DELEGATE(FOnClipEmpty);//�����п��˵�ί��



//VFX

class UNiagaraSystem;

USTRUCT(BlueprintType)
struct FDecalData//��������
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
		UMaterialInterface* Material;//���ʽӿ�

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
		FVector Size = FVector(10.0f);//������С��FVector����ʹFVector���������Ϊͬһ������

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
		float LifeTime = 5.0f;//����������ʱ��

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
		float FadeOutTime = 0.7f;//��������ʱ��
};

USTRUCT(BlueprintType)
struct FImpactData//ײ������
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
		UNiagaraSystem* NiagaraEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
		FDecalData DecalData;
};