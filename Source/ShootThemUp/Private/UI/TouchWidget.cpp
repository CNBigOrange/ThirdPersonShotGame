// Shoot Them Up , All Right Reserved


#include "UI/TouchWidget.h"

bool UTouchWidget::Initialize()
{
	if(!Super::Initialize())

	return false;

	//初始化声明的变量
	if (TestText)
	{
		TestText->SetText(FText::FromString("Nice Day!"));
	}

	if (TestButton)
	{
		//绑定button到button的点击事件
		/*TestButton->OnClicked.AddDynamic(this, &UTouchWidget::TestButtonClicked);*/
	}
	return true;
}

//void UTouchWidget::TestButtonClicked()
//{
//	GEngine->AddOnScreenDebugMessage(-1, 200.f, FColor::Blue, TEXT("Button Clicked !"));
//	//播放动画
//	if (Animation && bToggle)
//	{
//		this->PlayAnimation(Animation, 0, 1, EUMGSequencePlayMode::Reverse, 1, false);
//		bToggle = false;
//	}
//	else
//	{
//		this->PlayAnimation(Animation, 0, 1, EUMGSequencePlayMode::Forward, 1, false);
//		bToggle = true;
//	}
//}
