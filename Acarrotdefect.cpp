#include "carrotdefect.h"


#include <boost/filesystem.hpp>
#include <boost/regex.hpp>


using namespace std;
using namespace boost::filesystem;
using namespace boost;

CarrotDefect::CarrotDefect()
{
    Init();
}

void CarrotDefect::Init()
{
    LargeBlackPatches = false;
    SmallBlackPatches = false;
    BlackEnd = false;
    RotEnd = false;
    RotOutEnd = false;
    WitheFungus = false;
    GreenHead = false;
    CrackTravsverse = false;
    CrackLongitudial = false;
    Curvature = false;
    Split = false;
    OtherShapeDeformation = false;
    BiteInsect = false;
    BiteAnimal = false;
    Soilure = false;
}

void CarrotDefect::ReadFromFile(std::string FileName)
{
    path File = FileName;

    if (!exists(File))
        return;
    std::ifstream inFile(File.string());
    if (!inFile.is_open())
    {
        return;
    }
    // ------------------read params from file-----------------------------

    string Line;

    vector<string> StrVect;


    while(inFile.good())
    {
        getline(inFile, Line);
        for(int i = 0; i < 15; i++)
        {
            regex LinePattern("LargeBlackPatches.+");
            if (regex_match(Line.c_str(), LinePattern))
            {
                if(Line.substr(20,1) == "1")
                    LargeBlackPatches = true;
                break;
            }
        }

    }
    inFile.close();
    int lineCount = StrVect.size();

    for(int line = 0; line < lineCount; line++)
    {
        regex LinePattern("LargeBlackPatches.+");
        if (regex_match(Line.c_str(), LinePattern))
        {
            if(Line.substr(20,1) == "1")
                LargeBlackPatches = true;
            break;
        }
    }

    for(int line = 0; line < lineCount; line++)
    {
        regex LinePattern("SmallBlackPatches.+");
        if (regex_match(Line.c_str(), LinePattern))
        {
            if(Line.substr(20,1) == "1")
                SmallBlackPatches = true;
            break;
        }
    }

    for(int line = 0; line < lineCount; line++)
    {
        regex LinePattern("BlackEnd.+");
        if (regex_match(Line.c_str(), LinePattern))
        {
            if(Line.substr(20,1) == "1")
                BlackEnd = true;
            break;
        }
    }

    for(int line = 0; line < lineCount; line++)
    {
        regex LinePattern("LargeBlackPatches.+");
        if (regex_match(Line.c_str(), LinePattern))
        {
            if(Line.substr(20,1) == "1")
                LargeBlackPatches = true;
            break;
        }
    }


    for(int line = 0; line < lineCount; line++)
    {
        regex LinePattern("LargeBlackPatches.+");
        if (regex_match(Line.c_str(), LinePattern))
        {
            if(Line.substr(20,1) == "1")
                LargeBlackPatches = true;
            break;
        }
    }


    for(int line = 0; line < lineCount; line++)
    {
        regex LinePattern("LargeBlackPatches.+");
        if (regex_match(Line.c_str(), LinePattern))
        {
            if(Line.substr(20,1) == "1")
                LargeBlackPatches = true;
            break;
        }
    }


    for(int line = 0; line < lineCount; line++)
    {
        regex LinePattern("LargeBlackPatches.+");
        if (regex_match(Line.c_str(), LinePattern))
        {
            if(Line.substr(20,1) == "1")
                LargeBlackPatches = true;
            break;
        }
    }


    for(int line = 0; line < lineCount; line++)
    {
        regex LinePattern("LargeBlackPatches.+");
        if (regex_match(Line.c_str(), LinePattern))
        {
            if(Line.substr(20,1) == "1")
                LargeBlackPatches = true;
            break;
        }
    }


    for(int line = 0; line < lineCount; line++)
    {
        regex LinePattern("LargeBlackPatches.+");
        if (regex_match(Line.c_str(), LinePattern))
        {
            if(Line.substr(20,1) == "1")
                LargeBlackPatches = true;
            break;
        }
    }


    for(int line = 0; line < lineCount; line++)
    {
        regex LinePattern("LargeBlackPatches.+");
        if (regex_match(Line.c_str(), LinePattern))
        {
            if(Line.substr(20,1) == "1")
                LargeBlackPatches = true;
            break;
        }
    }


    for(int line = 0; line < lineCount; line++)
    {
        regex LinePattern("LargeBlackPatches.+");
        if (regex_match(Line.c_str(), LinePattern))
        {
            if(Line.substr(20,1) == "1")
                LargeBlackPatches = true;
            break;
        }
    }


    for(int line = 0; line < lineCount; line++)
    {
        regex LinePattern("LargeBlackPatches.+");
        if (regex_match(Line.c_str(), LinePattern))
        {
            if(Line.substr(20,1) == "1")
                LargeBlackPatches = true;
            break;
        }
    }


    for(int line = 0; line < lineCount; line++)
    {
        regex LinePattern("LargeBlackPatches.+");
        if (regex_match(Line.c_str(), LinePattern))
        {
            if(Line.substr(20,1) == "1")
                LargeBlackPatches = true;
            break;
        }
    }


    for(int line = 0; line < lineCount; line++)
    {
        regex LinePattern("LargeBlackPatches.+");
        if (regex_match(Line.c_str(), LinePattern))
        {
            if(Line.substr(20,1) == "1")
                LargeBlackPatches = true;
            break;
        }
    }


    for(int line = 0; line < lineCount; line++)
    {
        regex LinePattern("LargeBlackPatches.+");
        if (regex_match(Line.c_str(), LinePattern))
        {
            if(Line.substr(20,1) == "1")
                LargeBlackPatches = true;
            break;
        }
    }




    StrVect.clear();
    return;

}
