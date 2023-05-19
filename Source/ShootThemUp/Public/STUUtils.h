#pragma once
class STUUtils//�����ȡ��ɫ���ģ�庯��
{
public:
	template<class T>
	static T* GetSTUPlayerComponent(AActor* PlayerPawn)//��ȡ�����ģ�庯��
	{
		if (!PlayerPawn)return nullptr;

		const auto Component = PlayerPawn->GetComponentByClass(T::StaticClass());//���������������ص�����Ӧ�����ָ��
		//�����������Ҫ��StaticClass()
		return Cast<T>(Component);
	}
};
