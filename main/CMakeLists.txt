idf_component_register(
  SRCS "main.c"
  INCLUDE_DIRS "."
  REQUIRES spiffs fatfs wear_levelling vfs sdmmc driver
)
