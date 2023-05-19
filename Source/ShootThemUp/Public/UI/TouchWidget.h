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

		//��д��ʼ������
		virtual bool Initialize() override;

	//������ͼ��widget���������
	UPROPERTY(EditAnywhere, Meta = (BindWidget))
		UButton* TestButton = nullptr;

	UPROPERTY(EditAnywhere, Meta = (BindWidget))
		UButton* TestButton2 = nullptr;

	UPROPERTY(EditAnywhere, Meta = (BindWidget))
		UTextBlock* TestText = nullptr;

	//UPROPERTY(EditAnywhere, Meta = (BindWidget))
	//	UWidgetAnimation* Animation = nullptr;

	//�½�һ��button�������
	//UFUNCTION()
	//void TestButtonClicked();

	bool bToggle = false;
};
