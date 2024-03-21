#include "HelpDesigner.h"
#include "../MainWindowDefines.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QDebug>

#define HELP_FOLDER_BASE    "help"
#define HELP_FOLDER_FILES   "files"
#define HELP_FOLDER_IMG     "images"

#define HELP_FILE_SUFFIX    "hlp"

HelpDesigner::HelpDesigner (QObject *parent)
    : QObject (parent)
    , _userMode (ISystemGuiCore::USER_MODE)
{}

HelpDesigner::~HelpDesigner ()
{
    _helpFile.clear ();

    if (_helpWindow)
        _helpWindow->close ();

    _iconManager = nullptr;
}

IHelpSection *HelpDesigner::helpSection (const QString &title)
{
    HelpSection *buffSection = findSection (title);
    if (!buffSection)
        buffSection = new HelpSection (title, this);

    return buffSection;
}

bool HelpDesigner::addSectionInHelp(IHelpSection *section)
{
    if (!section) {
        qCritical() << "[HelpDesigner][addSectionInHelp] Section is NULL! Add failed!";
        return false;
    }

    HelpSection *buffSection = dynamic_cast<HelpSection*> (section);
    if (!buffSection) {
        qCritical() << "[HelpDesigner][addSectionInHelp] Convert to HelpSection failed! Add failed!";
        return false;
    }

    if (buffSection->isSubsection ()) {
        qCritical() << "[HelpDesigner][addSectionInHelp] This section is a subsection! Add failed! Section title: " << buffSection->title();
        return false;
    }

    if (!buffSection->isAdded ()) {
        buffSection->addedInHelp(true);
        _sections.push_back(buffSection);
    }
    return true;
}

QVector<HelpSection *> HelpDesigner::sections () {
    return _sections;
}

void HelpDesigner::setIconManager (ISystemIconManager *iconManager) {
    _iconManager = iconManager;
}

void HelpDesigner::setSystemName (const QString &systemName) {
    _systemName = systemName;
}

void HelpDesigner::initHelp()
{
    loadHelpFromFiles();
    initSectionSignature();
    initHelpContents();

    if (_helpFile.isEmpty ())
        return;

    initHelpData ();
}

void HelpDesigner::loadHelpFromFiles ()
{
    // help files
    QFileInfoList hlpFileList;
    QDir hlpFileDir(qApp->applicationDirPath());
    if(!hlpFileDir.cd(HELP_FOLDER_BASE))
        return;
    if(hlpFileDir.cd(HELP_FOLDER_FILES)) {
        QStringList filesSort;
        filesSort << "*.hlp";
        hlpFileList = hlpFileDir.entryInfoList(filesSort, QDir::Files);
    }

    if(hlpFileList.isEmpty())
        return;

    // help images
    QFileInfoList hlpImgList;
    QDir hlpImgDir(qApp->applicationDirPath());
    if(!hlpImgDir.cd(HELP_FOLDER_BASE))
        return;
    if(hlpImgDir.cd(HELP_FOLDER_IMG)) {
        QStringList filesSort;
        filesSort << "*.png" << "*.jpg" << "*.jpeg" << "*.bmp";
        hlpImgList = hlpImgDir.entryInfoList(filesSort, QDir::Files);
    }

    // scan files and add in help
    for(int i = 0; i < hlpFileList.size(); i++) {
        QMap<QString /*section*/, QString /*data*/> helpData = scanHelpFile(hlpFileList[i].absoluteFilePath());
        QMapIterator<QString, QString> it(helpData);
        while(it.hasNext()) {
            it.next();
            QStringList sectionList = it.key().split(":");
            QFileInfoList imgList;
            for(int j = 0; j < hlpImgList.size(); j++) {
                if(it.value().indexOf(hlpImgList[j].fileName()) != -1)
                    imgList.append(hlpImgList[j]);
            }

            // add in help
            if(sectionList.size() == 1) {
                IHelpSection *hSection = helpSection(sectionList[0]);
                if(!hSection) {
                    qCritical() << QString("[HelpSection][loadHelpFromFiles] Not find help section! Ignore! Name: %1")
                                   .arg(sectionList[0]);
                    continue;
                }

                hSection->setText(it.value());
                for(int k = 0; k < imgList.size(); k++)
                    hSection->addImageResource(imgList[k].fileName(), QImage(imgList[k].absoluteFilePath()));

                addSectionInHelp(hSection);

            } else if(sectionList.size() > 1) {
                IHelpSection *hSection = nullptr;
                IHelpSection *hPrevSection = nullptr;
                IHelpSection *hFirstSection = nullptr;
                for(int l = 0; l < sectionList.size() - 1; l++) {
                    hPrevSection = hSection;
                    hSection = helpSection(sectionList[l]);
                    if(!hSection) {
                        qCritical() << QString("[HelpSection][loadHelpFromFiles] Not found help section! Ignore! Name: %1")
                                       .arg(sectionList[l]);
                        continue;
                    }

                    if(hPrevSection != nullptr
                       && hSection != nullptr
                       && hSection->isSubsection() == false)
                        hPrevSection->addSubsection(hSection);

                    if(l == 0)
                        hFirstSection = hSection;
                }
                hPrevSection = hSection;

                hSection = helpSection(sectionList[sectionList.size() - 1]);
                if(!hSection) {
                    qCritical() << QString("[HelpSection][loadHelpFromFiles] Not find help section! Ignore! Name: %1")
                                   .arg(sectionList[sectionList.size() - 1]);
                    continue;
                }

                hSection->setText(it.value());
                for(int k = 0; k < imgList.size(); k++)
                    hSection->addImageResource(imgList[k].fileName(), QImage(imgList[k].absoluteFilePath()));

                if(hPrevSection)
                    hPrevSection->addSubsection(hSection);

                addSectionInHelp(hFirstSection);
            }
        }
    }
}

QMap<QString, QString> HelpDesigner::scanHelpFile(const QString &filePath)
{
    QFile *textFile = new QFile(filePath);
    if(!textFile->open(QIODevice::ReadOnly | QIODevice::Text)) {
        qCritical() << QString("[HelpSection][scanHelpFile] Can not open file! File: %1")
                       .arg(textFile->fileName());

        delete textFile;
        return QMap<QString /*section*/, QString /*data*/>();
    }

    QMap<QString /*section*/, QString /*data*/> helpData;

    QString buffSection;
    QString buffData;
    while(!textFile->atEnd()) {
        QString line = textFile->readLine();

        // if start section
        if(line.trimmed().indexOf("{%") != -1
           && line.trimmed().indexOf("%}") != -1) {

            QString buffLine = line.trimmed().replace("{%","");
            buffLine = buffLine.trimmed().replace("%}","");
            buffLine = buffLine.trimmed();

            // if invalid section name - ignore
            if(buffLine.isEmpty())
                continue;

            // save prev section
            if(!buffSection.isEmpty()
               && !buffData.isEmpty()) {

                if(!helpData.contains(buffSection))
                    helpData.insert(buffSection, buffData);
                else
                    qWarning() << QString ("[HelpSection][setTextFile] Dublicate section! Ignore! Section name: %1; File: %2")
                                  .arg (buffSection, textFile->fileName());

            }

            buffData.clear();
            buffSection.clear();
            buffSection = buffLine;
        } else {
            buffData += line;
        }
    }
    textFile->close();

    // save prev section
    if(!buffSection.isEmpty()
       && !buffData.isEmpty()) {

        if(!helpData.contains(buffSection))
            helpData.insert(buffSection, buffData);
        else
            qWarning() << QString("[HelpSection][setTextFile] Dublicate section! Ignore! Section name: %1; File: %2")
                          .arg(buffSection, textFile->fileName());
    }
    delete textFile;
    textFile = nullptr;

    return helpData;
}

void HelpDesigner::initSectionSignature(HelpSection *parentSection, const QString &signaturePath)
{
    if(!parentSection) {
        QVector<HelpSection*> buffList = sections();
        for(int i = 0; i < buffList.size(); i++) {
            HelpSection* buffSection = buffList[i];
            initSectionSignature(buffSection, buffSection->title());
        }
    } else {
        parentSection->setProperty(ACTION_PROP_SIGNATURE, signaturePath);

        QVector<HelpSection*> buffList = parentSection->sections();
        for(int i = 0; i < buffList.size(); i++) {
            HelpSection* buffSection = buffList[i];
            initSectionSignature(buffSection,
                                       QString ("%1/%2")
                                       .arg (signaturePath, buffSection->title()));
        }

        QVector<IHelpItem*> itemsList = parentSection->items();
        for(int i = 0; i < itemsList.size(); i++) {
            itemsList[i]->setProperty(ACTION_PROP_SIGNATURE,
                                      QString("%1/%2")
                                      .arg(signaturePath, itemsList[i]->title()));

//            qDebug() << itemsList[i]->property(ACTION_PROP_SIGNATURE).toString();
        }
    }
}

void HelpDesigner::initHelpContents(HelpSection *parentSection, const QString &sectionNum)
{
    if(!parentSection) {
        QVector<HelpSection*> buffList = sections();
        for(int i = 0; i < buffList.size(); i++) {
            HelpSection* buffSection = buffList[i];
            QString buffSectionNum = QString("%1.").arg(i + 1);
            initHelpContents(buffSection, buffSectionNum);
        }
    } else {
        // add section
        QString sectionTab;
        QStringList sectionNumStr = sectionNum.split(".");
        for(int i = 2; i < sectionNumStr.size(); i++)
            sectionTab = QString(sectionTab + HTML_SYMBOL_TAB);

        _helpFile = _helpFile + QString("<br/><b>%1<a href=\"%2\"><font size=4>%3 %4</font></a></b>")
                                .arg(sectionTab)
                                .arg(parentSection->property(ACTION_PROP_SIGNATURE).toString())
                                .arg(sectionNum)
                                .arg(parentSection->title());

        // add items
        int subNum = 1;
        sectionTab = QString(sectionTab + HTML_SYMBOL_TAB);
        QVector<IHelpItem*> itemsList = parentSection->items();
        for(int i = 0; i < itemsList.size(); i++) {
            _helpFile = _helpFile + QString("<br/><b>%1<a href=\"%2\"><font size=4>%3%4. %5</font></a></b>")
                                    .arg(sectionTab)
                                    .arg(itemsList[i]->property(ACTION_PROP_SIGNATURE).toString())
                                    .arg(sectionNum)
                                    .arg(subNum)
                                    .arg(itemsList[i]->title());
            subNum++;
        }

        // add subsections
        QVector<HelpSection*> buffList = parentSection->sections();
        for(int i = 0; i < buffList.size(); i++) {
            HelpSection* buffSection = buffList[i];
            QString buffSectionNum = QString("%1%2.").arg(sectionNum).arg(subNum + i);
            initHelpContents(buffSection, buffSectionNum);
        }
    }
}

void HelpDesigner::initHelpData(HelpSection *parentSection, const QString &sectionNum)
{
    if(!parentSection) {
        _helpFile = _helpFile + QString("<br/><br/><br/>");

        QVector<HelpSection*> buffList = sections();
        for(int i = 0; i < buffList.size(); i++) {
            HelpSection* buffSection = buffList[i];
            QString buffSectionNum = QString("%1.").arg(i + 1);
            initHelpData(buffSection, buffSectionNum);
        }
    } else {
        // add section
        _helpFile = _helpFile + QString("<h2 id=\"%1\">%2 %3</h2>")
                                .arg(parentSection->property(ACTION_PROP_SIGNATURE).toString())
                                .arg(sectionNum)
                                .arg(parentSection->title());

        // add section data
        if(!parentSection->text().isEmpty()) {
            _helpFile = _helpFile + parentSection->text();
        }

        // add items data
        int subNum = 1;
        QVector<IHelpItem*> itemsList = parentSection->items();
        for(int i = 0; i < itemsList.size(); i++) {
            _helpFile = _helpFile + QString("<h2 id=\"%1\">%2%3. %4</h2>")
                                    .arg(itemsList[i]->property(ACTION_PROP_SIGNATURE).toString())
                                    .arg(sectionNum)
                                    .arg(subNum)
                                    .arg(itemsList[i]->title());

            _helpFile = _helpFile + itemsList[i]->text();
            subNum++;
        }

        // add subsections data
        QVector<HelpSection*> buffList = parentSection->sections();
        for(int i = 0; i < buffList.size(); i++) {
            HelpSection* buffSection = buffList[i];
            QString buffSectionNum = QString("%1%2.").arg(sectionNum).arg(subNum + i);
            initHelpData(buffSection, buffSectionNum);
        }
    }
}

bool HelpDesigner::isEmpty () {
    return _helpFile.isEmpty ();
}

void HelpDesigner::setUserMode (ISystemGuiCore::UserMode userMode) {
    _userMode = userMode;
}

HelpSection *HelpDesigner::findSection(const QString &title, HelpSection *parentSection)
{
    if(!parentSection) {
        QVector<HelpSection*> buffList = sections();
        for(int i = 0; i < buffList.size(); i++) {
            HelpSection* buffSection = buffList[i];
            if(buffSection) {
                if(buffSection->title() == title)
                    return buffSection;

                if(buffSection->sections().size() != 0) {
                    HelpSection* retSection = findSection(title, buffSection);
                    if(retSection)
                        return retSection;
                }
            }
        }
    } else {
        QVector<HelpSection*> buffList = parentSection->sections();
        for(int i = 0; i < buffList.size(); i++) {
            HelpSection* buffSection = buffList[i];
            if(buffSection) {
                if(buffSection->title() == title)
                    return buffSection;

                if(buffSection->sections().size() != 0) {
                    HelpSection* retSection = findSection(title, buffSection);
                    if(retSection)
                        return retSection;
                }
            }
        }
    }
    return nullptr;
}

void HelpDesigner::showHelp ()
{
    if (_helpWindow) {
        _helpWindow->activateWindow ();
    } else {
        _helpWindow = new HelpWindow (_helpFile,
                                      _sections,
                                      _userMode,
                                      _iconManager->getSystemLogo (QSize (128,128)).pixmap (128,128));

        _helpWindow->setWindowIcon (_iconManager->getSystemLogo (16, 16));
        _helpWindow->setWindowTitle (_systemName + " - " + tr ("Context help"));
        //connect (_helpWindow, &HelpWindow::signalClose, this, &HelpDesigner::slotWindowDestroyed);
        connect (_helpWindow, &HelpWindow::destroyed, this, &HelpDesigner::slotWindowDestroyed);
        _helpWindow->show ();
    }
}

void HelpDesigner::slotWindowDestroyed ()
{
    //delete _helpWindow;
    _helpWindow = nullptr;
}
