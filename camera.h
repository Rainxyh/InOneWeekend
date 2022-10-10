#pragma once
#include "ray.h"
#include "rand.h"
#define M_PI 3.1415926535

class camera
{
public:
	camera(vec::vec3 lookfrom, vec::vec3 lookat, vec::vec3 vup, float vfov, float aspect, float aperture, float focus_dist)
	{ // vfov is top to bottom in degrees
		lens_radius = aperture / 2;
		float theta = vfov * M_PI / 180; // 角度转弧度
		float half_height = tan(theta / 2);
		float half_width = aspect * half_height;

		origin = lookfrom;
		w = vec::unit_vector(lookfrom - lookat);  //相机反方向 从观测点指向相机
		u = vec::unit_vector(vec::cross(vup, w)); //右侧
		v = vec::cross(w, u);					  //上方
		lower_left_corner = origin - (half_width * u + half_height * v + w) * focus_dist;
		// lower_left_corner = origin - (half_width * u + half_height * v + w);
		horizontal = 2 * half_width * u * focus_dist;
		vertical = 2 * half_height * v * focus_dist;
	};

	ray get_ray(float s, float t)
	{
		// return ray(origin, lower_left_corner + horizontal * u + vertical * v - origin);
		vec::vec3 rd = lens_radius * random_in_unit_disk();
		vec::vec3 offset = u * rd.x() + v * rd.y();
		return ray(origin + offset, lower_left_corner + horizontal * s + vertical * t - origin - offset);
	}

	vec::vec3 origin;
	vec::vec3 u, v, w;
	vec::vec3 lower_left_corner;
	vec::vec3 horizontal;
	vec::vec3 vertical;
	float lens_radius;
};