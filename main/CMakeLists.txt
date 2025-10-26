idf_component_register(
  SRCS "main.c"
  INCLUDE_DIRS "."
  REQUIRES spiffs fatfs wear_levelling vfs sdmmc driver
)

# Add strict warnings only for *your* component
target_compile_options(${COMPONENT_LIB} PRIVATE
  -Wall -Wextra -Werror
  -Wconversion -Wsign-conversion
  -Wdouble-promotion
  -Wshadow -Wformat=2 -Wundef
  -Wstrict-prototypes -Wmissing-prototypes -Wold-style-definition
)
