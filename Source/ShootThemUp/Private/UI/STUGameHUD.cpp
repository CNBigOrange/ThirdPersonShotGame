// Shoot Them Up , All Right Reserved


#include "UI/STUGameHUD.h"
#include "Engine/Canvas.h"//����ͷ�ļ�
#include "UI/STUPlayerHUDWidget.h"

void ASTUGameHUD::DrawHUD()
{
	Super::DrawHUD();//���麯���������и��ຯ��

	//DrawCrossHair();
}

void ASTUGameHUD::BeginPlay()
{
	Super::BeginPlay();//ִ�и����ļ�����

	//auto PlayerHUDWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerHUDWidgetClass);//����Widget
	//if (PlayerHUDWidget)
	//{
	//	PlayerHUDWidget->AddToViewport();//AddToViewport()��Ĭ�Ϻ���ZOrder = 0����������HUD�㼶
	//}

	//TSubclassOf<UUserWidget> PlayerHUDWidgetClass;
	for (auto PlayerHUDWidget : PlayerHUDWidgets)
	{
		//TSubclassOf<UUserWidget> PlayerHUDWidgetClass;
		auto HUDWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerHUDWidget);
		if (HUDWidget)
		{
			HUDWidget->AddToViewport();//AddToViewport()��Ĭ�Ϻ���ZOrder = 0����������HUD�㼶
		}
	}
}


void ASTUGameHUD::DrawCrossHair()//��ʮ����
{
	int32 SizeX = Canvas->SizeX;//Canvas��HUD�еĻ�����SizeX�ǻ���X���С
	int32 SizeY = Canvas->SizeY;//Canvas��HUD�еĻ�����SizeY�ǻ���Y���С
	const TInterval<float>Center(SizeX * 0.5f, SizeY * 0.5f);//������е㣬Min��Max�ֱ��ӦXY����
	//Interval���� ������һ���������һ��Min����������Max

	const float HalfLineSize = 10.0f;//�߳���
	const float LineThickness = 1.0f;//�ߴ�ϸ
	const FLinearColor LineColor = FLinearColor::Green;//������ɫ

	DrawLine(Center.Min - HalfLineSize,Center.Max,Center.Min-0.4*HalfLineSize,Center.Max,LineColor,LineThickness);
	//��һ���ݣ�����һ�����X�������������Y�����������յ�X�������ģ��յ�Y�������壺������ɫ����������������ϸ
	DrawLine(Center.Min + HalfLineSize, Center.Max, Center.Min + 0.4 * HalfLineSize, Center.Max, LineColor, LineThickness);
	DrawLine(Center.Min, Center.Max - HalfLineSize, Center.Min , Center.Max -0.4* HalfLineSize, LineColor, LineThickness);
	DrawLine(Center.Min, Center.Max + HalfLineSize, Center.Min, Center.Max + 0.4 * HalfLineSize, LineColor, LineThickness);
}
