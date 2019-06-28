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

    std::string FileName;
    std::string FileNameTxt;
    cv::Mat ImIn;
    cv::Mat ImOut;

    CarrotDefect defectList;

    double minIm;
    double maxIm;

    double displayScale;

    void ReadDefects();
    void SaveDefects();
    void OpenImageFolder();
    void ReadImage();
    void ShowsScaledImage(cv::Mat Im, std::string ImWindowName, double dispScale);

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

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
