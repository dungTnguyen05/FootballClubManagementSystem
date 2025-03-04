#include "editstaff.h"
#include "ui_editstaff.h"

EditStaff::EditStaff( QWidget *parent): QWidget(parent), ui(new Ui::EditStaff) {
    ui -> setupUi(this);
    loadStaff();

    // Set the size of the label to be a square
    ui -> infoLabel -> setFixedSize(30, 30);

    // Set the stylesheet to add a circular border around the label
    ui -> infoLabel -> setStyleSheet("QLabel {"
                                     "border: 1px solid black;"
                                     "border-radius: 15px;" // Half of width/height
                                     "}");
}

EditStaff::~EditStaff() {
    delete ui;
}

void EditStaff::loadStaff() {
    // load staff id into the combo box
    QSqlDatabase db = QSqlDatabase::database("DB1");
    QSqlQuery query(db);

    if (!query.exec("SELECT StaffID FROM Staff")) {
        return;
    }

    // add the staff id into the staff id combo box
    while (query.next()) {
        ui -> staffIdComboBox -> addItem(query.value("StaffID").toString());
    }
}

void EditStaff::on_confirmButton_clicked() {
    // validate before consider edit as successful
    if (validateInput()) {
        QSqlDatabase db = QSqlDatabase::database("DB1");
        QSqlQuery query(db);

        // update the new information of staff into the database
        query.prepare("UPDATE Staff SET StaffID = :newStaffId, Name = :name, DOB = :dob, Role = :role, Salary = :salary WHERE StaffID = :oldStaffId");
        query.bindValue(":oldStaffId", ui -> staffIdComboBox -> currentText().toInt());
        //query.bindValue(":newStaffId", ui -> newStaffIdLineEdit -> text().isEmpty() ? ui -> staffIdComboBox -> currentText().toInt() : ui -> newStaffIdLineEdit -> text().toInt());

        int newStaffId;
        if (ui -> newStaffIdLineEdit -> text().isEmpty()) {
            newStaffId = ui -> staffIdComboBox -> currentText().toInt();
        } else {
            newStaffId = ui -> newStaffIdLineEdit -> text().toInt();
        }

        query.bindValue(":newStaffId", newStaffId);
        query.bindValue(":name", ui -> nameLineEdit -> text());
        query.bindValue(":dob", ui -> dobLineEdit -> text());
        query.bindValue(":role", ui -> roleLineEdit -> text());
        query.bindValue(":salary", ui -> salaryLineEdit -> text().toInt());

        // check if query is executed
        if (query.exec()) {
            if (!ui -> newStaffIdLineEdit -> text().isEmpty()) {
                ui -> staffIdComboBox -> setItemText(ui -> staffIdComboBox -> currentIndex(), ui -> newStaffIdLineEdit -> text());
            }

            QMessageBox::information(this, "Update successful", "Staff data updated successfully");
        }

        else {
            QMessageBox::warning(this, "Update error", "Failed to update staff data or new staff id is already existed");
        }
    }

    ui -> newStaffIdLineEdit -> clear(); // clear new staff line edit
}

bool EditStaff::validateInput() {
    // check all the lines are filled
    QString newStaffId = ui -> newStaffIdLineEdit -> text();
    QString salary = ui -> salaryLineEdit -> text();

    if (ui -> nameLineEdit -> text().isEmpty() ||
        ui -> dobLineEdit -> text().isEmpty() ||
        ui -> roleLineEdit -> text().isEmpty() ||
        ui -> salaryLineEdit -> text().isEmpty()) {
        QMessageBox::warning(this, "Input error", "Please fill in all lines");
        return false;
    }

    if (!ui -> newStaffIdLineEdit -> text().isEmpty()) {
        bool ok;
        ui -> newStaffIdLineEdit -> text().toInt(&ok);
        if (!ok) {
            QMessageBox::warning(this, "Input error", "Staff ID must be a valid integer");
            return false;
        }
    }

    // check if the new staff id is unique or not
    bool ok;
    int newStaffIdInt = newStaffId.toInt(&ok);

    QSqlQuery query;
    query.prepare("SELECT * FROM Staff WHERE StaffID = :staffId");
    query.bindValue(":staffId", newStaffIdInt);

    if (query.exec() && query.next() && query.value("StaffId").toInt() != newStaffIdInt) {
        QMessageBox::warning(this, "Input error", "New Staff ID already exists");
        return false;
    }

    // check if Salary is a positive integer
    int salaryInt = salary.toInt(&ok);

    if (!ok || salaryInt <= 0) {
        QMessageBox::warning(this, "Input error", "Salary must be a positive integer");
        return false;
    }

    return true;
}


void EditStaff::on_staffIdComboBox_currentTextChanged(const QString &staffId) {
    // show the information of the stat based on their id the user choose
    QSqlDatabase db = QSqlDatabase::database("DB1");
    QSqlQuery query(db);

    query.prepare("SELECT * FROM Staff WHERE StaffID = :staffId");
    query.bindValue(":staffId", staffId.toInt());

    if (query.exec() && query.next()) {
        ui -> nameLineEdit -> setText(query.value("Name").toString());
        ui -> dobLineEdit -> setText(query.value("DOB").toString());
        ui -> roleLineEdit -> setText(query.value("Role").toString());
        ui -> salaryLineEdit -> setText(query.value("Salary").toString());
    }

    else {
        QMessageBox::warning(this, "Load error", "Failed to load staff data");
    }
}

