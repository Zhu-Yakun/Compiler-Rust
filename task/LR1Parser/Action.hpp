#pragma once

struct Action {
    enum class Type {
        SHIFT,
        REDUCE,
        ACCEPT,
        ERROR
    };
    Type type;
    size_t number;          // shift的新状态编号
    Production production;  // reduce使用的产生式
    Action() { type = Type::ERROR, number = -1, production = Production(); };

    Action(const Type &type, const size_t &number, Production production) : type(type), number(number),
                                                                            production(std::move(production)) {}

    [[nodiscard]] std::string to_string() const {
        switch (type) {
            case Type::SHIFT:
                return "S " + std::to_string(number);
            case Type::REDUCE:
                return "R " + production.to_string();
            case Type::ACCEPT:
                return "Accept";
            case Type::ERROR:
                return "x";
        }
        return "";
    }


    friend std::ostream &operator<<(std::ostream &os, Action::Type &_type) {
        return os << static_cast<int>(_type);
    }

    friend std::istream &operator>>(std::istream &is, Action::Type &_type) {
        int intType;
        is >> intType;
        _type = static_cast<Type>(intType);
        return is;
    }


    friend std::ostream &operator<<(std::ostream &os, Action &action) {
        return os << action.type << " " << action.number << " " << action.production;
    }

    friend std::istream &operator>>(std::istream &is, Action &action) {
        int temp;
        is >> temp >> action.number >> action.production;
        action.type = static_cast<Type>(temp);
        return is;
    }
};