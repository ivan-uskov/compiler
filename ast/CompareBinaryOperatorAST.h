#pragma once

#include "IAST.h"

namespace AST
{
    class CompareBinaryOperatorAST : public IAST
    {
    public:
        enum class Type
        {
            Less,
            More,
            Equals
        };
        explicit CompareBinaryOperatorAST(std::unique_ptr<IAST> && left, std::unique_ptr<IAST> && right, Type t);
        void accept(IASTVisitor & v) const override;
        void acceptLeft(IASTVisitor & v) const;
        void acceptRight(IASTVisitor & v) const;
        Type getType() const;

        ValueType getResultType() const override;
        ValueType getLeftType() const;
        ValueType getRightType() const;

        static std::string typeToString(Type t);
    private:
        std::unique_ptr<IAST> mLeft;
        std::unique_ptr<IAST> mRight;
        Type mType;
        ValueType mValueType;
    };

    std::ostream & operator << (std::ostream & out, AST::CompareBinaryOperatorAST::Type t);
}