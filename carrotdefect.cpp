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
    for(int i = 0; i<defectCount; i++)
    {
        defect[i] = false;
    }
}

void CarrotDefect::Load(string FileName)
{
    Init();
    path File = FileName;

    if (!exists(File))
        return;
    std::ifstream inFile(File.string());
    if (!inFile.is_open())
    {
        return;
    }
    string Line;
    while(inFile.good())
    {
        getline(inFile, Line);
        for(int i = 0; i < defectCount; i++)
        {
            regex LinePattern(DefectNames[i] + ".+");
            if (regex_match(Line.c_str(), LinePattern))
            {
               if(Line.substr(26,1) == "1")
                    defect[i] = true;
               break;
            }
        }
    }
    inFile.close();
}

void CarrotDefect::Save(std::string FileName)
{
    string OutDataString = "";

    for(int i = 0; i < defectCount; i++)
    {
        OutDataString += DefectNames[i];

        if (defect[i])
        {
            OutDataString += "1";
        }
        else
        {
            OutDataString += "0";
        }
        OutDataString += "\n";
    }
    std::ofstream out (FileName);
    out << OutDataString;
    out.close();
}

bool CarrotDefect::GetDefect(int defectNr)
{
    if (defectNr < defectCount && defectNr >= 0 )
        return defect[defectNr];
}

void CarrotDefect::SetDefect(int defectNr)
{

}
