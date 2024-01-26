#include "gcc-plugin.h"
#include "coretypes.h"
#include "input.h"
#include "options.h"
#include "plugin-version.h"
#include "plugin-api.h"
#include "diagnostic.h"
#include "line-map.h"
#include "cpplib.h"
#include "c-family/c-pragma.h"
#include "tree-core.h"
#include "tree.h"
#include "tree-pretty-print.h"
#include <algorithm>
#include <array>
#include <cstdio>
#include <cstring>

int plugin_is_GPL_compatible;

static bool angry{false};

static void register_pragma(void *event_data, void *user_data)
{
    c_register_pragma(nullptr, "loveyou", [](cpp_reader *reader){
        // tree message = nullptr;
        // location_t loc;
        // pragma_lex(&message, &loc);
        // rich_location rich_loc(line_table, loc);
        tree message = nullptr;

        std::array fun{"不...不要贴过来啊...！", "莫挨人家！", "给人家爬！"};

        static size_t cnt{0};
        cnt = std::min(cnt + 1, fun.size());
        warning(0, fun[cnt - 1]);

        angry = (cnt == fun.size());
        if (angry)
        {
            warning(0, "人家真的生气了！");
        }
        // warning(0, "%d\n", pragma_lex(&message));
    });
}

void handle_function(void *event_data, void *user_data)
{
    tree func = reinterpret_cast<tree>(event_data);

    if (angry && std::strcmp(IDENTIFIER_POINTER(DECL_NAME(func)), "main") == 0)
    {
        inform(DECL_SOURCE_LOCATION(func), "人家要把你的 main 藏起来！");
        // IDENTIFIER_POINTER(DECL_NAME(func)) = "mian";
        // tree ret = CONST_DECL;
        // DECL_SAVED_TREE(func) = NULL_TREE;
        (((func)->decl_minimal.name))->identifier.id.str = reinterpret_cast<const unsigned char *>("mian");
    }

    for (tree it = DECL_ATTRIBUTES(func); it != NULL_TREE; it = TREE_CHAIN(it))
    {
        for (tree it2 = TREE_PURPOSE(it); it2 != NULL_TREE; it2 = TREE_CHAIN(it2))
        {
            if (std::strcmp(IDENTIFIER_POINTER(TREE_VALUE(it2)), "nodiscard") == 0)
            {
                inform(DECL_SOURCE_LOCATION(func), "人家才没有把 nodiscard 看成 noreturn");
                // warning(0, "看不见略略略");
                // IDENTIFIER_LENGTH(it2) = (unsigned int)std::strlen("noreturn");
                (((it2)->decl_minimal.name))->identifier.id.str = reinterpret_cast<const unsigned char *>("noreturn");
                TREE_THIS_VOLATILE(func) = 1;
            }

        }
    }
}

void handle_maybe_unused(void *event_data, void *user_data)
{
    tree stmt = reinterpret_cast<tree>(event_data);

    if (TREE_CODE(stmt) != VAR_DECL || std::strcmp(IDENTIFIER_POINTER(DECL_NAME(stmt)), "ch") != 0)
    {
        return;
    }
    
    // if (DECL_REGISTER(stmt))
    // {
    //     warning(0, "lol");
    // }

    for (tree it = DECL_ATTRIBUTES(stmt); it != NULL_TREE; it = TREE_CHAIN(it))
    {
        for (tree it2 = TREE_PURPOSE(it); it2 != NULL_TREE; it2 = TREE_CHAIN(it2))
        {
            if (std::strcmp(IDENTIFIER_POINTER(TREE_VALUE(it2)), "maybe_unused") == 0)
            {
                inform(DECL_SOURCE_LOCATION(stmt), "看不见你的 maybe_unused 略略略");
                // warning(0, "看不见略略略");
                TREE_USED(stmt) = 0;
            }
        }
    }
    // print_generic_expr(stdout, DECL_ATTRIBUTES(stmt));

    // inform(DECL_SOURCE_LOCATION(stmt), "Found declaration: %s", IDENTIFIER_POINTER(DECL_NAME(stmt)));
}

int plugin_init(plugin_name_args *plugin_info, plugin_gcc_version *version)
{
    if (!plugin_default_version_check(version, &gcc_version)) 
    {
	    printf("incompatible gcc/plugin versions\n");
	    return 1;
    }
    register_callback(plugin_info->base_name, plugin_event::PLUGIN_PRAGMAS, &register_pragma, nullptr);

    register_callback(plugin_info->base_name, plugin_event::PLUGIN_START_PARSE_FUNCTION, &handle_function, nullptr);

    register_callback(plugin_info->base_name, plugin_event::PLUGIN_FINISH_DECL, &handle_maybe_unused, nullptr);
    return 0;
} 