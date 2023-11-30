#include "cancel.h"
#include "ui_cancel.h"

cancel::cancel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::cancel)
{
    ui->setupUi(this);
}

cancel::~cancel()
{
    delete ui;
}

void cancel::updateUI()
{
    ui->cmbSeats->clear();

    QSqlQuery query(MyDB::getInstance()->getDBInstance());
    query.prepare("SELECT Seat FROM cinema_booking WHERE Available = 'N'");

    if (!query.exec()) {
        qDebug() << query.lastError().text() << query.lastQuery();
        ui->lblInfo->setText(query.lastError().text());
    }
    else {
        qDebug() << "Read was successful: " << query.lastQuery();
    }

    while (query.next()) {
        ui->cmbSeats->addItem(query.value(0).toString());
    }

    if (ui->cmbSeats->count() <= 0)
        ui->btnCancel->setEnabled(false);
    else
        ui->btnCancel->setEnabled(true); // Включение кнопки отмены при наличии доступных мест
}

void cancel::on_btnCancel_clicked()
{
    QString sSeat = ui->cmbSeats->currentText();


    QSqlQuery selectQuery(MyDB::getInstance()->getDBInstance());
    selectQuery.prepare("SELECT Id FROM cinema_booking WHERE Seat = ?");
    selectQuery.addBindValue(sSeat);
    int seatId = -1;

    if (selectQuery.exec() && selectQuery.next()) {
        seatId = selectQuery.value(0).toInt();
    } else {
        qDebug() << "Error occurred while retrieving seatId: " << selectQuery.lastError().text();
    }

    QSqlQuery deleteQuery(MyDB::getInstance()->getDBInstance());
    deleteQuery.prepare("DELETE FROM users WHERE seatId = ?");
    deleteQuery.addBindValue(seatId);

    if (deleteQuery.exec()) {
        if (deleteQuery.numRowsAffected() > 0) {
            ui->lblInfo->setText("User record deleted successfully.");
            qDebug() << "User record deleted successfully.";
        } else {
            ui->lblInfo->setText("No matching user record found.");
            qDebug() << "No matching user record found.";
        }
    } else {
        qDebug() << "Error occurred while deleting user record: " << deleteQuery.lastError().text();
        ui->lblInfo->setText(deleteQuery.lastError().text());
    }

    QSqlQuery query(MyDB::getInstance()->getDBInstance());
    query.prepare("update cinema_booking set Available='Y' where Seat= '" + sSeat +"'");

    if(!query.exec())
    {
        qDebug() << query.lastError().text() << query.lastQuery();
        ui->lblInfo->setText(query.lastError().text());
    }
    else{
        if(query.numRowsAffected() > 0)
        {
            ui->lblInfo->setText("Success!");
            qDebug() << "read was successful "<< query.lastQuery();
        }
        else
        {
            ui->lblInfo->setText("Unable to update data");
        }
       }

    updateUI();
    emit mysignal1();
}
