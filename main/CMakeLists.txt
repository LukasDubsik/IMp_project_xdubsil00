idf_component_register(
  SRCS "app_main.c" "fs_test.c"
  INCLUDE_DIRS "."
  REQUIRES spiffs fatfs wear_levelling vfs sdmmc driver
)
