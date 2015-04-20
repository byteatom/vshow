#include "about_dialog.h"
#include "ui_about_dialog.h"
#include <windows.h>
#include <string>

#define vshow_ver(x) (#x)

AboutDialog::AboutDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::AboutDialog) {
    ui->setupUi(this);
    connect(ui->close, SIGNAL(clicked(bool)), this, SLOT(close(bool)));
    QString appName = QCoreApplication::applicationName();
    QString version;
    QString appFilePath = QCoreApplication::applicationFilePath();
    std::wstring appFileString = appFilePath.toStdWString();
    DWORD appFileHande = 0;
    DWORD appVersionInfoSize = 0;
    appVersionInfoSize
        = ::GetFileVersionInfoSize(appFileString.c_str(), &appFileHande);
    if (appVersionInfoSize > 0) {
        BYTE *verInfo = new BYTE[appVersionInfoSize + 1];
        ::memset(verInfo, 0, appVersionInfoSize + 1);
        ::GetFileVersionInfo(
            appFileString.c_str(), 0, appVersionInfoSize + 1, verInfo);
        VS_FIXEDFILEINFO *vs_fileInfo = nullptr;
        unsigned int len = 0;
        std::wstring subBlock = L"\\";
        if (::VerQueryValue(
                verInfo, subBlock.c_str(), (void **)&vs_fileInfo, &len)) {
            WORD ver[4];
            ver[0] = HIWORD(vs_fileInfo->dwProductVersionMS);
            ver[1] = LOWORD(vs_fileInfo->dwProductVersionMS);
            ver[2] = HIWORD(vs_fileInfo->dwProductVersionLS);
            ver[3] = LOWORD(vs_fileInfo->dwProductVersionLS);
            char appVersion[256] = {0};
            sprintf(appVersion, "%d.%d.%d.%d", ver[0], ver[1], ver[2], ver[3]);
            std::string wsVersion = appVersion;
            version = QString::fromStdString(wsVersion);
        }
        if (verInfo) {
            delete verInfo;
            verInfo = nullptr;
        }
    }
    setWindowTitle(tr("About") + " " + appName + " " + "[" + version + "]");
    ui->version->setText(version);
}

AboutDialog::~AboutDialog() {
    delete ui;
}

void AboutDialog::close(bool) {
    done(AboutDialog::Accepted);
}
