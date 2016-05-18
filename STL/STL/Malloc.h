#pragma once 
#include"Mstl_config.h"

#ifdef __USE_MALLOC
#define __THROW_BAD_ALLOC throw bad_alloc


//�ٿռ����ò�����ֱ��ʹ��C++new_handler ���ƣ���Ϊ������ʹ��operator new����ռ�
//��C++û���ṩrealloc�����ò���

//ע���ڴ治�㴦�������ǿͶ˵����Σ����趨�ڴ治�㴦������ Ҳ����ְ���ڴ治�㴦���������ض�ģʽ
template<int inst>
class __malloc_alloc_template
{
private:
	static void *oom_malloc(size_t);
	static void *oom_realloc(void *, size_t);
	static void(*__malloc_alloc_oom_handler)();
public:
	static void *allocate(size_t n)
	{
		void *result = mallo(n);
		if (0 == result)
		{
			result = oom_malloc(n);
		}
		return result;
	}
	static void *reallocate(void *p, size_t old_sz, size_t new_sz)
	{
		void* result = realloc(p, new_sz);
		if (0 == result)
		{
			result = oom_realloc(p, new_sz);
			return result;
		}
	}

	//beau
	void(*set_malloc_handler(void(*f)()))()
	{
		void(*old)() = __malloc_alloc_oom_handler;
		_malloc_alloc_oom_handler = f;
		return (old);
	}

	template<int inst>
	void(*__malloc_alloc_template<inst>::__malloc_alloc_oom_handler)() = 0;
	
	template<int inst>
	void * __malloc_alloc_template<inst>::oom_malloc(size_t n)
	{
		void(*my_malloc_handler)();
		void* result;
		for (;;)
		{
			my_malloc_handler = __malloc_alloc_oom_handler;
			if (0 == my_malloc_handler)
			{
				__THROW_BAD_ALLOC;
			}
			(*__malloc_alloc_oom_handler)();
			result = malloc(n);
			if (result)
				return result;
		}
	}

	template<int inst>
	void* oom_realloc(void* p, size_t n)
	{
		void(*my_malloc_handler)();
		void* result;

		for (;;)
		{
			my_malloc_handler = __malloc_alloc_oom_handler;
			if (0 == my_malloc_handler)
			{
				__THROW_BAD_ALLOC;
			}
			(*my_malloc_handler)();
			result = realloc(p, n);
			if (result)
				return result;
		}
	}
};
typedef __malloc_alloc_template<0> alloc;
#else


typedef __default_alloc_template alloc;
#endif