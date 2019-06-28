#ifndef CARROTDEFECT_H
#define CARROTDEFECT_H

#include <string>



class CarrotDefect
{
public:
    bool LargeBlackPatches;
    bool SmallBlackPatches;
    bool BlackEnd;
    bool RotEnd;
    bool RotOutEnd;
    bool WitheFungus;
    bool GreenHead;
    bool CrackTravsverse;
    bool CrackLongitudial;
    bool Curvature;
    bool Split;
    bool OtherShapeDeformation;
    bool BiteInsect;
    bool BiteAnimal;
    bool Soilure;
    void Init();
    void ReadFromFile(std::string FileName);
    CarrotDefect();
};

#endif // CARROTDEFECT_H
