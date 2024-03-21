#include "HelpWindow.h"
#include "ui_HelpWindow.h"

#include <QtCore/QDebug>
#include <QtCore/QUrl>
#include <QtCore/QVariant>

#include <QtPrintSupport/QPrinter>

#include <QtWidgets/QScrollBar>

#include "../MainWindowDefines.h"

HelpWindow::HelpWindow (const QString &fileText,
                        const QVector<HelpSection *> &sections,
                        bool isAdminMode,
                        const QPixmap &logo,
                        QWidget *parent)
    : QMainWindow (parent)
    , ui (new Ui::HelpWindow)
{
    ui->setupUi (this);
    setAttribute (Qt::WA_DeleteOnClose);

    _logo = logo;
    _anchorsList.push_back ("");

    _textEdit = new HelpTextEdit (this);
    connect (_textEdit, SIGNAL(gotoAnchor(QString)), this, SLOT(onAddAnchor(QString)));
    connect (_textEdit, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenu(QPoint)));
    ui->verticalLayoutForWidget->addWidget(_textEdit);

    _document = new QTextDocument(_textEdit);
    _textEdit->setDocument(_document);

    _fileText = fileText;
    _sections = sections;

    _textEdit->setHtml(_fileText);
    _textEdit->setReadOnly(true);
    _textEdit->setContextMenuPolicy(Qt::CustomContextMenu);
    this->setContextMenuPolicy(Qt::NoContextMenu);

    _searchWidget = new HelpSearchWidget(_textEdit, this);
    ui->statusBar->addWidget(_searchWidget);
    ui->statusBar->hide();
    connect(_searchWidget, SIGNAL(signalHide()), ui->statusBar, SLOT(hide()));

    ui->toolBar->setFloatable(false);
    ui->toolBar->setMovable(false);

    _actSearch = ui->toolBar->addAction(QIcon(":/icons/icons/binocular.png"), tr("Поиск"));
    _actSearch->setShortcut(QKeySequence(QKeySequence::Find));
    _actSearch->setIconVisibleInMenu(true);
    connect(_actSearch, SIGNAL(triggered()), ui->statusBar, SLOT(show()));

    ui->toolBar->addSeparator();

    _actContents = ui->toolBar->addAction(QIcon(":/icons/icons/book-open-list.png"), tr("К оглавлению"));
    _actContents->setIconVisibleInMenu(true);
    connect(_actContents, SIGNAL(triggered()), this, SLOT(onScrollToContents()));

    ui->toolBar->addSeparator();

    _actPrev = ui->toolBar->addAction(QIcon(":/icons/icons/arrow-skip-180.png"), tr("Назад"));
    _actPrev->setIconVisibleInMenu(true);
    connect(_actPrev, SIGNAL(triggered()), this, SLOT(onPrev()));

    _actNext = ui->toolBar->addAction(QIcon(":/icons/icons/arrow-skip.png"), tr("Вперёд"));
    _actNext->setIconVisibleInMenu(true);
    connect(_actNext, SIGNAL(triggered()), this, SLOT(onNext()));

    _actClear = ui->toolBar->addAction(QIcon(":/icons/icons/trashcan_full.png"), tr("Очистить историю"));
    _actClear->setIconVisibleInMenu(true);
    connect(_actClear, SIGNAL(triggered()), this, SLOT(onClear()));

    _actSeparatorPdf = ui->toolBar->addSeparator();

    _actPdf = ui->toolBar->addAction(QIcon(":/icons/icons/blue-document-pdf.png"), tr("Сохранить в *.pdf"));
    _actPdf->setIconVisibleInMenu(true);
    connect(_actPdf, SIGNAL(triggered()), this, SLOT(onPdf()));

    _actSeparatorPdf->setVisible(isAdminMode);
    _actPdf->setVisible(isAdminMode);

    _contextMenu = new QMenu(this);
    _contextMenu->addAction(_actSearch);
    _contextMenu->addSeparator();
    _contextMenu->addAction(_actContents);
    _contextMenu->addSeparator();
    _contextMenu->addAction(_actPrev);
    _contextMenu->addAction(_actNext);
    _contextMenu->addAction(_actClear);

    this->initSource();

    _actNext->setEnabled(false);
    _actPrev->setEnabled(false);
    _actClear->setEnabled(false);
}

HelpWindow::~HelpWindow()
{
    _fileText.clear();
    _sections.clear();
    _addedSources.clear();
    delete _searchWidget;
    delete _document;
    delete _textEdit;
    delete ui;
}

//void HelpWindow::closeEvent (QCloseEvent *event)
//{
//    event->accept ();
//    emit this->signalClose ();
//}

void HelpWindow::initSource (HelpSection *section)
{
    if(!section) {
        for(int i = 0; i < _sections.size(); i++)
            this->initSource(_sections[i]);

    } else {
        this->initSourceImages(section);

        QVector<HelpSection*> sectionList = section->sections();
        for(int i = 0; i < sectionList.size(); i++)
            this->initSource(sectionList[i]);

        QVector<IHelpItem*> itemsList = section->items();
        for(int i = 0; i < itemsList.size(); i++)
            this->initSourceImages(itemsList[i]);
    }
}

void HelpWindow::initSourceImages(HelpSection *section)
{
    if(!section)
        return;

    const QMap<QString, QImage> *sources = section->imageResources();
    if(!sources)
        return;

    QMapIterator<QString, QImage>i(*sources);
    while(i.hasNext()) {
        i.next();
        if(_addedSources.contains(i.key())) {
            qCritical() << QString("[HelpWindow][initSourceImages] ImageResource already added! Image name: %1; HelpSection title: %2; HelpSection puth: %3")
                           .arg(i.key())
                           .arg(section->title())
                           .arg(section->property(ACTION_PROP_SIGNATURE).toString());
        } else {
            _document->addResource(QTextDocument::ImageResource, QUrl(i.key()), QVariant(i.value()));
            _addedSources.insert(i.key(), i.value());
        }
    }
}

void HelpWindow::initSourceImages(IHelpItem *item)
{
    if(!item)
        return;

    const QMap<QString, QImage> *sources = item->imageResources();
    if(!sources)
        return;

    QMapIterator<QString, QImage>i(*sources);
    while(i.hasNext()) {
        i.next();
        if(_addedSources.contains(i.key())) {
            qCritical() << QString("[HelpWindow][initSourceImages] ImageResource already added! Image name: %1; IHelpItem title: %2; IHelpItem puth: %3")
                           .arg(i.key())
                           .arg(item->title())
                           .arg(item->property(ACTION_PROP_SIGNATURE).toString());
        } else {
            _document->addResource(QTextDocument::ImageResource, QUrl(i.key()), QVariant(i.value()));
            _addedSources.insert(i.key(), i.value());
        }
    }
}

void HelpWindow::onScrollToContents()
{
    _textEdit->moveCursor(QTextCursor::Start);
    _textEdit->ensureCursorVisible();
}

void HelpWindow::onNext()
{
    this->onScrollToContents();

    _currentAnchor++;
    _textEdit->scrollToAnchor(_anchorsList[_currentAnchor]);

    _actPrev->setEnabled(true);
    if(_currentAnchor == _anchorsList.size() - 1)
        _actNext->setEnabled(false);
}

void HelpWindow::onPrev()
{
    this->onScrollToContents();

    _currentAnchor--;
    QString buffAnchor = _anchorsList[_currentAnchor];

    if(!buffAnchor.isEmpty()) {
        _textEdit->scrollToAnchor(buffAnchor);
    }

    _actNext->setEnabled(true);
    if(_currentAnchor == 0)
        _actPrev->setEnabled(false);
}

void HelpWindow::onClear()
{
    _currentAnchor = 0;
    _anchorsList.clear();
    _anchorsList.push_back("");

    _actPrev->setEnabled(false);
    _actNext->setEnabled(false);
    _actClear->setEnabled(false);
}

void HelpWindow::onAddAnchor(QString anchor)
{
    _currentAnchor++;
    _anchorsList.push_back(anchor);
    _actPrev->setEnabled(true);
    _actClear->setEnabled(true);
}

void HelpWindow::contextMenu(QPoint point)
{
    _contextMenu->exec(this->mapToGlobal(point));
}

void HelpWindow::onPdf ()
{
    qDebug () << "[HelpWindow][onPdf] Under constuction";
//    QStringList mask;
//    mask.append("pdf");
//    QString fileName = IFileDialog::getSaveFileName(this,
//                                                    tr("Сохранение контекстной справки"),
//                                                    tr("%1.pdf").arg(this->windowTitle()),
//                                                    QString(),
//                                                    mask);

//    if(fileName.isEmpty())
//        return;

//    // проверка имени файла
//    QStringList str = fileName.split(".");
//    if(str.size() == 1) {
//        fileName = str.operator [](0) + ".pdf";
//    } else {
//        if(str.operator [](str.size()-1) != "pdf") {
//            fileName.clear();
//            for(int i = 0; i < str.size(); i++) {
//                if(fileName.isEmpty())
//                    fileName = str.operator [](i);
//                else
//                    fileName = fileName + "." + str.operator [](i);
//            }
//            fileName + fileName + ".pdf";
//        } else {
//            fileName.clear();
//            for(int i = 0; i < str.size(); i++) {
//                if(fileName.isEmpty())
//                    fileName = str.operator [](i);
//                else
//                    fileName = fileName + "." + str.operator [](i);
//            }
//        }
//    }

//    // редактирование размеров изображений
//    int buffIndex = -1;
//    while(true) {
//        buffIndex++;
//        buffIndex = _fileText.indexOf("<img", buffIndex);
//        if(buffIndex == -1)
//            break;
//        int buffIndex2 = _fileText.indexOf("/>", buffIndex);
//        QString imgData = _fileText.mid(buffIndex, buffIndex2 + 2 - buffIndex);
//        QStringList imgDataList = imgData.split("\"");
//        QString imgName;
//        for(int i = 0; i < imgDataList.size(); i++) {
//            if(imgDataList[i].indexOf("src=") != -1
//                    && i < (imgDataList.size() - 1)) {
//                imgName = imgDataList[i + 1];
//                break;
//            }
//        }
//        if(imgName.isEmpty())
//            continue;
//        if(!_addedSources.contains(imgName))
//            continue;

//        QImage imgFile = _addedSources.value(imgName);
//        int imgHeight = imgFile.size().height();
//        int imgWidth = imgFile.size().width();
//        if(imgWidth > 600) {
//            double tempWidth = (double) imgWidth;
//            double tempHeight = (double) imgHeight;
//            double tempSize = tempWidth / ((double) 600);
//            double tempHeightImg = tempHeight/tempSize;

//            QString imgDataNew = QString("<img src=\"%1\" width=\"600\" height=\"%2\" />")
//                    .arg(imgName)
//                    .arg((int)tempHeightImg);

//            _fileText = _fileText.replace(imgData, imgDataNew);
//        }
//    }

//    QString buffText = QString("<h1><center>%1</center></h1>").arg(this->windowTitle());
//    buffText += QString("<br/>");
//    buffText += QString("<br/><center><img src=\"2eca4868-8377-4f55-a8d3-60aa4dc1a1fd\" /><br/><captionimg></captionimg></center>");

//    for(int ii = 0; ii < 5; ii++)
//        buffText += QString("<br/>");

//    buffText += _fileText;


//    QPrinter printer(QPrinter::HighResolution);
//    printer.setOutputFormat(QPrinter::PdfFormat);
//    printer.setColorMode(QPrinter::Color);
//    printer.setOutputFileName(fileName);
//    printer.setPageSize(QPrinter::A4);

//    QTextEdit textEdit;
//    textEdit.setHtml(buffText);
//    textEdit.document()->addResource(QTextDocument::ImageResource,
//                                     QUrl("2eca4868-8377-4f55-a8d3-60aa4dc1a1fd"),
//                                     QVariant(_logo.toImage()));

//    // add image resources
//    QMapIterator<QString, QImage>i(_addedSources);
//    while(i.hasNext()) {
//        i.next();
//        textEdit.document()->addResource(QTextDocument::ImageResource, QUrl(i.key()), QVariant(i.value()));
//    }

//    textEdit.print(&printer);
}
