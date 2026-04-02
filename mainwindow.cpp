#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Устанавливаем валидаторы сразу при запуске

    // 1 и 2: Фамилия и Имя — русские + английские буквы, первая заглавная, остальное строчные
    QRegularExpression nameRx(R"(^[А-ЯA-Z][а-яa-zА-ЯA-Z]*$)");
    ui->leSurname->setValidator(new QRegularExpressionValidator(nameRx, this));
    ui->leName->setValidator(new QRegularExpressionValidator(nameRx, this));

    // 3: Отчество — такое же, но может быть пустым
    ui->lePatronymic->setValidator(new QRegularExpressionValidator(nameRx, this));

    // 4: Телефон — +7 или 8 или просто 10 цифр после кода
    QRegularExpression phoneRx(R"(^(\+7|8|\+1|\+39)?\d{10}$)");
    ui->lePhone->setValidator(new QRegularExpressionValidator(phoneRx, this));

    // Начальное состояние (как на картинке)
    ui->rbMale->setChecked(true);
    ui->cbRussian->setChecked(true);
    ui->cbEnglish->setChecked(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// ====================== ВАЛИДАЦИЯ ======================

bool MainWindow::validateSurname()
{
    QString s = ui->leSurname->text().trimmed();
    if (s.isEmpty()) return false;
    QRegularExpression rx(R"(^[А-ЯA-Z][а-яa-zА-ЯA-Z]*$)");
    return rx.match(s).hasMatch();
}

bool MainWindow::validateName()
{
    QString s = ui->leName->text().trimmed();
    if (s.isEmpty()) return false;
    QRegularExpression rx(R"(^[А-ЯA-Z][а-яa-zА-ЯA-Z]*$)");
    return rx.match(s).hasMatch();
}

bool MainWindow::validatePatronymic()
{
    QString s = ui->lePatronymic->text().trimmed();
    if (s.isEmpty()) return true;                    // разрешено пустое
    QRegularExpression rx(R"(^[А-ЯA-Z][а-яa-zА-ЯA-Z]*$)");
    return rx.match(s).hasMatch();
}

bool MainWindow::validatePhone()
{
    QString s = ui->lePhone->text().trimmed();
    if (s.isEmpty()) return false;

    // Примеры, которые проходят: +7123456789, 87123456789, 7123456789, +13951234567 и т.д.
    QRegularExpression rx(R"(^(\+?[17]|8|\+39|\+1)?\d{10}$)");
    return rx.match(s).hasMatch();
}

bool MainWindow::validateGender()
{
    return ui->rbMale->isChecked() || ui->rbFemale->isChecked();
}

bool MainWindow::validateLanguages()
{
    return ui->cbRussian->isChecked() ||
           ui->cbEnglish->isChecked() ||
           ui->cbFrench->isChecked();
}

// ====================== КНОПКА "СОХРАНИТЬ" ======================

void MainWindow::on_btnSave_clicked()
{
    QString errorMsg;

    if (!validateSurname())     errorMsg += "• Фамилия\n";
    if (!validateName())        errorMsg += "• Имя\n";
    if (!validatePatronymic())  errorMsg += "• Отчество (если указано — должно быть корректно)\n";
    if (!validatePhone())       errorMsg += "• Номер телефона (+7/8/10 цифр)\n";
    if (!validateGender())      errorMsg += "• Пол\n";
    if (!validateLanguages())   errorMsg += "• Хотя бы один язык\n";

    if (!errorMsg.isEmpty()) {
        QMessageBox::warning(this, "Ошибка валидации",
                             "Следующие поля заполнены неверно:\n\n" + errorMsg);
        return;
    }

    // Всё ок — сохраняем
    saveToFile();
    QMessageBox::information(this, "Успех", "Данные успешно сохранены в result.txt");
}

// ====================== СОХРАНЕНИЕ В ФАЙЛ ======================

void MainWindow::saveToFile()
{
    QFile file("result.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Ошибка", "Не удалось открыть result.txt для записи");
        return;
    }

    QTextStream out(&file);
    out << "Фамилия: " << ui->leSurname->text() << "\n";
    out << "Имя: " << ui->leName->text() << "\n";
    out << "Отчество: " << ui->lePatronymic->text() << "\n";
    out << "Телефон: " << ui->lePhone->text() << "\n";
    out << "Пол: " << (ui->rbMale->isChecked() ? "Мужской" : "Женский") << "\n";
    out << "Языки: ";
    if (ui->cbRussian->isChecked()) out << "Русский ";
    if (ui->cbEnglish->isChecked()) out << "Английский ";
    if (ui->cbFrench->isChecked())  out << "Французский";
    out << "\n";

    file.close();
}

// ====================== КНОПКА "СБРОС" ======================

void MainWindow::on_btnReset_clicked()
{
    clearForm();
}

void MainWindow::clearForm()
{
    ui->leSurname->clear();
    ui->leName->clear();
    ui->lePatronymic->clear();
    ui->lePhone->clear();

    ui->rbMale->setChecked(true);
    ui->rbFemale->setChecked(false);

    ui->cbRussian->setChecked(true);
    ui->cbEnglish->setChecked(true);
    ui->cbFrench->setChecked(false);
}
