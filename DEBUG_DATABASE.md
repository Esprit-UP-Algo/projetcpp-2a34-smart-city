# 🔍 Debug Database Issues

## Problem
Data entered in Qt UI is not appearing in the database.

## What I Added

### 1. **Better Error Handling**
- Added detailed debug output in `dechet.cpp`
- Shows SQL errors with error codes
- Displays what data is being inserted

### 2. **Improved Validation**
- Checks if fields are empty before inserting
- Shows clear error messages

### 3. **Better Display Function**
- Shows how many rows were found
- Displays error messages if query fails

## How to Debug

### Step 1: Check Application Output
When you run the application, look at the **"Sortie de l'application"** (Application Output) tab at the bottom. You should see:

```
=== AJOUT DÉCHET ===
CIN: 12345678 Date: 2024-11-28 Lieu: Tunis Volume: 100 Etat: recyclé
=== ATTEMPTING INSERT ===
CIN: 12345678
Date: 2024-11-28
Lieu: Tunis
Volume: 100
Etat: recyclé
✅ Insert successful! Rows affected: 1
```

If you see errors like:
```
❌ SQL Error: ORA-00001: unique constraint violated
```
or
```
❌ SQL Error: ORA-00942: table or view does not exist
```

### Step 2: Check Database Connection
1. Make sure the database connection is working
2. Check in SQL Developer that you can see the DECHETS table
3. Verify the table structure matches:
   - CIN (NUMBER)
   - DATE_COLLECTE (DATE)
   - LIEU_COLLECTE (VARCHAR2)
   - VOLUME_POIDS (NUMBER)
   - ETAT (VARCHAR2)

### Step 3: Test Direct Insert in SQL Developer
Try inserting data directly in SQL Developer:
```sql
INSERT INTO DECHETS (CIN, DATE_COLLECTE, LIEU_COLLECTE, VOLUME_POIDS, ETAT)
VALUES (12345678, DATE '2024-11-28', 'Test Location', 100, 'recyclé');

COMMIT;

SELECT * FROM DECHETS;
```

### Step 4: Check for Constraints
The CIN might be a PRIMARY KEY. If you try to insert the same CIN twice, it will fail.

### Step 5: Verify Data After Insert
After clicking "Ajouter" in the UI:
1. Check the Application Output for success/error messages
2. Click "Afficher" or "raffraichir" button to refresh the table
3. Check in SQL Developer: `SELECT * FROM DECHETS;`

## Common Issues

1. **CIN Already Exists**: Use a different CIN number
2. **Date Format**: Make sure the date is valid
3. **Empty Fields**: All fields should be filled
4. **Transaction Not Committed**: Oracle might need explicit COMMIT
5. **Wrong Table Name**: Verify it's "DECHETS" (uppercase in Oracle)

## Next Steps

1. **Rebuild and run** the application
2. **Try to add a waste entry**
3. **Check the Application Output** tab for debug messages
4. **Share the error messages** if any appear

The debug output will tell us exactly what's going wrong!

