// Shoot Them Up , All Right Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUCoreTypes.h"//�Զ���ĺ������࣬�����˽ṹ�塢ί�е�
#include "STUWeaponComponent.generated.h"


class ASTUBaseWeapon;//��ǰ��������������

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTTHEMUP_API USTUWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	USTUWeaponComponent();

	virtual void StartFire();
	void StopFire();
	virtual void NextWeapon();
	void Reload();

	bool GetCurrentWeaponUIData(FWeaponUIData& UIData) const;//�����������ڲ�״̬������const
	//��ȡ��ǰ������UIData
	bool GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const;
	//��ȡ��ǰ������AmmoData

	bool TryToAddAmmo(TSubclassOf<ASTUBaseWeapon> WeaponType, int32 ClipsAmount);

	ASTUBaseWeapon* GetCurrentWeaponForAI() { return CurrentWeapon; }
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		TArray<FWeaponData> WeaponData;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		FName WeaponEquipSocketName = "WeaponSocket";

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		FName WeaponArmorySocketName = "ArmorySocket";

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
		UAnimMontage* EquipAnimMontage;

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY()//����ָ��������Ҫ���UPROPERTY()������õ������ռ������������
		ASTUBaseWeapon* CurrentWeapon = nullptr;

	UPROPERTY()
		TArray<ASTUBaseWeapon*> Weapons;//��������,����Ԫ��Ϊָ�룬�����������Classes��Weapons��Ϊ��ʹ�������ͨ��ת����������ָ���ŵ������У����ҿ���ͨ����ͼ�������Ԫ��

	bool CanFire() const;
	bool CanEquip() const;

	void EquipWeapon(int32 WeaponIndex);

	int32 CurrentWeaponIndex = 0;//���������±�
private:

	UPROPERTY()
	UAnimMontage* CurrentReloadAnimMontage = nullptr;


	bool EquipAnimInProgress = false;//װ������������
	bool ReloadAnimInProgress = false;//װ������������


	void SpawnWeapons();

	void AttachWeaponToSocket(ASTUBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName);//�����������ڲ����


	void PlayAnimMontage(UAnimMontage* Animation);

	void InitAnimations();

	void OnEquipFinished(USkeletalMeshComponent* MeshComponent);
	void OnReloadFinished(USkeletalMeshComponent* MeshComponent);


	bool CanReload() const;

	void OnClipEmpty(ASTUBaseWeapon* AmmoEmptyWeapon);
};
