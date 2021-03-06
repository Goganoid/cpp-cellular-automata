//
// Created by egor0 on 5/26/2020.
//

#ifndef CPP_LIFE_APPLOG_H
#define CPP_LIFE_APPLOG_H
#include "imgui.h"
#include "imgui-SFML.h"
#include "imconfig.h"
#include <ctime>
#include <sstream>
#include <iomanip>

enum class LogType{
    Info,
    Error,
    Notice
};



class AppLog
{
private:
    ImGuiTextBuffer     Buf;
    ImGuiTextFilter     Filter;
    ImVector<int>       LineOffsets;        // Index to lines offset
    bool                ScrollToBottom;
    bool                allowedToScroll = true;
    unsigned int        _width,_height;
    std::string GetTime(){
        std::stringstream buffer;
        time_t curr_time;
        tm curr_tm;

        time(&curr_time);
        localtime_s(&curr_tm,&curr_time);
        buffer<<std::put_time(&curr_tm,"[%T]");
        return buffer.str();
    }

    std::string GetLogMark(const LogType& logType){
        switch (logType) {
            case LogType::Info:
                return "[info]";
                break;
            case LogType::Error:
                return "[error]";
                break;
            case LogType::Notice:
                return "[notice]";
                break;
            default:
                return "[NULL]";
        }
    }
public:
    AppLog(unsigned int width,unsigned int height){
        _width = width;
        _height = height;
    }
    void    Clear()     { Buf.clear(); LineOffsets.clear(); }
    void    AddLog(const std::string& fmt, const LogType& logType=LogType::Info)
    {
        int old_size = Buf.size();

        std::string str = GetTime()+ " " + GetLogMark(logType) + " " + fmt + "\n";

        Buf.append(str.c_str());

        for (int new_size = Buf.size(); old_size < new_size; old_size++)
            if (Buf[old_size] == '\n')
                LineOffsets.push_back(old_size);
        if(Buf.size()>=100000) Clear();
        ScrollToBottom = true;
    }

    void Draw(const char* title, bool* p_opened = NULL)
    {

        ImGui::SetNextWindowSize(ImVec2(_width,_height), ImGuiCond_FirstUseEver);
        ImGui::Begin(title, p_opened);
        if (ImGui::Button("Clear")) Clear();
        ImGui::SameLine();
        bool copy = ImGui::Button("Copy");
        ImGui::SameLine();
        Filter.Draw("Filter", -100.0f);
        ImGui::Checkbox("Scroll",&allowedToScroll);
        ImGui::Separator();
        ImGui::BeginChild("scrolling");
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0,1));
        if (copy) ImGui::LogToClipboard();

        if (Filter.IsActive())
        {
            const char* buf_begin = Buf.begin();
            const char* line = buf_begin;
            for (int line_no = 0; line != NULL; line_no++)
            {
                const char* line_end = (line_no < LineOffsets.Size) ? buf_begin + LineOffsets[line_no] : NULL;
                if (Filter.PassFilter(line, line_end))
                    ImGui::TextUnformatted(line, line_end);
                line = line_end && line_end[1] ? line_end + 1 : NULL;
            }
        }
        else
        {
            ImGui::TextUnformatted(Buf.begin());
        }

        if (ScrollToBottom && allowedToScroll)
            ImGui::SetScrollHere(1.0f);
        ScrollToBottom = false;
        ImGui::PopStyleVar();
        ImGui::EndChild();
        ImGui::End();
    }
};

#endif //CPP_LIFE_APPLOG_H
