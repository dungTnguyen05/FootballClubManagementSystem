#include "removestaff.h"
#include "ui_removestaff.h"

//Constructor with parameter
RemoveStaff::RemoveStaff(QWidget *parent): QWidget(parent), ui(new Ui::RemoveStaff) {
    ui -> setupUi(this);
    populateStaff();

    // Set the size of the label to be a square
    ui -> infoLabel -> setFixedSize(30, 30);

    // Set the stylesheet to add a circular border around the label
    ui -> infoLabel -> setStyleSheet("QLabel {"
                                     "border: 1px solid black;"
                                     "border-radius: 15px;" // Half of width/height
                                     "}");
}

//Default Destructor
RemoveStaff::~RemoveStaff() {
    qDebug() << "~RemoveStaff";
    delete ui;
}

void RemoveStaff::populateStaff() {
    // set database
    QSqlDatabase db = QSqlDatabase::database("DB1");

    // Check if the database is open
    if (!db.isOpen()) {
        qDebug() << "Database connection failed: ";
        return;
    }

    // set query
    QSqlQuery query(db);

    // check if query is executed
    if (!query.exec("SELECT StaffID, Name FROM Staff")) {
        qDebug() << "Query execution failed: " << query.lastError();
        return;
    }

    while (query.next()) {
        int staffID = query.value(0).toInt(); // Convert to int
        QString staffName = query.value(1).toString(); // Get the staff's name
        QString staffInfo = QString::number(staffID) + " - " + staffName; // Concatenate ID and name
        ui -> staffNameComboBox -> addItem(staffInfo); // Add the combined string to the combo box
    }
}

void RemoveStaff::on_staffRemoveConfirmButton_clicked() {
    QString staffInfo = ui -> staffNameComboBox -> currentText();
    QStringList infoParts = staffInfo.split(" - "); //Split the string into 2: staffID and Name

    if (infoParts.size() > 1) {
        int staffID = infoParts.at(0).toInt(); //StaffID is the first part of that string
        QSqlDatabase db = QSqlDatabase::database("DB1"); // set databse

        // set query
        QSqlQuery query(db);
        query.prepare("DELETE FROM Staff WHERE StaffID = :staffID");
        query.bindValue(":staffID", staffID);

        //Pop up message for situations
        if (query.exec()) {
            // update combo box
            QMessageBox::information(this, "Success", "Staff removed successfully\n"
                                                       "Please reload the table");
            ui -> staffNameComboBox -> removeItem(ui -> staffNameComboBox -> currentIndex());
        }

        else {
            QMessageBox::critical(this, "Error", "Could not remove staff " + query.lastError().text());        }
    }

    else {
        QMessageBox::critical(this, "Error", "Invalid selection.");
    }
}

