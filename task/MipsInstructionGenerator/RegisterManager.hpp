#pragma once

#include "Variable/Variable.hpp"
#include "MipsInstructionGenerator/Rvalue.hpp"
#include "MipsInstructionGenerator/Avalue.hpp"
#include "common/dlog.hpp"
#include "MipsInstructionGenerator/CodeList.hpp"
#include <sstream>
#include <stack>

// std::string tReg(const size_t &pos);

std::string tReg(const Avalue &avalue);
// std::string aReg(const Avalue &avalue);

/**
 * 寄存器管理
 *
 * 提供一个变量，
 */
class RegisterManager
{
private:
    CodeList &code_list;

public:
    explicit RegisterManager(const size_t regNums, CodeList &code_list) : stack_pointer(0),
                                                                          REG_NUMS(regNums),
                                                                          code_list(code_list)
    {
        regs.resize(REG_NUMS);
    }

private:
    const size_t REG_NUMS;
    std::vector<Rvalue> regs;                          // 每个寄存器中现在有存了什么变量
    std::unordered_map<std::string, Avalue> variables; // 变量id对应一个变量，

private:
    void expire_old_interval(const size_t &current_point)
    {
        for (auto &reg : regs)
        {
            if (!reg.busy)
                continue;
            if (reg.end_point < current_point)
            {
                reg.busy = false;
            }
        }
    }

    Avalue to_avalue(const Variable &variable, const size_t &start_point, const size_t &end_point)
    {
        Avalue avalue;
        avalue.variable_id = variable.id;
        avalue.load = true;
        avalue.start_point = start_point;
        avalue.end_point = end_point;
        avalue.variable_name = variable.name;
        avalue.param = variable.param;

        return avalue;
    }

    [[nodiscard]] size_t count_busy_register() const
    {
        size_t num = 0;
        for (auto &reg : regs)
        {
            num += reg.busy;
        }
        return num;
    }

    /**
     * 线性扫描算法，选择一个最适合寄存器溢出
     * @return 最适合溢出的寄存器编号
     */
    size_t choose_a_register_to_spill()
    {
        /*
         * 选择结束时间最晚的变量溢出，如果结束时间相等，选择开始时间最早的
         * */
        size_t ret = 0;
        while (!regs[ret].busy)
        {
            ret++;
        }

        for (size_t pos = ret + 1; pos < regs.size(); pos++)
        {
            if (!regs[pos].busy)
                continue;

            auto &reg1 = regs[ret];
            auto &reg2 = regs[pos];

            if ((reg1.end_point < reg2.end_point) ||
                (reg1.end_point == reg2.end_point && reg1.start_point > reg2.start_point))
            {
                ret = pos;
            }
        }
        return ret;
    }

    /**
     * 把寄存器中的变量溢出到内存中
     * @param pos
     */
    void spill_reg_to_memory(const size_t &pos)
    {
        if (pos >= REG_NUMS)
            return;

        // 解除寄存器占用
        regs[pos].busy = false;

        // 修改变量现在的位置
        auto var_id = regs[pos].variable_id;
        auto &var = variables[var_id];
        var.load = false;
        if (var.offset == -1)
        {
            var.offset = new_stack_position();
        }

        std::stringstream ss;
        ss << "sw " << tReg(var) << ", ";
        if (var.offset != 0)
            ss << "-";
        ss << var.offset << "($fp)";
        code_list.emplace_back(ss.str());
    }

    size_t stack_pointer; // default = 0
    size_t new_stack_position()
    {
        size_t ret = stack_pointer;
        stack_pointer += 4; // 应该要根据变量的实际长度而定，现在默认都是int，长度恒为4
        return ret;
    }

    Avalue retrieve_variable(const std::string &var_id, const size_t &current_point)
    {
        auto &var = variables[var_id];
        if (var.load)
        {
            return var;
        }
        expire_old_interval(current_point);
        if (count_busy_register() == REG_NUMS)
        {
            spill();
        }

        for (size_t pos = 0; pos < regs.size(); pos++)
        {
            auto &reg = regs[pos];
            if (reg.busy)
                continue;

            put_variable_in_register(var, pos);
            break;
        }

        // 写入mips代码
        std::stringstream ss;
        ss << "lw " << tReg(var) << ", ";
        if (var.offset != 0)
            ss << "-";
        ss << var.offset << "($fp)";
        code_list.emplace_back(ss.str());

        return var;
    }

public:
    [[nodiscard]] size_t get_stack_pointer() const { return stack_pointer; }

    /**
     * 给一个变量分配一个寄存器
     * @param variable 变量
     * @param start_point 生命周期开始时间
     * @param end_point 生命周期结束时间
     * @return avalue
     */
    Avalue
    alloc(const Variable &variable, const size_t &start_point, const size_t &end_point, const size_t &current_point)
    {
        // 如果已经分配过了
        if (exists_variable(variable.id))
        {
            return retrieve_variable(variable.id, current_point);
        }

        // 如果是前四个形式参数, 返回形参，否则把这个变量当成普通参数分配
        if (UTILS::start_with(variable.name, "param"))
        {
            return to_avalue(variable, start_point, end_point);
        }

        // 清理已经存储变量已经超过生命周期的寄存器
        expire_old_interval(current_point);
        // 如果没有空闲寄存器了，选择一个溢出到内存
        if (count_busy_register() == REG_NUMS)
        {
            spill();
        }

        // 遍历所有寄存器，找到空闲寄存器，分配给这个变量
        for (size_t pos = 0; pos < regs.size(); pos++)
        {
            auto &reg = regs[pos];
            if (reg.busy)
                continue;

            Avalue avalue = to_avalue(variable, start_point, end_point);
            avalue.param = -1;
            put_variable_in_register(avalue, pos);
            return avalue;
        }
        return {};
    }

    /**
     * 同时给两个变量分配寄存器
     * @param variable1 第一个变量
     * @param start_point1 第一个变量的生命周期开始时间
     * @param end_point1 第一个变量的生命周期结束时间
     * @param variable2 第二个变量
     * @param start_point2 第二个变量的生命周期开始时间
     * @param end_point2 第二个变量的生命周期结束时间
     * @param current_point 当前代码点
     * @return 两个变量的Avalue对象对
     */
    std::pair<Avalue, Avalue> allocPair(const Variable &variable1, const size_t &start_point1, const size_t &end_point1,
                                        const Variable &variable2, const size_t &start_point2, const size_t &end_point2,
                                        const size_t &current_point)
    {
        // 清除过期变量
        expire_old_interval(current_point);
        // 溢出变量到内存，直到有2个及以上的空闲寄存器
        while (REG_NUMS - count_busy_register() < 2)
        {
            spill();
        }
        Avalue avalue1 = alloc(variable1, start_point1, end_point1, current_point);
        Avalue avalue2 = alloc(variable2, start_point2, end_point2, current_point);
        return {avalue1, avalue2};
    }

    void spill()
    {
        // 如果现在没有空闲寄存器了，选择一个寄存器中的变量溢出到内存
        size_t i = choose_a_register_to_spill();
        spill_reg_to_memory(i);
    }

    [[nodiscard]] Avalue get_variable(const std::string &var_id)
    {
        return variables[var_id];
    }

    [[nodiscard]] bool exists_variable(const std::string &var_id)
    {
        return variables.find(var_id) != variables.end();
    }

    [[nodiscard]] bool is_load_variable(const std::string &var_id)
    {
        if (!exists_variable(var_id))
            return false;
        return variables[var_id].load;
    }

    void display_register()
    {
        std::cout << "id busy\tvar_id" << std::endl;
        for (size_t i = 0; i < regs.size(); i++)
        {
            auto &reg = regs[i];
            std::cout << i << ": " << reg.busy << '\t' << reg.variable_id << std::endl;
        }
        std::cout << "-----------------------------------" << std::endl;
    }

    std::stack<std::string> reserve_regs;
    void spill_all(const size_t &current_point)
    {
        expire_old_interval(current_point);
        for (auto i = 0; i < regs.size(); i++)
        {
            if (regs[i].busy)
            {
                reserve_regs.push(regs[i].variable_id);
                spill_reg_to_memory(i);
            }
        }
    }

    void reserve_all()
    {
        while (!reserve_regs.empty())
        {
            auto var_id = reserve_regs.top();
            auto &var = variables[var_id];
            reserve_regs.pop();
            put_variable_in_register(var, var.pos);
            var.load = true;

            std::stringstream ss;
            ss << "lw " << tReg(var) << ", ";
            if (var.offset != 0)
                ss << "-";
            ss << var.offset << "($fp)";
            code_list.emplace_back(ss.str());
        }
    }

    void put_variable_in_register(Avalue &var, const size_t &pos)
    {
        auto &reg = regs[pos];

        reg.busy = true;
        reg.start_point = var.start_point;
        reg.end_point = var.end_point;
        reg.variable_id = var.variable_id;

        var.pos = pos;
        variables[var.variable_id] = var;
        var.load = true;
    }
};
