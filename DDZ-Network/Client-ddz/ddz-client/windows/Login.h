#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QCheckBox>
#include <QStackedWidget>

class Login : public QDialog
{
    Q_OBJECT
public:
    explicit Login(QWidget* parent = nullptr);

    ~Login();

    void initAllPages();

    void initConnect();

    // 数据校验
    void regexCheck();

    bool verifyData(QLineEdit *edit);

public slots:
    void onLogin();
    void onRegister();
    void onSettingsConfirm();

private:
    void createLoginPage(QWidget* loginPage);
    void createRegisterPage(QWidget* registerPage);
    void createConfigServerPage(QWidget* configServerPage);

private:
    QStackedWidget* stackedWidget;

    QLineEdit* edit_login_userName;
    QLineEdit* edit_login_userPassword;

    QCheckBox* box_isSavePassword;
    QPushButton* btn_toRegisterPage;

    QPushButton* btn_login;

    QLineEdit* edit_register_userName;
    QLineEdit* edit_register_userPassword;
    QLineEdit* edit_register_phoneNumber;

    QPushButton* btn_register;

    QLineEdit* edit_config_address;
    QLineEdit* edit_config_port;

    QPushButton* btn_confirm;

    QPushButton* btn_toConfigServerPage;
    QPushButton* btn_returnLoginPage;
};

#endif // LOGIN_H
