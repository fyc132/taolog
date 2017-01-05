#pragma once

namespace taolog {

class ResultFilter : public taowin::window_creator
{
public:
    typedef std::function<void(std::vector<std::wstring>*, int*)> fnOnGetFields;
    typedef std::vector<std::pair<int, const wchar_t*>> IntStrPairs;
    typedef std::function<void(int, IntStrPairs*, bool*, taowin::combobox::OnDraw*)> fnGetValueList;
    typedef std::function<void(const std::wstring& name,
        int field_index, const std::wstring& field_name, int value_index,
        const std::wstring& value_name, const std::wstring& value_input)> fnOnNewFilter;

public:
    ResultFilter(EventContainerS& filters, fnOnGetFields getfields, ModuleEntry* curprj, EventContainer* curflt, fnGetValueList getvalues, fnOnNewFilter onnewfilter)
        : _filters(filters)
        , _on_get_fields(getfields)
        , _current_filter(curflt)
        , _currnet_project(curprj)
        , _get_value_list(getvalues)
        , _onnewfilter(onnewfilter)
    {
        _tipwnd = new TooltipWindow;
    }

    ~ResultFilter()
    {
        _tipwnd = nullptr;
    }

protected:
    taowin::listview*   _listview;
    taowin::button*     _btn_add;
    taowin::button*     _btn_delete;
    taowin::button*     _btn_all;

    TooltipWindow*      _tipwnd;

protected:
    EventContainerS&    _filters;
    fnOnGetFields       _on_get_fields;
    EventContainer*     _current_filter;
    ModuleEntry*        _currnet_project;
    fnGetValueList      _get_value_list;
    fnOnNewFilter       _onnewfilter;

protected:
    virtual void get_metas(WindowMeta* metas) override;
    virtual LPCTSTR get_skin_xml() const override;
    virtual LRESULT handle_message(UINT umsg, WPARAM wparam, LPARAM lparam) override;
    virtual LRESULT control_message(taowin::syscontrol* ctl, UINT umsg, WPARAM wparam, LPARAM lparam);
    virtual LRESULT on_notify(HWND hwnd, taowin::control * pc, int code, NMHDR * hdr) override;
    virtual void on_final_message() override { __super::on_final_message(); delete this; }
};

class AddNewFilter : public taowin::window_creator
{
public:
    std::wstring name;
    int          field_index;
    std::wstring field_name;
    int          value_index;
    std::wstring value_name;
    std::wstring value_input;

    AddNewFilter(ResultFilter::fnOnGetFields getfields, ResultFilter::fnGetValueList getvalues, ResultFilter::fnOnNewFilter onnew)
        : _on_get_fields(getfields)
        , _get_values(getvalues)
        , _on_new(onnew)
    { }

protected:
    std::vector<std::wstring>    _fields;
    ResultFilter::fnOnGetFields  _on_get_fields;
    ResultFilter::IntStrPairs    _values;
    ResultFilter::fnGetValueList _get_values;
    ResultFilter::fnOnNewFilter  _on_new;
    bool _value_editable;
    taowin::combobox::OnDraw _draw_value;

    taowin::edit*       _name;
    taowin::combobox*   _field_name;
    taowin::combobox*   _value_name;
    taowin::combobox*   _value_name_1;  // 不能编辑的
    taowin::combobox*   _value_name_2;  // 能编辑的
    taowin::combobox*   _value_name_3;  // 能编辑、自绘的
    taowin::edit*       _value_input;
    taowin::button*     _save;
    taowin::button*     _cancel;

protected:
    virtual void get_metas(WindowMeta* metas) override;
    virtual LPCTSTR get_skin_xml() const override;
    virtual LRESULT handle_message(UINT umsg, WPARAM wparam, LPARAM lparam) override;
    virtual LRESULT on_notify(HWND hwnd, taowin::control * pc, int code, NMHDR * hdr) override;
    virtual bool filter_special_key(int vk) override;

    void _change_value_control();

    int _on_save();
};

}
