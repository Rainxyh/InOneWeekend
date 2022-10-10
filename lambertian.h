#pragma once
#include "material.h"
#include "rand.h"

class lambertian :public material {
public:
	lambertian() {};
	lambertian(vec::vec3 albedo) :albedo(albedo) {};

	virtual bool scatter(const ray& ray_in, const hit_record& rec, vec::vec3& attenuation, ray& scattered) const {
		vec::vec3 target = rec.point + rec.normal +  random_in_unit_sphere();
		scattered = ray(rec.point, target-rec.point);
		attenuation = albedo;
		return true;
	}

	vec::vec3 albedo;
};