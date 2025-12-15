idf_component_register(
  SRCS "main.c" "periferies/matrix.c" "periferies/led.c" "periferies/uart.c" 
        "periferies/little_fs.c" "filesystem/filesystem.c" "tasks/tasks.c"
        "helper_functions/helper_functions.c" "functions_loop.c"
        "filesystem/commands/cmd_change.c" "filesystem/commands/cmd_create.c"
        "filesystem/commands/cmd_info.c" "filesystem/commands/cmd_move.c"
        "filesystem/commands/cmd_view.c"
        "benchmark/benchmark.c"
        "periferies/sdcard.c"
        "params/params.c" "params/tree.c"
  INCLUDE_DIRS "."
  REQUIRES esp_rom spiffs fatfs wear_levelling vfs sdmmc driver esp_timer littlefs
)

# Your strict flags…
target_compile_options(${COMPONENT_LIB} PRIVATE
  -Wall -Wextra -Werror
  -Wconversion -Wsign-conversion
  -Wdouble-promotion
  -Wshadow -Wformat=2
  -Wstrict-prototypes -Wmissing-prototypes -Wold-style-definition
  -Wno-undef
  -Wno-error=undef
)

