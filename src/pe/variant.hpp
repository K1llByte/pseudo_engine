#pragma once

#include <variant>
#include <functional>
#include <utility>

namespace pe {
    template<class... Ts>
    struct overloaded : Ts... {
        using Ts::operator()...;
    };

    template<typename ...Vars>
    struct Variant: public std::variant<Vars...> {
        // Enable the same ctors of std::variant
        using std::variant<Vars...>::variant;

        // Match fucntion like a switch case for the variants
        template<typename ...Fs>
        void match(Fs&& ...actions) {
            std::visit(
                overloaded {
                    std::forward<Fs>(actions)...
                }
            , *this);
        }

        template<typename T>
        constexpr bool is() const {
            return std::holds_alternative(*this);
        }
    };
}