#pragma once
//动画通知的处理工具

class AnimUtils//动画工具
{
public:
	template<typename T>//T可以是：USTUEquipFinishedAnimNotify或USTUReloadFinishedAnimNotify
	static T* FindNotifyByClass(UAnimSequenceBase* Animation)//根据类寻找动画通知的模板  UAnimSequenceBase是UAnimMontage父类的父类
		//使该函数静态化，如果把函数声明为static类型，则该函数名除了对该函数声明的文件可见外，其他文件都无法访问。
		//（1）、静态函数会被自动分配在一个一直使用的存储区，直到程序结束才从内存消失，避免调用函数时压栈出栈，速度快很多（2）、其他文件可以定义相同名字的函数，不会发生冲突（3）、静态函数不能被其它文件调用，作用于仅限于本文件
	{
		if (!Animation)return nullptr;

		const auto NotifyEvents = Animation->Notifies;//Animation->Notifies是数组 TArray<struct FAnimNotifyEvent>
		for (auto NotifyEvent : NotifyEvents)//遍历数组中的元素    FAnimNotifyEvent结构体包含class UAnimNotify * Notify
		{
			auto AnimNotify = Cast<T>(NotifyEvent.Notify);//通知时间中通知类型UAnimNotify转换为子类
			if (AnimNotify)
			{
				return AnimNotify;
			}
		}
		return nullptr;
	}
};