// Shoot Them Up , All Right Reserved


#include "UI/STUGameHUD.h"
#include "Engine/Canvas.h"//画布头文件
#include "UI/STUPlayerHUDWidget.h"

void ASTUGameHUD::DrawHUD()
{
	Super::DrawHUD();//纯虚函数，先运行父类函数

	//DrawCrossHair();
}

void ASTUGameHUD::BeginPlay()
{
	Super::BeginPlay();//执行父类文件代码

	//auto PlayerHUDWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerHUDWidgetClass);//创建Widget
	//if (PlayerHUDWidget)
	//{
	//	PlayerHUDWidget->AddToViewport();//AddToViewport()有默认函数ZOrder = 0，用于设置HUD层级
	//}

	//TSubclassOf<UUserWidget> PlayerHUDWidgetClass;
	for (auto PlayerHUDWidget : PlayerHUDWidgets)
	{
		//TSubclassOf<UUserWidget> PlayerHUDWidgetClass;
		auto HUDWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerHUDWidget);
		if (HUDWidget)
		{
			HUDWidget->AddToViewport();//AddToViewport()有默认函数ZOrder = 0，用于设置HUD层级
		}
	}
}


void ASTUGameHUD::DrawCrossHair()//画十字线
{
	int32 SizeX = Canvas->SizeX;//Canvas是HUD中的画布，SizeX是画布X轴大小
	int32 SizeY = Canvas->SizeY;//Canvas是HUD中的画布，SizeY是画布Y轴大小
	const TInterval<float>Center(SizeX * 0.5f, SizeY * 0.5f);//定义的中点，Min和Max分别对应XY坐标
	//Interval区间 ，定义一个区间参数一：Min，参数二：Max

	const float HalfLineSize = 10.0f;//线长度
	const float LineThickness = 1.0f;//线粗细
	const FLinearColor LineColor = FLinearColor::Green;//线条颜色

	DrawLine(Center.Min - HalfLineSize,Center.Max,Center.Min-0.4*HalfLineSize,Center.Max,LineColor,LineThickness);
	//画一条陷，参数一：起点X，参数二：起点Y，参数三：终点X，参数四：终点Y，参数五：线条颜色，参数六：线条粗细
	DrawLine(Center.Min + HalfLineSize, Center.Max, Center.Min + 0.4 * HalfLineSize, Center.Max, LineColor, LineThickness);
	DrawLine(Center.Min, Center.Max - HalfLineSize, Center.Min , Center.Max -0.4* HalfLineSize, LineColor, LineThickness);
	DrawLine(Center.Min, Center.Max + HalfLineSize, Center.Min, Center.Max + 0.4 * HalfLineSize, LineColor, LineThickness);
}
