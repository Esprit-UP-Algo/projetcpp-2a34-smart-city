# Debugging Linker Error

The linker error "ld returned 1 exit status" is generic. We need to see the actual undefined symbols.

## To get the actual error:

1. **In Qt Creator:**
   - Go to **Build → Run qmake** (if using qmake)
   - Then **Build → Rebuild Project**
   - Look at the **Compile Output** tab (not just the Issues tab)
   - Scroll to find lines with "undefined reference" or "undefined symbol"

2. **Or check the build log:**
   - The build output should show something like:
     ```
     undefined reference to `NotificationManager::someMethod()'
     ```

## Common causes:

1. **MOC not running** - Check if `moc_notificationmanager.cpp` is being generated
2. **Missing implementation** - All methods must be implemented
3. **Missing library link** - Qt6::Network must be linked (it is)
4. **Circular dependency** - Check includes

## Quick test:

Try commenting out the NotificationManager initialization in mainwindow.cpp temporarily to see if the error goes away. This will tell us if the issue is with NotificationManager or something else.

