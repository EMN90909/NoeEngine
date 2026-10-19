// Copyright (c) 2026 Noethric.
// SPDX-License-Identifier: LicenseRef-Noethric-Proprietary
#ifndef NOE_JAMILA_PANEL_H
#define NOE_JAMILA_PANEL_H

#include "scene/gui/panel_container.h"
#include "core/os/os.h"

class Button;
class HTTPRequest;
class HBoxContainer;
class Label;
class LineEdit;
class TextEdit;
class Timer;

// Native overlay: opens above Inspector/Node/Filter dock without changing
// the user's dock arrangement or affecting 2D/3D viewport layout.
class NoeJamilaPanel : public PanelContainer {
    GDCLASS(NoeJamilaPanel, PanelContainer);

    Button *toggle_button = nullptr;
    Button *plan_button = nullptr;
    Button *apply_button = nullptr;
    Button *export_button = nullptr;
    Button *release_button = nullptr;
    LineEdit *export_preset = nullptr;
    Label *status_label = nullptr;
    LineEdit *endpoint_field = nullptr;
    LineEdit *model_field = nullptr;
    LineEdit *key_field = nullptr;
    TextEdit *task_field = nullptr;
    TextEdit *review_field = nullptr;
    HTTPRequest *http = nullptr;
    Timer *startup = nullptr;
    String server_url;
    String server_token;
    String plan_id;
    OS::ProcessID helper_pid = 0;
    bool pending_plan = false;
    bool running = false;
    bool applying = false;
    bool exporting = false;
    bool exporting_release = false;

    static void _bind_methods() {}
    void _toggle();
    void _plan();
    void _apply();
    void _export_game();
    void _export_release();
    void _start_pending();
    void _on_response(int p_result, int p_status, const PackedStringArray &p_headers, const PackedByteArray &p_body);
    bool _launch_agent();
    void _send(const String &p_path, const Dictionary &p_payload);
    String _find_agent_script() const;

public:
    static NoeJamilaPanel *install(Control *p_root, HBoxContainer *p_toolbar);
    NoeJamilaPanel();
    ~NoeJamilaPanel();
};
#endif
