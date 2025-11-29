# Fix for Linker Error

The linker error is likely due to MOC (Meta-Object Compiler) not processing the NotificationManager class properly.

## Solution:

1. **Clean Build Directory:**
   - In Qt Creator: **Build → Clean Project**
   - Or manually delete the `build` folder

2. **Rebuild:**
   - In Qt Creator: **Build → Rebuild Project**

3. **If still failing, check:**
   - Make sure `CMAKE_AUTOMOC ON` is set (it is in CMakeLists.txt)
   - Verify that `notificationmanager.h` and `notificationmanager.cpp` are in the source list
   - Check that Qt6::Network is linked (it is)

## Alternative: Manual MOC (if needed)

If automatic MOC doesn't work, you can manually run MOC:
```bash
moc notificationmanager.h -o moc_notificationmanager.cpp
```

Then add `moc_notificationmanager.cpp` to your sources.

But with `CMAKE_AUTOMOC ON`, this should not be necessary.

