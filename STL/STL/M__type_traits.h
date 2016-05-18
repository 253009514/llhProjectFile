#pragma once
#include"Mstl_config.h"
namespace __STL
{
	//Ϊ�������б����ֶ������������սṹ�塣���ڲ���ѡ��
	struct __true_type
	{
		__true_type()
		:_t(1)
		{}
		bool _t;
	};
	struct __false_type
	{
		__false_type()
		:_t(0)
		{}
		bool _t;
	};
	//���ﶨ��Ĭ�����Ͷ���__false_type�ģ����������Ͷ��Դ������ػ���
	//���ԣ����Կ����������ͻ��Ц��������ȡ���ܡ�
	//��ô���ǵ��Զ������������Ҫ�Ӻ�STLʹ�ã���Ҫ��Э��ʵ���ػ���
	template<class T>
	struct __type_traits
	{
		//֪ͨ���������Զ���__type_traits�ػ����ı����������template������ġ�
		//��֤��ʹ��ͬ���޹ر��������е�����Ҳ��˳�����С�
		typedef __true_type this_dummy_member_must_be_first;

		typedef __false_type has_traivial_default_constructor;
		typedef __false_type has_traivial_copy_constructor;
		typedef __false_type has_traivial_assignment_operator;
		typedef __false_type has_traivial_destructor;
		typedef __false_type is_POD;
	};
	template <>
	struct __type_traits<unsigned char>
	{
		typedef __false_type has_traivial_default_constructor;
		typedef __false_type has_traivial_copy_constructor;
		typedef __false_type has_traivial_assignment_operator;
		typedef __false_type has_traivial_destructor;
		typedef __false_type is_POD;
	};
	template <>
	struct __type_traits<short>
	{
		typedef __false_type has_traivial_default_constructor;
		typedef __false_type has_traivial_copy_constructor;
		typedef __false_type has_traivial_assignment_operator;
		typedef __false_type has_traivial_destructor;
		typedef __false_type is_POD;
	};
	template <>
	struct __type_traits<unsigned short>
	{
		typedef __false_type has_traivial_default_constructor;
		typedef __false_type has_traivial_copy_constructor;
		typedef __false_type has_traivial_assignment_operator;
		typedef __false_type has_traivial_destructor;
		typedef __false_type is_POD;
	};
	template <>
	struct __type_traits<int>
	{
		typedef __false_type has_traivial_default_constructor;
		typedef __false_type has_traivial_copy_constructor;
		typedef __false_type has_traivial_assignment_operator;
		typedef __false_type has_traivial_destructor;
		typedef __false_type is_POD;
	};
	template <>
	struct __type_traits<unsigned int>
	{
		typedef __false_type has_traivial_default_constructor;
		typedef __false_type has_traivial_copy_constructor;
		typedef __false_type has_traivial_assignment_operator;
		typedef __false_type has_traivial_destructor;
		typedef __false_type is_POD;
	};
	template <>
	struct __type_traits<long>
	{
		typedef __false_type has_traivial_default_constructor;
		typedef __false_type has_traivial_copy_constructor;
		typedef __false_type has_traivial_assignment_operator;
		typedef __false_type has_traivial_destructor;
		typedef __false_type is_POD;
	};
	template <>
	struct __type_traits<unsigned long>
	{
		typedef __false_type has_traivial_default_constructor;
		typedef __false_type has_traivial_copy_constructor;
		typedef __false_type has_traivial_assignment_operator;
		typedef __false_type has_traivial_destructor;
		typedef __false_type is_POD;
	};
	template <>
	struct __type_traits<float>
	{
		typedef __false_type has_traivial_default_constructor;
		typedef __false_type has_traivial_copy_constructor;
		typedef __false_type has_traivial_assignment_operator;
		typedef __false_type has_traivial_destructor;
		typedef __false_type is_POD;
	};
	template <>
	struct __type_traits<double>
	{
		typedef __false_type has_traivial_default_constructor;
		typedef __false_type has_traivial_copy_constructor;
		typedef __false_type has_traivial_assignment_operator;
		typedef __false_type has_traivial_destructor;
		typedef __false_type is_POD;
	};
	template <>
	struct __type_traits<long double>
	{
		typedef __false_type has_traivial_default_constructor;
		typedef __false_type has_traivial_copy_constructor;
		typedef __false_type has_traivial_assignment_operator;
		typedef __false_type has_traivial_destructor;
		typedef __false_type is_POD;
	};
	template<class T>
	struct __type_traits<T*>
	{
		typedef __false_type has_traivial_default_constructor;
		typedef __false_type has_traivial_copy_constructor;
		typedef __false_type has_traivial_assignment_operator;
		typedef __false_type has_traivial_destructor;
		typedef __false_type is_POD;
	};
}

using namespace __STL;
template<class T>
class Test_type_traits
{
public:
	typedef __true_type has_traivial_default_constructor;
	typedef __true_type has_traivial_copy_constructor;
	typedef __true_type has_traivial_assignment_operator;
	typedef __true_type has_traivial_destructor;
	typedef __true_type is_POD;
};

void test(__true_type)
{}
void test(__false_type)
{}
void Test_type_trait()
{
	test(Test_type_traits<int>::is_POD());
}