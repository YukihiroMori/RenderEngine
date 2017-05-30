#include "Main.h"
#include "Shader.h"
#include "Paths.h"
#include "Mesh.h"
#include "ObjShader.h"
#include "Obj.h"
#include "AABB.h"
#include "Transform.h"

class ObjMesh : public Mesh
{
private:
	GLuint vbo[2];

public:
	Transform transform;
	AABB aabb;

	~ObjMesh(){
		use();

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDeleteBuffers(2 , vbo);

		unuse();
	};

	ObjMesh(const char *name, bool normalaize = false) 
		: Mesh(),
		transform()
		{

		vector<vec3> pos, norm;

		if (LoadObj(name, group, pos, norm , normalaize, aabb)) {

			use();

			glGenBuffers(2 ,vbo);
			
			glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * pos.size(), &pos[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			

			glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * norm.size(), &norm[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

			unuse();

		}
	}

	void attachShader(Shader &shader) {
		this->shader = &shader;
	}

	void attachShader(Shader *shader) {
		this->shader = shader;
	}

	void draw() {
		shader->UpdateUniform("ModelMatrix", transform.asMatrix());

		shader->use();
		use();

		for (unsigned int g = 0; g < group.size(); ++g)
		{
			Obj::mat mat = group[g].m;

			shader->UpdateUniform("kamb", mat.amb);
			shader->UpdateUniform("kdiff", mat.diff);
			shader->UpdateUniform("kspec", mat.spec);
			shader->UpdateUniform("kshi", mat.shi);

			glDrawArrays(GL_TRIANGLES, group[g].b , group[g].c);
		}

		unuse();
		shader->unuse();

	}

	void attachShadowShader(Shader &shader) {
		this->shadow = &shader;
	}

	void attachShadowShader(Shader *shader) {
		this->shadow = shader;
	}

	void shadow_pass() {
		shadow->UpdateUniform("ModelMatrix", transform.asMatrix());

		shadow->use();
		use();

		for (unsigned int g = 0; g < group.size(); ++g)
		{
			Obj::mat mat = group[g].m;
			glDrawArrays(GL_TRIANGLES, group[g].b, group[g].c);
		}

		unuse();
		shader->unuse();
	}
	
private:
	vector<Obj::grp> group;
	Shader *shader;
	Shader *shadow;
};