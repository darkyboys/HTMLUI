# HTMLUI

**HTMLUI** is a modern, lightweight C++ UI library that lets you build powerful native GUI applications using HTML, CSS, and JavaScript. It leverages GTK and WebKit2 to render HTML content and seamlessly integrate JavaScript callbacks into native C++ logic.

> Created by [ghgltggamer](https://github.com/darkyboys) — Licensed under the MIT License

---

## 🚀 Features

- Render HTML and CSS in native C++ windows
- Load HTML from strings, files, or URLs
- Call native C++ functions from JavaScript
- Execute JavaScript from C++
- Fine-tune WebKit settings for performance or security
- Auto flush queued JS when DOM is ready
- MIT licensed and fully open-source

---

## 📦 Requirements

- GTK 3
- WebKit2GTK
- C++17 or newer

Install dependencies on Arch Linux:

```bash
sudo pacman -S gtk3 webkit2gtk
````

Or on Ubuntu/Debian:

```bash
sudo apt install libgtk-3-dev libwebkit2gtk-4.0-dev
```

---

## 🔧 Build Instructions

You can build your application with any build system. Example using `g++`:

```bash
g++ main.cpp -o app `pkg-config --cflags --libs gtk+-3.0 webkit2gtk-4.0`
```

---

## ✨ Basic Example

```cpp
#include "htmlui.h"

int main() {
    HTMLUI app("My HTMLUI App", 800, 600);
    app.loadFile("index.html");

    app.registerFunction("sayHello", [](const std::string& name) {
        std::cout << "Hello, " << name << "!" << std::endl;
    });

    app.run();
}
```

Then in your `index.html`:

```html
<button onclick="sayHello('World')">Click Me</button>
```

---

## 📘 API Documentation

### `HTMLUI(const std::string& title, int width, int height)`

Creates a new window with the given title and dimensions.

### `void loadHTML(const std::string& html)`

Loads raw HTML content into the window.

### `void loadFile(const std::string& filepath)`

Loads a local `.html` file using a full or relative path.

### `void loadURL(const std::string& url)`

Loads a remote or local URL (e.g., `http://example.com` or `file:///path/to/file.html`).

### `void run()`

Starts the GTK main loop.

### `void registerFunction(const std::string& functionName, std::function<void(const std::string&)> callback)`

Exposes a C++ function to JavaScript with a given name. JavaScript can then call `window.functionName(arg)`.

### `void executeJS(const std::string& script)`

Executes a JavaScript string. If DOM isn't ready yet, it will be queued and run after load.

### `void setWebKitSetting(const std::string& setting, bool value)`

Changes WebKit settings dynamically.

Supported `setting` strings:

* `"javascript"`
* `"developer_extras"`
* `"webgl"`
* `"file_access"`
* `"universal_access"`
* `"resizable_text_areas"`
* `"smooth_scrolling"`

---

## 🧠 JavaScript Bridge

You can invoke C++ functions from JS like so:

```js
window.nativeBridge.invoke ("sayHello", "yourargs");
```

Behind the scenes, HTMLUI injects:

```js
window.nativeBridge = {
    invoke: function(funcName, arg) {
        window.webkit.messageHandlers.nativeCallback.postMessage(funcName + ':' + arg);
    }
};
```

When you register a function in C++, it's automatically exposed to JS.

---

## 🪪 License

This project is licensed under the [MIT License](https://opensource.org/licenses/MIT).

---

## 🔗 Links

* GitHub: [https://github.com/darkyboys/HTMLUI](https://github.com/darkyboys/HTMLUI)
