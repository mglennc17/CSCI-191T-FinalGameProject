#ifndef _OBJLOADER_H
#define _OBJLOADER_H

#include <_common.h>
#include <_textureloader.h>

typedef float vec3_t[3];

struct material{
    char name[100];
    float Ns;
    vec3 Ka;
    vec3 Kd;
    vec3 Ks;
    vec3 Ke;
    float Ni;
    float d;
    int illum;
};

class _objLoader
{
    public:
        _objLoader();
        virtual ~_objLoader();

        vector<vector<vec3>> vertex_groups;
        vector<vector<vec2>> uv_groups;
        vector<vector<vec3>> normal_groups;
        vector<material> mats;

        vector<vec3> vertices;
        vector<vec2> uvs;
        vector<vec3> normals;
        vector<int> groupIndex;

        _textureLoader *myTex = new _textureLoader();
        GLuint tex_id;
        vec3_t v;

        bool loadOBJ(const char *, const char *);
        void drawOBJ();

        const float mat_ambient[4] = {0.7,0.7,0.7,1.0};
        const float mat_diffuse[4] = {0.8,0.8,0.8,1.0};
        const float mat_specular[4]= {1.0,1.0,1.0,1.0};
        const float high_shininess[2]= {100,0.0};

    protected:

    private:
};

#endif // _OBJLOADER_H
