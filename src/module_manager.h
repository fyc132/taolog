#pragma once

#include <taowin/src/tw_taowin.h>

#include "_module_entry.hpp"

namespace taoetw {

class ModuleManager : public taowin::window_creator
{
public:
    typedef std::function<bool(ModuleEntry* module, bool enable, std::wstring* err)> fnOnToggleEnable;

protected:
    std::vector<ModuleEntry*>& _modules;

    fnOnToggleEnable    _on_toggle;

protected:
    taowin::listview*   _listview;
    taowin::button*     _btn_enable;
    taowin::button*     _btn_add;
    taowin::button*     _btn_modify;
    taowin::button*     _btn_delete;

public:
    ModuleManager(std::vector<ModuleEntry*>& modules)
        : _modules(modules)
    {}

    void on_toggle_enable(fnOnToggleEnable fn) { _on_toggle = fn; }

protected:
    virtual LPCTSTR get_skin_xml() const override;
    virtual LRESULT handle_message(UINT umsg, WPARAM wparam, LPARAM lparam) override;
    virtual LRESULT on_notify(HWND hwnd, taowin::control* pc, int code, NMHDR* hdr) override;
    virtual void on_final_message() override { __super::on_final_message(); delete this; }

protected:
    void _enable_items(const std::vector<int>& items, int state); // 1: enable, 0: disable, -1: toggle
    void _modify_item(int i);
    void _delete_items(const std::vector<int>& items);
    void _add_item();
    bool _has_guid(const GUID& guid);
    int _get_enable_state_for_items(const std::vector<int>& items);
    void _on_item_state_change();
};


}