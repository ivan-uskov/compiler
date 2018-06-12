#pragma once

#include <vector>
#include "ast/Visitor.h"

namespace AST
{
    class Node;
    class Mult;
    class Sum;

    using Visitor = BaseVisitor<Mult, Sum>;
    template <typename Derived>
    using Visitable = BaseVisitable<Derived, Visitor, Node>;

    class Node
    {
    public:
        virtual ~Node() = default;
        virtual void accept(Visitor & visitor) const = 0;
    };
}

