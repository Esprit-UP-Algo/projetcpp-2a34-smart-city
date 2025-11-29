# Temporary Fix to Build Without Notification System

If you need to build immediately, you can temporarily disable the notification system:

## In mainwindow.cpp, around line 91-99:

**Comment out:**
```cpp
// // Initialize notification manager
// notificationManager = new NotificationManager(this);
// connect(notificationManager, &NotificationManager::notificationSent, 
//         this, &MainWindow::onNotificationSent);
// connect(notificationManager, &NotificationManager::alertTriggered, 
//         this, &MainWindow::onAlertTriggered);
// 
// // Set up default alert rules
// setupDefaultAlertRules();
```

**And change to:**
```cpp
// Initialize notification manager (temporarily disabled)
notificationManager = nullptr;
```

## Also add null checks:

Before every `notificationManager->` call, add:
```cpp
if (!notificationManager) return;
```

This will let you build while we fix the linker issue.

## To re-enable later:
Just uncomment the code above.

