#pragma once

template<typename... Types>
class BaseVisitor;

template<typename T>
class BaseVisitor<T>
{
public:
    virtual void visit(const T & visitable) = 0;
};

template<typename T, typename... Types>
class BaseVisitor<T, Types...> : public BaseVisitor<Types...>
{
public:
    using BaseVisitor<Types...>::visit;

    virtual void visit(const T & visitable) = 0;
};

template<typename Derived, typename Visitor, typename Base>
class BaseVisitable : public Base
{
public:
    virtual void accept(Visitor & visitor) const final
    {
        visitor.visit(static_cast<const Derived&>(*this));
    }
};