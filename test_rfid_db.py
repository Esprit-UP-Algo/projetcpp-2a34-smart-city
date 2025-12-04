import sys
import cx_Oracle

# Database connection parameters
DB_USER = "SYSTEM"
DB_PASSWORD = "123456789"
DB_DSN = "localhost/XE"

def test_rfid_uids():
    try:
        # Connect to database
        connection = cx_Oracle.connect(DB_USER, DB_PASSWORD, DB_DSN)
        cursor = connection.cursor()
        
        print("=== Vérification des UID RFID dans la base de données ===")
        
        # Check if RFID_UID column exists
        cursor.execute("""
            SELECT COLUMN_NAME 
            FROM ALL_TAB_COLUMNS 
            WHERE TABLE_NAME = 'EMPLOYE' AND COLUMN_NAME = 'RFID_UID'
        """)
        
        column_exists = cursor.fetchone()
        if column_exists:
            print("✅ Colonne RFID_UID existe")
        else:
            print("❌ Colonne RFID_UID n'existe pas")
            return
        
        # Check all employees with RFID_UID values
        cursor.execute("""
            SELECT CIN, NOM, PRENOM, RFID_UID 
            FROM EMPLOYE 
            WHERE RFID_UID IS NOT NULL AND RFID_UID != ''
        """)
        
        employees_with_rfid = cursor.fetchall()
        
        if employees_with_rfid:
            print(f"\n📋 Employés avec UID RFID ({len(employees_with_rfid)}):")
            for emp in employees_with_rfid:
                cin, nom, prenom, rfid_uid = emp
                print(f"  CIN: {cin} | Nom: {nom} {prenom} | UID: {rfid_uid}")
        else:
            print("\n❌ Aucun employé n'a de UID RFID associé")
        
        # Test specific UIDs
        test_uids = ["79ECF504", "3F93CFC4"]
        
        print(f"\n🔍 Test des UID spécifiques:")
        for uid in test_uids:
            cursor.execute("""
                SELECT CIN, NOM, PRENOM 
                FROM EMPLOYE 
                WHERE RFID_UID = :uid
            """, {"uid": uid})
            
            result = cursor.fetchone()
            if result:
                cin, nom, prenom = result
                print(f"  ✅ UID {uid}: Trouvé - {nom} {prenom} (CIN: {cin})")
            else:
                print(f"  ❌ UID {uid}: Non trouvé")
        
        # Check all employees to see current state
        cursor.execute("""
            SELECT CIN, NOM, PRENOM, RFID_UID 
            FROM EMPLOYE 
            ORDER BY CIN
        """)
        
        all_employees = cursor.fetchall()
        print(f"\n📊 Tous les employés ({len(all_employees)}):")
        for emp in all_employees:
            cin, nom, prenom, rfid_uid = emp
            rfid_display = rfid_uid if rfid_uid else "NULL"
            print(f"  CIN: {cin} | Nom: {nom} {prenom} | UID: {rfid_display}")
        
    except cx_Oracle.Error as error:
        print(f"❌ Erreur de base de données: {error}")
    finally:
        if 'connection' in locals():
            connection.close()
            print("\n🔌 Connexion fermée")

if __name__ == "__main__":
    test_rfid_uids()
