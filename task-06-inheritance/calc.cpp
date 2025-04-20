#include <cctype>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
using namespace std;

class Expression {
public:
    Expression() = default;
    virtual ~Expression()
    {
    }

    Expression(const Expression&) = delete;
    Expression& operator=(const Expression&) = delete;
    Expression(Expression&&) = delete;
    Expression& operator=(Expression&&) = delete;

    virtual void add_operand(shared_ptr<Expression> operand) = 0;
    virtual double eval() const = 0;
};

class Number : public Expression {
public:
    explicit Number(double value) : value(value)
    {
    }

    void add_operand(shared_ptr<Expression>) override
    {
        throw runtime_error("Cannot add operand to a number");
    }

    double eval() const override
    {
        return value;
    }

private:
    double value;
};

class Add : public Expression {
public:
    Add() = default;

    void add_operand(shared_ptr<Expression> operand) override
    {
        operands.push_back(operand);
    }

    double eval() const override
    {
        double sum = 0;
        for (const auto& operand : operands) {
            sum += operand->eval();
        }
        return sum;
    }

private:
    vector<shared_ptr<Expression>> operands;
};

class Sub : public Expression {
public:
    Sub() = default;

    void add_operand(shared_ptr<Expression> operand) override
    {
        operands.push_back(operand);
    }

    double eval() const override
    {
        if (operands.empty())
            return 0;
        double result = operands[0]->eval();
        for (size_t i = 1; i < operands.size(); ++i) {
            result -= operands[i]->eval();
        }
        return result;
    }

private:
    vector<shared_ptr<Expression>> operands;
};

class Mul : public Expression {
public:
    Mul() = default;

    void add_operand(shared_ptr<Expression> operand) override
    {
        operands.push_back(operand);
    }

    double eval() const override
    {
        double product = 1;
        for (const auto& operand : operands) {
            product *= operand->eval();
        }
        return product;
    }

private:
    vector<shared_ptr<Expression>> operands;
};

class Div : public Expression {
public:
    Div() = default;

    void add_operand(shared_ptr<Expression> operand) override
    {
        operands.push_back(operand);
    }

    double eval() const override
    {
        if (operands.empty())
            throw runtime_error("No operands for division");
        double result = operands[0]->eval();
        for (size_t i = 1; i < operands.size(); ++i) {
            const double divisor = operands[i]->eval();
            if (divisor == 0) {
                throw runtime_error("Division by zero!");
            }
            result /= divisor;
        }
        return result;
    }

private:
    vector<shared_ptr<Expression>> operands;
};

class Invalid : public Expression {
public:
    Invalid() = default;

    void add_operand(shared_ptr<Expression>) override
    {
        throw runtime_error("Invalid expression cannot add operands");
    }

    double eval() const override
    {
        throw runtime_error("Invalid expression!");
    }
};

class Parser {
public:
    explicit Parser(const string& str) : stream(str), currentChar(' ')
    {
    }

    shared_ptr<Expression> parse()
    {
        expressions.clear();
        fold_indices.clear();

        while (stream >> ws && (currentChar = stream.peek()) != EOF) {
            if (currentChar == '(') {
                stream.get();
                parseOperator();
            } else if (currentChar == ')') {
                stream.get();
                performFold();
            } else if (
                    isdigit(currentChar) || currentChar == '-'
                    || currentChar == '+') {
                parseNumber();
            } else {
                return make_shared<Invalid>();
            }
        }

        if (expressions.size() == 1) {
            return expressions.back();
        }

        return make_shared<Invalid>();
    }

private:
    stringstream stream;
    char currentChar;
    vector<shared_ptr<Expression>> expressions;
    vector<size_t> fold_indices;

    void parseOperator()
    {
        string op;
        stream >> op;

        shared_ptr<Expression> expression;
        if (op == "+") {
            expression = make_shared<Add>();
        } else if (op == "-") {
            expression = make_shared<Sub>();
        } else if (op == "*") {
            expression = make_shared<Mul>();
        } else if (op == "/") {
            expression = make_shared<Div>();
        } else {
            throw runtime_error("Unknown operator: " + op);
        }

        expressions.push_back(expression);
        fold_indices.push_back(expressions.size() - 1);
    }

    void parseNumber()
    {
        double value;
        stream >> value;
        expressions.push_back(make_shared<Number>(value));
    }

    void performFold()
    {
        if (fold_indices.empty()) {
            throw runtime_error("Mismatched parentheses!");
        }

        const size_t startIndex = fold_indices.back();
        fold_indices.pop_back();

        if (startIndex >= expressions.size()) {
            throw runtime_error("Invalid index for folding!");
        }

        const shared_ptr<Expression> lastExpression
                = expressions[startIndex];

        for (size_t i = startIndex + 1; i < expressions.size(); ++i) {
            lastExpression->add_operand(expressions[i]);
        }

        expressions.erase(
                expressions.begin() + startIndex + 1, expressions.end());
    }
};

int main()
{
    string input;

    cout << "Введите выражение (ввод заканчивается пустой строкой):"
              << '\n';
    string line;
    while (getline(cin, line) && !line.empty()) {
        input += line + "\n";
    }

    Parser parser(input);

    try {
        const shared_ptr<Expression> result = parser.parse();
        cout << "Результат вычисления: " << result->eval() << '\n';
    } catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << '\n';
    }

    return 0;
}
