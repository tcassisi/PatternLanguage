#include <pl.hpp>
#include <pl/token.hpp>
#include <pl/log_console.hpp>
#include <pl/evaluator.hpp>
#include <pl/patterns/pattern.hpp>

#include <vector>
#include <string>
#include <variant>

#include <fmt/args.h>

namespace pl::libstd::libstd {

    namespace {

        std::string format(const auto &params) {
            auto format = pl::Token::literalToString(params[0], true);
            std::string message;

            fmt::dynamic_format_arg_store<fmt::format_context> formatArgs;

            for (u32 i = 1; i < params.size(); i++) {
                auto &param = params[i];

                std::visit(overloaded {
                                   [&](pl::Pattern *value) {
                                       formatArgs.push_back(value->toString());
                                   },
                                   [&](auto &&value) {
                                       formatArgs.push_back(value);
                                   } },
                           param);
            }

            try {
                return fmt::vformat(format, formatArgs);
            } catch (fmt::format_error &error) {
                pl::LogConsole::abortEvaluation(fmt::format("format error: {}", error.what()));
            }
        }

    }

    void registerFunctions(pl::PatternLanguage &runtime) {
        using FunctionParameterCount = pl::api::FunctionParameterCount;

        pl::api::Namespace nsStd = { "builtin", "std" };
        {
            /* print(format, args...) */
            runtime.addFunction(nsStd, "print", FunctionParameterCount::moreThan(0), [](Evaluator *ctx, auto params) -> std::optional<Token::Literal> {
                ctx->getConsole().log(LogConsole::Level::Info, format(params));

                return std::nullopt;
            });

            /* format(format, args...) */
            runtime.addFunction(nsStd, "format", FunctionParameterCount::moreThan(0), [](Evaluator *, auto params) -> std::optional<Token::Literal> {
                return format(params);
            });

            /* env(name) */
            runtime.addFunction(nsStd, "env", FunctionParameterCount::exactly(1), [](Evaluator *ctx, auto params) -> std::optional<Token::Literal> {
                auto name = Token::literalToString(params[0], false);

                auto env = ctx->getEnvVariable(name);
                if (env)
                    return env;
                else {
                    ctx->getConsole().log(LogConsole::Level::Warning, fmt::format("environment variable '{}' does not exist", name));
                    return "";
                }
            });

            /* pack_size(...) */
            runtime.addFunction(nsStd, "sizeof_pack", FunctionParameterCount::atLeast(0), [](Evaluator *, auto params) -> std::optional<Token::Literal> {
                return u128(params.size());
            });

            /* error(message) */
            runtime.addFunction(nsStd, "error", FunctionParameterCount::exactly(1), [](Evaluator *, auto params) -> std::optional<Token::Literal> {
                LogConsole::abortEvaluation(Token::literalToString(params[0], true));

                return std::nullopt;
            });

            /* warning(message) */
            runtime.addFunction(nsStd, "warning", FunctionParameterCount::exactly(1), [](Evaluator *ctx, auto params) -> std::optional<Token::Literal> {
                ctx->getConsole().log(LogConsole::Level::Warning, Token::literalToString(params[0], true));

                return std::nullopt;
            });
        }


    }

}