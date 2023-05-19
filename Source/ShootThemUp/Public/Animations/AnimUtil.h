#pragma once
//����֪ͨ�Ĵ�����

class AnimUtils//��������
{
public:
	template<typename T>//T�����ǣ�USTUEquipFinishedAnimNotify��USTUReloadFinishedAnimNotify
	static T* FindNotifyByClass(UAnimSequenceBase* Animation)//������Ѱ�Ҷ���֪ͨ��ģ��  UAnimSequenceBase��UAnimMontage����ĸ���
		//ʹ�ú�����̬��������Ѻ�������Ϊstatic���ͣ���ú��������˶Ըú����������ļ��ɼ��⣬�����ļ����޷����ʡ�
		//��1������̬�����ᱻ�Զ�������һ��һֱʹ�õĴ洢����ֱ����������Ŵ��ڴ���ʧ��������ú���ʱѹջ��ջ���ٶȿ�ܶࣨ2���������ļ����Զ�����ͬ���ֵĺ��������ᷢ����ͻ��3������̬�������ܱ������ļ����ã������ڽ����ڱ��ļ�
	{
		if (!Animation)return nullptr;

		const auto NotifyEvents = Animation->Notifies;//Animation->Notifies������ TArray<struct FAnimNotifyEvent>
		for (auto NotifyEvent : NotifyEvents)//���������е�Ԫ��    FAnimNotifyEvent�ṹ�����class UAnimNotify * Notify
		{
			auto AnimNotify = Cast<T>(NotifyEvent.Notify);//֪ͨʱ����֪ͨ����UAnimNotifyת��Ϊ����
			if (AnimNotify)
			{
				return AnimNotify;
			}
		}
		return nullptr;
	}
};