# Fixing Linker Error - Step by Step

## The Problem
Linker error "ld returned 1 exit status" means some symbol is undefined. We need to find which one.

## Solution Steps:

### Step 1: Get the Actual Error Message
In Qt Creator:
1. Go to **Help → About Plugins**
2. Make sure "Compile Output" plugin is enabled
3. Build the project
4. Look at the **Compile Output** tab (bottom panel)
5. Scroll to find lines with:
   - `undefined reference to`
   - `undefined symbol`
   - These will tell us what's missing

### Step 2: Common Fixes

**If MOC isn't running:**
- Check if `moc_notificationmanager.cpp` exists in build folder
- If not, MOC isn't processing the file

**If specific methods are undefined:**
- Check if all methods in notificationmanager.h are implemented in notificationmanager.cpp

**If it's a Qt symbol:**
- Make sure Qt6::Network is linked (it is in CMakeLists.txt)

### Step 3: Temporary Workaround
If you need to build immediately, you can temporarily comment out NotificationManager usage:

In `mainwindow.cpp`, comment out:
```cpp
// notificationManager = new NotificationManager(this);
// setupDefaultAlertRules();
```

And add null checks before all `notificationManager->` calls.

### Step 4: Verify Build System
Make sure:
- CMakeLists.txt has `CMAKE_AUTOMOC ON` ✓
- notificationmanager.h and .cpp are in PROJECT_SOURCES ✓
- Qt6::Network is linked ✓

## Next Steps:
Please share the actual linker error message (the "undefined reference" lines) so we can fix the exact issue.

