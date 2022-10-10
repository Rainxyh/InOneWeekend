#include <fstream>
#include <iomanip>
#include "sphere.h"
#include "hitablelist.h"
#include "camera.h"
#include "lambertian.h"
#include "metal.h"
#include "dielectric.h"

hitable*   scene_TheNextWeek() {
	int n = 500;
	int count = 0;
	hitable** list = new hitable*[n + 1];
	list[count] = new sphere(vec::vec3(0, -1000, 0), 1000, new lambertian(vec::vec3(0.5, 0.5, 0.5)));
	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			float choose_mat = rand_float();
			vec::vec3 center(a + 0.9 * rand_float(), 0.2, b + 0.9 * rand_float());
			if ((center - vec::vec3(4, 0.2, 0)).length() > 0.9) {
				if (choose_mat < 0.8) {  // diffuse
					list[++count] = new sphere(center, 0.2, new lambertian(vec::vec3(square_rand_float(), square_rand_float(), square_rand_float())));
				}
				else if (choose_mat < 0.95) {  // metal
					list[++count] = new sphere(center, 0.2, new metal(0.5 * vec::vec3(1 + rand_float(), 1 + rand_float(), 1 + rand_float()), 0.5 * rand_float()));
				}
				else {  // glass
					list[++count] = new sphere(center, 0.2, new dielectric(1.5));
				}
			}
		}
	}
	list[++count] = new sphere(vec::vec3(0, 1, 0), 1.0, new dielectric(1.5));
	list[++count] = new sphere(vec::vec3(-4, 1, 0), 1.0, new lambertian(vec::vec3(0.4, 0.2, 0.1)));	
	list[++count] = new sphere(vec::vec3(4, 1, 0), 1.0, new metal(vec::vec3(0.7, 0.6, 0.5), 0.0));

	hitable* world_TheNextWeek = new hitable_list(list, count+1);
	return world_TheNextWeek;
}

void showProgress(int num, int sum)
{
	std::cout << "\r";
	std::cout << "running: " << (sum - num) * 100 / sum << "%";
}
int main()
{
	std::ofstream outfile;
	outfile.open("InOneWeekend.ppm");
	int nx = 1920;
	int ny = 1080;
	int ns = 100;
	outfile << "P3\n" << nx << " " << ny << "\n255" << std::endl;

	vec::vec3 lookfrom = vec::vec3(13, 2, 3);
	vec::vec3 lookat = vec::vec3(0, 0, 0);
	vec::vec3 vup = vec::vec3(0, 1, 0);
	float vfov = 20;
	float aspect = float(nx) / float(ny);
	float aperture = 0.1;
	float dist_to_focus = 10;
	camera camera(lookfrom, lookat, vup, vfov, aspect, aperture, dist_to_focus);

	hitable* world =   scene_TheNextWeek();

	for (int j = ny; j >= 0; --j) {
		showProgress(j, ny);
		for (int i = 0; i < nx; ++i) {
			vec::vec3 color(0, 0, 0);
			for (int s = 0; s < ns; ++s) {
				float u = ((float)i + rand_float()) / nx, v = ((float)j + rand_float()) / ny;
				ray ray = camera.get_ray(u, v);
				color += get_color(ray, world, 1);
			}
			color /= (float)ns;
			color = gamma_correct(color);
			int ir = int(255.99f * color.r());
			int ig = int(255.99f * color.g());
			int ib = int(255.99f * color.b());
			outfile << ir << " " << ig << " " << ib << std::endl;
		}
	}
	return 0;
}