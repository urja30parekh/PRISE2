#include "InstructionDlg.h"
#include "ui_InstructionDlg.h"
#include <QUrl>
#include <QDir>
#include <QDesktopServices>

InstructionDlg::InstructionDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InstructionDlg)
{
    ui->setupUi(this);
}

InstructionDlg::~InstructionDlg()
{
    delete ui;
}

void InstructionDlg::openManual()
{
    QString url = "";
#ifdef MAC
    QString appPath = QDir::currentPath();
    url += "file:/";
    url += appPath;
    url += "/";
#endif

#ifdef WIN
    url += "instructions\\PRISE_Manual.pdf";
#else
    url += "instructions/PRISE_Manual.pdf";
#endif
    QDesktopServices::openUrl(QUrl::fromLocalFile(url));
}

void InstructionDlg::openTutorial()
{
    QString url = "";
#ifdef MAC
    QString appPath = QDir::currentPath();
    url += "file:/";
    url += appPath;
    url += "/";
#endif

#ifdef WIN
    url += "instructions\\PRISE_Tutorial.pdf";
#else
    url += "instructions/PRISE_Tutorial.pdf";
#endif
    QDesktopServices::openUrl(QUrl::fromLocalFile(url));
}
