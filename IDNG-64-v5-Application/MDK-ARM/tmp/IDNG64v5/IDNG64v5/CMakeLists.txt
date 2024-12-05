# CMSIS Build CMakeLists generated on 2024-07-09T13:36:28

cmake_minimum_required(VERSION 3.22)

# Target options

set(TARGET IDNG64v5)
set(CPU Cortex-M0+)
set(PRJ_DIR "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/MDK-ARM")
set(OUT_DIR "C:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/MDK-ARM/out/IDNG64v5/IDNG64v5")
set(INT_DIR "C:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/MDK-ARM/tmp/IDNG64v5/IDNG64v5")
set(FPU NO_FPU)
set(BYTE_ORDER Little-endian)
set(OPTIMIZE debug)
set(DEBUG on)
set(CC_FLAGS_GLOBAL "-std=c99 -flto -fno-rtti -funsigned-char -fshort-enums -fshort-wchar -ffunction-sections -w")
set(CXX_FLAGS_GLOBAL "-xc++ -std=c++11 -flto -fno-rtti -funsigned-char -fshort-enums -fshort-wchar -ffunction-sections -w")
set(LD_FLAGS_GLOBAL "--library_type=microlib --map --load_addr_map_info --xref --callgraph --symbols --info sizes --info totals --info unused --info veneers --lto --strict --summary_stderr --info summarysizes")
set(LD_SCRIPT "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/MDK-ARM/IDNG64v5_IDNG64v5.sct")

set(DEFINES
  STM32G070xx
  _RTE_
  USE_HAL_DRIVER
  __MICROLIB
)

set(INC_PATHS
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/Core/Inc"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/Drivers/STM32G0xx_HAL_Driver/Inc"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/Drivers/STM32G0xx_HAL_Driver/Inc/Legacy"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/Drivers/CMSIS/Device/ST/STM32G0xx/Include"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/Drivers/CMSIS/Include"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/MDK-ARM"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/app"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/MDK-ARM/RTE/_IDNG64v5"
  "C:/Users/i.garcia/AppData/Local/Arm/Packs/ARM/CMSIS/5.9.0/CMSIS/Core/Include"
  "C:/Users/i.garcia/AppData/Local/arm/packs/Keil/STM32G0xx_DFP/1.5.0/Drivers/CMSIS/Device/ST/STM32G0xx/Include"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/app/w5100"
)

set(AS_ARM_SRC_FILES
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/MDK-ARM/startup_stm32g070xx.s"
)

set(CC_SRC_FILES
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/Core/Src/main.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/Core/Src/stm32g0xx_hal_msp.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/Core/Src/stm32g0xx_it.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/Core/Src/system_stm32g0xx.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_cortex.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_dma.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_dma_ex.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_exti.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_flash.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_flash_ex.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_gpio.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_i2c.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_i2c_ex.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_iwdg.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_pwr.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_pwr_ex.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_rcc.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_rcc_ex.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_rtc.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_rtc_ex.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_spi.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_spi_ex.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_tim.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_hal_tim_ex.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_ll_dma.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/Drivers/STM32G0xx_HAL_Driver/Src/stm32g0xx_ll_rcc.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/MDK-ARM/board.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/MDK-ARM/tasks.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/app/actualLevel.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/app/botonera.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/app/config.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/app/dADSA.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/app/dALS.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/app/dANI.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/app/dAPSA.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/app/dAQCGL.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/app/dAQDTL.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/app/dASE.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/app/dASRA.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/app/dali.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/app/daliAddressing.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/app/daliCommands.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/app/daliLoop.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/app/daliSensores.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/app/daliTransceiver.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/app/daliUtils.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/app/debug.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/app/embeddedBootloader.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/app/ethFramming.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/app/ethPing.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/app/ethTransceiver.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/app/fallos.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/app/historico.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/app/historicoTest.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/app/iED.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/app/interfaces.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/app/loop.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/app/m25p.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/app/normaLink62386P102.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/app/normaLink62386P202.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/app/normaLink62386P209.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/app/normaLinkCfgInfoPasarela.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/app/normaLinkCmdPasarela.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/app/normaLinkDaliUtils.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/app/normaLinkSensores.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/app/normaLinkSingleLoop.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/app/normalink.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/app/polling.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/app/qSN.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/app/rtcc.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/app/sensorScanning.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/app/sensorStoring.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/app/sensores.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/app/sensoresConfig.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/app/sensoresGrupos/sensoresGrupos.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/app/sensoresGrupos/sensoresGruposDetectoresPresencia.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/app/sensoresGrupos/sensoresGruposPulsadoresAccionBasculante.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/app/sensoresGrupos/sensoresGruposPulsadoresAccionFija.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/app/sensoresGrupos/sensoresGruposPulsadoresAccionRegulacion.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/app/sensoresGrupos/sensoresGruposPulsadoresAccionTemporizada.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/app/sensoresGrupos/sensoresGruposPulsadoresTW.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/app/sensoresGrupos/sensoresGruposReguladoresLuz.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/app/sensors.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/app/spi1.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/app/spi2.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/app/storage.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/app/systick.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/app/temp.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/app/test.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/app/utils.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/app/w5100/md5.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/app/w5100/socket.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/app/w5100/w5100.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/app/wdt.c"
  "c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/app/webserver.c"
)

set(AS_FLAGS_c:/Users/i.garcia/Documents/IDNG-64-v5-Application/IDNG-64-v5-Application/MDK-ARM/startup_stm32g070xx.s "-masm=auto -x assembler")

# Toolchain config map

set(REGISTERED_TOOLCHAIN_ROOT "c:/Users/i.garcia/.vcpkg/artifacts/2139c4c6/compilers.arm.armclang/6.21.0/bin")
set(REGISTERED_TOOLCHAIN_VERSION "6.21.0")
set(TOOLCHAIN_VERSION_MIN "6.21.0")
set(TOOLCHAIN_VERSION_MAX "6.21.0")
include ("C:/Users/i.garcia/.vcpkg/artifacts/2139c4c6/tools.open.cmsis.pack.cmsis.toolbox/2.4.0/etc/AC6.6.16.2.cmake")
include ("C:/Users/i.garcia/.vcpkg/artifacts/2139c4c6/tools.open.cmsis.pack.cmsis.toolbox/2.4.0/etc/CMSIS-Build-Utils.cmake")

# Setup project

project(${TARGET} LANGUAGES AS_ARM C)

cbuild_get_running_toolchain(TOOLCHAIN_ROOT TOOLCHAIN_VERSION C)

# Global Flags

set(CMAKE_AS_ARM_FLAGS "${AS_ARM_CPU} ${AS_ARM_BYTE_ORDER} ${AS_ARM_DEFINES} ${AS_ARM_OPTIONS_FLAGS} ${AS_ARM_FLAGS}")
cbuild_get_system_includes(CC_SYS_INC_PATHS_LIST CC_SYS_INC_PATHS)
set(CMAKE_C_FLAGS "${CC_CPU} ${CC_BYTE_ORDER} ${CC_DEFINES} ${CC_OPTIONS_FLAGS} ${CC_FLAGS} ${CC_FLAGS_GLOBAL} ${CC_SYS_INC_PATHS}")
set(CMAKE_C_LINK_FLAGS "${LD_CPU} ${_LS}\"${LD_SCRIPT}\" ${LD_FLAGS_GLOBAL} ${LD_OPTIONS_FLAGS} ${LD_FLAGS}")

# Local Flags

foreach(SRC ${AS_ARM_SRC_FILES})
  string(REPLACE " " "?" S ${SRC})
  if(DEFINED AS_FLAGS_${S})
    set(AS_FLAGS_LOCAL "${AS_FLAGS_${S}}")
  else()
    set(AS_FLAGS_LOCAL "${AS_FLAGS_GLOBAL}")
  endif()
  set_source_files_properties(${SRC} PROPERTIES COMPILE_FLAGS "${AS_FLAGS_LOCAL}")
  set_source_files_properties(${SRC} PROPERTIES LANGUAGE AS_ARM)
endforeach()

# Compilation Database

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_custom_target(database COMMAND ${CMAKE_COMMAND} -E copy_if_different "${INT_DIR}/compile_commands.json" "${OUT_DIR}")

# Setup Target

add_executable(${TARGET} ${AS_ARM_SRC_FILES} ${CC_SRC_FILES})
set_target_properties(${TARGET} PROPERTIES PREFIX "" SUFFIX ".axf" OUTPUT_NAME "IDNG64v5")
set_target_properties(${TARGET} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${OUT_DIR} LINK_DEPENDS ${LD_SCRIPT})
target_include_directories(${TARGET} PUBLIC ${INC_PATHS})
