#ifndef CARROTDEFECT_H
#define CARROTDEFECT_H

#include <string>

const int defectCount = 15;

const std::string DefectNames[defectCount] = {
    "LargeBlackPatches       - ",
    "SmallBlackPatches       - ",
    "BlackEnd                - ",
    "RotEnd                  - ",
    "RotOutEnd               - ",
    "WhiteFungus             - ",
    "GreenHead               - ",
    "CrackTravsverse         - ",
    "CrackLongitudial        - ",
    "Curvature               - ",
    "Split                   - ",
    "OtherShapeDeformation   - ",
    "BiteInsect              - ",
    "BiteAnimal              - ",
    "Soilure                 - "
};

class CarrotDefect
{
public:
    bool defect[defectCount];
    CarrotDefect();
    bool GetDefect(int defectNr);
    void SetDefect(int defectNr);
    void Init();
    void Load(std::string FileName);
    void Save(std::string FileName);
};

#endif // CARROTDEFECT_H
