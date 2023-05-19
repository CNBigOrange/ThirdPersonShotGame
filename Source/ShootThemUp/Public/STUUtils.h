#pragma once
class STUUtils//按类获取角色组件模板函数
{
public:
	template<class T>
	static T* GetSTUPlayerComponent(AActor* PlayerPawn)//获取组件的模板函数
	{
		if (!PlayerPawn)return nullptr;

		const auto Component = PlayerPawn->GetComponentByClass(T::StaticClass());//按类查找组件，返回的是相应组件的指针
		//按类查找类型要加StaticClass()
		return Cast<T>(Component);
	}
};
