#include "_objLoader.h"

_objLoader::_objLoader()
{
    //ctor
}

_objLoader::~_objLoader()
{
    //dtor
}

bool _objLoader::loadOBJ(const char* path, const char* mtl)
{
    vector< unsigned int > vertexIndices, uvIndices, normalIndices;
    vector< vec3 > temp_vertices;
    vector< vec2 > temp_uvs;
    vector< vec3 > temp_normals;

    vector< vector<vec3> > vert_groups;
    vector< vector<vec2> > uv_groups;
    vector< vector<vec3> > normal_groups;

    FILE *mtlFile = fopen(mtl,"r");

    //groupIndex.push_back(0);

    FILE * file = fopen(path, "r");
    if( file == NULL ){
        printf("Impossible to open the file !\n");
        return false;
    }

    while( 1 ){

    char lineHeader[128];
    // read the first word of the line
    int res = fscanf(file, "%s", lineHeader);
    if (res == EOF)
        break; // EOF = End Of File. Quit the loop.

    // else : parse lineHeader

        if ( strcmp( lineHeader, "v" ) == 0 ){
    vec3 vertex;
    fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
    temp_vertices.push_back(vertex);

        }else if ( strcmp( lineHeader, "vt" ) == 0 ){
    vec2 uv;
    fscanf(file, "%f %f\n", &uv.x, &uv.y );
    temp_uvs.push_back(uv);

        }else if ( strcmp( lineHeader, "vn" ) == 0 ){
    vec3 normal;
    fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
    temp_normals.push_back(normal);

        }else if ( strcmp( lineHeader, "f" ) == 0 ){
    std::string vertex1, vertex2, vertex3;
    unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
    int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
    if (matches != 9){
        printf("File can't be read by our simple parser : ( Try exporting with other options\n");
        return false;
    }
    vertexIndices.push_back(vertexIndex[0]);
    vertexIndices.push_back(vertexIndex[1]);
    vertexIndices.push_back(vertexIndex[2]);
    uvIndices    .push_back(uvIndex[0]);
    uvIndices    .push_back(uvIndex[1]);
    uvIndices    .push_back(uvIndex[2]);
    normalIndices.push_back(normalIndex[0]);
    normalIndices.push_back(normalIndex[1]);
    normalIndices.push_back(normalIndex[2]);
        }else if(strcmp(lineHeader,"usemtl") == 0) {
            groupIndex.push_back(vertexIndices.size());
            char name[128];
            fscanf(file,"%s\n",name);
            while (1) {
                //char lineHeader[128];
                int res = fscanf(mtlFile,"%s",lineHeader);
                if (res == EOF) break;
                if (strcmp(lineHeader,"newmtl") == 0) {
                    material mat;
                    fscanf(mtlFile,"%s\n",lineHeader);
                    if (strcmp(name,lineHeader) != 0) continue;
                    strcpy(mat.name,name);
                    fscanf(mtlFile,"%s %f\n",lineHeader,&mat.Ns);
                    fscanf(mtlFile,"%s %f %f %f\n",lineHeader,&mat.Ka.x,&mat.Ka.y,&mat.Ka.z);
                    fscanf(mtlFile,"%s %f %f %f\n",lineHeader,&mat.Kd.x,&mat.Kd.y,&mat.Kd.z);
                    fscanf(mtlFile,"%s %f %f %f\n",lineHeader,&mat.Ks.x,&mat.Ks.y,&mat.Ks.z);
                    fscanf(mtlFile,"%s %f %f %f\n",lineHeader,&mat.Ke.x,&mat.Ke.y,&mat.Ke.z);
                    fscanf(mtlFile,"%s %f\n",lineHeader,&mat.Ni);
                    fscanf(mtlFile,"%s %d\n",lineHeader,&mat.illum);
                    mats.push_back(mat);
                }
            }
            fseek(mtlFile,0,SEEK_SET);
        }
    }

    for ( int i = 0; i < vertexIndices.size(); i++ ) {
        unsigned int vertexIndex = vertexIndices[i];
        vec3 vertex = temp_vertices[vertexIndex-1];
        vertices.push_back(vertex);
    }

    for (int i = 0; i < uvIndices.size(); i++ ) {
        unsigned int uvIndex = uvIndices[i];
        vec2 uv = temp_uvs[uvIndex - 1];
        uvs.push_back(uv);
    }

    for ( int i = 0; i < normalIndices.size(); i++ ) {
        unsigned int normalIndex = normalIndices[i];
        vec3 normal = temp_normals[normalIndex-1];
        normals.push_back(normal);
    }

    myTex->loadTexture("models/car/Tex.png");
    tex_id = myTex->textID;
}

void _objLoader::drawOBJ()
{
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK,GL_SPECULAR);

    glColor3f(0.1,0.2,0.1);
    //glDisable(GL_TEXTURE_2D);
    int matIndex = 0;

    glBegin(GL_TRIANGLES);
        for (int i = 0; i < vertices.size(); i++) {
            if (i == groupIndex[matIndex]) {
                GLfloat specular[4] = { mats[matIndex].Ks.x,mats[matIndex].Ks.y,mats[matIndex].Ks.z,1.0 };
                GLfloat diffuse[4] = { mats[matIndex].Kd.x,mats[matIndex].Kd.y,mats[matIndex].Kd.z,1.0 };
                GLfloat ambient[4] = { mats[matIndex].Ka.x,mats[matIndex].Ka.y,mats[matIndex].Ka.z,1.0 };

                glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,specular);
                glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,ambient);
                glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,diffuse);

                matIndex++;
            }
            glTexCoord2f(uvs[i].x,uvs[i].y);
            glNormal3f(normals[i].x,normals[i].y,normals[i].z);
            v[0] = vertices[i].x;
            v[1] = vertices[i].y;
            v[2] = vertices[i].z;
            glVertex3fv(v);
        }
    glEnd();
    glDisable(GL_COLOR_MATERIAL);
    glEnable(GL_TEXTURE_2D);
    glClear(GL_COLOR_MATERIAL);

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS,high_shininess);
}
