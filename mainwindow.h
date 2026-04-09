#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QRadioButton>
#include <QCheckBox>
#include <QPushButton>
#include <QStringList>

class Person {
public:
    QString lastName;
    QString firstName;
    QString middleName;
    QString phone;
    QString gender;
    QStringList languages;

    Person() = default;
    Person(const QString& ln, const QString& fn, const QString& mn,
           const QString& ph, const QString& g, const QStringList& langs);

    bool saveToFile(const QString& filename = "result.txt") const;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onSaveClicked();
    void onResetClicked();

private:
    QLineEdit *leLastName;
    QLineEdit *leFirstName;
    QLineEdit *leMiddleName;
    QLineEdit *lePhone;

    QRadioButton *rbMale;
    QRadioButton *rbFemale;

    QCheckBox *cbRussian;
    QCheckBox *cbEnglish;

    QPushButton *btnReset;
    QPushButton *btnSave;

    void setupValidators();
    bool validateForm(QString &errorField) const;
    void resetForm();
};

#endif // MAINWINDOW_H
