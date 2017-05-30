#include "Main.h"
#include "Shader.h"
#include "Paths.h"
#include "Mesh.h"
#include "ObjShader.h"
#include "Obj.h"
#include "Transform.h"

class ObjMesh : public Mesh
{
private:
	GLuint vbo[2];

public:
	Transform transform;

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

		if (LoadObj(name, group, pos, norm , normalaize)) {

			use();

			glGenBuffers(2 ,vbo);
			
			glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * pos.size(), &pos[0], GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(0);

			glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * norm.size(), &norm[0], GL_STATIC_DRAW);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(1);

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

	int face_size;
	

	bool LoadObj(const char *name, vector<Obj::grp> &group, vector<vec3> &pos, vector<vec3> &norm, bool normalized) {
		group = {};
		int nv = 0;
		pos = norm = {};

		std::string path(ObjResourcePath);
		path += name;
		const size_t base = path.find_last_of("/\\");
		string dirname = (base == string::npos) ? "" : path.substr(0 , base + 1);

		ifstream file(path.c_str());
		if (file.fail()) {
			cerr << "Error: Can't open OBJ file: " << path << endl;
			return false;
		}

		map<string, Obj::mat> mtl;
		static const char *defmtl = "Default";
		string mtlname(defmtl);

		Obj::mat default(
			vec4(0.1f, 0.1f, 0.1f, 1.0f),
			vec4(0.6f, 0.6f, 0.6f, 1.0f),
			vec4(0.3f, 0.3f, 0.3f, 1.0f),
			60.0f,1.0f);

		mtl[mtlname] = default;

		vector<vec3> _pos;
		vector<vec3> _tex;
		vector<vec3> _norm;
		vector<Obj::face> _face;
		vector<Obj::grp> _group;

		int groupbegin = 0;

		bool smooth = false;

		float xmin, xmax, ymin, ymax, zmin, zmax;
		xmax = ymax = zmax = -(xmin = ymin = zmin = FLT_MAX);

		string line;
		while (getline(file, line)) {
			if (*(line.end() - 1) == '\r') line.erase(--line.end());

			istringstream str(line);

			string token;
			str >> token;

			if (token == "v") {
				vec3 v;

				str >> v.x >> v.y >> v.z;

				xmin = std::min(xmin, v.x);
				xmax = std::max(xmax, v.x);
				ymin = std::min(ymin, v.y);
				ymax = std::max(ymax, v.y);
				zmin = std::min(zmin, v.z);
				zmax = std::max(zmax, v.z);

				_pos.push_back(v);
			}
			else if (token == "vt") {
				vec3 t;

				str >> t.x >> t.y;
				if (!str.eof()) {
					str >> t.z;
				}
				else
					t.z = 0.0f;

				_tex.push_back(t);
			}
			else if (token == "vn") {
				vec3 n;

				str >> n.x >> n.y >> n.z;

				_norm.push_back(n);
			}
			else if (token == "f") {
				Obj::face f;

				f.p = { 0,0,0 };
				f.n = { 0,0,0 };
				f.t = { 0,0,0 };

				f.smooth = smooth;

				for (int i = 0; i < 3; i++) {
					string s;
					str >> s;

					f.p[i] = atoi(s.c_str());
					size_t l = s.find('/', 0);
					if (l != string::npos) {
						++l;

						f.t[i] = atoi(s.c_str() + l);

						l = s.find('/', l);

						if (l != string::npos) {
							++l;

							f.n[i] = atoi(s.c_str() + l);
						}
					}
				}

				_face.push_back(f);
			}
			else if (token == "s") {
				string s;
				str >> s;
				smooth = s == "1";
			}
			else if (token == "usemtl") {
				int groupcount = static_cast<int>(_face.size()) * 3 - groupbegin;
				if (groupcount > 0) {
					Obj::grp b(mtlname, groupbegin, groupcount, mtl[mtlname]);
					_group.push_back(b);

					groupbegin += groupcount;
				}

				str >> mtlname;

				if (mtl.find(mtlname) == mtl.end()) {
					cerr << "Warning: Undefined material: " << mtlname << endl;
					mtlname = defmtl;
				}
				else {
					cerr << "usemtl: " << mtlname << endl;
				}
			}
			else if (token == "mtllib") {
				str >> std::ws;
				string mtlpath;
				getline(str, mtlpath);
				mtlpath = dirname + mtlpath;

				ifstream mtlfile(mtlpath.c_str(), ios::binary);
				if (mtlfile.fail()) {
					cerr << "Warning: Can't open MTL file: " << mtlpath << endl;
				}
				else {
					string mtlline;

					while (getline(mtlfile, mtlline)) {
						istringstream mtlstr(mtlline);
						string mtltoken;
						mtlstr >> mtltoken;

						if (mtltoken == "newmtl")
						{
							mtlstr >> mtlname;
							cerr << "newmtl: " << mtlname << endl;
							mtl[mtlname] = default;
						}
						else if (mtltoken == "Ka")
						{
							mtlstr >> mtl[mtlname].amb.r >> mtl[mtlname].amb.g >> mtl[mtlname].amb.b;
							//cout << " Ka " << to_string(mtl[mtlname].amb) << endl;
						}
						else if (mtltoken == "Kd")
						{
							mtlstr >> mtl[mtlname].diff.r >> mtl[mtlname].diff.g >> mtl[mtlname].diff.b;
							//cout << "Kd " << to_string(mtl[mtlname].diff) << endl;
						}
						else if (mtltoken == "Ks")
						{
							mtlstr >> mtl[mtlname].spec.r >> mtl[mtlname].spec.g >> mtl[mtlname].spec.b;
							//cout << "Ks " << to_string(mtl[mtlname].spec) << endl;
						}
						else if (mtltoken == "Ns")
						{
							mtlstr >> mtl[mtlname].shi;
							//cout << "Ns " << to_string(mtl[mtlname].shi) << endl;
						}
						else if (mtltoken == "d")
						{
							mtlstr >> mtl[mtlname].dis;
							//cout << "d " << to_string(mtl[mtlname].dis) << endl;
						}
					}

					if (mtlfile.bad())
					{
						cerr << "Warning: Can't read MTL file: " << mtlpath << endl;
					}
					mtlfile.close();
				}
			}
		}

		if (file.bad()) {
			cerr << "Warning: Can't read OBJ file: " << path << endl;
			file.close();
			return false;
		}

		file.close();

		int groupcount = static_cast<int>(_face.size()) * 3 - groupbegin;
		if (groupcount > 0)
		{
			Obj::grp b(mtlname,groupbegin, groupcount, mtl[mtlname]);
			_group.push_back(b);
		}

		int nf = static_cast<int>(_face.size());
		nv = nf * 3;
		int ng = static_cast<int>(_group.size());

		try {
			group = vector<Obj::grp>(ng);
			pos = vector<vec3>(nv);
			norm = vector<vec3>(nv);
		}
		catch (bad_alloc e) {
			group.clear();
			pos.clear();

			group = {};
			nv = 0;
			pos = norm = {};

			return false;
		}

		float scale;
		vec3 c;
		if (normalized)
		{
			float sx = xmax - xmin;
			float sy = ymax - ymin;
			float sz = zmax - zmin;
			scale = sx;
			if (sy > scale) scale = sy;
			if (sz > scale) scale = sz;
			scale = (scale != 0.0f) ? 2.0f / scale : 1.0f;
			c.x = (xmax + xmin) * 0.5f;
			c.y = (ymax + ymin) * 0.5f;
			c.z = (zmax + zmin) * 0.5f;
		}
		else {
			scale = 1.0f;
			c.x = c.y = c.z = 0.0f;
		}


		if (_norm.empty())
		{
			static const vec3 zero = vec3(0.0f, 0.0f, 0.0f);
			_norm.resize(_pos.size(), zero);

			for (vector<Obj::face>::iterator it = _face.begin(); it != _face.end(); ++it)
			{
				int v0 = it->p[0] - 1;
				int v1 = it->p[1] - 1;
				int v2 = it->p[2] - 1;

				const vec3 & p0 = _pos[v0];
				const vec3 & p1 = _pos[v1];
				const vec3 & p2 = _pos[v2];

				vec3 a = p1 - p0;
				vec3 b = p2 - p0;
				vec3 n = cross(a,b);

				if (it->smooth) {
					_norm[v0] += n;
					_norm[v1] += n;
					_norm[v2] += n;

					it->n[0] = it->p[0];
					it->n[1] = it->p[1];
					it->n[2] = it->p[2];
				}
				else {
					n = normalize(n);

					const size_t v = _norm.size();
					_norm.resize(v + 3);

					_norm[v + 0] = n;
					_norm[v + 1] = n;
					_norm[v + 2] = n;

					it->n[0] = v + 1;
					it->n[1] = v + 2;
					it->n[2] = v + 3;
				}
			}
		}

		int id = 0;
		for (Obj::face f : _face)
		{

			for (unsigned int i = 0; i < f.p.size(); i++) {

				pos[id] = (_pos[f.p[i] - 1] - c)* scale;

				if (f.smooth) {
					norm[id] = normalize(_norm[f.n[i] - 1]);
				}
				else {
					norm[id] = _norm[f.n[i] - 1];
				}

#if 0
				int t = _f.t[i] - 1;
				if (t > 0)
				{
					tex[id] = _tex[t];
				}
#endif
				id++;
			}
		}

		for (int i = 0; i < _group.size(); i++) {
			group[i].name = _group[i].name;
			group[i].b = _group[i].b;
			group[i].c = _group[i].c;
			group[i].m = _group[i].m;
		}

		face_size = _face.size();

		return true;
	}
};