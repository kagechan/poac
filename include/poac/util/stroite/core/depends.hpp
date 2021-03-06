#ifndef STROITE_CORE_DEPENDS_HPP
#define STROITE_CORE_DEPENDS_HPP

#include <iostream>
#include <string>
#include <vector>
#include <optional>

#include "../utils.hpp"
#include "../../command.hpp"


namespace stroite::core::depends {
    template <typename Opts>
    std::optional<std::string>
    calc(const Opts& opts, const std::string& src_cpp)
    {
        poac::util::command cmd(opts.system);
        cmd += opts.version_prefix + std::to_string(opts.cpp_version);
        for (const auto& isp : opts.include_search_path)
            cmd += "-I" + isp;
        for (const auto& cta : opts.other_args) // TODO: other_argとして，include search pathを指定する可能性がある．
            cmd += cta;
        // Like -M but do not mention header files that are found in system header directories,
        //  nor header files that are included, directly or indirectly, from such a header.
        // This implies that the choice of angle brackets or double quotes in an ‘#include’ directive
        //  does not in itself determine whether that header appears in -MM dependency output.
        // (https://gcc.gnu.org/onlinedocs/gcc/Preprocessor-Options.html#Preprocessor-Options)
        cmd += "-MM " + src_cpp;

        if (opts.verbose)
            std::cout << cmd << std::endl;

        return cmd.exec();
    }

    template <typename Opts>
    std::optional<std::vector<std::string>>
    gen(const Opts& opts, const std::string& src_cpp)
    {
        if (const auto ret = calc(opts, src_cpp)) {
            auto deps_headers = utils::misc::split(*ret, " \n\\");
            deps_headers.erase(deps_headers.begin()); // main.o:
            deps_headers.erase(deps_headers.begin()); // main.cpp
            return deps_headers;
        }
        else {
            return std::nullopt;
        }
    }
} // end namespace
#endif // STROITE_CORE_DEPENDS_HPP
