#pragma once
#include "ray.h"
#include "hitable.h"

class material
{
public:
	// hit_record is to avoid a bunch of arguments so we can stuff whatever info we want in there. You can use arguments instead;
	virtual bool scatter(const ray &ray_in, const hit_record &rec, vec::vec3 &attenuation, ray &scattered) const = 0;
};

vec::vec3 get_color(const ray &ray_, hitable *world, int depth)
{
	hit_record rec;
	if (world->hit(ray_, 0.001, FLT_MAX, rec))
	{ // 击中物体则进行递归并衰减 相当于物体吸收光线 最小值不为零防止近距离弹射多次导致能量快速衰减出现暗疮
		ray scattered;
		vec::vec3 attenuation;
		if (depth < 50 && rec.mat_ptr->scatter(ray_, rec, attenuation, scattered))
		{
			return attenuation * get_color(scattered, world, depth + 1);
		}
		return vec::vec3(0, 0, 0);
	}
	else
	{ // 否则返回背景色
		vec::vec3 unit_direction = vec::unit_vector(ray_.direction());
		float t = 0.5 * (unit_direction.y() + 1.0);
		return (1.0 - t) * vec::vec3(1, 1, 1) + t * vec::vec3(0.5, 0.7, 1.0);
	}
}

vec::vec3 gamma_correct(vec::vec3 col)
{
	return vec::vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
}

inline vec::vec3 reflect(const vec::vec3 &v, const vec::vec3 &n)
{
	return v - 2 * dot(v, n) * n;
}

inline bool refract(const vec::vec3 &v, const vec::vec3 &normal, float eta, vec::vec3 &refracted)
{
	vec::vec3 uv = unit_vector(v);
	float cosine = dot(uv, normal);
	float discrimination = 1.0f - eta * eta * (1.0f - cosine * cosine);
	// 若discrimination>0则，当前仍满足菲涅尔定律 sin_theta_i = (n_prime/n)*sin_theta_t <= 1，
	// 但此时sin_theta_t仍可继续增大，导致判别式小于0，出现全反射现象。
	// 球面并不会出现全反射，入射球面的光一定能够以相同角度出射。
	if (discrimination > 0)
	{
		refracted = eta * (uv - normal * cosine) - normal * sqrt(discrimination);
		return true;
	}
	return false;
}

inline float schlick(float cosine, float eta)
{
	float r0 = (1.0f - eta) / (1.0f + eta);
	r0 = r0 * r0;
	return r0 + (1.0f - r0) * (float)pow((1 - cosine), 5);
}
