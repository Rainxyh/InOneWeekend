#pragma once
#include "hitable.h"

class sphere : public hitable
{
public:
	sphere()
	{
		center = vec::vec3();
		radius = 1;
		material = NULL;
	};
	sphere(vec::vec3 center, float radius, material *material) : center(center), radius(radius), material(material){};
	virtual bool hit(const ray &ray, float t_min, float t_max, hit_record &rec) const;

	vec::vec3 center;
	float radius;
	material *material;
};

bool sphere::hit(const ray &ray, float t_min, float t_max, hit_record &rec) const
{
	float a = vec::dot(ray.dir, ray.dir);
	float b = 2.0 * vec::dot(ray.dir, ray.ori - center);
	float c = vec::dot(ray.ori - center, ray.ori - center) - radius * radius;
	float delta = b * b - 4 * a * c;
	if (delta > 0)
	{ //与球体有交点
		float temp = (-b - sqrt(delta)) / (2 * a);
		if (temp < t_max && temp > t_min)
		{
			rec.t = temp;
			rec.point = ray.point_at_parameter(rec.t);
			rec.normal = (rec.point - center) / radius; // 法线始终从球心指向外部 使用半径进行标准化 引入方向 可以使用负半径来模拟透明材质
			rec.mat_ptr = material;
			return true;
		}
		temp = (-b + sqrt(delta)) / (2 * a);
		if (temp < t_max && temp > t_min)
		{
			rec.t = temp;
			rec.point = ray.point_at_parameter(rec.t);
			rec.normal = (rec.point - center) / radius; // 当半径为负时 法线指向球心 可以用作气泡来制作空心玻璃球体
			rec.mat_ptr = material;
			return true;
		}
	}
	return false;
}
