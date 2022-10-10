#pragma once
#include "ray.h"

/*
（1）写两个头文件A.h和B.h分别用于声明类A和B；
（2）写两个.cpp文件分别用于定义类A和B；
（3）在A的头文件中导入B的头文件；
（4）在B的头文件中不导入A的头文件，但是用extern 的方式声明类A，并且，在B中使用A的时候要用指针的形式。

原理：在B中用指针调用A，那么在A需要知道B占空间大小的时候，就会去找到B的定义文件，
	  虽然B的定义文件中并没有导入A的头文件，不知道A的占空间大小，但是由于在B中调用A的时候用的指针形式，
	  B只知道指针占4个字节就可以，不需要知道A真正占空间大小，也就是说，A也是知道B的占空间大小的。
*/
extern class material;

struct hit_record
{
	float t;
	vec::vec3 point;
	vec::vec3 normal;
	material *mat_ptr;
};

class hitable
{
public:
	virtual bool hit(const ray &ray, float t_min, float t_max, hit_record &rec) const = 0;
};
