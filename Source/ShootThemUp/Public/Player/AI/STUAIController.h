// Shoot Them Up , All Right Reserved

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "STUAIController.generated.h"

class USTUAIPerceptionComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUAIController : public AAIController
{
	GENERATED_BODY()
public:
	ASTUAIController();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		USTUAIPerceptionComponent* STUAIPerceptionComponent;//AI��֪���

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		FName FocusOnKeyName = "EnemyActor";//������Կ����


	virtual void OnPossess(APawn* InPawn) override;//�����ӱ�������ռ��ʱ���õĺ���
	virtual void Tick(float DeltaTime) override;


private:
	AActor* GetFocusOnActor() const;

	void AttackClosestEnemy(AActor* ClosestEnemy)const;//�����������
};
