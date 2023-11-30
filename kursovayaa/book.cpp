#include "book.h"
#include "ui_book.h"
#include <QMessageBox>


book::book(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::book)
{
    ui->setupUi(this);
    validColor = Qt::white;
    invalidColor = Qt::red;
}

book::~book()
{
    delete ui;
}

void book::updateUI()
{
    ui->cmbSeats->clear();
    //ui->lblInfo->setText("");

    QSqlQuery query(MyDB::getInstance()->getDBInstance());
    query.prepare("select Seat from cinema_booking where Available='Y'");

    if(!query.exec())
    {
        qDebug() << query.lastError().text() << query.lastQuery();
        ui->lblInfo->setText(query.lastError().text());
    }
    else{
        qDebug() << "read was successful "<< query.lastQuery();
       }

    while(query.next())
    {
        ui->cmbSeats->addItem(query.value(0).toString());
    }

    if(ui->cmbSeats->count()<=0)
        ui->btnBook->setEnabled(false);
}

void book::on_lineEdit_textEdited(const QString &arg1)
{

}

void book::on_lineEdit_2_textEdited(const QString &arg1)
{

}

void book::on_btnBook_clicked()
{
    QString sSeat = ui->cmbSeats->currentText();

    QString name = ui->lineEdit->text();
    QString surname = ui->lineEdit_2->text();


        QRegExp nameRegExp("[A-Za-z]{1,15}"); // Регулярное выражение для имени (только буквы, длина до 15 символов)
        bool isNameValid = nameRegExp.exactMatch(name);

        QRegExp surnameRegExp("[A-Za-z]{1,15}"); // Регулярное выражение для фамилии (только буквы, длина до 15 символов)
        bool isSurnameValid = surnameRegExp.exactMatch(surname);

        if (isNameValid) {
            ui->lineEdit->setStyleSheet("QLineEdit { background-color: " + validColor.name() + "; }");
        } else {
            ui->lineEdit->setStyleSheet("QLineEdit { background-color: " + invalidColor.name() + "; }");
        }

        if (isSurnameValid) {
            ui->lineEdit_2->setStyleSheet("QLineEdit { background-color: " + validColor.name() + "; }");
        } else {
            ui->lineEdit_2->setStyleSheet("QLineEdit { background-color: " + invalidColor.name() + "; }");
        }

        if (!isNameValid || !isSurnameValid) {
            QMessageBox::critical(this, "Error", "Please enter valid name and surname.");
            return;
        }



    QSqlQuery query(MyDB::getInstance()->getDBInstance());
    query.prepare("SELECT Id FROM cinema_booking WHERE Seat = ?");
    query.addBindValue(sSeat);
    int seatId;

    if (query.exec()) {
        if (query.next()) {
            seatId = query.value(0).toInt();
        } else {
            QMessageBox::critical(this, "Error", "Seat not found.");
        }
    } else {
        qDebug() << "Database error occurred: " << query.lastError().text();
    }


    QSqlQuery insertQuery(MyDB::getInstance()->getDBInstance());
    insertQuery.prepare("INSERT INTO users (name, surname, seatId) VALUES (?, ?, ?)");
    insertQuery.addBindValue(name);
    insertQuery.addBindValue(surname);
    insertQuery.addBindValue(seatId);

    if (insertQuery.exec()) {
        QMessageBox::information(this, "Success", "Booking successful.");
    } else {
        qDebug() << "Database error occurred: " << insertQuery.lastError().text();
        QMessageBox::critical(this, "Error", "Booking failed. Please try again.");
    }


    QSqlQuery updateQuery(MyDB::getInstance()->getDBInstance());
    updateQuery.prepare("UPDATE cinema_booking SET Available = 'N' WHERE Id = ?");
    updateQuery.addBindValue(seatId);

    if (updateQuery.exec()) {
        qDebug() << "Availability updated for seatId: " << seatId;
    } else {
        qDebug() << "Database error occurred: " << updateQuery.lastError().text();
    }
    updateUI();
    emit mysignal();
}



