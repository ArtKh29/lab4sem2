#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QRegularExpressionValidator>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnReset_clicked();      // Сброс
    void on_btnSave_clicked();       // Сохранить

private:
    Ui::MainWindow *ui;

    // Регулярные выражения по твоим правилам
    bool validateSurname();     // 1. анг+рус, первая буква заглавная
    bool validateName();        // 2. то же самое
    bool validatePatronymic();  // 3. может быть пустым
    bool validatePhone();       // 4. +7... или 8... или 10 цифр
    bool validateGender();      // обязательно выбран
    bool validateLanguages();   // хотя бы один

    void showError(const QString &field, const QString &message);
    void saveToFile();
    void clearForm();
};

#endif // MAINWINDOW_H
