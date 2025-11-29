/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QTabWidget *tabWidget;
    QWidget *tab_2;
    QLabel *label_13;
    QPushButton *pushButton_4;
    QComboBox *comboBox_2;
    QPushButton *pushButton_12;
    QTabWidget *tabWidget_3;
    QWidget *tab_8;
    QLabel *label_32;
    QLabel *label_33;
    QLabel *label_34;
    QLabel *label_35;
    QLabel *label_36;
    QLabel *label_37;
    QLineEdit *lineEdit_30;
    QLineEdit *lineEdit_31;
    QLineEdit *lineEdit_32;
    QLineEdit *lineEdit_33;
    QLineEdit *lineEdit_34;
    QLineEdit *lineEdit_35;
    QPushButton *pushButton_13;
    QPushButton *pushButton_14;
    QLabel *label_38;
    QLineEdit *lineEdit_36;
    QWidget *tab_9;
    QLineEdit *lineEdit_37;
    QLabel *label_39;
    QLineEdit *lineEdit_38;
    QLabel *label_40;
    QLineEdit *lineEdit_39;
    QLabel *label_41;
    QLineEdit *lineEdit_40;
    QLineEdit *lineEdit_41;
    QLineEdit *lineEdit_42;
    QLabel *label_42;
    QPushButton *pushButton_15;
    QLabel *label_43;
    QLabel *label_44;
    QPushButton *pushButton_16;
    QTableView *tableView;
    QComboBox *comboBox;
    QLabel *label_45;
    QWidget *tab;
    QPushButton *refresh_2;
    QPushButton *pushButton_5;
    QPushButton *pushButton_3;
    QPushButton *refresh;
    QLabel *label_14;
    QLineEdit *lineEdit_13;
    QPushButton *supprimer;
    QFrame *frame;
    QTableView *tab_emp;
    QTabWidget *tabWidget_2;
    QWidget *tab_3;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QLineEdit *lineEdit_cin;
    QLineEdit *lineEdit_nom;
    QLineEdit *lineEdit_prenom;
    QLineEdit *lineEdit_email;
    QLineEdit *lineEdit_motdepass;
    QLineEdit *lineEdit_telephon;
    QPushButton *ajouter;
    QPushButton *pushButton_7;
    QLabel *label_15;
    QLineEdit *lineEdit_poste;
    QLabel *label_16;
    QLineEdit *lineEdit_salaire;
    QWidget *tab_4;
    QLineEdit *lineEdit_cin2;
    QLabel *label_7;
    QLineEdit *lineEdit_email2;
    QLabel *label_8;
    QLineEdit *lineEdit_tel2;
    QLabel *label_9;
    QLineEdit *lineEdit_prenom2;
    QLineEdit *lineEdit_motdepass2;
    QLineEdit *lineEdit_nom2;
    QLabel *label_10;
    QPushButton *modifier;
    QLabel *label_11;
    QLabel *label_12;
    QPushButton *pushButton_6;
    QLineEdit *lineEdit_poste_2;
    QLabel *label_17;
    QLabel *label_18;
    QLineEdit *lineEdit_salaire_2;
    QWidget *tab_5;
    QPushButton *refresh_3;
    QListView *listView;
    QWidget *tab_dechet;
    QTabWidget *statistiques_dechet_btn;
    QWidget *tab_23;
    QLabel *cin;
    QLabel *date;
    QLabel *lieu;
    QLabel *volume;
    QLabel *etat;
    QLineEdit *cin_dechet;
    QPushButton *ajouter_dechet_btn;
    QPushButton *annuler_dechet_btn;
    QDateEdit *date_dechet;
    QLineEdit *lieu_dechet;
    QLineEdit *volume_dechet;
    QRadioButton *recycle;
    QRadioButton *nonrecycle;
    QWidget *tab_24;
    QPushButton *modifier_dechet_btn;
    QPushButton *anuuler_modifier_dechet_btn;
    QLineEdit *volume_dechet_modifier;
    QLabel *date_dechet_mod;
    QDateEdit *date_dechet_modifier;
    QLineEdit *lieu_dechet_modifier;
    QLineEdit *cin_dechet_modifier;
    QLabel *lieu_dechet_mod;
    QLabel *cin_dechet_mod;
    QLabel *etat_dechet_mod;
    QLabel *volume_dechet_mod;
    QRadioButton *recycle_modifier;
    QRadioButton *nonrecycle_modifier;
    QWidget *tab_25;
    QPushButton *afficher_stats_tableau_btn;
    QGraphicsView *graphicsView_dechet_stat;
    QLineEdit *lineEdit_rech_dechet;
    QPushButton *supprimer_dechet_btn;
    QTableView *tableView_dechet;
    QPushButton *trier_dechet_btn;
    QPushButton *refresh_dechet_btn;
    QPushButton *rech_dechet_btn;
    QPushButton *export_pdf_stats_btn;
    QComboBox *comboBox_trier_dechet;
    QComboBox *comboBox_rech_dechet;
    QLineEdit *lineEdit_supprimer_dechet;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1519, 768);
        MainWindow->setStyleSheet(QString::fromUtf8("QWidget {\n"
"    background-color: #e6f7f5; /* light greenish background */\n"
"    color: #003366; /* dark blue text */\n"
"    font-size: 14px;\n"
"}\n"
"\n"
"QPushButton {\n"
"    background-color: #4CAF50; /* green button */\n"
"    color: white;\n"
"    border: none;\n"
"    padding: 6px 12px;\n"
"    border-radius: 6px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #45a049; /* darker green on hover */\n"
"}\n"
"\n"
"QLineEdit, QTextEdit {\n"
"    background-color: #ffffff;\n"
"    border: 2px solid #3399ff; /* blue border */\n"
"    border-radius: 4px;\n"
"    padding: 4px;\n"
"}\n"
"\n"
"QLineEdit:focus, QTextEdit:focus {\n"
"    border: 2px solid #0066cc; /* darker blue when focused */\n"
"}\n"
"\n"
"QLabel {\n"
"    color: #004d66; /* teal text */\n"
"    font-weight: bold;\n"
"}\n"
"QTabWidget::pane {\n"
"    border: 2px solid #3399ff; /* blue border */\n"
"    border-radius: 6px;\n"
"    background: #e6f7f5; /* light greenish background */\n"
"    padding: 4px;\n"
"}\n"
"\n"
"QTabBar"
                        "::tab {\n"
"    background: #4CAF50; /* green tabs */\n"
"    color: white;\n"
"    border: 1px solid #3399ff; /* blue border */\n"
"    border-radius: 6px;\n"
"    padding: 6px 12px;\n"
"    margin-right: 2px;\n"
"}\n"
"\n"
"QTabBar::tab:selected {\n"
"    background: #3399ff; /* blue when selected */\n"
"    color: white;\n"
"}\n"
"\n"
"QTabBar::tab:hover {\n"
"    background: #45a049; /* darker green hover */\n"
"}\n"
"QListView {\n"
"    background-color: #ffffff;\n"
"    border: 2px solid #3399ff; /* blue border */\n"
"    border-radius: 6px;\n"
"    padding: 4px;\n"
"    show-decoration-selected: 1; /* highlight whole row */\n"
"}\n"
"\n"
"QListView::item {\n"
"    padding: 6px;\n"
"    border-bottom: 1px solid #e6f7f5; /* subtle divider */\n"
"    color: #003366; /* dark blue text */\n"
"}\n"
"\n"
"QListView::item:selected {\n"
"    background-color: #4CAF50; /* green selection */\n"
"    color: white;\n"
"    border: none;\n"
"}\n"
"\n"
"QListView::item:hover {\n"
"    background-color: #cceeff; /* lig"
                        "ht blue hover */\n"
"    color: #004d66;\n"
"}\n"
"\n"
""));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName("tabWidget");
        tabWidget->setGeometry(QRect(30, 0, 1411, 621));
        tabWidget->setStyleSheet(QString::fromUtf8("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
"<ui version=\"4.0\">\n"
" <class>MainWindow</class>\n"
" <widget class=\"QMainWindow\" name=\"MainWindow\">\n"
"  <property name=\"geometry\">\n"
"   <rect>\n"
"    <x>0</x>\n"
"    <y>0</y>\n"
"    <width>1400</width>\n"
"    <height>700</height>\n"
"   </rect>\n"
"  </property>\n"
"  <property name=\"windowTitle\">\n"
"   <string>Gestion Int\303\251gr\303\251e</string>\n"
"  </property>\n"
"  <property name=\"styleSheet\">\n"
"   <string notr=\"true\">\n"
"QMainWindow {\n"
"    background-color: #f0f0f0;\n"
"}\n"
"\n"
"QTabWidget::pane {\n"
"    border: 2px solid #C2C7CB;\n"
"    background-color: white;\n"
"}\n"
"\n"
"QTabBar::tab {\n"
"    background-color: #E1E1E1;\n"
"    border: 1px solid #C4C4C3;\n"
"    padding: 8px 20px;\n"
"    margin-right: 2px;\n"
"}\n"
"\n"
"QTabBar::tab:selected {\n"
"    background-color: #4CAF50;\n"
"    color: white;\n"
"}\n"
"\n"
"QPushButton {\n"
"    background-color: #4CAF50;\n"
"    color: white;\n"
"    border: none;\n"
"  "
                        "  padding: 8px 16px;\n"
"    border-radius: 4px;\n"
"    font-weight: bold;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #45a049;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: #3d8b40;\n"
"}\n"
"\n"
"QLineEdit, QComboBox {\n"
"    padding: 6px;\n"
"    border: 1px solid #ccc;\n"
"    border-radius: 4px;\n"
"    background-color: white;\n"
"}\n"
"\n"
"QLineEdit:focus, QComboBox:focus {\n"
"    border-color: #4CAF50;\n"
"}\n"
"\n"
"QTableView {\n"
"    gridline-color: #d0d0d0;\n"
"    selection-background-color: #4CAF50;\n"
"    alternate-background-color: #f6f6f6;\n"
"}\n"
"\n"
"QHeaderView::section {\n"
"    background-color: #4CAF50;\n"
"    color: white;\n"
"    padding: 6px;\n"
"    border: 1px solid #45a049;\n"
"}\n"
"   </string>\n"
"  </property>\n"
"  <widget class=\"QWidget\" name=\"centralwidget\">\n"
"   <layout class=\"QVBoxLayout\" name=\"verticalLayout\">\n"
"    <item>\n"
"     <widget class=\"QTabWidget\" name=\"tabWidget\">\n"
"      <property name=\"currentInde"
                        "x\">\n"
"       <number>0</number>\n"
"      </property>\n"
"      <widget class=\"QWidget\" name=\"tabEmployes\">\n"
"       <attribute name=\"title\">\n"
"        <string>Gestion des Employ\303\251s</string>\n"
"       </attribute>\n"
"       <layout class=\"QHBoxLayout\" name=\"horizontalLayout\">\n"
"        <item>\n"
"         <widget class=\"QGroupBox\" name=\"groupBox\">\n"
"          <property name=\"title\">\n"
"           <string>Formulaire Employ\303\251</string>\n"
"          </property>\n"
"          <layout class=\"QFormLayout\" name=\"formLayout\">\n"
"           <item row=\"0\" column=\"0\">\n"
"            <widget class=\"QLabel\" name=\"label\">\n"
"             <property name=\"text\">\n"
"              <string>CIN:</string>\n"
"             </property>\n"
"            </widget>\n"
"           </item>\n"
"           <item row=\"0\" column=\"1\">\n"
"            <widget class=\"QLineEdit\" name=\"txtCin\"/>\n"
"           </item>\n"
"           <item row=\"1\" column=\"0\">\n"
"            <w"
                        "idget class=\"QLabel\" name=\"label_2\">\n"
"             <property name=\"text\">\n"
"              <string>Nom:</string>\n"
"             </property>\n"
"            </widget>\n"
"           </item>\n"
"           <item row=\"1\" column=\"1\">\n"
"            <widget class=\"QLineEdit\" name=\"txtNom\"/>\n"
"           </item>\n"
"           <item row=\"2\" column=\"0\">\n"
"            <widget class=\"QLabel\" name=\"label_3\">\n"
"             <property name=\"text\">\n"
"              <string>Pr\303\251nom:</string>\n"
"             </property>\n"
"            </widget>\n"
"           </item>\n"
"           <item row=\"2\" column=\"1\">\n"
"            <widget class=\"QLineEdit\" name=\"txtPrenom\"/>\n"
"           </item>\n"
"           <item row=\"3\" column=\"0\">\n"
"            <widget class=\"QLabel\" name=\"label_4\">\n"
"             <property name=\"text\">\n"
"              <string>Email:</string>\n"
"             </property>\n"
"            </widget>\n"
"           </item>\n"
"           <item "
                        "row=\"3\" column=\"1\">\n"
"            <widget class=\"QLineEdit\" name=\"txtEmail\"/>\n"
"           </item>\n"
"           <item row=\"4\" column=\"0\">\n"
"            <widget class=\"QLabel\" name=\"label_5\">\n"
"             <property name=\"text\">\n"
"              <string>Mot de passe:</string>\n"
"             </property>\n"
"            </widget>\n"
"           </item>\n"
"           <item row=\"4\" column=\"1\">\n"
"            <widget class=\"QLineEdit\" name=\"txtMotDePasse\"/>\n"
"           </item>\n"
"           <item row=\"5\" column=\"0\">\n"
"            <widget class=\"QLabel\" name=\"label_6\">\n"
"             <property name=\"text\">\n"
"              <string>T\303\251l\303\251phone:</string>\n"
"             </property>\n"
"            </widget>\n"
"           </item>\n"
"           <item row=\"5\" column=\"1\">\n"
"            <widget class=\"QLineEdit\" name=\"txtTelephone\"/>\n"
"           </item>\n"
"           <item row=\"6\" column=\"0\">\n"
"            <widget class=\"QLabel\""
                        " name=\"label_7\">\n"
"             <property name=\"text\">\n"
"              <string>Poste:</string>\n"
"             </property>\n"
"            </widget>\n"
"           </item>\n"
"           <item row=\"6\" column=\"1\">\n"
"            <widget class=\"QLineEdit\" name=\"txtPoste\"/>\n"
"           </item>\n"
"           <item row=\"7\" column=\"0\">\n"
"            <widget class=\"QLabel\" name=\"label_8\">\n"
"             <property name=\"text\">\n"
"              <string>Salaire:</string>\n"
"             </property>\n"
"            </widget>\n"
"           </item>\n"
"           <item row=\"7\" column=\"1\">\n"
"            <widget class=\"QLineEdit\" name=\"txtSalaire\"/>\n"
"           </item>\n"
"           <item row=\"8\" column=\"0\" colspan=\"2\">\n"
"            <layout class=\"QHBoxLayout\" name=\"horizontalLayout_2\">\n"
"             <item>\n"
"              <widget class=\"QPushButton\" name=\"btnAjouterEmploye\">\n"
"               <property name=\"text\">\n"
"                <string>Ajou"
                        "ter</string>\n"
"               </property>\n"
"              </widget>\n"
"             </item>\n"
"             <item>\n"
"              <widget class=\"QPushButton\" name=\"btnModifierEmploye\">\n"
"               <property name=\"text\">\n"
"                <string>Modifier</string>\n"
"               </property>\n"
"              </widget>\n"
"             </item>\n"
"             <item>\n"
"              <widget class=\"QPushButton\" name=\"btnSupprimerEmploye\">\n"
"               <property name=\"text\">\n"
"                <string>Supprimer</string>\n"
"               </property>\n"
"              </widget>\n"
"             </item>\n"
"            </layout>\n"
"           </item>\n"
"          </layout>\n"
"         </widget>\n"
"        </item>\n"
"        <item>\n"
"         <widget class=\"QGroupBox\" name=\"groupBox_2\">\n"
"          <property name=\"title\">\n"
"           <string>Liste des Employ\303\251s</string>\n"
"          </property>\n"
"          <layout class=\"QVBoxLayout\" name=\"vert"
                        "icalLayout_2\">\n"
"           <item>\n"
"            <layout class=\"QHBoxLayout\" name=\"horizontalLayout_3\">\n"
"             <item>\n"
"              <widget class=\"QPushButton\" name=\"btnActualiserEmployes\">\n"
"               <property name=\"text\">\n"
"                <string>Actualiser</string>\n"
"               </property>\n"
"              </widget>\n"
"             </item>\n"
"             <item>\n"
"              <widget class=\"QPushButton\" name=\"btnRechercherEmploye\">\n"
"               <property name=\"text\">\n"
"                <string>Rechercher</string>\n"
"               </property>\n"
"              </widget>\n"
"             </item>\n"
"             <item>\n"
"              <widget class=\"QPushButton\" name=\"btnTriEmployes\">\n"
"               <property name=\"text\">\n"
"                <string>Trier par Nom</string>\n"
"               </property>\n"
"              </widget>\n"
"             </item>\n"
"             <item>\n"
"              <widget class=\"QPushButton\" name="
                        "\"btnPDFEmployes\">\n"
"               <property name=\"text\">\n"
"                <string>PDF</string>\n"
"               </property>\n"
"              </widget>\n"
"             </item>\n"
"             <item>\n"
"              <widget class=\"QPushButton\" name=\"btnStatistiques\">\n"
"               <property name=\"text\">\n"
"                <string>Statistiques</string>\n"
"               </property>\n"
"              </widget>\n"
"             </item>\n"
"            </layout>\n"
"           </item>\n"
"           <item>\n"
"            <widget class=\"QTableView\" name=\"tableEmployes\"/>\n"
"           </item>\n"
"          </layout>\n"
"         </widget>\n"
"        </item>\n"
"       </layout>\n"
"      </widget>\n"
"      <widget class=\"QWidget\" name=\"tabEquipements\">\n"
"       <attribute name=\"title\">\n"
"        <string>Gestion des \303\211quipements</string>\n"
"       </attribute>\n"
"       <layout class=\"QHBoxLayout\" name=\"horizontalLayout_4\">\n"
"        <item>\n"
"         <wi"
                        "dget class=\"QGroupBox\" name=\"groupBox_3\">\n"
"          <property name=\"title\">\n"
"           <string>Formulaire \303\211quipement</string>\n"
"          </property>\n"
"          <layout class=\"QFormLayout\" name=\"formLayout_2\">\n"
"           <item row=\"0\" column=\"0\">\n"
"            <widget class=\"QLabel\" name=\"label_9\">\n"
"             <property name=\"text\">\n"
"              <string>ID:</string>\n"
"             </property>\n"
"            </widget>\n"
"           </item>\n"
"           <item row=\"0\" column=\"1\">\n"
"            <widget class=\"QLineEdit\" name=\"txtIdEquipement\"/>\n"
"           </item>\n"
"           <item row=\"1\" column=\"0\">\n"
"            <widget class=\"QLabel\" name=\"label_10\">\n"
"             <property name=\"text\">\n"
"              <string>Nom:</string>\n"
"             </property>\n"
"            </widget>\n"
"           </item>\n"
"           <item row=\"1\" column=\"1\">\n"
"            <widget class=\"QLineEdit\" name=\"txtNomEquipement\"/>\n"
""
                        "           </item>\n"
"           <item row=\"2\" column=\"0\">\n"
"            <widget class=\"QLabel\" name=\"label_11\">\n"
"             <property name=\"text\">\n"
"              <string>Type:</string>\n"
"             </property>\n"
"            </widget>\n"
"           </item>\n"
"           <item row=\"2\" column=\"1\">\n"
"            <widget class=\"QLineEdit\" name=\"txtTypeEquipement\"/>\n"
"           </item>\n"
"           <item row=\"3\" column=\"0\">\n"
"            <widget class=\"QLabel\" name=\"label_12\">\n"
"             <property name=\"text\">\n"
"              <string>Zone:</string>\n"
"             </property>\n"
"            </widget>\n"
"           </item>\n"
"           <item row=\"3\" column=\"1\">\n"
"            <widget class=\"QLineEdit\" name=\"txtZoneEquipement\"/>\n"
"           </item>\n"
"           <item row=\"4\" column=\"0\">\n"
"            <widget class=\"QLabel\" name=\"label_13\">\n"
"             <property name=\"text\">\n"
"              <string>\303\211tat:</strin"
                        "g>\n"
"             </property>\n"
"            </widget>\n"
"           </item>\n"
"           <item row=\"4\" column=\"1\">\n"
"            <widget class=\"QLineEdit\" name=\"txtEtatEquipement\"/>\n"
"           </item>\n"
"           <item row=\"5\" column=\"0\">\n"
"            <widget class=\"QLabel\" name=\"label_14\">\n"
"             <property name=\"text\">\n"
"              <string>Consommation:</string>\n"
"             </property>\n"
"            </widget>\n"
"           </item>\n"
"           <item row=\"5\" column=\"1\">\n"
"            <widget class=\"QLineEdit\" name=\"txtConsommation\"/>\n"
"           </item>\n"
"           <item row=\"6\" column=\"0\" colspan=\"2\">\n"
"            <layout class=\"QHBoxLayout\" name=\"horizontalLayout_5\">\n"
"             <item>\n"
"              <widget class=\"QPushButton\" name=\"btnAjouterEquipement\">\n"
"               <property name=\"text\">\n"
"                <string>Ajouter</string>\n"
"               </property>\n"
"              </widget>\n"
" "
                        "            </item>\n"
"             <item>\n"
"              <widget class=\"QPushButton\" name=\"btnModifierEquipement\">\n"
"               <property name=\"text\">\n"
"                <string>Modifier</string>\n"
"               </property>\n"
"              </widget>\n"
"             </item>\n"
"             <item>\n"
"              <widget class=\"QPushButton\" name=\"btnSupprimerEquipement\">\n"
"               <property name=\"text\">\n"
"                <string>Supprimer</string>\n"
"               </property>\n"
"              </widget>\n"
"             </item>\n"
"            </layout>\n"
"           </item>\n"
"          </layout>\n"
"         </widget>\n"
"        </item>\n"
"        <item>\n"
"         <widget class=\"QGroupBox\" name=\"groupBox_4\">\n"
"          <property name=\"title\">\n"
"           <string>Liste des \303\211quipements</string>\n"
"          </property>\n"
"          <layout class=\"QVBoxLayout\" name=\"verticalLayout_3\">\n"
"           <item>\n"
"            <layout class="
                        "\"QHBoxLayout\" name=\"horizontalLayout_6\">\n"
"             <item>\n"
"              <widget class=\"QPushButton\" name=\"btnActualiserEquipements\">\n"
"               <property name=\"text\">\n"
"                <string>Actualiser</string>\n"
"               </property>\n"
"              </widget>\n"
"             </item>\n"
"             <item>\n"
"              <widget class=\"QComboBox\" name=\"cbxTriEquipements\"/>\n"
"             </item>\n"
"             <item>\n"
"              <widget class=\"QPushButton\" name=\"btnPDFEquipements\">\n"
"               <property name=\"text\">\n"
"                <string>PDF</string>\n"
"               </property>\n"
"              </widget>\n"
"             </item>\n"
"             <item>\n"
"              <spacer name=\"horizontalSpacer\">\n"
"               <property name=\"orientation\">\n"
"                <enum>Qt::Horizontal</enum>\n"
"               </property>\n"
"               <property name=\"sizeHint\" stdset=\"0\">\n"
"                <size>\n"
"     "
                        "            <width>40</width>\n"
"                 <height>20</height>\n"
"                </size>\n"
"               </property>\n"
"              </spacer>\n"
"             </item>\n"
"            </layout>\n"
"           </item>\n"
"           <item>\n"
"            <widget class=\"QTableView\" name=\"tableEquipements\"/>\n"
"           </item>\n"
"          </layout>\n"
"         </widget>\n"
"        </item>\n"
"       </layout>\n"
"      </widget>\n"
"     </widget>\n"
"    </item>\n"
"   </layout>\n"
"  </widget>\n"
"  <widget class=\"QMenuBar\" name=\"menubar\">\n"
"   <property name=\"geometry\">\n"
"    <rect>\n"
"     <x>0</x>\n"
"     <y>0</y>\n"
"     <width>1400</width>\n"
"     <height>24</height>\n"
"    </rect>\n"
"   </property>\n"
"  </widget>\n"
"  <widget class=\"QStatusBar\" name=\"statusbar\"/>\n"
" </widget>\n"
" <resources/>\n"
" <connections/>\n"
"</ui>"));
        tabWidget->setTabBarAutoHide(false);
        tab_2 = new QWidget();
        tab_2->setObjectName("tab_2");
        label_13 = new QLabel(tab_2);
        label_13->setObjectName("label_13");
        label_13->setGeometry(QRect(1110, 170, 81, 20));
        pushButton_4 = new QPushButton(tab_2);
        pushButton_4->setObjectName("pushButton_4");
        pushButton_4->setGeometry(QRect(970, 60, 90, 29));
        comboBox_2 = new QComboBox(tab_2);
        comboBox_2->setObjectName("comboBox_2");
        comboBox_2->setGeometry(QRect(1200, 170, 76, 26));
        pushButton_12 = new QPushButton(tab_2);
        pushButton_12->setObjectName("pushButton_12");
        pushButton_12->setGeometry(QRect(860, 60, 90, 29));
        tabWidget_3 = new QTabWidget(tab_2);
        tabWidget_3->setObjectName("tabWidget_3");
        tabWidget_3->setGeometry(QRect(70, 70, 311, 401));
        tab_8 = new QWidget();
        tab_8->setObjectName("tab_8");
        label_32 = new QLabel(tab_8);
        label_32->setObjectName("label_32");
        label_32->setGeometry(QRect(10, 30, 51, 20));
        label_33 = new QLabel(tab_8);
        label_33->setObjectName("label_33");
        label_33->setGeometry(QRect(10, 70, 63, 20));
        label_34 = new QLabel(tab_8);
        label_34->setObjectName("label_34");
        label_34->setGeometry(QRect(10, 110, 63, 20));
        label_35 = new QLabel(tab_8);
        label_35->setObjectName("label_35");
        label_35->setGeometry(QRect(10, 150, 63, 20));
        label_36 = new QLabel(tab_8);
        label_36->setObjectName("label_36");
        label_36->setGeometry(QRect(10, 190, 91, 20));
        label_37 = new QLabel(tab_8);
        label_37->setObjectName("label_37");
        label_37->setGeometry(QRect(10, 230, 63, 20));
        lineEdit_30 = new QLineEdit(tab_8);
        lineEdit_30->setObjectName("lineEdit_30");
        lineEdit_30->setGeometry(QRect(140, 30, 131, 28));
        lineEdit_31 = new QLineEdit(tab_8);
        lineEdit_31->setObjectName("lineEdit_31");
        lineEdit_31->setGeometry(QRect(140, 70, 131, 28));
        lineEdit_32 = new QLineEdit(tab_8);
        lineEdit_32->setObjectName("lineEdit_32");
        lineEdit_32->setGeometry(QRect(140, 110, 131, 28));
        lineEdit_33 = new QLineEdit(tab_8);
        lineEdit_33->setObjectName("lineEdit_33");
        lineEdit_33->setGeometry(QRect(140, 150, 131, 28));
        lineEdit_34 = new QLineEdit(tab_8);
        lineEdit_34->setObjectName("lineEdit_34");
        lineEdit_34->setGeometry(QRect(140, 190, 131, 28));
        lineEdit_35 = new QLineEdit(tab_8);
        lineEdit_35->setObjectName("lineEdit_35");
        lineEdit_35->setGeometry(QRect(140, 230, 131, 28));
        pushButton_13 = new QPushButton(tab_8);
        pushButton_13->setObjectName("pushButton_13");
        pushButton_13->setGeometry(QRect(30, 320, 90, 29));
        pushButton_14 = new QPushButton(tab_8);
        pushButton_14->setObjectName("pushButton_14");
        pushButton_14->setGeometry(QRect(160, 320, 90, 29));
        label_38 = new QLabel(tab_8);
        label_38->setObjectName("label_38");
        label_38->setGeometry(QRect(10, 270, 101, 20));
        lineEdit_36 = new QLineEdit(tab_8);
        lineEdit_36->setObjectName("lineEdit_36");
        lineEdit_36->setGeometry(QRect(140, 270, 131, 28));
        tabWidget_3->addTab(tab_8, QString());
        tab_9 = new QWidget();
        tab_9->setObjectName("tab_9");
        lineEdit_37 = new QLineEdit(tab_9);
        lineEdit_37->setObjectName("lineEdit_37");
        lineEdit_37->setGeometry(QRect(130, 30, 131, 28));
        label_39 = new QLabel(tab_9);
        label_39->setObjectName("label_39");
        label_39->setGeometry(QRect(20, 70, 63, 20));
        lineEdit_38 = new QLineEdit(tab_9);
        lineEdit_38->setObjectName("lineEdit_38");
        lineEdit_38->setGeometry(QRect(130, 150, 131, 28));
        label_40 = new QLabel(tab_9);
        label_40->setObjectName("label_40");
        label_40->setGeometry(QRect(2, 230, 101, 20));
        lineEdit_39 = new QLineEdit(tab_9);
        lineEdit_39->setObjectName("lineEdit_39");
        lineEdit_39->setGeometry(QRect(130, 230, 131, 28));
        label_41 = new QLabel(tab_9);
        label_41->setObjectName("label_41");
        label_41->setGeometry(QRect(20, 150, 63, 20));
        lineEdit_40 = new QLineEdit(tab_9);
        lineEdit_40->setObjectName("lineEdit_40");
        lineEdit_40->setGeometry(QRect(130, 110, 131, 28));
        lineEdit_41 = new QLineEdit(tab_9);
        lineEdit_41->setObjectName("lineEdit_41");
        lineEdit_41->setGeometry(QRect(130, 190, 131, 28));
        lineEdit_42 = new QLineEdit(tab_9);
        lineEdit_42->setObjectName("lineEdit_42");
        lineEdit_42->setGeometry(QRect(130, 70, 131, 28));
        label_42 = new QLabel(tab_9);
        label_42->setObjectName("label_42");
        label_42->setGeometry(QRect(20, 190, 91, 20));
        pushButton_15 = new QPushButton(tab_9);
        pushButton_15->setObjectName("pushButton_15");
        pushButton_15->setGeometry(QRect(0, 310, 90, 29));
        label_43 = new QLabel(tab_9);
        label_43->setObjectName("label_43");
        label_43->setGeometry(QRect(20, 30, 51, 20));
        label_44 = new QLabel(tab_9);
        label_44->setObjectName("label_44");
        label_44->setGeometry(QRect(20, 110, 63, 20));
        pushButton_16 = new QPushButton(tab_9);
        pushButton_16->setObjectName("pushButton_16");
        pushButton_16->setGeometry(QRect(180, 310, 90, 29));
        tabWidget_3->addTab(tab_9, QString());
        tableView = new QTableView(tab_2);
        tableView->setObjectName("tableView");
        tableView->setGeometry(QRect(400, 100, 691, 371));
        comboBox = new QComboBox(tab_2);
        comboBox->setObjectName("comboBox");
        comboBox->setGeometry(QRect(1200, 130, 76, 26));
        label_45 = new QLabel(tab_2);
        label_45->setObjectName("label_45");
        label_45->setGeometry(QRect(1110, 130, 51, 20));
        tabWidget->addTab(tab_2, QString());
        tab = new QWidget();
        tab->setObjectName("tab");
        refresh_2 = new QPushButton(tab);
        refresh_2->setObjectName("refresh_2");
        refresh_2->setGeometry(QRect(410, 50, 151, 29));
        pushButton_5 = new QPushButton(tab);
        pushButton_5->setObjectName("pushButton_5");
        pushButton_5->setGeometry(QRect(1060, 300, 101, 51));
        pushButton_3 = new QPushButton(tab);
        pushButton_3->setObjectName("pushButton_3");
        pushButton_3->setGeometry(QRect(1060, 100, 101, 51));
        refresh = new QPushButton(tab);
        refresh->setObjectName("refresh");
        refresh->setGeometry(QRect(850, 40, 101, 29));
        label_14 = new QLabel(tab);
        label_14->setObjectName("label_14");
        label_14->setGeometry(QRect(1110, 960, 41, 20));
        lineEdit_13 = new QLineEdit(tab);
        lineEdit_13->setObjectName("lineEdit_13");
        lineEdit_13->setGeometry(QRect(610, 50, 121, 31));
        supprimer = new QPushButton(tab);
        supprimer->setObjectName("supprimer");
        supprimer->setGeometry(QRect(1060, 200, 101, 51));
        frame = new QFrame(tab);
        frame->setObjectName("frame");
        frame->setGeometry(QRect(1140, 960, 261, 191));
        frame->setFrameShape(QFrame::Shape::StyledPanel);
        frame->setFrameShadow(QFrame::Shadow::Raised);
        tab_emp = new QTableView(tab);
        tab_emp->setObjectName("tab_emp");
        tab_emp->setGeometry(QRect(380, 90, 601, 391));
        tabWidget_2 = new QTabWidget(tab);
        tabWidget_2->setObjectName("tabWidget_2");
        tabWidget_2->setGeometry(QRect(60, 50, 311, 461));
        tab_3 = new QWidget();
        tab_3->setObjectName("tab_3");
        label = new QLabel(tab_3);
        label->setObjectName("label");
        label->setGeometry(QRect(10, 30, 51, 20));
        label_2 = new QLabel(tab_3);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(10, 70, 63, 20));
        label_3 = new QLabel(tab_3);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(10, 110, 63, 20));
        label_4 = new QLabel(tab_3);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(10, 150, 63, 20));
        label_5 = new QLabel(tab_3);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(10, 190, 91, 20));
        label_6 = new QLabel(tab_3);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(10, 230, 63, 20));
        lineEdit_cin = new QLineEdit(tab_3);
        lineEdit_cin->setObjectName("lineEdit_cin");
        lineEdit_cin->setGeometry(QRect(120, 30, 131, 28));
        lineEdit_nom = new QLineEdit(tab_3);
        lineEdit_nom->setObjectName("lineEdit_nom");
        lineEdit_nom->setGeometry(QRect(120, 70, 131, 28));
        lineEdit_prenom = new QLineEdit(tab_3);
        lineEdit_prenom->setObjectName("lineEdit_prenom");
        lineEdit_prenom->setGeometry(QRect(120, 110, 131, 28));
        lineEdit_email = new QLineEdit(tab_3);
        lineEdit_email->setObjectName("lineEdit_email");
        lineEdit_email->setGeometry(QRect(120, 150, 131, 28));
        lineEdit_motdepass = new QLineEdit(tab_3);
        lineEdit_motdepass->setObjectName("lineEdit_motdepass");
        lineEdit_motdepass->setGeometry(QRect(120, 190, 131, 28));
        lineEdit_telephon = new QLineEdit(tab_3);
        lineEdit_telephon->setObjectName("lineEdit_telephon");
        lineEdit_telephon->setGeometry(QRect(120, 230, 131, 28));
        ajouter = new QPushButton(tab_3);
        ajouter->setObjectName("ajouter");
        ajouter->setGeometry(QRect(0, 350, 90, 29));
        pushButton_7 = new QPushButton(tab_3);
        pushButton_7->setObjectName("pushButton_7");
        pushButton_7->setGeometry(QRect(200, 350, 90, 29));
        label_15 = new QLabel(tab_3);
        label_15->setObjectName("label_15");
        label_15->setGeometry(QRect(10, 270, 63, 20));
        lineEdit_poste = new QLineEdit(tab_3);
        lineEdit_poste->setObjectName("lineEdit_poste");
        lineEdit_poste->setGeometry(QRect(120, 270, 131, 28));
        label_16 = new QLabel(tab_3);
        label_16->setObjectName("label_16");
        label_16->setGeometry(QRect(10, 310, 63, 20));
        lineEdit_salaire = new QLineEdit(tab_3);
        lineEdit_salaire->setObjectName("lineEdit_salaire");
        lineEdit_salaire->setGeometry(QRect(120, 320, 131, 28));
        tabWidget_2->addTab(tab_3, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName("tab_4");
        lineEdit_cin2 = new QLineEdit(tab_4);
        lineEdit_cin2->setObjectName("lineEdit_cin2");
        lineEdit_cin2->setGeometry(QRect(130, 30, 131, 28));
        label_7 = new QLabel(tab_4);
        label_7->setObjectName("label_7");
        label_7->setGeometry(QRect(20, 70, 63, 20));
        lineEdit_email2 = new QLineEdit(tab_4);
        lineEdit_email2->setObjectName("lineEdit_email2");
        lineEdit_email2->setGeometry(QRect(130, 150, 131, 28));
        label_8 = new QLabel(tab_4);
        label_8->setObjectName("label_8");
        label_8->setGeometry(QRect(20, 230, 63, 20));
        lineEdit_tel2 = new QLineEdit(tab_4);
        lineEdit_tel2->setObjectName("lineEdit_tel2");
        lineEdit_tel2->setGeometry(QRect(130, 230, 131, 28));
        label_9 = new QLabel(tab_4);
        label_9->setObjectName("label_9");
        label_9->setGeometry(QRect(20, 150, 63, 20));
        lineEdit_prenom2 = new QLineEdit(tab_4);
        lineEdit_prenom2->setObjectName("lineEdit_prenom2");
        lineEdit_prenom2->setGeometry(QRect(130, 110, 131, 28));
        lineEdit_motdepass2 = new QLineEdit(tab_4);
        lineEdit_motdepass2->setObjectName("lineEdit_motdepass2");
        lineEdit_motdepass2->setGeometry(QRect(130, 190, 131, 28));
        lineEdit_nom2 = new QLineEdit(tab_4);
        lineEdit_nom2->setObjectName("lineEdit_nom2");
        lineEdit_nom2->setGeometry(QRect(130, 70, 131, 28));
        label_10 = new QLabel(tab_4);
        label_10->setObjectName("label_10");
        label_10->setGeometry(QRect(20, 190, 91, 20));
        modifier = new QPushButton(tab_4);
        modifier->setObjectName("modifier");
        modifier->setGeometry(QRect(0, 370, 90, 29));
        label_11 = new QLabel(tab_4);
        label_11->setObjectName("label_11");
        label_11->setGeometry(QRect(20, 30, 51, 20));
        label_12 = new QLabel(tab_4);
        label_12->setObjectName("label_12");
        label_12->setGeometry(QRect(20, 110, 63, 20));
        pushButton_6 = new QPushButton(tab_4);
        pushButton_6->setObjectName("pushButton_6");
        pushButton_6->setGeometry(QRect(180, 370, 90, 29));
        lineEdit_poste_2 = new QLineEdit(tab_4);
        lineEdit_poste_2->setObjectName("lineEdit_poste_2");
        lineEdit_poste_2->setGeometry(QRect(130, 270, 131, 28));
        label_17 = new QLabel(tab_4);
        label_17->setObjectName("label_17");
        label_17->setGeometry(QRect(20, 310, 63, 20));
        label_18 = new QLabel(tab_4);
        label_18->setObjectName("label_18");
        label_18->setGeometry(QRect(20, 270, 63, 20));
        lineEdit_salaire_2 = new QLineEdit(tab_4);
        lineEdit_salaire_2->setObjectName("lineEdit_salaire_2");
        lineEdit_salaire_2->setGeometry(QRect(130, 320, 131, 28));
        tabWidget_2->addTab(tab_4, QString());
        tab_5 = new QWidget();
        tab_5->setObjectName("tab_5");
        refresh_3 = new QPushButton(tab_5);
        refresh_3->setObjectName("refresh_3");
        refresh_3->setGeometry(QRect(70, 360, 141, 29));
        listView = new QListView(tab_5);
        listView->setObjectName("listView");
        listView->setGeometry(QRect(10, 30, 281, 281));
        tabWidget_2->addTab(tab_5, QString());
        tabWidget->addTab(tab, QString());
        tab_dechet = new QWidget();
        tab_dechet->setObjectName("tab_dechet");
        statistiques_dechet_btn = new QTabWidget(tab_dechet);
        statistiques_dechet_btn->setObjectName("statistiques_dechet_btn");
        statistiques_dechet_btn->setGeometry(QRect(60, 50, 311, 461));
        tab_23 = new QWidget();
        tab_23->setObjectName("tab_23");
        cin = new QLabel(tab_23);
        cin->setObjectName("cin");
        cin->setGeometry(QRect(10, 50, 51, 20));
        date = new QLabel(tab_23);
        date->setObjectName("date");
        date->setGeometry(QRect(10, 90, 111, 20));
        lieu = new QLabel(tab_23);
        lieu->setObjectName("lieu");
        lieu->setGeometry(QRect(10, 130, 101, 20));
        volume = new QLabel(tab_23);
        volume->setObjectName("volume");
        volume->setGeometry(QRect(10, 170, 121, 20));
        etat = new QLabel(tab_23);
        etat->setObjectName("etat");
        etat->setGeometry(QRect(10, 230, 91, 20));
        cin_dechet = new QLineEdit(tab_23);
        cin_dechet->setObjectName("cin_dechet");
        cin_dechet->setGeometry(QRect(150, 50, 131, 28));
        ajouter_dechet_btn = new QPushButton(tab_23);
        ajouter_dechet_btn->setObjectName("ajouter_dechet_btn");
        ajouter_dechet_btn->setGeometry(QRect(20, 340, 90, 29));
        annuler_dechet_btn = new QPushButton(tab_23);
        annuler_dechet_btn->setObjectName("annuler_dechet_btn");
        annuler_dechet_btn->setGeometry(QRect(170, 340, 90, 29));
        date_dechet = new QDateEdit(tab_23);
        date_dechet->setObjectName("date_dechet");
        date_dechet->setGeometry(QRect(149, 90, 131, 29));
        lieu_dechet = new QLineEdit(tab_23);
        lieu_dechet->setObjectName("lieu_dechet");
        lieu_dechet->setGeometry(QRect(150, 130, 131, 28));
        volume_dechet = new QLineEdit(tab_23);
        volume_dechet->setObjectName("volume_dechet");
        volume_dechet->setGeometry(QRect(150, 170, 131, 28));
        recycle = new QRadioButton(tab_23);
        recycle->setObjectName("recycle");
        recycle->setGeometry(QRect(160, 230, 111, 25));
        nonrecycle = new QRadioButton(tab_23);
        nonrecycle->setObjectName("nonrecycle");
        nonrecycle->setGeometry(QRect(160, 280, 111, 25));
        statistiques_dechet_btn->addTab(tab_23, QString());
        tab_24 = new QWidget();
        tab_24->setObjectName("tab_24");
        modifier_dechet_btn = new QPushButton(tab_24);
        modifier_dechet_btn->setObjectName("modifier_dechet_btn");
        modifier_dechet_btn->setGeometry(QRect(20, 340, 90, 29));
        anuuler_modifier_dechet_btn = new QPushButton(tab_24);
        anuuler_modifier_dechet_btn->setObjectName("anuuler_modifier_dechet_btn");
        anuuler_modifier_dechet_btn->setGeometry(QRect(170, 340, 90, 29));
        volume_dechet_modifier = new QLineEdit(tab_24);
        volume_dechet_modifier->setObjectName("volume_dechet_modifier");
        volume_dechet_modifier->setGeometry(QRect(150, 170, 131, 28));
        date_dechet_mod = new QLabel(tab_24);
        date_dechet_mod->setObjectName("date_dechet_mod");
        date_dechet_mod->setGeometry(QRect(10, 90, 111, 20));
        date_dechet_modifier = new QDateEdit(tab_24);
        date_dechet_modifier->setObjectName("date_dechet_modifier");
        date_dechet_modifier->setGeometry(QRect(149, 90, 131, 29));
        lieu_dechet_modifier = new QLineEdit(tab_24);
        lieu_dechet_modifier->setObjectName("lieu_dechet_modifier");
        lieu_dechet_modifier->setGeometry(QRect(150, 130, 131, 28));
        cin_dechet_modifier = new QLineEdit(tab_24);
        cin_dechet_modifier->setObjectName("cin_dechet_modifier");
        cin_dechet_modifier->setGeometry(QRect(150, 50, 131, 28));
        lieu_dechet_mod = new QLabel(tab_24);
        lieu_dechet_mod->setObjectName("lieu_dechet_mod");
        lieu_dechet_mod->setGeometry(QRect(10, 130, 101, 20));
        cin_dechet_mod = new QLabel(tab_24);
        cin_dechet_mod->setObjectName("cin_dechet_mod");
        cin_dechet_mod->setGeometry(QRect(10, 50, 51, 20));
        etat_dechet_mod = new QLabel(tab_24);
        etat_dechet_mod->setObjectName("etat_dechet_mod");
        etat_dechet_mod->setGeometry(QRect(10, 230, 91, 20));
        volume_dechet_mod = new QLabel(tab_24);
        volume_dechet_mod->setObjectName("volume_dechet_mod");
        volume_dechet_mod->setGeometry(QRect(10, 170, 101, 20));
        recycle_modifier = new QRadioButton(tab_24);
        recycle_modifier->setObjectName("recycle_modifier");
        recycle_modifier->setGeometry(QRect(160, 230, 111, 25));
        nonrecycle_modifier = new QRadioButton(tab_24);
        nonrecycle_modifier->setObjectName("nonrecycle_modifier");
        nonrecycle_modifier->setGeometry(QRect(160, 280, 111, 25));
        statistiques_dechet_btn->addTab(tab_24, QString());
        tab_25 = new QWidget();
        tab_25->setObjectName("tab_25");
        afficher_stats_tableau_btn = new QPushButton(tab_25);
        afficher_stats_tableau_btn->setObjectName("afficher_stats_tableau_btn");
        afficher_stats_tableau_btn->setGeometry(QRect(70, 360, 141, 29));
        graphicsView_dechet_stat = new QGraphicsView(tab_25);
        graphicsView_dechet_stat->setObjectName("graphicsView_dechet_stat");
        graphicsView_dechet_stat->setGeometry(QRect(10, 30, 281, 281));
        statistiques_dechet_btn->addTab(tab_25, QString());
        lineEdit_rech_dechet = new QLineEdit(tab_dechet);
        lineEdit_rech_dechet->setObjectName("lineEdit_rech_dechet");
        lineEdit_rech_dechet->setGeometry(QRect(390, 60, 151, 31));
        supprimer_dechet_btn = new QPushButton(tab_dechet);
        supprimer_dechet_btn->setObjectName("supprimer_dechet_btn");
        supprimer_dechet_btn->setGeometry(QRect(1020, 180, 111, 61));
        tableView_dechet = new QTableView(tab_dechet);
        tableView_dechet->setObjectName("tableView_dechet");
        tableView_dechet->setGeometry(QRect(390, 100, 601, 391));
        trier_dechet_btn = new QPushButton(tab_dechet);
        trier_dechet_btn->setObjectName("trier_dechet_btn");
        trier_dechet_btn->setGeometry(QRect(890, 60, 91, 31));
        refresh_dechet_btn = new QPushButton(tab_dechet);
        refresh_dechet_btn->setObjectName("refresh_dechet_btn");
        refresh_dechet_btn->setGeometry(QRect(1020, 250, 111, 61));
        rech_dechet_btn = new QPushButton(tab_dechet);
        rech_dechet_btn->setObjectName("rech_dechet_btn");
        rech_dechet_btn->setGeometry(QRect(640, 60, 91, 29));
        export_pdf_stats_btn = new QPushButton(tab_dechet);
        export_pdf_stats_btn->setObjectName("export_pdf_stats_btn");
        export_pdf_stats_btn->setGeometry(QRect(1020, 320, 111, 61));
        comboBox_trier_dechet = new QComboBox(tab_dechet);
        comboBox_trier_dechet->addItem(QString());
        comboBox_trier_dechet->addItem(QString());
        comboBox_trier_dechet->addItem(QString());
        comboBox_trier_dechet->addItem(QString());
        comboBox_trier_dechet->setObjectName("comboBox_trier_dechet");
        comboBox_trier_dechet->setGeometry(QRect(770, 60, 111, 31));
        comboBox_rech_dechet = new QComboBox(tab_dechet);
        comboBox_rech_dechet->addItem(QString());
        comboBox_rech_dechet->addItem(QString());
        comboBox_rech_dechet->setObjectName("comboBox_rech_dechet");
        comboBox_rech_dechet->setGeometry(QRect(550, 60, 76, 31));
        lineEdit_supprimer_dechet = new QLineEdit(tab_dechet);
        lineEdit_supprimer_dechet->setObjectName("lineEdit_supprimer_dechet");
        lineEdit_supprimer_dechet->setGeometry(QRect(1020, 140, 111, 28));
        tabWidget->addTab(tab_dechet, QString());
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1519, 24));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(2);
        tabWidget_3->setCurrentIndex(0);
        tabWidget_2->setCurrentIndex(0);
        statistiques_dechet_btn->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        label_13->setText(QCoreApplication::translate("MainWindow", "Recherche", nullptr));
        pushButton_4->setText(QCoreApplication::translate("MainWindow", "Suprimer", nullptr));
        pushButton_12->setText(QCoreApplication::translate("MainWindow", "Afficher", nullptr));
        label_32->setText(QCoreApplication::translate("MainWindow", "ID", nullptr));
        label_33->setText(QCoreApplication::translate("MainWindow", "Nom", nullptr));
        label_34->setText(QCoreApplication::translate("MainWindow", "Type", nullptr));
        label_35->setText(QCoreApplication::translate("MainWindow", "Zone", nullptr));
        label_36->setText(QCoreApplication::translate("MainWindow", "Etat", nullptr));
        label_37->setText(QCoreApplication::translate("MainWindow", "Date", nullptr));
        pushButton_13->setText(QCoreApplication::translate("MainWindow", "Ajouter", nullptr));
        pushButton_14->setText(QCoreApplication::translate("MainWindow", "Annuler", nullptr));
        label_38->setText(QCoreApplication::translate("MainWindow", "consommation", nullptr));
        tabWidget_3->setTabText(tabWidget_3->indexOf(tab_8), QCoreApplication::translate("MainWindow", "Ajouter", nullptr));
        label_39->setText(QCoreApplication::translate("MainWindow", "Nom", nullptr));
        label_40->setText(QCoreApplication::translate("MainWindow", "consommation", nullptr));
        label_41->setText(QCoreApplication::translate("MainWindow", "Zone", nullptr));
        label_42->setText(QCoreApplication::translate("MainWindow", "Etat", nullptr));
        pushButton_15->setText(QCoreApplication::translate("MainWindow", "Modifier", nullptr));
        label_43->setText(QCoreApplication::translate("MainWindow", "ID", nullptr));
        label_44->setText(QCoreApplication::translate("MainWindow", "Type", nullptr));
        pushButton_16->setText(QCoreApplication::translate("MainWindow", "Annuler", nullptr));
        tabWidget_3->setTabText(tabWidget_3->indexOf(tab_9), QCoreApplication::translate("MainWindow", "Modifier", nullptr));
        label_45->setText(QCoreApplication::translate("MainWindow", "Tri", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("MainWindow", "equipement", nullptr));
        refresh_2->setText(QCoreApplication::translate("MainWindow", "Recherche Par Cin", nullptr));
        pushButton_5->setText(QCoreApplication::translate("MainWindow", "PDF", nullptr));
        pushButton_3->setText(QCoreApplication::translate("MainWindow", "Trie Par Nom", nullptr));
        refresh->setText(QCoreApplication::translate("MainWindow", "refresh", nullptr));
        label_14->setText(QCoreApplication::translate("MainWindow", "Stat", nullptr));
        supprimer->setText(QCoreApplication::translate("MainWindow", "Supprimer", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "CIN", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Nom", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "Pr\303\251nom", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "Email", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "Mot de passe", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "N Tel", nullptr));
        ajouter->setText(QCoreApplication::translate("MainWindow", "Ajouter", nullptr));
        pushButton_7->setText(QCoreApplication::translate("MainWindow", "Annuler", nullptr));
        label_15->setText(QCoreApplication::translate("MainWindow", "Poste", nullptr));
        label_16->setText(QCoreApplication::translate("MainWindow", "Salaire", nullptr));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_3), QCoreApplication::translate("MainWindow", "Ajouter", nullptr));
        lineEdit_cin2->setText(QString());
        label_7->setText(QCoreApplication::translate("MainWindow", "Nom", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "N Tel", nullptr));
        label_9->setText(QCoreApplication::translate("MainWindow", "Email", nullptr));
        label_10->setText(QCoreApplication::translate("MainWindow", "Mot de passe", nullptr));
        modifier->setText(QCoreApplication::translate("MainWindow", "Modifier", nullptr));
        label_11->setText(QCoreApplication::translate("MainWindow", "CIN", nullptr));
        label_12->setText(QCoreApplication::translate("MainWindow", "Pr\303\251nom", nullptr));
        pushButton_6->setText(QCoreApplication::translate("MainWindow", "Annuler", nullptr));
        label_17->setText(QCoreApplication::translate("MainWindow", "Salaire", nullptr));
        label_18->setText(QCoreApplication::translate("MainWindow", "Poste", nullptr));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_4), QCoreApplication::translate("MainWindow", "Modifier", nullptr));
        refresh_3->setText(QCoreApplication::translate("MainWindow", "afficher_stat", nullptr));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_5), QCoreApplication::translate("MainWindow", "stat", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("MainWindow", "employee", nullptr));
        cin->setText(QCoreApplication::translate("MainWindow", "CIN", nullptr));
        date->setText(QCoreApplication::translate("MainWindow", "Date de collecte", nullptr));
        lieu->setText(QCoreApplication::translate("MainWindow", "lieu de collecte", nullptr));
        volume->setText(QCoreApplication::translate("MainWindow", "volume/poids", nullptr));
        etat->setText(QCoreApplication::translate("MainWindow", "Etat", nullptr));
        ajouter_dechet_btn->setText(QCoreApplication::translate("MainWindow", "Ajouter", nullptr));
        annuler_dechet_btn->setText(QCoreApplication::translate("MainWindow", "Annuler", nullptr));
        recycle->setText(QCoreApplication::translate("MainWindow", "recycl\303\251", nullptr));
        nonrecycle->setText(QCoreApplication::translate("MainWindow", "Non recycl\303\251", nullptr));
        statistiques_dechet_btn->setTabText(statistiques_dechet_btn->indexOf(tab_23), QCoreApplication::translate("MainWindow", "Ajouter", nullptr));
        modifier_dechet_btn->setText(QCoreApplication::translate("MainWindow", "Modifier", nullptr));
        anuuler_modifier_dechet_btn->setText(QCoreApplication::translate("MainWindow", "Annuler", nullptr));
        date_dechet_mod->setText(QCoreApplication::translate("MainWindow", "Date de collecte", nullptr));
        lieu_dechet_mod->setText(QCoreApplication::translate("MainWindow", "lieu de collecte", nullptr));
        cin_dechet_mod->setText(QCoreApplication::translate("MainWindow", "CIN", nullptr));
        etat_dechet_mod->setText(QCoreApplication::translate("MainWindow", "Etat", nullptr));
        volume_dechet_mod->setText(QCoreApplication::translate("MainWindow", "volume/poids", nullptr));
        recycle_modifier->setText(QCoreApplication::translate("MainWindow", "recycl\303\251", nullptr));
        nonrecycle_modifier->setText(QCoreApplication::translate("MainWindow", "Non recycl\303\251", nullptr));
        statistiques_dechet_btn->setTabText(statistiques_dechet_btn->indexOf(tab_24), QCoreApplication::translate("MainWindow", "Modifier", nullptr));
        afficher_stats_tableau_btn->setText(QCoreApplication::translate("MainWindow", "afficher", nullptr));
        statistiques_dechet_btn->setTabText(statistiques_dechet_btn->indexOf(tab_25), QCoreApplication::translate("MainWindow", "stat", nullptr));
        supprimer_dechet_btn->setText(QCoreApplication::translate("MainWindow", "Supprimer", nullptr));
        trier_dechet_btn->setText(QCoreApplication::translate("MainWindow", "trier", nullptr));
        refresh_dechet_btn->setText(QCoreApplication::translate("MainWindow", "raffraichir", nullptr));
        rech_dechet_btn->setText(QCoreApplication::translate("MainWindow", "Rechercher", nullptr));
        export_pdf_stats_btn->setText(QCoreApplication::translate("MainWindow", "PDF", nullptr));
        comboBox_trier_dechet->setItemText(0, QCoreApplication::translate("MainWindow", "date", nullptr));
        comboBox_trier_dechet->setItemText(1, QCoreApplication::translate("MainWindow", "lieu", nullptr));
        comboBox_trier_dechet->setItemText(2, QCoreApplication::translate("MainWindow", "etat", nullptr));
        comboBox_trier_dechet->setItemText(3, QString());

        comboBox_rech_dechet->setItemText(0, QCoreApplication::translate("MainWindow", "cin", nullptr));
        comboBox_rech_dechet->setItemText(1, QCoreApplication::translate("MainWindow", "lieu", nullptr));

        tabWidget->setTabText(tabWidget->indexOf(tab_dechet), QCoreApplication::translate("MainWindow", "Page", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
