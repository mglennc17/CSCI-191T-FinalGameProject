#ifndef _OBJLOADER_H
#define _OBJLOADER_H

#include <_common.h>

class _objLoader
{
    public:
        _objLoader();
        virtual ~_objLoader();

        bool loadOBJ(const char *, vector<vec3> &,vector<vec2> &,vector<vec3> &);


    protected:

    private:
};

#endif // _OBJLOADER_H
