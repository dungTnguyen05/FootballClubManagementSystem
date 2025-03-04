#include "chooseourxi.h"
#include "ui_chooseourxi.h"

ChooseOurXI::ChooseOurXI(QWidget *parent): QWidget(parent), ui(new Ui::ChooseOurXI) {
    ui -> setupUi(this);
    loadPlayers();

    // Set the size of the label to be a square
    ui -> infoLabel -> setFixedSize(30, 30);

    // Set the stylesheet to add a circular border around the label
    ui -> infoLabel -> setStyleSheet("QLabel {"
                                     "border: 1px solid black;"
                                     "border-radius: 15px;" // Half of width/height
                                     "}");
}

ChooseOurXI::~ChooseOurXI() {
    delete ui;
}

void ChooseOurXI::loadPlayers() {
    // set database
    QSqlDatabase db = QSqlDatabase::database("DB1");

    // set query
    QSqlQuery query(db);

    query.prepare("SELECT ShirtNumber, Position, Name, Selected FROM Player");

    // check if query is executed
    if (query.exec()) {
        while (query.next()) {
            QString playerInfo = QString("%1 - %2 - %3")
                                        .arg(query.value("ShirtNumber").toString())
                                        .arg(query.value("Position").toString())
                                        .arg(query.value("Name").toString());

            QListWidgetItem *item = new QListWidgetItem(playerInfo, ui -> listWidget);
            item -> setFlags(item -> flags() | Qt::ItemIsUserCheckable);

            // if player is already selected -> Checked, Unchecked otherwise
            if (query.value("Selected") == 1) {
                item -> setCheckState(Qt::Checked);
            }

            else {
                item -> setCheckState(Qt::Unchecked);
            }
        }
    }

    else {
        QMessageBox::critical(this, "Error", "Failed to load players");
    }
}

void ChooseOurXI::on_confirmButton_clicked() {
    // set database
    QSqlDatabase db = QSqlDatabase::database("DB1");

    // set query
    QSqlQuery query(db);

    // Set all players to unselected
    query.prepare("UPDATE Player SET Selected = 0");

    // check if query is executed
    if (!query.exec()) {
        QMessageBox::critical(this, "Error", "Failed to update player selection");
        return;
    }

    // prepare values
    query.prepare("UPDATE Player SET Selected = 1 WHERE ShirtNumber = :ShirtNumber");
    bool success = true;

    // update selected players
    for (int i = 0; i < ui -> listWidget -> count(); i++) {
        QListWidgetItem *item = ui -> listWidget -> item(i);

        if (item -> checkState() == Qt::Checked) {
            QString shirtNumber = item -> text().split(" - ").first();
            query.bindValue(":ShirtNumber", shirtNumber);

            // check if query is executed
            if (!query.exec()) {
                success = false;
                QMessageBox::critical(this, "Error", "Failed to update player selection");
                return; // avoid display multiple error messages - break when error happens
            }
        }
    }

    if (success) {
        QMessageBox::information(this, "Update Successfully", "Players are successfully selected\n"
                                                              "Please reload the table");
    }
}
