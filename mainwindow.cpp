#include "mainwindow.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QRegularExpressionValidator>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>

Person::Person(const QString& ln, const QString& fn, const QString& mn,
               const QString& ph, const QString& g, const QStringList& langs)
    : lastName(ln), firstName(fn), middleName(mn),
    phone(ph), gender(g), languages(langs) {}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    QWidget *central = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(central);

    QFormLayout *form = new QFormLayout();
    leLastName   = new QLineEdit(this);
    leFirstName  = new QLineEdit(this);
    leMiddleName = new QLineEdit(this);
    lePhone      = new QLineEdit(this);

    form->addRow("Фамилия:", leLastName);
    form->addRow("Имя:", leFirstName);
    form->addRow("Отчество:", leMiddleName);
    form->addRow("Номер телефона:", lePhone);

    // Пол
    QGroupBox *gbGender = new QGroupBox("Пол");
    QHBoxLayout *genderLayout = new QHBoxLayout(gbGender);
    rbMale   = new QRadioButton("Мужской");
    rbFemale = new QRadioButton("Женский");
    rbMale->setChecked(true);
    genderLayout->addWidget(rbMale);
    genderLayout->addWidget(rbFemale);

    QGroupBox *gbLang = new QGroupBox("Языки");
    QVBoxLayout *langLayout = new QVBoxLayout(gbLang);
    cbRussian = new QCheckBox("Русский");
    cbEnglish = new QCheckBox("Английский");
    cbRussian->setChecked(true);
    langLayout->addWidget(cbRussian);
    langLayout->addWidget(cbEnglish);

    // Кнопки
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnReset = new QPushButton("Сброс");
    btnSave  = new QPushButton("Сохранить");

    btnReset->setStyleSheet("background-color: #ff6666; color: white;");
    btnSave->setStyleSheet("background-color: #66cc66; color: white;");

    btnLayout->addWidget(btnReset);
    btnLayout->addWidget(btnSave);

    mainLayout->addLayout(form);
    mainLayout->addWidget(gbGender);
    mainLayout->addWidget(gbLang);
    mainLayout->addLayout(btnLayout);

    setCentralWidget(central);
    setWindowTitle("Анкета");

    setupValidators();

    connect(btnSave,  &QPushButton::clicked, this, &MainWindow::onSaveClicked);
    connect(btnReset, &QPushButton::clicked, this, &MainWindow::onResetClicked);
}

void MainWindow::setupValidators()
{
    QRegularExpression nameRx("^[А-ЯЁA-Z][а-яёa-z]*$");
    auto *nameVal = new QRegularExpressionValidator(nameRx, this);

    leLastName->setValidator(nameVal);
    leFirstName->setValidator(nameVal);
    leMiddleName->setValidator(nameVal);

    QRegularExpression phoneRx(R"(^\+([17]|395)?\d{10}$)");
    lePhone->setValidator(new QRegularExpressionValidator(phoneRx, this));
}

bool MainWindow::validateForm(QString &errorField) const
{
    if (leLastName->text().trimmed().isEmpty())  { errorField = "Фамилия"; return false; }
    if (leFirstName->text().trimmed().isEmpty()) { errorField = "Имя"; return false; }
    if (lePhone->text().trimmed().isEmpty())     { errorField = "Номер телефона"; return false; }

    if (!leLastName->hasAcceptableInput())  { errorField = "Фамилия (формат)"; return false; }
    if (!leFirstName->hasAcceptableInput()) { errorField = "Имя (формат)"; return false; }
    if (!leMiddleName->hasAcceptableInput() && !leMiddleName->text().isEmpty()) {
        errorField = "Отчество (формат)"; return false;
    }
    if (!lePhone->hasAcceptableInput()) { errorField = "Номер телефона (формат)"; return false; }

    return true;
}

void MainWindow::onSaveClicked()
{
    QString error;
    if (!validateForm(error)) {
        QMessageBox::warning(this, "Ошибка", "Поле не соответствует требованиям: " + error);
        return;
    }

    QString gender = rbMale->isChecked() ? "Мужской" : "Женский";

    QStringList langs;
    if (cbRussian->isChecked()) langs << "Русский";
    if (cbEnglish->isChecked()) langs << "Английский";

    Person p(leLastName->text().trimmed(),
             leFirstName->text().trimmed(),
             leMiddleName->text().trimmed(),
             lePhone->text().trimmed(),
             gender,
             langs);

    if (p.saveToFile()) {
        QMessageBox::information(this, "Успех", "Данные сохранены в result.txt");
        resetForm();
    } else {
        QMessageBox::critical(this, "Ошибка", "Не удалось сохранить файл");
    }
}

void MainWindow::onResetClicked()
{
    resetForm();
}

void MainWindow::resetForm()
{
    leLastName->clear();
    leFirstName->clear();
    leMiddleName->clear();
    lePhone->clear();

    rbMale->setChecked(true);
    cbRussian->setChecked(true);
    cbEnglish->setChecked(false);
}

bool Person::saveToFile(const QString& filename) const
{
    QFile file(filename);
    if (!file.open(QIODevice::Append | QIODevice::Text))
        return false;

    QTextStream out(&file);
    out << "Фамилия: " << lastName << "\n";
    out << "Имя: " << firstName << "\n";
    out << "Отчество: " << (middleName.isEmpty() ? "(нет)" : middleName) << "\n";
    out << "Телефон: " << phone << "\n";
    out << "Пол: " << gender << "\n";
    out << "Языки: " << (languages.isEmpty() ? "Нет" : languages.join(", ")) << "\n";
    out << "----------------------------------------\n";

    return true;
}

MainWindow::~MainWindow() {}
