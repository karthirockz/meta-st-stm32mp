SUMMARY = "ST STM32MP1 projects for Linux examples"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/COPYING.MIT;md5=3da9cfbcb788c80a0384361b4de20420"

COMPATIBLE_MACHINE = "(stm32mpcommon)"

PACKAGE_ARCH = "${MACHINE_ARCH}"

SRC_URI = "git://github.com/STMicroelectronics/linux-examples;protocol=https;branch=master"
SRCREV  = "bcfba47fb4d9c3756a0b94242e42541c8145f1ab"

PV = "4.19+${SRCPV}"

S = "${WORKDIR}/git"

PACKAGES += "${PN}-userfs"

FILES_${PN}-userfs = "${prefix}/local/Linux-A7-examples"

do_install() {
    # Install scripts and README in userfs:
    # <userfs>/Linux-A7-examples/
    #              |-- utils (ie common scripts)
    #              |-- <IP> (ie UART,DCMI,GPIO ...)
    #                   |-- <EXAMPLE_NAME> (ie Camera preview)
    #                        |-- .sh script
    #                        |-- README.md
    for examples_list in $(ls ${S}); do
        if [ -d ${S}/${examples_list} ]; then
            install -d ${D}${prefix}/local/Linux-A7-examples/${examples_list}/
            cp -rf ${S}/${examples_list}/ ${D}${prefix}/local/Linux-A7-examples/
        fi
    done
}
