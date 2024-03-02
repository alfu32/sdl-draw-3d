#ifndef __RLIB_RLMODELS_EXTRAS__
#define __RLIB_RLMODELS_EXTRAS__
#include <raylib.h>
#include <rlgl.h>


// Draw a grid centered at (0, 0, 0)
void DrawGridAt(Vector3 base,int slices, float spacing)
{
    int halfSlices = slices/2;

    rlBegin(RL_LINES);
        for (int i = -halfSlices; i <= halfSlices; i++)
        {
            if (i == 0)
            {
                rlColor3f(0.0f, 0.5f, 0.5f);
                rlColor3f(0.0f, 0.5f, 0.5f);
                rlColor3f(0.0f, 0.5f, 0.5f);
                rlColor3f(0.5f, 0.5f, 0.5f);
            }
            else
            {
                rlColor3f(0.75f, 0.75f, 0.75f);
                rlColor3f(0.75f, 0.75f, 0.75f);
                rlColor3f(0.75f, 0.75f, 0.75f);
                rlColor3f(0.75f, 0.75f, 0.75f);
            }

            rlVertex3f(base.x+(float)i*spacing, base.y+0.0f, base.z+(float)-halfSlices*spacing);
            rlVertex3f(base.x+(float)i*spacing, base.y+0.0f, base.z+(float)halfSlices*spacing);

            rlVertex3f(base.x+(float)-halfSlices*spacing, base.y+0.0f, base.z+(float)i*spacing);
            rlVertex3f(base.x+(float)halfSlices*spacing, base.y+0.0f, base.z+(float)i*spacing);
        }
    rlEnd();
}

#endif