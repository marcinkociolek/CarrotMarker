#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>

#include <string>

#include <boost/filesystem.hpp>
#include <boost/regex.hpp>


#include <math.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "NormalizationLib.h"
#include "DispLib.h"
#include "histograms.h"
#include "RegionU16Lib.h"

#include "mazdaroi.h"
#include "mazdaroiio.h"

#include <tiffio.h>

#include "mazdaroi.h"
#include "mazdaroiio.h"
typedef MazdaRoi<unsigned int, 2> MR2DType;

using namespace boost;
using namespace std;
using namespace boost::filesystem;
using namespace cv;
/*
//------------------------------------------------------------------------------------------------------------------------------
string MatPropetiesAsText(Mat Im)
{
    string Out ="Image properties: ";
    Out += "max x = " + to_string(Im.cols);
    Out += ", max y = " + to_string(Im.rows);
    Out += ", # channels = " + to_string(Im.channels());
    Out += ", depth = " + to_string(Im.depth());

    switch(Im.depth())
    {
    case CV_8U:
        Out += "CV_8U";
        break;
    case CV_8S:
        Out += "CV_8S";
        break;
    case CV_16U:
        Out += "CV_16U";
        break;
    case CV_16S:
        Out += "CV_16S";
        break;
    case CV_32S:
        Out += "CV_32S";
        break;
    case CV_32F:
        Out += "CV_32F";
        break;
    case CV_64F:
        Out += "CV_64F";
        break;
    default:
        Out += " unrecognized ";
    break;
    }
    return Out;
}
//------------------------------------------------------------------------------------------------------------------------------
string TiffFilePropetiesAsText(string FileName)
{
    float xRes,yRes;
    uint32 imWidth, imLength;
    uint16 resolutionUnit;
    TIFF *tifIm = TIFFOpen(FileName.c_str(),"r");
    string Out ="Tiff properties: ";
    if(tifIm)
    {
        TIFFGetField(tifIm, TIFFTAG_XRESOLUTION , &xRes);
        TIFFGetField(tifIm, TIFFTAG_YRESOLUTION , &yRes);
        TIFFGetField(tifIm, TIFFTAG_IMAGEWIDTH , &imWidth);
        TIFFGetField(tifIm, TIFFTAG_IMAGELENGTH , &imLength);
        TIFFGetField(tifIm, TIFFTAG_RESOLUTIONUNIT , &resolutionUnit);


        Out += "max x = " + to_string(imLength);
        Out += ", max y = " + to_string(imWidth);
        Out += ", ResUnit = " + to_string(resolutionUnit);
        Out += ", xRes = " + to_string(1.0/xRes);
        Out += ", yRes = " + to_string(1.0/yRes);
        TIFFClose(tifIm);
    }
    else
        Out += " improper file ";
    //TIFFGetField(tifIm, TIFFTAG_IMAGEWIDTH, &width);

    return Out;
}
//------------------------------------------------------------------------------------------------------------------------------
*/
bool GetTiffProperties(string FileName, float &xRes, float &yRes)
{
    //float xRes,yRes;
    //uint32 imWidth, imLength;
    //uint16 resolutionUnit;
    TIFF *tifIm = TIFFOpen(FileName.c_str(),"r");
    string Out ="Tiff properties: ";
    if(tifIm)
    {
        TIFFGetField(tifIm, TIFFTAG_XRESOLUTION , &xRes);
        TIFFGetField(tifIm, TIFFTAG_YRESOLUTION , &yRes);
        //TIFFGetField(tifIm, TIFFTAG_IMAGEWIDTH , &imWidth);
        //TIFFGetField(tifIm, TIFFTAG_IMAGELENGTH , &imLength);
        //TIFFGetField(tifIm, TIFFTAG_RESOLUTIONUNIT , &resolutionUnit);

        TIFFClose(tifIm);
        return 1;
    }
    else
    {
        xRes = 1.0;
        yRes = 1.0;
        return 0;
    }
}
//------------------------------------------------------------------------------------------------------------------------------
cv::Mat SegmetBlue(cv::Mat ImIn, unsigned char threshold)
{
    if(ImIn.empty())
        return Mat::zeros(1,1,CV_16U);
    int maxX = ImIn.cols;
    int maxY = ImIn.rows;
    int maxXY = maxX * maxY;

    unsigned char *wImInB = (unsigned char*)ImIn.data;
    unsigned char *wImInG = wImInB + 1;
    unsigned char *wImInR = wImInB + 2;

    Mat Mask = Mat::zeros(maxY, maxX, CV_16U);
    unsigned short *wMask = (unsigned short*)Mask.data;

    for(int i = 0; i < maxXY; i++)
    {
        if(*wImInB < threshold || *wImInB < *wImInG || *wImInB < *wImInR)
            *wMask = 1;
        wImInG+= 3;
        wImInB+= 3;
        wImInR+= 3;

        wMask++;
    }
    return Mask;
}
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//          constructor Destructor
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    FileName = "";
    FileNameTxt = "";

    ui->comboBoxOutputMode->addItem("Gray");
    ui->comboBoxOutputMode->addItem("B");
    ui->comboBoxOutputMode->addItem("G");
    ui->comboBoxOutputMode->addItem("R");
    ui->comboBoxOutputMode->setCurrentIndex(1);

    MazdaFolder = "E:/PortableSoft/qmazda1902_win64";
    ui->lineEditMaZdaFolder->setText(QString::fromStdString(MazdaFolder.string()));
    MazdaOptionsFile = "I:/Marchew/MaZdaOpions01.txt";
    ui->lineEditOptionsFile->setText(QString::fromStdString(MazdaOptionsFile.string()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//          CLASS FUNCTIONS
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
void MainWindow::OpenImageFolder()
{
    if (!exists(ImageFolder))
    {
        ui->textEditOut->append(QString::fromStdString(" Image folder : " + ImageFolder.string()+ " not exists "));
        ImageFolder = "d:\\";
    }
    if (!is_directory(ImageFolder))
    {
        ui->textEditOut->append(QString::fromStdString(" Image folder : " + ImageFolder.string()+ " This is not a directory path "));
        ImageFolder = "C:\\Data\\";
    }
    ui->lineEditImageFolder->setText(QString::fromStdString(ImageFolder.string()));
    ui->listWidgetImageFiles->clear();
    for (directory_entry& FileToProcess : directory_iterator(ImageFolder))
    {
        regex FilePattern(ui->lineEditRegexImageFile->text().toStdString());
        if (!regex_match(FileToProcess.path().filename().string().c_str(), FilePattern ))
            continue;
        path PathLocal = FileToProcess.path();
        if (!exists(PathLocal))
        {
            ui->textEditOut->append(QString::fromStdString(PathLocal.filename().string() + " File not exists" ));
            break;
        }
        ui->listWidgetImageFiles->addItem(PathLocal.filename().string().c_str());
    }

}
//------------------------------------------------------------------------------------------------------------------------------
void MainWindow::ReadImage()
{
    if(ui->checkBoxAutocleanOut->checkState())
        ui->textEditOut->clear();
    int flags;
    if(ui->checkBoxLoadAnydepth->checkState())
        flags = CV_LOAD_IMAGE_ANYDEPTH;
    else
        flags = IMREAD_COLOR;

    ImIn.release();
    ImOut.release();
    Mask.release();

    ImIn = imread(FileName, flags);
    if(ImIn.cols > ImIn.rows)
        rotate(ImIn, ImIn, cv::ROTATE_90_CLOCKWISE);

    int croppX = 0;
    int croppWidth = ImIn.cols-1;
    int croppY = ui->spinBoxCropY->value();
    int croppHeight = ImIn.rows - croppY * 2;
    ImIn(Rect(croppX,croppY,croppWidth,croppHeight)).copyTo(ImIn);

    if(ImIn.empty())
    {
        ui->textEditOut->append("improper file");
        return;
    }

    path FileNamePath(FileName);
    string extension = FileNamePath.extension().string();

    if((extension == ".tif" || extension == ".tiff") && ui->checkBoxShowTiffInfo->checkState())
        ui->textEditOut->append(QString::fromStdString(TiffFilePropetiesAsText(FileName)));

    if(ui->checkBoxShowMatInfo->checkState())
        ui->textEditOut->append(QString::fromStdString(MatPropetiesAsText(ImIn)));

    ProcessImage();
}
//------------------------------------------------------------------------------------------------------------------------------
void MainWindow::ShowsScaledImage(Mat Im, string ImWindowName, double dispScale)
{
    if(Im.empty())
    {
        ui->textEditOut->append(QString::fromStdString(string("Empty Image to show") + ImWindowName));
        return;
    }

    Mat ImToShow;


    ImToShow = Im.clone();

    displayScale = pow(double(ui->spinBoxScaleBase->value()), double(ui->spinBoxScalePower->value()));
    if (displayScale != 1.0)
        cv::resize(ImToShow,ImToShow,Size(), displayScale, displayScale, INTER_AREA);
    if(ui->checkBoxImRotate->checkState())
        rotate(ImToShow,ImToShow, ROTATE_90_CLOCKWISE);
    imshow(ImWindowName, ImToShow);

}
//------------------------------------------------------------------------------------------------------------------------------
void MainWindow::ShowImages()
{
    if(ui->checkBoxShowInput->checkState())
        ShowsScaledImage(ImIn, "Input Image", displayScale);
    if(ui->checkBoxShowMask->checkState())
        ShowsScaledImage(ShowRegion(Mask), "Mask", displayScale);
    if(ui->checkBoxShowOutput->checkState())
        ShowsScaledImage(ShowTransparentRegionOnImage(Mask, ImIn, ui->spinBoxTransparency->value()), "Output Image", displayScale);
}

//---------------------------------------------------------------------------------------------------
void MainWindow::ReadDefects()
{
    ui->checkBoxLargeBlackPatches->setChecked(defectList.GetDefect(0));
    ui->checkBoxSmallBlackPatches->setChecked(defectList.GetDefect(1));
    ui->checkBoxBlackEnd->setChecked(defectList.GetDefect(2));
    ui->checkBoxRotEnd->setChecked(defectList.GetDefect(3));
    ui->checkBoxRotOutEnd->setChecked(defectList.GetDefect(4));
    ui->checkBoxWhiteFungus->setChecked(defectList.GetDefect(5));
    ui->checkBoxGreenHead->setChecked(defectList.GetDefect(6));
    ui->checkBoxCrackTravsverse->setChecked(defectList.GetDefect(7));
    ui->checkBoxCrackLongitudial->setChecked(defectList.GetDefect(8));
    ui->checkBoxCurvature->setChecked(defectList.GetDefect(9));
    ui->checkBoxSplit->setChecked(defectList.GetDefect(10));
    ui->checkBoxOtherShapeDeformation->setChecked(defectList.GetDefect(11));
    ui->checkBoxBiteInsect->setChecked(defectList.GetDefect(12));
    ui->checkBoxBiteAnimal->setChecked(defectList.GetDefect(13));
    ui->checkBoxSoilure->setChecked(defectList.GetDefect(14));
}
//---------------------------------------------------------------------------------------------------
void MainWindow::SaveDefects()
{
    defectList.defect[0] =  ui->checkBoxLargeBlackPatches->checkState();
    defectList.defect[1] =  ui->checkBoxSmallBlackPatches->checkState();
    defectList.defect[2] =  ui->checkBoxBlackEnd->checkState();
    defectList.defect[3] =  ui->checkBoxRotEnd->checkState();
    defectList.defect[4] =  ui->checkBoxRotOutEnd->checkState();
    defectList.defect[5] =  ui->checkBoxWhiteFungus->checkState();
    defectList.defect[6] =  ui->checkBoxGreenHead->checkState();
    defectList.defect[7] =  ui->checkBoxCrackTravsverse->checkState();
    defectList.defect[8] =  ui->checkBoxCrackLongitudial->checkState();
    defectList.defect[9] =  ui->checkBoxCurvature->checkState();
    defectList.defect[10] =  ui->checkBoxSplit->checkState();
    defectList.defect[11] =  ui->checkBoxOtherShapeDeformation->checkState();
    defectList.defect[12] =  ui->checkBoxBiteInsect->checkState();
    defectList.defect[13] =  ui->checkBoxBiteAnimal->checkState();
    defectList.defect[14] =  ui->checkBoxSoilure->checkState();
}
//---------------------------------------------------------------------------------------------------
void MainWindow::ProcessImage()
{
    if(ImIn.empty())
    {
        return;
    }
/*
    Mat ImGrayTemp, ImB, ImG, ImR;
    if(ui->checkBoxLoadAnydepth->checkState())
    {
        ImGrayTemp = ImIn;
    }
    else
    {
        switch(ui->comboBoxOutputMode->currentIndex())
        {
            case 1:
            {
                Mat Planes[3];
                split(ImIn,Planes);
                ImGrayTemp = Planes[0];
            }
            break;

            case 2:
            {
                Mat Planes[3];
                split(ImIn,Planes);
                ImGrayTemp = Planes[1];
            }
            break;
            case 3:
            {
                Mat Planes[3];
                split(ImIn,Planes);
                ImGrayTemp = Planes[2];
            }
        break;
        default:
            cvtColor(ImIn, ImGrayTemp, CV_BGR2GRAY);
        break;
        }
    }

    ImGrayTemp.convertTo(ImGray,CV_16U);
    Mask = Threshold16(ImGray, ui->spinBoxThreshold->value());
*/
    Mask = SegmetBlue(ImIn, ui->spinBoxThreshold->value());

    ShowImages();

}
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//          Slots
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------

void MainWindow::on_pushButtonOpenImageFolder_clicked()
{
    QFileDialog dialog(this, "Open Folder");
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setDirectory(QString::fromStdString(ImageFolder.string()));

    if(dialog.exec())
    {
        ImageFolder = dialog.directory().path().toStdWString();
    }
    else
        return;
    OpenImageFolder();
}

void MainWindow::on_listWidgetImageFiles_currentTextChanged(const QString &currentText)
{
    if(FileNameTxt != "")
    {
        SaveDefects();
        defectList.Save(FileNameTxt);
    }
    path fileToOpen = ImageFolder;
    fileToOpen.append(currentText.toStdString());
    FileName = fileToOpen.string();
    ReadImage();
    path fileToOpenTxt = ImageFolder;
    fileToOpenTxt.append(fileToOpen.stem().string() +".txt");
    FileNameTxt = fileToOpenTxt.string();
    ui->textEditOut->append(QString::fromStdString(FileNameTxt));
    defectList.Load(FileNameTxt);
    ReadDefects();
}

void MainWindow::on_lineEditRegexImageFile_returnPressed()
{
    OpenImageFolder();
}

void MainWindow::on_checkBoxShowInput_toggled(bool checked)
{
     ReadImage();
}

void MainWindow::on_checkBoxShowTiffInfo_toggled(bool checked)
{
     ReadImage();
}

void MainWindow::on_checkBoxAutocleanOut_toggled(bool checked)
{
     ReadImage();
}

void MainWindow::on_checkBoxShowMatInfo_toggled(bool checked)
{
     ReadImage();
}

void MainWindow::on_checkBoxLoadAnydepth_toggled(bool checked)
{
     ReadImage();
}

void MainWindow::on_spinBoxScaleBase_valueChanged(int arg1)
{
     ReadImage();
}

void MainWindow::on_spinBoxScalePower_valueChanged(int arg1)
{
     ReadImage();
}

void MainWindow::on_checkBoxImRotate_toggled(bool checked)
{
    ReadImage();
}

void MainWindow::on_spinBoxThreshold_valueChanged(int arg1)
{
    ProcessImage();
}

void MainWindow::on_spinBoxTransparency_valueChanged(int arg1)
{
    ShowImages();
}

void MainWindow::on_comboBoxOutputMode_currentIndexChanged(int index)
{
    ProcessImage();
}

void MainWindow::on_pushButtonOpenOutFolder_clicked()
{
    QFileDialog dialog(this, "Open Output Folder");
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setDirectory(QString::fromStdString(ImageFolder.string()));

    if(dialog.exec())
    {
        OutFolder = dialog.directory().path().toStdWString();
        ui->lineEditOutFolder->setText(QString::fromStdString(OutFolder.string()));
    }
    else
        return;
}

void MainWindow::on_pushButtonCreateMazdaFiles_clicked()
{
    string BatString = "";
    string MaZdaPath = MazdaFolder.string() + "/MzGenerator.exe";
    string MazdaOptionsFileName = MazdaOptionsFile.string();


    int filesCount = ui->listWidgetImageFiles ->count();
    int firstFile = 0;

    int defectNr = ui->spinBoxDefectNr->value();
    int defectCount = 0;

    string OutFileName = "";
    OutFileName += DefectNamesShort[defectNr] + path(MazdaOptionsFileName).stem().string();

    for(int fileNr = firstFile; fileNr< filesCount; fileNr++)
    {
        path fileToOpen = ImageFolder;
        fileToOpen.append(ui->listWidgetImageFiles->item(fileNr)->text().toStdString());
        FileName = fileToOpen.string();
        //path LocalFileToOpen = ImageFolder;
        //FileName = ui->listWidgetImageFiles->item(fileNr)->text().toStdString();
        //LocalFileToOpen.append(FileName);

        //ImIn = imread(LocalFileToOpen.string().c_str(),CV_LOAD_IMAGE_ANYDEPTH);

        ReadImage();
        path fileToOpenTxt = ImageFolder;
        fileToOpenTxt.append(fileToOpen.stem().string() +".txt");
        FileNameTxt = fileToOpenTxt.string();
        defectList.Load(FileNameTxt);
        string FileToSaveBase = fileToOpen.stem().string();

        path FileToSaveIm = OutFolder;
        FileToSaveIm.append(FileToSaveBase + ".tif");
        path FileToSaveMask = OutFolder;
        FileToSaveMask.append(FileToSaveBase + ".roi");

        imwrite(FileToSaveIm.string(), ImIn);

        unsigned short *wMask;
        vector <MR2DType*> ROIVect;

        int begin[MR2DType::Dimensions];
        int end[MR2DType::Dimensions];

        //Mask 1
        begin[0] = 0;
        begin[1] = 0;
        end[0] = Mask.cols-1;
        end[1] = Mask.rows-1;

        MR2DType *ROI;
        ROI = new MR2DType(begin, end);

        MazdaRoiIterator<MR2DType> iterator(ROI);
        wMask = (unsigned short*)Mask.data;
        while(! iterator.IsBehind())
        {
            if (*wMask)
                iterator.SetPixel();
            ++iterator;
            wMask++;
        }


        if(defectList.GetDefect(defectNr))
        {
            ROI->SetName(DefectNamesShort[defectNr]);
            ROI->SetColor(0xff0000);
            defectCount++;
        }
        else
        {
            ROI->SetName("NoDefect");
            ROI->SetColor(0x00ff00);
        }

        ROIVect.push_back(ROI);

        MazdaRoiIO<MR2DType>::Write(FileToSaveMask.string(), &ROIVect, NULL);

        while(ROIVect.size() > 0)
        {
             delete ROIVect.back();
             ROIVect.pop_back();
        }



        //FileName = fileToOpen.string();
        BatString += MaZdaPath;
        BatString += " -m roi -i ";
        BatString += FileToSaveIm.stem().string() + ".tif";
        BatString += " -r ";
        BatString += FileToSaveMask.stem().string() + ".roi";

        if(fileNr == firstFile)
        {
            BatString += "    -o ";
            BatString += OutFileName + ".csv";
            BatString += " -f ";
            BatString += MazdaOptionsFileName;
        }
        else
        {
            BatString += " -a -o ";
            BatString += OutFileName + ".csv";
        }
        BatString += "\n";


        waitKey(20);

    }
    string BatFileName = OutFolder.string() + "/" + OutFileName + ".bat";
    std::ofstream out ( BatFileName);
    out << BatString;
    out.close();

    ui->textEditOut->append("defect count " + QString::fromStdString(to_string( defectCount)));
}

void MainWindow::on_pushButtonSelectMazdaFolder_clicked()
{
    QFileDialog dialog(this, "Open MaZda Folder");
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setDirectory(QString::fromStdString(ImageFolder.string()));

    if(dialog.exec())
    {
        MazdaFolder = dialog.directory().path().toStdWString();
    }
    else
        return;
    if (!exists(MazdaFolder))
    {
        ui->textEditOut->append(QString::fromStdString(" Image folder : " + MazdaFolder.string()+ " not exists "));
        MazdaFolder = "C:/";
    }
    if (!is_directory(MazdaFolder))
    {
        ui->textEditOut->append(QString::fromStdString(" Image folder : " + MazdaFolder.string()+ " This is not a directory path "));
        MazdaFolder = "C:/";
    }
    ui->lineEditMaZdaFolder->setText(QString::fromStdString(MazdaFolder.string()));
}

void MainWindow::on_pushButtonSelectMazdaOptions_clicked()
{
    QFileDialog dialog(this, "Open MaZda Options File");
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setDirectory(QString::fromStdString(ImageFolder.string()));

    if(dialog.exec())
    {
        MazdaOptionsFile = dialog.directory().path().toStdWString();
    }
    else
        return;
    if (!exists(MazdaOptionsFile))
    {
        ui->textEditOut->append(QString::fromStdString(" file : " + MazdaOptionsFile.string()+ " not exists "));
        MazdaOptionsFile = "c:/temp.txt";
    }
    ui->lineEditOptionsFile->setText(QString::fromStdString(MazdaOptionsFile.string()));
}

void MainWindow::on_spinBoxDefectNr_valueChanged(int arg1)
{
    ui->lineEditDefectName->setText(QString::fromStdString(DefectNamesShort[arg1]));
}
