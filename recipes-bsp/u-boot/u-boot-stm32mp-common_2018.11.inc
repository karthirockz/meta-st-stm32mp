# Adaptation from u-boot-common_${PV}.inc

HOMEPAGE = "http://www.denx.de/wiki/U-Boot/WebHome"
SECTION = "bootloaders"

LICENSE = "GPLv2+"
LIC_FILES_CHKSUM = "file://Licenses/README;md5=30503fd321432fc713238f582193b78e"

DEPENDS += "dtc-native bc-native"
DEPENDS += "flex-native bison-native"

COMPATIBLE_MACHINE = "(stm32mpcommon)"

SRC_URI = "https://github.com/u-boot/u-boot/archive/v${PV}.tar.gz"
SRC_URI[md5sum] = "7ee14909d5d4d701fd3a6b12aad4d762"
SRC_URI[sha256sum] = "33b5cf99bac91d678ed6708be7b7cbbed36c45eb071e6228f83c25ad3a1de13a"

SRC_URI += " \
    file://0001-ARM-v2018.11-stm32mp-r1-MACHINE.patch \
    file://0002-ARM-v2018.11-stm32mp-r1-BOARD.patch \
    file://0003-ARM-v2018.11-stm32mp-r1-DEVICETREE.patch \
    file://0004-ARM-v2018.11-stm32mp-r1-CONFIG.patch \
    file://0005-ARM-v2018.11-stm32mp-r1-MISC.patch \
    file://0006-ARM-v2018.11-stm32mp-r2-MACHINE.patch \
    file://0007-ARM-v2018.11-stm32mp-r2-DEVICETREE.patch \
    file://0008-ARM-v2018.11-stm32mp-r2-MISC.patch \
    file://0009-ARM-v2018.11-stm32mp-r3-MACHINE.patch \
    file://0010-ARM-v2018.11-stm32mp-r3-BOARD.patch \
    file://0011-ARM-v2018.11-stm32mp-r3-DEVICETREE.patch \
    file://0012-ARM-v2018.11-stm32mp-r3-CONFIG.patch \
    file://0013-ARM-v2018.11-stm32mp-r3-MISC.patch \
    "

U_BOOT_VERSION = "2018.11"
PV = "${U_BOOT_VERSION}"

S = "${WORKDIR}/u-boot-${PV}"

# ---------------------------------
# Configure devupstream class usage
# ---------------------------------
BBCLASSEXTEND = "devupstream:target"

SRC_URI_class-devupstream = "git://github.com/STMicroelectronics/u-boot.git;name=uboot;protocol=https;branch=v2018.11-stm32mp"
SRCREV_class-devupstream = "22eea618b3295cfae777aa80789eb1b46824db12"
SRCREV_FORMAT_class-devupstream = "uboot"
PV_class-devupstream = "${U_BOOT_VERSION}+github+${SRCPV}"

# ---------------------------------
# Configure default preference to manage dynamic selection between tarball and github
# ---------------------------------
STM32MP_SOURCE_SELECTION ?= "tarball"

DEFAULT_PREFERENCE = "${@bb.utils.contains('STM32MP_SOURCE_SELECTION', 'github', '-1', '1', d)}"
