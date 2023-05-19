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
		USTUAIPerceptionComponent* STUAIPerceptionComponent;//AI感知组件

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		FName FocusOnKeyName = "EnemyActor";//敌人秘钥名字


	virtual void OnPossess(APawn* InPawn) override;//当棋子被控制器占有时调用的函数
	virtual void Tick(float DeltaTime) override;


private:
	AActor* GetFocusOnActor() const;

	void AttackClosestEnemy(AActor* ClosestEnemy)const;//攻击最近敌人
};
