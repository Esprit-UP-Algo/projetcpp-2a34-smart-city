# 🎯 How to Access the New Advanced Features

## ✅ Features Are Now Available!

All advanced features have been implemented and **buttons have been automatically added** to your waste management tab. Here's where to find them:

## 📍 Location of New Buttons

The new feature buttons are located in the **waste management tab** (the "Page" tab), on the **right side panel**, below the existing buttons:

### Button Layout (Right Side Panel):
1. **Supprimer** (Delete) - Existing button
2. **raffraichir** (Refresh) - Existing button  
3. **PDF** - Existing button
4. **🆕 Filtrer Dates** - Filter by date range
5. **🆕 Filtrer Volume** - Filter by volume range
6. **🆕 Filtre Avancé** - Multi-criteria filtering
7. **🆕 Excel** - Export to Excel
8. **🆕 CSV** - Export to CSV
9. **🆕 Graphiques** - Show visual charts
10. **🆕 Impact Env.** - Calculate environmental impact
11. **🆕 Alertes** - View alerts
12. **🆕 Tendances** - Analyze trends

## 🚀 How to Use Each Feature

### 1. **Filtrer Dates** (Date Range Filter)
- Click the button
- Select start and end dates in the dialog
- Click "Filtrer" to see filtered results

### 2. **Filtrer Volume** (Volume Range Filter)
- Click the button
- Enter minimum and maximum volume (kg)
- Click "Filtrer" to see filtered results

### 3. **Filtre Avancé** (Advanced Multi-Criteria Filter)
- Click the button
- Set multiple criteria:
  - Date range
  - Location
  - Type (recycled/non-recycled)
  - Volume range
- Click "Filtrer" to see filtered results

### 4. **Excel** (Export to Excel)
- Click the button
- Choose save location
- Data will be exported to .xls/.csv format

### 5. **CSV** (Export to CSV)
- Click the button
- Choose save location
- Data will be exported to CSV format

### 6. **Graphiques** (Charts)
- Click the button
- View interactive charts:
  - Bar chart: Collections by location
  - Pie chart: Recycling rate
  - Line chart: Volume trends

### 7. **Impact Env.** (Environmental Impact)
- Click the button
- See environmental impact calculations:
  - CO2 saved (kg)
  - Energy saved (kWh)
  - Impact score
  - Equivalences (trees, km)

### 8. **Alertes** (Alerts)
- Click the button
- View alerts for:
  - High volume collections (>1000 kg)
  - Overdue collections (>7 days old)

### 9. **Tendances** (Trends)
- Click the button
- See trend analysis:
  - Daily/weekly/monthly statistics
  - Volume trends
  - Average collections per day

## 🔍 If Buttons Don't Appear

If you don't see the buttons after rebuilding:

1. **Rebuild the project**:
   ```bash
   # In Qt Creator: Build > Rebuild Project
   # Or from command line:
   cmake --build build
   ```

2. **Check the tab**: Make sure you're in the **"Page" tab** (waste management tab)

3. **Scroll down**: The buttons are below the PDF button on the right side

## 💡 Alternative: Access via Code

You can also call the methods directly in your code:

```cpp
// In any slot or method:
Dechet dechet;

// Filter by date range
QDate start = QDate(2024, 1, 1);
QDate end = QDate(2024, 12, 31);
QSqlQueryModel *model = dechet.filtrerParDateRange(start, end);
ui->tableView_dechet->setModel(model);

// Export to CSV
dechet.exportToCSV("export.csv");

// Show charts
afficherCharts();

// Calculate environmental impact
afficherImpactEnvironnemental();

// Show alerts
afficherAlertes();

// Analyze trends
afficherTendances();
```

## 📝 Notes

- All buttons are automatically created when the application starts
- Buttons are positioned on the right side panel of the waste management tab
- If Qt Charts is not installed, the charts button will show a message
- All features work with your existing database structure

## ✨ Enjoy Your New Features!

All advanced features are now accessible through the UI buttons. Just rebuild and run your application!

