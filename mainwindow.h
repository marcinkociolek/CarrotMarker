#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <boost/filesystem.hpp>

#include "carrotdefect.h"
#include <opencv2/core/core.hpp>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    boost::filesystem::path ImageFolder;
    boost::filesystem::path OutFolder;
    boost::filesystem::path MazdaFolder;
    boost::filesystem::path MazdaOptionsFile;

    std::string FileName;
    std::string FileNameTxt;
    cv::Mat ImIn;
    cv::Mat ImGray;
    cv::Mat ImOut;
    cv::Mat Mask;

    CarrotDefect defectList;

    double minIm;
    double maxIm;

    double displayScale;

    void ReadDefects();
    void SaveDefects();
    void OpenImageFolder();
    void ReadImage();
    void ShowsScaledImage(cv::Mat Im, std::string ImWindowName, double dispScale);
    void ShowImages();
    void ProcessImage();

private slots:
    void on_pushButtonOpenImageFolder_clicked();

    void on_listWidgetImageFiles_currentTextChanged(const QString &currentText);

    void on_lineEditRegexImageFile_returnPressed();

    void on_checkBoxShowInput_toggled(bool checked);

    void on_checkBoxShowTiffInfo_toggled(bool checked);

    void on_checkBoxAutocleanOut_toggled(bool checked);

    void on_checkBoxShowMatInfo_toggled(bool checked);

    void on_checkBoxLoadAnydepth_toggled(bool checked);

    void on_spinBoxScaleBase_valueChanged(int arg1);

    void on_spinBoxScalePower_valueChanged(int arg1);

    void on_checkBoxImRotate_toggled(bool checked);

    void on_spinBoxThreshold_valueChanged(int arg1);

    void on_spinBoxTransparency_valueChanged(int arg1);

    void on_comboBoxOutputMode_currentIndexChanged(int index);

    void on_pushButtonOpenOutFolder_clicked();

    void on_pushButtonCreateMazdaFiles_clicked();



    void on_pushButtonSelectMazdaFolder_clicked();

    void on_pushButtonSelectMazdaOptions_clicked();

    void on_spinBoxDefectNr_valueChanged(int arg1);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
