# HTMLUI - Lightweight C++ HTML GUI Library

## Overview
HTMLUI3 is a lightweight C++ library for creating graphical user interfaces (GUIs) using HTML, CSS, and JavaScript. It leverages GTK and WebKit to provide a seamless way to build native-like applications with web technologies. HTMLUI is designed for developers who prefer to use web-based UI components in their C++ applications while maintaining high performance and flexibility.

## Why Use HTMLUI?
- **Leverage Web Technologies**: Utilize HTML, CSS, and JavaScript to design interactive and responsive UIs.
- **Lightweight and Fast**: Built with GTK and WebKit for efficient rendering and performance.
- **Native Integration**: Allows bidirectional communication between JavaScript and C++.
- **Customizable Settings**: Configure WebKit settings for better security and performance.
- **Cross-Platform**: Works on Linux and other systems that support GTK and WebKit.

## When NOT to Use HTMLUI
- If you need a **pure native** UI without any web-based components.
- If your application requires deep OS integration with custom widgets.
- If you prefer a dedicated GUI framework like Qt or GTK without web dependencies.

---

## Getting Started

### 1. Install Dependencies & HTMLUI
Ensure that GTK and WebKit2GTK are installed on your system.
```sh
sudo apt install libgtk-3-dev libwebkit2gtk-4.0-dev  # Debian-based
sudo pacman -S gtk3 webkit2gtk                        # Arch Linux
```

**HTMLUI Install**
```bash
# Copy and paste these commands in your terminal
git clone https://github.com/darkyboys/HTMLUI.git
cd HTMLUI
mv HTMLUI.h ..
cd ..
rm -rf HTMLUI
```


### 2. Initialize an HTMLUI Application
```cpp
#include "htmlui.h"

int main() {
    HTMLUI ui("My App", 800, 600);
    ui.run();
    return 0;
}
```

*Note You Can Also Define A Custom Path To Cookies. From where the program will read cookies.db file, with `HTMLUI ui("My App", 800, 600, "path/to/cookies.db");`*

### 3. Load HTML Content
- **Inline HTML**
  ```cpp
  ui.loadHTML("<h1>Hello, HTMLUI!</h1>");
  ```
- **Load from File**
  ```cpp
  ui.loadFile("/path/to/file.html");
  ```
- **Load from URL**
  ```cpp
  ui.loadURL("https://example.com");
  ```

### 4. Register a Native Function
```cpp
ui.registerFunction("showMessage", [](const std::string& message) {
    std::cout << "Message from JS: " << message << std::endl;
});
```
This function can be called from JavaScript as:
```js
window.showMessage("Hello from JavaScript!");
```

### 5. Execute JavaScript from C++
```cpp
ui.executeJS("document.body.style.backgroundColor = 'lightblue';");
```

### 6. Configure WebKit Settings
```cpp
ui.setWebKitSetting("javascript", true);  // Enable JavaScript
ui.setWebKitSetting("webgl", true);       // Enable WebGL
```

### 7. Run the Application
```cpp
ui.run();
```

### 8. Change Window Icon (X11 Support)
For changing the window icon you can use the `setWindowIcon` function which will set the window icon for `X11` Platforms.
```cpp
ui.setWindowIcon("path/to/icon.png");
```

---

## API Reference

### Constructor
```cpp
HTMLUI(const std::string& title, int width, int height);
```
Creates an application window with the given title and size.

### Load Content
```cpp
void loadHTML(const std::string& html);
void loadFile(const std::string& filepath);
void loadURL(const std::string& url);
```
Loads an HTML string, file, or URL into the WebView.

### JavaScript Integration
```cpp
void registerFunction(const std::string& functionName, std::function<void(const std::string&)> callback);
void executeJS(const std::string& script);
```
Registers a C++ function that can be called from JavaScript and executes JavaScript code from C++.

### WebKit Settings
```cpp
void setWebKitSetting(const std::string& setting, bool value);
```
Configures WebKit settings such as JavaScript, WebGL, and file access.

### Application Control
```cpp
void run();
```
Starts the GTK main loop, running the application.

---

## Compilation
Compile and link against GTK and WebKit:
```sh
g++ -o myapp main.cpp `pkg-config --cflags --libs gtk+-3.0 webkit2gtk-4.0`
```

## License
MIT License. Feel free to use and modify HTMLUI as needed.

## Contribution
Please read the [Guidelines](CONTRIBUTING.md) for more information

Copyright (c) ghgltggamers
