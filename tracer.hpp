#include "math/vector.h"
#include "math/intersect.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "math/stb_image_write.h"

#include "ImPropertyEditor.hpp"


typedef float f32;
typedef int i32;
typedef unsigned int u32;
typedef unsigned char u8;

/*f32 lerp( f32 from, f32 to, f32 t )
{
	return from + (to - from) * t;
}
f32 inverseLerp( f32 from, f32 to, f32 value )
{
	return (value - from) / (to - from);
}*/

template<typename T>
T lerp( T from, T to, f32 t )
{
	return from + (to - from) * t;
}
template<typename T>
f32 inverseLerp( T from, T to, T value )
{
	return (value - from) / (to - from);
}

f32 clamp( f32 min, f32 max, f32 value )
{
	return value < min ? min : value > max ? max : value;
}
f32 lerp01( f32 from, f32 to, f32 t )
{
	return lerp(from, to, clamp(0, 1, t));
}
f32 inverseLerp01( f32 from, f32 to, f32 value )
{
	return inverseLerp(from, to, clamp(from, to, value));
}


static const Color black(0, 0, 0, 1);
static const Color white(1, 1, 1, 1);
static const Color red(1, 0, 0, 1);
static const Color green(0, 1, 0, 1);
static const Color blue(0, 0, 1, 1);
static const Color cyan(0, 1, 1, 1);
static const Color magenta(1, 0, 1, 1);
static const Color yellow(1, 1, 0, 1);

static const size_t AxisX = 0;
static const size_t AxisY = 1;
static const size_t AxisZ = 2;

#include <vector>


class Ray
{
public:
	Vec3f pos;
	Vec3f dir;

	Ray()
	{
	}

	Vec3f at( f32 dist ) const
	{
		return pos + dir * dist;
	}
};

class Prim
{
public:
	Color color;
	bool flat;
	
	Prim() //: this(white)
	{
	}

	Prim( Color _color )
		: color(_color)
		, flat(false)
	{
	}

	virtual Vec3f getNormal( const Vec3f hitPos ) const = 0;

	void shade( const Vec3f lightPos, const Ray& ray, const f32 dist, Color& pixel ) const
	{
		if (flat)
		{
			pixel = color;
			return;
		}

		Vec3f hitPos = ray.at(dist);
		Vec3f hitNormal = getNormal(hitPos);

		Vec3f lightNormal = (lightPos - hitPos).normalized();
		f32 dot = lightNormal.dot(hitNormal);

		float t = inverseLerp01(-1, 1, dot);
		pixel = lerp(black, color, t);
	}
	void shade( const Vec3f lightPos, const Vec3f hitPos, const Vec3f hitNormal, Color& pixel ) const
	{
		if (flat)
		{
			pixel = color;
			return;
		}

		Vec3f lightNormal = (lightPos - hitPos).normalized();
		f32 dot = lightNormal.dot(hitNormal);

		float t = inverseLerp01(-1, 1, dot);
		pixel = lerp(black, color, t);
	}
};

class Sphere : public Prim
{
public:
	Vec3f pos;
	f32 radius;
	
	Sphere()
	{
	}

	Sphere( Vec3f _pos, f32 _radius, Color _color )
		: Prim(_color)
		, pos(_pos)
		, radius(_radius)
	{
	}

	bool intersect( const Ray& ray, f32& dist )
	{
		return intersect_sphere(dist, ray.dir, ray.pos, pos, radius);
	}

	virtual Vec3f getNormal( const Vec3f hitPos ) const
	{
		return (hitPos - pos).normalized();
	}
};

class Plane : public Prim
{
public:
	size_t axis;
	f32 pos;
	
	Plane()
	{
	}

	Plane( size_t _axis, f32 _pos, Color _color )
		: Prim(_color)
		, axis(_axis)
		, pos(_pos)
	{
	}

	bool intersect( const Ray& ray, f32& dist ) const
	{
		return intersect_plane(dist, ray.dir, ray.pos, axis, pos);
	}

	Vec3f normal() const
	{
		Vec3f normal;
		if (pos == 0.0f)
		{
			normal[axis] = -1;
		}
		else
		{
			normal[axis] = -pos;
			normal = normal.normalized();
		}
		return normal;
	}

	virtual Vec3f getNormal( const Vec3f hitPos ) const
	{
		return normal();
	}
};

class Scene
{
public:
	Scene()
	{
	}

	Vec3f camPos;
	Vec3f lightPos;

	std::vector<Sphere> spheres;
	std::vector<Plane> planes;

	class Hit
	{
	public:
		Hit()
			: prim(NULL)
		{
		}

		Prim* prim;
		f32 dist;
		Vec3f pos;
		Vec3f normal;

		operator bool() const { return prim; }
	};

	Hit intersect( const Ray& ray, const f32 dist = FLT_MAX )
	{
		Hit hit;
		hit.dist = dist;

		for (u32 i = 0; i < planes.size(); ++i)
		{
			if (planes[i].intersect(ray, hit.dist))
			{
				hit.prim = &planes[i];
			}
		}

		for (u32 i = 0; i < spheres.size(); ++i)
		{
			if (spheres[i].intersect(ray, hit.dist))
			{
				hit.prim = &spheres[i];
			}
		}

		if (hit.prim)
		{
			hit.pos = ray.at(hit.dist);
			hit.normal = hit.prim->getNormal(hit.pos);
		}

		return hit;
	}

	Color shade( const Ray& ray )
	{
		Color pixel;

		if (Hit hit = intersect(ray))
		{
			hit.prim->shade(lightPos, hit.pos, hit.normal, pixel);
		}

		return pixel;
	}


	bool onGui()
	{
		/*ImGui::Text("%d spheres", spheres.size());
		for (int i = 0; i < spheres.size(); i++)
		{
			const Sphere& sphere = spheres[i];
			ImGui::TextColored(
				(const ImVec4&)sphere.color,
				"Sphere %i : pos (%f, %f), radius %f",
				i, sphere.pos.x, sphere.pos.y, sphere.radius);
		}*/


		bool changed = false;

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2,2));
		ImGui::Columns(2);
		ImGui::Separator();

		ImGui::BeginProperty("Camera");
		changed |= ImGui::DragFloat3("", (float*)&camPos, 0.1f);
		ImGui::NextColumn();
		ImGui::EndProperty();

		ImGui::BeginProperty("Light");
		changed |= ImGui::DragFloat3("", (float*)&lightPos, 0.1f);
		ImGui::NextColumn();
		ImGui::EndProperty();

		if (ImGui::BeginProperty("Spheres", true))
		{
			for (u32 i = 0; i < spheres.size(); i++)
			{
				Sphere& sphere = spheres[i];

				char label[32];sprintf(label, "Sphere %d", i);
				if (ImGui::BeginProperty(label, true))
				{
					ImGui::BeginProperty("Pos");
					changed |= ImGui::DragFloat3("", (float*)&sphere.pos, 0.1f);
					ImGui::NextColumn();
					ImGui::EndProperty();

					ImGui::BeginProperty("Radius");
					changed |= ImGui::DragFloat("", &sphere.radius, 0.1f, 0.1f, 10.f);
					ImGui::NextColumn();
					ImGui::EndProperty();

					ImGui::BeginProperty("Color");
					changed |= ImGui::ColorEdit4("", (float*)&sphere.color);
					ImGui::NextColumn();
					ImGui::EndProperty();

					ImGui::BeginProperty("Flat");
					changed |= ImGui::Checkbox("", &sphere.flat);
					ImGui::NextColumn();
					ImGui::EndProperty();
				}
				ImGui::EndProperty();
			}
		}
		ImGui::EndProperty();

		if (ImGui::BeginProperty("Planes", true))
		{
			for (u32 i = 0; i < planes.size(); i++)
			{
				Plane& plane = planes[i];

				char label[32];sprintf(label, "Plane %d", i);
				if (ImGui::BeginProperty(label, true))
				{
					static const char* axes[3] = { "X", "Y", "Z" };
					ImGui::BeginProperty("Axis");
					changed |= ImGui::Combo("", (int*)&plane.axis, axes, 3);
					ImGui::NextColumn();
					ImGui::EndProperty();

					ImGui::BeginProperty("Pos");
					changed |= ImGui::DragFloat("", &plane.pos, 0.1f);
					ImGui::NextColumn();
					ImGui::EndProperty();

					ImGui::BeginProperty("Color");
					changed |= ImGui::ColorEdit4("", (float*)&plane.color);
					ImGui::NextColumn();
					ImGui::EndProperty();

					ImGui::BeginProperty("Flat");
					changed |= ImGui::Checkbox("", &plane.flat);
					ImGui::NextColumn();
					ImGui::EndProperty();
				}
				ImGui::EndProperty();
			}
		}
		ImGui::EndProperty();

		ImGui::Columns(1);
		ImGui::Separator();
		ImGui::PopStyleVar();

		return changed;
	}
};

class Tracer
{
public:
	Vec2u imageSize;
	Vec2f imageSizeInv;
	RGBA* image;
	GLuint glTextureID;

	Scene scene;

	Tracer()
		: image(NULL)
		, glTextureID(0)
	{
	}
	~Tracer()
	{
		freeImage();
	}

	void initImage( const Vec2u _imageSize )
	{
		freeImage();

		imageSize = _imageSize;
		imageSizeInv = Vec2f(1.f / imageSize.x, 1.f / imageSize.y);

		u32 pixelCount = imageSize.x * imageSize.y;
		image = new RGBA[pixelCount];
	}
	void freeImage()
	{
		if (image)
		{
			delete[] image;
		}
	}

	void initScene()
	{
		scene.camPos = Vec3f(0, 0, -6);
		scene.lightPos = Vec3f(2, 2, 0);

		scene.planes.clear();
		scene.planes.push_back(Plane(AxisY, -2, white));
		scene.planes.push_back(Plane(AxisY, +2, white));
		scene.planes.push_back(Plane(AxisZ, +2, white));
		scene.planes.push_back(Plane(AxisX, -2, red));
		scene.planes.push_back(Plane(AxisX, +2, green));

		scene.spheres.clear();
		scene.spheres.push_back(Sphere(Vec3f(-1, -1, -0.5f), 1, cyan));
		scene.spheres.push_back(Sphere(Vec3f(+1, -1, +0.5f), 1, yellow));
	}

	void render()
	{
		Ray ray;
		ray.pos = scene.camPos;

		for (u32 ix = 0; ix < imageSize.x; ++ix)
		{
			f32 x = ix * imageSizeInv.x - 0.5f;

			for (u32 iy = 0; iy < imageSize.y; ++iy)
			{
				f32 y = iy * imageSizeInv.y - 0.5f;

				ray.dir = (Vec3f(x, y, ray.pos.z + 1) - ray.pos).normalized();

				Color pixel = scene.shade(ray);

				u32 iPixel = ix + (imageSize.y - 1 - iy) * imageSize.x;
				RGBA rgba(u8(pixel.r * 255), u8(pixel.g * 255), u8(pixel.b * 255), u8(pixel.a * 255));
				image[iPixel] = rgba;
			}
		}
	}

	void uploadToGPU()
	{
		if (glTextureID == 0)
		{
			glGenTextures(1, &glTextureID);
			glBindTexture(GL_TEXTURE_2D, glTextureID);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}

		glBindTexture(GL_TEXTURE_2D, glTextureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageSize.x, imageSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	}

	int dumpToPng()
	{
		//int stbi_write_png(char const *filename, int w, int h, int comp, const void *data, int stride_in_bytes);
		const u32 channelCount = 4;
		return stbi_write_png("out.png", imageSize.x, imageSize.y, channelCount, image, imageSize.x * channelCount);
	}


	void init()
	{
		initImage(Vec2u(256, 256));
		initScene();
		render();
		uploadToGPU();

		ImGuiStyle& style = ImGui::GetStyle();
		style.FrameRounding = 4;
		style.GrabRounding = 3;
	}

	void update()
	{
		static bool show_test_window = false;
		static bool show_app_metrics = true;

		ImGui::SetNextWindowSize(ImVec2(200,100), ImGuiSetCond_FirstUseEver);
		ImGui::Begin("Scene");
		{
			//ImGui::Text("glTextureID %d", glTextureID);
			//ImGui::Text("sizeof(RGBA) %d", sizeof(RGBA));

			if (scene.onGui())
			{
				render();
				uploadToGPU();
			}

			if (ImGui::Button("Dump to PNG"))
			{
				dumpToPng();
			}

			ImGui::Checkbox("ImGui demo", &show_test_window);
			ImGui::Checkbox("Metrics", &show_app_metrics);
		}
		ImGui::End();

		ImGui::SetNextWindowSize(ImVec2(300,300), ImGuiSetCond_FirstUseEver);
		ImGui::Begin("Render");
		{
			ImGui::Image((ImTextureID)glTextureID, ImVec2((float)imageSize.x, (float)imageSize.y));
		}
		ImGui::End();


		if (show_test_window)
		{
			ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
			ImGui::ShowTestWindow(&show_test_window);
		}
		if (show_app_metrics)
		{
			ImGui::ShowMetricsWindow(&show_app_metrics);
		}
	}
};
