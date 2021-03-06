/*!
 *  @file Objects.h
 *  @author Gabriela Havranova
 *  @date 2021-05-12
 *  @project Wonderland
 *
 *  This file contains definitions of all classes inheriting from ObjectBase class - all the scene objects
 */
 
#pragma once
#include <vector>

#include "ConstsAndTypes.h"
#include "Shader.h"
#include "VerticesLib.h"
#include "stb_image.h"
#include "ObjectBase.h"
#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"

const glm::vec3 ones3f = glm::vec3 (1.0f, 1.0f, 1.0f);
const glm::vec3 zeroes3f = glm::vec3 (0.0f, 0.0f, 0.0f);

/*!
 *  Base class for all the mushrooms - they all draw in the same way from positions and rotations stored in vectors
 */
class MushroomBase : public ObjectBase {
public:
	MushroomBase (const float* vertices, const int vertices_cnt,
		const unsigned int* indices, const int indices_cnt,
		std::shared_ptr<Shader> shader, const std::string& tex_name,
		std::vector<glm::vec3>& colliders)
		: ObjectBase (vertices, vertices_cnt, indices, indices_cnt, shader) {
		for (auto& mesh : meshes) mesh.createTexture (tex_name.c_str (), true, true);
	}

	void DrawObject () override {
		for (int i = 0; i < mushroom_cnt; i++) {
			glm::mat4 model = glm::mat4 (1.0f);
			model = glm::translate (model, this->positions[i]);
			model = glm::rotate (model, glm::radians (-90.0f), x_axis);
			model = glm::rotate (model, glm::radians (angles[i]), rotations[i]);
			model = glm::scale (model, scales[i]);
			setModelMatrices (model);

			if (is_clickable) {
				shader->setVec3 ("click_test.object_color", click_test_obj_colors[i]);
				shader->setBool ("is_clickable", true);
			}

			setMeshMaterial (ones3f, 2.0f);
			glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
			for (const auto& mesh : meshes) mesh.Draw ();

			if (is_clickable) {
				shader->setVec3 ("click_test.object_color", zeroes3f);
				shader->setBool ("is_clickable", false);
			}
		}
	}

	int mushroom_cnt;
	bool is_clickable = false;
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> scales;
	std::vector<glm::vec3> rotations;
	std::vector<glm::vec3> click_test_obj_colors;
	std::vector<float> angles;
};

/*!
 *	Represents red mushroom near lava.
 */
class RedMushroom : public MushroomBase {
public:
	RedMushroom (const float* vertices, const int vertices_cnt,
		const unsigned int* indices, const int indices_cnt,
		std::shared_ptr<Shader> shader, const std::string& tex_name,
		std::vector<glm::vec3>& colliders)
		: MushroomBase (vertices, vertices_cnt, indices, indices_cnt, shader,
			tex_name, colliders) {
		this->mushroom_cnt = 1;
		this->positions = { glm::vec3 (1.0f, 1.0f, 1.0f) };
		this->scales = { glm::vec3 (3.0f) };
		this->angles = { 10.0f };
		this->rotations = { z_axis };
		float diameter = 4.0f;
		colliders.emplace_back (1.0f, 1.0f, diameter);
	}
};

/*!
 *  Represents blue single mushrooms. 
 */
class BlueShrooms : public MushroomBase {
public:
	BlueShrooms (const float* vertices, const int vertices_cnt,
		const unsigned int* indices, const int indices_cnt,
		std::shared_ptr<Shader> shader, const std::string& tex_name,
		std::vector<glm::vec3>& colliders)
		: MushroomBase (vertices, vertices_cnt, indices, indices_cnt, shader,
			tex_name, colliders) {
		this->mushroom_cnt = 4;
		this->positions = { glm::vec3 (96.768f, 35.0f, 33.276f),
						   glm::vec3 (-2.11738f, 45.0f, 80.3774f),
						   glm::vec3 (-63.5439f, 35.0f, 41.4656f),
						   glm::vec3 (31.0f, 20.0f, -15.0f) };
		this->scales = { glm::vec3 (4.0f), glm::vec3 (5.5f), glm::vec3 (5.5f),
						glm::vec3 (2.0f) };
		this->angles = { 10.0f, 125.0f, 25.0f, 17.0f };
		this->rotations = { x_axis, z_axis, z_axis, x_axis + z_axis * 0.5f };
		float diameter = 4.0f;
		colliders.emplace_back (29.4098f, -22.5141f, diameter);
		colliders.emplace_back (95.4057f, 19.5499f, diameter);
		colliders.emplace_back (-11.4088f, 86.2395f, diameter + 1.5f);
		colliders.emplace_back (-66.6502f, 33.1402f, diameter + 5.5f);
		is_clickable = true;
		for (int i = 1; i < 5; i++)
			click_test_obj_colors.emplace_back (0.0f, 0.0f, (float)i / 255.0f);
	}

	/*!
	 *  Performs the action after click is recognized.
	 *
	 *  @param [in] object_id 
	 */
	void applyClick (int object_id) override {
		if (object_id > scales.size ()) return;
		float random = ((int)(getTimeSeed () * 1000) % 100 / 20.0f) + 1.5f;
		this->scales[object_id - 1] = glm::vec3 (random);
	}
};

/*!
 *  Represents double mushrooms in the scene. 
 */
class DoubleMushrooms : public MushroomBase {
public:
	DoubleMushrooms (const float* vertices, const int vertices_cnt,
		const unsigned int* indices, const int indices_cnt,
		std::shared_ptr<Shader> shader, const std::string& tex_name,
		std::vector<glm::vec3>& colliders)
		: MushroomBase (vertices, vertices_cnt, indices, indices_cnt, shader,
			tex_name, colliders) {
		this->mushroom_cnt = 6;
		this->positions = { glm::vec3 (-94.3434f, 50.001f, -10.4867f),
						   glm::vec3 (-71.5002f, 35.001f, 53.5723f),
						   glm::vec3 (-36.0634f, 5.01f, 65.7154f),
						   glm::vec3 (17.6295f, 17.0f, 76.5066f),
						   glm::vec3 (58.6476f, 10.001f, 90.6306f),
						   glm::vec3 (59.3222f, 50.001f, 3.98706f) };
		this->scales = { glm::vec3 (4.0f), glm::vec3 (2.2f), glm::vec3 (0.5f),
						glm::vec3 (2.0f), glm::vec3 (1.2f), glm::vec3 (4.0f) };
		this->angles = { 10.0f, 15.0f, 25.0f, 17.0f, 180.0f, 160.0f };
		this->rotations = {
			x_axis, y_axis + z_axis, z_axis, x_axis + z_axis * 0.5f, z_axis, z_axis,
		};
		const float diameters[6] = { 12.0f, 10.0f, 6.0f, 10.0f, 8.0f, 13.0f };
		for (int i = 0; i < this->mushroom_cnt; i++)
			colliders.emplace_back (positions[i].x, positions[i].z, diameters[i]);
		is_clickable = true;
		for (int i = 10; i < 17; i++)
			click_test_obj_colors.emplace_back (0.0f, (float)i / 255.0f, 0.0f);
	}

	/*!
	 *  Performs the action after click is recognized.
	 *
	 *  @param [in] object_id
	 */
	void applyClick (int object_id) override {
		if (object_id - 10 > rotations.size ()) return;
		float random = ((int)(getTimeSeed () * 1000) % 100 / 20.0f) + 1.5f;
		this->angles[object_id - 10] = random * 90.0f;
		this->rotations[object_id - 10] = glm::vec3 (z_axis);
	}
};

/*!
 *	Represents small Pine.
 */
class Pine : public MushroomBase {
public:
	Pine (const float* vertices, const int vertices_cnt,
		const unsigned int* indices, const int indices_cnt,
		std::shared_ptr<Shader> shader, const std::string& tex_name,
		std::vector<glm::vec3>& colliders)
		: MushroomBase (vertices, vertices_cnt, indices, indices_cnt, shader,
			tex_name, colliders) {
		this->mushroom_cnt = 3;
		this->positions = { glm::vec3 (-40.0f, 17.0f, 1.0f), glm::vec3 (-60.0f, 17.0f, 10.0f), glm::vec3 (-20.0f, 17.0f, 2.0f) };
		this->scales = { glm::vec3 (3.0f),  glm::vec3 (3.5f),  glm::vec3 (3.0f) };
		this->angles = { 10.0f, 40.0f, 80.0f };
		this->rotations = { z_axis };
		float diameter = 4.0f;
		colliders.emplace_back (-40.0f, 1.0f, diameter);
		colliders.emplace_back (-60.0f, 10.0f, diameter);
		colliders.emplace_back (-20.0f, 2.0f, diameter);
	}
};

/*!
 *	Represents small RoundTree.
 */
class RoundTree : public MushroomBase {
public:
	RoundTree (const float* vertices, const int vertices_cnt,
		const unsigned int* indices, const int indices_cnt,
		std::shared_ptr<Shader> shader, const std::string& tex_name,
		std::vector<glm::vec3>& colliders)
		: MushroomBase (vertices, vertices_cnt, indices, indices_cnt, shader,
			tex_name, colliders) {
		this->mushroom_cnt = 1;
		this->positions = { glm::vec3 (-80.0f, 15.0f, -100.0f) };
		this->scales = { glm::vec3 (0.5f, 0.5f, 4.0f) };
		this->angles = { 10.0f };
		this->rotations = { z_axis };
		float diameter = 4.0f;
		colliders.emplace_back (1.0f, 1.0f, diameter);
	}
};

/*!
 *  Respresents the monkey.
 */
class God : public ObjectBase {
public:
	God (const float* vertices, const int vertices_cnt,
		const unsigned int* indices, const int indices_cnt,
		std::shared_ptr<Shader> shader)
		: ObjectBase (vertices, vertices_cnt, indices, indices_cnt, shader) {
		for (auto& mesh : meshes) {
			mesh.createTexture (".\\objects\\textures\\monke.png", true, true);
		}
	}
	/*!
	 *  Draws the object.
	 */
	void DrawObject () override {
		glm::mat4 model = glm::mat4 (1.0f);
		model = glm::translate (model, glm::vec3 (150.0f, 65.0f, -250.0f));
		float angle = -90.0f;
		model = glm::rotate (model, glm::radians (angle), glm::vec3 (1.0f, 0.0f, 0.0f));
		model = glm::rotate (model, glm::radians (25.0f), glm::vec3 (0.0f, 1.0f, 0.2f));
		model = glm::scale (model, glm::vec3 (50.0f));

		setModelMatrices (model);
		glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
		setMeshMaterial (ones3f, ones3f, 2.0f);
		for (const auto& mesh : meshes) {
			mesh.Draw ();
		}
	}
};


class Lava : public ObjectBase {
public:
	Lava (const float* vertices, const int vertices_cnt,
		const unsigned int* indices, const int indices_cnt,
		std::shared_ptr<Shader> shader, char color_id)
		: ObjectBase (vertices, vertices_cnt, indices, indices_cnt, shader,
			color_id) {
		for (auto& mesh : meshes)
			mesh.createTexture (".\\objects\\textures\\lava_diffuse.jpg", false);
		speed = 2.0f;
	}

	void applyClick (int object_id) override {
		lava_sped_up = !lava_sped_up;
		speed = ((int)(getTimeSeed () * 1000) % 100 / 20.0f) + 1.5f;
	}

	void DrawObject () override {
		shader->setBool ("is_lava", true);
		shader->setBool ("is_lava_frag", true);
		shader->setBool ("is_clickable", true);
		shader->setFloat ("lava_sped", speed);
		shader->setVec3 ("click_test.object_color", glm::vec3 (1.0f, 0.0f, 0.0f));
		glm::mat4 model = glm::mat4 (1.0f);
		model = glm::translate (model, glm::vec3 (-20.0f, -5.5f, -39.0f));
		float angle = -90.0f;
		model = glm::rotate (model, glm::radians (angle), glm::vec3 (1.0f, 0.0f, 0.0f));
		model = glm::scale (model, glm::vec3 (5.5, 5.5, 5.5));
		setModelMatrices (model);
		setMeshMaterial (ones3f, 2.0f);
		for (const auto& mesh : meshes) mesh.Draw ();
		shader->setBool ("is_lava", false);
		shader->setBool ("is_lava_frag", false);
		shader->setBool ("is_clickable", false);
		shader->setVec3 ("click_test.object_color", glm::vec3 (0.0f, 0.0f, 0.0f));
	}

private:
	bool lava_sped_up = false;
	float speed;
};

class Flame : public ObjectBase {
public:
	Flame (const float* vertices, const int vertices_cnt,
		const unsigned int* indices, const int indices_cnt,
		std::shared_ptr<Shader> shader)
		: ObjectBase (vertices, vertices_cnt, indices, indices_cnt, shader) {
		for (auto& mesh : meshes)
			mesh.createTexture (".\\objects\\textures\\flame_spritesheet.png", true, true);
		positions = { glm::vec3 (-33.3198f, -5.0f, -60.7827f), glm::vec3 (-43.3198f, -4.0f, -56.7827f),
					  glm::vec3 (-53.3198f, -4.3f, -46.7827f), glm::vec3 (-38.3198f, -4.7f, -50.7827f),
					  glm::vec3 (-47.3198f, -4.1f, -53.7827f) };
	}

	void DrawObject () override {
		shader->setBool ("is_flame", true);
		shader->setBool ("is_flame_frag", true);

		for (size_t i = 0; i < positions.size (); i++) {
			glm::mat4 model = glm::mat4 (1.0f);
			model = glm::translate (model, positions[i]);
			model = glm::scale (model, glm::vec3 (2.0f, 2.0f, 2.0f));
			setModelMatrices (model);
			setMeshMaterial (ones3f, 2.0f);
			glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
			for (const auto& mesh : meshes) mesh.Draw ();
		}

		shader->setBool ("is_flame", false);
		shader->setBool ("is_flame_frag", false);
	}
private:
	std::vector <glm::vec3> positions;
};


class Stars : public ObjectBase {
public:
	Stars (const float* vertices, const int vertices_cnt,
		const unsigned int* indices, const int indices_cnt,
		std::shared_ptr<Shader> shader)
		: ObjectBase (vertices, vertices_cnt, indices, indices_cnt, shader) { }

	glm::vec3 position = glm::vec3 (7.2748f, 64.168f, 17.4503f);
	const int radius = 5;

	void DrawObject () override {
		float s = (float)getTimeSeed ();
		float angle = 0.2f;
		shader->setVec3 ("col", glm::vec3 (1.0f, 1.0f, 1.0f));

		for (int i = 0; i < star_count; i += 3) {
			glm::mat4 model = glm::mat4 (1.0f);
			int rad = i * 4 % 8 + 12;
			bool sign = i % 2 == 0 ? 1 : -1;
			float an = 0.1f * 2 / i * sign;
			model = glm::translate (
				model, glm::vec3 (star_positions[i] + glm::cos (an * s) * rad,
					star_positions[i + 1],
					star_positions[i + 2] + glm::sin (s * an) * rad));
			model = glm::scale (model, glm::vec3 (0.25f, 0.25f, 0.25f));
			setModelMatrices (model);
			for (const auto& mesh : meshes) {
				mesh.Draw ();
			}
		}

		glm::mat4 model = glm::mat4 (1.0f);
		model = glm::translate (
			model, glm::vec3 (this->position.x + glm::cos (angle * s) * radius,
				this->position.y,
				this->position.z + glm::sin (s * angle) * radius));
		model = glm::rotate (model, s * glm::radians (angle),
			glm::vec3 (1.0f, 0.0f, 1.0f));
		model = glm::scale (model, glm::vec3 (0.25f, 0.25f, 0.25f));
		setModelMatrices (model);

		for (const auto& mesh : meshes) {
			mesh.Draw ();
		}
	}

private:
	const int star_count = 117 / 3;
};

/*!
 *  LightSource class for representing lighting in the scene.
 */
class LightSource {
public:
	LightSource (const float* vertices, const int vertices_cnt,
		const unsigned int* indices, const int indices_cnt,
		std::shared_ptr<Shader> own_shader,
		std::shared_ptr<Shader> object_shader) {
		this->position = glm::vec3 (-33.3198f, 30.0f, -56.7827f);
		object_shader->use ();
		object_shader->setVec3 ("pointlight.ambient", glm::vec3 (0.2f, 0.15f, 0.05f));
		object_shader->setVec3 ("pointlight.diffuse", glm::vec3 (0.5f, 0.5f, 0.5f));  
		object_shader->setVec3 ("pointlight.specular", glm::vec3 (1.0f, 1.0f, 1.0f));
		object_shader->setVec3 ("pointlight.position", this->position);

		// constants used for point light attenuation
		object_shader->setFloat ("pointlight.constant", 0.5f);
		object_shader->setFloat ("pointlight.linear", 0.007);
		object_shader->setFloat ("pointlight.quadratic", 0.0002f);

		// direction is set with moon position
		object_shader->setVec3 ("dir_light.direction", glm::vec3 (12.0f + 0.5f * 140.0f, 500.0f, 5.0f +0.5f * 140.0f));
		float dir_intensity = 0.04f;
		object_shader->setVec3 ("dir_light.ambient", glm::vec3 (dir_intensity, dir_intensity, dir_intensity));
		object_shader->setVec3 ("dir_light.diffuse", glm::vec3 (dir_intensity, dir_intensity, dir_intensity));
		object_shader->setVec3 ("dir_light.specular", glm::vec3 (0.05f, 0.05f, 0.05f));
	}

private:
	glm::vec3 position;
};