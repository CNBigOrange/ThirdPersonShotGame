// Shoot Them Up , All Right Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "TouchWidget.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API UTouchWidget : public UUserWidget
{
	GENERATED_BODY()

		//重写初始化函数
		virtual bool Initialize() override;

	//声明蓝图中widget的组件变量
	UPROPERTY(EditAnywhere, Meta = (BindWidget))
		UButton* TestButton = nullptr;

	UPROPERTY(EditAnywhere, Meta = (BindWidget))
		UButton* TestButton2 = nullptr;

	UPROPERTY(EditAnywhere, Meta = (BindWidget))
		UTextBlock* TestText = nullptr;

	//UPROPERTY(EditAnywhere, Meta = (BindWidget))
	//	UWidgetAnimation* Animation = nullptr;

	//新建一个button点击函数
	//UFUNCTION()
	//void TestButtonClicked();

	bool bToggle = false;
};
