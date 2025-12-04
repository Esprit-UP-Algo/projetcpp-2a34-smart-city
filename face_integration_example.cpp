// EXEMPLE D'INTÉGRATION DANS VOTRE INTERFACE EMPLOYÉ
// Ajoutez ce code dans le slot de votre bouton "Capturer visage"

#include "employee.h"
#include "faceenrolldialog.h"
#include <QMessageBox>

// Dans votre classe d'interface (mainwindow ou autre)
// Ajoutez un membre pour stocker l'image capturée
private:
    QImage m_currentFaceImage;

// Slot pour le bouton "Capturer visage"
void VotreClasse::on_capturerVisage_clicked()
{
    // Utiliser notre méthode de capture
    QImage faceImage = employee::capturerVisage(this);
    
    if (!faceImage.isNull()) {
        m_currentFaceImage = faceImage;
        
        // Afficher un aperçu si vous avez un QLabel pour ça
        // ui->facePreviewLabel->setPixmap(QPixmap::fromImage(faceImage).scaled(
        //     ui->facePreviewLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        
        QMessageBox::information(this, "✅ Visage capturé", 
                               "Le visage a été capturé avec succès!\n"
                               "L'image sera associée à l'employé lors de l'ajout.");
    } else {
        QMessageBox::warning(this, "⚠️ Capture annulée", 
                           "Aucune image n'a été capturée.");
    }
}

// Modifiez votre slot d'ajout existant pour inclure l'image faciale
void VotreClasse::on_ajouter_clicked()
{
    // Récupérer les données des champs comme vous le faites déjà
    int cin = ui->cinLineEdit->text().toInt();
    QString nom = ui->nomLineEdit->text();
    QString prenom = ui->prenomLineEdit->text();
    QString email = ui->emailLineEdit->text();
    QString motdepasse = ui->motdepasseLineEdit->text();
    int telephone = ui->telephoneLineEdit->text().toInt();
    QString poste = ui->posteLineEdit->text();
    float salaire = ui->salaireLineEdit->text().toFloat();
    QDate dateEmbauche = ui->dateEmbaucheDateEdit->date();
    
    // Créer l'objet employé
    employee emp(cin, nom, prenom, email, motdepasse, telephone, poste, salaire, dateEmbauche);
    
    bool success = false;
    
    // Vérifier si nous avons une image faciale capturée
    if (!m_currentFaceImage.isNull()) {
        // Ajouter avec visage
        success = emp.ajouterAvecFace(m_currentFaceImage);
        if (success) {
            QMessageBox::information(this, "✅ Succès", 
                                   "Employé ajouté avec succès!\n"
                                   "Photo faciale enregistrée pour la reconnaissance.");
            m_currentFaceImage = QImage(); // Réinitialiser l'image
        }
    } else {
        // Ajouter sans visage (comportement normal)
        success = emp.ajouter();
        if (success) {
            QMessageBox::information(this, "✅ Succès", 
                                   "Employé ajouté avec succès!\n"
                                   "Aucune photo faciale n'a été associée.");
        }
    }
    
    if (!success) {
        QMessageBox::critical(this, "❌ Erreur", 
                            "Échec de l'ajout de l'employé.\n"
                            "Vérifiez les données saisies.");
    } else {
        // Rafraîchir l'affichage comme vous le faites déjà
        // ui->tableView->setModel(emp.afficher());
        // Vider les champs, etc.
    }
}
