# Advanced Waste Management Features

## Overview
This document describes the advanced features added to the waste management (gestion du dechet) module.

## New Features Implemented

### 1. **Advanced Filtering**
- **Date Range Filtering**: Filter waste collections by date range
  - Method: `filtrerParDateRange(QDate dateDebut, QDate dateFin)`
  - Access: Call `on_filtrer_date_range_btn_clicked()` or add button in UI

- **Volume Range Filtering**: Filter by minimum and maximum volume
  - Method: `filtrerParVolumeRange(float volumeMin, float volumeMax)`
  - Access: Call `on_filtrer_volume_range_btn_clicked()` or add button in UI

- **Multi-Criteria Filtering**: Filter using multiple criteria simultaneously
  - Method: `filtrerMultiCritere(QDate dateDebut, QDate dateFin, QString lieu, QString type, float volumeMin, float volumeMax)`
  - Access: Call `on_filtrer_multi_critere_btn_clicked()` or add button in UI

### 2. **Data Export**
- **Excel Export**: Export waste data to Excel format (.xls/.csv)
  - Method: `exportToExcel(QString filename, QSqlQueryModel* model)`
  - Access: Call `on_export_excel_btn_clicked()` or add button in UI

- **CSV Export**: Export waste data to CSV format
  - Method: `exportToCSV(QString filename, QSqlQueryModel* model)`
  - Access: Call `on_export_csv_btn_clicked()` or add button in UI

### 3. **Visual Charts and Graphs**
- **Bar Charts**: Collections by location
- **Pie Charts**: Recycling rate distribution
- **Line Charts**: Volume trends over time
- **Access**: Call `on_afficher_charts_btn_clicked()` or `afficherCharts()`
- **Requires**: Qt Charts module (optional, with fallback to text statistics)

### 4. **Environmental Impact Calculator**
- **CO2 Saved**: Calculate CO2 emissions saved through recycling
  - Method: `calculateCO2Saved()` - Returns kg of CO2 saved
- **Energy Saved**: Calculate energy equivalent saved
  - Method: `calculateEnergySaved()` - Returns kWh saved
- **Impact Score**: Overall environmental impact score (0-100)
  - Method: `getEnvironmentalImpactScore()`
- **Access**: Call `on_calculer_impact_env_btn_clicked()` or `afficherImpactEnvironnemental()`

### 5. **Trend Analysis**
- **Daily Collections**: Statistics by day
  - Method: `getCollectionsByDay()`
- **Weekly Collections**: Statistics by week
  - Method: `getCollectionsByWeek()`
- **Monthly Volume Trends**: Volume trends by month
  - Method: `getVolumeTrendByMonth()`
- **Average Collections Per Day**: Efficiency metric
  - Method: `getAverageCollectionsPerDay()`
- **Access**: Call `on_analyser_tendances_btn_clicked()` or `afficherTendances()`

### 6. **Alerts System**
- **High Volume Alerts**: Alert when waste volume exceeds threshold
  - Method: `getHighVolumeAlerts(float threshold = 1000.0)`
- **Overdue Collections**: Alert for old collections
  - Method: `getOverdueCollections(int daysThreshold = 7)`
- **Access**: Call `on_afficher_alertes_btn_clicked()` or `afficherAlertes()`

### 7. **Waste Type Statistics**
- **Collections by Type**: Statistics grouped by waste type
  - Method: `getCollectionsByType()`
- **Volume by Type**: Volume statistics by type
  - Method: `getVolumeByType()`
- **Recycling Rate by Type**: Recycling rate for specific type
  - Method: `getRecyclingRateByType(QString type)`

### 8. **Collection Efficiency Metrics**
- **Average Collections Per Day**: Calculate average collections per day
  - Method: `getAverageCollectionsPerDay()`
- **Collections in Date Range**: Count collections in specific date range
  - Method: `getCollectionsInDateRange(QDate start, QDate end)`
- **Average Volume Per Collection**: Average volume per collection
  - Method: `getAverageVolumePerCollection()`

## How to Add UI Buttons

To make these features accessible from the UI, you can:

1. **Add buttons in Qt Designer** (mainwindow.ui):
   - Add buttons in the waste management tab
   - Name them: `filtrer_date_range_btn`, `filtrer_volume_range_btn`, `filtrer_multi_critere_btn`, `export_excel_btn`, `export_csv_btn`, `afficher_charts_btn`, `calculer_impact_env_btn`, `afficher_alertes_btn`, `analyser_tendances_btn`

2. **Or create buttons programmatically** in MainWindow constructor:
```cpp
// Example: Add export buttons
QPushButton *exportExcelBtn = new QPushButton("Exporter Excel", ui->tab_dechet);
exportExcelBtn->setGeometry(1020, 400, 111, 31);
connect(exportExcelBtn, &QPushButton::clicked, this, &MainWindow::on_export_excel_btn_clicked);

QPushButton *exportCSVBtn = new QPushButton("Exporter CSV", ui->tab_dechet);
exportCSVBtn->setGeometry(1020, 440, 111, 31);
connect(exportCSVBtn, &QPushButton::clicked, this, &MainWindow::on_export_csv_btn_clicked);

// Add more buttons as needed...
```

## Usage Examples

### Filter by Date Range
```cpp
QDate startDate = QDate(2024, 1, 1);
QDate endDate = QDate(2024, 12, 31);
QSqlQueryModel *model = dechet.filtrerParDateRange(startDate, endDate);
tableView->setModel(model);
```

### Export to CSV
```cpp
if (dechet.exportToCSV("dechets_export.csv")) {
    qDebug() << "Export successful!";
}
```

### Calculate Environmental Impact
```cpp
double co2 = dechet.calculateCO2Saved();
double energy = dechet.calculateEnergySaved();
double score = dechet.getEnvironmentalImpactScore();
```

### Get Alerts
```cpp
QList<QString> alerts = dechet.getHighVolumeAlerts(1000.0);
for (const QString &alert : alerts) {
    qDebug() << alert;
}
```

## Database Considerations

Note: Some features (like waste type categorization) assume the database structure. The current implementation uses the `ETAT` field to determine waste type. For full type categorization, you may want to add a `TYPE_DECHET` column to the DECHETS table:

```sql
ALTER TABLE DECHETS ADD TYPE_DECHET VARCHAR2(50);
```

Then update the `ajouter()` and `modifier()` methods to include this field.

## Requirements

- Qt 6.x with Charts module (optional, for visual charts)
- Standard Qt modules: Core, Widgets, Sql, PrintSupport, Pdf

## Notes

- All new methods are in the `Dechet` class
- UI integration methods are in `MainWindow` class
- Charts functionality is optional and will show a message if Qt Charts is not available
- Export functions work with any QSqlQueryModel, not just the default display

