#pragma once

#include "Quater.hpp"
#include <vector>
#include "common/dlog.hpp"
#include "Variable/VariableTable.hpp"
#include <iomanip>
#include <sstream>

class QuaterList : public std::vector<std::pair<size_t, Quater>>
{
private:
    size_t next_quater_id;

    // 辅助函数：格式化输出列
    std::string format_column(const std::string& content, size_t width) const{
        std::stringstream ss;
        ss << std::left << std::setw(width) << content;
        return ss.str();
    }

public:
    QuaterList() : next_quater_id(0) {}

    // size_t add_quater(const std::string &op, const std::string &arg1, const std::string &arg2, const std::string &result) {
    //     Quater quater = Quater(op, arg1, arg2, result);
    //     size_t this_id = next_quater_id++;
    //     this->emplace_back(this_id, std::move(quater));
    //     return this_id;
    // }
    std::string to_string() const
    {
        // 计算每列的最大宽度
        std::vector<size_t> col_widths = {6, 8, 8, 8, 8}; // 初始宽度
        
        // 动态计算实际需要的宽度
        for (const auto& [index, quater] : *this) {
            col_widths[0] = std::max(col_widths[0], std::to_string(index).size());
            col_widths[1] = std::max(col_widths[1], quater.op.size());
            col_widths[2] = std::max(col_widths[2], quater.arg1.size());
            col_widths[3] = std::max(col_widths[3], quater.arg2.size());
            col_widths[4] = std::max(col_widths[4], quater.result.size());
        }

        // 添加表头宽度
        col_widths[0] = std::max(col_widths[0], std::string("Index").size());
        col_widths[1] = std::max(col_widths[1], std::string("Oper").size());
        col_widths[2] = std::max(col_widths[2], std::string("Arg1").size());
        col_widths[3] = std::max(col_widths[3], std::string("Arg2").size());
        col_widths[4] = std::max(col_widths[4], std::string("Result").size());

        // 构建表格字符串
        std::stringstream ss;
        
        // 表头
        ss << format_column("Index", col_widths[0]) << " "
           << format_column("Oper", col_widths[1]) << " "
           << format_column("Arg1", col_widths[2]) << " "
           << format_column("Arg2", col_widths[3]) << " "
           << format_column("Result", col_widths[4]) << "\n";
        
        // 分隔线
        ss << std::string(col_widths[0], '-') << " "
           << std::string(col_widths[1], '-') << " "
           << std::string(col_widths[2], '-') << " "
           << std::string(col_widths[3], '-') << " "
           << std::string(col_widths[4], '-') << "\n";
        
        // 数据行
        for (const auto& [index, quater] : *this) {
            ss << format_column(std::to_string(index), col_widths[0]) << " "
               << format_column(quater.op, col_widths[1]) << " "
               << format_column(quater.arg1, col_widths[2]) << " "
               << format_column(quater.arg2, col_widths[3]) << " "
               << format_column(quater.result, col_widths[4]) << "\n";
        }

        return ss.str();
    }

    size_t add_quater(const std::string &op, const std::string &arg1, const std::string &arg2, const std::string &result,
                      const VariableTable &table)
    {
        Quater quater(op, arg1, arg2, result);

        // 设置变量表中的变量
        auto v_arg1 = table.lookup(arg1);
        auto v_arg2 = table.lookup(arg2);
        auto v_result = table.lookup(result);
        if (v_arg1.has_value())
        {
            quater.meta.set("arg1", v_arg1.value());
        }
        if (v_arg2.has_value())
        {
            quater.meta.set("arg2", v_arg2.value());
        }
        if (v_result.has_value())
        {
            quater.meta.set("result", v_result.value());
        }

        // 设置临时变量
        if (Quater::is_temp(arg1))
        {
            quater.meta.set("arg1", Variable(arg1, VariableType::I32, 0, -1));
        }
        if (Quater::is_temp(arg2))
        {
            quater.meta.set("arg2", Variable(arg2, VariableType::I32, 0, -1));
        }
        if (Quater::is_temp(result))
        {
            quater.meta.set("result", Variable(result, VariableType::I32, 0, -1));
        }

        size_t this_id = next_quater_id++;
        this->emplace_back(this_id, std::move(quater));
        return this_id;
    }

    size_t add_quater(const size_t &base_id, const Quater &quater)
    {
        this->emplace_back(base_id, quater);
        return base_id;
    }

    size_t append_quaters(const QuaterList &quaters)
    {
        size_t base_id = next_quater_id;
        for (const auto &[id, quater] : quaters)
        {
            if (quater.is_jump())
            {
                // 跳转语句不仅id要+base_id, 而且result也要加
                Quater q(quater);

                try
                {
                    int result_int = std::stoi(q.result);
                    q.result = std::to_string(result_int + base_id);
                }
                catch (const std::invalid_argument &e)
                {
                    // If conversion fails, do nothing (q.result is not an integer)
                }

                this->add_quater(id + base_id, q);
                continue;
            }

            this->add_quater(id + base_id, quater);
        }
        next_quater_id += quaters.size();
        return base_id;
    }

    void display() const
    {
        std::cout << to_string() << std::endl;
    }

    void set_bool_exit(const vector<size_t> &list, size_t exit_id, size_t base_id = 0)
    {
        const std::string &exit_id_str = std::to_string(exit_id);
        for (const auto &id : list)
        {
            auto &[_, quater] = this->at(id + base_id);
            quater.result = exit_id_str;
        }
    }
};
