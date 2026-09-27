#include "Login.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QRegularExpression>
#include <QRegularExpressionValidator>

Login::Login(QWidget *parent) : QDialog(parent)
{
    initAllPages();

    regexCheck();

    initConnect();
}

Login::~Login()
{

}

void Login::initAllPages()
{
    QVBoxLayout* main_layout = new QVBoxLayout;

    stackedWidget = new QStackedWidget(this);
    QWidget* login_page = new QWidget;
    QWidget* register_page = new QWidget;
    QWidget* config_server_page =new QWidget;

    createLoginPage(login_page);
    createRegisterPage(register_page);
    createConfigServerPage(config_server_page);

    stackedWidget->addWidget(login_page);
    stackedWidget->addWidget(register_page);
    stackedWidget->addWidget(config_server_page);
    stackedWidget->setCurrentIndex(0);
    main_layout->addWidget(stackedWidget);

    QHBoxLayout* bottom_layout = new QHBoxLayout;
    btn_returnLoginPage = new QPushButton;
    btn_returnLoginPage->setText(u8"主页");
    btn_toConfigServerPage= new QPushButton;
    btn_toConfigServerPage->setText(u8"网络设置");
    bottom_layout->addWidget(btn_returnLoginPage);
    bottom_layout->addStretch();
    bottom_layout->addWidget(btn_toConfigServerPage);

    main_layout->addLayout(bottom_layout);

    this->setLayout(main_layout);
}

void Login::createLoginPage(QWidget *loginPage)
{
    QVBoxLayout* layout = new QVBoxLayout;

    QWidget* center = new QWidget;
    QGridLayout* center_layout = new QGridLayout;
    QLabel* lbl_user_name = new QLabel;
    lbl_user_name->setText(u8"用户名:");

    edit_login_userName = new QLineEdit;

    QLabel* lbl_user_password = new QLabel;
    lbl_user_password->setText(u8"密码:");

    edit_login_userPassword = new QLineEdit;

    box_isSavePassword = new QCheckBox;
    box_isSavePassword->setText(u8"保存密码");

    btn_toRegisterPage = new QPushButton;
    btn_toRegisterPage->setText(u8"没有账号, 马上注册");

    QHBoxLayout* btn_login_layout = new QHBoxLayout;
    btn_login_layout->addStretch();

    btn_login = new QPushButton;
    btn_login->setText(u8"登录");
    btn_login_layout->addWidget(btn_login);

    btn_login_layout->addStretch();

    center_layout->addWidget(lbl_user_name, 0, 0);
    center_layout->addWidget(edit_login_userName, 0, 1);
    center_layout->addWidget(lbl_user_password, 1, 0);
    center_layout->addWidget(edit_login_userPassword, 1, 1);
    center_layout->addWidget(box_isSavePassword, 2, 0);
    center_layout->addWidget(btn_toRegisterPage, 2, 1);
    center_layout->addLayout(btn_login_layout, 3, 0, 1, 2);
    center->setLayout(center_layout);

    layout->addWidget(center, 0, Qt::AlignCenter);
    loginPage->setLayout(layout);
}

void Login::createRegisterPage(QWidget *registerPage)
{
    QVBoxLayout* layout = new QVBoxLayout;

    QWidget* center = new QWidget;
    QGridLayout* center_layout = new QGridLayout;
    QLabel* lbl_user_name = new QLabel;
    lbl_user_name->setText(u8"用户名:");

    edit_register_userName = new QLineEdit;

    QLabel* lbl_user_password = new QLabel;
    lbl_user_password->setText(u8"密码:");

    edit_register_userPassword = new QLineEdit;

    QLabel* lbl_phone_number = new QLabel;
    lbl_phone_number->setText(u8"手机号");

    edit_register_phoneNumber = new QLineEdit;

    QHBoxLayout* btn_register_layout = new QHBoxLayout;
    btn_register_layout->addStretch();

    btn_register = new QPushButton;
    btn_register->setText(u8"注册");
    btn_register_layout->addWidget(btn_register);

    btn_register_layout->addStretch();

    center_layout->addWidget(lbl_user_name, 0, 0);
    center_layout->addWidget(edit_register_userName, 0, 1);
    center_layout->addWidget(lbl_user_password, 1, 0);
    center_layout->addWidget(edit_register_userPassword, 1, 1);
    center_layout->addWidget(lbl_phone_number, 2, 0);
    center_layout->addWidget(edit_register_phoneNumber, 2, 1);
    center_layout->addLayout(btn_register_layout, 3, 0, 1, 2);
    center->setLayout(center_layout);

    layout->addWidget(center, 0, Qt::AlignCenter);
    registerPage->setLayout(layout);
}

void Login::createConfigServerPage(QWidget *configServerPage)
{
    QVBoxLayout* layout = new QVBoxLayout;

    QWidget* center = new QWidget;
    QGridLayout* center_layout = new QGridLayout;
    QLabel* lbl_server_address = new QLabel;
    lbl_server_address->setText(u8"地址:");

    edit_config_address = new QLineEdit;

    QLabel* lbl_server_port = new QLabel;
    lbl_server_port->setText(u8"端口:");

    edit_config_port = new QLineEdit;

    QHBoxLayout* btn_confirm_layout = new QHBoxLayout;
    btn_confirm_layout->addStretch();

    btn_confirm = new QPushButton;
    btn_confirm->setText(u8"确认");
    btn_confirm_layout->addWidget(btn_confirm);

    btn_confirm_layout->addStretch();

    center_layout->addWidget(lbl_server_address, 0, 0);
    center_layout->addWidget(edit_config_address, 0, 1);
    center_layout->addWidget(lbl_server_port, 1, 0);
    center_layout->addWidget(edit_config_port, 1, 1);
    center_layout->addLayout(btn_confirm_layout, 2, 0, 1, 2);
    center->setLayout(center_layout);

    layout->addWidget(center, 0, Qt::AlignCenter);
    configServerPage->setLayout(layout);
}

void Login::initConnect()
{
    connect(btn_returnLoginPage, &QPushButton::clicked, this, [&](){
        stackedWidget->setCurrentIndex(0);
    });

    connect(btn_toRegisterPage, &QPushButton::clicked, this, [&](){
        stackedWidget->setCurrentIndex(1);
    });

    connect(btn_toConfigServerPage, &QPushButton::clicked, this, [&](){
        stackedWidget->setCurrentIndex(2);
    });

    connect(btn_login, &QPushButton::clicked, this, &Login::onLogin);
    connect(btn_register, &QPushButton::clicked, this, &Login::onRegister);
    connect(btn_confirm, &QPushButton::clicked, this, &Login::onSettingsConfirm);
}

void Login::regexCheck()
{
    /**
     * 用户名:
     *  1. 长度为3~12个字符
     *  2. 由a~z、A~Z、0~9、_组成
     */
    QRegularExpression expreg("^[a-zA-Z0-9_]{3,16}$");
    QRegularExpressionValidator* user_name_validator = new QRegularExpressionValidator(expreg, this);
    edit_login_userName->setValidator(user_name_validator);
    edit_register_userName->setValidator(user_name_validator);

    /**
     * 密码:
     *  1. 长度为4~20个字符
     *  2. 包含至少一个大写字母、小写字母、数字和特殊字符
     */
    expreg.setPattern("^(?=.*[A-Z])(?=.*[a-z])(?=.*\\d)(?=.*[@$!%*?&])[A-Za-z\\d@$!%*?&]{4,20}$");
    QRegularExpressionValidator* user_password_validator = new QRegularExpressionValidator(expreg, this);
    edit_login_userPassword->setValidator(user_password_validator);
    edit_register_userPassword->setValidator(user_password_validator);

    /**
     * 电话号码:
     *  1. 长度为11位数字
     *  2. 必须以1开头, 但不能以10、11、12开头
     */
    expreg.setPattern("^1[3456789]\\d{9}$");
    QRegularExpressionValidator* phone_number_validator = new QRegularExpressionValidator(expreg, this);
    edit_register_phoneNumber->setValidator(phone_number_validator);

    /**
     * IP地址:
     *  1. 符合IPV4协议: 0.0.0.0~255.255.255.255
     */
    expreg.setPattern("^((\\d|[1-9]\\d|1\\d{2}|2[0-4]\\d|25[0-5])\\.){3}(\\d|[1-9]\\d|1\\d{2}|2[0-4]\\d|25[0-5])$");
    QRegularExpressionValidator* ip_address_validator = new QRegularExpressionValidator(expreg, this);
    edit_config_address->setValidator(ip_address_validator);

    /**
     * 端口号:
     *  1. 满足: 0~65535
     */
    expreg.setPattern("^(0|([1-9]\\d{0,3})|([1-5]\\d{4})|(6[0-4]\\d{3})|(65[0-4]\\d{2})|(655[0-2]\\d)|(6553[0-5]))$");
    QRegularExpressionValidator* port_validator = new QRegularExpressionValidator(expreg, this);
    edit_config_port->setValidator(port_validator);
}

bool Login::verifyData(QLineEdit* edit)
{
    if (!edit->hasAcceptableInput())
    {
        edit->setStyleSheet("border: 2px solid red;");
        return false;
    }
    else
    {
        edit->setStyleSheet("none");
    }
    return true;
}

void Login::onLogin()
{
    bool name_flag = verifyData(edit_login_userName);
    bool password_flag = verifyData(edit_login_userPassword);
    if (name_flag && password_flag)
    {

    }
}

void Login::onRegister()
{
    bool name_flag = verifyData(edit_register_userName);
    bool password_flag = verifyData(edit_register_userPassword);
    bool phone_flag = verifyData(edit_register_phoneNumber);
    if (name_flag && password_flag && phone_flag)
    {

    }
}

void Login::onSettingsConfirm()
{
    bool adress_flag = verifyData(edit_config_address);
    bool port_flag = verifyData(edit_config_port);
    if (adress_flag && port_flag)
    {

    }
}
