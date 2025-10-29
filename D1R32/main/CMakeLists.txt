idf_component_register(
  SRCS "main.c" "periferies/matrix.c" "periferies/led.c" "periferies/uart.c" "periferies/little_fs.c" "functions_loop.c"
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

