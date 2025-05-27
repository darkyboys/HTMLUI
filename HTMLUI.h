/*
    * Copyright (c) ghgltggamer 2025
    * Written by ghgltggamer
    * HTMLUI a UI library for making html powered C++ Apps
    * Licensed under the MIT License
    * Checkout the README.md for more information
    * Checkout the GitHub Repo : https://github.com/darkyboys/HTMLUI
*/

#ifndef HTMLUI_H
#define HTMLUI_H

#include <gtk/gtk.h>
#include <webkit2/webkit2.h>
#include <functional>
#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>

class HTMLUI {
public:
    HTMLUI(const std::string& title, int width, int height);
    ~HTMLUI();

    void loadHTML(const std::string& html);
    void loadFile(const std::string& filepath);
    void loadURL(const std::string& url);
    void registerFunction(const std::string& functionName, std::function<void(const std::string&)> callback);
    void run();
    void configureSettings();
    void setWebKitSetting(const std::string& setting, bool value);
    void executeJS(const std::string& script);

    std::string lastDriveData;

private:
    GtkWidget* window;
    WebKitWebView* webView;
    WebKitSettings* settings;
    WebKitUserContentManager* contentManager;

    std::unordered_map<std::string, std::function<void(const std::string&)>> jsFunctions;
    std::vector<std::string> jsQueue;
    bool domReady = false;

    void initialize(const std::string& title, int width, int height);
    void injectJSBridge();
    void flushJSQueue();

    static void jsCallback(WebKitUserContentManager* manager, WebKitJavascriptResult* jsResult, gpointer userData);
};

HTMLUI::HTMLUI(const std::string& title, int width, int height) {
    initialize(title, width, height);
    configureSettings();
    injectJSBridge();
}

HTMLUI::~HTMLUI() {
    gtk_widget_destroy(window);
}

void HTMLUI::initialize(const std::string& title, int width, int height) {
    gtk_init(nullptr, nullptr);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), title.c_str());
    gtk_window_set_default_size(GTK_WINDOW(window), width, height);

    contentManager = webkit_user_content_manager_new();
    webView = WEBKIT_WEB_VIEW(webkit_web_view_new_with_user_content_manager(contentManager));

    if (!webView) {
        g_error("Failed to initialize WebKit WebView");
    }

    gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(webView));
    settings = webkit_web_view_get_settings(webView);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), nullptr);
    g_signal_connect(contentManager, "script-message-received::nativeCallback", G_CALLBACK(jsCallback), this);
    webkit_user_content_manager_register_script_message_handler(contentManager, "nativeCallback");

    // Hook load-changed to detect DOM readiness
    g_signal_connect(webView, "load-changed", G_CALLBACK(+[](
        WebKitWebView* web_view,
        WebKitLoadEvent load_event,
        gpointer user_data
    ) {
        if (load_event == WEBKIT_LOAD_FINISHED) {
            HTMLUI* self = static_cast<HTMLUI*>(user_data);
            self->domReady = true;
            self->flushJSQueue();
        }
    }), this);

    gtk_widget_show_all(window);
}

void HTMLUI::configureSettings() {
    webkit_settings_set_enable_javascript(settings, TRUE);
    webkit_settings_set_enable_developer_extras(settings, TRUE);
    webkit_settings_set_enable_webgl(settings, TRUE);
    webkit_settings_set_allow_file_access_from_file_urls(settings, TRUE);
    webkit_settings_set_allow_universal_access_from_file_urls(settings, TRUE);
    webkit_settings_set_enable_resizable_text_areas(settings, TRUE);
    webkit_settings_set_enable_smooth_scrolling(settings, TRUE);
}

void HTMLUI::setWebKitSetting(const std::string& setting, bool value) {
    if (setting == "javascript") {
        webkit_settings_set_enable_javascript(settings, value);
    } else if (setting == "developer_extras") {
        webkit_settings_set_enable_developer_extras(settings, value);
    } else if (setting == "webgl") {
        webkit_settings_set_enable_webgl(settings, value);
    } else if (setting == "file_access") {
        webkit_settings_set_allow_file_access_from_file_urls(settings, value);
    } else if (setting == "universal_access") {
        webkit_settings_set_allow_universal_access_from_file_urls(settings, value);
    } else if (setting == "resizable_text_areas") {
        webkit_settings_set_enable_resizable_text_areas(settings, value);
    } else if (setting == "smooth_scrolling") {
        webkit_settings_set_enable_smooth_scrolling(settings, value);
    }
}

void HTMLUI::injectJSBridge() {
    const char* jsBridgeScript = R"(
        window.nativeBridge = {
            invoke: function(funcName, arg) {
                window.webkit.messageHandlers.nativeCallback.postMessage(funcName + ':' + arg);
            }
        };
    )";

    WebKitUserScript* script = webkit_user_script_new(
        jsBridgeScript,
        WEBKIT_USER_CONTENT_INJECT_TOP_FRAME,
        WEBKIT_USER_SCRIPT_INJECT_AT_DOCUMENT_START,
        nullptr, nullptr
    );

    webkit_user_content_manager_add_script(contentManager, script);
}

void HTMLUI::loadHTML(const std::string& html) {
    domReady = false;  // reset DOM state
    jsQueue.clear();   // clear JS queue for new page
    webkit_web_view_load_html(webView, html.c_str(), nullptr);
}

void HTMLUI::loadFile(const std::string& filepath) {
    domReady = false;
    jsQueue.clear();
    webkit_web_view_load_uri(webView, ("file://" + filepath).c_str());
}

void HTMLUI::loadURL(const std::string& url) {
    domReady = false;
    jsQueue.clear();
    webkit_web_view_load_uri(webView, url.c_str());
}

void HTMLUI::registerFunction(const std::string& functionName, std::function<void(const std::string&)> callback) {
    jsFunctions[functionName] = callback;
    std::string jsExpose = "window." + functionName + " = function(arg) { window.nativeBridge.invoke('" + functionName + "', arg); };";
    executeJS(jsExpose);
}

void HTMLUI::executeJS(const std::string& script) {
    if (!domReady) {
        jsQueue.push_back(script);
        return;
    }

    webkit_web_view_evaluate_javascript(
        webView, script.c_str(), script.length(),
        nullptr, nullptr, nullptr, nullptr, nullptr
    );
}

void HTMLUI::flushJSQueue() {
    for (const std::string& js : jsQueue) {
        std::cout << "[JS Flush] " << js << '\n';
        webkit_web_view_evaluate_javascript(
            webView, js.c_str(), js.length(),
            nullptr, nullptr, nullptr, nullptr, nullptr
        );
    }
    jsQueue.clear();
}

void HTMLUI::run() {
    gtk_main();
}

void HTMLUI::jsCallback(WebKitUserContentManager* manager, WebKitJavascriptResult* jsResult, gpointer userData) {
    HTMLUI* ui = static_cast<HTMLUI*>(userData);
    JSCValue* value = webkit_javascript_result_get_js_value(jsResult);

    if (jsc_value_is_string(value)) {
        gchar* message = jsc_value_to_string(value);
        std::string functionCall(message);
        g_free(message);

        auto pos = functionCall.find(":");
        if (pos != std::string::npos) {
            std::string functionName = functionCall.substr(0, pos);
            std::string argument = functionCall.substr(pos + 1);

            if (ui->jsFunctions.find(functionName) != ui->jsFunctions.end()) {
                ui->jsFunctions[functionName](argument);
            }
        }
    }
}

#endif // HTMLUI_H
