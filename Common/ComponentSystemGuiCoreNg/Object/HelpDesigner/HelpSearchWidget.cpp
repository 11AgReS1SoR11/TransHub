#include "HelpSearchWidget.h"
#include "ui_HelpSearchWidget.h"

HelpSearchWidget::HelpSearchWidget(QTextEdit *textEdit, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HelpSearchWidget),
    _textEdit(textEdit),
    _document(textEdit->document())
{
    ui->setupUi(this);

    _currentIndex = 0;

    ui->pushButtonHide->setIcon(QIcon("://icons/icons/icon_cancel.png"));
    ui->pushButtonNext->setIcon(QIcon("://icons/icons/arrow-skip-270.png"));
    ui->pushButtonPrev->setIcon(QIcon("://icons/icons/arrow-skip-090.png"));

    ui->pushButtonNext->setEnabled(false);
    ui->pushButtonPrev->setEnabled(false);

    connect(ui->pushButtonHide, SIGNAL(clicked()), this, SLOT(onHide()));
    connect(ui->pushButtonNext, SIGNAL(clicked()), this, SLOT(onNext()));
    connect(ui->pushButtonPrev, SIGNAL(clicked()), this, SLOT(onPrev()));
    connect(ui->lineEdit, SIGNAL(textChanged(QString)), this, SLOT(onSearch(QString)));
}

HelpSearchWidget::~HelpSearchWidget()
{
    _document = nullptr;
    _textEdit = nullptr;
    _cursorPos.clear();
    delete ui;
}

void HelpSearchWidget::clearSearch()
{
    _currentIndex = 0;
    _cursorPos.clear();

    ui->lineEdit->clear();
    ui->pushButtonNext->setEnabled(false);
    ui->pushButtonPrev->setEnabled(false);
}

void HelpSearchWidget::onSearch(const QString &text)
{
    _currentIndex = 0;
    _cursorPos.clear();
    _searchText.clear();

    _prevCursor = _document->find(text);
    if(!_prevCursor.isNull()) {
        _prevCursor.select(QTextCursor::LineUnderCursor);
        _textEdit->setTextCursor(_prevCursor);

        _searchText = text;
        _cursorPos.push_back(_prevCursor.position());

        ui->pushButtonNext->setEnabled(true);
        ui->pushButtonPrev->setEnabled(true);
    } else {
        _textEdit->moveCursor(QTextCursor::Start);
        _textEdit->ensureCursorVisible();

        ui->pushButtonNext->setEnabled(false);
        ui->pushButtonPrev->setEnabled(false);
    }
}

void HelpSearchWidget::onNext()
{
    if(_prevCursor.isNull())
        return;

    _prevCursor = _document->find(_searchText, _prevCursor);
    if(_prevCursor.isNull()) {
        ui->pushButtonNext->setEnabled(false);
        ui->pushButtonPrev->setEnabled(true);
    } else {
        _prevCursor.select(QTextCursor::LineUnderCursor);
        _textEdit->setTextCursor(_prevCursor);

        _cursorPos.push_back(_prevCursor.position());
        _currentIndex++;
    }
}

void HelpSearchWidget::onPrev()
{
    if(_cursorPos.size() == 0)
        return;
    if(_currentIndex == 0)
        return;

    _currentIndex--;

    QTextCursor buffCursor = _textEdit->textCursor();
    buffCursor.setPosition(_cursorPos[_currentIndex]);
    buffCursor.select(QTextCursor::LineUnderCursor);
    _textEdit->setTextCursor(buffCursor);

    if(_currentIndex == 0) {
        ui->pushButtonNext->setEnabled(true);
        ui->pushButtonPrev->setEnabled(false);
    }
}

void HelpSearchWidget::onHide()
{
    _textEdit->moveCursor(QTextCursor::StartOfLine);
    _textEdit->ensureCursorVisible();
    this->clearSearch();

    emit this->signalHide();
}
