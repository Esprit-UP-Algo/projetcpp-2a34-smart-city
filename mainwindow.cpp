#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "employee.h"
#include <QMessageBox>
#include <QRegularExpression>
#include <QPrinter>
#include <QPainter>
#include <QFileDialog>
#include <QTextDocument>
#include <QSqlQuery>
#include <QInputDialog>
#include <QDate>
#include <QDebug>
#include <QDir>
#include <QStringListModel>
#include <QMapIterator>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tab_emp->setModel(E.afficher());

    // Permettre la sélection d'une ligne entière
    ui->tab_emp->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tab_emp->setSelectionMode(QAbstractItemView::SingleSelection);

    // Ajuster la largeur des colonnes
    ui->tab_emp->resizeColumnsToContents();

    for(int i = 0; i < ui->tab_emp->model()->columnCount(); i++) {
        ui->tab_emp->setColumnWidth(i, 100);
    }

    // Connecter la combobox de tri
    connect(ui->comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::on_comboBox_currentIndexChanged);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Fonctions de validation
bool MainWindow::validerNom(const QString &nom)
{
    if (nom.isEmpty()) {
        return false;
    }
    QRegularExpression regex("[0-9]");
    if (regex.match(nom).hasMatch()) {
        return false;
    }
    return true;
}

bool MainWindow::validerEmail(const QString &email)
{
    if (email.isEmpty()) {
        return false;
    }
    QRegularExpression regex("^[a-zA-Z0-9._-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");
    return regex.match(email).hasMatch();
}

bool MainWindow::validerTelephone(const QString &telephone)
{
    if (telephone.isEmpty()) {
        return false;
    }
    QRegularExpression regex("^[0-9]{8}$");
    return regex.match(telephone).hasMatch();
}

bool MainWindow::validerSalaire(const QString &salaire)
{
    if (salaire.isEmpty()) {
        return false;
    }
    bool ok;
    float val = salaire.toFloat(&ok);
    return ok && val > 0;
}

bool MainWindow::validerCIN(const QString &cin)
{
    if (cin.isEmpty()) {
        return false;
    }
    QRegularExpression regex("^[0-9]{8}$");
    return regex.match(cin).hasMatch();
}

void MainWindow::on_ajouter_clicked()
{
    QString cinStr = ui->lineEdit_cin->text();
    QString nom = ui->lineEdit_nom->text();
    QString prenom = ui->lineEdit_prenom->text();
    QString email = ui->lineEdit_email->text();
    QString telephoneStr = ui->lineEdit_telephon->text();
    QString salaireStr = ui->lineEdit_salaire->text();

    if (!validerCIN(cinStr)) {
        QMessageBox::warning(this, "Erreur de saisie",
                             "Le CIN doit contenir exactement 8 chiffres!");
        return;
    }

    if (!validerNom(nom)) {
        QMessageBox::warning(this, "Erreur de saisie",
                             "Le nom ne doit pas contenir de chiffres!");
        return;
    }

    if (!validerNom(prenom)) {
        QMessageBox::warning(this, "Erreur de saisie",
                             "Le prénom ne doit pas contenir de chiffres!");
        return;
    }

    if (!validerEmail(email)) {
        QMessageBox::warning(this, "Erreur de saisie",
                             "L'email doit contenir @ et être dans un format valide!\n"
                             "Exemple: nom@domaine.com");
        return;
    }

    if (!validerTelephone(telephoneStr)) {
        QMessageBox::warning(this, "Erreur de saisie",
                             "Le numéro de téléphone doit contenir exactement 8 chiffres!");
        return;
    }

    if (!validerSalaire(salaireStr)) {
        QMessageBox::warning(this, "Erreur de saisie",
                             "Le salaire doit être un nombre positif!");
        return;
    }

    int cin = cinStr.toInt();
    QString motdepass = ui->lineEdit_motdepass->text();
    int n_telephon = telephoneStr.toInt();
    QString poste = ui->lineEdit_poste->text();
    float salaire = salaireStr.toFloat();

    employee E(cin, nom, prenom, email, motdepass, n_telephon, poste, salaire);

    bool test = E.ajouter();
    if (test)
    {
        ui->tab_emp->setModel(E.afficher());
        ui->tab_emp->resizeColumnsToContents();
        QMessageBox::information(this, "Succès",
                                 "Ajout effectué avec succès!");

        ui->lineEdit_cin->clear();
        ui->lineEdit_nom->clear();
        ui->lineEdit_prenom->clear();
        ui->lineEdit_email->clear();
        ui->lineEdit_motdepass->clear();
        ui->lineEdit_telephon->clear();
        ui->lineEdit_poste->clear();
        ui->lineEdit_salaire->clear();
    }
    else
    {
        QMessageBox::critical(this, "Erreur",
                              "Ajout non effectué. Vérifiez que le CIN n'existe pas déjà.");
    }
}

void MainWindow::on_refresh_clicked()
{
    ui->tab_emp->setModel(E.afficher());
    ui->tab_emp->resizeColumnsToContents();
}

void MainWindow::on_refresh_2_clicked()
{
    qDebug() << "Recherche par CIN activée";

    bool ok;
    QString cinStr = QInputDialog::getText(this, "Recherche par CIN",
                                           "Entrez le CIN à rechercher (8 chiffres):",
                                           QLineEdit::Normal, "", &ok);

    if (!ok || cinStr.isEmpty()) {
        return;
    }

    if (!validerCIN(cinStr)) {
        QMessageBox::warning(this, "Erreur",
                             "Le CIN doit contenir exactement 8 chiffres!");
        return;
    }

    int cin = cinStr.toInt();
    QSqlQueryModel* model = E.rechercherParCIN(cin);

    if (model) {
        ui->tab_emp->setModel(model);
        ui->tab_emp->resizeColumnsToContents();

        if (ui->tab_emp->model()->rowCount() == 0) {
            QMessageBox::information(this, "Recherche",
                                     "Aucun employé trouvé avec ce CIN.");
            ui->tab_emp->setModel(E.afficher());
        } else {
            QMessageBox::information(this, "Recherche",
                                     "Employé trouvé!");
        }
    } else {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la recherche!");
    }
}

// AFFICHER STATISTIQUES DANS LE LISTVIEW
void MainWindow::on_refresh_3_clicked()
{
    afficherStatistiques();
}

void MainWindow::on_tab_emp_clicked(const QModelIndex &index)
{
    if (!index.isValid()) {
        return;
    }

    int row = index.row();

    QString cin = ui->tab_emp->model()->index(row, 1).data().toString();
    QString nom = ui->tab_emp->model()->index(row, 2).data().toString();
    QString prenom = ui->tab_emp->model()->index(row, 3).data().toString();
    QString email = ui->tab_emp->model()->index(row, 4).data().toString();
    QString motdepass = ui->tab_emp->model()->index(row, 5).data().toString();
    QString telephone = ui->tab_emp->model()->index(row, 6).data().toString();
    QString poste = ui->tab_emp->model()->index(row, 7).data().toString();
    QString salaire = ui->tab_emp->model()->index(row, 8).data().toString();

    ui->lineEdit_cin2->setText(cin);
    ui->lineEdit_nom2->setText(nom);
    ui->lineEdit_prenom2->setText(prenom);
    ui->lineEdit_email2->setText(email);
    ui->lineEdit_motdepass2->setText(motdepass);
    ui->lineEdit_tel2->setText(telephone);
    ui->lineEdit_poste_2->setText(poste);
    ui->lineEdit_salaire_2->setText(salaire);
}

void MainWindow::on_modifier_clicked()
{
    QString cinStr = ui->lineEdit_cin2->text();
    QString nom = ui->lineEdit_nom2->text();
    QString prenom = ui->lineEdit_prenom2->text();
    QString email = ui->lineEdit_email2->text();
    QString telephoneStr = ui->lineEdit_tel2->text();
    QString salaireStr = ui->lineEdit_salaire_2->text();

    if (!validerCIN(cinStr)) {
        QMessageBox::warning(this, "Erreur de saisie",
                             "Le CIN doit contenir exactement 8 chiffres!");
        return;
    }

    if (!validerNom(nom)) {
        QMessageBox::warning(this, "Erreur de saisie",
                             "Le nom ne doit pas contenir de chiffres!");
        return;
    }

    if (!validerNom(prenom)) {
        QMessageBox::warning(this, "Erreur de saisie",
                             "Le prénom ne doit pas contenir de chiffres!");
        return;
    }

    if (!validerEmail(email)) {
        QMessageBox::warning(this, "Erreur de saisie",
                             "L'email doit contenir @ et être dans un format valide!\n"
                             "Exemple: nom@domaine.com");
        return;
    }

    if (!validerTelephone(telephoneStr)) {
        QMessageBox::warning(this, "Erreur de saisie",
                             "Le numéro de téléphone doit contenir exactement 8 chiffres!");
        return;
    }

    if (!validerSalaire(salaireStr)) {
        QMessageBox::warning(this, "Erreur de saisie",
                             "Le salaire doit être un nombre positif!");
        return;
    }

    int cin = cinStr.toInt();
    QString motdepass = ui->lineEdit_motdepass2->text();
    int n_telephon = telephoneStr.toInt();
    QString poste = ui->lineEdit_poste_2->text();
    float salaire = salaireStr.toFloat();

    employee E(cin, nom, prenom, email, motdepass, n_telephon, poste, salaire);
    bool test = E.modifier(cin, nom, prenom, email, motdepass, n_telephon, poste, salaire);

    if (test)
    {
        ui->tab_emp->setModel(E.afficher());
        ui->tab_emp->resizeColumnsToContents();
        QMessageBox::information(this, "Succès",
                                 "Employé modifié avec succès!");

        ui->lineEdit_cin2->clear();
        ui->lineEdit_nom2->clear();
        ui->lineEdit_prenom2->clear();
        ui->lineEdit_email2->clear();
        ui->lineEdit_motdepass2->clear();
        ui->lineEdit_tel2->clear();
        ui->lineEdit_poste_2->clear();
        ui->lineEdit_salaire_2->clear();
    }
    else
    {
        QMessageBox::critical(this, "Erreur",
                              "Erreur lors de la modification!");
    }
}

void MainWindow::on_supprimer_clicked()
{
    QModelIndexList selection = ui->tab_emp->selectionModel()->selectedRows();
    if (selection.isEmpty()) {
        QMessageBox::warning(this, "Attention",
                             "Veuillez sélectionner un employé à supprimer.");
        return;
    }

    int row = selection.at(0).row();
    QString id = ui->tab_emp->model()->index(row, 0).data().toString();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation",
                                  "Êtes-vous sûr de vouloir supprimer cet employé?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::No) {
        return;
    }

    employee E;
    bool test = E.supprimer(id);

    if (test) {
        ui->tab_emp->setModel(E.afficher());
        ui->tab_emp->resizeColumnsToContents();
        QMessageBox::information(this, "Succès",
                                 "Suppression effectuée avec succès!");
    } else {
        QMessageBox::critical(this, "Erreur",
                              "Suppression non effectuée!");
    }
}

// NOUVELLE FONCTION POUR LE TRI VIA COMBOBOX
void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    QSqlQueryModel* model = nullptr;

    switch(index) {
    case 0: // "trie par salaire"
        model = E.trierParSalaire(true); // Croissant
        QMessageBox::information(this, "Tri", "Liste triée par salaire (croissant)");
        break;
    case 1: // "trie par nom"
        model = E.trierParNom();
        QMessageBox::information(this, "Tri", "Liste triée par nom (A-Z)");
        break;
    default:
        model = E.afficher();
        break;
    }

    if (model) {
        ui->tab_emp->setModel(model);
        ui->tab_emp->resizeColumnsToContents();
    }
}

void MainWindow::on_pushButton_5_clicked()
{
    qDebug() << "Export PDF activé";

    QString fileName = QFileDialog::getSaveFileName(this,
                                                    "Enregistrer le PDF",
                                                    QDir::homePath() + "/liste_employes.pdf",
                                                    "Fichiers PDF (*.pdf)");

    if (fileName.isEmpty()) {
        return;
    }

    if (!fileName.endsWith(".pdf", Qt::CaseInsensitive)) {
        fileName += ".pdf";
    }

    genererPDF(fileName);

    QMessageBox::information(this, "Succès",
                             "Le fichier PDF a été créé avec succès!\n\n" + fileName);
}

void MainWindow::on_pushButton_6_clicked()
{
    ui->lineEdit_cin2->clear();
    ui->lineEdit_nom2->clear();
    ui->lineEdit_prenom2->clear();
    ui->lineEdit_email2->clear();
    ui->lineEdit_motdepass2->clear();
    ui->lineEdit_tel2->clear();
    ui->lineEdit_poste_2->clear();
    ui->lineEdit_salaire_2->clear();

    QMessageBox::information(this, "Annulation", "Modification annulée.");
}

void MainWindow::on_pushButton_7_clicked()
{
    ui->lineEdit_cin->clear();
    ui->lineEdit_nom->clear();
    ui->lineEdit_prenom->clear();
    ui->lineEdit_email->clear();
    ui->lineEdit_motdepass->clear();
    ui->lineEdit_telephon->clear();
    ui->lineEdit_poste->clear();
    ui->lineEdit_salaire->clear();

    QMessageBox::information(this, "Annulation", "Ajout annulé.");
}

// GENERATION PDF LISTE EMPLOYÉS
void MainWindow::genererPDF(const QString &fileName)
{
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    printer.setPageSize(QPageSize::A4);
    printer.setPageOrientation(QPageLayout::Landscape);

    QPainter painter;
    if (!painter.begin(&printer)) {
        QMessageBox::critical(this, "Erreur", "Impossible de créer le PDF");
        return;
    }

    // Titre
    QFont titleFont = painter.font();
    titleFont.setPointSize(18);
    titleFont.setBold(true);
    painter.setFont(titleFont);
    painter.drawText(100, 200, "LISTE DES EMPLOYÉS");

    // Date
    QFont normalFont = painter.font();
    normalFont.setPointSize(11);
    normalFont.setBold(false);
    painter.setFont(normalFont);
    painter.drawText(100, 300, "Date d'export: " + QDate::currentDate().toString("dd/MM/yyyy"));

    // En-têtes du tableau
    int y = 500;
    int x = 100;
    int colWidths[] = {180, 200, 200, 280, 180, 200, 180};

    QFont headerFont = painter.font();
    headerFont.setPointSize(11);
    headerFont.setBold(true);
    painter.setFont(headerFont);

    painter.drawText(x, y, "CIN");
    painter.drawText(x + colWidths[0], y, "Nom");
    painter.drawText(x + colWidths[0] + colWidths[1], y, "Prénom");
    painter.drawText(x + colWidths[0] + colWidths[1] + colWidths[2], y, "Email");
    painter.drawText(x + colWidths[0] + colWidths[1] + colWidths[2] + colWidths[3], y, "Téléphone");
    painter.drawText(x + colWidths[0] + colWidths[1] + colWidths[2] + colWidths[3] + colWidths[4], y, "Poste");
    painter.drawText(x + colWidths[0] + colWidths[1] + colWidths[2] + colWidths[3] + colWidths[4] + colWidths[5], y, "Salaire");

    // Ligne sous les en-têtes
    y += 30;
    int totalWidth = colWidths[0] + colWidths[1] + colWidths[2] + colWidths[3] + colWidths[4] + colWidths[5] + colWidths[6];
    painter.drawLine(x, y, x + totalWidth, y);

    // Données
    painter.setFont(normalFont);
    y += 50;

    QSqlQuery query("SELECT CIN, NOM, PRENOM, EMAIL, N_TELEPHON, POSTE, SALAIRE FROM EMPLOYE ORDER BY NOM");

    int rowCount = 0;
    while (query.next()) {
        if (y > 3000) {
            printer.newPage();
            y = 200;
        }

        painter.drawText(x, y, query.value(0).toString());
        painter.drawText(x + colWidths[0], y, query.value(1).toString());
        painter.drawText(x + colWidths[0] + colWidths[1], y, query.value(2).toString());
        painter.drawText(x + colWidths[0] + colWidths[1] + colWidths[2], y, query.value(3).toString());
        painter.drawText(x + colWidths[0] + colWidths[1] + colWidths[2] + colWidths[3], y, query.value(4).toString());
        painter.drawText(x + colWidths[0] + colWidths[1] + colWidths[2] + colWidths[3] + colWidths[4], y, query.value(5).toString());
        painter.drawText(x + colWidths[0] + colWidths[1] + colWidths[2] + colWidths[3] + colWidths[4] + colWidths[5], y,
                         QString::number(query.value(6).toFloat(), 'f', 2) + " DT");

        y += 50;
        rowCount++;
    }

    // Pied de page
    painter.setFont(normalFont);
    painter.drawText(100, 3200, QString("Total: %1 employé(s)").arg(rowCount));

    painter.end();
}

// AFFICHER STATISTIQUES DANS LE LISTVIEW
void MainWindow::afficherStatistiques()
{
    // Créer un modèle pour le ListView
    QStringListModel *model = new QStringListModel(this);
    QStringList statistiques;

    // Récupérer les statistiques
    int totalEmployes = E.nombreEmployes();
    float moyenneGlobale = E.salaireMoyen();
    float salaireMin = E.salaireMin();
    float salaireMax = E.salaireMax();

    QMap<QString, int> statsParPoste = E.statistiquesParPoste();
    QMap<QString, float> salaireMoyenPoste = E.salaireMoyenParPoste();

    // Construire la liste des statistiques
    statistiques << "═══════════════════════════════════════";
    statistiques << "       STATISTIQUES DES EMPLOYÉS";
    statistiques << "═══════════════════════════════════════";
    statistiques << "";

    statistiques << "📊 STATISTIQUES GLOBALES";
    statistiques << "───────────────────────────────────────";
    statistiques << QString("• Nombre total d'employés: %1").arg(totalEmployes);
    statistiques << QString("• Salaire moyen: %1 DT").arg(moyenneGlobale, 0, 'f', 2);
    statistiques << QString("• Salaire minimum: %1 DT").arg(salaireMin, 0, 'f', 2);
    statistiques << QString("• Salaire maximum: %1 DT").arg(salaireMax, 0, 'f', 2);
    statistiques << QString("• Écart salarial: %1 DT").arg(salaireMax - salaireMin, 0, 'f', 2);
    statistiques << "";

    if (!statsParPoste.isEmpty()) {
        statistiques << "💼 RÉPARTITION PAR POSTE";
        statistiques << "───────────────────────────────────────";

        QMapIterator<QString, int> i(statsParPoste);
        while (i.hasNext()) {
            i.next();
            QString poste = i.key();
            int nombre = i.value();
            float moyenne = salaireMoyenPoste.value(poste, 0.0);
            float pourcentage = (float)nombre / totalEmployes * 100;

            statistiques << QString("• %1:").arg(poste);
            statistiques << QString("  - Employés: %1 (%2%)")
                                .arg(nombre)
                                .arg(pourcentage, 0, 'f', 1);
            statistiques << QString("  - Salaire moyen: %1 DT")
                                .arg(moyenne, 0, 'f', 2);
            statistiques << "";
        }
    }

    statistiques << "═══════════════════════════════════════";
    statistiques << QString("Date: %1").arg(QDate::currentDate().toString("dd/MM/yyyy"));

    // Appliquer le modèle au ListView
    model->setStringList(statistiques);
    ui->listView->setModel(model);

    QMessageBox::information(this, "Statistiques",
                             "Les statistiques ont été chargées dans la liste!");
}

// GENERATION PDF STATISTIQUES
void MainWindow::genererPDFStatistiques()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    "Enregistrer les statistiques",
                                                    QDir::homePath() + "/statistiques_employes.pdf",
                                                    "Fichiers PDF (*.pdf)");

    if (fileName.isEmpty()) {
        return;
    }

    if (!fileName.endsWith(".pdf", Qt::CaseInsensitive)) {
        fileName += ".pdf";
    }

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    printer.setPageSize(QPageSize::A4);

    QPainter painter;
    if (!painter.begin(&printer)) {
        QMessageBox::critical(this, "Erreur", "Impossible de créer le PDF");
        return;
    }

    // Titre
    QFont titleFont = painter.font();
    titleFont.setPointSize(20);
    titleFont.setBold(true);
    painter.setFont(titleFont);
    painter.drawText(100, 200, "STATISTIQUES DES EMPLOYÉS");

    // Date
    QFont normalFont = painter.font();
    normalFont.setPointSize(11);
    normalFont.setBold(false);
    painter.setFont(normalFont);
    painter.drawText(100, 300, "Date: " + QDate::currentDate().toString("dd/MM/yyyy"));

    int y = 500;

    // Statistiques globales
    QFont sectionFont = painter.font();
    sectionFont.setPointSize(14);
    sectionFont.setBold(true);
    painter.setFont(sectionFont);
    painter.drawText(100, y, "Statistiques Globales");
    y += 80;

    painter.setFont(normalFont);
    painter.drawText(100, y, QString("Nombre total d'employés: %1").arg(E.nombreEmployes()));
    y += 50;
    painter.drawText(100, y, QString("Salaire moyen: %1 DT").arg(E.salaireMoyen(), 0, 'f', 2));
    y += 50;
    painter.drawText(100, y, QString("Salaire minimum: %1 DT").arg(E.salaireMin(), 0, 'f', 2));
    y += 50;
    painter.drawText(100, y, QString("Salaire maximum: %1 DT").arg(E.salaireMax(), 0, 'f', 2));
    y += 100;

    // Statistiques par poste
    painter.setFont(sectionFont);
    painter.drawText(100, y, "Répartition par Poste");
    y += 80;

    painter.setFont(normalFont);
    QMap<QString, int> statsParPoste = E.statistiquesParPoste();
    QMap<QString, float> salaireMoyenPoste = E.salaireMoyenParPoste();

    QMapIterator<QString, int> i(statsParPoste);
    while (i.hasNext()) {
        i.next();
        QString poste = i.key();
        int nombre = i.value();
        float moyenne = salaireMoyenPoste.value(poste, 0.0);

        painter.drawText(100, y, QString("%1: %2 employé(s) - Moyenne: %3 DT")
                                     .arg(poste)
                                     .arg(nombre)
                                     .arg(moyenne, 0, 'f', 2));
        y += 50;
    }

    painter.end();

    QMessageBox::information(this, "Succès",
                             "Les statistiques ont été exportées avec succès!\n\n" + fileName);
}
