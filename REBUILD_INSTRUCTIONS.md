# 🔧 How to Rebuild the Project to See New Features

## Problem
The application is running an old version because the build directory has a cached executable.

## Solution: Clean and Rebuild

### Method 1: Using Qt Creator (Recommended)

1. **Clean the Project:**
   - In Qt Creator, go to: **Build > Clean Project "integration_crudd"**
   - Or press: `Ctrl+Shift+C`

2. **Delete Build Directory (Optional but recommended):**
   - Close Qt Creator
   - Delete the `build` folder: `C:\Users\anisk\OneDrive\Desktop\smartcity\smartcity\build`
   - Or just delete: `build\Desktop_Qt_6_7_3_MinGW_64_bit-Debug\integration_crudd.exe`

3. **Rebuild the Project:**
   - In Qt Creator: **Build > Rebuild Project "integration_crudd"**
   - Or press: `Ctrl+Shift+B`
   - Wait for the build to complete

4. **Run the Application:**
   - Press `Ctrl+R` or click the Run button
   - The new executable with all features will be created

### Method 2: Using Command Line

```powershell
# Navigate to project directory
cd C:\Users\anisk\OneDrive\Desktop\smartcity\smartcity

# Delete old executable
Remove-Item build\Desktop_Qt_6_7_3_MinGW_64_bit-Debug\integration_crudd.exe -ErrorAction SilentlyContinue

# Rebuild (if you have CMake in PATH)
cd build\Desktop_Qt_6_7_3_MinGW_64_bit-Debug
cmake --build . --clean-first
```

### Method 3: Manual Clean

1. **Delete these files/folders:**
   - `build\Desktop_Qt_6_7_3_MinGW_64_bit-Debug\integration_crudd.exe`
   - `build\Desktop_Qt_6_7_3_MinGW_64_bit-Debug\CMakeCache.txt`
   - `build\Desktop_Qt_6_7_3_MinGW_64_bit-Debug\integration_crudd_autogen\` (entire folder)

2. **In Qt Creator:**
   - Right-click on the project → **Run CMake**
   - Then **Build > Rebuild Project**

## Verify the Build

After rebuilding, check:
1. The executable timestamp should be recent
2. The file size should be updated
3. When you run it, you should see the new buttons in the waste management tab

## If Buttons Still Don't Appear

1. **Check Qt Creator Build Settings:**
   - Go to: **Projects > Build Settings**
   - Make sure the build directory is: `build/Desktop_Qt_6_7_3_MinGW_64_bit-Debug`
   - Check that all source files are included

2. **Verify UI File is Updated:**
   - Open `mainwindow.ui` in Qt Designer
   - Check that the new buttons exist (filtrer_date_range_btn, etc.)
   - Save the file

3. **Check for Build Errors:**
   - Look at the **Compile Output** tab in Qt Creator
   - Make sure there are no errors

4. **Force UI Recompilation:**
   - Delete: `build\Desktop_Qt_6_7_3_MinGW_64_bit-Debug\integration_crudd_autogen\`
   - Rebuild the project

## Quick Checklist

- [ ] Cleaned the project
- [ ] Rebuilt the project (not just built)
- [ ] Checked for build errors
- [ ] Verified executable is new (check timestamp)
- [ ] Run the application
- [ ] Checked the "Page" tab (waste management)
- [ ] Looked for buttons on the right side panel

## Expected Result

After rebuilding, when you run the application and go to the "Page" tab, you should see:
- **Right side, below PDF button:** Filtrer Dates, Filtrer Volume, Filtre Avancé, Excel, CSV, Graphiques
- **Right side, second column:** Impact Env., Alertes, Tendances

If you still don't see them, the UI file might not have been saved properly. Let me know!

